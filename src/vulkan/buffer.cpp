
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/buffer.hpp>

static VkMemoryPropertyFlags ToVkMemoryFlags(GPUMemoryType type)
{
    ASSERT(type != GPUMemoryType::eNull);

    static ea::array Table
    {
        0,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    };

    return Table[EnumCast(type)];
}

static VkBufferUsageFlags ToVkBufferUsageFlags(GPUBufferUsage usage)
{
    ASSERT(usage != GPUBufferUsage::eNull);

    static ea::array Table
    {
        0,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
    };

    return Table[EnumCast(usage)];
}

GPUVulkanBuffer::GPUVulkanBuffer()
{
    m_gpu = nullptr;
    m_memory = VK_NULL_HANDLE;
    m_allocSize = 0;
    m_memoryFlags = 0;
    m_usage = GPUBufferUsage::eNull;
    m_buffer = nullptr;
    m_map = nullptr;
}

GPUVulkanBuffer::GPUVulkanBuffer(GPUVulkan& gpu, uint64 allocSize, GPUBufferUsage usage, GPUMemoryType memoryType, GPUMemoryAllocation allocation) : GPUVulkanBuffer()
{
    ASSERT(allocSize > 0);
    ASSERT(memoryType != GPUMemoryType::eNull);
    ASSERT(usage != GPUBufferUsage::eNull);
    ASSERT(allocation != GPUMemoryAllocation::eNull);

    VexDevice& device = gpu.GetVexDevice();

    VkBufferUsageFlags usageFlags = ToVkBufferUsageFlags(usage);

    const bool isGPUVulkanLocal = memoryType == GPUMemoryType::eGPULocal;
    if (isGPUVulkanLocal)
    {
        usageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    }

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = allocSize;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkMemoryPropertyFlags wantedMemoryFlags = ToVkMemoryFlags(memoryType);
    VkMemoryPropertyFlags unwantedMemoryFlags = 0;

    VkDeviceMemory memory;
    VkBuffer buffer = CreateBackedBuffer(gpu.GetVexDevice(), bufferInfo, wantedMemoryFlags, unwantedMemoryFlags, &memory);

    void* map = nullptr;
    if (!isGPUVulkanLocal)
    {
        device.MapMemory(memory, 0, allocSize, 0, &map);
    }

    m_gpu = &gpu;
    m_buffer = &device.GetVexBuffer(buffer);
    m_allocSize = allocSize;
    m_memoryFlags = wantedMemoryFlags;
    m_usage = usage;
    m_map = (byte*)map;
    m_memory = std::move(memory);

    if (allocation == GPUMemoryAllocation::eZero)
    {
        ZeroMemory();
    }
}

GPUVulkanBuffer::~GPUVulkanBuffer()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();

    if (m_map)
    {
        device.UnmapMemory(m_memory);
        m_map = nullptr;
    }

    device.DestroyBuffer(m_buffer->handle);
    device.FreeMemory(m_memory);
}

bool GPUVulkanBuffer::IsNull() const
{
    return m_buffer == nullptr;
}

GPUBackend GPUVulkanBuffer::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanBuffer::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

bool GPUVulkanBuffer::IsGPULocal() const
{
    AssertNotNull();
    return (m_memoryFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
}

void GPUVulkanBuffer::ZeroMemory()
{
    AssertNotNull();

    if (IsGPULocal())
    {
        ZeroMemoryGPULocal();
    }
    else
    {
        ZeroMemoryCPUVisible();
    }
}

void GPUVulkanBuffer::Upload(ea::span<const byte> data, uint32 dstOffset)
{
    AssertNotNull();
    ASSERT(data.size() <= m_allocSize);

    if (IsGPULocal())
    {
        UploadGPULocal(data, dstOffset);
    }
    else
    {
        UploadCPUVisible(data, dstOffset);
    }
}

uint64 GPUVulkanBuffer::GetAllocSize() const
{
    AssertNotNull();
    return m_allocSize;
}

GPUBufferUsage GPUVulkanBuffer::GetUsage() const
{
    AssertNotNull();
    return m_usage;
}

const VexBuffer& GPUVulkanBuffer::GetVexBuffer() const
{
    AssertNotNull();
    return *m_buffer;
}

VkBuffer GPUVulkanBuffer::GetHandle() const
{
    AssertNotNull();
    return m_buffer->handle;
}

void GPUVulkanBuffer::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void GPUVulkanBuffer::UploadCPUVisible(ea::span<const byte> data, uint32 byteDstOffset)
{
    AssertNotNull();
    ASSERT(byteDstOffset + data.size() <= m_allocSize);
    memcpy(m_map + byteDstOffset, data.data(), data.size());
}

void GPUVulkanBuffer::ZeroMemoryCPUVisible()
{
    AssertNotNull();
    memset(m_map, 0, m_allocSize);
}

VkBuffer GPUVulkanBuffer::CreateStagingBuffer(VkDeviceSize allocSize, VkDeviceMemory* outMemory, byte** outMap)
{
    AssertNotNull();
    VexDevice& device = m_gpu->GetVexDevice();

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = allocSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkMemoryPropertyFlags wantedMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    VkMemoryPropertyFlags unwantedMemoryFlags = 0;

    VkDeviceMemory memory;
    VkBuffer buffer = CreateBackedBuffer(device, bufferInfo, wantedMemoryFlags, unwantedMemoryFlags, &memory);

    void* map;
    device.MapMemory(memory, 0, allocSize, 0, &map);

    *outMemory = memory;
    *outMap = (byte*)map;
    return buffer;
}

void GPUVulkanBuffer::FlushStagingBuffer(const VexBuffer& stagingBuffer, uint32 byteDstOffset)
{
    AssertNotNull();
    VexDevice& device = m_gpu->GetVexDevice();
    VkCommandPool commandPool = m_gpu->GetCommandPool();
    VkQueue queue = device.graphicsQueue;

    VkCommandBuffer cmdBuffer;
    AllocateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &cmdBuffer);

    BeginCommandBuffer(device, cmdBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkBufferCopy copyRegion = {};
    copyRegion.dstOffset = byteDstOffset;
    copyRegion.size = stagingBuffer.size;
    device.CmdCopyBuffer(cmdBuffer, stagingBuffer.handle, m_buffer->handle, 1, &copyRegion);

    device.EndCommandBuffer(cmdBuffer);

    VkFence fence;
    CreateFence(device, 0, &fence);

    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &cmdBuffer;

    device.QueueSubmit(queue, 1, &submit, fence);
    device.WaitForFences(1, &fence, true, Uint64Max);

    device.DestroyFence(fence);
    device.FreeCommandBuffers(commandPool, 1, &cmdBuffer);
}

void GPUVulkanBuffer::UploadGPULocal(ea::span<const byte> data, uint32 byteDstOffset)
{
    AssertNotNull();
    ASSERT(data.size() <= m_allocSize);
    ASSERT_MSG(!m_map, "Memory map pointer should be null for GPUVulkanLocal buffer");
    VexDevice& device = m_gpu->GetVexDevice();

    byte* map;
    VkDeviceMemory stagingBufferMemory;
    VkBuffer stagingBuffer = CreateStagingBuffer(data.size(), &stagingBufferMemory, &map);

    memcpy(map, data.data(), data.size());

    device.UnmapMemory(stagingBufferMemory);
    map = nullptr;

    FlushStagingBuffer(device.GetVexBuffer(stagingBuffer), byteDstOffset);

    device.DestroyBuffer(stagingBuffer);
    device.FreeMemory(stagingBufferMemory);
}

void GPUVulkanBuffer::ZeroMemoryGPULocal()
{
    AssertNotNull();
    ASSERT_MSG(!m_map, "Memory map pointer should be null for GPUVulkanLocal buffer");
    VexDevice& device = m_gpu->GetVexDevice();

    byte* map;
    VkDeviceMemory stagingBufferMemory;
    VkBuffer stagingBuffer = CreateStagingBuffer(m_allocSize, &stagingBufferMemory, &map);

    memset(map, 0, m_allocSize);

    device.UnmapMemory(stagingBufferMemory);
    map = nullptr;

    FlushStagingBuffer(device.GetVexBuffer(stagingBuffer), 0);

    device.DestroyBuffer(stagingBuffer);
    device.FreeMemory(stagingBufferMemory);
}