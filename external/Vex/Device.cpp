
#include <pch.hpp>
#include <VkBootstrap/VkBootstrap.h>
#include <Vex/Device.hpp>

static const char* ToString(VkDebugUtilsMessageSeverityFlagBitsEXT s)
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

static const char* ToString(VkDebugUtilsMessageTypeFlagsEXT s)
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

static VKAPI_ATTR VkBool32 VKAPI_CALL DefaultValidationLayersCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*)
{
    const char* severityStr = ToString(messageSeverity);
    const char* typeStr = ToString(messageType);
    printf("[%s: %s]\n%s\n", severityStr, typeStr, pCallbackData->pMessage);

    return VK_FALSE; // Applications must return false here
}

static void LoadFullProcTable(PFN_vkGetInstanceProcAddr fnGetProcAddr, VkInstance instance, VkDevice device, VexProcTable* outTable)
{
    VexInitProcTable(fnGetProcAddr, outTable);
    VexLoadInstanceProcTable(fnGetProcAddr, instance, outTable);
    VexLoadDeviceProcTable(outTable->GetDeviceProcAddr, device, outTable);
}

static bool CreateInstance(const VexDeviceCreateInfo* info, PFN_vkGetInstanceProcAddr fnGetInstanceProcAddr, vkb::Instance* outInstance)
{
    vkb::InstanceBuilder builder(fnGetInstanceProcAddr);
    builder.require_api_version(info->minVersionMajor, info->minVersionMinor);

    if (info->enableValidationLayers)
    {
        builder.request_validation_layers();
        builder.use_default_debug_messenger();
    }

    if (info->fnValidationLayerCallback)
    {
        builder.set_debug_callback(info->fnValidationLayerCallback);
    }

    if (info->validationLayerCallbackUserPtr)
    {
        builder.set_debug_callback_user_data_pointer(info->validationLayerCallbackUserPtr);
    }

    for (uint32_t i = 0; i < info->enabledInstanceExtensionCount; ++i)
    {
        builder.enable_extension(info->enabledInstanceExtensionNames[i]);
    }

    auto ret = builder.build();
    if (!ret)
    {
        printf("Failed to initialize Vulkan Instance: %s\n", ret.error().message().c_str());
        return false;
    }

    *outInstance = std::move(ret.value());
    return true;
}

static bool SelectPhysicalDevice(const VexDeviceCreateInfo* info, const vkb::Instance* instance, vkb::PhysicalDevice* outPhysicalDevice)
{
    VkSurfaceKHR surface = info->fnCreateMainSurfaceCallback(instance->instance, instance->allocation_callbacks, info->createMainSurfaceCallbackUserPtr);
    if (surface == VK_NULL_HANDLE)
        return false;

    vkb::PhysicalDeviceSelector selector(*instance);
    selector.set_surface(surface);
    selector.set_minimum_version(info->minVersionMajor, info->minVersionMinor);

    if (info->physicalDeviceFeatures)
    {
        selector.set_required_features(*info->physicalDeviceFeatures);
    }

    auto ret = selector.select();
    if (!ret)
    {
        printf("Failed to initialize Vulkan PhysicalDevice: %s\n", ret.error().message().c_str());
        return false;
    }

    *outPhysicalDevice = std::move(ret.value());
    return true;
}

static bool CreateDevice(const VexDeviceCreateInfo*, const vkb::PhysicalDevice* physDevice, vkb::Device* outDevice)
{
    vkb::DeviceBuilder builder(*physDevice);

    auto ret = builder.build();
    if (!ret)
    {
        printf("Failed to initialize Vulkan Device: %s\n", ret.error().message().c_str());
        return false;
    }

    *outDevice = ret.value();
    return true;
}

static bool GetQueue(const VexDeviceCreateInfo*, vkb::Device* device, vkb::QueueType type, VkQueue* outQueue, uint32_t* outQueueFamilyIndex)
{
    auto retQueue = device->get_queue(type);
    if (!retQueue)
    {
        printf("Failed to retrieve Vulkan Queue: %s\n", retQueue.error().message().c_str());
        return false;
    }

    auto retFamilyIndex = device->get_queue_index(type);
    if (!retFamilyIndex)
    {
        printf("Failed to retrieve Vulkan Queue family index: %s\n", retQueue.error().message().c_str());
        return false;
    }

    *outQueue = retQueue.value();
    *outQueueFamilyIndex = retFamilyIndex.value();
    return true;
}

bool VexCreateDevice(const VexDeviceCreateInfo* info, VexDevice* outDevice)
{
    PFN_vkGetInstanceProcAddr fnGetInstanceProcAddr = VexFindInstanceProcAddr();

    vkb::Instance vkbInstance;
    if (!CreateInstance(info, fnGetInstanceProcAddr, &vkbInstance))
        return false;

    vkb::PhysicalDevice vkbPhysDevice;
    if (!SelectPhysicalDevice(info, &vkbInstance, &vkbPhysDevice))
        return false;

    vkb::Device vkbDevice;
    if (!CreateDevice(info, &vkbPhysDevice, &vkbDevice))
        return false;

    VkQueue graphicsQueue;
    uint32_t graphicsQueueFamilyIndex;
    if (!GetQueue(info, &vkbDevice, vkb::QueueType::graphics, &graphicsQueue, &graphicsQueueFamilyIndex))
        return false;

    VkQueue presentQueue;
    uint32_t presentQueueFamilyIndex;
    if (!GetQueue(info, &vkbDevice, vkb::QueueType::present, &presentQueue, &presentQueueFamilyIndex))
        return false;

    VexDevice& vexDevice = *outDevice;
    LoadFullProcTable(fnGetInstanceProcAddr, vkbInstance.instance, vkbDevice.device, &vexDevice.procTable);
    vexDevice.instance = vkbInstance.instance;
    vexDevice.debugMessenger = vkbInstance.debug_messenger;
    vexDevice.physDevice = vkbPhysDevice.physical_device;
    vexDevice.mainSurface = vkbPhysDevice.surface;
    vexDevice.device = vkbDevice.device;
    vexDevice.graphicsQueue = graphicsQueue;
    vexDevice.graphicsQueueFamilyIndex = graphicsQueueFamilyIndex;
    vexDevice.presentQueueFamilyIndex = presentQueueFamilyIndex;
    vexDevice.presentQueue = presentQueue;
    vexDevice.allocator = nullptr;

    return true;
}

VexDeviceBuilder::VexDeviceBuilder()
{
    memset(this, 0, sizeof(*this));

    this->SetMinVersion(1, 0);
    this->SetValidationLayersCallback(DefaultValidationLayersCallback, nullptr);
}

VexDeviceBuilder& VexDeviceBuilder::SetMinVersion(uint32_t major, uint32_t minor)
{
    this->info.minVersionMajor = major;
    this->info.minVersionMinor = minor;
    return *this;
}

VexDeviceBuilder& VexDeviceBuilder::EnableValidationLayers()
{
    this->info.enableValidationLayers = true;
    return *this;
}

VexDeviceBuilder& VexDeviceBuilder::SetValidationLayersCallback(PFN_vkDebugUtilsMessengerCallbackEXT fnCallback, void* userPtr)
{
    this->info.fnValidationLayerCallback = fnCallback;
    this->info.validationLayerCallbackUserPtr = userPtr;
    return *this;
}

VexDeviceBuilder& VexDeviceBuilder::EnableInstanceExtensions(const char* const* extensionNames, uint32_t extensionCount)
{
    this->info.enabledInstanceExtensionNames = extensionNames;
    this->info.enabledInstanceExtensionCount = extensionCount;
    return *this;
}

VexDeviceBuilder& VexDeviceBuilder::SetCreateMainSurfaceCallback(VexFnCreateSurface fnCallback, void* userPtr)
{
    this->info.fnCreateMainSurfaceCallback = fnCallback;
    this->info.createMainSurfaceCallbackUserPtr = userPtr;
    return *this;
}

VexDeviceBuilder& VexDeviceBuilder::SetPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures* features)
{
    this->info.physicalDeviceFeatures = features;
    return *this;
}

bool VexDeviceBuilder::Build(VexDevice* outDevice)
{
    return VexCreateDevice(&this->info, outDevice);
}

VexDevice::VexDevice()
{
    this->instance = VK_NULL_HANDLE;
    this->debugMessenger = VK_NULL_HANDLE;
    this->physDevice = VK_NULL_HANDLE;
    this->mainSurface = VK_NULL_HANDLE;
    this->device = VK_NULL_HANDLE;
    this->allocator = nullptr;
    this->graphicsQueue = 0;
    this->presentQueue = 0;
    this->graphicsQueueFamilyIndex = 0;
    this->presentQueueFamilyIndex = 0;
    memset(&this->procTable, 0, sizeof(this->procTable));
}

void VexDevice::Destroy()
{
    this->procTable.DestroyDevice(this->device, this->allocator);

    this->procTable.DestroySurfaceKHR(this->instance, this->mainSurface, this->allocator);

    if (this->procTable.DestroyDebugUtilsMessengerEXT)
    {
        this->procTable.DestroyDebugUtilsMessengerEXT(this->instance, this->debugMessenger, this->allocator);
    }

    this->procTable.DestroyInstance(this->instance, this->allocator);
}

void VexDevice::GetPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties* pMemoryProperties) const
{
    if (this->procTable.GetPhysicalDeviceMemoryProperties != nullptr)
    {
        this->procTable.GetPhysicalDeviceMemoryProperties(this->physDevice, pMemoryProperties);
    }
    else if (this->procTable.GetPhysicalDeviceMemoryProperties2 != nullptr)
    {
        VkPhysicalDeviceMemoryProperties2 ret = {};
        ret.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
        this->procTable.GetPhysicalDeviceMemoryProperties2(this->physDevice, &ret);
        *pMemoryProperties = ret.memoryProperties;
    }
    else
    {
        assert(false && "Failed to retrieve VkPhysicalDeviceMemoryProperties");
    }
}

static void InitVexBuffer(const VkBufferCreateInfo& info, VkBuffer handle, VexBuffer& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.size = info.size;
    vexObj.usage = info.usage;
    vexObj.sharingMode = info.sharingMode;
    vexObj.queueFamilyIndices = std::vector<uint32_t>(info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount);
}

static void InitVexBufferView(const VkBufferViewCreateInfo& info, VkBufferView handle, VexBufferView& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.buffer = info.buffer;
    vexObj.format = info.format;
    vexObj.offset = info.offset;
    vexObj.range = info.range;
}

static void InitVexCommandBuffer(const VkCommandBufferAllocateInfo& info, VkCommandBuffer handle, VexCommandBuffer& vexObj)
{
    vexObj.handle = handle;
    vexObj.pool = info.commandPool;
    vexObj.level = info.level;
}

static void InitVexCommandPool(const VkCommandPoolCreateInfo& info, VkCommandPool handle, VexCommandPool& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.queueFamilyIndex = info.queueFamilyIndex;
}

static void InitVexDescriptorPool(const VkDescriptorPoolCreateInfo& info, VkDescriptorPool handle, VexDescriptorPool& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.maxSets = info.maxSets;
    vexObj.poolSizes = std::vector<VkDescriptorPoolSize>(info.pPoolSizes, info.pPoolSizes + info.poolSizeCount);
}

static void InitVexDescriptorSet(const VkDescriptorSetAllocateInfo& info, VkDescriptorSet handle, VexDescriptorSet& vexObj)
{
    vexObj.handle = handle;
    vexObj.descriptorPool = info.descriptorPool;
    vexObj.setLayouts = std::vector<VkDescriptorSetLayout>(info.pSetLayouts, info.pSetLayouts + info.descriptorSetCount);
}

static void InitVexDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& info, VkDescriptorSetLayout handle, VexDescriptorSetLayout& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
}

static void InitVexDeviceMemory(const VkMemoryAllocateInfo& info, VkDeviceMemory handle, VexDeviceMemory& vexObj)
{
    vexObj.handle = handle;
    vexObj.allocationSize = info.allocationSize;
    vexObj.memoryTypeIndex = info.memoryTypeIndex;
}

static void InitVexFramebuffer(const VkFramebufferCreateInfo& info, VkFramebuffer handle, VexFramebuffer& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.width = info.width;
    vexObj.height = info.height;
    vexObj.layers = info.layers;
    vexObj.renderPass = info.renderPass;
    vexObj.attachments = std::vector<VkImageView>(info.pAttachments, info.pAttachments + info.attachmentCount);
}

static void InitVexImage(const VkImageCreateInfo& info, VkImage handle, VexImage& vexImage)
{
    vexImage.handle = handle;
    vexImage.flags = info.flags;
    vexImage.imageType = info.imageType;
    vexImage.format = info.format;
    vexImage.extent = info.extent;
    vexImage.mipLevels = info.mipLevels;
    vexImage.arrayLayers = info.arrayLayers;
    vexImage.samples = info.samples;
    vexImage.tiling = info.tiling;
    vexImage.usage = info.usage;
    vexImage.sharingMode = info.sharingMode;
    vexImage.queueFamilyIndices = std::vector<uint32_t>(info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount);
    vexImage.initialLayout = info.initialLayout;
}

static void InitVexImageView(const VkImageViewCreateInfo& info, VkImageView handle, VexImageView& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.image = info.image;
    vexObj.viewType = info.viewType;
    vexObj.format = info.format;
    vexObj.components = info.components;
    vexObj.subresourceRange = info.subresourceRange;
}

static void InitVexPipelineCache(const VkPipelineCacheCreateInfo& info, VkPipelineCache handle, VexPipelineCache& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
}

static void InitVexPipelineLayout(const VkPipelineLayoutCreateInfo& info, VkPipelineLayout handle, VexPipelineLayout& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.setLayouts = std::vector<VkDescriptorSetLayout>(info.pSetLayouts, info.pSetLayouts + info.setLayoutCount);
    vexObj.pushConstantRanges = std::vector<VkPushConstantRange>(info.pPushConstantRanges, info.pPushConstantRanges + info.pushConstantRangeCount);
}

static void InitVexRenderPass(const VkRenderPassCreateInfo& info, VkRenderPass handle, VexRenderPass& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.attachments = std::vector<VkAttachmentDescription>(info.pAttachments, info.pAttachments + info.attachmentCount);
    vexObj.dependencies = std::vector<VkSubpassDependency>(info.pDependencies, info.pDependencies + info.dependencyCount);
}

static void InitVexSampler(const VkSamplerCreateInfo& info, VkSampler handle, VexSampler& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = vexObj.flags;
    vexObj.magFilter = info.magFilter;
    vexObj.minFilter = info.minFilter;
    vexObj.mipmapMode = info.mipmapMode;
    vexObj.addressModeU = info.addressModeU;
    vexObj.addressModeV = info.addressModeV;
    vexObj.addressModeW = info.addressModeW;
    vexObj.mipLodBias = info.mipLodBias;
    vexObj.anisotropyEnable = info.anisotropyEnable;
    vexObj.maxAnisotropy = info.maxAnisotropy;
    vexObj.compareEnable = info.compareEnable;
    vexObj.compareOp = info.compareOp;
    vexObj.minLod = info.minLod;
    vexObj.maxLod = info.maxLod;
    vexObj.borderColor = info.borderColor;
    vexObj.unnormalizedCoordinates = info.unnormalizedCoordinates;
}

static void InitVexShaderModule(const VkShaderModuleCreateInfo& info, VkShaderModule handle, VexShaderModule& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
}

static void InitVexSwapchainKHR(const VkSwapchainCreateInfoKHR& info, VkSwapchainKHR handle, VexSwapchainKHR& vexObj)
{
    vexObj.handle = handle;
    vexObj.flags = info.flags;
    vexObj.surface = info.surface;
    vexObj.minImageCount = info.minImageCount;
    vexObj.imageFormat = info.imageFormat;
    vexObj.imageColorSpace = info.imageColorSpace;
    vexObj.imageExtent = info.imageExtent;
    vexObj.imageArrayLayers = info.imageArrayLayers;
    vexObj.imageUsage = info.imageUsage;
    vexObj.imageSharingMode = info.imageSharingMode;
    vexObj.preTransform = info.preTransform;
    vexObj.compositeAlpha = info.compositeAlpha;
    vexObj.presentMode = info.presentMode;
    vexObj.clipped = info.clipped;
    vexObj.queueFamilyIndices = std::vector<uint32_t>(info.pQueueFamilyIndices, info.pQueueFamilyIndices + info.queueFamilyIndexCount);
}

void VexDevice::RegisterDeviceMemory(const VkMemoryAllocateInfo* pCreateInfo, VkDeviceMemory handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexDeviceMemory* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapDeviceMemory, handle);

    if (vexObj != nullptr)
        InitVexDeviceMemory(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::AllocateMemory(const VkMemoryAllocateInfo* pCreateInfo, VkDeviceMemory* pHandle)
{
    VkResult result = this->procTable.AllocateMemory(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterDeviceMemory(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterDeviceMemory(VkDeviceMemory handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapDeviceMemory, handle);
    }
}

void VexDevice::FreeMemory(VkDeviceMemory handle)
{
    this->DeregisterDeviceMemory(handle);
    this->procTable.FreeMemory(this->device, handle, this->allocator);
}

const VexDeviceMemory& VexDevice::GetVexDeviceMemory(VkDeviceMemory handle) const
{
    return VexThreadedHashMapAt(objTable.mapDeviceMemory, handle);
}

void VexDevice::RegisterCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, const VkCommandBuffer* pCommandBuffers)
{
    auto fnInitVexCmdBuffer = [pAllocateInfo, pCommandBuffers](VkCommandBuffer vkCmdBuffer, VexCommandBuffer& vexCmdBuffer)
    {
        InitVexCommandBuffer(*pAllocateInfo, vkCmdBuffer, vexCmdBuffer);
    };

    VexThreadedHashMapTryAddRange(this->objTable.mapCommandBuffer, pAllocateInfo->commandBufferCount, pCommandBuffers, fnInitVexCmdBuffer);
}

VkResult VexDevice::AllocateCommandBuffers(const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers)
{
    VkResult result = this->procTable.AllocateCommandBuffers(this->device, pAllocateInfo, pCommandBuffers);
    this->RegisterCommandBuffers(pAllocateInfo, pCommandBuffers);
    return result;
}

void VexDevice::DeregisterCommandBuffers(uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    VexThreadedHashMapRemoveRange(this->objTable.mapCommandBuffer, commandBufferCount, pCommandBuffers);
}

void VexDevice::FreeCommandBuffers(VkCommandPool commandPool, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers)
{
    // Deregister after FreeCommandBuffers to handle the case where pCommandBuffers == &VexCommandBuffer::handle
    this->procTable.FreeCommandBuffers(this->device, commandPool, commandBufferCount, pCommandBuffers);
    this->DeregisterCommandBuffers(commandBufferCount, pCommandBuffers);
}

const VexCommandBuffer& VexDevice::GetVexCommandBuffer(VkCommandBuffer handle) const
{
    return VexThreadedHashMapAt(objTable.mapCommandBuffer, handle);
}

void VexDevice::RegisterDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, const VkDescriptorSet* pDescriptorSets)
{
    auto fnInitVexDscSet = [pAllocateInfo, pDescriptorSets](VkDescriptorSet vkDscSet, VexDescriptorSet& vexDscSet)
    {
        InitVexDescriptorSet(*pAllocateInfo, vkDscSet, vexDscSet);
    };

    VexThreadedHashMapTryAddRange(this->objTable.mapDescriptorSet, pAllocateInfo->descriptorSetCount, pDescriptorSets, fnInitVexDscSet);
}

VkResult VexDevice::AllocateDescriptorSets(const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets)
{
    VkResult result = this->procTable.AllocateDescriptorSets(this->device, pAllocateInfo, pDescriptorSets);
    this->RegisterDescriptorSets(pAllocateInfo, pDescriptorSets);
    return result;
}

void VexDevice::DeregisterDescriptorSets(uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    VexThreadedHashMapRemoveRange(this->objTable.mapDescriptorSet, descriptorSetCount, pDescriptorSets);
}

void VexDevice::FreeDescriptorSets(VkDescriptorPool descriptorPool, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets)
{
    // Deregister after FreeDescriptorSets to handle the case where pDescriptorSets == &VexDescriptorSet::handle
    this->procTable.FreeDescriptorSets(this->device, descriptorPool, descriptorSetCount, pDescriptorSets);
    this->DeregisterDescriptorSets(descriptorSetCount, pDescriptorSets);
}

const VexDescriptorSet& VexDevice::GetVexDescriptorSet(VkDescriptorSet handle) const
{
    return VexThreadedHashMapAt(objTable.mapDescriptorSet, handle);
}

/* VEX_GENERATE_VEX_OBJ_FUNCS_C */

void VexDevice::RegisterBuffer(const VkBufferCreateInfo* pCreateInfo, VkBuffer handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexBuffer* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapBuffer, handle);

    if (vexObj != nullptr)
        InitVexBuffer(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateBuffer(const VkBufferCreateInfo* pCreateInfo, VkBuffer* pHandle)
{
    VkResult result = this->procTable.CreateBuffer(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterBuffer(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterBuffer(VkBuffer handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapBuffer, handle);
    }
}

void VexDevice::DestroyBuffer(VkBuffer handle)
{
    this->DeregisterBuffer(handle);
    this->procTable.DestroyBuffer(this->device, handle, this->allocator);
}

const VexBuffer& VexDevice::GetVexBuffer(VkBuffer handle) const
{
    return VexThreadedHashMapAt(objTable.mapBuffer, handle);
}

void VexDevice::RegisterBufferView(const VkBufferViewCreateInfo* pCreateInfo, VkBufferView handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexBufferView* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapBufferView, handle);

    if (vexObj != nullptr)
        InitVexBufferView(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateBufferView(const VkBufferViewCreateInfo* pCreateInfo, VkBufferView* pHandle)
{
    VkResult result = this->procTable.CreateBufferView(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterBufferView(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterBufferView(VkBufferView handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapBufferView, handle);
    }
}

void VexDevice::DestroyBufferView(VkBufferView handle)
{
    this->DeregisterBufferView(handle);
    this->procTable.DestroyBufferView(this->device, handle, this->allocator);
}

const VexBufferView& VexDevice::GetVexBufferView(VkBufferView handle) const
{
    return VexThreadedHashMapAt(objTable.mapBufferView, handle);
}

void VexDevice::RegisterCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, VkCommandPool handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexCommandPool* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapCommandPool, handle);

    if (vexObj != nullptr)
        InitVexCommandPool(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateCommandPool(const VkCommandPoolCreateInfo* pCreateInfo, VkCommandPool* pHandle)
{
    VkResult result = this->procTable.CreateCommandPool(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterCommandPool(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterCommandPool(VkCommandPool handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapCommandPool, handle);
    }
}

void VexDevice::DestroyCommandPool(VkCommandPool handle)
{
    this->DeregisterCommandPool(handle);
    this->procTable.DestroyCommandPool(this->device, handle, this->allocator);
}

const VexCommandPool& VexDevice::GetVexCommandPool(VkCommandPool handle) const
{
    return VexThreadedHashMapAt(objTable.mapCommandPool, handle);
}

void VexDevice::RegisterDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, VkDescriptorPool handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexDescriptorPool* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapDescriptorPool, handle);

    if (vexObj != nullptr)
        InitVexDescriptorPool(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateDescriptorPool(const VkDescriptorPoolCreateInfo* pCreateInfo, VkDescriptorPool* pHandle)
{
    VkResult result = this->procTable.CreateDescriptorPool(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterDescriptorPool(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterDescriptorPool(VkDescriptorPool handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapDescriptorPool, handle);
    }
}

void VexDevice::DestroyDescriptorPool(VkDescriptorPool handle)
{
    this->DeregisterDescriptorPool(handle);
    this->procTable.DestroyDescriptorPool(this->device, handle, this->allocator);
}

const VexDescriptorPool& VexDevice::GetVexDescriptorPool(VkDescriptorPool handle) const
{
    return VexThreadedHashMapAt(objTable.mapDescriptorPool, handle);
}

void VexDevice::RegisterDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayout handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexDescriptorSetLayout* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapDescriptorSetLayout, handle);

    if (vexObj != nullptr)
        InitVexDescriptorSetLayout(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateDescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayout* pHandle)
{
    VkResult result = this->procTable.CreateDescriptorSetLayout(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterDescriptorSetLayout(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterDescriptorSetLayout(VkDescriptorSetLayout handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapDescriptorSetLayout, handle);
    }
}

void VexDevice::DestroyDescriptorSetLayout(VkDescriptorSetLayout handle)
{
    this->DeregisterDescriptorSetLayout(handle);
    this->procTable.DestroyDescriptorSetLayout(this->device, handle, this->allocator);
}

const VexDescriptorSetLayout& VexDevice::GetVexDescriptorSetLayout(VkDescriptorSetLayout handle) const
{
    return VexThreadedHashMapAt(objTable.mapDescriptorSetLayout, handle);
}

void VexDevice::RegisterFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, VkFramebuffer handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexFramebuffer* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapFramebuffer, handle);

    if (vexObj != nullptr)
        InitVexFramebuffer(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateFramebuffer(const VkFramebufferCreateInfo* pCreateInfo, VkFramebuffer* pHandle)
{
    VkResult result = this->procTable.CreateFramebuffer(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterFramebuffer(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterFramebuffer(VkFramebuffer handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapFramebuffer, handle);
    }
}

void VexDevice::DestroyFramebuffer(VkFramebuffer handle)
{
    this->DeregisterFramebuffer(handle);
    this->procTable.DestroyFramebuffer(this->device, handle, this->allocator);
}

const VexFramebuffer& VexDevice::GetVexFramebuffer(VkFramebuffer handle) const
{
    return VexThreadedHashMapAt(objTable.mapFramebuffer, handle);
}

void VexDevice::RegisterImage(const VkImageCreateInfo* pCreateInfo, VkImage handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexImage* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapImage, handle);

    if (vexObj != nullptr)
        InitVexImage(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateImage(const VkImageCreateInfo* pCreateInfo, VkImage* pHandle)
{
    VkResult result = this->procTable.CreateImage(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterImage(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterImage(VkImage handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapImage, handle);
    }
}

void VexDevice::DestroyImage(VkImage handle)
{
    this->DeregisterImage(handle);
    this->procTable.DestroyImage(this->device, handle, this->allocator);
}

const VexImage& VexDevice::GetVexImage(VkImage handle) const
{
    return VexThreadedHashMapAt(objTable.mapImage, handle);
}

void VexDevice::RegisterImageView(const VkImageViewCreateInfo* pCreateInfo, VkImageView handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexImageView* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapImageView, handle);

    if (vexObj != nullptr)
        InitVexImageView(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateImageView(const VkImageViewCreateInfo* pCreateInfo, VkImageView* pHandle)
{
    VkResult result = this->procTable.CreateImageView(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterImageView(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterImageView(VkImageView handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapImageView, handle);
    }
}

void VexDevice::DestroyImageView(VkImageView handle)
{
    this->DeregisterImageView(handle);
    this->procTable.DestroyImageView(this->device, handle, this->allocator);
}

const VexImageView& VexDevice::GetVexImageView(VkImageView handle) const
{
    return VexThreadedHashMapAt(objTable.mapImageView, handle);
}

void VexDevice::RegisterPipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, VkPipelineCache handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexPipelineCache* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapPipelineCache, handle);

    if (vexObj != nullptr)
        InitVexPipelineCache(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreatePipelineCache(const VkPipelineCacheCreateInfo* pCreateInfo, VkPipelineCache* pHandle)
{
    VkResult result = this->procTable.CreatePipelineCache(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterPipelineCache(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterPipelineCache(VkPipelineCache handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapPipelineCache, handle);
    }
}

void VexDevice::DestroyPipelineCache(VkPipelineCache handle)
{
    this->DeregisterPipelineCache(handle);
    this->procTable.DestroyPipelineCache(this->device, handle, this->allocator);
}

const VexPipelineCache& VexDevice::GetVexPipelineCache(VkPipelineCache handle) const
{
    return VexThreadedHashMapAt(objTable.mapPipelineCache, handle);
}

void VexDevice::RegisterPipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, VkPipelineLayout handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexPipelineLayout* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapPipelineLayout, handle);

    if (vexObj != nullptr)
        InitVexPipelineLayout(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreatePipelineLayout(const VkPipelineLayoutCreateInfo* pCreateInfo, VkPipelineLayout* pHandle)
{
    VkResult result = this->procTable.CreatePipelineLayout(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterPipelineLayout(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterPipelineLayout(VkPipelineLayout handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapPipelineLayout, handle);
    }
}

void VexDevice::DestroyPipelineLayout(VkPipelineLayout handle)
{
    this->DeregisterPipelineLayout(handle);
    this->procTable.DestroyPipelineLayout(this->device, handle, this->allocator);
}

const VexPipelineLayout& VexDevice::GetVexPipelineLayout(VkPipelineLayout handle) const
{
    return VexThreadedHashMapAt(objTable.mapPipelineLayout, handle);
}

void VexDevice::RegisterRenderPass(const VkRenderPassCreateInfo* pCreateInfo, VkRenderPass handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexRenderPass* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapRenderPass, handle);

    if (vexObj != nullptr)
        InitVexRenderPass(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateRenderPass(const VkRenderPassCreateInfo* pCreateInfo, VkRenderPass* pHandle)
{
    VkResult result = this->procTable.CreateRenderPass(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterRenderPass(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterRenderPass(VkRenderPass handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapRenderPass, handle);
    }
}

void VexDevice::DestroyRenderPass(VkRenderPass handle)
{
    this->DeregisterRenderPass(handle);
    this->procTable.DestroyRenderPass(this->device, handle, this->allocator);
}

const VexRenderPass& VexDevice::GetVexRenderPass(VkRenderPass handle) const
{
    return VexThreadedHashMapAt(objTable.mapRenderPass, handle);
}

void VexDevice::RegisterSampler(const VkSamplerCreateInfo* pCreateInfo, VkSampler handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexSampler* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapSampler, handle);

    if (vexObj != nullptr)
        InitVexSampler(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateSampler(const VkSamplerCreateInfo* pCreateInfo, VkSampler* pHandle)
{
    VkResult result = this->procTable.CreateSampler(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterSampler(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterSampler(VkSampler handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapSampler, handle);
    }
}

void VexDevice::DestroySampler(VkSampler handle)
{
    this->DeregisterSampler(handle);
    this->procTable.DestroySampler(this->device, handle, this->allocator);
}

const VexSampler& VexDevice::GetVexSampler(VkSampler handle) const
{
    return VexThreadedHashMapAt(objTable.mapSampler, handle);
}

void VexDevice::RegisterShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModule handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexShaderModule* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapShaderModule, handle);

    if (vexObj != nullptr)
        InitVexShaderModule(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateShaderModule(const VkShaderModuleCreateInfo* pCreateInfo, VkShaderModule* pHandle)
{
    VkResult result = this->procTable.CreateShaderModule(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterShaderModule(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterShaderModule(VkShaderModule handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapShaderModule, handle);
    }
}

void VexDevice::DestroyShaderModule(VkShaderModule handle)
{
    this->DeregisterShaderModule(handle);
    this->procTable.DestroyShaderModule(this->device, handle, this->allocator);
}

const VexShaderModule& VexDevice::GetVexShaderModule(VkShaderModule handle) const
{
    return VexThreadedHashMapAt(objTable.mapShaderModule, handle);
}

void VexDevice::RegisterSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, VkSwapchainKHR handle)
{
    if (handle == VK_NULL_HANDLE)
        return;

    VexSwapchainKHR* vexObj = VexThreadedHashMapTryAdd(this->objTable.mapSwapchainKHR, handle);

    if (vexObj != nullptr)
        InitVexSwapchainKHR(*pCreateInfo, handle, *vexObj);
}

VkResult VexDevice::CreateSwapchainKHR(const VkSwapchainCreateInfoKHR* pCreateInfo, VkSwapchainKHR* pHandle)
{
    VkResult result = this->procTable.CreateSwapchainKHR(this->device, pCreateInfo, this->allocator, pHandle);
    this->RegisterSwapchainKHR(pCreateInfo, *pHandle);
    return result;
}

void VexDevice::DeregisterSwapchainKHR(VkSwapchainKHR handle)
{
    if (handle != VK_NULL_HANDLE)
    {
        VexThreadedHashMapRemove(this->objTable.mapSwapchainKHR, handle);
    }
}

void VexDevice::DestroySwapchainKHR(VkSwapchainKHR handle)
{
    this->DeregisterSwapchainKHR(handle);
    this->procTable.DestroySwapchainKHR(this->device, handle, this->allocator);
}

const VexSwapchainKHR& VexDevice::GetVexSwapchainKHR(VkSwapchainKHR handle) const
{
    return VexThreadedHashMapAt(objTable.mapSwapchainKHR, handle);
}

/* VEX_GENERATE_VEX_OBJ_FUNCS_C */

/* VEX_GENERATE_DEVICE_WRAPPERS_C */
#if defined(VK_VERSION_1_0)
VkResult VexDevice::EnumeratePhysicalDevices(uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices) const
{
    return this->procTable.EnumeratePhysicalDevices(this->instance, pPhysicalDeviceCount, pPhysicalDevices);
}

void VexDevice::GetPhysicalDeviceFeatures(VkPhysicalDeviceFeatures* pFeatures) const
{
    this->procTable.GetPhysicalDeviceFeatures(this->physDevice, pFeatures);
}

void VexDevice::GetPhysicalDeviceFormatProperties(VkFormat format, VkFormatProperties* pFormatProperties) const
{
    this->procTable.GetPhysicalDeviceFormatProperties(this->physDevice, format, pFormatProperties);
}

VkResult VexDevice::GetPhysicalDeviceImageFormatProperties(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) const
{
    return this->procTable.GetPhysicalDeviceImageFormatProperties(this->physDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

void VexDevice::GetPhysicalDeviceProperties(VkPhysicalDeviceProperties* pProperties) const
{
    this->procTable.GetPhysicalDeviceProperties(this->physDevice, pProperties);
}

void VexDevice::GetPhysicalDeviceQueueFamilyProperties(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties* pQueueFamilyProperties) const
{
    this->procTable.GetPhysicalDeviceQueueFamilyProperties(this->physDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

PFN_vkVoidFunction VexDevice::GetInstanceProcAddr(const char* pName) const
{
    return this->procTable.GetInstanceProcAddr(this->instance, pName);
}

PFN_vkVoidFunction VexDevice::GetDeviceProcAddr(const char* pName) const
{
    return this->procTable.GetDeviceProcAddr(this->device, pName);
}

VkResult VexDevice::EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const
{
    return this->procTable.EnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}

VkResult VexDevice::EnumerateDeviceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) const
{
    return this->procTable.EnumerateDeviceExtensionProperties(this->physDevice, pLayerName, pPropertyCount, pProperties);
}

VkResult VexDevice::EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) const
{
    return this->procTable.EnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VkResult VexDevice::EnumerateDeviceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) const
{
    return this->procTable.EnumerateDeviceLayerProperties(this->physDevice, pPropertyCount, pProperties);
}

void VexDevice::GetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue) const
{
    this->procTable.GetDeviceQueue(this->device, queueFamilyIndex, queueIndex, pQueue);
}

VkResult VexDevice::QueueSubmit(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) const
{
    return this->procTable.QueueSubmit(queue, submitCount, pSubmits, fence);
}

VkResult VexDevice::QueueWaitIdle(VkQueue queue) const
{
    return this->procTable.QueueWaitIdle(queue);
}

VkResult VexDevice::DeviceWaitIdle() const
{
    return this->procTable.DeviceWaitIdle(this->device);
}

VkResult VexDevice::MapMemory(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) const
{
    return this->procTable.MapMemory(this->device, memory, offset, size, flags, ppData);
}

void VexDevice::UnmapMemory(VkDeviceMemory memory) const
{
    this->procTable.UnmapMemory(this->device, memory);
}

VkResult VexDevice::FlushMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) const
{
    return this->procTable.FlushMappedMemoryRanges(this->device, memoryRangeCount, pMemoryRanges);
}

VkResult VexDevice::InvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const VkMappedMemoryRange* pMemoryRanges) const
{
    return this->procTable.InvalidateMappedMemoryRanges(this->device, memoryRangeCount, pMemoryRanges);
}

void VexDevice::GetDeviceMemoryCommitment(VkDeviceMemory memory, VkDeviceSize* pCommittedMemoryInBytes) const
{
    this->procTable.GetDeviceMemoryCommitment(this->device, memory, pCommittedMemoryInBytes);
}

VkResult VexDevice::BindBufferMemory(VkBuffer buffer, VkDeviceMemory memory, VkDeviceSize memoryOffset) const
{
    return this->procTable.BindBufferMemory(this->device, buffer, memory, memoryOffset);
}

VkResult VexDevice::BindImageMemory(VkImage image, VkDeviceMemory memory, VkDeviceSize memoryOffset) const
{
    return this->procTable.BindImageMemory(this->device, image, memory, memoryOffset);
}

void VexDevice::GetBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements) const
{
    this->procTable.GetBufferMemoryRequirements(this->device, buffer, pMemoryRequirements);
}

void VexDevice::GetImageMemoryRequirements(VkImage image, VkMemoryRequirements* pMemoryRequirements) const
{
    this->procTable.GetImageMemoryRequirements(this->device, image, pMemoryRequirements);
}

void VexDevice::GetImageSparseMemoryRequirements(VkImage image, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements* pSparseMemoryRequirements) const
{
    this->procTable.GetImageSparseMemoryRequirements(this->device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void VexDevice::GetPhysicalDeviceSparseImageFormatProperties(VkFormat format, VkImageType type, VkSampleCountFlagBits samples, VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount, VkSparseImageFormatProperties* pProperties) const
{
    this->procTable.GetPhysicalDeviceSparseImageFormatProperties(this->physDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}

VkResult VexDevice::QueueBindSparse(VkQueue queue, uint32_t bindInfoCount, const VkBindSparseInfo* pBindInfo, VkFence fence) const
{
    return this->procTable.QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}

VkResult VexDevice::CreateFence(const VkFenceCreateInfo* pCreateInfo, VkFence* pFence) const
{
    return this->procTable.CreateFence(this->device, pCreateInfo, this->allocator, pFence);
}

void VexDevice::DestroyFence(VkFence fence) const
{
    this->procTable.DestroyFence(this->device, fence, this->allocator);
}

VkResult VexDevice::ResetFences(uint32_t fenceCount, const VkFence* pFences) const
{
    return this->procTable.ResetFences(this->device, fenceCount, pFences);
}

VkResult VexDevice::GetFenceStatus(VkFence fence) const
{
    return this->procTable.GetFenceStatus(this->device, fence);
}

VkResult VexDevice::WaitForFences(uint32_t fenceCount, const VkFence* pFences, VkBool32 waitAll, uint64_t timeout) const
{
    return this->procTable.WaitForFences(this->device, fenceCount, pFences, waitAll, timeout);
}

VkResult VexDevice::CreateSemaphore(const VkSemaphoreCreateInfo* pCreateInfo, VkSemaphore* pSemaphore) const
{
    return this->procTable.CreateSemaphore(this->device, pCreateInfo, this->allocator, pSemaphore);
}

void VexDevice::DestroySemaphore(VkSemaphore semaphore) const
{
    this->procTable.DestroySemaphore(this->device, semaphore, this->allocator);
}

VkResult VexDevice::CreateEvent(const VkEventCreateInfo* pCreateInfo, VkEvent* pEvent) const
{
    return this->procTable.CreateEvent(this->device, pCreateInfo, this->allocator, pEvent);
}

void VexDevice::DestroyEvent(VkEvent event) const
{
    this->procTable.DestroyEvent(this->device, event, this->allocator);
}

VkResult VexDevice::GetEventStatus(VkEvent event) const
{
    return this->procTable.GetEventStatus(this->device, event);
}

VkResult VexDevice::SetEvent(VkEvent event) const
{
    return this->procTable.SetEvent(this->device, event);
}

VkResult VexDevice::ResetEvent(VkEvent event) const
{
    return this->procTable.ResetEvent(this->device, event);
}

VkResult VexDevice::CreateQueryPool(const VkQueryPoolCreateInfo* pCreateInfo, VkQueryPool* pQueryPool) const
{
    return this->procTable.CreateQueryPool(this->device, pCreateInfo, this->allocator, pQueryPool);
}

void VexDevice::DestroyQueryPool(VkQueryPool queryPool) const
{
    this->procTable.DestroyQueryPool(this->device, queryPool, this->allocator);
}

VkResult VexDevice::GetQueryPoolResults(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) const
{
    return this->procTable.GetQueryPoolResults(this->device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

void VexDevice::GetImageSubresourceLayout(VkImage image, const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) const
{
    this->procTable.GetImageSubresourceLayout(this->device, image, pSubresource, pLayout);
}

VkResult VexDevice::GetPipelineCacheData(VkPipelineCache pipelineCache, size_t* pDataSize, void* pData) const
{
    return this->procTable.GetPipelineCacheData(this->device, pipelineCache, pDataSize, pData);
}

VkResult VexDevice::MergePipelineCaches(VkPipelineCache dstCache, uint32_t srcCacheCount, const VkPipelineCache* pSrcCaches) const
{
    return this->procTable.MergePipelineCaches(this->device, dstCache, srcCacheCount, pSrcCaches);
}

VkResult VexDevice::CreateGraphicsPipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos, VkPipeline* pPipelines) const
{
    return this->procTable.CreateGraphicsPipelines(this->device, pipelineCache, createInfoCount, pCreateInfos, this->allocator, pPipelines);
}

VkResult VexDevice::CreateComputePipelines(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos, VkPipeline* pPipelines) const
{
    return this->procTable.CreateComputePipelines(this->device, pipelineCache, createInfoCount, pCreateInfos, this->allocator, pPipelines);
}

void VexDevice::DestroyPipeline(VkPipeline pipeline) const
{
    this->procTable.DestroyPipeline(this->device, pipeline, this->allocator);
}

VkResult VexDevice::ResetDescriptorPool(VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags) const
{
    return this->procTable.ResetDescriptorPool(this->device, descriptorPool, flags);
}

void VexDevice::UpdateDescriptorSets(uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const VkCopyDescriptorSet* pDescriptorCopies) const
{
    this->procTable.UpdateDescriptorSets(this->device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

void VexDevice::GetRenderAreaGranularity(VkRenderPass renderPass, VkExtent2D* pGranularity) const
{
    this->procTable.GetRenderAreaGranularity(this->device, renderPass, pGranularity);
}

VkResult VexDevice::ResetCommandPool(VkCommandPool commandPool, VkCommandPoolResetFlags flags) const
{
    return this->procTable.ResetCommandPool(this->device, commandPool, flags);
}

VkResult VexDevice::BeginCommandBuffer(VkCommandBuffer commandBuffer, const VkCommandBufferBeginInfo* pBeginInfo) const
{
    return this->procTable.BeginCommandBuffer(commandBuffer, pBeginInfo);
}

VkResult VexDevice::EndCommandBuffer(VkCommandBuffer commandBuffer) const
{
    return this->procTable.EndCommandBuffer(commandBuffer);
}

VkResult VexDevice::ResetCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) const
{
    return this->procTable.ResetCommandBuffer(commandBuffer, flags);
}

void VexDevice::CmdBindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline pipeline) const
{
    this->procTable.CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}

void VexDevice::CmdSetViewport(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewport* pViewports) const
{
    this->procTable.CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

void VexDevice::CmdSetScissor(VkCommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const VkRect2D* pScissors) const
{
    this->procTable.CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

void VexDevice::CmdSetLineWidth(VkCommandBuffer commandBuffer, float lineWidth) const
{
    this->procTable.CmdSetLineWidth(commandBuffer, lineWidth);
}

void VexDevice::CmdSetDepthBias(VkCommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const
{
    this->procTable.CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void VexDevice::CmdSetBlendConstants(VkCommandBuffer commandBuffer, const float blendConstants[4]) const
{
    this->procTable.CmdSetBlendConstants(commandBuffer, blendConstants);
}

void VexDevice::CmdSetDepthBounds(VkCommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) const
{
    this->procTable.CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}

void VexDevice::CmdSetStencilCompareMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t compareMask) const
{
    this->procTable.CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}

void VexDevice::CmdSetStencilWriteMask(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t writeMask) const
{
    this->procTable.CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}

void VexDevice::CmdSetStencilReference(VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask, uint32_t reference) const
{
    this->procTable.CmdSetStencilReference(commandBuffer, faceMask, reference);
}

void VexDevice::CmdBindDescriptorSets(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets) const
{
    this->procTable.CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void VexDevice::CmdBindIndexBuffer(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkIndexType indexType) const
{
    this->procTable.CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

void VexDevice::CmdBindVertexBuffers(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) const
{
    this->procTable.CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void VexDevice::CmdDraw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const
{
    this->procTable.CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VexDevice::CmdDrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const
{
    this->procTable.CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VexDevice::CmdDrawIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
{
    this->procTable.CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void VexDevice::CmdDrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
{
    this->procTable.CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void VexDevice::CmdDispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const
{
    this->procTable.CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void VexDevice::CmdDispatchIndirect(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset) const
{
    this->procTable.CmdDispatchIndirect(commandBuffer, buffer, offset);
}

void VexDevice::CmdCopyBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferCopy* pRegions) const
{
    this->procTable.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void VexDevice::CmdCopyImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageCopy* pRegions) const
{
    this->procTable.CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void VexDevice::CmdBlitImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageBlit* pRegions, VkFilter filter) const
{
    this->procTable.CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void VexDevice::CmdCopyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkBufferImageCopy* pRegions) const
{
    this->procTable.CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void VexDevice::CmdCopyImageToBuffer(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount, const VkBufferImageCopy* pRegions) const
{
    this->procTable.CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void VexDevice::CmdUpdateBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) const
{
    this->procTable.CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void VexDevice::CmdFillBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) const
{
    this->procTable.CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}

void VexDevice::CmdClearColorImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const
{
    this->procTable.CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void VexDevice::CmdClearDepthStencilImage(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount, const VkImageSubresourceRange* pRanges) const
{
    this->procTable.CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void VexDevice::CmdClearAttachments(VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) const
{
    this->procTable.CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void VexDevice::CmdResolveImage(VkCommandBuffer commandBuffer, VkImage srcImage, VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount, const VkImageResolve* pRegions) const
{
    this->procTable.CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void VexDevice::CmdSetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) const
{
    this->procTable.CmdSetEvent(commandBuffer, event, stageMask);
}

void VexDevice::CmdResetEvent(VkCommandBuffer commandBuffer, VkEvent event, VkPipelineStageFlags stageMask) const
{
    this->procTable.CmdResetEvent(commandBuffer, event, stageMask);
}

void VexDevice::CmdWaitEvents(VkCommandBuffer commandBuffer, uint32_t eventCount, const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const
{
    this->procTable.CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void VexDevice::CmdPipelineBarrier(VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers) const
{
    this->procTable.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void VexDevice::CmdBeginQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags) const
{
    this->procTable.CmdBeginQuery(commandBuffer, queryPool, query, flags);
}

void VexDevice::CmdEndQuery(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) const
{
    this->procTable.CmdEndQuery(commandBuffer, queryPool, query);
}

void VexDevice::CmdResetQueryPool(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const
{
    this->procTable.CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}

void VexDevice::CmdWriteTimestamp(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkQueryPool queryPool, uint32_t query) const
{
    this->procTable.CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}

void VexDevice::CmdCopyQueryPoolResults(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize stride, VkQueryResultFlags flags) const
{
    this->procTable.CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void VexDevice::CmdPushConstants(VkCommandBuffer commandBuffer, VkPipelineLayout layout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) const
{
    this->procTable.CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void VexDevice::CmdBeginRenderPass(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) const
{
    this->procTable.CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

void VexDevice::CmdNextSubpass(VkCommandBuffer commandBuffer, VkSubpassContents contents) const
{
    this->procTable.CmdNextSubpass(commandBuffer, contents);
}

void VexDevice::CmdEndRenderPass(VkCommandBuffer commandBuffer) const
{
    this->procTable.CmdEndRenderPass(commandBuffer);
}

void VexDevice::CmdExecuteCommands(VkCommandBuffer commandBuffer, uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) const
{
    this->procTable.CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}

#endif
#if defined(VK_VERSION_1_1)
VkResult VexDevice::EnumerateInstanceVersion(uint32_t* pApiVersion) const
{
    return this->procTable.EnumerateInstanceVersion(pApiVersion);
}

VkResult VexDevice::BindBufferMemory2(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) const
{
    return this->procTable.BindBufferMemory2(this->device, bindInfoCount, pBindInfos);
}

VkResult VexDevice::BindImageMemory2(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) const
{
    return this->procTable.BindImageMemory2(this->device, bindInfoCount, pBindInfos);
}

void VexDevice::GetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) const
{
    this->procTable.GetDeviceGroupPeerMemoryFeatures(this->device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void VexDevice::CmdSetDeviceMask(VkCommandBuffer commandBuffer, uint32_t deviceMask) const
{
    this->procTable.CmdSetDeviceMask(commandBuffer, deviceMask);
}

void VexDevice::CmdDispatchBase(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const
{
    this->procTable.CmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

VkResult VexDevice::EnumeratePhysicalDeviceGroups(uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) const
{
    return this->procTable.EnumeratePhysicalDeviceGroups(this->instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void VexDevice::GetImageMemoryRequirements2(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const
{
    this->procTable.GetImageMemoryRequirements2(this->device, pInfo, pMemoryRequirements);
}

void VexDevice::GetBufferMemoryRequirements2(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const
{
    this->procTable.GetBufferMemoryRequirements2(this->device, pInfo, pMemoryRequirements);
}

void VexDevice::GetImageSparseMemoryRequirements2(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) const
{
    this->procTable.GetImageSparseMemoryRequirements2(this->device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void VexDevice::GetPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* pFeatures) const
{
    this->procTable.GetPhysicalDeviceFeatures2(this->physDevice, pFeatures);
}

void VexDevice::GetPhysicalDeviceProperties2(VkPhysicalDeviceProperties2* pProperties) const
{
    this->procTable.GetPhysicalDeviceProperties2(this->physDevice, pProperties);
}

void VexDevice::GetPhysicalDeviceFormatProperties2(VkFormat format, VkFormatProperties2* pFormatProperties) const
{
    this->procTable.GetPhysicalDeviceFormatProperties2(this->physDevice, format, pFormatProperties);
}

VkResult VexDevice::GetPhysicalDeviceImageFormatProperties2(const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) const
{
    return this->procTable.GetPhysicalDeviceImageFormatProperties2(this->physDevice, pImageFormatInfo, pImageFormatProperties);
}

void VexDevice::GetPhysicalDeviceQueueFamilyProperties2(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const
{
    this->procTable.GetPhysicalDeviceQueueFamilyProperties2(this->physDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void VexDevice::GetPhysicalDeviceMemoryProperties2(VkPhysicalDeviceMemoryProperties2* pMemoryProperties) const
{
    this->procTable.GetPhysicalDeviceMemoryProperties2(this->physDevice, pMemoryProperties);
}

void VexDevice::GetPhysicalDeviceSparseImageFormatProperties2(const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) const
{
    this->procTable.GetPhysicalDeviceSparseImageFormatProperties2(this->physDevice, pFormatInfo, pPropertyCount, pProperties);
}

void VexDevice::TrimCommandPool(VkCommandPool commandPool, VkCommandPoolTrimFlags flags) const
{
    this->procTable.TrimCommandPool(this->device, commandPool, flags);
}

void VexDevice::GetDeviceQueue2(const VkDeviceQueueInfo2* pQueueInfo, VkQueue* pQueue) const
{
    this->procTable.GetDeviceQueue2(this->device, pQueueInfo, pQueue);
}

VkResult VexDevice::CreateSamplerYcbcrConversion(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkSamplerYcbcrConversion* pYcbcrConversion) const
{
    return this->procTable.CreateSamplerYcbcrConversion(this->device, pCreateInfo, this->allocator, pYcbcrConversion);
}

void VexDevice::DestroySamplerYcbcrConversion(VkSamplerYcbcrConversion ycbcrConversion) const
{
    this->procTable.DestroySamplerYcbcrConversion(this->device, ycbcrConversion, this->allocator);
}

VkResult VexDevice::CreateDescriptorUpdateTemplate(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) const
{
    return this->procTable.CreateDescriptorUpdateTemplate(this->device, pCreateInfo, this->allocator, pDescriptorUpdateTemplate);
}

void VexDevice::DestroyDescriptorUpdateTemplate(VkDescriptorUpdateTemplate descriptorUpdateTemplate) const
{
    this->procTable.DestroyDescriptorUpdateTemplate(this->device, descriptorUpdateTemplate, this->allocator);
}

void VexDevice::UpdateDescriptorSetWithTemplate(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const
{
    this->procTable.UpdateDescriptorSetWithTemplate(this->device, descriptorSet, descriptorUpdateTemplate, pData);
}

void VexDevice::GetPhysicalDeviceExternalBufferProperties(const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) const
{
    this->procTable.GetPhysicalDeviceExternalBufferProperties(this->physDevice, pExternalBufferInfo, pExternalBufferProperties);
}

void VexDevice::GetPhysicalDeviceExternalFenceProperties(const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) const
{
    this->procTable.GetPhysicalDeviceExternalFenceProperties(this->physDevice, pExternalFenceInfo, pExternalFenceProperties);
}

void VexDevice::GetPhysicalDeviceExternalSemaphoreProperties(const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) const
{
    this->procTable.GetPhysicalDeviceExternalSemaphoreProperties(this->physDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

void VexDevice::GetDescriptorSetLayoutSupport(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) const
{
    this->procTable.GetDescriptorSetLayoutSupport(this->device, pCreateInfo, pSupport);
}

#endif
#if defined(VK_KHR_surface)
void VexDevice::DestroySurfaceKHR(VkSurfaceKHR surface) const
{
    this->procTable.DestroySurfaceKHR(this->instance, surface, this->allocator);
}

VkResult VexDevice::GetPhysicalDeviceSurfaceSupportKHR(uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) const
{
    return this->procTable.GetPhysicalDeviceSurfaceSupportKHR(this->physDevice, queueFamilyIndex, surface, pSupported);
}

VkResult VexDevice::GetPhysicalDeviceSurfaceCapabilitiesKHR(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) const
{
    return this->procTable.GetPhysicalDeviceSurfaceCapabilitiesKHR(this->physDevice, surface, pSurfaceCapabilities);
}

VkResult VexDevice::GetPhysicalDeviceSurfaceFormatsKHR(VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats) const
{
    return this->procTable.GetPhysicalDeviceSurfaceFormatsKHR(this->physDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}

VkResult VexDevice::GetPhysicalDeviceSurfacePresentModesKHR(VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const
{
    return this->procTable.GetPhysicalDeviceSurfacePresentModesKHR(this->physDevice, surface, pPresentModeCount, pPresentModes);
}

#endif
#if defined(VK_KHR_swapchain)
VkResult VexDevice::GetSwapchainImagesKHR(VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) const
{
    return this->procTable.GetSwapchainImagesKHR(this->device, swapchain, pSwapchainImageCount, pSwapchainImages);
}

VkResult VexDevice::AcquireNextImageKHR(VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) const
{
    return this->procTable.AcquireNextImageKHR(this->device, swapchain, timeout, semaphore, fence, pImageIndex);
}

VkResult VexDevice::QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo) const
{
    return this->procTable.QueuePresentKHR(queue, pPresentInfo);
}

#endif
#if defined(VK_KHR_display)
VkResult VexDevice::GetPhysicalDeviceDisplayPropertiesKHR(uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) const
{
    return this->procTable.GetPhysicalDeviceDisplayPropertiesKHR(this->physDevice, pPropertyCount, pProperties);
}

VkResult VexDevice::GetPhysicalDeviceDisplayPlanePropertiesKHR(uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) const
{
    return this->procTable.GetPhysicalDeviceDisplayPlanePropertiesKHR(this->physDevice, pPropertyCount, pProperties);
}

VkResult VexDevice::GetDisplayPlaneSupportedDisplaysKHR(uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) const
{
    return this->procTable.GetDisplayPlaneSupportedDisplaysKHR(this->physDevice, planeIndex, pDisplayCount, pDisplays);
}

VkResult VexDevice::GetDisplayModePropertiesKHR(VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) const
{
    return this->procTable.GetDisplayModePropertiesKHR(this->physDevice, display, pPropertyCount, pProperties);
}

VkResult VexDevice::CreateDisplayModeKHR(VkDisplayKHR display, const VkDisplayModeCreateInfoKHR* pCreateInfo, VkDisplayModeKHR* pMode) const
{
    return this->procTable.CreateDisplayModeKHR(this->physDevice, display, pCreateInfo, this->allocator, pMode);
}

VkResult VexDevice::GetDisplayPlaneCapabilitiesKHR(VkDisplayModeKHR mode, uint32_t planeIndex, VkDisplayPlaneCapabilitiesKHR* pCapabilities) const
{
    return this->procTable.GetDisplayPlaneCapabilitiesKHR(this->physDevice, mode, planeIndex, pCapabilities);
}

VkResult VexDevice::CreateDisplayPlaneSurfaceKHR(const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateDisplayPlaneSurfaceKHR(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_KHR_display_swapchain)
VkResult VexDevice::CreateSharedSwapchainsKHR(uint32_t swapchainCount, const VkSwapchainCreateInfoKHR* pCreateInfos, VkSwapchainKHR* pSwapchains) const
{
    return this->procTable.CreateSharedSwapchainsKHR(this->device, swapchainCount, pCreateInfos, this->allocator, pSwapchains);
}

#endif
#if defined(VK_KHR_xlib_surface)
VkResult VexDevice::CreateXlibSurfaceKHR(const VkXlibSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateXlibSurfaceKHR(this->instance, pCreateInfo, this->allocator, pSurface);
}

VkBool32 VexDevice::GetPhysicalDeviceXlibPresentationSupportKHR(uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) const
{
    return this->procTable.GetPhysicalDeviceXlibPresentationSupportKHR(this->physDevice, queueFamilyIndex, dpy, visualID);
}

#endif
#if defined(VK_KHR_xcb_surface)
VkResult VexDevice::CreateXcbSurfaceKHR(const VkXcbSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateXcbSurfaceKHR(this->instance, pCreateInfo, this->allocator, pSurface);
}

VkBool32 VexDevice::GetPhysicalDeviceXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t* connection, xcb_visualid_t visual_id) const
{
    return this->procTable.GetPhysicalDeviceXcbPresentationSupportKHR(this->physDevice, queueFamilyIndex, connection, visual_id);
}

#endif
#if defined(VK_KHR_wayland_surface)
VkResult VexDevice::CreateWaylandSurfaceKHR(const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateWaylandSurfaceKHR(this->instance, pCreateInfo, this->allocator, pSurface);
}

VkBool32 VexDevice::GetPhysicalDeviceWaylandPresentationSupportKHR(uint32_t queueFamilyIndex, struct wl_display* display) const
{
    return this->procTable.GetPhysicalDeviceWaylandPresentationSupportKHR(this->physDevice, queueFamilyIndex, display);
}

#endif
#if defined(VK_KHR_android_surface)
VkResult VexDevice::CreateAndroidSurfaceKHR(const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateAndroidSurfaceKHR(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_KHR_win32_surface)
VkResult VexDevice::CreateWin32SurfaceKHR(const VkWin32SurfaceCreateInfoKHR* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateWin32SurfaceKHR(this->instance, pCreateInfo, this->allocator, pSurface);
}

VkBool32 VexDevice::GetPhysicalDeviceWin32PresentationSupportKHR(uint32_t queueFamilyIndex) const
{
    return this->procTable.GetPhysicalDeviceWin32PresentationSupportKHR(this->physDevice, queueFamilyIndex);
}

#endif
#if defined(VK_EXT_debug_report)
VkResult VexDevice::CreateDebugReportCallbackEXT(const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, VkDebugReportCallbackEXT* pCallback) const
{
    return this->procTable.CreateDebugReportCallbackEXT(this->instance, pCreateInfo, this->allocator, pCallback);
}

void VexDevice::DestroyDebugReportCallbackEXT(VkDebugReportCallbackEXT callback) const
{
    this->procTable.DestroyDebugReportCallbackEXT(this->instance, callback, this->allocator);
}

void VexDevice::DebugReportMessageEXT(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage) const
{
    this->procTable.DebugReportMessageEXT(this->instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

#endif
#if defined(VK_EXT_debug_marker)
VkResult VexDevice::DebugMarkerSetObjectTagEXT(const VkDebugMarkerObjectTagInfoEXT* pTagInfo) const
{
    return this->procTable.DebugMarkerSetObjectTagEXT(this->device, pTagInfo);
}

VkResult VexDevice::DebugMarkerSetObjectNameEXT(const VkDebugMarkerObjectNameInfoEXT* pNameInfo) const
{
    return this->procTable.DebugMarkerSetObjectNameEXT(this->device, pNameInfo);
}

void VexDevice::CmdDebugMarkerBeginEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const
{
    this->procTable.CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}

void VexDevice::CmdDebugMarkerEndEXT(VkCommandBuffer commandBuffer) const
{
    this->procTable.CmdDebugMarkerEndEXT(commandBuffer);
}

void VexDevice::CmdDebugMarkerInsertEXT(VkCommandBuffer commandBuffer, const VkDebugMarkerMarkerInfoEXT* pMarkerInfo) const
{
    this->procTable.CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}

#endif
#if defined(VK_EXT_transform_feedback)
void VexDevice::CmdBindTransformFeedbackBuffersEXT(VkCommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets, const VkDeviceSize* pSizes) const
{
    this->procTable.CmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void VexDevice::CmdBeginTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const
{
    this->procTable.CmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void VexDevice::CmdEndTransformFeedbackEXT(VkCommandBuffer commandBuffer, uint32_t firstCounterBuffer, uint32_t counterBufferCount, const VkBuffer* pCounterBuffers, const VkDeviceSize* pCounterBufferOffsets) const
{
    this->procTable.CmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void VexDevice::CmdBeginQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, VkQueryControlFlags flags, uint32_t index) const
{
    this->procTable.CmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
}

void VexDevice::CmdEndQueryIndexedEXT(VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query, uint32_t index) const
{
    this->procTable.CmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
}

void VexDevice::CmdDrawIndirectByteCountEXT(VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance, VkBuffer counterBuffer, VkDeviceSize counterBufferOffset, uint32_t counterOffset, uint32_t vertexStride) const
{
    this->procTable.CmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

#endif
#if defined(VK_NVX_image_view_handle)
uint32_t VexDevice::GetImageViewHandleNVX(const VkImageViewHandleInfoNVX* pInfo) const
{
    return this->procTable.GetImageViewHandleNVX(this->device, pInfo);
}

#endif
#if defined(VK_AMD_draw_indirect_count)
void VexDevice::CmdDrawIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const
{
    this->procTable.CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VexDevice::CmdDrawIndexedIndirectCountAMD(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const
{
    this->procTable.CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

#endif
#if defined(VK_AMD_shader_info)
VkResult VexDevice::GetShaderInfoAMD(VkPipeline pipeline, VkShaderStageFlagBits shaderStage, VkShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo) const
{
    return this->procTable.GetShaderInfoAMD(this->device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

#endif
#if defined(VK_GGP_stream_descriptor_surface)
VkResult VexDevice::CreateStreamDescriptorSurfaceGGP(const VkStreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateStreamDescriptorSurfaceGGP(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_NV_external_memory_capabilities)
VkResult VexDevice::GetPhysicalDeviceExternalImageFormatPropertiesNV(VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage, VkImageCreateFlags flags, VkExternalMemoryHandleTypeFlagsNV externalHandleType, VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) const
{
    return this->procTable.GetPhysicalDeviceExternalImageFormatPropertiesNV(this->physDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}

#endif
#if defined(VK_NV_external_memory_win32)
VkResult VexDevice::GetMemoryWin32HandleNV(VkDeviceMemory memory, VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) const
{
    return this->procTable.GetMemoryWin32HandleNV(this->device, memory, handleType, pHandle);
}

#endif
#if defined(VK_KHR_get_physical_device_properties2)
void VexDevice::GetPhysicalDeviceFeatures2KHR(VkPhysicalDeviceFeatures2* pFeatures) const
{
    this->procTable.GetPhysicalDeviceFeatures2KHR(this->physDevice, pFeatures);
}

void VexDevice::GetPhysicalDeviceProperties2KHR(VkPhysicalDeviceProperties2* pProperties) const
{
    this->procTable.GetPhysicalDeviceProperties2KHR(this->physDevice, pProperties);
}

void VexDevice::GetPhysicalDeviceFormatProperties2KHR(VkFormat format, VkFormatProperties2* pFormatProperties) const
{
    this->procTable.GetPhysicalDeviceFormatProperties2KHR(this->physDevice, format, pFormatProperties);
}

VkResult VexDevice::GetPhysicalDeviceImageFormatProperties2KHR(const VkPhysicalDeviceImageFormatInfo2* pImageFormatInfo, VkImageFormatProperties2* pImageFormatProperties) const
{
    return this->procTable.GetPhysicalDeviceImageFormatProperties2KHR(this->physDevice, pImageFormatInfo, pImageFormatProperties);
}

void VexDevice::GetPhysicalDeviceQueueFamilyProperties2KHR(uint32_t* pQueueFamilyPropertyCount, VkQueueFamilyProperties2* pQueueFamilyProperties) const
{
    this->procTable.GetPhysicalDeviceQueueFamilyProperties2KHR(this->physDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void VexDevice::GetPhysicalDeviceMemoryProperties2KHR(VkPhysicalDeviceMemoryProperties2* pMemoryProperties) const
{
    this->procTable.GetPhysicalDeviceMemoryProperties2KHR(this->physDevice, pMemoryProperties);
}

void VexDevice::GetPhysicalDeviceSparseImageFormatProperties2KHR(const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo, uint32_t* pPropertyCount, VkSparseImageFormatProperties2* pProperties) const
{
    this->procTable.GetPhysicalDeviceSparseImageFormatProperties2KHR(this->physDevice, pFormatInfo, pPropertyCount, pProperties);
}

#endif
#if defined(VK_KHR_device_group)
void VexDevice::GetDeviceGroupPeerMemoryFeaturesKHR(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, VkPeerMemoryFeatureFlags* pPeerMemoryFeatures) const
{
    this->procTable.GetDeviceGroupPeerMemoryFeaturesKHR(this->device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void VexDevice::CmdSetDeviceMaskKHR(VkCommandBuffer commandBuffer, uint32_t deviceMask) const
{
    this->procTable.CmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}

void VexDevice::CmdDispatchBaseKHR(VkCommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const
{
    this->procTable.CmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

#endif
#if defined(VK_NN_vi_surface)
VkResult VexDevice::CreateViSurfaceNN(const VkViSurfaceCreateInfoNN* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateViSurfaceNN(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_KHR_maintenance1)
void VexDevice::TrimCommandPoolKHR(VkCommandPool commandPool, VkCommandPoolTrimFlags flags) const
{
    this->procTable.TrimCommandPoolKHR(this->device, commandPool, flags);
}

#endif
#if defined(VK_KHR_device_group_creation)
VkResult VexDevice::EnumeratePhysicalDeviceGroupsKHR(uint32_t* pPhysicalDeviceGroupCount, VkPhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties) const
{
    return this->procTable.EnumeratePhysicalDeviceGroupsKHR(this->instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

#endif
#if defined(VK_KHR_external_memory_capabilities)
void VexDevice::GetPhysicalDeviceExternalBufferPropertiesKHR(const VkPhysicalDeviceExternalBufferInfo* pExternalBufferInfo, VkExternalBufferProperties* pExternalBufferProperties) const
{
    this->procTable.GetPhysicalDeviceExternalBufferPropertiesKHR(this->physDevice, pExternalBufferInfo, pExternalBufferProperties);
}

#endif
#if defined(VK_KHR_external_memory_win32)
VkResult VexDevice::GetMemoryWin32HandleKHR(const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const
{
    return this->procTable.GetMemoryWin32HandleKHR(this->device, pGetWin32HandleInfo, pHandle);
}

VkResult VexDevice::GetMemoryWin32HandlePropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, VkMemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties) const
{
    return this->procTable.GetMemoryWin32HandlePropertiesKHR(this->device, handleType, handle, pMemoryWin32HandleProperties);
}

#endif
#if defined(VK_KHR_external_memory_fd)
VkResult VexDevice::GetMemoryFdKHR(const VkMemoryGetFdInfoKHR* pGetFdInfo, int* pFd) const
{
    return this->procTable.GetMemoryFdKHR(this->device, pGetFdInfo, pFd);
}

VkResult VexDevice::GetMemoryFdPropertiesKHR(VkExternalMemoryHandleTypeFlagBits handleType, int fd, VkMemoryFdPropertiesKHR* pMemoryFdProperties) const
{
    return this->procTable.GetMemoryFdPropertiesKHR(this->device, handleType, fd, pMemoryFdProperties);
}

#endif
#if defined(VK_KHR_external_semaphore_capabilities)
void VexDevice::GetPhysicalDeviceExternalSemaphorePropertiesKHR(const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo, VkExternalSemaphoreProperties* pExternalSemaphoreProperties) const
{
    this->procTable.GetPhysicalDeviceExternalSemaphorePropertiesKHR(this->physDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

#endif
#if defined(VK_KHR_external_semaphore_win32)
VkResult VexDevice::ImportSemaphoreWin32HandleKHR(const VkImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo) const
{
    return this->procTable.ImportSemaphoreWin32HandleKHR(this->device, pImportSemaphoreWin32HandleInfo);
}

VkResult VexDevice::GetSemaphoreWin32HandleKHR(const VkSemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const
{
    return this->procTable.GetSemaphoreWin32HandleKHR(this->device, pGetWin32HandleInfo, pHandle);
}

#endif
#if defined(VK_KHR_external_semaphore_fd)
VkResult VexDevice::ImportSemaphoreFdKHR(const VkImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo) const
{
    return this->procTable.ImportSemaphoreFdKHR(this->device, pImportSemaphoreFdInfo);
}

VkResult VexDevice::GetSemaphoreFdKHR(const VkSemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd) const
{
    return this->procTable.GetSemaphoreFdKHR(this->device, pGetFdInfo, pFd);
}

#endif
#if defined(VK_KHR_push_descriptor)
void VexDevice::CmdPushDescriptorSetKHR(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) const
{
    this->procTable.CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

#endif
#if defined(VK_EXT_conditional_rendering)
void VexDevice::CmdBeginConditionalRenderingEXT(VkCommandBuffer commandBuffer, const VkConditionalRenderingBeginInfoEXT* pConditionalRenderingBegin) const
{
    this->procTable.CmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
}

void VexDevice::CmdEndConditionalRenderingEXT(VkCommandBuffer commandBuffer) const
{
    this->procTable.CmdEndConditionalRenderingEXT(commandBuffer);
}

#endif
#if defined(VK_KHR_descriptor_update_template)
VkResult VexDevice::CreateDescriptorUpdateTemplateKHR(const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo, VkDescriptorUpdateTemplate* pDescriptorUpdateTemplate) const
{
    return this->procTable.CreateDescriptorUpdateTemplateKHR(this->device, pCreateInfo, this->allocator, pDescriptorUpdateTemplate);
}

void VexDevice::DestroyDescriptorUpdateTemplateKHR(VkDescriptorUpdateTemplate descriptorUpdateTemplate) const
{
    this->procTable.DestroyDescriptorUpdateTemplateKHR(this->device, descriptorUpdateTemplate, this->allocator);
}

void VexDevice::UpdateDescriptorSetWithTemplateKHR(VkDescriptorSet descriptorSet, VkDescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData) const
{
    this->procTable.UpdateDescriptorSetWithTemplateKHR(this->device, descriptorSet, descriptorUpdateTemplate, pData);
}

#endif
#if defined(VK_NVX_device_generated_commands)
void VexDevice::CmdProcessCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo) const
{
    this->procTable.CmdProcessCommandsNVX(commandBuffer, pProcessCommandsInfo);
}

void VexDevice::CmdReserveSpaceForCommandsNVX(VkCommandBuffer commandBuffer, const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo) const
{
    this->procTable.CmdReserveSpaceForCommandsNVX(commandBuffer, pReserveSpaceInfo);
}

VkResult VexDevice::CreateIndirectCommandsLayoutNVX(const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo, VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout) const
{
    return this->procTable.CreateIndirectCommandsLayoutNVX(this->device, pCreateInfo, this->allocator, pIndirectCommandsLayout);
}

void VexDevice::DestroyIndirectCommandsLayoutNVX(VkIndirectCommandsLayoutNVX indirectCommandsLayout) const
{
    this->procTable.DestroyIndirectCommandsLayoutNVX(this->device, indirectCommandsLayout, this->allocator);
}

VkResult VexDevice::CreateObjectTableNVX(const VkObjectTableCreateInfoNVX* pCreateInfo, VkObjectTableNVX* pObjectTable) const
{
    return this->procTable.CreateObjectTableNVX(this->device, pCreateInfo, this->allocator, pObjectTable);
}

void VexDevice::DestroyObjectTableNVX(VkObjectTableNVX objectTable) const
{
    this->procTable.DestroyObjectTableNVX(this->device, objectTable, this->allocator);
}

VkResult VexDevice::RegisterObjectsNVX(VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices) const
{
    return this->procTable.RegisterObjectsNVX(this->device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
}

VkResult VexDevice::UnregisterObjectsNVX(VkObjectTableNVX objectTable, uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices) const
{
    return this->procTable.UnregisterObjectsNVX(this->device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
}

void VexDevice::GetPhysicalDeviceGeneratedCommandsPropertiesNVX(VkDeviceGeneratedCommandsFeaturesNVX* pFeatures, VkDeviceGeneratedCommandsLimitsNVX* pLimits) const
{
    this->procTable.GetPhysicalDeviceGeneratedCommandsPropertiesNVX(this->physDevice, pFeatures, pLimits);
}

#endif
#if defined(VK_NV_clip_space_w_scaling)
void VexDevice::CmdSetViewportWScalingNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) const
{
    this->procTable.CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}

#endif
#if defined(VK_EXT_direct_mode_display)
VkResult VexDevice::ReleaseDisplayEXT(VkDisplayKHR display) const
{
    return this->procTable.ReleaseDisplayEXT(this->physDevice, display);
}

#endif
#if defined(VK_EXT_acquire_xlib_display)
VkResult VexDevice::AcquireXlibDisplayEXT(Display* dpy, VkDisplayKHR display) const
{
    return this->procTable.AcquireXlibDisplayEXT(this->physDevice, dpy, display);
}

VkResult VexDevice::GetRandROutputDisplayEXT(Display* dpy, RROutput rrOutput, VkDisplayKHR* pDisplay) const
{
    return this->procTable.GetRandROutputDisplayEXT(this->physDevice, dpy, rrOutput, pDisplay);
}

#endif
#if defined(VK_EXT_display_surface_counter)
VkResult VexDevice::GetPhysicalDeviceSurfaceCapabilities2EXT(VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) const
{
    return this->procTable.GetPhysicalDeviceSurfaceCapabilities2EXT(this->physDevice, surface, pSurfaceCapabilities);
}

#endif
#if defined(VK_EXT_display_control)
VkResult VexDevice::DisplayPowerControlEXT(VkDisplayKHR display, const VkDisplayPowerInfoEXT* pDisplayPowerInfo) const
{
    return this->procTable.DisplayPowerControlEXT(this->device, display, pDisplayPowerInfo);
}

VkResult VexDevice::RegisterDeviceEventEXT(const VkDeviceEventInfoEXT* pDeviceEventInfo, VkFence* pFence) const
{
    return this->procTable.RegisterDeviceEventEXT(this->device, pDeviceEventInfo, this->allocator, pFence);
}

VkResult VexDevice::RegisterDisplayEventEXT(VkDisplayKHR display, const VkDisplayEventInfoEXT* pDisplayEventInfo, VkFence* pFence) const
{
    return this->procTable.RegisterDisplayEventEXT(this->device, display, pDisplayEventInfo, this->allocator, pFence);
}

VkResult VexDevice::GetSwapchainCounterEXT(VkSwapchainKHR swapchain, VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) const
{
    return this->procTable.GetSwapchainCounterEXT(this->device, swapchain, counter, pCounterValue);
}

#endif
#if defined(VK_GOOGLE_display_timing)
VkResult VexDevice::GetRefreshCycleDurationGOOGLE(VkSwapchainKHR swapchain, VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) const
{
    return this->procTable.GetRefreshCycleDurationGOOGLE(this->device, swapchain, pDisplayTimingProperties);
}

VkResult VexDevice::GetPastPresentationTimingGOOGLE(VkSwapchainKHR swapchain, uint32_t* pPresentationTimingCount, VkPastPresentationTimingGOOGLE* pPresentationTimings) const
{
    return this->procTable.GetPastPresentationTimingGOOGLE(this->device, swapchain, pPresentationTimingCount, pPresentationTimings);
}

#endif
#if defined(VK_EXT_discard_rectangles)
void VexDevice::CmdSetDiscardRectangleEXT(VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle, uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) const
{
    this->procTable.CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

#endif
#if defined(VK_EXT_hdr_metadata)
void VexDevice::SetHdrMetadataEXT(uint32_t swapchainCount, const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) const
{
    this->procTable.SetHdrMetadataEXT(this->device, swapchainCount, pSwapchains, pMetadata);
}

#endif
#if defined(VK_KHR_create_renderpass2)
VkResult VexDevice::CreateRenderPass2KHR(const VkRenderPassCreateInfo2KHR* pCreateInfo, VkRenderPass* pRenderPass) const
{
    return this->procTable.CreateRenderPass2KHR(this->device, pCreateInfo, this->allocator, pRenderPass);
}

void VexDevice::CmdBeginRenderPass2KHR(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfoKHR* pSubpassBeginInfo) const
{
    this->procTable.CmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void VexDevice::CmdNextSubpass2KHR(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR* pSubpassBeginInfo, const VkSubpassEndInfoKHR* pSubpassEndInfo) const
{
    this->procTable.CmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void VexDevice::CmdEndRenderPass2KHR(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR* pSubpassEndInfo) const
{
    this->procTable.CmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}

#endif
#if defined(VK_KHR_shared_presentable_image)
VkResult VexDevice::GetSwapchainStatusKHR(VkSwapchainKHR swapchain) const
{
    return this->procTable.GetSwapchainStatusKHR(this->device, swapchain);
}

#endif
#if defined(VK_KHR_external_fence_capabilities)
void VexDevice::GetPhysicalDeviceExternalFencePropertiesKHR(const VkPhysicalDeviceExternalFenceInfo* pExternalFenceInfo, VkExternalFenceProperties* pExternalFenceProperties) const
{
    this->procTable.GetPhysicalDeviceExternalFencePropertiesKHR(this->physDevice, pExternalFenceInfo, pExternalFenceProperties);
}

#endif
#if defined(VK_KHR_external_fence_win32)
VkResult VexDevice::ImportFenceWin32HandleKHR(const VkImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo) const
{
    return this->procTable.ImportFenceWin32HandleKHR(this->device, pImportFenceWin32HandleInfo);
}

VkResult VexDevice::GetFenceWin32HandleKHR(const VkFenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle) const
{
    return this->procTable.GetFenceWin32HandleKHR(this->device, pGetWin32HandleInfo, pHandle);
}

#endif
#if defined(VK_KHR_external_fence_fd)
VkResult VexDevice::ImportFenceFdKHR(const VkImportFenceFdInfoKHR* pImportFenceFdInfo) const
{
    return this->procTable.ImportFenceFdKHR(this->device, pImportFenceFdInfo);
}

VkResult VexDevice::GetFenceFdKHR(const VkFenceGetFdInfoKHR* pGetFdInfo, int* pFd) const
{
    return this->procTable.GetFenceFdKHR(this->device, pGetFdInfo, pFd);
}

#endif
#if defined(VK_KHR_get_surface_capabilities2)
VkResult VexDevice::GetPhysicalDeviceSurfaceCapabilities2KHR(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkSurfaceCapabilities2KHR* pSurfaceCapabilities) const
{
    return this->procTable.GetPhysicalDeviceSurfaceCapabilities2KHR(this->physDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VkResult VexDevice::GetPhysicalDeviceSurfaceFormats2KHR(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount, VkSurfaceFormat2KHR* pSurfaceFormats) const
{
    return this->procTable.GetPhysicalDeviceSurfaceFormats2KHR(this->physDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}

#endif
#if defined(VK_KHR_get_display_properties2)
VkResult VexDevice::GetPhysicalDeviceDisplayProperties2KHR(uint32_t* pPropertyCount, VkDisplayProperties2KHR* pProperties) const
{
    return this->procTable.GetPhysicalDeviceDisplayProperties2KHR(this->physDevice, pPropertyCount, pProperties);
}

VkResult VexDevice::GetPhysicalDeviceDisplayPlaneProperties2KHR(uint32_t* pPropertyCount, VkDisplayPlaneProperties2KHR* pProperties) const
{
    return this->procTable.GetPhysicalDeviceDisplayPlaneProperties2KHR(this->physDevice, pPropertyCount, pProperties);
}

VkResult VexDevice::GetDisplayModeProperties2KHR(VkDisplayKHR display, uint32_t* pPropertyCount, VkDisplayModeProperties2KHR* pProperties) const
{
    return this->procTable.GetDisplayModeProperties2KHR(this->physDevice, display, pPropertyCount, pProperties);
}

VkResult VexDevice::GetDisplayPlaneCapabilities2KHR(const VkDisplayPlaneInfo2KHR* pDisplayPlaneInfo, VkDisplayPlaneCapabilities2KHR* pCapabilities) const
{
    return this->procTable.GetDisplayPlaneCapabilities2KHR(this->physDevice, pDisplayPlaneInfo, pCapabilities);
}

#endif
#if defined(VK_MVK_ios_surface)
VkResult VexDevice::CreateIOSSurfaceMVK(const VkIOSSurfaceCreateInfoMVK* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateIOSSurfaceMVK(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_MVK_macos_surface)
VkResult VexDevice::CreateMacOSSurfaceMVK(const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateMacOSSurfaceMVK(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_EXT_debug_utils)
VkResult VexDevice::SetDebugUtilsObjectNameEXT(const VkDebugUtilsObjectNameInfoEXT* pNameInfo) const
{
    return this->procTable.SetDebugUtilsObjectNameEXT(this->device, pNameInfo);
}

VkResult VexDevice::SetDebugUtilsObjectTagEXT(const VkDebugUtilsObjectTagInfoEXT* pTagInfo) const
{
    return this->procTable.SetDebugUtilsObjectTagEXT(this->device, pTagInfo);
}

void VexDevice::QueueBeginDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) const
{
    this->procTable.QueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}

void VexDevice::QueueEndDebugUtilsLabelEXT(VkQueue queue) const
{
    this->procTable.QueueEndDebugUtilsLabelEXT(queue);
}

void VexDevice::QueueInsertDebugUtilsLabelEXT(VkQueue queue, const VkDebugUtilsLabelEXT* pLabelInfo) const
{
    this->procTable.QueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}

void VexDevice::CmdBeginDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) const
{
    this->procTable.CmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void VexDevice::CmdEndDebugUtilsLabelEXT(VkCommandBuffer commandBuffer) const
{
    this->procTable.CmdEndDebugUtilsLabelEXT(commandBuffer);
}

void VexDevice::CmdInsertDebugUtilsLabelEXT(VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) const
{
    this->procTable.CmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

VkResult VexDevice::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, VkDebugUtilsMessengerEXT* pMessenger) const
{
    return this->procTable.CreateDebugUtilsMessengerEXT(this->instance, pCreateInfo, this->allocator, pMessenger);
}

void VexDevice::DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT messenger) const
{
    this->procTable.DestroyDebugUtilsMessengerEXT(this->instance, messenger, this->allocator);
}

void VexDevice::SubmitDebugUtilsMessageEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) const
{
    this->procTable.SubmitDebugUtilsMessageEXT(this->instance, messageSeverity, messageTypes, pCallbackData);
}

#endif
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
VkResult VexDevice::GetAndroidHardwareBufferPropertiesANDROID(const struct AHardwareBuffer* buffer, VkAndroidHardwareBufferPropertiesANDROID* pProperties) const
{
    return this->procTable.GetAndroidHardwareBufferPropertiesANDROID(this->device, buffer, pProperties);
}

VkResult VexDevice::GetMemoryAndroidHardwareBufferANDROID(const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct AHardwareBuffer** pBuffer) const
{
    return this->procTable.GetMemoryAndroidHardwareBufferANDROID(this->device, pInfo, pBuffer);
}

#endif
#if defined(VK_EXT_sample_locations)
void VexDevice::CmdSetSampleLocationsEXT(VkCommandBuffer commandBuffer, const VkSampleLocationsInfoEXT* pSampleLocationsInfo) const
{
    this->procTable.CmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}

void VexDevice::GetPhysicalDeviceMultisamplePropertiesEXT(VkSampleCountFlagBits samples, VkMultisamplePropertiesEXT* pMultisampleProperties) const
{
    this->procTable.GetPhysicalDeviceMultisamplePropertiesEXT(this->physDevice, samples, pMultisampleProperties);
}

#endif
#if defined(VK_KHR_get_memory_requirements2)
void VexDevice::GetImageMemoryRequirements2KHR(const VkImageMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const
{
    this->procTable.GetImageMemoryRequirements2KHR(this->device, pInfo, pMemoryRequirements);
}

void VexDevice::GetBufferMemoryRequirements2KHR(const VkBufferMemoryRequirementsInfo2* pInfo, VkMemoryRequirements2* pMemoryRequirements) const
{
    this->procTable.GetBufferMemoryRequirements2KHR(this->device, pInfo, pMemoryRequirements);
}

void VexDevice::GetImageSparseMemoryRequirements2KHR(const VkImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, VkSparseImageMemoryRequirements2* pSparseMemoryRequirements) const
{
    this->procTable.GetImageSparseMemoryRequirements2KHR(this->device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

#endif
#if defined(VK_KHR_sampler_ycbcr_conversion)
VkResult VexDevice::CreateSamplerYcbcrConversionKHR(const VkSamplerYcbcrConversionCreateInfo* pCreateInfo, VkSamplerYcbcrConversion* pYcbcrConversion) const
{
    return this->procTable.CreateSamplerYcbcrConversionKHR(this->device, pCreateInfo, this->allocator, pYcbcrConversion);
}

void VexDevice::DestroySamplerYcbcrConversionKHR(VkSamplerYcbcrConversion ycbcrConversion) const
{
    this->procTable.DestroySamplerYcbcrConversionKHR(this->device, ycbcrConversion, this->allocator);
}

#endif
#if defined(VK_KHR_bind_memory2)
VkResult VexDevice::BindBufferMemory2KHR(uint32_t bindInfoCount, const VkBindBufferMemoryInfo* pBindInfos) const
{
    return this->procTable.BindBufferMemory2KHR(this->device, bindInfoCount, pBindInfos);
}

VkResult VexDevice::BindImageMemory2KHR(uint32_t bindInfoCount, const VkBindImageMemoryInfo* pBindInfos) const
{
    return this->procTable.BindImageMemory2KHR(this->device, bindInfoCount, pBindInfos);
}

#endif
#if defined(VK_EXT_image_drm_format_modifier)
VkResult VexDevice::GetImageDrmFormatModifierPropertiesEXT(VkImage image, VkImageDrmFormatModifierPropertiesEXT* pProperties) const
{
    return this->procTable.GetImageDrmFormatModifierPropertiesEXT(this->device, image, pProperties);
}

#endif
#if defined(VK_EXT_validation_cache)
VkResult VexDevice::CreateValidationCacheEXT(const VkValidationCacheCreateInfoEXT* pCreateInfo, VkValidationCacheEXT* pValidationCache) const
{
    return this->procTable.CreateValidationCacheEXT(this->device, pCreateInfo, this->allocator, pValidationCache);
}

void VexDevice::DestroyValidationCacheEXT(VkValidationCacheEXT validationCache) const
{
    this->procTable.DestroyValidationCacheEXT(this->device, validationCache, this->allocator);
}

VkResult VexDevice::MergeValidationCachesEXT(VkValidationCacheEXT dstCache, uint32_t srcCacheCount, const VkValidationCacheEXT* pSrcCaches) const
{
    return this->procTable.MergeValidationCachesEXT(this->device, dstCache, srcCacheCount, pSrcCaches);
}

VkResult VexDevice::GetValidationCacheDataEXT(VkValidationCacheEXT validationCache, size_t* pDataSize, void* pData) const
{
    return this->procTable.GetValidationCacheDataEXT(this->device, validationCache, pDataSize, pData);
}

#endif
#if defined(VK_NV_shading_rate_image)
void VexDevice::CmdBindShadingRateImageNV(VkCommandBuffer commandBuffer, VkImageView imageView, VkImageLayout imageLayout) const
{
    this->procTable.CmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
}

void VexDevice::CmdSetViewportShadingRatePaletteNV(VkCommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const VkShadingRatePaletteNV* pShadingRatePalettes) const
{
    this->procTable.CmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void VexDevice::CmdSetCoarseSampleOrderNV(VkCommandBuffer commandBuffer, VkCoarseSampleOrderTypeNV sampleOrderType, uint32_t customSampleOrderCount, const VkCoarseSampleOrderCustomNV* pCustomSampleOrders) const
{
    this->procTable.CmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

#endif
#if defined(VK_NV_ray_tracing)
VkResult VexDevice::CreateAccelerationStructureNV(const VkAccelerationStructureCreateInfoNV* pCreateInfo, VkAccelerationStructureNV* pAccelerationStructure) const
{
    return this->procTable.CreateAccelerationStructureNV(this->device, pCreateInfo, this->allocator, pAccelerationStructure);
}

void VexDevice::DestroyAccelerationStructureNV(VkAccelerationStructureNV accelerationStructure) const
{
    this->procTable.DestroyAccelerationStructureNV(this->device, accelerationStructure, this->allocator);
}

void VexDevice::GetAccelerationStructureMemoryRequirementsNV(const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo, VkMemoryRequirements2KHR* pMemoryRequirements) const
{
    this->procTable.GetAccelerationStructureMemoryRequirementsNV(this->device, pInfo, pMemoryRequirements);
}

VkResult VexDevice::BindAccelerationStructureMemoryNV(uint32_t bindInfoCount, const VkBindAccelerationStructureMemoryInfoNV* pBindInfos) const
{
    return this->procTable.BindAccelerationStructureMemoryNV(this->device, bindInfoCount, pBindInfos);
}

void VexDevice::CmdBuildAccelerationStructureNV(VkCommandBuffer commandBuffer, const VkAccelerationStructureInfoNV* pInfo, VkBuffer instanceData, VkDeviceSize instanceOffset, VkBool32 update, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkBuffer scratch, VkDeviceSize scratchOffset) const
{
    this->procTable.CmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

void VexDevice::CmdCopyAccelerationStructureNV(VkCommandBuffer commandBuffer, VkAccelerationStructureNV dst, VkAccelerationStructureNV src, VkCopyAccelerationStructureModeNV mode) const
{
    this->procTable.CmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
}

void VexDevice::CmdTraceRaysNV(VkCommandBuffer commandBuffer, VkBuffer raygenShaderBindingTableBuffer, VkDeviceSize raygenShaderBindingOffset, VkBuffer missShaderBindingTableBuffer, VkDeviceSize missShaderBindingOffset, VkDeviceSize missShaderBindingStride, VkBuffer hitShaderBindingTableBuffer, VkDeviceSize hitShaderBindingOffset, VkDeviceSize hitShaderBindingStride, VkBuffer callableShaderBindingTableBuffer, VkDeviceSize callableShaderBindingOffset, VkDeviceSize callableShaderBindingStride, uint32_t width, uint32_t height, uint32_t depth) const
{
    this->procTable.CmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

VkResult VexDevice::CreateRayTracingPipelinesNV(VkPipelineCache pipelineCache, uint32_t createInfoCount, const VkRayTracingPipelineCreateInfoNV* pCreateInfos, VkPipeline* pPipelines) const
{
    return this->procTable.CreateRayTracingPipelinesNV(this->device, pipelineCache, createInfoCount, pCreateInfos, this->allocator, pPipelines);
}

VkResult VexDevice::GetRayTracingShaderGroupHandlesNV(VkPipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData) const
{
    return this->procTable.GetRayTracingShaderGroupHandlesNV(this->device, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult VexDevice::GetAccelerationStructureHandleNV(VkAccelerationStructureNV accelerationStructure, size_t dataSize, void* pData) const
{
    return this->procTable.GetAccelerationStructureHandleNV(this->device, accelerationStructure, dataSize, pData);
}

void VexDevice::CmdWriteAccelerationStructuresPropertiesNV(VkCommandBuffer commandBuffer, uint32_t accelerationStructureCount, const VkAccelerationStructureNV* pAccelerationStructures, VkQueryType queryType, VkQueryPool queryPool, uint32_t firstQuery) const
{
    this->procTable.CmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

VkResult VexDevice::CompileDeferredNV(VkPipeline pipeline, uint32_t shader) const
{
    return this->procTable.CompileDeferredNV(this->device, pipeline, shader);
}

#endif
#if defined(VK_KHR_maintenance3)
void VexDevice::GetDescriptorSetLayoutSupportKHR(const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport) const
{
    this->procTable.GetDescriptorSetLayoutSupportKHR(this->device, pCreateInfo, pSupport);
}

#endif
#if defined(VK_KHR_draw_indirect_count)
void VexDevice::CmdDrawIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const
{
    this->procTable.CmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void VexDevice::CmdDrawIndexedIndirectCountKHR(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const
{
    this->procTable.CmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

#endif
#if defined(VK_EXT_external_memory_host)
VkResult VexDevice::GetMemoryHostPointerPropertiesEXT(VkExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, VkMemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties) const
{
    return this->procTable.GetMemoryHostPointerPropertiesEXT(this->device, handleType, pHostPointer, pMemoryHostPointerProperties);
}

#endif
#if defined(VK_AMD_buffer_marker)
void VexDevice::CmdWriteBufferMarkerAMD(VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage, VkBuffer dstBuffer, VkDeviceSize dstOffset, uint32_t marker) const
{
    this->procTable.CmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

#endif
#if defined(VK_EXT_calibrated_timestamps)
VkResult VexDevice::GetPhysicalDeviceCalibrateableTimeDomainsEXT(uint32_t* pTimeDomainCount, VkTimeDomainEXT* pTimeDomains) const
{
    return this->procTable.GetPhysicalDeviceCalibrateableTimeDomainsEXT(this->physDevice, pTimeDomainCount, pTimeDomains);
}

VkResult VexDevice::GetCalibratedTimestampsEXT(uint32_t timestampCount, const VkCalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation) const
{
    return this->procTable.GetCalibratedTimestampsEXT(this->device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

#endif
#if defined(VK_NV_mesh_shader)
void VexDevice::CmdDrawMeshTasksNV(VkCommandBuffer commandBuffer, uint32_t taskCount, uint32_t firstTask) const
{
    this->procTable.CmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
}

void VexDevice::CmdDrawMeshTasksIndirectNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride) const
{
    this->procTable.CmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
}

void VexDevice::CmdDrawMeshTasksIndirectCountNV(VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const
{
    this->procTable.CmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

#endif
#if defined(VK_NV_scissor_exclusive)
void VexDevice::CmdSetExclusiveScissorNV(VkCommandBuffer commandBuffer, uint32_t firstExclusiveScissor, uint32_t exclusiveScissorCount, const VkRect2D* pExclusiveScissors) const
{
    this->procTable.CmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

#endif
#if defined(VK_NV_device_diagnostic_checkpoints)
void VexDevice::CmdSetCheckpointNV(VkCommandBuffer commandBuffer, const void* pCheckpointMarker) const
{
    this->procTable.CmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}

void VexDevice::GetQueueCheckpointDataNV(VkQueue queue, uint32_t* pCheckpointDataCount, VkCheckpointDataNV* pCheckpointData) const
{
    this->procTable.GetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}

#endif
#if defined(VK_KHR_timeline_semaphore)
VkResult VexDevice::GetSemaphoreCounterValueKHR(VkSemaphore semaphore, uint64_t* pValue) const
{
    return this->procTable.GetSemaphoreCounterValueKHR(this->device, semaphore, pValue);
}

VkResult VexDevice::WaitSemaphoresKHR(const VkSemaphoreWaitInfoKHR* pWaitInfo, uint64_t timeout) const
{
    return this->procTable.WaitSemaphoresKHR(this->device, pWaitInfo, timeout);
}

VkResult VexDevice::SignalSemaphoreKHR(const VkSemaphoreSignalInfoKHR* pSignalInfo) const
{
    return this->procTable.SignalSemaphoreKHR(this->device, pSignalInfo);
}

#endif
#if defined(VK_INTEL_performance_query)
VkResult VexDevice::InitializePerformanceApiINTEL(const VkInitializePerformanceApiInfoINTEL* pInitializeInfo) const
{
    return this->procTable.InitializePerformanceApiINTEL(this->device, pInitializeInfo);
}

void VexDevice::UninitializePerformanceApiINTEL() const
{
    this->procTable.UninitializePerformanceApiINTEL(this->device);
}

VkResult VexDevice::CmdSetPerformanceMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceMarkerInfoINTEL* pMarkerInfo) const
{
    return this->procTable.CmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
}

VkResult VexDevice::CmdSetPerformanceStreamMarkerINTEL(VkCommandBuffer commandBuffer, const VkPerformanceStreamMarkerInfoINTEL* pMarkerInfo) const
{
    return this->procTable.CmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
}

VkResult VexDevice::CmdSetPerformanceOverrideINTEL(VkCommandBuffer commandBuffer, const VkPerformanceOverrideInfoINTEL* pOverrideInfo) const
{
    return this->procTable.CmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
}

VkResult VexDevice::AcquirePerformanceConfigurationINTEL(const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, VkPerformanceConfigurationINTEL* pConfiguration) const
{
    return this->procTable.AcquirePerformanceConfigurationINTEL(this->device, pAcquireInfo, pConfiguration);
}

VkResult VexDevice::ReleasePerformanceConfigurationINTEL(VkPerformanceConfigurationINTEL configuration) const
{
    return this->procTable.ReleasePerformanceConfigurationINTEL(this->device, configuration);
}

VkResult VexDevice::QueueSetPerformanceConfigurationINTEL(VkQueue queue, VkPerformanceConfigurationINTEL configuration) const
{
    return this->procTable.QueueSetPerformanceConfigurationINTEL(queue, configuration);
}

VkResult VexDevice::GetPerformanceParameterINTEL(VkPerformanceParameterTypeINTEL parameter, VkPerformanceValueINTEL* pValue) const
{
    return this->procTable.GetPerformanceParameterINTEL(this->device, parameter, pValue);
}

#endif
#if defined(VK_AMD_display_native_hdr)
void VexDevice::SetLocalDimmingAMD(VkSwapchainKHR swapChain, VkBool32 localDimmingEnable) const
{
    this->procTable.SetLocalDimmingAMD(this->device, swapChain, localDimmingEnable);
}

#endif
#if defined(VK_FUCHSIA_imagepipe_surface)
VkResult VexDevice::CreateImagePipeSurfaceFUCHSIA(const VkImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateImagePipeSurfaceFUCHSIA(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_EXT_metal_surface)
VkResult VexDevice::CreateMetalSurfaceEXT(const VkMetalSurfaceCreateInfoEXT* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateMetalSurfaceEXT(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_EXT_buffer_device_address)
VkDeviceAddress VexDevice::GetBufferDeviceAddressEXT(const VkBufferDeviceAddressInfoEXT* pInfo) const
{
    return this->procTable.GetBufferDeviceAddressEXT(this->device, pInfo);
}

#endif
#if defined(VK_NV_cooperative_matrix)
VkResult VexDevice::GetPhysicalDeviceCooperativeMatrixPropertiesNV(uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties) const
{
    return this->procTable.GetPhysicalDeviceCooperativeMatrixPropertiesNV(this->physDevice, pPropertyCount, pProperties);
}

#endif
#if defined(VK_NV_coverage_reduction_mode)
VkResult VexDevice::GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(uint32_t* pCombinationCount, VkFramebufferMixedSamplesCombinationNV* pCombinations) const
{
    return this->procTable.GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(this->physDevice, pCombinationCount, pCombinations);
}

#endif
#if defined(VK_EXT_full_screen_exclusive)
VkResult VexDevice::GetPhysicalDeviceSurfacePresentModes2EXT(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes) const
{
    return this->procTable.GetPhysicalDeviceSurfacePresentModes2EXT(this->physDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
}

VkResult VexDevice::AcquireFullScreenExclusiveModeEXT(VkSwapchainKHR swapchain) const
{
    return this->procTable.AcquireFullScreenExclusiveModeEXT(this->device, swapchain);
}

VkResult VexDevice::ReleaseFullScreenExclusiveModeEXT(VkSwapchainKHR swapchain) const
{
    return this->procTable.ReleaseFullScreenExclusiveModeEXT(this->device, swapchain);
}

#endif
#if defined(VK_EXT_headless_surface)
VkResult VexDevice::CreateHeadlessSurfaceEXT(const VkHeadlessSurfaceCreateInfoEXT* pCreateInfo, VkSurfaceKHR* pSurface) const
{
    return this->procTable.CreateHeadlessSurfaceEXT(this->instance, pCreateInfo, this->allocator, pSurface);
}

#endif
#if defined(VK_EXT_line_rasterization)
void VexDevice::CmdSetLineStippleEXT(VkCommandBuffer commandBuffer, uint32_t lineStippleFactor, uint16_t lineStipplePattern) const
{
    this->procTable.CmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}

#endif
#if defined(VK_EXT_host_query_reset)
void VexDevice::ResetQueryPoolEXT(VkQueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const
{
    this->procTable.ResetQueryPoolEXT(this->device, queryPool, firstQuery, queryCount);
}

#endif
#if defined(VK_KHR_pipeline_executable_properties)
VkResult VexDevice::GetPipelineExecutablePropertiesKHR(const VkPipelineInfoKHR* pPipelineInfo, uint32_t* pExecutableCount, VkPipelineExecutablePropertiesKHR* pProperties) const
{
    return this->procTable.GetPipelineExecutablePropertiesKHR(this->device, pPipelineInfo, pExecutableCount, pProperties);
}

VkResult VexDevice::GetPipelineExecutableStatisticsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pStatisticCount, VkPipelineExecutableStatisticKHR* pStatistics) const
{
    return this->procTable.GetPipelineExecutableStatisticsKHR(this->device, pExecutableInfo, pStatisticCount, pStatistics);
}

VkResult VexDevice::GetPipelineExecutableInternalRepresentationsKHR(const VkPipelineExecutableInfoKHR* pExecutableInfo, uint32_t* pInternalRepresentationCount, VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) const
{
    return this->procTable.GetPipelineExecutableInternalRepresentationsKHR(this->device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}

#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
VkResult VexDevice::GetDeviceGroupPresentCapabilitiesKHR(VkDeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities) const
{
    return this->procTable.GetDeviceGroupPresentCapabilitiesKHR(this->device, pDeviceGroupPresentCapabilities);
}

VkResult VexDevice::GetDeviceGroupSurfacePresentModesKHR(VkSurfaceKHR surface, VkDeviceGroupPresentModeFlagsKHR* pModes) const
{
    return this->procTable.GetDeviceGroupSurfacePresentModesKHR(this->device, surface, pModes);
}

VkResult VexDevice::GetPhysicalDevicePresentRectanglesKHR(VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) const
{
    return this->procTable.GetPhysicalDevicePresentRectanglesKHR(this->physDevice, surface, pRectCount, pRects);
}

#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain))
VkResult VexDevice::AcquireNextImage2KHR(const VkAcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex) const
{
    return this->procTable.AcquireNextImage2KHR(this->device, pAcquireInfo, pImageIndex);
}

#endif
#if (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor))
void VexDevice::CmdPushDescriptorSetWithTemplateKHR(VkCommandBuffer commandBuffer, VkDescriptorUpdateTemplate descriptorUpdateTemplate, VkPipelineLayout layout, uint32_t set, const void* pData) const
{
    this->procTable.CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

#endif
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
VkResult VexDevice::GetDeviceGroupSurfacePresentModes2EXT(const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, VkDeviceGroupPresentModeFlagsKHR* pModes) const
{
    return this->procTable.GetDeviceGroupSurfacePresentModes2EXT(this->device, pSurfaceInfo, pModes);
}

#endif
/* VEX_GENERATE_DEVICE_WRAPPERS_C */
