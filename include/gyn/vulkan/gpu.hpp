#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkan : public GPU
    {
    public:

        GPUVulkan();

        GPUVulkan(Window& window, JobScheduler& jobScheduler);

        ~GPUVulkan();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPUProperties GetProperties() const override;

        GPUSwapchain* GetSwapchain() override;

        const GPURenderPass& GetPresentRenderPass() const override;

        UniqueGPUShaderPtr LoadShaderFromFile(const char* path, const GPURenderPass& renderPassLayout, uint32 subpass, const ShaderOptions& options) override;

        void DestroyShader(GPUShader* shader) override;

        UniqueGPURenderPassPtr CreateRenderPass(ea::span<const GPUFormat> formats, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks) override;

        UniqueGPURenderPassPtr CreateRenderPass(ea::span<const GPUFormat> colorFormats, bool hasDepthStencil) override;

        void DestroyRenderPass(GPURenderPass* renderPass) override;

        UniqueGPUBindSetPtr CreateBindSet(const GPUShader& shader, uint32 set) override;

        void DestroyBindSet(GPUBindSet* bindSet) override;

        UniqueGPUBufferPtr CreateBuffer(uint64 allocSize, GPUBufferUsage usage, GPUMemoryType memoryType, GPUMemoryAllocation allocation) override;

        void DestroyBuffer(GPUBuffer* buffer) override;

        UniqueGPUImagePtr CreateImage(uint32 width, uint32 height, uint32 layerCount, GPUFormat format, GPUImageType type, GPUImageUsageFlags usage) override;

        void DestroyImage(GPUImage* image) override;

        UniqueGPUImageViewPtr CreateImageView(const GPUImage& image, GPUImageViewType viewType, GPUImageComponent component, uint32 firstLayer, uint32 layerCount) override;

        void DestroyImageView(GPUImageView* imageView) override;

        UniqueGPUFramebufferPtr CreateFramebuffer(const GPURenderPass* renderPassLayout, ea::span<const GPUImageView* const> attachments) override;

        void DestroyFramebuffer(GPUFramebuffer* framebuffer) override;

        ea::span<const Vertex> GetScreenQuadVertices() override;

        ea::span<const uint16> GetScreenQuadIndices() override;

        UniqueGPUFencePtr Submit(const GPUFrameExecution& frameExecution) override;

        void DestroyFence(GPUFence* fence) override;

        void Flush() override;

        // TODO: Remove ImGui stuff from the backend
        void ImGuiInit() override;

        void ImGuiNewFrame() override;

        void ImGuiImage(const GPUImageView* imageView, 
            float2 size, 
            float2 uv0 = float2(0, 0), 
            float2 uv1 = float2(1, 1), 
            float4 tint_col = float4(1, 1, 1, 1), 
            float4 border_col = float4(0, 0, 0, 0)) override;

        void ImGUIUninit() override;

        Window& GetWindow();

        VexDevice& GetVexDevice();

        VkCommandPool GetCommandPool();

        VkCommandPool GetWorkerCommandPool(uint32 workerIndex);

        VkDescriptorPool GetDescriptorPool();

        VkSampler GetSampler(GPUSamplerFiltering filtering, GPUSamplerAddressing addressing) const;

        VkFormat ToVkFormat(GPUFormat format) const;

        UniqueGPUImagePtr CreateImage(VkSwapchainKHR swapchain, const VexImage& vexImage);

        VkDescriptorSet GetDummyBindSet() const;

        UniqueGPUFencePtr CreateFence(ea::span<const VkFence> fences, ea::span<const VkSemaphore> semaphores);

        UniqueGPUFencePtr CreateFence(VkFence fence, VkSemaphore semaphore);

        // TODO: Remove ImGui stuff from the backend
        void ImGuiRecordRendering(VkCommandBuffer cmdBuffer);

    private:

        void AssertNotNull() const;

        JobScheduler* m_jobScheduler;
        Window* m_window;
        VexDevice* m_device;
        VkCommandPool m_commandPool;
        ea::vector<VkCommandPool> m_workerCommandPools;
        VkDescriptorPool m_descriptorPool;
        VkSampler m_linearRepeatSampler;
        VkSampler m_linearMirroredRepeatSampler;
        VkSampler m_linearClampToEdgeSampler;
        VkDescriptorSetLayout m_dummyBindSetLayout;
        VkDescriptorSet m_dummyBindSet;
        ea::vector<ea::vector<VkCommandBuffer>> m_cmdBuffers;
        UniqueGPURenderPassPtr m_presentPass;
        UniquePtr<GPUVulkanSwapchain> m_swapchain;
    };

    inline const GPUVulkan& CastVulkan(const GPU& gpu)
    {
        ASSERT(gpu.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkan&>(gpu);
    }

    inline GPUVulkan& CastVulkan(GPU& gpu)
    {
        ASSERT(gpu.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkan&>(gpu);
    }
}