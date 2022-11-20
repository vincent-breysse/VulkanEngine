
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/fence.hpp>

GPUVulkanFence::GPUVulkanFence()
{
    m_gpu = nullptr;
}

GPUVulkanFence::GPUVulkanFence(GPUVulkan& gpu, ea::span<const VkFence> fences, ea::span<const VkSemaphore> semaphores) : GPUVulkanFence()
{
    for (VkFence fence : fences)
    {
        MaybeUnused(fence);
        ASSERT(fence != VK_NULL_HANDLE);
    }

    for (VkSemaphore semaphore : semaphores)
    {
        MaybeUnused(semaphore);
        ASSERT(semaphore != VK_NULL_HANDLE);
    }

    m_gpu = &gpu;
    m_fences = CopyToVector(fences);
    m_semaphores = CopyToVector(semaphores);
}

GPUVulkanFence::~GPUVulkanFence()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();

    for (VkFence fence : m_fences)
    {
        device.DestroyFence(fence);
    }

    for (VkSemaphore semaphore : m_semaphores)
    {
        device.DestroySemaphore(semaphore);
    }
}

bool GPUVulkanFence::IsNull() const
{
    return !m_gpu;
}

GPUBackend GPUVulkanFence::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanFence::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

void GPUVulkanFence::Wait() const
{
    AssertNotNull();
    VexDevice& device = m_gpu->GetVexDevice();

    VkResult result = device.WaitForFences(ToU32(m_fences.size()), m_fences.data(), true, Uint64Max);
    MaybeUnused(result);
    ASSERT(result == VK_SUCCESS);
}

bool GPUVulkanFence::IsSignaled() const
{
    AssertNotNull();
    VexDevice& device = m_gpu->GetVexDevice();

    for (auto& fence : m_fences)
    {
        if (device.GetFenceStatus(fence) != VK_SUCCESS)
        {
            return false;
        }
    }

    return true;
}

ea::span<const VkFence> GPUVulkanFence::GetFences() const
{
    AssertNotNull();
    return m_fences;
}

ea::span<const VkSemaphore> GPUVulkanFence::GetSemaphores() const
{
    AssertNotNull();
    return m_semaphores;
}

void GPUVulkanFence::AssertNotNull() const
{
    ASSERT(!IsNull());
}