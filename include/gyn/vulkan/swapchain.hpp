#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkanSwapchain : public GPUSwapchain
    {
    public:

        GPUVulkanSwapchain();

        GPUVulkanSwapchain(GPUVulkan& gpu);

        ~GPUVulkanSwapchain();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        GPUFormat GetFormat() const override;

        uint2 GetExtent() const override;

        uint32 AcquireImage() override;

        uint32 GetImageCount() const override;

        const GPUImageView* GetView(uint32 index) const override;

        void Present(const GPUFence* waitFence) override;

        const VexSwapchainKHR& GetVexSwapchain() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        const VexSwapchainKHR* m_swapchain;
        ea::vector<UniqueGPUImagePtr> m_image;
        ea::vector<UniqueGPUImageViewPtr> m_imageViews;
        uint32 m_currentID;
    };

    inline const GPUVulkanSwapchain* CastVulkan(const GPUSwapchain* swapchain)
    {
        ASSERT(swapchain->GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanSwapchain*>(swapchain);
    }

    inline GPUVulkanSwapchain* CastVulkan(GPUSwapchain* swapchain)
    {
        ASSERT(swapchain->GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanSwapchain*>(swapchain);
    }
}