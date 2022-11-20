#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    // TODO: Remove when the WorkGraph-based abstraction is implemented
    class GPUVulkanFramebuffer : public GPUFramebuffer
    {
    public:

        GPUVulkanFramebuffer();

        GPUVulkanFramebuffer(GPUVulkan& gpu, const GPUVulkanRenderPass* renderPassLayout, ea::vector<const GPUImageView*>&& attachments);

        ~GPUVulkanFramebuffer();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        const GPURenderPass& GetRenderPass() const override;

        ea::span<const GPUImageView* const> GetAttachments() const override;

        const VexFramebuffer& GetVexFramebuffer() const;

        VkFramebuffer GetHandle() const;

        const VkViewport& GetViewport() const;

        const VkRect2D& GetRenderArea() const;

        ea::span<const VkClearValue> GetClearValues() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        const GPURenderPass* m_renderPassLayout;
        VkViewport m_viewport;
        VkRect2D m_renderArea;
        VkExtent2D m_extent;
        ea::vector<VkClearValue> m_clearValues;
        ea::vector<const GPUImageView*> m_attachments;
        const VexFramebuffer* m_framebuffer;
    };

    inline const GPUVulkanFramebuffer& CastVulkan(const GPUFramebuffer& framebuffer)
    {
        ASSERT(framebuffer.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanFramebuffer&>(framebuffer);
    }

    inline GPUVulkanFramebuffer& CastVulkan(GPUFramebuffer& framebuffer)
    {
        ASSERT(framebuffer.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanFramebuffer&>(framebuffer);
    }
}