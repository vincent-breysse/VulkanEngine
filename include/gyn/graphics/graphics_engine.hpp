#pragma once

#include <gyn/graphics/common.hpp>
#include <gyn/graphics/mat_mesh_query.hpp>
#include <gyn/graphics/pvs.hpp>

namespace Gyn
{
    struct GlobalUniforms
    {
        alignas(16) Matrix4x4 sunShadowWorldClip;
        alignas(16) float4 debugFloat4;
        alignas(16) float4 ambientAndIntensity;
        alignas(16) float4 dirLightDirection;
        alignas(16) float4 dirLightColorAndIntensity;

        alignas(16) ea::array<float4, MaxPointLightCount> pointLightPositionAndRanges;
        alignas(16) ea::array<float4, MaxPointLightCount> pointLightColorAndIntensities;

        alignas(16) float4 colorFilterAndIntensity;
        alignas(16) float4 bloomColorAndIntensity;
        float contrast;
        float saturation;
        float exposure;

        uint32 pointLightCount;
        uint32 useAO;
        uint32 debugBuffer;
        uint32 useEmissive;
        uint32 useBloom;
        uint32 castShadows;
    };

    struct ViewUniforms
    {
        alignas(16) Matrix4x4 worldView;
        alignas(16) Matrix4x4 viewClip;
        alignas(16) Matrix4x4 worldClip;
        alignas(16) Matrix4x4 clipWorld;
        alignas(16) float4 viewerPos;
        alignas(8) uint2 resolution;
        alignas(8) uint2 tileCullingResolution;
    };

    struct Resolution : public ECSComponent
    {
        Resolution() = default;

        Resolution(uint32 width, uint32 height) : val{ width, height } {}

        uint2 val;
    };

    struct PerspectiveProjection : public ECSComponent
    {
        PerspectiveProjection() = default;

        PerspectiveProjection(float fov, float near, float far) : 
            fov(fov), near(near), far(far) {}

        float fov;
        float near;
        float far;
    };

    struct WorldToView : public ECSComponent
    {
        WorldToView() = default;

        WorldToView(const Matrix4x4& matrix) : val(matrix) {}

        Matrix4x4 val;
    };

    struct ViewToClip : public ECSComponent
    {
        ViewToClip() = default;

        ViewToClip(const Matrix4x4& matrix) : val(matrix) {}

        Matrix4x4 val;
    };

    struct WorldToClip : public ECSComponent
    {
        WorldToClip() = default;

        WorldToClip(const Matrix4x4& matrix) : val(matrix) {}

        Matrix4x4 val;
    };

    struct InstanceDrawingLayout
    {
        ea::vector<uint32> firstInstances;
        uint32 maxInstanceCount;
    };

    struct GBuffer
    {
        SharedPtr<Image2D> color;
        SharedPtr<Image2D> normal;
        SharedPtr<Image2D> depthStencil;
        SharedPtr<Image2D> emissive;
        SharedPtr<Image2D> mask;
        SharedPtr<Image2D> heapIndex;
    };

    struct MatMeshMetadataCache
    {
        uint32 mainPassLastVersion;

        MatMeshQuery mainMatMeshQuery;
        ea::vector<uint32> mainRefCounts;
        InstanceDrawingLayout mainDrawingLayout;

        MatMeshQuery pointShadowMatMeshQuery;
        ea::vector<uint32> pointShadowRefCounts;
        InstanceDrawingLayout pointShadowDrawingLayout;

        MatMeshQuery sunShadowMatMeshQuery;
        ea::vector<uint32> sunShadowRefCounts;
        InstanceDrawingLayout sunShadowDrawingLayout;
    };

    struct FrameData
    {
        ea::hash_map<MaterialID, uint32> materialVersions;
        ea::hash_map<EntityArchetypeID, ComponentLaneVersion> localToWorldGPUVersions;
        uint32 frameIndex;
        UniqueGPUFencePtr fence;
        
        SharedPtr<ImageCubeArray> shadowCubeArray;

        UniqueGPUBufferPtr sunShadowEntityRemapTables;
        ea::vector<UniqueGPUBufferPtr> pointShadowEntityRemapTables;
        UniqueGPUBufferPtr shadowCBuffer;
        UniqueGPUBufferPtr ssaoCBuffer;
        UniqueGPUBufferPtr tileCullingBufferOutput;
        UniqueGPUBufferPtr mainViewUniforms;
        UniqueGPUBufferPtr mainEntityRemapTable;
        UniqueGPUBufferPtr globalUniforms;
        UniqueGPUBufferPtr litMaterialUniforms;
        UniqueGPUBufferPtr localToWorlds;

        GBuffer gbuffer;
        SharedPtr<Image2D> ssaoColorBuffer;
        SharedPtr<Image2D> blurColorBuffer0;
        SharedPtr<Image2D> blurColorBuffer1;
        SharedPtr<Image2D> tileCullingTextureOutput;
        SharedPtr<Image2D> lightingOutput;
        SharedPtr<Image2D> sunShadowMap;

        UniqueGPUBindSetPtr bindSet0;
        UniqueGPUBindSetPtr sunShadowBindSet1;
        ea::vector<UniqueGPUBindSetPtr> pointShadowBindSet1s;
        UniqueGPUBindSetPtr mainBindSet1;

        SharedPtr<Material> tileCullingProcess;
        SharedPtr<Material> ssaoProcess;
        SharedPtr<Material> firstBlurProcess;
        SharedPtr<Material> blurProcess0;
        SharedPtr<Material> blurProcess1;
        SharedPtr<Material> lightingProcess;

        ea::vector<UniqueGPUFramebufferPtr> pointShadowFramebuffers;
        UniqueGPUFramebufferPtr gbufferFramebuffer;
        UniqueGPUFramebufferPtr ssaoFramebuffer;
        UniqueGPUFramebufferPtr blurFramebuffer0;
        UniqueGPUFramebufferPtr blurFramebuffer1;
        UniqueGPUFramebufferPtr tileCullingFramebuffer;
        UniqueGPUFramebufferPtr lightingFramebuffer;
        UniqueGPUFramebufferPtr sunShadowFramebuffer;
    };

    struct FrameDataBuffer
    {
        ea::vector<FrameData> frames;
        MatMeshMetadataCache matMeshCache;
    };

    struct RenderEntityChunk
    {
        EntityArchetypeID archetypeID = {};

        const Matrix4x4* localToWorlds = {};
        ComponentLaneVersion localToWorldsVersion = {};

        const uint32* gpuIndices = {};
        const ModelID* modelIDs = {};
        const AABB* worldAABBs = {};
        const bool* disableShadows = {};
        uint32 size = {};
    };

    struct PointLightChunk
    {
        const PointLight* lights = {};
        const float3* positions = {};
        const EntityID* pvsIDs = {};
        uint32 size = {};
    };
    
    struct RenderContext
    {
        const GlobalData* global = {};
        const RenderAssetManager* renderAssets = {};
        const MatMeshRefCountManager* refCountManager = {};
        uint32 totalEntityCount = {};
        ea::vector<RenderEntityChunk> entityChunks = {};

        float3 sunLightDirection = {};
        Matrix4x4 sunShadowsViewMatrix = {};
        Matrix4x4 sunShadowsProjMatrix = {};

        const PointLightCommon* pointLightCommon = {};
        uint32 totalPointLightCount = {};
        ea::vector<PointLightChunk> pointLightChunks = {};
        ea::vector<const PVSDataCache*> pointLightPVSCaches = {};

        float3 cameraPosition = {};
        Matrix4x4 cameraWorldToView = {};
        Matrix4x4 cameraViewToClip = {};
        Matrix4x4 cameraWorldToClip = {};
        ea::array<float4, 6> cameraFrustrumPlanes = {};
        FrustrumPlanesSOA cameraFrustrumPlanesSOA = {};
        const PVSDataCache* cameraPVSDataCache = {};
        uint2 cameraResolution = {};
    };

    void InitGraphicsEngine(World& world);

    SharedPtr<Material> CreatePresentProcess(RenderAssetManager* renderAssets, const Image2D* lightingOutput);

    void BeforeFirstUpdateGraphicsEngine(World& world);

    void UpdateGraphicsEngine(World& world, const GPUFramebuffer* presentFramebuffer, const Material* presentProcess, uint32 frameIndex);

    ea::span<const TypeID> GetCameraComponentTypes();
}