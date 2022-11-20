#pragma once

#include <gyn/common.hpp>
#include <gyn/gpu.hpp>
#include <gyn/graphics/fwd.hpp>

namespace Gyn
{
    constexpr uint32 MaxPointLightCount = 256;
    constexpr uint32 MaxPointLightCastingShadows = 64;
    constexpr uint32 SSAOKernelSize = 64;
    constexpr uint32 SSAONoiseSize = 4;

    struct MaterialNoUniforms {};

    enum class MeshID : uint64 {};
    constexpr MeshID MeshNullID = MeshID(0);

    enum class MaterialID : uint64 {};
    constexpr MaterialID MaterialNullID = MaterialID(0);

    enum class ModelID : uint64 {};
    constexpr ModelID ModelNullID = ModelID(0);

    struct MatMeshID
    {
        MaterialID material;
        MeshID mesh;
    };

    struct MatMeshLocalIndex
    {
        uint32 material;
        uint32 mesh;
    };

    struct LitMaterialUniforms
    {
        alignas(16) float4 color;
        alignas(16) float4 emissiveColor;
        alignas(8) float2 uvScale;
        float roughness;
        float metallic;
        float normal;
        float ao;
        int isUnlit;
    };

    struct ShadowMatrixUniforms
    {
        alignas(16) Matrix4x4 right;
        alignas(16) Matrix4x4 left;
        alignas(16) Matrix4x4 up;
        alignas(16) Matrix4x4 down;
        alignas(16) Matrix4x4 front;
        alignas(16) Matrix4x4 back;
    };

    struct ShadowParamUniforms
    {
        float near;
        float far;
        float pad0;
        float pad1;
    };

    struct ShadowUniforms
    {
        alignas(16) ea::array<ShadowMatrixUniforms, MaxPointLightCastingShadows> matrices;
        alignas(16) ea::array<ShadowParamUniforms, MaxPointLightCastingShadows> params;
    };

    struct SSAOUniforms
    {
        alignas(16) ea::array<float4, SSAOKernelSize> kernel;
        float radius;
        float strength;
    };

    struct FrustrumPlanesSOA
    {
        vfloat4 px0;
        vfloat4 px1;

        vfloat4 py0;
        vfloat4 py1;

        vfloat4 pz0;
        vfloat4 pz1;

        vfloat4 pw0;
        vfloat4 pw1;
    };

    struct SceneStats
    {
        uint32 materialCount;
        uint32 texture2DCount;
        uint32 meshCount;
        uint32 modelCount;
        uint32 pointLightCount;
        uint32 rendererCount;
        uint32 visibleRendererCount;
        uint32 pvsCount;
        uint32 visiblePointLightCount;
        uint32 shadowCasterCount;
        uint32 visibleShadowCasterCount;
    };

    struct GlobalData
    {
        TransformRecord transformRecord;

        ea::array<float4, SSAOKernelSize> ssaoKernel;
        float3 ambient;
        float ambientIntensity;
        float3 sunLightEulerAngles;
        float3 sunLightColor;
        float sunLightIntensity;

        float3 colorFilter;
        float colorFilterIntensity;
        float3 bloomColor;
        float bloomIntensity;
        int32 bloomBlurPass;
        float contrast;
        float saturation;
        float exposure;
        float ssaoRadius;
        float ssaoStrength;
        float4 debugFloat4;

        bool useAO;
        int debugBuffer;
        bool useEmissive;
        bool useBloom;
        bool usePVS;
        bool castShadows;
        bool threadedCulling;
        bool isGameViewFocused;

        SceneStats stats;
    };

    struct AllImageViews
    {
        UniqueGPUImageViewPtr color;
        UniqueGPUImageViewPtr depthStencil;
        UniqueGPUImageViewPtr depth;
        UniqueGPUImageViewPtr stencil;
    };

    struct DisableShadow : public ECSComponent
    {
        DisableShadow() = default;

        DisableShadow(bool value) : val(value) {}

        bool val;
    };

    struct EntityGPUBufferIndex : public ECSComponent
    {
        EntityGPUBufferIndex() = default;

        EntityGPUBufferIndex(const uint32& index) : val(index) {}

        uint32 val;
    };

    struct EntityModelID : public ECSComponent
    {
        EntityModelID() = default;

        EntityModelID(const ModelID& id) : val(id) {}

        ModelID val;
    };

    ea::array<float4, SSAOKernelSize> CreateSSAOKernel();

    AllImageViews CreateAllAvailableImageViews(GPU& gpu, const GPUImage& image, GPUImageViewType viewType, uint32 firstLayer, uint32 layerCount);

    bool ImageFormatContainsComponent(GPUFormat format, GPUImageComponent component);

    const GPUImageView* PickImageViewFromComponent(const AllImageViews& views, GPUImageComponent component);

    bool operator==(const MatMeshLocalIndex& a, const MatMeshLocalIndex& b);

    bool operator!=(const MatMeshLocalIndex& a, const MatMeshLocalIndex& b);

    bool operator==(const MatMeshID& a, const MatMeshID& b);

    bool operator!=(const MatMeshID& a, const MatMeshID& b);
}

namespace eastl
{
    template <> struct hash<Gyn::MatMeshLocalIndex>
    {
        size_t operator()(Gyn::MatMeshLocalIndex val) const
        {
            size_t a = size_t(val.material);
            size_t b = size_t(val.mesh);
            return (a << 32) ^ b;
        }
    };

    template <> struct hash<Gyn::MatMeshID>
    {
        size_t operator()(Gyn::MatMeshID val) const
        {
            size_t a = size_t(val.material);
            size_t b = size_t(val.mesh);
            return (a << 32) ^ b;
        }
    };
}