#pragma once

#include <gyn/common.hpp>

#define GYN_GPU_VALIDATION_LAYER GYN_DEBUG

namespace Gyn
{
    enum class GPUBackend
    {
        eNull,

        eVulkan,

        eEnd
    };
    constexpr int GPUBackendCount = EnumCast(GPUBackend::eEnd);

    enum class GPUFormat
    {
        eNull,

        eUNorm8x1, // Unsigned normalized float (8 bits)
        eUNorm8x4, // Unsigned normalized float4 (8 bits per channel)
        eRGBA8 = eUNorm8x4,

        eSNorm8x4, // Signed normalized float4 (8 bits per channel)

        eUNorm16x4, // Unsigned normalized float4 (16 bits per channel)
        eRGBA16 = eUNorm16x4,

        eSNorm16x4, // Signed normalized float4 (16 bits per channel)
        eSFloat16x4, // Signed float4 (16 bits per channel)

        ePresent,
        eDepth24S8,
        eDepth32,

        eUint16,

        eEnd
    };
    constexpr int GPUFormatCount = EnumCast(GPUFormat::eEnd);

    enum class GPUMemoryType
    {
        eNull,

        eCPUVisible,
        eGPULocal,

        eEnd
    };
    constexpr int GPUMemoryTypeCount = EnumCast(GPUMemoryType::eEnd);

    enum class GPUImageType
    {
        eNull,

        e2D,
        eCube,

        eEnd
    };
    constexpr int GPUImageTypeCount = EnumCast(GPUImageType::eEnd);

    enum class GPUImageUsageFlags
    {
        eNull = 0,

        eSample = 1 << 0,
        eRender = 1 << 1,
        eTransfer = 1 << 2,

        eRenderAndSample = eSample | eRender
    };
    GYN_ENUM_CLASS_FLAG_MANIPS(GPUImageUsageFlags);

    enum class GPUImageViewType
    {
        eNull,

        e2D,
        eCube,
        eCubeArray,

        eEnd
    };
    constexpr int GPUImageViewTypeCount = EnumCast(GPUImageViewType::eEnd);

    enum class GPUImageComponent
    {
        eNull,

        eColor,
        eDepth,
        eStencil,
        eDepthStencil,

        eEnd
    };
    constexpr int GPUImageViewComponentCount = EnumCast(GPUImageComponent::eEnd);

    enum class GPUSamplerFiltering
    {
        eLinear,

        eEnd
    };
    constexpr int GPUSamplerFilteringCount = EnumCast(GPUSamplerFiltering::eEnd);

    enum class GPUSamplerAddressing
    {
        eRepeat,
        eMirroredRepeat,
        eClampToEdge,

        eEnd
    };
    constexpr int GPUSamplerAddressingCount = EnumCast(GPUSamplerAddressing::eEnd);

    enum class GPUBufferUsage
    {
        eNull,

        eVertexBuffer,
        eIndexBuffer,
        eUniformBuffer,
        eSSBO,

        eEnd
    };
    constexpr int GPUBufferUsageCount = EnumCast(GPUBufferUsage::eEnd);

    enum class GPUMemoryAllocation
    {
        eNull,

        eZero,
        eUninitialized,

        eEnd
    };
    constexpr int GPUMemoryAllocationCount = EnumCast(GPUMemoryAllocation::eEnd);

    enum class GPURenderPassID : uint64 {};
    constexpr GPURenderPassID GPURenderPassNullID = GPURenderPassID(0);

    struct UniqueGPUFencePtrDestroyer
    {
        void operator()(GPUFence* fence) const;
    };
    using UniqueGPUFencePtr = UniquePtr<GPUFence, UniqueGPUFencePtrDestroyer>;

    struct UniqueGPURenderPassPtrDestroyer
    {
        void operator()(GPURenderPass* renderPass) const;
    };
    using UniqueGPURenderPassPtr = UniquePtr<GPURenderPass, UniqueGPURenderPassPtrDestroyer>;

    struct UniqueGPUFramebufferPtrDestroyer
    {
        void operator()(GPUFramebuffer* framebuffer) const;
    };
    using UniqueGPUFramebufferPtr = UniquePtr<GPUFramebuffer, UniqueGPUFramebufferPtrDestroyer>;

    struct UniqueGPUShaderPtrDestroyer
    {
        void operator()(GPUShader* shader) const;
    };
    using UniqueGPUShaderPtr = UniquePtr<GPUShader, UniqueGPUShaderPtrDestroyer>;

    struct UniqueGPUBindSetPtrDestroyer
    {
        void operator()(GPUBindSet* bindSet) const;
    };
    using UniqueGPUBindSetPtr = UniquePtr<GPUBindSet, UniqueGPUBindSetPtrDestroyer>;

    struct UniqueGPUBufferPtrDestroyer
    {
        void operator()(GPUBuffer* buffer) const;
    };
    using UniqueGPUBufferPtr = UniquePtr<GPUBuffer, UniqueGPUBufferPtrDestroyer>;

    struct UniqueGPUImagePtrDestroyer
    {
        void operator()(GPUImage* image) const;
    };
    using UniqueGPUImagePtr = UniquePtr<GPUImage, UniqueGPUImagePtrDestroyer>;

    struct UniqueGPUImageViewPtrDestroyer
    {
        void operator()(GPUImageView* imageView) const;
    };
    using UniqueGPUImageViewPtr = UniquePtr<GPUImageView, UniqueGPUImageViewPtrDestroyer>;

    struct GPUProperties
    {
        ea::string deviceName;
    };

    // TODO: Remove this and just use SSBOs to store vertex data
    struct Vertex
    {
        float3 position;
        float3 normal;
        float2 uv;
        float3 tangent;
    };

    constexpr uint32 NullSubpassRef = Uint32Max;
    struct SubpassLayout
    {
        ea::vector<uint32> colorBufferRefs;
        uint32 depthStencilRef;
        ea::vector<uint32> supassInputRefs;
    };

    struct ShaderOptions
    {
        bool backFaceCulling;
    };

    struct GPUCommandBuffer
    {
        // Size N
        ea::vector<const GPUShader*> shaders;
        ea::vector<const GPUBindSet*> bindSets3;

        // Size M
        ea::vector<const GPUBuffer*> vertexBuffers;
        ea::vector<const GPUBuffer*> indexBuffers;

        // Size N x M
        ea::vector<uint32> firstInstances;
        ea::vector<uint32> instanceCounts;
    };

    struct GPUSupbassExecution
    {
        const GPUBindSet* bindSet2;
        GPUCommandBuffer commandBuffer;
    };

    struct GPURenderPassExecution
    {
        const GPUBindSet* bindSet1;
        const GPUFramebuffer* framebuffer;
        ea::vector<GPUSupbassExecution> subpasses;
    };

    // Just hardcode the render pipeline for now.
    // TODO: Replace all that stuff with a WorkGraph-based abstraction. 
    struct GPUFrameExecution
    {
        uint32 frameIndex;
        uint32 blurPassCount;
        const GPUBindSet* bindSet0;

        GPURenderPassExecution gbufferPass;
        GPURenderPassExecution sunShadowPass;
        GPURenderPassExecution tileCullingPass;
        GPURenderPassExecution ssaoPass;
        GPURenderPassExecution firstBlurPass0;
        GPURenderPassExecution firstBlurPass1;
        GPURenderPassExecution blurPass0;
        GPURenderPassExecution blurPass1;
        GPURenderPassExecution lightingPass;
        GPURenderPassExecution presentPass;
        ea::vector<GPURenderPassExecution> shadowPasses;

        const GPUImage* shadowCubeArray;
        const GPUImage* depthBuffer;
        const GPUImage* normalBuffer;
        const GPUImage* emissiveBuffer;
        const GPUBuffer* tileCullingBuffer;
        const GPUImage* tileCullingTexture;
        const GPUImage* blurBuffer0;
        const GPUImage* blurBuffer1;
        const GPUImage* lightingOutput;
    };

    class GPUFence : public NoCopyMove
    {
    public:

        virtual ~GPUFence() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual void Wait() const = 0;

        virtual bool IsSignaled() const = 0;
    };

    class GPU : public NoCopyMove
    {
    public:

        virtual ~GPU() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPUProperties GetProperties() const = 0;

        virtual GPUSwapchain* GetSwapchain() = 0;

        virtual const GPURenderPass& GetPresentRenderPass() const = 0;

        virtual UniqueGPUShaderPtr LoadShaderFromFile(const char* path, const GPURenderPass& renderPassLayout, uint32 subpass, const ShaderOptions& options) = 0;

        virtual void DestroyShader(GPUShader* shader) = 0;

        virtual UniqueGPURenderPassPtr CreateRenderPass(ea::span<const GPUFormat> formats, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks = {}) = 0;

        virtual UniqueGPURenderPassPtr CreateRenderPass(ea::span<const GPUFormat> colorFormats, bool hasDepthStencil) = 0;

        virtual void DestroyRenderPass(GPURenderPass* renderPass) = 0;

        virtual UniqueGPUBindSetPtr CreateBindSet(const GPUShader& shader, uint32 set) = 0;

        virtual void DestroyBindSet(GPUBindSet* bindSet) = 0;

        virtual UniqueGPUFramebufferPtr CreateFramebuffer(const GPURenderPass* renderPassLayout, ea::span<const GPUImageView* const> attachments) = 0;

        virtual void DestroyFramebuffer(GPUFramebuffer* framebuffer) = 0;

        virtual UniqueGPUBufferPtr CreateBuffer(uint64 allocSize, GPUBufferUsage usage, GPUMemoryType memoryType, GPUMemoryAllocation allocation = GPUMemoryAllocation::eZero) = 0;

        virtual void DestroyBuffer(GPUBuffer* buffer) = 0;

        virtual UniqueGPUImagePtr CreateImage(uint32 width, uint32 height, uint32 layerCount, GPUFormat format, GPUImageType type, GPUImageUsageFlags usage) = 0;

        virtual void DestroyImage(GPUImage* image) = 0;

        virtual UniqueGPUImageViewPtr CreateImageView(const GPUImage& image, GPUImageViewType viewType, GPUImageComponent component, uint32 firstLayer, uint32 layerCount) = 0;

        virtual void DestroyImageView(GPUImageView* imageView) = 0;

        virtual ea::span<const Vertex> GetScreenQuadVertices() = 0;

        virtual ea::span<const uint16> GetScreenQuadIndices() = 0;

        virtual UniqueGPUFencePtr Submit(const GPUFrameExecution& frameExecution) = 0;

        virtual void DestroyFence(GPUFence* fence) = 0;

        virtual void Flush() = 0;

        // TODO: Remove ImGui stuff from the backend
        virtual void ImGuiInit() = 0;

        virtual void ImGuiNewFrame() = 0;

        virtual void ImGuiImage(const GPUImageView* imageView,
            float2 size,
            float2 uv0 = float2(0, 0),
            float2 uv1 = float2(1, 1),
            float4 tint_col = float4(1, 1, 1, 1),
            float4 border_col = float4(0, 0, 0, 0)) = 0;

        virtual void ImGUIUninit() = 0;


        UniqueGPUBufferPtr CreateBuffer(ea::span<const byte> data, GPUBufferUsage usageFlags, GPUMemoryType memoryType);
    };

    class ShaderBinding
    {
    public:

        virtual ~ShaderBinding() = default;
    };

    class GPUBuffer : public NoCopyMove, public ShaderBinding
    {
    public:

        virtual ~GPUBuffer() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual bool IsGPULocal() const = 0;

        virtual void ZeroMemory() = 0;

        virtual void Upload(ea::span<const byte> data, uint32 byteDstOffset = 0) = 0;

        template <typename T>
        void Upload(ea::span<const T> data, uint32 elementDstOffset = 0);

        virtual uint64 GetAllocSize() const = 0;

        virtual GPUBufferUsage GetUsage() const = 0;
    };

    class GPUImage : public NoCopyMove
    {
    public:

        virtual ~GPUImage() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual GPUImageType GetType() const = 0;

        virtual GPUImageUsageFlags GetUsageFlags() const = 0;

        virtual GPUMemoryType GetMemoryType() const = 0;

        virtual GPUFormat GetFormat() const = 0;

        virtual uint2 GetExtent() const = 0;

        virtual void Upload(ea::span<const byte> bytes) = 0;
    };

    class GPUImageView : public NoCopyMove
    {
    public:

        virtual ~GPUImageView() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual const GPUImage& GetImage() const = 0;

        virtual GPUImageViewType GetType() const = 0;

        virtual uint2 GetExtent() const = 0;
    };

    // TODO: Remove when the WorkGraph-based abstraction is implemented
    class GPURenderPass : public NoCopyMove
    {
    public:

        virtual ~GPURenderPass() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual GPURenderPassID GetID() const = 0;

        virtual uint32 GetColorAttachmentCount(uint32 subpass) const = 0;

        virtual ea::span<const GPUFormat> GetAttachments() const = 0;

        virtual ea::span<const SubpassLayout> GetSubpasses() const = 0;
    };

    // TODO: Remove when the WorkGraph-based abstraction is implemented
    class GPUFramebuffer : public NoCopyMove
    {
    public:

        virtual ~GPUFramebuffer() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual const GPURenderPass& GetRenderPass() const = 0;

        virtual ea::span<const GPUImageView* const> GetAttachments() const = 0;
    };

    class GPUShader : public NoCopyMove
    {
    public:

        virtual ~GPUShader() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual const GPURenderPass& GetRenderPass() const = 0;

        virtual uint32 GetSubpass() const = 0;
    };

    class GPUBindSet : public NoCopyMove
    {
    public:

        virtual ~GPUBindSet() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        // TODO : BindSet should not store a particular Shader. Remove this when we have GPUBindSetLayout
        virtual const GPUShader& GetShader() = 0;

        virtual uint32 GetSetIndex() const = 0;

        virtual void SetBinding(const GPUImageView* imageView, uint32 binding, GPUSamplerFiltering filtering, GPUSamplerAddressing addressing) = 0;

        virtual void SetBinding(const GPUBuffer* buffer, uint32 binding) = 0;
    };

    class GPUSwapchain : public NoCopyMove
    {
    public:

        virtual ~GPUSwapchain() = default;

        virtual bool IsNull() const = 0;

        virtual GPUBackend GetGPUBackend() const = 0;

        virtual GPU& GetGPU() const = 0;

        virtual GPUFormat GetFormat() const = 0;

        virtual uint2 GetExtent() const = 0;

        virtual uint32 AcquireImage() = 0;

        virtual uint32 GetImageCount() const = 0;

        virtual const GPUImageView* GetView(uint32 index) const = 0;

        virtual void Present(const GPUFence* waitFence) = 0;
    };

    bool IsDepthStencilFormat(GPUFormat format);

    bool IsDepthFormat(GPUFormat format);

    bool IsStencilFormat(GPUFormat format);

    bool IsColorFormat(GPUFormat format);

    template <typename T>
    void GPUBuffer::Upload(ea::span<const T> data, uint32 elementDstOffset)
    {
        Upload(ToByteSpan(data), elementDstOffset * sizeof(T));
    }
}