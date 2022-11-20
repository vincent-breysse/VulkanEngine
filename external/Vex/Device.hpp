#pragma once

#include <vector>
#include <cstdint>
#include <Vex/Config.hpp>
#include <Vex/Loader.hpp>

struct VexBuffer
{
    VkBuffer handle;
    VkBufferCreateFlags flags;
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkSharingMode sharingMode;
    std::vector<uint32_t> queueFamilyIndices;
};

struct VexBufferView
{
    VkBufferView handle;
    VkBufferViewCreateFlags flags;
    VkBuffer buffer;
    VkFormat format;
    VkDeviceSize offset;
    VkDeviceSize range;
};

struct VexCommandBuffer
{
    VkCommandBuffer handle;
    VkCommandPool pool;
    VkCommandBufferLevel level;
};

struct VexCommandPool
{
    VkCommandPool handle;
    VkCommandPoolCreateFlags flags;
    uint32_t queueFamilyIndex;
};

struct VexDescriptorPool
{
    VkDescriptorPool handle;
    VkDescriptorPoolCreateFlags flags;
    uint32_t maxSets;
    std::vector<VkDescriptorPoolSize> poolSizes;
};

struct VexDescriptorSet
{
    VkDescriptorSet handle;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSetLayout> setLayouts;
};

struct VexDescriptorSetLayout
{
    VkDescriptorSetLayout handle;
    VkDescriptorSetLayoutCreateFlags flags;
};

struct VexDeviceMemory
{
    VkDeviceMemory handle;
    VkDeviceSize allocationSize;
    uint32_t memoryTypeIndex;
};

struct VexFramebuffer
{
    VkFramebuffer handle;
    VkFramebufferCreateFlags flags;
    VkRenderPass renderPass;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
    std::vector<VkImageView> attachments;
};

struct VexImage
{
    VkImage handle;
    VkImageCreateFlags flags;
    VkImageType imageType;
    VkFormat format;
    VkExtent3D extent;
    uint32_t mipLevels;
    uint32_t arrayLayers;
    VkSampleCountFlags samples;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkSharingMode sharingMode;
    std::vector<uint32_t> queueFamilyIndices;
    VkImageLayout initialLayout;
};

struct VexImageView
{
    VkImageView handle;
    VkImageViewCreateFlags flags;
    VkImage image;
    VkImageViewType viewType;
    VkFormat format;
    VkComponentMapping components;
    VkImageSubresourceRange subresourceRange;
};

struct VexPipelineCache
{
    VkPipelineCache handle;
    VkPipelineCacheCreateFlags flags;
};

struct VexPipelineLayout
{
    VkPipelineLayout handle;
    VkPipelineLayoutCreateFlags flags;
    std::vector<VkDescriptorSetLayout> setLayouts;
    std::vector<VkPushConstantRange> pushConstantRanges;
};

struct VexRenderPass
{
    VkRenderPass handle;
    VkRenderPassCreateFlags flags;
    std::vector<VkAttachmentDescription> attachments;
    std::vector<VkSubpassDependency> dependencies;
};

struct VexSampler
{
    VkSampler handle;
    VkSamplerCreateFlags flags;
    VkFilter magFilter;
    VkFilter minFilter;
    VkSamplerMipmapMode mipmapMode;
    VkSamplerAddressMode addressModeU;
    VkSamplerAddressMode addressModeV;
    VkSamplerAddressMode addressModeW;
    float mipLodBias;
    VkBool32 anisotropyEnable;
    float maxAnisotropy;
    VkBool32 compareEnable;
    VkCompareOp compareOp;
    float minLod;
    float maxLod;
    VkBorderColor borderColor;
    VkBool32 unnormalizedCoordinates;
};

struct VexShaderModule
{
    VkShaderModule handle;
    VkShaderModuleCreateFlags flags;
};

struct VexSwapchainKHR
{
    VkSwapchainKHR handle;
    VkSwapchainCreateFlagsKHR flags;
    VkSurfaceKHR surface;
    uint32_t minImageCount;
    VkFormat imageFormat;
    VkColorSpaceKHR imageColorSpace;
    VkExtent2D imageExtent;
    uint32_t imageArrayLayers;
    VkImageUsageFlags imageUsage;
    VkSharingMode imageSharingMode;
    std::vector<uint32_t> queueFamilyIndices;
    VkSurfaceTransformFlagBitsKHR preTransform;
    VkCompositeAlphaFlagBitsKHR compositeAlpha;
    VkPresentModeKHR presentMode;
    VkBool32 clipped;
};

struct VexObjectTable
{
    /* VEX_GENERATE_OBJECT_TABLE */

    VexThreadedHashMap<VkBuffer, VexBuffer> mapBuffer;
    VexThreadedHashMap<VkBufferView, VexBufferView> mapBufferView;
    VexThreadedHashMap<VkCommandBuffer, VexCommandBuffer> mapCommandBuffer;
    VexThreadedHashMap<VkCommandPool, VexCommandPool> mapCommandPool;
    VexThreadedHashMap<VkDescriptorPool, VexDescriptorPool> mapDescriptorPool;
    VexThreadedHashMap<VkDescriptorSet, VexDescriptorSet> mapDescriptorSet;
    VexThreadedHashMap<VkDescriptorSetLayout, VexDescriptorSetLayout> mapDescriptorSetLayout;
    VexThreadedHashMap<VkDeviceMemory, VexDeviceMemory> mapDeviceMemory;
    VexThreadedHashMap<VkFramebuffer, VexFramebuffer> mapFramebuffer;
    VexThreadedHashMap<VkImage, VexImage> mapImage;
    VexThreadedHashMap<VkImageView, VexImageView> mapImageView;
    VexThreadedHashMap<VkPipelineCache, VexPipelineCache> mapPipelineCache;
    VexThreadedHashMap<VkPipelineLayout, VexPipelineLayout> mapPipelineLayout;
    VexThreadedHashMap<VkRenderPass, VexRenderPass> mapRenderPass;
    VexThreadedHashMap<VkSampler, VexSampler> mapSampler;
    VexThreadedHashMap<VkShaderModule, VexShaderModule> mapShaderModule;
    VexThreadedHashMap<VkSwapchainKHR, VexSwapchainKHR> mapSwapchainKHR;

    /* VEX_GENERATE_OBJECT_TABLE */
};

struct VexDevice
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physDevice;
    VkSurfaceKHR mainSurface;
    VkDevice device;
    const VkAllocationCallbacks* allocator;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    uint32_t graphicsQueueFamilyIndex;
    uint32_t presentQueueFamilyIndex;
    VexProcTable procTable;
    VexObjectTable objTable;

    VexDevice();

    void Destroy();

    void GetPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties* pMemoryProperties) const;

    void RegisterDeviceMemory(const VkMemoryAllocateInfo* pCreateInfo, VkDeviceMemory handle);
    VkResult AllocateMemory(const VkMemoryAllocateInfo* pCreateInfo, VkDeviceMemory* pHandle);
    void DeregisterDeviceMemory(VkDeviceMemory handle);
    void FreeMemory(VkDeviceMemory handle);
    const VexDeviceMemory& GetVexDeviceMemory(VkDeviceMemory handle) const;

    void RegisterCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, const VkCommandBuffer* pCommandBuffers);
    VkResult AllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers);
    void DeregisterCommandBuffers(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
    void FreeCommandBuffers(VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers);
    const VexCommandBuffer& GetVexCommandBuffer(VkCommandBuffer handle) const;

    void RegisterDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, const VkDescriptorSet* pDescriptorSets);
    VkResult AllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets);
    void DeregisterDescriptorSets(uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
    void FreeDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets);
    const VexDescriptorSet& GetVexDescriptorSet(VkDescriptorSet handle) const;

    /* VEX_GENERATE_VEX_OBJ_FUNCS_H */

    void RegisterBuffer(const VkBufferCreateInfo* pCreateInfo, VkBuffer handle);
    VkResult CreateBuffer(const VkBufferCreateInfo* pCreateInfo, VkBuffer* pHandle);
    void DeregisterBuffer(VkBuffer handle);
    void DestroyBuffer(VkBuffer handle);
    const VexBuffer& GetVexBuffer(VkBuffer handle) const;

    void RegisterBufferView(const VkBufferViewCreateInfo* pCreateInfo, VkBufferView handle);
    VkResult CreateBufferView(const VkBufferViewCreateInfo* pCreateInfo, VkBufferView* pHandle);
    void DeregisterBufferView(VkBufferView handle);
    void DestroyBufferView(VkBufferView handle);
    const VexBufferView& GetVexBufferView(VkBufferView handle) const;

    void RegisterCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, VkCommandPool handle);
    VkResult CreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, VkCommandPool* pHandle);
    void DeregisterCommandPool(VkCommandPool handle);
    void DestroyCommandPool(VkCommandPool handle);
    const VexCommandPool& GetVexCommandPool(VkCommandPool handle) const;

    void RegisterDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, VkDescriptorPool handle);
    VkResult CreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, VkDescriptorPool* pHandle);
    void DeregisterDescriptorPool(VkDescriptorPool handle);
    void DestroyDescriptorPool(VkDescriptorPool handle);
    const VexDescriptorPool& GetVexDescriptorPool(VkDescriptorPool handle) const;

    void RegisterDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayout handle);
    VkResult CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayout* pHandle);
    void DeregisterDescriptorSetLayout(VkDescriptorSetLayout handle);
    void DestroyDescriptorSetLayout(VkDescriptorSetLayout handle);
    const VexDescriptorSetLayout& GetVexDescriptorSetLayout(VkDescriptorSetLayout handle) const;

    void RegisterFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, VkFramebuffer handle);
    VkResult CreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, VkFramebuffer* pHandle);
    void DeregisterFramebuffer(VkFramebuffer handle);
    void DestroyFramebuffer(VkFramebuffer handle);
    const VexFramebuffer& GetVexFramebuffer(VkFramebuffer handle) const;

    void RegisterImage(const VkImageCreateInfo* pCreateInfo, VkImage handle);
    VkResult CreateImage(const VkImageCreateInfo* pCreateInfo, VkImage* pHandle);
    void DeregisterImage(VkImage handle);
    void DestroyImage(VkImage handle);
    const VexImage& GetVexImage(VkImage handle) const;

    void RegisterImageView(const VkImageViewCreateInfo* pCreateInfo, VkImageView handle);
    VkResult CreateImageView(const VkImageViewCreateInfo* pCreateInfo, VkImageView* pHandle);
    void DeregisterImageView(VkImageView handle);
    void DestroyImageView(VkImageView handle);
    const VexImageView& GetVexImageView(VkImageView handle) const;

    void RegisterPipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, VkPipelineCache handle);
    VkResult CreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, VkPipelineCache* pHandle);
    void DeregisterPipelineCache(VkPipelineCache handle);
    void DestroyPipelineCache(VkPipelineCache handle);
    const VexPipelineCache& GetVexPipelineCache(VkPipelineCache handle) const;

    void RegisterPipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, VkPipelineLayout handle);
    VkResult CreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, VkPipelineLayout* pHandle);
    void DeregisterPipelineLayout(VkPipelineLayout handle);
    void DestroyPipelineLayout(VkPipelineLayout handle);
    const VexPipelineLayout& GetVexPipelineLayout(VkPipelineLayout handle) const;

    void RegisterRenderPass(const VkRenderPassCreateInfo* pCreateInfo, VkRenderPass handle);
    VkResult CreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, VkRenderPass* pHandle);
    void DeregisterRenderPass(VkRenderPass handle);
    void DestroyRenderPass(VkRenderPass handle);
    const VexRenderPass& GetVexRenderPass(VkRenderPass handle) const;

    void RegisterSampler(const VkSamplerCreateInfo* pCreateInfo, VkSampler handle);
    VkResult CreateSampler(const VkSamplerCreateInfo* pCreateInfo, VkSampler* pHandle);
    void DeregisterSampler(VkSampler handle);
    void DestroySampler(VkSampler handle);
    const VexSampler& GetVexSampler(VkSampler handle) const;

    void RegisterShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModule handle);
    VkResult CreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModule* pHandle);
    void DeregisterShaderModule(VkShaderModule handle);
    void DestroyShaderModule(VkShaderModule handle);
    const VexShaderModule& GetVexShaderModule(VkShaderModule handle) const;

    void RegisterSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, VkSwapchainKHR handle);
    VkResult CreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, VkSwapchainKHR* pHandle);
    void DeregisterSwapchainKHR(VkSwapchainKHR handle);
    void DestroySwapchainKHR(VkSwapchainKHR handle);
    const VexSwapchainKHR& GetVexSwapchainKHR(VkSwapchainKHR handle) const;

    /* VEX_GENERATE_VEX_OBJ_FUNCS_H */

    /* VEX_GENERATE_DEVICE_WRAPPERS_H */
#if defined(VK_VERSION_1_0)
    VkResult EnumeratePhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const;
    void GetPhysicalDeviceFeatures(VkPhysicalDeviceFeatures* pFeatures) const;
    void GetPhysicalDeviceFormatProperties(VkFormat format, VkFormatProperties* pFormatProperties) const;
    VkResult GetPhysicalDeviceImageFormatProperties(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) const;
    void GetPhysicalDeviceProperties(VkPhysicalDeviceProperties* pProperties) const;
    void GetPhysicalDeviceQueueFamilyProperties(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) const;
    PFN_vkVoidFunction GetInstanceProcAddr(const char* pName) const;
    PFN_vkVoidFunction GetDeviceProcAddr(const char* pName) const;
    VkResult EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const;
    VkResult EnumerateDeviceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const;
    VkResult EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) const;
    VkResult EnumerateDeviceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) const;
    void GetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) const;
    VkResult QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) const;
    VkResult QueueWaitIdle(VkQueue queue) const;
    VkResult DeviceWaitIdle() const;
    VkResult MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) const;
    void UnmapMemory(VkDeviceMemory memory) const;
    VkResult FlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) const;
    VkResult InvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) const;
    void GetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) const;
    VkResult BindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) const;
    VkResult BindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) const;
    void GetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) const;
    void GetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements) const;
    void GetImageSparseMemoryRequirements(VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) const;
    void GetPhysicalDeviceSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) const;
    VkResult QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) const;
    VkResult CreateFence(const VkFenceCreateInfo* pCreateInfo, VkFence* pFence) const;
    void DestroyFence(VkFence fence) const;
    VkResult ResetFences(uint32_t fenceCount, const VkFence* pFences) const;
    VkResult GetFenceStatus(VkFence fence) const;
    VkResult WaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) const;
    VkResult CreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, VkSemaphore* pSemaphore) const;
    void DestroySemaphore(VkSemaphore semaphore) const;
    VkResult CreateEvent(const VkEventCreateInfo* pCreateInfo, VkEvent* pEvent) const;
    void DestroyEvent(VkEvent event) const;
    VkResult GetEventStatus(VkEvent event) const;
    VkResult SetEvent(VkEvent event) const;
    VkResult ResetEvent(VkEvent event) const;
    VkResult CreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo, VkQueryPool* pQueryPool) const;
    void DestroyQueryPool(VkQueryPool queryPool) const;
    VkResult GetQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) const;
    void GetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) const;
    VkResult GetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) const;
    VkResult MergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) const;
    VkResult CreateGraphicsPipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, VkPipeline* pPipelines) const;
    VkResult CreateComputePipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, VkPipeline* pPipelines) const;
    void DestroyPipeline(VkPipeline pipeline) const;
    VkResult ResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) const;
    void UpdateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) const;
    void GetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity) const;
    VkResult ResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags) const;
    VkResult BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) const;
    VkResult EndCommandBuffer(VkCommandBuffer commandBuffer) const;
    VkResult ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) const;
    void CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const;
    void CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const;
    void CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const;
    void CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) const;
    void CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const;
    void CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) const;
    void CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) const;
    void CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) const;
    void CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) const;
    void CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) const;
    void CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const;
    void CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const;
    void CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const;
    void CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const;
    void CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const;
    void CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
    void CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
    void CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
    void CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) const;
    void CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const;
    void CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const;
    void CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const;
    void CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const;
    void CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const;
    void CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const;
    void CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const;
    void CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const;
    void CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const;
    void CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const;
    void CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const;
    void CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) const;
    void CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) const;
    void CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const;
    void CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const;
    void CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const;
    void CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) const;
    void CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;
    void CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const;
    void CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const;
    void CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const;
    void CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const;
    void CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) const;
    void CmdEndRenderPass(VkCommandBuffer commandBuffer) const;
    void CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const;
#endif
#if defined(VK_VERSION_1_1)
    VkResult EnumerateInstanceVersion(uint32_t* pApiVersion) const;
    VkResult BindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) const;
    VkResult BindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) const;
    void GetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) const;
    void CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) const;
    void CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
    VkResult EnumeratePhysicalDeviceGroups(uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) const;
    void GetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const;
    void GetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const;
    void GetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) const;
    void GetPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* pFeatures) const;
    void GetPhysicalDeviceProperties2(VkPhysicalDeviceProperties2* pProperties) const;
    void GetPhysicalDeviceFormatProperties2(VkFormat format, VkFormatProperties2* pFormatProperties) const;
    VkResult GetPhysicalDeviceImageFormatProperties2(const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) const;
    void GetPhysicalDeviceQueueFamilyProperties2(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const;
    void GetPhysicalDeviceMemoryProperties2(VkPhysicalDeviceMemoryProperties2* pMemoryProperties) const;
    void GetPhysicalDeviceSparseImageFormatProperties2(const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) const;
    void TrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags) const;
    void GetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) const;
    VkResult CreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkSamplerYcbcrConversion* pYcbcrConversion) const;
    void DestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion) const;
    VkResult CreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) const;
    void DestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate) const;
    void UpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const;
    void GetPhysicalDeviceExternalBufferProperties(const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) const;
    void GetPhysicalDeviceExternalFenceProperties(const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) const;
    void GetPhysicalDeviceExternalSemaphoreProperties(const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) const;
    void GetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) const;
#endif
#if defined(VK_KHR_surface)
    void DestroySurfaceKHR(VkSurfaceKHR surface) const;
    VkResult GetPhysicalDeviceSurfaceSupportKHR(uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const;
    VkResult GetPhysicalDeviceSurfaceCapabilitiesKHR(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const;
    VkResult GetPhysicalDeviceSurfaceFormatsKHR(VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const;
    VkResult GetPhysicalDeviceSurfacePresentModesKHR(VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const;
#endif
#if defined(VK_KHR_swapchain)
    VkResult GetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) const;
    VkResult AcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) const;
    VkResult QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) const;
#endif
#if defined(VK_KHR_display)
    VkResult GetPhysicalDeviceDisplayPropertiesKHR(uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) const;
    VkResult GetPhysicalDeviceDisplayPlanePropertiesKHR(uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) const;
    VkResult GetDisplayPlaneSupportedDisplaysKHR(uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) const;
    VkResult GetDisplayModePropertiesKHR(VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) const;
    VkResult CreateDisplayModeKHR(VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, VkDisplayModeKHR* pMode) const;
    VkResult GetDisplayPlaneCapabilitiesKHR(VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) const;
    VkResult CreateDisplayPlaneSurfaceKHR(const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_KHR_display_swapchain)
    VkResult CreateSharedSwapchainsKHR(uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, VkSwapchainKHR* pSwapchains) const;
#endif
#if defined(VK_KHR_xlib_surface)
    VkResult CreateXlibSurfaceKHR(const VkXlibSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const;
    VkBool32 GetPhysicalDeviceXlibPresentationSupportKHR(uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) const;
#endif
#if defined(VK_KHR_xcb_surface)
    VkResult CreateXcbSurfaceKHR(const VkXcbSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const;
    VkBool32 GetPhysicalDeviceXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) const;
#endif
#if defined(VK_KHR_wayland_surface)
    VkResult CreateWaylandSurfaceKHR(const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const;
    VkBool32 GetPhysicalDeviceWaylandPresentationSupportKHR(uint32_t queueFamilyIndex, struct wl_display* display) const;
#endif
#if defined(VK_KHR_android_surface)
    VkResult CreateAndroidSurfaceKHR(const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_KHR_win32_surface)
    VkResult CreateWin32SurfaceKHR(const VkWin32SurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const;
    VkBool32 GetPhysicalDeviceWin32PresentationSupportKHR(uint32_t queueFamilyIndex) const;
#endif
#if defined(VK_EXT_debug_report)
    VkResult CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, VkDebugReportCallbackEXT* pCallback) const;
    void DestroyDebugReportCallbackEXT(VkDebugReportCallbackEXT callback) const;
    void DebugReportMessageEXT(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) const;
#endif
#if defined(VK_EXT_debug_marker)
    VkResult DebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo) const;
    VkResult DebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo) const;
    void CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const;
    void CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) const;
    void CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const;
#endif
#if defined(VK_EXT_transform_feedback)
    void CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const;
    void CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const;
    void CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const;
    void CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const;
    void CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) const;
    void CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const;
#endif
#if defined(VK_NVX_image_view_handle)
    uint32_t GetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo) const;
#endif
#if defined(VK_AMD_draw_indirect_count)
    void CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
    void CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_AMD_shader_info)
    VkResult GetShaderInfoAMD(VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) const;
#endif
#if defined(VK_GGP_stream_descriptor_surface)
    VkResult CreateStreamDescriptorSurfaceGGP(const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_NV_external_memory_capabilities)
    VkResult GetPhysicalDeviceExternalImageFormatPropertiesNV(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) const;
#endif
#if defined(VK_NV_external_memory_win32)
    VkResult GetMemoryWin32HandleNV(VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) const;
#endif
#if defined(VK_KHR_get_physical_device_properties2)
    void GetPhysicalDeviceFeatures2KHR(VkPhysicalDeviceFeatures2* pFeatures) const;
    void GetPhysicalDeviceProperties2KHR(VkPhysicalDeviceProperties2* pProperties) const;
    void GetPhysicalDeviceFormatProperties2KHR(VkFormat format, VkFormatProperties2* pFormatProperties) const;
    VkResult GetPhysicalDeviceImageFormatProperties2KHR(const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) const;
    void GetPhysicalDeviceQueueFamilyProperties2KHR(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const;
    void GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDeviceMemoryProperties2* pMemoryProperties) const;
    void GetPhysicalDeviceSparseImageFormatProperties2KHR(const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) const;
#endif
#if defined(VK_KHR_device_group)
    void GetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) const;
    void CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) const;
    void CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const;
#endif
#if defined(VK_NN_vi_surface)
    VkResult CreateViSurfaceNN(const VkViSurfaceCreateInfoNN* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_KHR_maintenance1)
    void TrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags) const;
#endif
#if defined(VK_KHR_device_group_creation)
    VkResult EnumeratePhysicalDeviceGroupsKHR(uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) const;
#endif
#if defined(VK_KHR_external_memory_capabilities)
    void GetPhysicalDeviceExternalBufferPropertiesKHR(const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) const;
#endif
#if defined(VK_KHR_external_memory_win32)
    VkResult GetMemoryWin32HandleKHR(const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const;
    VkResult GetMemoryWin32HandlePropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) const;
#endif
#if defined(VK_KHR_external_memory_fd)
    VkResult GetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) const;
    VkResult GetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) const;
#endif
#if defined(VK_KHR_external_semaphore_capabilities)
    void GetPhysicalDeviceExternalSemaphorePropertiesKHR(const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) const;
#endif
#if defined(VK_KHR_external_semaphore_win32)
    VkResult ImportSemaphoreWin32HandleKHR(const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) const;
    VkResult GetSemaphoreWin32HandleKHR(const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const;
#endif
#if defined(VK_KHR_external_semaphore_fd)
    VkResult ImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) const;
    VkResult GetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) const;
#endif
#if defined(VK_KHR_push_descriptor)
    void CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const;
#endif
#if defined(VK_EXT_conditional_rendering)
    void CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const;
    void CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) const;
#endif
#if defined(VK_KHR_descriptor_update_template)
    VkResult CreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) const;
    void DestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate) const;
    void UpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const;
#endif
#if defined(VK_NVX_device_generated_commands)
    void CmdProcessCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo) const;
    void CmdReserveSpaceForCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo) const;
    VkResult CreateIndirectCommandsLayoutNVX(const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout) const;
    void DestroyIndirectCommandsLayoutNVX(VkIndirectCommandsLayoutNVX indirectCommandsLayout) const;
    VkResult CreateObjectTableNVX(const VkObjectTableCreateInfoNVX* pCreateInfo, VkObjectTableNVX* pObjectTable) const;
    void DestroyObjectTableNVX(VkObjectTableNVX objectTable) const;
    VkResult RegisterObjectsNVX(VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices) const;
    VkResult UnregisterObjectsNVX(VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices) const;
    void GetPhysicalDeviceGeneratedCommandsPropertiesNVX(VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits) const;
#endif
#if defined(VK_NV_clip_space_w_scaling)
    void CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const;
#endif
#if defined(VK_EXT_direct_mode_display)
    VkResult ReleaseDisplayEXT(VkDisplayKHR display) const;
#endif
#if defined(VK_EXT_acquire_xlib_display)
    VkResult AcquireXlibDisplayEXT(Display* dpy, VkDisplayKHR display) const;
    VkResult GetRandROutputDisplayEXT(Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) const;
#endif
#if defined(VK_EXT_display_surface_counter)
    VkResult GetPhysicalDeviceSurfaceCapabilities2EXT(VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) const;
#endif
#if defined(VK_EXT_display_control)
    VkResult DisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) const;
    VkResult RegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo, VkFence* pFence) const;
    VkResult RegisterDisplayEventEXT(VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, VkFence* pFence) const;
    VkResult GetSwapchainCounterEXT(VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) const;
#endif
#if defined(VK_GOOGLE_display_timing)
    VkResult GetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) const;
    VkResult GetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) const;
#endif
#if defined(VK_EXT_discard_rectangles)
    void CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const;
#endif
#if defined(VK_EXT_hdr_metadata)
    void SetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) const;
#endif
#if defined(VK_KHR_create_renderpass2)
    VkResult CreateRenderPass2KHR(const VkRenderPassCreateInfo2KHR* pCreateInfo, VkRenderPass* pRenderPass) const;
    void CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfoKHR* pSubpassBeginInfo) const;
    void CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR* pSubpassBeginInfo, const VkSubpassEndInfoKHR* pSubpassEndInfo) const;
    void CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR* pSubpassEndInfo) const;
#endif
#if defined(VK_KHR_shared_presentable_image)
    VkResult GetSwapchainStatusKHR(VkSwapchainKHR swapchain) const;
#endif
#if defined(VK_KHR_external_fence_capabilities)
    void GetPhysicalDeviceExternalFencePropertiesKHR(const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) const;
#endif
#if defined(VK_KHR_external_fence_win32)
    VkResult ImportFenceWin32HandleKHR(const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) const;
    VkResult GetFenceWin32HandleKHR(const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const;
#endif
#if defined(VK_KHR_external_fence_fd)
    VkResult ImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo) const;
    VkResult GetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) const;
#endif
#if defined(VK_KHR_get_surface_capabilities2)
    VkResult GetPhysicalDeviceSurfaceCapabilities2KHR(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) const;
    VkResult GetPhysicalDeviceSurfaceFormats2KHR(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) const;
#endif
#if defined(VK_KHR_get_display_properties2)
    VkResult GetPhysicalDeviceDisplayProperties2KHR(uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) const;
    VkResult GetPhysicalDeviceDisplayPlaneProperties2KHR(uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) const;
    VkResult GetDisplayModeProperties2KHR(VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) const;
    VkResult GetDisplayPlaneCapabilities2KHR(const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) const;
#endif
#if defined(VK_MVK_ios_surface)
    VkResult CreateIOSSurfaceMVK(const VkIOSSurfaceCreateInfoMVK* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_MVK_macos_surface)
    VkResult CreateMacOSSurfaceMVK(const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_EXT_debug_utils)
    VkResult SetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo) const;
    VkResult SetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo) const;
    void QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) const;
    void QueueEndDebugUtilsLabelEXT(VkQueue queue) const;
    void QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) const;
    void CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) const;
    void CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) const;
    void CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) const;
    VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkDebugUtilsMessengerEXT* pMessenger) const;
    void DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT messenger) const;
    void SubmitDebugUtilsMessageEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const;
#endif
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
    VkResult GetAndroidHardwareBufferPropertiesANDROID(const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) const;
    VkResult GetMemoryAndroidHardwareBufferANDROID(const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) const;
#endif
#if defined(VK_EXT_sample_locations)
    void CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const;
    void GetPhysicalDeviceMultisamplePropertiesEXT(VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) const;
#endif
#if defined(VK_KHR_get_memory_requirements2)
    void GetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const;
    void GetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const;
    void GetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) const;
#endif
#if defined(VK_KHR_sampler_ycbcr_conversion)
    VkResult CreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkSamplerYcbcrConversion* pYcbcrConversion) const;
    void DestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion) const;
#endif
#if defined(VK_KHR_bind_memory2)
    VkResult BindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) const;
    VkResult BindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) const;
#endif
#if defined(VK_EXT_image_drm_format_modifier)
    VkResult GetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) const;
#endif
#if defined(VK_EXT_validation_cache)
    VkResult CreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo, VkValidationCacheEXT* pValidationCache) const;
    void DestroyValidationCacheEXT(VkValidationCacheEXT validationCache) const;
    VkResult MergeValidationCachesEXT(VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) const;
    VkResult GetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) const;
#endif
#if defined(VK_NV_shading_rate_image)
    void CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) const;
    void CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const;
    void CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const;
#endif
#if defined(VK_NV_ray_tracing)
    VkResult CreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo, VkAccelerationStructureNV* pAccelerationStructure) const;
    void DestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure) const;
    void GetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const;
    VkResult BindAccelerationStructureMemoryNV(uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) const;
    void CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const;
    void CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeNV mode) const;
    void CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const;
    VkResult CreateRayTracingPipelinesNV(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkPipeline* pPipelines) const;
    VkResult GetRayTracingShaderGroupHandlesNV(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) const;
    VkResult GetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) const;
    void CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const;
    VkResult CompileDeferredNV(VkPipeline pipeline, uint32_t shader) const;
#endif
#if defined(VK_KHR_maintenance3)
    void GetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) const;
#endif
#if defined(VK_KHR_draw_indirect_count)
    void CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
    void CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_EXT_external_memory_host)
    VkResult GetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) const;
#endif
#if defined(VK_AMD_buffer_marker)
    void CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const;
#endif
#if defined(VK_EXT_calibrated_timestamps)
    VkResult GetPhysicalDeviceCalibrateableTimeDomainsEXT(uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) const;
    VkResult GetCalibratedTimestampsEXT(uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) const;
#endif
#if defined(VK_NV_mesh_shader)
    void CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) const;
    void CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const;
    void CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const;
#endif
#if defined(VK_NV_scissor_exclusive)
    void CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const;
#endif
#if defined(VK_NV_device_diagnostic_checkpoints)
    void CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) const;
    void GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) const;
#endif
#if defined(VK_KHR_timeline_semaphore)
    VkResult GetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue) const;
    VkResult WaitSemaphoresKHR(const VkSemaphoreWaitInfoKHR* pWaitInfo, uint64_t timeout) const;
    VkResult SignalSemaphoreKHR(const VkSemaphoreSignalInfoKHR* pSignalInfo) const;
#endif
#if defined(VK_INTEL_performance_query)
    VkResult InitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) const;
    void UninitializePerformanceApiINTEL() const;
    VkResult CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const;
    VkResult CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const;
    VkResult CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const;
    VkResult AcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) const;
    VkResult ReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration) const;
    VkResult QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) const;
    VkResult GetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) const;
#endif
#if defined(VK_AMD_display_native_hdr)
    void SetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) const;
#endif
#if defined(VK_FUCHSIA_imagepipe_surface)
    VkResult CreateImagePipeSurfaceFUCHSIA(const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_EXT_metal_surface)
    VkResult CreateMetalSurfaceEXT(const VkMetalSurfaceCreateInfoEXT* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_EXT_buffer_device_address)
    VkDeviceAddress GetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfoEXT* pInfo) const;
#endif
#if defined(VK_NV_cooperative_matrix)
    VkResult GetPhysicalDeviceCooperativeMatrixPropertiesNV(uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) const;
#endif
#if defined(VK_NV_coverage_reduction_mode)
    VkResult GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) const;
#endif
#if defined(VK_EXT_full_screen_exclusive)
    VkResult GetPhysicalDeviceSurfacePresentModes2EXT(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const;
    VkResult AcquireFullScreenExclusiveModeEXT(VkSwapchainKHR swapchain) const;
    VkResult ReleaseFullScreenExclusiveModeEXT(VkSwapchainKHR swapchain) const;
#endif
#if defined(VK_EXT_headless_surface)
    VkResult CreateHeadlessSurfaceEXT(const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, VkSurfaceKHR* pSurface) const;
#endif
#if defined(VK_EXT_line_rasterization)
    void CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) const;
#endif
#if defined(VK_EXT_host_query_reset)
    void ResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const;
#endif
#if defined(VK_KHR_pipeline_executable_properties)
    VkResult GetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) const;
    VkResult GetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) const;
    VkResult GetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) const;
#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
    VkResult GetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) const;
    VkResult GetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) const;
    VkResult GetPhysicalDevicePresentRectanglesKHR(VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) const;
#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain))
    VkResult AcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) const;
#endif
#if (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor))
    void CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const;
#endif
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
    VkResult GetDeviceGroupSurfacePresentModes2EXT(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) const;
#endif
    /* VEX_GENERATE_DEVICE_WRAPPERS_H */
};

using VexFnCreateSurface = VkSurfaceKHR(*)(VkInstance instance, const VkAllocationCallbacks* pAllocator, void* userPtr);

struct VexDeviceCreateInfo
{
    uint32_t minVersionMajor;
    uint32_t minVersionMinor;
    bool enableValidationLayers;
    PFN_vkDebugUtilsMessengerCallbackEXT fnValidationLayerCallback;
    void* validationLayerCallbackUserPtr;
    const char* const* enabledInstanceExtensionNames;
    uint32_t enabledInstanceExtensionCount;
    VexFnCreateSurface fnCreateMainSurfaceCallback;
    void* createMainSurfaceCallbackUserPtr;
    const VkPhysicalDeviceFeatures* physicalDeviceFeatures;
};

bool VexCreateDevice(const VexDeviceCreateInfo* info, VexDevice* outDevice);

struct VexDeviceBuilder
{
    VexDeviceBuilder();

    VexDeviceBuilder& SetMinVersion(uint32_t major, uint32_t minor);

    VexDeviceBuilder& EnableValidationLayers();

    VexDeviceBuilder& SetValidationLayersCallback(PFN_vkDebugUtilsMessengerCallbackEXT fnCallback, void* userPtr);

    VexDeviceBuilder& EnableInstanceExtensions(const char* const* extensionNames, uint32_t extensionCount);

    VexDeviceBuilder& SetCreateMainSurfaceCallback(VexFnCreateSurface fnCallback, void* userPtr);

    VexDeviceBuilder& SetPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures* features);

    bool Build(VexDevice* outDevice);

    VexDeviceCreateInfo info;
};