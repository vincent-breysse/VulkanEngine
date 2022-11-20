#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    // TODO: Remove when the WorkGraph-based abstraction is implemented
    class GPUVulkanRenderPass : public GPURenderPass
    {
    public:

        GPUVulkanRenderPass();

        GPUVulkanRenderPass(GPUVulkan& gpu, ea::span<const GPUFormat> attachments, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks);

        GPUVulkanRenderPass(GPUVulkan& gpu, ea::span<const GPUFormat> colorAttachments, bool hasDepthStencil);

        ~GPUVulkanRenderPass();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        GPURenderPassID GetID() const override;

        uint32 GetColorAttachmentCount(uint32 subpass) const override;

        ea::span<const GPUFormat> GetAttachments() const override;

        ea::span<const SubpassLayout> GetSubpasses() const override;

        const VexRenderPass& GetVexRenderPass() const;

        VkRenderPass GetHandle() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        GPURenderPassID m_id;
        const VexRenderPass* m_renderPass;
        ea::vector<GPUFormat> m_attachments;
        ea::vector<SubpassLayout> m_subpasses;
    };

    inline const GPUVulkanRenderPass& CastVulkan(const GPURenderPass& renderPass)
    {
        ASSERT(renderPass.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanRenderPass&>(renderPass);
    }

    inline GPUVulkanRenderPass& CastVulkan(GPURenderPass& renderPass)
    {
        ASSERT(renderPass.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanRenderPass&>(renderPass);
    }
}