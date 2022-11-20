#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    struct MaterialLitCreateInfo
    {
        SharedPtr<Image2D> albedoMap;
        SharedPtr<Image2D> normalMap;
        SharedPtr<Image2D> maskMap;
        SharedPtr<Image2D> emissiveMap;
        LitMaterialUniforms uniforms;
    };

    class RenderAssetManager : NoCopyMove
    {
    public:

        RenderAssetManager() = default;

        RenderAssetManager(GPU& gpu);

        bool IsNull() const;

        GPU& GetGPU() const;

        SharedPtr<const Mesh> GetScreenMesh() const;

        const GPUShader* GetGBufferShader() const;

        const GPUShader* GetSunShadowsShader() const;

        const GPUShader* GetTileCullingShader() const;

        const GPUShader* GetSSAOShader() const;

        const GPUShader* GetBlurShader() const;

        const GPUShader* GetDeferredLightingShader() const;

        const GPUShader* GetPresentShader() const;

        SharedPtr<Image2D> GetBlackTexture() const;

        SharedPtr<Image2D> GetDefaultMaskMap() const;

        SharedPtr<Image2D> GetNormalTexture() const;

        SharedPtr<Image2D> GetWhiteTexture() const;

        SharedPtr<Image2D> GetSSAONoiseTexture() const;

        SharedPtr<Material> GetWhiteMaterial() const;

        SharedPtr<Mesh> GetCubeMesh() const;

        SharedPtr<const Model> GetWhiteCube() const;

        const GPURenderPass* GetGBufferPass() const;

        const GPURenderPass* GetSunShadowsPass() const;

        const GPURenderPass* GetTileCullingPass() const;

        const GPURenderPass* GetSSAOPass() const;

        const GPURenderPass* GetBlurPass() const;

        const GPURenderPass* GetLightingPass() const;

        const GPURenderPass* GetPresentPass() const;

        UniqueGPURenderPassPtr CreateRenderPass(ea::span<const GPUFormat> formats, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks = {});

        UniqueGPURenderPassPtr CreateRenderPass(ea::span<const GPUFormat> colorFormats, bool hasDepthStencil);

        SharedPtr<const Material> GetSunShadowsMaterial() const;

        SharedPtr<Image2D> LoadTexture(const char* path);

        void RegisterMaterial(SharedPtr<Material> material);

        // TODO : Remove CreateMaterial() functions. Only keep RegisterMaterial()
        SharedPtr<Material> CreateMaterial(const GPUShader* shader, TypeID uniformStructTypeID, uint64 uniformStructSizeByte);

        SharedPtr<Material> CreateLitMaterial(MaterialLitCreateInfo&& info);

        SharedPtr<Material> CreateUnlitMaterial(float3 color, float emissiveStrength);

        SharedPtr<Mesh> CreateMesh(ea::span<const Vertex> vertices, ea::span<const uint16> indices, const AABB& bounds);

        SharedPtr<Model> CreateModel(ea::vector<SharedPtr<Material>>&& materials, ea::vector<SharedPtr<Mesh>>&& meshes);

        SharedPtr<Model> CreateModel(SharedPtr<Material> material, SharedPtr<Mesh> mesh);

        SharedPtr<Model> LoadModel(const char* path);

        ea::span<const MatMeshLocalIndex> GetModelLocalIndices(ModelID id) const;

        uint32 GetModelLocalIndicesVersion(ModelID id) const;

        ea::span<const SharedPtr<Material>> GetMaterialBatch(GPURenderPassID id, uint32 subpass) const;

        uint32 GetMaterialBatchSize(GPURenderPassID id, uint32 subpass) const;

        uint32 GetSubpassDataVersion(GPURenderPassID id, uint32 subpass) const;

        ea::span<const SharedPtr<Mesh>> GetMeshBatch() const;

        uint32 GetMeshBatchSize() const;

        uint32 GetInstanceBatchCount(GPURenderPassID id, uint32 subpass) const;

        ea::span<const GPURenderPass* const> GetGPURenderPasses() const;

        uint32 GetMeshIndex(MeshID id) const;

        uint32 GetSubpassLocalMaterialIndex(MaterialID id) const;

        uint32 GetHeapIndex(MaterialID id) const;

        SharedPtr<Image2D> CreateTexture2D(const sf::Image& sfImage);

        SharedPtr<ImageCube> CreateImageCube(uint32 size, GPUFormat format, GPUImageUsageFlags usageFlags);

        SharedPtr<ImageCubeArray> CreateImageCubeArray(uint32 size, GPUFormat format, uint32 cubeCount, GPUImageUsageFlags usageFlags);

        SharedPtr<Image2D> CreateImage2D(uint32 width, uint32 height, GPUFormat format, GPUImageUsageFlags usageFlags);

        template <typename T>
        SharedPtr<Material> CreateMaterial(const GPUShader* shader);

    private:

        struct SubpassData
        {
            ea::vector<SharedPtr<Material>> materials;
            uint32 version;
        };

        struct RenderPassData
        {
            ea::vector<SubpassData> subpasses;
        };

        struct ModelData
        {
            ea::vector<MatMeshLocalIndex> localIndices;
            uint32 localIndicesVersion;
        };

        struct MaterialData
        {
            uint32 subpassLocalIndex;
            uint32 heapIndex;
        };

        void AssertNotNull() const;

        void RegisterGPURenderPass(const GPURenderPass* renderPass);

        SubpassData& GetSubpassDataMutable(GPURenderPassID id, uint32 subpass);

        const SubpassData& GetSubpassData(GPURenderPassID id, uint32 subpass) const;

        GPU* m_gpu = {};
        uint32 m_materialHeapSize = {};
        ea::hash_map<GPURenderPassID, RenderPassData> m_rpassDatas = {};
        ea::hash_map<MeshID, uint32> m_meshIndices = {};
        ea::hash_map<MaterialID, MaterialData> m_materiaDatas = {};
        ea::hash_map<ModelID, ModelData> m_modelDatas = {};
        ea::vector<SharedPtr<Mesh>> m_meshes = {};
        ea::vector<const GPURenderPass*> m_renderPasses = {};

        SharedPtr<Mesh> m_screenMesh = {};
        SharedPtr<Mesh> m_cubeMesh = {};
        UniqueGPURenderPassPtr m_gbufferPass = {};
        UniqueGPURenderPassPtr m_sunShadowsPass = {};
        UniqueGPURenderPassPtr m_tileCullingPass = {};
        UniqueGPURenderPassPtr m_ssaoPass = {};
        UniqueGPURenderPassPtr m_blurPass = {};
        UniqueGPURenderPassPtr m_lightingPass = {};
        UniqueGPURenderPassPtr m_presentPass = {};
        UniqueGPUShaderPtr m_gbufferShader = {};
        UniqueGPUShaderPtr m_sunShadowShader = {};
        UniqueGPUShaderPtr m_tileCullingProcess = {};
        UniqueGPUShaderPtr m_ssaoProcess = {};
        UniqueGPUShaderPtr m_blurProcess = {};
        UniqueGPUShaderPtr m_deferredLighting = {};
        UniqueGPUShaderPtr m_presentShader = {};
        SharedPtr<Image2D> m_whiteTexture = {};
        SharedPtr<Image2D> m_blackTexture = {};
        SharedPtr<Image2D> m_normalTexture = {};
        SharedPtr<Image2D> m_defaultMaskMap = {};
        SharedPtr<Image2D> m_ssaoNoise = {};
        SharedPtr<Material> m_whiteMaterial = {};
        SharedPtr<Model> m_whiteCube = {};
        SharedPtr<const Material> m_sunShadowMaterial = {};
    };

    template <typename T>
    SharedPtr<Material> RenderAssetManager::CreateMaterial(const GPUShader* shader)
    {
        GYN_STATIC_ASSERT_TRIVIALLY_COPYABLE(T);
        return CreateMaterial(shader, GetTypeID<T>(), sizeof(T));
    }
}