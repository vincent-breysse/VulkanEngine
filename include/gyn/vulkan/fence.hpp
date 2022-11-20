#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkanFence : public GPUFence
    {
    public:

        GPUVulkanFence();

        GPUVulkanFence(GPUVulkan& gpu, ea::span<const VkFence> fences, ea::span<const VkSemaphore> semaphores);

        ~GPUVulkanFence();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        void Wait() const override;

        bool IsSignaled() const override;

        ea::span<const VkFence> GetFences() const;

        ea::span<const VkSemaphore> GetSemaphores() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        ea::vector<VkFence> m_fences;
        ea::vector<VkSemaphore> m_semaphores;
    };

    inline const GPUVulkanFence& CastVulkan(const GPUFence& fence)
    {
        ASSERT(fence.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanFence&>(fence);
    }

    inline GPUVulkanFence& CastVulkan(GPUFence& fence)
    {
        ASSERT(fence.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanFence&>(fence);
    }
}