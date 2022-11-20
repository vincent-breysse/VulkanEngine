
#include <pch.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/image2d.hpp>
#include <gyn/graphics/image_cube.hpp>
#include <gyn/graphics/image_cube_array.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/model.hpp>
#include <gyn/graphics/vertex.hpp>

static const AABB LargeBounds = AABB(float3(0, 0, 0), float3(1, 1, 1) * 1e6f);

static UniqueGPURenderPassPtr CreateGBufferPass(RenderAssetManager& renderAssets)
{
    const ea::array formats
    {
        GPUFormat::eDepth24S8, // Depth
        GPUFormat::eRGBA8, // Albedo
        GPUFormat::eSNorm8x4, // Normals
        GPUFormat::eRGBA8, // Emissive
        GPUFormat::eRGBA8, // MaskMap (Metallic, AO, Details, Smoothness)
        GPUFormat::eUint16, // Material HeapIndex
    };

    ea::array<SubpassLayout, 1> subpassLayouts = {};

    // G-Buffer pass
    subpassLayouts[0].depthStencilRef = 0;
    subpassLayouts[0].colorBufferRefs.assign({ 1, 2, 3, 4, 5 });

    return renderAssets.CreateRenderPass(formats, subpassLayouts);
}

static UniqueGPURenderPassPtr CreateSunShadowsPass(RenderAssetManager& renderAssets)
{
    GPUFormat format = GPUFormat::eDepth24S8;

    SubpassLayout subpassLayout = {};
    subpassLayout.depthStencilRef = 0;

    return renderAssets.CreateRenderPass(ea::span(&format, 1), ea::span(&subpassLayout, 1));
}

static UniqueGPURenderPassPtr CreateTileCullingPass(RenderAssetManager& renderAssets)
{
    const ea::array formats
    {
        GPUFormat::eUint16, // Light count
    };

    ea::array<SubpassLayout, 1> subpassLayouts = {};

    // SSAO pas
    subpassLayouts[0].depthStencilRef = NullSubpassRef;
    subpassLayouts[0].colorBufferRefs.assign({ 0 });

    return renderAssets.CreateRenderPass(formats, subpassLayouts);
}

static UniqueGPURenderPassPtr CreateSSAOPass(RenderAssetManager& renderAssets)
{
    const ea::array formats
    {
        GPUFormat::eUNorm8x1, // SSAO
    };

    ea::array<SubpassLayout, 1> subpassLayouts = {};

    // SSAO pas
    subpassLayouts[0].depthStencilRef = NullSubpassRef;
    subpassLayouts[0].colorBufferRefs.assign({ 0 });

    return renderAssets.CreateRenderPass(formats, subpassLayouts);
}

static UniqueGPURenderPassPtr CreateBlurPass(RenderAssetManager& renderAssets)
{
    const ea::array formats
    {
        GPUFormat::eRGBA8, // Blurred output
    };

    ea::array<SubpassLayout, 1> subpassLayouts = {};

    subpassLayouts[0].depthStencilRef = NullSubpassRef;
    subpassLayouts[0].colorBufferRefs.assign({ 0 });

    return renderAssets.CreateRenderPass(formats, subpassLayouts);
}

static UniqueGPURenderPassPtr CreateLightingPass(RenderAssetManager& renderAssets)
{
    const ea::array formats
    {
        GPUFormat::eRGBA8, // Lighting pass output
    };

    ea::array<SubpassLayout, 1> subpassLayouts = {};

    // Lighting pass
    subpassLayouts[0].depthStencilRef = NullSubpassRef;
    subpassLayouts[0].colorBufferRefs.assign({ 0 });

    return renderAssets.CreateRenderPass(formats, subpassLayouts);
}

static UniqueGPURenderPassPtr CreatePresentPass(RenderAssetManager& renderAssets)
{
    const ea::array formats
    {
        GPUFormat::ePresent, // Lighting pass output
    };

    ea::array<SubpassLayout, 1> subpassLayouts = {};

    // Lighting pass
    subpassLayouts[0].depthStencilRef = NullSubpassRef;
    subpassLayouts[0].colorBufferRefs.assign({ 0 });

    return renderAssets.CreateRenderPass(formats, subpassLayouts);
}

static SharedPtr<Material> CreateSolidColorMaterial(RenderAssetManager& renderAssets,
    const GPUShader* gbufferShader,
    float3 color,
    float emissiveStrength,
    bool isUnlit,
    SharedPtr<const Image2D> defaultAlbedo,
    SharedPtr<const Image2D> defaultNormal,
    SharedPtr<const Image2D> defaultEmissive,
    SharedPtr<const Image2D> defaultMaskMap)
{
    GPU& gpu = renderAssets.GetGPU();

    LitMaterialUniforms uniforms{};
    uniforms.color = float4(color, 1);
    uniforms.emissiveColor = float4(emissiveStrength * color, 1);
    uniforms.uvScale = float2(1, 1);
    uniforms.roughness = 1.0f;
    uniforms.metallic = 0.0f;
    uniforms.ao = 1.0f;
    uniforms.normal = 1.0f;
    uniforms.isUnlit = isUnlit ? 1 : 0;

    SharedPtr<Material> material = renderAssets.CreateMaterial<LitMaterialUniforms>(gbufferShader);
    material->SetUniformData(uniforms);

    uint32 heapIndex = renderAssets.GetHeapIndex(material->GetID());

    UniqueGPUBufferPtr buffer = gpu.CreateBuffer(sizeof(uint32), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);
    buffer->Upload(AsByteSpan(heapIndex));

    material->SetBinding(buffer.get(), 0);
    material->SetBinding(defaultAlbedo->GetView(GPUImageComponent::eColor), 1);
    material->SetBinding(defaultNormal->GetView(GPUImageComponent::eColor), 2);
    material->SetBinding(defaultEmissive->GetView(GPUImageComponent::eColor), 3);
    material->SetBinding(defaultMaskMap->GetView(GPUImageComponent::eColor), 4);

    material->AddOwnedBuffer(std::move(buffer));
    material->AddOwnedImage(std::move(defaultAlbedo));
    material->AddOwnedImage(std::move(defaultNormal));
    material->AddOwnedImage(std::move(defaultEmissive));
    material->AddOwnedImage(std::move(defaultMaskMap));

    return material;
}

static SharedPtr<Image2D> CreateDefaultNormalTexture(RenderAssetManager& renderAssets)
{
    sf::Image image{};
    image.create(1, 1, sf::Color(128, 128, 255, 255));

    return renderAssets.CreateTexture2D(image);
}

static SharedPtr<Image2D> CreateDefaultMaskMap(RenderAssetManager& renderAssets)
{
    sf::Image image{};
    image.create(1, 1, sf::Color(0, 0, 0, 0));

    return renderAssets.CreateTexture2D(image);
}

static SharedPtr<Image2D> CreateBlackTexture(RenderAssetManager& renderAssets)
{
    sf::Image image{};
    image.create(1, 1, sf::Color(0, 0, 0, 255));

    return renderAssets.CreateTexture2D(image);
}

static SharedPtr<Image2D> CreateWhiteTexture(RenderAssetManager& renderAssets)
{
    sf::Image image{};
    image.create(1, 1, sf::Color(255, 255, 255, 255));

    return renderAssets.CreateTexture2D(image);
}

static SharedPtr<Image2D> CreateSSAONoiseMap(RenderAssetManager& renderAssets)
{
    std::array<sf::Color, SSAONoiseSize* SSAONoiseSize> noise{};

    std::default_random_engine generator;
    std::uniform_real_distribution<float> random(0.0, 1.0);

    for (size_t i = 0; i < noise.size(); i++)
    {
        float3 n
        {
            random(generator),
            random(generator),
            0.0f
        };

        n = Vec3Normalize(n);

        noise[i] = ToSfColorOpaque(n);
    }

    sf::Image image{};
    image.create(SSAONoiseSize, SSAONoiseSize, &noise[0].r);

    return renderAssets.CreateTexture2D(image);
}

RenderAssetManager::RenderAssetManager(GPU& gpu)
{
    m_gpu = &gpu;

    m_screenMesh = CreateMesh(gpu.GetScreenQuadVertices(), gpu.GetScreenQuadIndices(), LargeBounds);
    m_cubeMesh = CreateMesh(CubeVertices, CubeIndices, AABB(float3(0,0,0), Vec3One));
    m_gbufferPass = CreateGBufferPass(*this);
    m_sunShadowsPass = CreateSunShadowsPass(*this);
    m_tileCullingPass = CreateTileCullingPass(*this);
    m_ssaoPass = CreateSSAOPass(*this);
    m_lightingPass = CreateLightingPass(*this);
    m_presentPass = CreatePresentPass(*this);
    m_blurPass = CreateBlurPass(*this);

    ShaderOptions shaderOptions = {};
    shaderOptions.backFaceCulling = true;

    m_gbufferShader = m_gpu->LoadShaderFromFile("shader/gbuffer-lit.glsl", *m_gbufferPass, 0, shaderOptions);
    m_sunShadowShader = m_gpu->LoadShaderFromFile("shader/sun-shadows.glsl", *m_sunShadowsPass, 0, shaderOptions);
    m_tileCullingProcess = m_gpu->LoadShaderFromFile("shader/tile-culling.glsl", *m_tileCullingPass, 0, shaderOptions);
    m_ssaoProcess = m_gpu->LoadShaderFromFile("shader/ssao.glsl", *m_ssaoPass, 0, shaderOptions);
    m_blurProcess = m_gpu->LoadShaderFromFile("shader/blur.glsl", *m_blurPass, 0, shaderOptions);
    m_deferredLighting = m_gpu->LoadShaderFromFile("shader/lighting.glsl", *m_lightingPass, 0, shaderOptions);
    m_presentShader = m_gpu->LoadShaderFromFile("shader/present.glsl", *m_presentPass, 0, shaderOptions);

    m_normalTexture = CreateDefaultNormalTexture(*this);
    m_whiteTexture = CreateWhiteTexture(*this);
    m_blackTexture = CreateBlackTexture(*this);
    m_defaultMaskMap = CreateDefaultMaskMap(*this);

    m_ssaoNoise = CreateSSAONoiseMap(*this);

    m_whiteMaterial = CreateSolidColorMaterial(*this, m_gbufferShader.get(), float3(1,1,1), 0.0f, false, m_whiteTexture, m_normalTexture, m_whiteTexture, m_whiteTexture);
    m_whiteCube = CreateModel(m_whiteMaterial, m_cubeMesh);
    m_sunShadowMaterial = CreateMaterial<MaterialNoUniforms>(m_sunShadowShader.get());
}

bool RenderAssetManager::IsNull() const
{
    return m_gpu == nullptr;
}

GPU& RenderAssetManager::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

SharedPtr<const Mesh> RenderAssetManager::GetScreenMesh() const
{
    AssertNotNull();
    return m_screenMesh;
}

const GPUShader* RenderAssetManager::GetGBufferShader() const
{
    AssertNotNull();
    return m_gbufferShader.get();
}

const GPUShader* RenderAssetManager::GetSunShadowsShader() const
{
    AssertNotNull();
    return m_sunShadowShader.get();
}

const GPUShader* RenderAssetManager::GetTileCullingShader() const
{
    AssertNotNull();
    return m_tileCullingProcess.get();
}

const GPUShader* RenderAssetManager::GetSSAOShader() const
{
    AssertNotNull();
    return m_ssaoProcess.get();
}

const GPUShader* RenderAssetManager::GetBlurShader() const
{
    AssertNotNull();
    return m_blurProcess.get();
}

const GPUShader* RenderAssetManager::GetDeferredLightingShader() const
{
    AssertNotNull();
    return m_deferredLighting.get();
}

const GPUShader* RenderAssetManager::GetPresentShader() const
{
    AssertNotNull();
    return m_presentShader.get();
}

SharedPtr<Image2D> RenderAssetManager::GetBlackTexture() const
{
    AssertNotNull();
    return m_blackTexture;
}

SharedPtr<Image2D> RenderAssetManager::GetDefaultMaskMap() const
{
    AssertNotNull();
    return m_defaultMaskMap;
}

SharedPtr<Image2D> RenderAssetManager::GetNormalTexture() const
{
    AssertNotNull();
    return m_normalTexture;
}

SharedPtr<Image2D> RenderAssetManager::GetWhiteTexture() const
{
    AssertNotNull();
    return m_whiteTexture;
}

SharedPtr<Image2D> RenderAssetManager::GetSSAONoiseTexture() const
{
    AssertNotNull();
    return m_ssaoNoise;
}

SharedPtr<Material> RenderAssetManager::GetWhiteMaterial() const
{
    AssertNotNull();
    return m_whiteMaterial;
}

SharedPtr<Mesh> RenderAssetManager::GetCubeMesh() const
{
    AssertNotNull();
    return m_cubeMesh;
}

SharedPtr<const Model> RenderAssetManager::GetWhiteCube() const
{
    AssertNotNull();
    return m_whiteCube;
}

const GPURenderPass* RenderAssetManager::GetGBufferPass() const
{
    AssertNotNull();
    return m_gbufferPass.get();
}

const GPURenderPass* RenderAssetManager::GetSunShadowsPass() const
{
    AssertNotNull();
    return m_sunShadowsPass.get();
}

const GPURenderPass* RenderAssetManager::GetTileCullingPass() const
{
    AssertNotNull();
    return m_tileCullingPass.get();
}

const GPURenderPass* RenderAssetManager::GetSSAOPass() const
{
    AssertNotNull();
    return m_ssaoPass.get();
}

const GPURenderPass* RenderAssetManager::GetBlurPass() const
{
    AssertNotNull();
    return m_blurPass.get();
}

const GPURenderPass* RenderAssetManager::GetLightingPass() const
{
    AssertNotNull();
    return m_lightingPass.get();
}

const GPURenderPass* RenderAssetManager::GetPresentPass() const
{
    AssertNotNull();
    return m_presentPass.get();
}

UniqueGPURenderPassPtr RenderAssetManager::CreateRenderPass(ea::span<const GPUFormat> formats, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks)
{
    AssertNotNull();

    UniqueGPURenderPassPtr layout = GetGPU().CreateRenderPass(formats, subpassLayouts, multiviewMasks);
    RegisterGPURenderPass(layout.get());

    return layout;
}

UniqueGPURenderPassPtr RenderAssetManager::CreateRenderPass(ea::span<const GPUFormat> colorFormats, bool hasDepthStencil)
{
    AssertNotNull();

    UniqueGPURenderPassPtr layout = GetGPU().CreateRenderPass(colorFormats, hasDepthStencil);
    RegisterGPURenderPass(layout.get());

    return layout;
}

SharedPtr<const Material> RenderAssetManager::GetSunShadowsMaterial() const
{
    AssertNotNull();
    return m_sunShadowMaterial;
}

SharedPtr<Image2D> RenderAssetManager::LoadTexture(const char* path)
{
    sf::Image image{};
    image.loadFromFile(path);

    return CreateTexture2D(image);
}

void RenderAssetManager::RegisterMaterial(SharedPtr<Material> material)
{
    AssertNotNull();
    ASSERT(material);

    ASSERT_MSG(!HashMapHasValue(m_materiaDatas, material->GetID()), 
        "Material already registered");

    const GPUShader& shader = *material->GetShader();
    const GPURenderPassID renderPassID = shader.GetRenderPass().GetID();
    const uint32 subpassIndex = shader.GetSubpass();

    SubpassData& subpass = GetSubpassDataMutable(renderPassID, subpassIndex);

    MaterialData data{};
    data.subpassLocalIndex = ToU32(subpass.materials.size());
    data.heapIndex = m_materialHeapSize;

    m_materiaDatas[material->GetID()] = data;

    subpass.materials.emplace_back(std::move(material));
    ++subpass.version;
    ++m_materialHeapSize;
}

SharedPtr<Material> RenderAssetManager::CreateMaterial(const GPUShader* shader, TypeID uniformStructTypeID, uint64 uniformStructSizeByte)
{
    AssertNotNull();

    SharedPtr<Material> material = ::CreateMaterial(*m_gpu, shader, uniformStructTypeID, uniformStructSizeByte);
    RegisterMaterial(material);

    return material;
}

SharedPtr<Material> RenderAssetManager::CreateLitMaterial(MaterialLitCreateInfo&& info)
{
    AssertNotNull();

    SharedPtr<Material> material = CreateMaterial<LitMaterialUniforms>(m_gbufferShader.get());
    material->SetUniformData(info.uniforms);

    UniqueGPUBufferPtr buffer = m_gpu->CreateBuffer(sizeof(uint32), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);
    uint32 heapIndex = GetHeapIndex(material->GetID());
    buffer->Upload(AsByteSpan(heapIndex));

    SharedPtr<Image2D> albedoMap = info.albedoMap ? std::move(info.albedoMap) : m_whiteTexture;
    SharedPtr<Image2D> normalMap = info.normalMap ? std::move(info.normalMap) : m_normalTexture;
    SharedPtr<Image2D> emissiveMap = info.emissiveMap ? std::move(info.emissiveMap) : m_whiteTexture;
    SharedPtr<Image2D> maskMap = info.maskMap ? std::move(info.maskMap) : m_blackTexture;

    material->SetBinding(buffer.get(), 0);
    material->SetBinding(albedoMap->GetView(GPUImageComponent::eColor), 1);
    material->SetBinding(normalMap->GetView(GPUImageComponent::eColor), 2);
    material->SetBinding(emissiveMap->GetView(GPUImageComponent::eColor), 3);
    material->SetBinding(maskMap->GetView(GPUImageComponent::eColor), 4);

    material->AddOwnedBuffer(std::move(buffer));
    material->AddOwnedImage(std::move(albedoMap));
    material->AddOwnedImage(std::move(normalMap));
    material->AddOwnedImage(std::move(emissiveMap));
    material->AddOwnedImage(std::move(maskMap));

    return material;
}

SharedPtr<Material> RenderAssetManager::CreateUnlitMaterial(float3 color, float emissiveStrength)
{
    AssertNotNull();
    return CreateSolidColorMaterial(*this, m_gbufferShader.get(), color, emissiveStrength, true, m_whiteTexture, m_normalTexture, m_whiteTexture, m_whiteTexture);
}

SharedPtr<Mesh> RenderAssetManager::CreateMesh(ea::span<const Vertex> vertices, ea::span<const uint16> indices, const AABB& bounds)
{
    AssertNotNull();

    UniqueGPUBufferPtr vertexBuffer = m_gpu->CreateBuffer(ToByteSpan(vertices), GPUBufferUsage::eVertexBuffer, GPUMemoryType::eGPULocal);
    UniqueGPUBufferPtr index16Buffer = m_gpu->CreateBuffer(ToByteSpan(indices), GPUBufferUsage::eIndexBuffer, GPUMemoryType::eGPULocal);

    SharedPtr<Mesh> mesh = CreateMeshSharedPtr(*m_gpu, std::move(vertexBuffer), std::move(index16Buffer), bounds);
    const uint32 meshIndex = uint32(m_meshes.size());
    m_meshes.emplace_back(mesh);
    m_meshIndices[mesh->GetID()] = meshIndex;

    return mesh;
}

SharedPtr<Model> RenderAssetManager::CreateModel(ea::vector<SharedPtr<Material>>&& materials, ea::vector<SharedPtr<Mesh>>&& meshes)
{
    AssertNotNull();

    ASSERT(materials.size() == meshes.size());
    const usize count = materials.size();

    ea::vector<MatMeshLocalIndex> localIndices{};
    localIndices.resize(count);

    for (usize i = 0; i < count; ++i)
    {
        MatMeshLocalIndex localIndex{};
        localIndex.material = GetSubpassLocalMaterialIndex(materials[i]->GetID());
        localIndex.mesh = GetMeshIndex(meshes[i]->GetID());

        localIndices[i] = localIndex;
    }

    ModelData modelData{};
    modelData.localIndices = std::move(localIndices);
    modelData.localIndicesVersion = 1;

    SharedPtr<Model> model = ea::make_shared<Model>(std::move(materials), std::move(meshes));
    const ModelID id = model->GetID();

    ASSERT(!HashMapHasValue(m_modelDatas, id));
    m_modelDatas[id] = std::move(modelData);

    return model;
}

SharedPtr<Model> RenderAssetManager::CreateModel(SharedPtr<Material> material, SharedPtr<Mesh> mesh)
{
    AssertNotNull();
    return CreateModel(ea::vector{ material }, ea::vector{ mesh });
}

SharedPtr<Model> RenderAssetManager::LoadModel(const char* path)
{
    AssertNotNull();

    ModelLoading loadData = ::LoadModelData(path);
    const usize textureCount = loadData.textures.size();
    const usize materialCount = loadData.materials.size();
    const usize subMeshCount = loadData.subMeshes.size();

    ea::vector<SharedPtr<Image2D>> textures;
    textures.resize(textureCount);

    for (usize i = 0; i < textureCount; ++i)
    {
        const auto& filePath = loadData.textures[i].filePath;
        if (!filePath.empty())
        {
            textures[i] = LoadTexture(filePath.c_str());
        }
    }

    ea::vector<SharedPtr<Material>> materials;
    materials.resize(materialCount);

    for (usize i = 0; i < materialCount; ++i)
    {
        MaterialLoading& materialLoad = loadData.materials[i];
        const auto albedoIndex = materialLoad.albedoMapIndex;
        const auto normalIndex = materialLoad.normalMapIndex;
        const auto emissiveIndex = materialLoad.emissiveMapIndex;
        const auto maskIndex = materialLoad.maskMapIndex;

        SharedPtr<Image2D> albedoMap = albedoIndex == -1 ? m_whiteTexture : textures[albedoIndex];
        SharedPtr<Image2D> normalMap = normalIndex == -1 ? m_normalTexture : textures[normalIndex];
        SharedPtr<Image2D> emissiveMap = emissiveIndex == -1 ? m_blackTexture : textures[emissiveIndex];
        SharedPtr<Image2D> maskMap = maskIndex == -1 ? m_defaultMaskMap  : textures[maskIndex];

        LitMaterialUniforms uniforms{};
        uniforms.color = float4(materialLoad.color, 1.0f);
        uniforms.emissiveColor = emissiveIndex == -1 ? Vec4Zero : Vec4One;
        uniforms.uvScale = float2(1,1);
        uniforms.roughness = 1.0f;
        uniforms.metallic = maskIndex == -1 ? 0.0f : 1.0f;
        uniforms.ao = 1.0f;
        uniforms.normal = 1.0f;

        SharedPtr<Material> material = CreateMaterial<LitMaterialUniforms>(m_gbufferShader.get());
        material->SetUniformData(uniforms);

        uint32 heapIndex = GetHeapIndex(material->GetID());
        UniqueGPUBufferPtr buffer = m_gpu->CreateBuffer(sizeof(uint32), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);
        buffer->Upload(AsByteSpan(heapIndex));

        material->SetBinding(buffer.get(), 0);
        material->SetBinding(albedoMap->GetView(GPUImageComponent::eColor), 1);
        material->SetBinding(normalMap->GetView(GPUImageComponent::eColor), 2);
        material->SetBinding(emissiveMap->GetView(GPUImageComponent::eColor), 3);
        material->SetBinding(maskMap->GetView(GPUImageComponent::eColor), 4);

        material->AddOwnedBuffer(std::move(buffer));
        material->AddOwnedImage(std::move(albedoMap));
        material->AddOwnedImage(std::move(normalMap));
        material->AddOwnedImage(std::move(emissiveMap));
        material->AddOwnedImage(std::move(maskMap));

        materials[i] = std::move(material);
    }

    ea::vector<SharedPtr<Material>> subMaterials;
    subMaterials.resize(subMeshCount);

    ea::vector<SharedPtr<Mesh>> subMeshes;
    subMeshes.resize(subMeshCount);

    for (usize i = 0; i < subMeshCount; ++i)
    {
        SubMeshLoading& subMesh = loadData.subMeshes[i];

        subMeshes[i] = CreateMesh(subMesh.vertices, subMesh.indices, LargeBounds);
        subMaterials[i] = materials[subMesh.materialIndex];
    }

    return CreateModel(std::move(subMaterials), std::move(subMeshes));
}

ea::span<const MatMeshLocalIndex> RenderAssetManager::GetModelLocalIndices(ModelID id) const
{
    AssertNotNull();
    return m_modelDatas.at(id).localIndices;
}

uint32 RenderAssetManager::GetModelLocalIndicesVersion(ModelID id) const
{
    AssertNotNull();
    return m_modelDatas.at(id).localIndicesVersion;
}

ea::span<const SharedPtr<Material>> RenderAssetManager::GetMaterialBatch(GPURenderPassID id, uint32 subpass) const
{
    AssertNotNull();
    return GetSubpassData(id, subpass).materials;
}

uint32 RenderAssetManager::GetMaterialBatchSize(GPURenderPassID id, uint32 subpass) const
{
    AssertNotNull();
    return uint32(GetMaterialBatch(id, subpass).size());
}

uint32 RenderAssetManager::GetSubpassDataVersion(GPURenderPassID id, uint32 subpass) const
{
    AssertNotNull();
    return GetSubpassData(id, subpass).version;
}

ea::span<const SharedPtr<Mesh>> RenderAssetManager::GetMeshBatch() const
{
    AssertNotNull();
    return m_meshes;
}

uint32 RenderAssetManager::GetMeshBatchSize() const
{
    AssertNotNull();
    return uint32(GetMeshBatch().size());
}

uint32 RenderAssetManager::GetInstanceBatchCount(GPURenderPassID id, uint32 subpass) const
{
    AssertNotNull();
    return GetMaterialBatchSize(id, subpass) * GetMeshBatchSize();
}

ea::span<const GPURenderPass *const> RenderAssetManager::GetGPURenderPasses() const
{
    AssertNotNull();
    return m_renderPasses;
}

uint32 RenderAssetManager::GetMeshIndex(MeshID id) const
{
    AssertNotNull();
    ASSERT_MSG(HashMapHasValue(m_meshIndices, id), "Unknown MeshID");

    return m_meshIndices.at(id);
}

uint32 RenderAssetManager::GetSubpassLocalMaterialIndex(MaterialID id) const
{
    AssertNotNull();
    ASSERT_MSG(HashMapHasValue(m_materiaDatas, id), "Unknown MaterialID");

    return m_materiaDatas.at(id).subpassLocalIndex;
}

uint32 RenderAssetManager::GetHeapIndex(MaterialID id) const
{
    AssertNotNull();
    ASSERT_MSG(HashMapHasValue(m_materiaDatas, id), "Unknown MaterialID");

    return m_materiaDatas.at(id).heapIndex;
}

SharedPtr<Image2D> RenderAssetManager::CreateTexture2D(const sf::Image& sfImage)
{
    AssertNotNull();
    return ea::make_shared<Image2D>(*m_gpu, sfImage);
}

SharedPtr<ImageCube> RenderAssetManager::CreateImageCube(uint32 size, GPUFormat format, GPUImageUsageFlags usageFlags)
{
    AssertNotNull();
    return ea::make_shared<ImageCube>(*m_gpu, size, format, usageFlags);
}

SharedPtr<ImageCubeArray> RenderAssetManager::CreateImageCubeArray(uint32 size, GPUFormat format, uint32 cubeCount, GPUImageUsageFlags usageFlags)
{
    AssertNotNull();
    return ea::make_shared<ImageCubeArray>(*m_gpu, size, format, cubeCount, usageFlags);
}

SharedPtr<Image2D> RenderAssetManager::CreateImage2D(uint32 width, uint32 height, GPUFormat format, GPUImageUsageFlags usageFlags)
{
    AssertNotNull();
    ASSERT(width > 0 && height > 0);

    return ea::make_shared<Image2D>(*m_gpu, width, height, format, usageFlags);
}

void RenderAssetManager::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void RenderAssetManager::RegisterGPURenderPass(const GPURenderPass* renderPass)
{
    const GPURenderPassID id = renderPass->GetID();
    ASSERT(!HashMapHasValue(m_rpassDatas, id));

    RenderPassData& rpassData = m_rpassDatas[id];
    rpassData.subpasses.resize(renderPass->GetSubpasses().size());
    for (SubpassData& subpass : rpassData.subpasses)
    {
        subpass.version = 1;
    }

    m_renderPasses.emplace_back(renderPass);
}

auto RenderAssetManager::GetSubpassDataMutable(GPURenderPassID id, uint32 subpass) -> SubpassData&
{
    AssertNotNull();

    return m_rpassDatas.at(id).subpasses[subpass];
}

auto RenderAssetManager::GetSubpassData(GPURenderPassID id, uint32 subpass) const -> const SubpassData&
{
    AssertNotNull();

    return m_rpassDatas.at(id).subpasses[subpass];
}