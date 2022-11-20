
#include <pch.hpp>
#include <gyn/unity.hpp>
#include <gyn/engine.hpp>
#include <gyn/world.hpp>
#include <gyn/entity.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/vertex.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/point_light.hpp>
#include <gyn/graphics/render_component.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/pvs.hpp>

static float2 LoadVec2FromJson(const Json::Value& json)
{
    return float2(json["x"].asFloat(), json["y"].asFloat());
}
static float3 LoadVec3FromJson(const Json::Value& json)
{
    return float3(json["x"].asFloat(), json["y"].asFloat(), json["z"].asFloat());
}
static float4 LoadVec4FromJson(const Json::Value& json)
{
    return float4(json["x"].asFloat(), json["y"].asFloat(), json["z"].asFloat(), json["w"].asFloat());
}
static Quaternion LoadQuaternionFromJson(const Json::Value& json)
{
    return Quaternion(json["x"].asFloat(), json["y"].asFloat(), json["z"].asFloat(), json["w"].asFloat());
}
static AABB LoadAABBFromJson(const Json::Value& json)
{
    return AABB(LoadVec3FromJson(json["m_Center"]), LoadVec3FromJson(json["m_Extent"]));
}
static ea::vector<uint32> LoadU32ArrayFromJson(const Json::Value& json)
{
    ea::vector<uint32> ret = {};
    ret.reserve(json.size());

    for (auto& j : json)
    {
        ret.push_back(j.asUInt());
    }

    return ret;
}

static ea::vector<EntityID> FetchIDs(ea::span<const Entity> entities, ea::span<const uint32> indices)
{
    ea::vector<EntityID> entityIDs = {};
    entityIDs.resize(indices.size());

    for (usize k = 0; k < indices.size(); ++k)
    {
        entityIDs[k] = entities[indices[k]].GetID();
    }

    return entityIDs;
}

Unity::Mesh LoadLevelModel(const char* path)
{
    namespace fs = std::filesystem;

    std::ifstream file(path);
    if (!file)
    {
        THROW_MSG(fmt::format("Failed to load model level file at {}\n", path));
    }

    Json::Value root = {};
    file >> root;

    auto& positionsJson = root["Positions"];
    auto& normalsJson = root["Normals"];
    auto& uvsJson = root["UVs"];
    auto& tangentsJson = root["Tangents"];
    auto& indicesJson = root["Indices"];

    ea::vector<Vertex> vertices = {};
    vertices.resize(positionsJson.size());

    for (uint i = 0; i < positionsJson.size(); ++i)
    {
        auto& posJson = positionsJson[i];
        auto& normalJson = normalsJson[i];
        auto& uvJson = uvsJson[i];
        auto& tangentJson = tangentsJson[i];

        Vertex v = {};
        v.normal.x = normalJson["x"].asFloat();
        v.normal.y = normalJson["y"].asFloat();
        v.normal.z = normalJson["z"].asFloat();

        v.position.x = posJson["x"].asFloat();
        v.position.y = posJson["y"].asFloat();
        v.position.z = posJson["z"].asFloat();

        v.tangent.x = tangentJson["x"].asFloat();
        v.tangent.y = tangentJson["y"].asFloat();
        v.tangent.z = tangentJson["z"].asFloat();

        v.uv.x = uvJson["x"].asFloat();
        v.uv.y = 1.f - uvJson["y"].asFloat();

        vertices[i] = v;
    }

    ea::vector<uint16> indices = {};
    indices.reserve(indicesJson.size());

    for (auto& indexJson : indicesJson)
    {
        int32 i = indexJson.asInt();
        ASSERT(i <= Uint16Max);

        indices.push_back(uint16(i));
    }

    Unity::Mesh loading = {};
    loading.vertices = std::move(vertices);
    loading.indices = std::move(indices);
    loading.bounds = LoadAABBFromJson(root["Bounds"]);

    return loading;
}

Unity::Scene Unity::LoadScene(const char* folderPath)
{
    namespace fs = std::filesystem;

    const auto path = fmt::format("{}/level.json", folderPath);

    std::ifstream file(path);
    if (!file)
    {
        THROW_MSG(fmt::format("Failed to load level file at {}\n", path));
    }

    Json::Value root = {};
    file >> root;

    Json::Value& modelsJson = root["Models"];

    ea::vector<Unity::Model> models = {};
    models.reserve(modelsJson.size());

    for (auto& modelJson : modelsJson)
    {
        const auto& subMaterialsJson = modelJson["SubMaterials"];
        const auto& subMeshesJson = modelJson["SubMeshes"];

        ea::vector<uint32> subMaterialIDs(subMaterialsJson.size());
        ea::vector<uint32> subMeshIDs(subMeshesJson.size());

        ASSERT(subMaterialIDs.size() == subMeshIDs.size());
        const usize subMeshCount = subMeshIDs.size();

        for (usize i = 0; i < subMeshCount; ++i)
        {
            subMaterialIDs[i] = subMaterialsJson[Json::ArrayIndex(i)].asInt();
            subMeshIDs[i] = subMeshesJson[Json::ArrayIndex(i)].asInt();
        }

        Unity::Model loading = {};
        loading.subMaterialIDs = std::move(subMaterialIDs);
        loading.subMeshIDs = std::move(subMeshIDs);

        models.push_back(std::move(loading));
    }

    Json::Value& texturesJson = root["Textures"];

    ea::vector<Unity::Texture> textures = {};
    textures.reserve(texturesJson.size());

    for (auto& textureJson : texturesJson)
    {
        Unity::Texture loading = {};
        loading.path = textureJson["Path"].asCString();
        loading.type = TextureType(textureJson["Type"].asInt());

        textures.push_back(std::move(loading));
    }

    Json::Value& materialsJson = root["Materials"];

    ea::vector<Unity::Material> materials = {};
    materials.reserve(materialsJson.size());

    for (const auto& materialJson : materialsJson)
    {
        Unity::Material loading = {};
        loading.color = LoadVec4FromJson(materialJson["Color"]);
        loading.emissiveColor = LoadVec3FromJson(materialJson["EmissiveColor"]);
        loading.uvScale = LoadVec2FromJson(materialJson["UVScale"]);
        loading.normalScale = materialJson["NormalScale"].asFloat();
        loading.smoothness = materialJson["Smoothness"].asFloat();
        loading.metallic = materialJson["Metallic"].asFloat();
        loading.albedoMapIndex = materialJson["AlbedoMapIndex"].asInt();
        loading.normalMapIndex = materialJson["NormalMapIndex"].asInt();
        loading.emissiveMapIndex = materialJson["EmissiveMapIndex"].asInt();
        loading.maskMapIndex = materialJson["MaskMapIndex"].asInt();

        materials.push_back(loading);
    }

    Json::Value& entitiesJson = root["Entities"];

    ea::vector<Unity::Mesh> meshes = {};
    ea::vector<bool> meshesLoaded = {};

    ea::vector<Unity::Entity> entities = {};
    entities.reserve(entitiesJson.size());

    for (auto& entityJson : entitiesJson)
    {
        const float3 pos = LoadVec3FromJson(entityJson["Position"]);
        const Quaternion rot = LoadQuaternionFromJson(entityJson["Rotation"]);
        const float3 scale = LoadVec3FromJson(entityJson["Scale"]);
        const AABB worldBounds = LoadAABBFromJson(entityJson["WorldBounds"]);
        const bool castShadows = entityJson["CastShadows"].asBool();

        const uint32 modelID = entityJson["ModelID"].asInt();
        const auto& subMeshIDs = models[modelID].subMeshIDs;

        for (usize i = 0; i < subMeshIDs.size(); ++i)
        {
            const uint32 meshID = subMeshIDs[i];
            if (meshID >= meshes.size() || !meshesLoaded[meshID])
            {
                meshes.resize(meshID + 1);
                meshesLoaded.resize(meshID + 1);

                const auto meshPath = fmt::format("{}/meshes/{}.json", folderPath, meshID);

                meshes[meshID] = LoadLevelModel(meshPath.c_str());
                meshesLoaded[meshID] = true;
            }
        }

        Unity::Entity entity = {};
        entity.position = pos;
        entity.rotation = rot;
        entity.scale = scale;
        entity.modelID = modelID;
        entity.worldBounds = worldBounds;
        entity.castShadows = castShadows;

        entities.push_back(entity);
    }

    Json::Value& pointLightsJson = root["PointLights"];

    ea::vector<Unity::PointLight> pointLights = {};
    pointLights.reserve(pointLightsJson.size());

    for (auto& pointLightJson : pointLightsJson)
    {
        const float3 pos = LoadVec3FromJson(pointLightJson["Position"]);
        const float3 color = LoadVec3FromJson(pointLightJson["Color"]);
        const int bakeType = pointLightJson["BakeType"].asInt();
        const int shadowType = pointLightJson["Shadows"].asInt();
        const float intensity = pointLightJson["Intensity"].asFloat();
        const float range = pointLightJson["Range"].asFloat();

        PointLight light = {};
        light.position = pos;
        light.color = color;
        light.bakeType = LightBakeType(bakeType);
        light.shadowType = ShadowType(shadowType);
        light.intensity = intensity;
        light.range = range;

        pointLights.emplace_back(light);
    }

    Json::Value& pvsArrayJson = root["PVSs"];

    ea::vector<Unity::PVS> pvsArray = {};
    pvsArray.reserve(pvsArrayJson.size());

    for (auto& pvsJson : pvsArrayJson)
    {
        PVS pvs{};
        pvs.bounds = LoadAABBFromJson(pvsJson["Bounds"]);
        pvs.pointLightIndices = LoadU32ArrayFromJson(pvsJson["PointLightIndices"]);
        pvs.renderersIndices = LoadU32ArrayFromJson(pvsJson["RenderersIndices"]);
        pvs.visiblePVSIndices = LoadU32ArrayFromJson(pvsJson["VisiblePVSIndices"]);

        pvsArray.emplace_back(std::move(pvs));
    }

    if constexpr (EnableAsserts)
    {
        ASSERT(meshesLoaded.size() == meshes.size());
        for (bool isLoaded : meshesLoaded)
        {
            ASSERT(isLoaded);
        }
    }

    Unity::Scene loading = {};
    loading.meshes = std::move(meshes);
    loading.textures = std::move(textures);
    loading.materials = std::move(materials);
    loading.models = std::move(models);
    loading.entities = std::move(entities);
    loading.pointLights = std::move(pointLights);
    loading.pvsArray = std::move(pvsArray);

    return loading;
}

static SharedPtr<Image2D> LoadTexture(RenderAssetManager& renderAssets, const Unity::Texture& loading)
{
    ASSERT(loading.type != Unity::TextureType::Null);
    ASSERT(!loading.path.empty());

    return renderAssets.LoadTexture(loading.path.c_str());
}

SceneStats Gyn::SetupWorldFromUnityScene(World& world, const char* path)
{
    auto& entityManager = world.entityManager;
    auto& renderAssets = *world.engine->renderAssets;
    auto& matMeshRefCounts = *world.engine->matMeshRefCounts;
        
    const Unity::Scene levelLoad = Unity::LoadScene(path);

    ea::vector<SharedPtr<Mesh>> meshes = {};
    meshes.resize(levelLoad.meshes.size());

    for (usize i = 0; i < levelLoad.meshes.size(); ++i)
    {
        const Unity::Mesh& load = levelLoad.meshes[i];
        meshes[i] = renderAssets.CreateMesh(load.vertices, load.indices, load.bounds);
    }

    ea::vector<SharedPtr<Image2D>> textures = {};
    textures.resize(levelLoad.textures.size());

    for (usize i = 0; i < levelLoad.textures.size(); ++i)
    {
        textures[i] = LoadTexture(renderAssets, levelLoad.textures[i]);
    }

    ea::vector<SharedPtr<Material>> materials = {};
    materials.resize(levelLoad.materials.size());

    for (usize i = 0; i < levelLoad.materials.size(); ++i)
    {
        const Unity::Material& load = levelLoad.materials[i];

        SharedPtr<Image2D> albedoMap = load.albedoMapIndex == -1 ? renderAssets.GetWhiteTexture() : textures[load.albedoMapIndex];
        SharedPtr<Image2D> normalMap = load.normalMapIndex == -1 ? renderAssets.GetNormalTexture() : textures[load.normalMapIndex];
        SharedPtr<Image2D> emissionMap = load.emissiveMapIndex == -1 ? renderAssets.GetBlackTexture() : textures[load.emissiveMapIndex];
        SharedPtr<Image2D> maskMap = load.maskMapIndex == -1 ? renderAssets.GetDefaultMaskMap() : textures[load.maskMapIndex];

        MaterialLitCreateInfo info = {};
        info.albedoMap = std::move(albedoMap);
        info.normalMap = std::move(normalMap);
        info.emissiveMap = std::move(emissionMap);
        info.maskMap = std::move(maskMap);
        info.uniforms.color = load.color;
        info.uniforms.emissiveColor = float4(load.emissiveColor, 1.0f);
        info.uniforms.uvScale = load.uvScale;
        info.uniforms.metallic = std::clamp(load.metallic, 0.f, 1.f);
        info.uniforms.roughness = powf(1.0f - std::clamp(load.smoothness, 0.f, 1.f), 1.7f);
        info.uniforms.normal = std::clamp(load.normalScale, 0.f, 1.f);
        info.uniforms.ao = 1.0f;

        materials[i] = renderAssets.CreateLitMaterial(std::move(info));
    }

    ea::vector<SharedPtr<Model>> models = {};
    models.resize(levelLoad.models.size());

    for (usize i = 0; i < levelLoad.models.size(); ++i)
    {
        const Unity::Model& load = levelLoad.models[i];
        const usize subMeshCount = load.subMeshIDs.size();
        ASSERT(subMeshCount == load.subMaterialIDs.size());

        ea::vector<SharedPtr<Material>> subMaterials = {};
        subMaterials.resize(subMeshCount);

        ea::vector<SharedPtr<Mesh>> subMeshes = {};
        subMeshes.resize(subMeshCount);

        for (usize j = 0; j < subMeshCount; ++j)
        {
            subMaterials[j] = materials[load.subMaterialIDs[j]];
            subMeshes[j] = meshes[load.subMeshIDs[j]];
        }

        models[i] = renderAssets.CreateModel(std::move(subMaterials), std::move(subMeshes));
    }

    // Simple way to instantiate the scene many times
    constexpr uint32 SceneCountX = 1;
    constexpr uint32 SceneCountY = 1;
    constexpr uint32 SceneCountZ = 1;
    constexpr uint32 TotalSceneCount = SceneCountX * SceneCountY * SceneCountZ;

    ea::vector<Entity> renderers = {};
    renderers.resize(levelLoad.entities.size() * TotalSceneCount);

    uint32 k = 0;
    for (uint32 y = 0; y < SceneCountY; ++y)
    {
        for (uint32 z = 0; z < SceneCountZ; ++z)
        {
            for (uint32 x = 0; x < SceneCountX; ++x)
            {
                float3 offset = float3(x * 30.0f, y * 20.0f, z * 50.0f);

                for (usize i = 0; i < levelLoad.entities.size(); ++i)
                {
                    const Unity::Entity& load = levelLoad.entities[i];
                    Entity entity = CreateRenderEntity(entityManager, matMeshRefCounts, models[load.modelID], load.castShadows, load.position + offset, load.rotation, load.scale);
                    renderers[k] = entity;

                    ++k;
                }
            }
        }
    }

    ea::vector<Entity> pointLights = {};
    pointLights.resize(levelLoad.pointLights.size());

    for (usize i = 0; i < levelLoad.pointLights.size(); ++i)
    {
        const Unity::PointLight& unityLight = levelLoad.pointLights[i];

        PointLight light = {};
        light.color = Vec3Pow(unityLight.color, 1.7f);
        light.intensity = unityLight.intensity * 50.0f;
        light.range = unityLight.range;
        light.shadowNear = 0.1f;
        light.shadowFar = unityLight.range;

        pointLights[i] = CreateLightEntity(entityManager, unityLight.position, light);
    }

    ea::vector<Entity> pvsEntities = {};
    pvsEntities.resize(levelLoad.pvsArray.size());

    for (usize i = 0; i < levelLoad.pvsArray.size(); ++i)
    {
        pvsEntities[i] = entityManager.CreateEntity(GetPVSComponentTypes());
    }

    for (usize i = 0; i < levelLoad.pvsArray.size(); ++i)
    {
        const Unity::PVS& load = levelLoad.pvsArray[i];

        Entity& entity = pvsEntities[i];

        PVSBounds bounds = {};
        bounds.val = load.bounds;
        bounds.val.center = bounds.val.center;

        EntityIDComponent idComponent = {};
        idComponent.val = entity.GetID();

        auto dynamicData = ea::make_unique<PVSDynamicData>();
        dynamicData->rendererIDs = FetchIDs(renderers, load.renderersIndices);
        dynamicData->pointLightIDs = FetchIDs(pointLights, load.pointLightIndices);
        dynamicData->visiblePVSIDs = FetchIDs(pvsEntities, load.visiblePVSIndices);

        auto cache = ea::make_unique<PVSDataCache>();

        entity.SetComponent(idComponent);
        entity.SetComponent(bounds);
        entity.SetAbstractComponent(std::move(dynamicData));
        entity.SetAbstractComponent(std::move(cache));
    }

    for (usize i = 0; i < levelLoad.pvsArray.size(); ++i)
    {
        Entity& entity = pvsEntities[i];

        PVSDynamicData& dynamicData = *entity.GetMutableAbstractComponent<PVSDynamicData>();
        dynamicData.combinedRendererIDs = GetCombinedPVSVisibleRenderers(entityManager, dynamicData);
        dynamicData.combinedPointLightIDs = GetCombinedPVSVisiblePointLights(entityManager, dynamicData);
        dynamicData.combinedRendererCount = GetCombinedPVSVisibleRendererCount(dynamicData.combinedRendererIDs);
    }

    SceneStats stats = {};
    stats.texture2DCount = ToU32(levelLoad.textures.size());
    stats.materialCount = ToU32(levelLoad.materials.size());
    stats.meshCount = ToU32(levelLoad.meshes.size());
    stats.modelCount = ToU32(levelLoad.models.size());
    stats.rendererCount = ToU32(renderers.size());
    stats.pointLightCount = ToU32(levelLoad.pointLights.size());
    stats.pvsCount = ToU32(levelLoad.pvsArray.size());

    return stats;
}

TransformRecord Gyn::LoadRecord(const char* path)
{
    namespace fs = std::filesystem;

    std::ifstream file(path);
    if (!file)
    {
        THROW_MSG(fmt::format("Failed to load record file at {}\n", path));
    }

    Json::Value root = {};
    file >> root;

    const float sampleFrequency = root["sampleFrequency"].asFloat();
    const auto& samplesJson = root["samples"];

    ea::vector<float3> positions = {};
    positions.reserve(samplesJson.size());

    ea::vector<Quaternion> rotations = {};
    rotations.reserve(samplesJson.size());

    for (auto& sampleJson : samplesJson)
    {
        float3 position = LoadVec3FromJson(sampleJson["position"]);
        Quaternion rotation = LoadQuaternionFromJson(sampleJson["rotation"]);

        positions.emplace_back(position);
        rotations.emplace_back(rotation);
    }

    TransformRecord record = {};
    record.sampleFrequency = sampleFrequency;
    record.positions = std::move(positions);
    record.rotations = std::move(rotations);

    return record;
}