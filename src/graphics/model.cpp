
#include <pch.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/model.hpp>

static ModelID CreateUniqueModelID()
{
    static std::atomic<uint64> id = 1;
    return ModelID(id++);
}

static void AssertValidModelCtor(const ea::vector<SharedPtr<Material>>& subMaterials, const ea::vector<SharedPtr<Mesh>>& subMeshes)
{
    if constexpr (EnableAsserts)
    {
        ASSERT(subMaterials.size() > 0);
        ASSERT(subMeshes.size() > 0);
        ASSERT(subMaterials.size() == subMeshes.size());

        const GPUShader* shader = subMaterials[0]->GetShader();
        for (auto& subMat : subMaterials)
        {
            ASSERT(subMat->GetShader() == shader);
        }
    }
}

static ea::vector<uint16> FetchIndices(aiMesh& mesh)
{
    ea::vector<uint16> indices;

    for (uint i = 0; i < mesh.mNumFaces; ++i)
    {
        const aiFace& aFace = mesh.mFaces[i];

        for (uint j = 0; j < aFace.mNumIndices; ++j)
        {
            uint index = aFace.mIndices[j];
            ASSERT(index < Uint16Max);

            indices.push_back(uint16(index));
        }
    }

    return indices;
}

static ea::vector<Vertex> FetchVertices(aiMesh& mesh, float scale = 1.f)
{
    ea::vector<Vertex> vertices{};
    vertices.reserve(mesh.mNumVertices);

    const aiVector3D* aTexCoords = mesh.mTextureCoords[0];

    for (uint i = 0; i < mesh.mNumVertices; ++i)
    {
        Vertex vertex{};

        if (mesh.mVertices)
        {
            const aiVector3D& aPos = mesh.mVertices[i];
            vertex.position.x = aPos.x * scale;
            vertex.position.y = aPos.y * scale;
            vertex.position.z = aPos.z * scale;
        }

        if (mesh.mNormals)
        {
            const aiVector3D& aNormal = mesh.mNormals[i];
            vertex.normal.x = aNormal.x;
            vertex.normal.y = aNormal.y;
            vertex.normal.z = aNormal.z;
        }

        if (aTexCoords)
        {
            const aiVector3D& aUv = aTexCoords[i];
            vertex.uv.x = aUv.x;
            vertex.uv.y = aUv.y;
        }

        if (mesh.mTangents)
        {
            const aiVector3D& aTangent = mesh.mTangents[i];
            vertex.tangent.x = aTangent.x;
            vertex.tangent.y = aTangent.y;
            vertex.tangent.z = aTangent.z;
        }

        vertices.push_back(vertex);
    }

    return vertices;
}

static ea::vector<ea::string> GetTexturePaths(const aiMaterial& material, aiTextureType type)
{
    const uint texCount = material.GetTextureCount(type);
    if (texCount == 0) return {};

    ea::vector<ea::string> texPaths{};
    texPaths.resize(texCount);

    for (uint i = 0; i < texCount; ++i)
    {
        aiString path{};
        material.GetTexture(type, i, &path);

        texPaths[i] = ea::string(path.C_Str());
    }

    return texPaths;
}

static const char* ToString(aiTextureType type)
{
    constexpr ea::array Table
    {
        "aiTextureType_NONE",
        "aiTextureType_DIFFUSE",
        "aiTextureType_SPECULAR",
        "aiTextureType_AMBIENT",
        "aiTextureType_EMISSIVE",
        "aiTextureType_HEIGHT",
        "aiTextureType_NORMALS",
        "aiTextureType_SHININESS",
        "aiTextureType_OPACITY",
        "aiTextureType_DISPLACEMENT",
        "aiTextureType_LIGHTMAP",
        "aiTextureType_REFLECTION",
        "aiTextureType_BASE_COLOR",
        "aiTextureType_NORMAL_CAMERA",
        "aiTextureType_EMISSION_COLOR",
        "aiTextureType_METALNESS",
        "aiTextureType_DIFFUSE_ROUGHNESS",
        "aiTextureType_AMBIENT_OCCLUSION",
        "aiTextureType_UNKNOWN",
    };

    return Table[EnumCast(type)];
}

static Json::Value LoadModelMetaFile(const char* folderPath)
{
    namespace fs = std::filesystem;

    fs::path metaPath(folderPath);
    metaPath /= "meta.json";

    std::ifstream file(metaPath);
    if (!file)
    {
        THROW_MSG(fmt::format("Failed to load meta.json file at {}\n", metaPath.u8string()));
    }

    Json::Value root;
    file >> root;

    return root;
}

static const aiScene* LoadImporter(const char* folderPath, const Json::Value& meta, Assimp::Importer& importer)
{
    const std::string modelPath = fmt::format("{}/{}", folderPath, meta["model"].asString());
    const Json::Value& flipUVJson = meta["flip-uv"];

    const bool flipUV = !flipUVJson ? true : flipUVJson.asBool();

    int flags = aiProcess_SplitLargeMeshes | aiProcess_PreTransformVertices | aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;
    if (!flipUV)
    {
        flags &= ~aiProcess_FlipUVs;
    }

    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, Uint16Max);

    const aiScene* pScene = importer.ReadFile(modelPath, flags);

    if (!pScene || (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !pScene->mRootNode)
    {
        THROW_MSG(fmt::format("Error when loading mesh at {}\n{}", modelPath, importer.GetErrorString()));
    }

    fmt::print("Loading Mesh from disk : {}\nSubMeshes ({}), Materials ({})\n", modelPath, pScene->mNumMeshes, pScene->mNumMaterials);

    return pScene;
}

static AABB AABBMerge(const AABB& b1, const AABB& b2)
{
    vfloat4 b1Center = vLoad(&b1.center);
    vfloat4 b1Extents = vLoad(&b1.extents);

    vfloat4 b2Center = vLoad(&b2.center);
    vfloat4 b2Extents = vLoad(&b2.extents);

    vfloat4 Min = v4Sub(b1Center, b1Extents);
    Min = v4Min(Min, v4Sub(b2Center, b2Extents));

    vfloat4 Max = v4Add(b1Center, b1Extents);
    Max = v4Max(Max, v4Add(b2Center, b2Extents));

    assert(v3All(v4LessEqual(Min, Max)));

    AABB out;
    vStore(&out.center, v4Mul(v4Add(Min, Max), v4Spread(0.5f)));
    vStore(&out.extents, v4Mul(v4Sub(Max, Min), v4Spread(0.5f)));

    return out;
}

static AABB ComputeModelBounds(ea::span<const SharedPtr<Mesh>> meshes)
{
    ASSERT(meshes.size() > 0);

    AABB bounds = meshes[0]->GetBounds();

    for (usize i = 1; i < meshes.size(); ++i)
    {
        bounds = AABBMerge(bounds, meshes[i]->GetBounds());
    }

    return bounds;
}

void Gyn::PrintModelInfo(const char* path)
{
    namespace fs = std::filesystem;

    Assimp::Importer import;

    const aiScene* pScene = import.ReadFile(path, 0);

    if (!pScene || (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !pScene->mRootNode)
    {
        THROW_MSG(fmt::format("Error when loading mesh at {}\n{}", path, import.GetErrorString()));
    }

    const aiScene& scene = *pScene;

    constexpr const char* IntroductionLayout = 
        "Model : {}\n"
        "===================================\n"
        "SubMeshes ({}), Materials ({})\n\n";

    fmt::print(IntroductionLayout, path, scene.mNumMeshes, scene.mNumMaterials);

    ea::vector<ea::string> texturePaths{};

    ea::hash_map<ea::string, usize> textureMap;

    for (uint i = 0; i < scene.mNumMaterials; i++)
    {
        aiMaterial& mat = *scene.mMaterials[i];

        fmt::print("Material \"{}\"\n--------\n", mat.GetName().C_Str());

        aiColor3D aiColor(0.f, 0.f, 0.f);
        mat.Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
        const auto color = float3(aiColor.r, aiColor.g, aiColor.b);

        fmt::print("Diffuse color {}\n", color);

        const uint TextureTypeCount = aiTextureType_UNKNOWN + 1;

        for (uint j = 0; j < TextureTypeCount; ++j)
        {
            const aiTextureType texType = aiTextureType(j);

            ea::vector<ea::string> paths = GetTexturePaths(mat, texType);
            if (paths.empty()) continue;

            fmt::print("Textures IDs {} ({}) : ", ::ToString(texType), paths.size());

            for (usize k = 0; k < paths.size(); ++k)
            {
                const ea::string& texPath = paths[k];

                if (!HashMapHasValue(textureMap, texPath))
                {
                    const usize index = texturePaths.size();
                    texturePaths.push_back(texPath);

                    textureMap[texPath] = index;
                }

                ASSERT(HashMapHasValue(textureMap, texPath));

                fmt::print("{}, ", textureMap[texPath]);
            }

            fmt::print("\n");
        }

        fmt::print("\n");
    }

    fmt::print("Textures :\n--------------\n");

    for (const auto& s : texturePaths)
    {
        auto p = fs::path(s.c_str()).filename();
        fmt::print("\"{}\",\n", p.u8string());
    }
    fmt::print("\n");

    fmt::print("SubMeshes material IDs :\n--------------------\n");

    for (uint i = 0; i < scene.mNumMeshes; ++i)
    {
        fmt::print("{}, ", scene.mMeshes[i]->mMaterialIndex);
    }

    fmt::print("\n\n");
}

ModelLoading Gyn::LoadModelData(const char* folderPath)
{
    namespace fs = std::filesystem;
    
    Json::Value meta = LoadModelMetaFile(folderPath);
    
    Assimp::Importer importer;
    const aiScene& scene = *LoadImporter(folderPath, meta, importer);

    Json::Value& jsonTextures = meta["textures"];
    const uint texCount = jsonTextures.size();

    ea::vector<TextureLoading> textures{};
    textures.resize(texCount);

    for (uint i = 0; i < texCount; ++i)
    {
        TextureLoading loading;
        loading.filePath = ToEAString(fmt::format("{}/{}", folderPath, jsonTextures[i].asCString()));

        textures[i] = std::move(loading);
    }

    Json::Value& jsonMaterials = meta["materials"];
    const uint matCount = jsonMaterials.size();
    ASSERT_MSG(matCount == scene.mNumMaterials, "Every material in the model must be described in the meta.json file");

    ea::vector<MaterialLoading> materials{};
    materials.resize(matCount);

    for (uint i = 0; i < matCount; ++i)
    {
        aiMaterial& aiMat = *scene.mMaterials[i];

        aiColor3D aiColor(0.f, 0.f, 0.f);
        aiMat.Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

        aiColor3D aiShininess{};
        aiMat.Get(AI_MATKEY_SHININESS, aiShininess);

        Json::Value& albedoMapJson = jsonMaterials[i]["albedoMap"];
        Json::Value& normalMapJson = jsonMaterials[i]["normalMap"];
        Json::Value& emissiveMapJson = jsonMaterials[i]["emissiveMap"];
        Json::Value& maskMapJson = jsonMaterials[i]["maskMap"];

        MaterialLoading loading;
        loading.albedoMapIndex = !albedoMapJson ? -1 : albedoMapJson.asInt();
        loading.normalMapIndex = !normalMapJson ? -1 : normalMapJson.asInt();
        loading.emissiveMapIndex = !emissiveMapJson ? -1 : emissiveMapJson.asInt();
        loading.maskMapIndex = !maskMapJson ? -1 : maskMapJson.asInt();
        loading.color = float3(aiColor.r, aiColor.g, aiColor.b);
        loading.shininess = aiShininess.r;

        materials[i] = std::move(loading);
    }

    const float scale = meta["scale"].asFloat();

    ea::vector<SubMeshLoading> subMeshes{};
    subMeshes.resize(scene.mNumMeshes);

    for (uint i = 0; i < scene.mNumMeshes; i++)
    {
        aiMesh* mesh = scene.mMeshes[i];

        SubMeshLoading& subMesh = subMeshes[i];
        subMesh.vertices = FetchVertices(*mesh, scale);
        subMesh.indices = FetchIndices(*mesh);
        subMesh.materialIndex = mesh->mMaterialIndex;
    }

    ModelLoading data{};
    data.textures = std::move(textures);
    data.materials = std::move(materials);
    data.subMeshes = std::move(subMeshes);

    return data;
}

Model::Model(ea::vector<SharedPtr<Material>>&& subMaterials, ea::vector<SharedPtr<Mesh>>&& subMeshes)
{
    AssertValidModelCtor(subMaterials, subMeshes);

    const ModelID id = CreateUniqueModelID();
    const AABB bounds = ComputeModelBounds(subMeshes);
    const GPUShader* shader = subMaterials[0]->GetShader();

    m_id = id;
    m_bounds = bounds;
    m_subMaterials = std::move(subMaterials);
    m_subMeshes = std::move(subMeshes);
    m_sharedShader = std::move(shader);
}

bool Model::IsNull() const
{
    return m_id == ModelNullID;
}

ModelID Model::GetID() const
{
    AssertNotNull();
    return m_id;
}

ea::span<const SharedPtr<const Mesh>> Model::GetSubMeshes() const
{
    AssertNotNull();
    return ToSpanConst(ea::span(m_subMeshes));
}

const Mesh& Model::GetSubMesh(uint i) const
{
    AssertNotNull();
    return *m_subMeshes[i];
}

ea::span<const SharedPtr<const Material>> Model::GetSubMaterials() const
{
    AssertNotNull();
    return ToSpanConst(ea::span(m_subMaterials));
}

const Material& Model::GetSubMaterial(uint i) const
{
    AssertNotNull();
    return *m_subMaterials[i];
}

const AABB& Model::GetBounds() const
{
    AssertNotNull();
    return m_bounds;
}

void Model::AssertNotNull() const
{
    ASSERT(!IsNull());
}