
#include <pch.hpp>
#include <gyn/gpu.hpp>
#include <gyn/engine.hpp>
#include <gyn/job_scheduler.hpp>
#include <gyn/entity_archetype.hpp>
#include <gyn/entity_chunk.hpp>
#include <gyn/entity_manager.hpp>
#include <gyn/world.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/graphics_engine.hpp>
#include <gyn/graphics/point_light.hpp>
#include <gyn/graphics/image_cube.hpp>
#include <gyn/graphics/image_cube_array.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/model.hpp>
#include <gyn/graphics/image2d.hpp>
#include <gyn/graphics/mat_mesh_query.hpp>
#include <gyn/graphics/render_component.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/mat_mesh_ref_count.hpp>
#include <gyn/graphics/pvs.hpp>
#include <gyn/graphics/culling.hpp>

constexpr uint32 MaxMaterialCount = Uint16Max;
constexpr uint32 MaxEntityCount = 262'144;
constexpr uint32 BloomResolutionDiv = 4;
constexpr uint32 SSAOResolutionDiv = 2;
constexpr uint32 MaxLightCountPerTile = 64;
constexpr uint32 TileCullingDiv = 10;

ea::span<const TypeID> Gyn::GetCameraComponentTypes()
{
    static const ea::array ComponentTypes
    {
        GetTypeID<Position>(),
        GetTypeID<Rotation>(),
        GetTypeID<Scale>(),
        GetTypeID<LocalToWorld>(),

        GetTypeID<PerspectiveProjection>(),
        GetTypeID<WorldToView>(),
        GetTypeID<ViewToClip>(),
        GetTypeID<WorldToClip>(),

        GetTypeID<Resolution>(),

        GetTypeID<PVSEntityID>(),
    };

    return ComponentTypes;
}

static ea::vector<RenderEntityChunk> CreateRenderEntityChunks(const EntityManager& entityManager, uint32& outTotalEntityCount)
{
    const ea::array ComponentTypes =
    {
        GetTypeID<LocalToWorld>(),
        GetTypeID<EntityGPUBufferIndex>(),
        GetTypeID<EntityModelID>(),
        GetTypeID<WorldRenderBounds>(),
    };

    ea::vector<const EntityChunk*> ecsChunks = entityManager.QueryEntityChunks(ComponentTypes);

    ea::vector<RenderEntityChunk> chunks = {};
    chunks.resize(ecsChunks.size());

    uint32 totalEntityCount = 0;

    for (usize i = 0; i < chunks.size(); ++i)
    {
        const EntityChunk* ecsChunk = ecsChunks[i];
        const LocalToWorld* localToWorldLane = ecsChunk->GetComponentLane<LocalToWorld>();
        const WorldRenderBounds* worldAABBLane = ecsChunk->GetComponentLane<WorldRenderBounds>();
        const EntityGPUBufferIndex* gpuIndexLane = ecsChunk->GetComponentLane<EntityGPUBufferIndex>();
        const EntityModelID* modelIDLane = ecsChunk->GetComponentLane<EntityModelID>();
        const DisableShadow* disableShadows = ecsChunk->GetComponentLane<DisableShadow>();

        EntityArchetypeID archetypeID = ecsChunk->GetArchetype().GetID();
        ComponentLaneVersion localToWorldsVersion = ecsChunk->GetLaneVersion<LocalToWorld>();

        uint32 size = ecsChunk->GetEntityCount();

        RenderEntityChunk& chunk = chunks[i];
        chunk.localToWorlds = ReinterpretCast<Matrix4x4>(localToWorldLane);
        chunk.worldAABBs = ReinterpretCast<AABB>(worldAABBLane);
        chunk.gpuIndices = ReinterpretCast<uint32>(gpuIndexLane);
        chunk.modelIDs = ReinterpretCast<ModelID>(modelIDLane);
        chunk.disableShadows = ReinterpretCast<bool>(disableShadows);
        chunk.archetypeID = archetypeID;
        chunk.localToWorldsVersion = localToWorldsVersion;
        chunk.size = size;

        totalEntityCount += size;
    }

    outTotalEntityCount = totalEntityCount;
    return chunks;
}

static ea::vector<PointLightChunk> CreatePointLightChunks(const EntityManager& entityManager, uint32& outTotalLightCount)
{
    const ea::array ComponentTypes =
    {
        GetTypeID<PointLight>(),
        GetTypeID<Position>(),
        GetTypeID<PVSEntityID>(),
    };

    ea::vector<const EntityChunk*> ecsChunks = entityManager.QueryEntityChunks(ComponentTypes);

    ea::vector<PointLightChunk> chunks = {};
    chunks.resize(ecsChunks.size());

    uint32 totalLightCount = 0;

    for (usize i = 0; i < chunks.size(); ++i)
    {
        const EntityChunk* ecsChunk = ecsChunks[i];
        const PointLight* lightLane = ecsChunk->GetComponentLane<PointLight>();
        const Position* positionLane = ecsChunk->GetComponentLane<Position>();
        const PVSEntityID* pvsIDs = ecsChunk->GetComponentLane<PVSEntityID>();

        uint32 size = ecsChunk->GetEntityCount();

        PointLightChunk& chunk = chunks[i];
        chunk.lights = ReinterpretCast<PointLight>(lightLane);
        chunk.positions = ReinterpretCast<float3>(positionLane);
        chunk.pvsIDs = ReinterpretCast<EntityID>(pvsIDs);
        chunk.size = size;

        totalLightCount += size;
    }

    outTotalLightCount = totalLightCount;
    return chunks;
}

static ea::vector<const PVSDataCache*> CreatePointLightPVSCaches(const EntityManager& entityManager, ea::span<const PointLightChunk> chunks, uint32 totalLightCount)
{
    ea::vector<const PVSDataCache*> pvsCaches = {};
    pvsCaches.resize(totalLightCount);

    usize i = 0;
    for (usize r = 0; r < chunks.size(); ++r)
    {
        const PointLightChunk& chunk = chunks[r];

        for (uint32 k = 0; k < chunk.size; ++k, ++i)
        {
            EntityID pvsID = chunk.pvsIDs[k];
            pvsCaches[i] = pvsID != EntityNullID ? entityManager.GetAbstractComponent<PVSDataCache>(pvsID) : nullptr;
        }
    }

    return pvsCaches;
}

static Matrix4x4 ComputeSunShadowsViewMatrix(float3 direction)
{
    float3 upVector = {};
    if (Vec3AllNearEqual(direction, float3(0, 1, 0), float3(1, 1, 1) * 0.01f))
    {
        upVector = float3(0, 0, 1);
    }
    else
    {
        upVector = float3(0, 1, 0);
    }

    return Mat4LookTo(-direction * 1000.0f, direction, upVector);
}

static Matrix4x4 ComputeSunShadowsProjMatrix()
{
    return Mat4Orthographic(50, 50, 1.0f, 2 * 1000.0f);
}

static RenderContext CreateRenderContext(World& world)
{
    auto& engine = *world.engine;
    auto& entityManager = world.entityManager;
    auto& global = world.global;
    auto& pointLightCommon = world.pointLightCommon;
    auto& renderAssets = *engine.renderAssets;
    auto& refCountManager = *engine.matMeshRefCounts;

    uint32 totalEntityCount = {};
    ea::vector<RenderEntityChunk> entityChunks = CreateRenderEntityChunks(entityManager, totalEntityCount);

    uint32 totalPointLightCount = {};
    ea::vector<PointLightChunk> pointLightChunks = CreatePointLightChunks(entityManager, totalPointLightCount);
    ea::vector<const PVSDataCache*> pointLightPVSCaches = CreatePointLightPVSCaches(entityManager, pointLightChunks, totalPointLightCount);

    Entity cameraEntity = entityManager.GetTheEntity(GetCameraComponentTypes());
    float3 cameraPosition = cameraEntity.GetComponent<Position>().val;
    Matrix4x4 cameraWorldToView = cameraEntity.GetComponent<WorldToView>().val;
    Matrix4x4 cameraViewToClip = cameraEntity.GetComponent<ViewToClip>().val;
    Matrix4x4 cameraWorldToClip = cameraEntity.GetComponent<WorldToClip>().val;
    uint2 cameraResolution = cameraEntity.GetComponent<Resolution>().val;
    ea::array<float4, 6> cameraFrustrumPlanes = ExtractCullingPlanes(cameraWorldToClip);
    FrustrumPlanesSOA cameraFrustrumPlanesSOA = BuildFrustrumPlanesSOA(cameraFrustrumPlanes);
    EntityID cameraPVSEntityID = cameraEntity.GetComponent<PVSEntityID>().val;
    const PVSDataCache* cameraPVSDataCache = cameraPVSEntityID != EntityNullID ? entityManager.GetAbstractComponent<PVSDataCache>(cameraPVSEntityID) : nullptr;
    float3 sunLightDirection = Vec3Rotate(float3(0, 0, 1), QuatRotationRollPitchYawFromVector(global.sunLightEulerAngles * DegToRadFactor));

    RenderContext context = {};
    context.global = &global;
    context.renderAssets = &renderAssets;
    context.refCountManager = &refCountManager;
    context.totalEntityCount = totalEntityCount;
    context.entityChunks = std::move(entityChunks);

    context.sunLightDirection = sunLightDirection;
    context.sunShadowsViewMatrix = ComputeSunShadowsViewMatrix(sunLightDirection);
    context.sunShadowsProjMatrix = ComputeSunShadowsProjMatrix();

    context.pointLightCommon = &pointLightCommon;
    context.totalPointLightCount = totalPointLightCount;
    context.pointLightChunks = std::move(pointLightChunks);
    context.pointLightPVSCaches = std::move(pointLightPVSCaches);

    context.cameraPosition = cameraPosition;
    context.cameraWorldToView = cameraWorldToView;
    context.cameraViewToClip = cameraViewToClip;
    context.cameraWorldToClip = cameraWorldToClip;
    context.cameraFrustrumPlanes = cameraFrustrumPlanes;
    context.cameraFrustrumPlanesSOA = cameraFrustrumPlanesSOA;
    context.cameraPVSDataCache = cameraPVSDataCache;
    context.cameraResolution = cameraResolution;

    return context;
}

static GPUCommandBuffer CreatePostProcessCommandBuffer(RenderAssetManager& renderAssets, const GPUShader* shader, const GPUBindSet* bindSet3)
{
    ASSERT(shader);
    ASSERT(bindSet3);

    const Mesh& screenMesh = *renderAssets.GetScreenMesh();

    ea::vector<const GPUShader*> shaders;
    shaders.push_back(shader);

    ea::vector<const GPUBindSet*> bindSets3;
    bindSets3.push_back(bindSet3);

    ea::vector<const GPUBuffer*> vertexBuffers;
    vertexBuffers.push_back(&screenMesh.GetVertexBuffer());

    ea::vector<const GPUBuffer*> indexBuffers;
    indexBuffers.push_back(&screenMesh.GetIndex16Buffer());

    ea::vector<uint32> firstInstances;
    firstInstances.push_back(0);

    ea::vector<uint32> instanceCounts;
    instanceCounts.push_back(1);

    GPUCommandBuffer cmdBuffer = {};
    cmdBuffer.shaders = std::move(shaders);
    cmdBuffer.bindSets3 = std::move(bindSets3);
    cmdBuffer.vertexBuffers = std::move(vertexBuffers);
    cmdBuffer.indexBuffers = std::move(indexBuffers);
    cmdBuffer.firstInstances = std::move(firstInstances);
    cmdBuffer.instanceCounts = std::move(instanceCounts);

    return cmdBuffer;
}

static GPUCommandBuffer CreatePostProcessCommandBuffer(RenderAssetManager& renderAssets, const Material& material)
{
    return CreatePostProcessCommandBuffer(renderAssets, material.GetShader(), material.GetBindSet());
}

static UniqueGPUFramebufferPtr CreateShadowFramebuffer(GPU& gpu, ImageCubeArray& shadowMapArray, const GPURenderPass* shadowPass, uint32 cubeIndex)
{
    ea::vector<const GPUImageView*> attachements{};
    attachements.emplace_back(shadowMapArray.GetCubeView(cubeIndex, GPUImageComponent::eDepthStencil));

    return gpu.CreateFramebuffer(shadowPass, std::move(attachements));
}

static GBuffer CreateGBuffer(RenderAssetManager& renderAssets, uint32 width, uint32 height)
{
    GBuffer gbuffer = {};
    gbuffer.color = renderAssets.CreateImage2D(width, height, GPUFormat::eRGBA8, GPUImageUsageFlags::eRenderAndSample);
    gbuffer.emissive = renderAssets.CreateImage2D(width, height, GPUFormat::eRGBA8, GPUImageUsageFlags::eRenderAndSample);
    gbuffer.mask = renderAssets.CreateImage2D(width, height, GPUFormat::eRGBA8, GPUImageUsageFlags::eRenderAndSample);
    gbuffer.normal = renderAssets.CreateImage2D(width, height, GPUFormat::eSNorm8x4, GPUImageUsageFlags::eRenderAndSample);
    gbuffer.heapIndex = renderAssets.CreateImage2D(width, height, GPUFormat::eUint16, GPUImageUsageFlags::eRenderAndSample);
    gbuffer.depthStencil = renderAssets.CreateImage2D(width, height, GPUFormat::eDepth24S8, GPUImageUsageFlags::eRenderAndSample);

    return gbuffer;
}

static UniqueGPUFramebufferPtr CreateGBufferFramebuffer(GPU& gpu, const GPURenderPass* renderPass, const GBuffer& gbuffer)
{
    ea::vector<const GPUImageView*> attachments
    {
        gbuffer.depthStencil->GetView(GPUImageComponent::eDepthStencil),
        gbuffer.color->GetView(GPUImageComponent::eColor),
        gbuffer.normal->GetView(GPUImageComponent::eColor),
        gbuffer.emissive->GetView(GPUImageComponent::eColor),
        gbuffer.mask->GetView(GPUImageComponent::eColor),
        gbuffer.heapIndex->GetView(GPUImageComponent::eColor),
    };

    return gpu.CreateFramebuffer(renderPass, std::move(attachments));
}

static UniqueGPUFramebufferPtr CreateFramebufferFromColorBuffer(GPU& gpu, const GPURenderPass* renderPass, const Image2D& colorBuffer)
{
    ea::vector<const GPUImageView*> attachments
    {
        colorBuffer.GetView(GPUImageComponent::eColor)
    };

    return gpu.CreateFramebuffer(renderPass, std::move(attachments));
}

static UniqueGPUFramebufferPtr CreateFramebufferFromDepthBuffer(GPU& gpu, const GPURenderPass* renderPass, const Image2D& depthBuffer)
{
    ea::vector<const GPUImageView*> attachments
    {
        depthBuffer.GetView(GPUImageComponent::eDepth)
    };

    return gpu.CreateFramebuffer(renderPass, std::move(attachments));
}

InstanceDrawingLayout CreateInstanceDrawingLayout(ea::span<const uint32> maxInstanceCounts)
{
    const uint64 count = maxInstanceCounts.size();
    if (count == 0) return {};

    ea::vector<uint32> firstInstances{};
    firstInstances.resize(count);

    for (uint64 i = 1; i < count; ++i)
    {
        firstInstances[i] = firstInstances[i - 1] + maxInstanceCounts[i - 1];
    }

    const uint32 maxInstanceCount = firstInstances.back() + maxInstanceCounts.back();

    InstanceDrawingLayout layout{};
    layout.firstInstances = std::move(firstInstances);
    layout.maxInstanceCount = maxInstanceCount;

    return layout;
}

static GPUCommandBuffer CreateCommandBuffer(const MatMeshQuery& matMeshes, ea::vector<uint32>&& instanceCounts, ea::vector<uint32>&& firstInstances)
{
    ea::span<const SharedPtr<const Material>> materials = matMeshes.GetMaterialBatch();
    ea::span<const SharedPtr<const Mesh>> meshes = matMeshes.GetMeshBatch();
    const uint32 matCount = matMeshes.GetMaterialBatchSize();
    const uint32 meshCount = matMeshes.GetMeshBatchSize();

    ea::vector<const GPUShader*> shaders(matCount);
    ea::vector<const GPUBindSet*> bindSets(matCount);

    ea::vector<const GPUBuffer*> vertexBuffers(meshCount);
    ea::vector<const GPUBuffer*> indexBuffers(meshCount);

    for (uint i = 0; i < matCount; ++i)
    {
        shaders[i] = materials[i]->GetShader();
        bindSets[i] = materials[i]->GetBindSet();
    }

    for (uint i = 0; i < meshCount; ++i)
    {
        vertexBuffers[i] = &meshes[i]->GetVertexBuffer();
        indexBuffers[i] = &meshes[i]->GetIndex16Buffer();
    }

    GPUCommandBuffer cmdBuffer{};
    cmdBuffer.shaders = std::move(shaders);
    cmdBuffer.bindSets3 = std::move(bindSets);
    cmdBuffer.vertexBuffers = std::move(vertexBuffers);
    cmdBuffer.indexBuffers = std::move(indexBuffers);
    cmdBuffer.firstInstances = std::move(firstInstances);
    cmdBuffer.instanceCounts = std::move(instanceCounts);

    return cmdBuffer;
}

static FrameData CreateFrameData(World& world, uint32 frameIndex)
{
    auto& engine = *world.engine;
    auto& lightCommon = world.pointLightCommon;
    auto& renderAssets = *engine.renderAssets;
    auto& gpu = *engine.gpu;

    FrameData frame = {};

    GPUSwapchain* swapchain = renderAssets.GetGPU().GetSwapchain();
    const uint2 extent = swapchain->GetExtent();

    UniqueGPUBufferPtr globalUniforms = gpu.CreateBuffer(sizeof(GlobalUniforms), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);
    UniqueGPUBufferPtr localToWorlds = gpu.CreateBuffer(MaxEntityCount * sizeof(Matrix4x4), GPUBufferUsage::eSSBO, GPUMemoryType::eCPUVisible);
    UniqueGPUBufferPtr litMaterialUniforms = gpu.CreateBuffer(MaxMaterialCount * sizeof(LitMaterialUniforms), GPUBufferUsage::eSSBO, GPUMemoryType::eCPUVisible);

    // Use this shader because it should carry the good VkPipelineLayout.
    // Remove this later when we have BindSetLayout
    UniqueGPUBindSetPtr bindSet0 = gpu.CreateBindSet(*renderAssets.GetDeferredLightingShader(), 0);
    bindSet0->SetBinding(globalUniforms.get(), 0);
    bindSet0->SetBinding(localToWorlds.get(), 1);
    bindSet0->SetBinding(litMaterialUniforms.get(), 2);

    UniqueGPUBufferPtr mainViewUniforms = gpu.CreateBuffer(sizeof(ViewUniforms), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);
    UniqueGPUBufferPtr mainEntityRemapTable = gpu.CreateBuffer(MaxEntityCount * sizeof(uint32), GPUBufferUsage::eSSBO, GPUMemoryType::eCPUVisible);
    UniqueGPUBindSetPtr mainBindSet1 = gpu.CreateBindSet(*renderAssets.GetDeferredLightingShader(), 1);
    mainBindSet1->SetBinding(mainViewUniforms.get(), 0);
    mainBindSet1->SetBinding(mainEntityRemapTable.get(), 1);

    UniqueGPUBufferPtr shadowCBuffer = gpu.CreateBuffer(sizeof(ShadowUniforms), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);

    UniqueGPUBufferPtr sunShadowEntityRemapTable = gpu.CreateBuffer(MaxEntityCount * sizeof(uint32), GPUBufferUsage::eSSBO, GPUMemoryType::eCPUVisible);
    UniqueGPUBindSetPtr sunShadowBindSet1 = gpu.CreateBindSet(*renderAssets.GetSunShadowsShader(), 1);
    sunShadowBindSet1->SetBinding(sunShadowEntityRemapTable.get(), 0);

    ea::vector<UniqueGPUBufferPtr> pointShadowEntityRemapTables = {};
    pointShadowEntityRemapTables.resize(MaxPointLightCastingShadows);

    ea::vector<UniqueGPUBindSetPtr> pointShadowBindSet1s{};
    pointShadowBindSet1s.resize(MaxPointLightCastingShadows);

    for (uint32 i = 0; i < MaxPointLightCastingShadows; ++i)
    {
        UniqueGPUBufferPtr remapTableGPU = gpu.CreateBuffer(MaxEntityCount * sizeof(uint32), GPUBufferUsage::eSSBO, GPUMemoryType::eCPUVisible);
        UniqueGPUBindSetPtr shadowBindSet1 = gpu.CreateBindSet(*lightCommon.shadowShader, 1);

        shadowBindSet1->SetBinding(mainViewUniforms.get(), 0);
        shadowBindSet1->SetBinding(shadowCBuffer.get(), 1);
        shadowBindSet1->SetBinding(lightCommon.shadowCubeIDUBOs[i].get(), 2);
        shadowBindSet1->SetBinding(remapTableGPU.get(), 3);

        pointShadowEntityRemapTables[i] = std::move(remapTableGPU);
        pointShadowBindSet1s[i] = std::move(shadowBindSet1);
    }

    SharedPtr<Image2D> sunShadowMap = renderAssets.CreateImage2D(4096, 4096, GPUFormat::eDepth24S8, GPUImageUsageFlags::eRenderAndSample);
    UniqueGPUFramebufferPtr sunShadowFramebuffer = CreateFramebufferFromDepthBuffer(gpu, renderAssets.GetSunShadowsPass(), *sunShadowMap);

    SharedPtr<ImageCubeArray> shadowCubeArray = renderAssets.CreateImageCubeArray(512, GPUFormat::eDepth24S8, MaxPointLightCastingShadows, GPUImageUsageFlags::eRenderAndSample);

    ea::vector<UniqueGPUFramebufferPtr> pointShadowFramebuffers{};
    pointShadowFramebuffers.resize(MaxPointLightCastingShadows);

    for (uint32 i = 0; i < MaxPointLightCastingShadows; ++i)
    {
        pointShadowFramebuffers[i] = CreateShadowFramebuffer(gpu, *shadowCubeArray, lightCommon.shadowPassLayout.get(), i);
    }

    GBuffer gbuffer = CreateGBuffer(renderAssets, extent.x, extent.y);
    UniqueGPUFramebufferPtr gbufferFramebuffer = CreateGBufferFramebuffer(gpu, renderAssets.GetGBufferPass(), gbuffer);

    uint32 tileCullingWidth = DivideRoundUp(extent.x, TileCullingDiv);
    uint32 tileCullingHeight = DivideRoundUp(extent.y, TileCullingDiv);

    SharedPtr<Image2D> tileCullingTextureOutput = renderAssets.CreateImage2D(tileCullingWidth, tileCullingHeight, GPUFormat::eUint16, GPUImageUsageFlags::eRenderAndSample);
    UniqueGPUBufferPtr tileCullingBufferOutput = gpu.CreateBuffer(tileCullingWidth * tileCullingHeight * MaxLightCountPerTile * sizeof(uint32),
        GPUBufferUsage::eSSBO, GPUMemoryType::eGPULocal);
    UniqueGPUFramebufferPtr tileCullingFramebuffer = CreateFramebufferFromColorBuffer(gpu, renderAssets.GetTileCullingPass(), *tileCullingTextureOutput);

    SharedPtr<Material> tileCullingProcess = renderAssets.CreateMaterial<MaterialNoUniforms>(renderAssets.GetTileCullingShader());
    tileCullingProcess->SetBinding(gbuffer.depthStencil->GetView(GPUImageComponent::eDepth), 0, GPUSamplerFiltering::eLinear, GPUSamplerAddressing::eClampToEdge);
    tileCullingProcess->SetBinding(tileCullingBufferOutput.get(), 1);

    SharedPtr<Image2D> ssaoColorBuffer = renderAssets.CreateImage2D(extent.x / SSAOResolutionDiv, extent.y / SSAOResolutionDiv, GPUFormat::eUNorm8x1, GPUImageUsageFlags::eRenderAndSample);
    UniqueGPUFramebufferPtr ssaoFramebuffer = CreateFramebufferFromColorBuffer(gpu, renderAssets.GetSSAOPass(), *ssaoColorBuffer);

    SharedPtr<Image2D> blurColorBuffer0 = renderAssets.CreateImage2D(extent.x / BloomResolutionDiv, extent.y / BloomResolutionDiv, GPUFormat::eRGBA8, GPUImageUsageFlags::eRenderAndSample);
    UniqueGPUFramebufferPtr blurFramebuffer0 = CreateFramebufferFromColorBuffer(gpu, renderAssets.GetBlurPass(), *blurColorBuffer0);

    SharedPtr<Image2D> blurColorBuffer1 = renderAssets.CreateImage2D(extent.x / BloomResolutionDiv, extent.y / BloomResolutionDiv, GPUFormat::eRGBA8, GPUImageUsageFlags::eRenderAndSample);
    UniqueGPUFramebufferPtr blurFramebuffer1 = CreateFramebufferFromColorBuffer(gpu, renderAssets.GetBlurPass(), *blurColorBuffer1);

    SharedPtr<Image2D> lightingOutput = renderAssets.CreateImage2D(extent.x, extent.y, GPUFormat::eRGBA8, GPUImageUsageFlags::eRenderAndSample);
    UniqueGPUFramebufferPtr lightingFramebuffer = CreateFramebufferFromColorBuffer(gpu, renderAssets.GetLightingPass(), *lightingOutput);

    UniqueGPUBufferPtr ssaoCBuffer = gpu.CreateBuffer(sizeof(SSAOUniforms), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);

    SharedPtr<Material> ssaoProcess = renderAssets.CreateMaterial<MaterialNoUniforms>(renderAssets.GetSSAOShader());
    ssaoProcess->SetBinding(ssaoCBuffer.get(), 0);
    ssaoProcess->SetBinding(renderAssets.GetSSAONoiseTexture()->GetView(GPUImageComponent::eColor), 1);
    ssaoProcess->SetBinding(gbuffer.depthStencil->GetView(GPUImageComponent::eDepth), 2, GPUSamplerFiltering::eLinear, GPUSamplerAddressing::eMirroredRepeat);
    ssaoProcess->SetBinding(gbuffer.normal->GetView(GPUImageComponent::eColor), 3);

    SharedPtr<Material> firstBlurProcess = renderAssets.CreateMaterial<MaterialNoUniforms>(renderAssets.GetBlurShader());
    firstBlurProcess->SetBinding(gbuffer.emissive->GetView(GPUImageComponent::eColor), 0, GPUSamplerFiltering::eLinear, GPUSamplerAddressing::eMirroredRepeat);

    SharedPtr<Material> blurProcess0 = renderAssets.CreateMaterial<MaterialNoUniforms>(renderAssets.GetBlurShader());
    blurProcess0->SetBinding(blurColorBuffer1->GetView(GPUImageComponent::eColor), 0, GPUSamplerFiltering::eLinear, GPUSamplerAddressing::eMirroredRepeat);

    SharedPtr<Material> blurProcess1 = renderAssets.CreateMaterial<MaterialNoUniforms>(renderAssets.GetBlurShader());
    blurProcess1->SetBinding(blurColorBuffer0->GetView(GPUImageComponent::eColor), 0, GPUSamplerFiltering::eLinear, GPUSamplerAddressing::eMirroredRepeat);

    SharedPtr<Material> lightingProcess = renderAssets.CreateMaterial<MaterialNoUniforms>(renderAssets.GetDeferredLightingShader());
    lightingProcess->SetBinding(gbuffer.depthStencil->GetView(GPUImageComponent::eDepth), 0);
    lightingProcess->SetBinding(gbuffer.color->GetView(GPUImageComponent::eColor), 1);
    lightingProcess->SetBinding(gbuffer.normal->GetView(GPUImageComponent::eColor), 2);
    lightingProcess->SetBinding(gbuffer.emissive->GetView(GPUImageComponent::eColor), 3);
    lightingProcess->SetBinding(gbuffer.mask->GetView(GPUImageComponent::eColor), 4);
    lightingProcess->SetBinding(gbuffer.heapIndex->GetView(GPUImageComponent::eColor), 5);
    lightingProcess->SetBinding(ssaoColorBuffer->GetView(GPUImageComponent::eColor), 6);
    lightingProcess->SetBinding(shadowCubeArray->GetCubeArrayView(GPUImageComponent::eDepth), 7);
    lightingProcess->SetBinding(shadowCBuffer.get(), 8);
    lightingProcess->SetBinding(blurColorBuffer1->GetView(GPUImageComponent::eColor), 9);
    lightingProcess->SetBinding(tileCullingTextureOutput->GetView(GPUImageComponent::eColor), 10);
    lightingProcess->SetBinding(tileCullingBufferOutput.get(), 11);
    lightingProcess->SetBinding(sunShadowMap->GetView(GPUImageComponent::eDepth), 12);

    frame.frameIndex = frameIndex;
    frame.gbufferFramebuffer = std::move(gbufferFramebuffer);
    frame.tileCullingTextureOutput = std::move(tileCullingTextureOutput);
    frame.tileCullingBufferOutput = std::move(tileCullingBufferOutput);
    frame.tileCullingFramebuffer = std::move(tileCullingFramebuffer);
    frame.tileCullingProcess = std::move(tileCullingProcess);
    frame.sunShadowFramebuffer = std::move(sunShadowFramebuffer);
    frame.ssaoFramebuffer = std::move(ssaoFramebuffer);
    frame.blurFramebuffer0 = std::move(blurFramebuffer0);
    frame.blurFramebuffer1 = std::move(blurFramebuffer1);
    frame.lightingOutput = std::move(lightingOutput);
    frame.lightingFramebuffer = std::move(lightingFramebuffer);
    frame.globalUniforms = std::move(globalUniforms);
    frame.litMaterialUniforms = std::move(litMaterialUniforms);
    frame.localToWorlds = std::move(localToWorlds);
    frame.bindSet0 = std::move(bindSet0);
    frame.gbuffer = std::move(gbuffer);
    frame.mainViewUniforms = std::move(mainViewUniforms);
    frame.mainEntityRemapTable = std::move(mainEntityRemapTable);
    frame.mainBindSet1 = std::move(mainBindSet1);
    frame.shadowCBuffer = std::move(shadowCBuffer);
    frame.sunShadowEntityRemapTables = std::move(sunShadowEntityRemapTable);
    frame.sunShadowBindSet1 = std::move(sunShadowBindSet1);
    frame.pointShadowEntityRemapTables = std::move(pointShadowEntityRemapTables);
    frame.pointShadowBindSet1s = std::move(pointShadowBindSet1s);
    frame.shadowCubeArray = std::move(shadowCubeArray);
    frame.pointShadowFramebuffers = std::move(pointShadowFramebuffers);
    frame.sunShadowMap = std::move(sunShadowMap);
    frame.ssaoColorBuffer = std::move(ssaoColorBuffer);
    frame.ssaoCBuffer = std::move(ssaoCBuffer);
    frame.ssaoProcess = std::move(ssaoProcess);
    frame.blurColorBuffer0 = std::move(blurColorBuffer0);
    frame.blurColorBuffer1 = std::move(blurColorBuffer1);
    frame.firstBlurProcess = std::move(firstBlurProcess);
    frame.blurProcess0 = std::move(blurProcess0);
    frame.blurProcess1 = std::move(blurProcess1);
    frame.lightingProcess = std::move(lightingProcess);

    return frame;
}

static void SetupRenderEntityData(EntityManager& entityManager)
{
    const ea::array AllTypes =
    {
        GetTypeID<EntityGPUBufferIndex>(),
        GetTypeID<EntityModelID>(),
        GetTypeID<RenderComponent>(),
    };

    ea::vector<EntityChunk*> chunks = entityManager.QueryEntityChunks(AllTypes);

    uint32 gpuIndex = 0;
    for (EntityChunk* chunk : chunks)
    {
        EntityGPUBufferIndex* gpuIndices = chunk->GetMutableComponentLane<EntityGPUBufferIndex>();
        EntityModelID* modelIDs = chunk->GetMutableComponentLane<EntityModelID>();
        const UniquePtr<const RenderComponent>* renderComponents = chunk->GetAbstractComponentLane<RenderComponent>();

        for (uint32 i = 0; i < chunk->GetEntityCount(); ++i)
        {
            const EntityID entityID = entityManager.GetEntityID(*chunk, i);

            gpuIndices[i].val = gpuIndex;
            modelIDs[i].val = renderComponents[i]->GetModel().GetID();

            ++gpuIndex;
        }
    }
}

static void UpdatePVSDataCaches(EntityManager& entityManager)
{
    const ea::array ComponentTypes
    {
        GetTypeID<PVSDynamicData>(),
        GetTypeID<PVSDataCache>()
    };

    ea::vector<EntityChunk*> chunks = entityManager.QueryEntityChunks(ComponentTypes);
    for (EntityChunk* chunk : chunks)
    {
        const UniquePtr<const PVSDynamicData>* dynamicDatas = chunk->GetAbstractComponentLane<PVSDynamicData>();
        UniquePtr<PVSDataCache>* caches = chunk->GetMutableAbstractComponentLane<PVSDataCache>();

        for (uint32 i = 0; i < chunk->GetEntityCount(); ++i)
        {
            const PVSDynamicData& dynData = *dynamicDatas[i];

            *caches[i] = BuildPVSDataCache(entityManager, dynData);
        }
    }
}

static void UpdateMatMeshMainPassCache(const RenderAssetManager& renderAssets, const MatMeshRefCountManager& refCountManager, MatMeshMetadataCache& cache)
{
    const GPURenderPass* gbufferPass = renderAssets.GetGBufferPass();

    MatMeshQuery matMeshQuery = MatMeshQueryBuilder(*gbufferPass, 0, renderAssets)
        .Build();

    ea::vector<uint32> refCounts = refCountManager.CreateRefCountArray(gbufferPass->GetID(), 0);

    InstanceDrawingLayout drawingLayout = CreateInstanceDrawingLayout(refCounts);

    cache.mainMatMeshQuery = std::move(matMeshQuery);
    cache.mainRefCounts = std::move(refCounts);
    cache.mainDrawingLayout = std::move(drawingLayout);
}

static void UpdateMatMeshShadowPassCache(const PointLightCommon& lightCommon, const RenderAssetManager& renderAssets, MatMeshMetadataCache& cache)
{
    const GPURenderPass* gbufferPass = renderAssets.GetGBufferPass();

    MatMeshQuery pointShadowMatMeshQuery = MatMeshQueryBuilder(*gbufferPass, 0, renderAssets)
        .SetGlobalMaterialOverride(lightCommon.shadowMaterial)
        .Build();

    ea::vector<uint32> pointShadowRefCounts = pointShadowMatMeshQuery.CreateRemappedVector(cache.mainRefCounts, renderAssets);
    InstanceDrawingLayout pointShadowDrawingLayout = CreateInstanceDrawingLayout(pointShadowRefCounts);

    MatMeshQuery sunShadowMatMeshQuery = MatMeshQueryBuilder(*gbufferPass, 0, renderAssets)
        .SetGlobalMaterialOverride(renderAssets.GetSunShadowsMaterial())
        .Build();

    ea::vector<uint32> sunShadowRefCounts = sunShadowMatMeshQuery.CreateRemappedVector(cache.mainRefCounts, renderAssets);
    InstanceDrawingLayout sunShadowDrawingLayout = CreateInstanceDrawingLayout(sunShadowRefCounts);

    cache.pointShadowMatMeshQuery = std::move(pointShadowMatMeshQuery);
    cache.pointShadowRefCounts = std::move(pointShadowRefCounts);
    cache.pointShadowDrawingLayout = std::move(pointShadowDrawingLayout);

    cache.sunShadowMatMeshQuery = std::move(sunShadowMatMeshQuery);
    cache.sunShadowRefCounts = std::move(sunShadowRefCounts);
    cache.sunShadowDrawingLayout = std::move(sunShadowDrawingLayout);
}

static void UpdateMatMeshMetadataCache(const RenderContext& context, MatMeshMetadataCache& cache)
{
    const RenderAssetManager& renderAssets = *context.renderAssets;
    const MatMeshRefCountManager& refCountManager = *context.refCountManager;

    const GPURenderPass* layout = renderAssets.GetGBufferPass();
    uint32 newMainPassVersion = renderAssets.GetSubpassDataVersion(layout->GetID(), 0);

    if (cache.mainPassLastVersion != newMainPassVersion)
    {
        UpdateMatMeshMainPassCache(renderAssets, refCountManager, cache);
        UpdateMatMeshShadowPassCache(*context.pointLightCommon, renderAssets, cache);

        cache.mainPassLastVersion = newMainPassVersion;
    }
}

static void FlushGlobalUniforms(const RenderContext& context, const PointLightCulling& pointLights, GPUBuffer& buffer)
{
    const GlobalData& global = *context.global;
    uint32 pointLightCount = pointLights.lightCount;

    GlobalUniforms uniforms{};

    uniforms.sunShadowWorldClip = Mat4Transpose(context.sunShadowsViewMatrix * Mat4ProjToVulkanProj(context.sunShadowsProjMatrix));
    uniforms.debugFloat4 = global.debugFloat4;
    uniforms.ambientAndIntensity = float4(global.ambient, global.ambientIntensity);

    uniforms.dirLightColorAndIntensity = float4(global.sunLightColor, global.sunLightIntensity);
    uniforms.dirLightDirection = float4(context.sunLightDirection, 0);

    memcpy(&uniforms.pointLightColorAndIntensities, pointLights.colorAndIntensities.data(), pointLightCount * sizeof(float4));
    memcpy(&uniforms.pointLightPositionAndRanges, pointLights.positionAndRanges.data(), pointLightCount * sizeof(float4));
    uniforms.pointLightCount = pointLightCount;

    uniforms.colorFilterAndIntensity = float4(global.colorFilter, global.colorFilterIntensity);
    uniforms.bloomColorAndIntensity = float4(global.bloomColor, global.bloomIntensity);
    uniforms.contrast = global.contrast * 0.001f;
    uniforms.saturation = global.saturation * 0.01f;
    uniforms.exposure = global.exposure;

    uniforms.useAO = global.useAO;
    uniforms.useEmissive = global.useEmissive;
    uniforms.useBloom = global.useBloom;
    uniforms.debugBuffer = ToU32(global.debugBuffer);
    uniforms.castShadows = global.castShadows;

    buffer.Upload(AsByteSpan(uniforms));
}

static void FlushViewUniforms(const RenderContext& context, GPUBuffer& buffer)
{
    const uint2& resolution = context.cameraResolution;
    const float3& viewerPos = context.cameraPosition;
    const Matrix4x4& worldView = context.cameraWorldToView;
    const Matrix4x4 viewClip = Mat4ProjToVulkanProj(context.cameraViewToClip);
    const Matrix4x4& worldClip = Mat4ProjToVulkanProj(context.cameraWorldToClip);

    ViewUniforms viewUniforms{};
    viewUniforms.viewClip = Mat4Transpose(viewClip);
    viewUniforms.worldView = Mat4Transpose(worldView);
    viewUniforms.worldClip = Mat4Transpose(worldClip);
    viewUniforms.clipWorld = Mat4Transpose(Mat4Inverse(worldClip));
    viewUniforms.viewerPos = float4(viewerPos, 1);
    viewUniforms.resolution = uint2(resolution.x, resolution.y);
    viewUniforms.tileCullingResolution = uint2(DivideRoundUp(resolution.x, TileCullingDiv), DivideRoundUp(resolution.y, TileCullingDiv));

    buffer.Upload(AsByteSpan(viewUniforms));
}

static void FlushShadowUniforms(const FrameCulling& culling, FrameData& gpuFrame)
{
    const ea::span<const ShadowMatrixUniforms> matrices = culling.pointLights.shadowMatrices;
    const ea::span<const ShadowParamUniforms> params = culling.pointLights.shadowParams;

    ASSERT(matrices.size() == params.size());

    const usize lightCount = culling.pointLights.shadowLightCount;

    ShadowUniforms uniforms{};
    memcpy(uniforms.matrices.data(), matrices.data(), lightCount * sizeof(matrices[0]));
    memcpy(uniforms.params.data(), params.data(), lightCount * sizeof(params[0]));

    gpuFrame.shadowCBuffer->Upload(AsByteSpan(uniforms));
}

static void FlushMaterialUniforms(RenderAssetManager& renderAssets, GPUBuffer& buffer, ea::hash_map<MaterialID, uint32>& materialVersion)
{
    const GPURenderPass* gbufferPass = renderAssets.GetGBufferPass();
    ea::span<const SharedPtr<Material>> materials = renderAssets.GetMaterialBatch(gbufferPass->GetID(), 0);

    for (const SharedPtr<Material>& material : materials)
    {
        if (!material->IsUniformDataType<LitMaterialUniforms>())
            continue;

        uint32& oldVersion = materialVersion[material->GetID()];
        const uint32 newVersion = material->GetUniformDataVersion();

        if (oldVersion != newVersion)
        {
            const uint32 heapIndex = renderAssets.GetHeapIndex(material->GetID());
            const LitMaterialUniforms& uniforms = material->GetUniformData<LitMaterialUniforms>();

            buffer.Upload(ea::span(&uniforms, 1), heapIndex);
            oldVersion = newVersion;
        }
    }
}

static void FlushLocalToWorlds(const RenderContext& context, GPUBuffer& matrixBuffer, ea::hash_map<EntityArchetypeID, ComponentLaneVersion>& localToWorldGPUVersions)
{
    ea::span<const RenderEntityChunk> chunks = context.entityChunks;

    uint64 begin = 0;
    for (const RenderEntityChunk& chunk : chunks)
    {
        ComponentLaneVersion& localToWorldsVersion = localToWorldGPUVersions[chunk.archetypeID];
        const ComponentLaneVersion newLocalToWorldsVersion = chunk.localToWorldsVersion;

        if (newLocalToWorldsVersion != localToWorldsVersion)
        {
            ea::vector<Matrix4x4> matrices;
            matrices.resize(chunk.size);

            for (uint32 i = 0; i < chunk.size; ++i)
            {
                matrices[i] = Mat4Transpose(chunk.localToWorlds[i]);
            }

            matrixBuffer.Upload<Matrix4x4>(ea::span(matrices), uint32(begin));

            localToWorldsVersion = newLocalToWorldsVersion;
        }

        begin += chunk.size;
    }
}

static void FlushSSAOUniforms(const RenderContext& context, GPUBuffer& ssaoCBuffer)
{
    static_assert(sizeof(SSAOUniforms::kernel[0]) == sizeof(GlobalData::ssaoKernel[0]));
    const GlobalData& global = *context.global;

    SSAOUniforms uniforms{};
    memcpy(uniforms.kernel.data(), global.ssaoKernel.data(), sizeof(global.ssaoKernel[0]) * global.ssaoKernel.size());
    uniforms.strength = global.ssaoStrength;
    uniforms.radius = global.ssaoRadius;

    ssaoCBuffer.Upload(AsByteSpan(uniforms));
}

static GPURenderPassExecution CreateSunShadowPassGPUExecution(const MatMeshMetadataCache& cache, const FrameCulling& frameCulling, FrameData& gpuFrame)
{
    const InstanceDrawingLayout& drawingLayout = cache.sunShadowDrawingLayout;
    const MatMeshQuery& matMeshQuery = cache.sunShadowMatMeshQuery;

    GPUSupbassExecution subpass{};
    const CullingOutput& culling = frameCulling.sunShadows;
    subpass.commandBuffer = CreateCommandBuffer(matMeshQuery, ea::vector(culling.instanceCounts), ea::vector(drawingLayout.firstInstances));

    ea::vector<GPUSupbassExecution> subpasses{};
    subpasses.emplace_back(std::move(subpass));

    GPURenderPassExecution exec{};
    exec.framebuffer = gpuFrame.sunShadowFramebuffer.get();
    exec.bindSet1 = gpuFrame.sunShadowBindSet1.get();
    exec.subpasses = std::move(subpasses);

    return exec;
}

static GPURenderPassExecution CreatePointShadowPassGPUExecution(const MatMeshMetadataCache& cache, const FrameCulling& frameCulling, FrameData& gpuFrame, uint32 passIndex, bool emptyPass = false)
{
    const InstanceDrawingLayout& drawingLayout = cache.pointShadowDrawingLayout;
    const MatMeshQuery& matMeshQuery = cache.pointShadowMatMeshQuery;

    GPUSupbassExecution subpass{};
    if (!emptyPass)
    {
        const CullingOutput& culling = frameCulling.pointShadows[passIndex];
        subpass.commandBuffer = CreateCommandBuffer(matMeshQuery, ea::vector(culling.instanceCounts), ea::vector(drawingLayout.firstInstances));
    }

    ea::vector<GPUSupbassExecution> subpasses{};
    subpasses.emplace_back(std::move(subpass));

    GPURenderPassExecution exec{};
    exec.framebuffer = gpuFrame.pointShadowFramebuffers[passIndex].get();
    exec.bindSet1 = gpuFrame.pointShadowBindSet1s[passIndex].get();
    exec.subpasses = std::move(subpasses);

    return exec;
}

static GPURenderPassExecution CreateGBufferGPUExecution(const MatMeshMetadataCache& cache, const FrameCulling& frameCulling, FrameData& gpuFrame)
{
    const CullingOutput& culling = frameCulling.gbuffer;
    const MatMeshQuery& matMeshQuery = cache.mainMatMeshQuery;
    const InstanceDrawingLayout& drawingLayout = cache.mainDrawingLayout;

    GPUCommandBuffer gbufferCommand = ::CreateCommandBuffer(matMeshQuery, ea::vector(culling.instanceCounts), ea::vector(drawingLayout.firstInstances));

    GPURenderPassExecution exec{};
    exec.framebuffer = gpuFrame.gbufferFramebuffer.get();
    exec.bindSet1 = gpuFrame.mainBindSet1.get();
    exec.subpasses =
    {
        { nullptr, std::move(gbufferCommand) }
    };

    return exec;
}

static GPURenderPassExecution CreatePostProcessGPUExecution(RenderAssetManager& renderAssets, const Material& process, const GPUFramebuffer* framebuffer, const GPUBindSet* bindSet1)
{
    GPUCommandBuffer command = CreatePostProcessCommandBuffer(renderAssets, process);

    GPURenderPassExecution exec = {};
    exec.framebuffer = framebuffer;
    exec.bindSet1 = bindSet1;
    exec.subpasses =
    {
        { nullptr, std::move(command) }
    };

    return exec;
}

static GPUFrameExecution CreateGPUFrameExecution(RenderAssetManager& renderAssets, const MatMeshMetadataCache& cache, const FrameCulling& culling, FrameData& gpuFrame, const GlobalData& global)
{
    usize shadowLightCount = culling.pointLights.shadowLightCount;

    ea::vector<GPURenderPassExecution> pointShadowPasses{};
    pointShadowPasses.resize(MaxPointLightCastingShadows);

    for (uint32 i = 0; i < MaxPointLightCastingShadows; ++i)
    {
        bool emptyPass = i >= shadowLightCount;
        pointShadowPasses[i] = CreatePointShadowPassGPUExecution(cache, culling, gpuFrame, i, emptyPass);
    }

    GPUFrameExecution exec{};
    exec.frameIndex = gpuFrame.frameIndex;
    exec.blurPassCount = global.bloomBlurPass;
    exec.bindSet0 = gpuFrame.bindSet0.get();

    exec.shadowPasses = std::move(pointShadowPasses);
    exec.gbufferPass = CreateGBufferGPUExecution(cache, culling, gpuFrame);
    exec.sunShadowPass = CreateSunShadowPassGPUExecution(cache, culling, gpuFrame);
    exec.tileCullingPass = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.tileCullingProcess, gpuFrame.tileCullingFramebuffer.get(), gpuFrame.mainBindSet1.get());
    exec.ssaoPass = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.ssaoProcess, gpuFrame.ssaoFramebuffer.get(), gpuFrame.mainBindSet1.get());
    exec.firstBlurPass0 = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.firstBlurProcess, gpuFrame.blurFramebuffer0.get(), nullptr);
    exec.firstBlurPass1 = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.firstBlurProcess, gpuFrame.blurFramebuffer1.get(), nullptr);
    exec.blurPass0 = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.blurProcess0, gpuFrame.blurFramebuffer0.get(), nullptr);
    exec.blurPass1 = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.blurProcess1, gpuFrame.blurFramebuffer1.get(), nullptr);
    exec.lightingPass = CreatePostProcessGPUExecution(renderAssets, *gpuFrame.lightingProcess, gpuFrame.lightingFramebuffer.get(), gpuFrame.mainBindSet1.get());

    exec.shadowCubeArray = &gpuFrame.shadowCubeArray->GetGPUImage();
    exec.depthBuffer = &gpuFrame.gbuffer.depthStencil->GetGPUImage();
    exec.normalBuffer = &gpuFrame.gbuffer.normal->GetGPUImage();
    exec.emissiveBuffer = &gpuFrame.gbuffer.emissive->GetGPUImage();
    exec.tileCullingTexture = &gpuFrame.tileCullingTextureOutput->GetGPUImage();
    exec.tileCullingBuffer = gpuFrame.tileCullingBufferOutput.get();
    exec.blurBuffer0 = &gpuFrame.blurColorBuffer0->GetGPUImage();
    exec.blurBuffer1 = &gpuFrame.blurColorBuffer1->GetGPUImage();
    exec.lightingOutput = &gpuFrame.lightingOutput->GetGPUImage();

    return exec;
}

static void UploadShadowEntityRemapTables(const FrameCulling& culling, FrameData& gpuFrame)
{
    uint32 shadowLightCount = culling.pointLights.shadowLightCount;

    for (uint32 i = 0; i < shadowLightCount; ++i)
    {
        gpuFrame.pointShadowEntityRemapTables[i]->Upload<uint32>(culling.pointShadows[i].remappedEntityIDs);
    }

    gpuFrame.sunShadowEntityRemapTables->Upload<uint32>(culling.sunShadows.remappedEntityIDs);
}

static void UpdateStats(SceneStats& stats, const FrameCulling& culling)
{
    const PointLightCulling& lightCulling = culling.pointLights;

    uint32 visibleShadowCasterCount = 0;
    for (uint32 i = 0; i < lightCulling.shadowLightCount; ++i)
    {
        visibleShadowCasterCount += culling.pointShadows[i].visibleRendererCount;
    }
    visibleShadowCasterCount += culling.sunShadows.visibleRendererCount;

    stats.visibleRendererCount = culling.gbuffer.visibleRendererCount;
    stats.shadowCasterCount = stats.rendererCount * lightCulling.shadowLightCount + stats.rendererCount;
    stats.visibleShadowCasterCount = visibleShadowCasterCount;
    stats.visiblePointLightCount = culling.pointLights.lightCount;
}

void Gyn::InitGraphicsEngine(World& world)
{
    auto& engine = *world.engine;
    auto& frames = world.frameData.frames;
    auto& gpu = *engine.gpu;
    auto& swapchain = *gpu.GetSwapchain();

    const uint32 imageCount = swapchain.GetImageCount();
    frames.resize(imageCount);

    for (uint32 i = 0; i < imageCount; ++i)
    {
        frames[i] = CreateFrameData(world, i);
    }

    gpu.Flush();
}

SharedPtr<Material> Gyn::CreatePresentProcess(RenderAssetManager* renderAssets, const Image2D* lightingOutput)
{
    SharedPtr<Material> presentProcess = renderAssets->CreateMaterial<MaterialNoUniforms>(renderAssets->GetPresentShader());
    presentProcess->SetBinding(lightingOutput->GetView(GPUImageComponent::eColor), 0);

    return presentProcess;
}

void Gyn::BeforeFirstUpdateGraphicsEngine(World& world)
{
    SetupRenderEntityData(world.entityManager);
    UpdatePVSDataCaches(world.entityManager);
}

void Gyn::UpdateGraphicsEngine(World& world, const GPUFramebuffer* presentFramebuffer, const Material* presentProcess, uint32 frameIndex)
{
    auto& engine = *world.engine;
    auto& renderAssets = *engine.renderAssets;
    auto& gpu = *engine.gpu;
    auto& jobScheduler = *engine.jobScheduler;
    auto& global = world.global;
    auto& frameData = world.frameData;

    SceneStats& stats = global.stats;
    ea::vector<FrameData>& frames = frameData.frames;
    MatMeshMetadataCache& matMeshCache = frameData.matMeshCache;

    RenderContext context = CreateRenderContext(world);

    UpdateMatMeshMetadataCache(context, matMeshCache);

    FrameCulling culling = CullScene(jobScheduler, context, matMeshCache, 8192);

    FrameData& gpuFrame = frames[frameIndex];
    if (gpuFrame.fence)
    {
        gpuFrame.fence->Wait();
    }

    gpuFrame.mainEntityRemapTable->Upload<uint32>(culling.gbuffer.remappedEntityIDs);
    UploadShadowEntityRemapTables(culling, gpuFrame);

    FlushGlobalUniforms(context, culling.pointLights, *gpuFrame.globalUniforms);
    FlushViewUniforms(context, *gpuFrame.mainViewUniforms);
    FlushShadowUniforms(culling, gpuFrame);
    FlushMaterialUniforms(renderAssets, *gpuFrame.litMaterialUniforms, gpuFrame.materialVersions);
    FlushLocalToWorlds(context, *gpuFrame.localToWorlds, gpuFrame.localToWorldGPUVersions);
    FlushSSAOUniforms(context, *gpuFrame.ssaoCBuffer);

    UpdateStats(stats, culling);

    GPUFrameExecution frameExec = CreateGPUFrameExecution(renderAssets, matMeshCache, culling, gpuFrame, global);
    frameExec.presentPass = CreatePostProcessGPUExecution(renderAssets, *presentProcess, presentFramebuffer, nullptr);
    auto fence = gpu.Submit(frameExec);

    gpuFrame.fence = std::move(fence);
}