
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>

uint2 Gyn::ToVec2u(VkExtent2D e)
{
    return uint2(e.width, e.height);
}

VkExtent2D Gyn::ToVkExtent2D(uint2 v)
{
    VkExtent2D ret;
    ret.width = v.x;
    ret.height= v.y;
    return ret;
}

const char* Gyn::ToString(VkDebugUtilsMessageSeverityFlagBitsEXT s)
{
    switch (s)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return "Verbose";
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return "Error";
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return "Warning";
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return "Info";
    }

    return "Unknown";
}

const char* Gyn::ToString(VkDebugUtilsMessageTypeFlagsEXT s)
{
    switch (s)
    {
    case 1: return "General";
    case 2: return "Validation";
    case 3: return "General | Validation";
    case 4: return "Performance";
    case 5: return "General | Performance";
    case 6: return "Validation | Performance";
    case 7: return "General | Validation | Performance";
    }

    return "Unknown";
}

const char* Gyn::ToString(VkShaderStageFlagBits stage)
{
    switch (stage)
    {
    case VK_SHADER_STAGE_VERTEX_BIT: return "Vertex";
    case VK_SHADER_STAGE_FRAGMENT_BIT: return "Fragment";
    }

    return "Unknown";
}

VkResult Gyn::CreateSampler(VexDevice& device, VkFilter minFilter, VkFilter magFilter,
    VkSamplerMipmapMode mipMapMode, VkSamplerAddressMode addressMode, VkSampler* pHandle)
{
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.addressModeU = addressMode;
    info.addressModeV = addressMode;
    info.addressModeW = addressMode;
    info.borderColor = {};
    info.anisotropyEnable = false;
    info.compareEnable = false;
    info.compareOp = VK_COMPARE_OP_ALWAYS;
    info.minFilter = minFilter;
    info.magFilter = magFilter;
    info.minLod = 0.f;
    info.maxLod = VK_LOD_CLAMP_NONE;
    info.mipLodBias = 0.f;
    info.mipmapMode = mipMapMode;
    info.unnormalizedCoordinates = false;

    return device.CreateSampler(&info, pHandle);
}

VkResult Gyn::AllocateDescriptorSet(VexDevice& device, VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDescriptorSet* pHandle)
{
    VkDescriptorSetAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    info.descriptorPool = pool;
    info.pSetLayouts = &layout;
    info.descriptorSetCount = 1;

    return device.AllocateDescriptorSets(&info, pHandle);
}

VkResult Gyn::AllocateCommandBuffer(VexDevice& device, VkCommandPool pool, VkCommandBufferLevel level, VkCommandBuffer* pHandle)
{
    VkCommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = level;
    allocateInfo.commandPool = pool;
    allocateInfo.commandBufferCount = 1;

    return device.AllocateCommandBuffers(&allocateInfo, pHandle);
}

VkResult Gyn::BeginCommandBuffer(VexDevice& device, VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags flags)
{
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flags;

    return device.BeginCommandBuffer(commandBuffer, &beginInfo);
}

VkResult Gyn::CreateFence(VexDevice& device, VkFenceCreateFlags flags, VkFence* pHandle)
{
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.flags = flags;

    return device.CreateFence(&info, pHandle);
}

VkResult Gyn::CreateSemaphore(VexDevice& device, VkSemaphore* pHandle)
{
    VkSemaphoreCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    return device.CreateSemaphore(&info, pHandle);
}

void Gyn::FreeCommandBuffers(VexDevice& device, ea::span<const VkCommandBuffer> cmdBuffers)
{
    for (size_t i = 0; i < cmdBuffers.size(); ++i)
    {
        VkCommandBuffer cmdBuffer = cmdBuffers[i];
        device.FreeCommandBuffers(device.GetVexCommandBuffer(cmdBuffer).pool, 1, &cmdBuffer);
    }
}

void Gyn::FreeDescriptorSets(VexDevice& device, ea::span<const VkDescriptorSet> dscSets)
{
    for (size_t i = 0; i < dscSets.size(); ++i)
    {
        VkDescriptorSet dscSet = dscSets[i];
        device.FreeDescriptorSets(device.GetVexDescriptorSet(dscSet).descriptorPool, 1, &dscSet);
    }
}

uint32_t Gyn::FindSuitableMemoryType(const VexDevice& device, const VkMemoryRequirements& requirements, VkMemoryPropertyFlags wantedFlags, VkMemoryPropertyFlags unwantedFlags)
{
    VkPhysicalDeviceMemoryProperties properties;
    device.GetPhysicalDeviceMemoryProperties(&properties);

    for (uint32_t i = 0; i < properties.memoryTypeCount; ++i)
    {
        bool isMemoryTypeSupported = requirements.memoryTypeBits & (1 << i);
        bool wantedFlagsPresent = (properties.memoryTypes[i].propertyFlags & wantedFlags) == wantedFlags;
        bool unwantedFlagsNotPresent = (properties.memoryTypes[i].propertyFlags & unwantedFlags) == 0;

        if (isMemoryTypeSupported && wantedFlagsPresent && unwantedFlagsNotPresent)
            return i;
    }

    ASSERT(false);
    return ~0u;
}

VkResult Gyn::AllocateRequiredDeviceMemory(VexDevice& device, 
    const VkMemoryRequirements& memoryRequirements, 
    VkMemoryPropertyFlags wantedMemoryFlags, 
    VkMemoryPropertyFlags unwantedMemoryFlags, 
    VkDeviceMemory* pMemory)
{
    VkMemoryAllocateInfo memoryInfo = {};
    memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryInfo.allocationSize = memoryRequirements.size;
    memoryInfo.memoryTypeIndex = FindSuitableMemoryType(device, memoryRequirements, wantedMemoryFlags, unwantedMemoryFlags);
    return device.AllocateMemory(&memoryInfo, pMemory);
}

VkBuffer Gyn::CreateBackedBuffer(VexDevice& device, 
    const VkBufferCreateInfo& info, 
    VkMemoryPropertyFlags wantedMemoryFlags, 
    VkMemoryPropertyFlags unwantedMemoryFlags, 
    VkDeviceMemory* outMemory,
    VkMemoryRequirements* outRequirements)
{
    VkBuffer buffer;
    device.CreateBuffer(&info, &buffer);

    VkMemoryRequirements memoryRequirements;
    device.GetBufferMemoryRequirements(buffer, &memoryRequirements);

    VkDeviceMemory deviceMemory;
    AllocateRequiredDeviceMemory(device, memoryRequirements, wantedMemoryFlags, unwantedMemoryFlags, &deviceMemory);

    device.BindBufferMemory(buffer, deviceMemory, 0);

    *outMemory = deviceMemory;

    if (outRequirements != nullptr)
        *outRequirements = memoryRequirements;

    return buffer;
}

VkImage Gyn::CreateBackedImage(VexDevice& device, 
    const VkImageCreateInfo& info,
    VkMemoryPropertyFlags wantedMemoryFlags,
    VkMemoryPropertyFlags unwantedMemoryFlags, 
    VkDeviceMemory* outMemory,
    VkMemoryRequirements* outRequirements)
{
    VkImage image;
    device.CreateImage(&info, &image);

    VkMemoryRequirements memoryRequirements;
    device.GetImageMemoryRequirements(image, &memoryRequirements);

    VkDeviceMemory deviceMemory;
    AllocateRequiredDeviceMemory(device, memoryRequirements, wantedMemoryFlags, unwantedMemoryFlags, &deviceMemory);

    device.BindImageMemory(image, deviceMemory, 0);

    *outMemory = deviceMemory;

    if (outRequirements != nullptr)
        *outRequirements = memoryRequirements;

    return image;
}