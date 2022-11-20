#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkanBuffer : public GPUBuffer
    {
    public:

        GPUVulkanBuffer();

        GPUVulkanBuffer(GPUVulkan& gpu, uint64 allocSize, GPUBufferUsage usage, GPUMemoryType memoryType, GPUMemoryAllocation allocation);

        ~GPUVulkanBuffer();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        bool IsGPULocal() const override;

        void ZeroMemory() override;

        void Upload(ea::span<const byte> data, uint32 byteDstOffset = 0) override;

        using GPUBuffer::Upload;

        uint64 GetAllocSize() const override;

        GPUBufferUsage GetUsage() const override;

        const VexBuffer& GetVexBuffer() const;

        VkBuffer GetHandle() const;

    private:

        void AssertNotNull() const;

        void UploadCPUVisible(ea::span<const byte> data, uint32 byteDstOffset);

        void ZeroMemoryCPUVisible();

        VkBuffer CreateStagingBuffer(VkDeviceSize allocSize, VkDeviceMemory* outMemory, byte** outMap);

        void FlushStagingBuffer(const VexBuffer& stagingBuffer, uint32 byteDstOffset);

        void UploadGPULocal(ea::span<const byte> data, uint32 byteDstOffset);

        void ZeroMemoryGPULocal();

        GPUVulkan* m_gpu;
        VkDeviceMemory m_memory;
        VkDeviceSize m_allocSize;
        VkMemoryPropertyFlags m_memoryFlags;
        GPUBufferUsage m_usage;
        const VexBuffer* m_buffer;
        byte* m_map;
    };

    inline const GPUVulkanBuffer& CastVulkan(const GPUBuffer& buffer)
    {
        ASSERT(buffer.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanBuffer&>(buffer);
    }

    inline GPUVulkanBuffer& CastVulkan(GPUBuffer& buffer)
    {
        ASSERT(buffer.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanBuffer&>(buffer);
    }
}