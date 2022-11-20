#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    uint2 ToVec2u(VkExtent2D e);

    VkExtent2D ToVkExtent2D(uint2 v);

    const char* ToString(VkDebugUtilsMessageSeverityFlagBitsEXT s);

    const char* ToString(VkDebugUtilsMessageTypeFlagsEXT s);

    const char* ToString(VkShaderStageFlagBits stage);

    VkResult CreateSampler(VexDevice& device, VkFilter minFilter, VkFilter magFilter,
        VkSamplerMipmapMode mipMapMode, VkSamplerAddressMode addressMode, VkSampler* pHandle);

    VkResult AllocateDescriptorSet(VexDevice& device, VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDescriptorSet* pHandle);

    VkResult AllocateCommandBuffer(VexDevice& device, VkCommandPool pool, VkCommandBufferLevel level, VkCommandBuffer* pHandle);

    VkResult BeginCommandBuffer(VexDevice& device, VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags flags);

    VkResult CreateFence(VexDevice& device, VkFenceCreateFlags flags, VkFence* pHandle);

    VkResult CreateSemaphore(VexDevice& device, VkSemaphore* pHandle);

    void FreeCommandBuffers(VexDevice& device, ea::span<const VkCommandBuffer> cmdBuffers);

    void FreeDescriptorSets(VexDevice& device, ea::span<const VkDescriptorSet> dscSets);

    uint32_t FindSuitableMemoryType(const VexDevice& device, const VkMemoryRequirements& requirements, VkMemoryPropertyFlags wantedFlags, VkMemoryPropertyFlags unwantedFlags);

    VkResult AllocateRequiredDeviceMemory(VexDevice& device, 
        const VkMemoryRequirements& memoryRequirements, 
        VkMemoryPropertyFlags wantedMemoryFlags, 
        VkMemoryPropertyFlags unwantedMemoryFlags, 
        VkDeviceMemory* pMemory);

    VkBuffer CreateBackedBuffer(VexDevice& device, 
        const VkBufferCreateInfo& info, 
        VkMemoryPropertyFlags wantedMemoryFlags, 
        VkMemoryPropertyFlags unwantedMemoryFlags, 
        VkDeviceMemory* outMemory,
        VkMemoryRequirements* outRequirements = nullptr);

    VkImage CreateBackedImage(VexDevice& device, 
        const VkImageCreateInfo& info,
        VkMemoryPropertyFlags wantedMemoryFlags,
        VkMemoryPropertyFlags unwantedMemoryFlags, 
        VkDeviceMemory* outMemory,
        VkMemoryRequirements* outRequirements = nullptr);
}