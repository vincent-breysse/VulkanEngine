
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/job_scheduler.hpp>
#include <gyn/window.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/buffer.hpp>
#include <gyn/vulkan/image.hpp>
#include <gyn/vulkan/swapchain.hpp>
#include <gyn/vulkan/shader.hpp>
#include <gyn/vulkan/bind_set.hpp>
#include <gyn/vulkan/render_pass.hpp>
#include <gyn/vulkan/framebuffer.hpp>
#include <gyn/vulkan/fence.hpp>
#include <gyn/vulkan/command_buffer.hpp>

static VkCommandPool CreateCommandPool(VexDevice& device, uint32_t queueFamily)
{
    VkCommandPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    info.queueFamilyIndex = queueFamily;

    VkCommandPool handle = VK_NULL_HANDLE;
    device.CreateCommandPool(&info, &handle);

    return handle;
}

static ea::vector<VkCommandPool> CreateWorkerCommandPools(VexDevice& device, uint32_t queueFamily, uint32 nbWorker)
{
    ea::vector<VkCommandPool> pools{};
    pools.resize(nbWorker);

    for (uint32 i = 0; i < nbWorker; ++i)
    {
        pools[i] = CreateCommandPool(device, queueFamily);
    }

    return pools;
}

static VkDescriptorPool CreateDescriptorPool(VexDevice& device)
{
    ea::array<VkDescriptorPoolSize, 4> sizes = {};

    sizes[0].descriptorCount = 1024;
    sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    sizes[1].descriptorCount = 256;
    sizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

    sizes[2].descriptorCount = 512;
    sizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    sizes[3].descriptorCount = 256;
    sizes[3].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

    VkDescriptorPoolCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    info.maxSets = 2048;
    info.poolSizeCount = ToU32(sizes.size());
    info.pPoolSizes = sizes.data();
    info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    VkDescriptorPool handle;
    device.CreateDescriptorPool(&info, &handle);

    return handle;
}

static VkDescriptorSetLayout CreateDummyDescriptorSetLayout(GPUVulkan& gpu)
{
    std::array<VkDescriptorSetLayoutBinding, 1> bindings = {};
    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].stageFlags = VK_SHADER_STAGE_ALL;

    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = ToU32(bindings.size());
    info.pBindings = bindings.data();

    VkDescriptorSetLayout handle;
    gpu.GetVexDevice().CreateDescriptorSetLayout(&info, &handle);

    return handle;
}

static VkSurfaceKHR CreateMainSurfaceCallback(VkInstance instance, const VkAllocationCallbacks* pAllocator, void* userPtr)
{
    Window* window = reinterpret_cast<Window*>(userPtr);

    VkSurfaceKHR surface;
    
    VkResult result = window->CreateVkSurface(instance, pAllocator, &surface);
    if (result != VK_SUCCESS)
    {
        fmt::print("Vulkan Surface creation failed");
        return VK_NULL_HANDLE;
    }

    return surface;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void*)
{
    static std::mutex mutex{};
    std::scoped_lock lock(mutex);

    static ea::set<std::string> messagePrint;
    const std::string msgIDName = std::string(pCallbackData->pMessageIdName);

    if (messagePrint.find(msgIDName) == messagePrint.end())
    {
        const char* severityStr = ToString(messageSeverity);
        const char* typeStr = ToString(messageType);

        const char* layout =
            "Vulkan Debug Callback [{}, {}]\n"
            "============================================================\n"
            "{}\n\n";

        fmt::print(layout, severityStr, typeStr, pCallbackData->pMessage);

        messagePrint.emplace(msgIDName);

        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            std::terminate();
        }
    }

    return VK_FALSE;
}
static auto DebugCallbackMaybeUnused = &DebugCallback; // Avoid warning in release mode

GPUVulkan::GPUVulkan()
{
    m_jobScheduler = nullptr;
    m_window = nullptr;
    m_device = nullptr;
    m_commandPool = VK_NULL_HANDLE;
    m_descriptorPool = VK_NULL_HANDLE;
    m_linearRepeatSampler = VK_NULL_HANDLE;
    m_linearMirroredRepeatSampler = VK_NULL_HANDLE;
    m_linearClampToEdgeSampler = VK_NULL_HANDLE;
    m_dummyBindSetLayout = VK_NULL_HANDLE;
    m_dummyBindSet = VK_NULL_HANDLE;
    m_presentPass = nullptr;
    m_swapchain = nullptr;
}

GPUVulkan::GPUVulkan(Window& window, JobScheduler& jobScheduler) : GPUVulkan()
{
    ea::span<const char*> instanceExtensions = Window::GetVkRequiredInstanceExtensions();
    
    VkPhysicalDeviceFeatures physDeviceFeatures = {};
    physDeviceFeatures.imageCubeArray = true;
    physDeviceFeatures.geometryShader = true;
    physDeviceFeatures.fragmentStoresAndAtomics = true;

    VexDeviceBuilder deviceBuilder = VexDeviceBuilder()
#if GYN_GPU_VALIDATION_LAYER
        .SetValidationLayersCallback(DebugCallback, nullptr)
        .EnableValidationLayers()
#endif
        .SetMinVersion(1, 1)
        .SetCreateMainSurfaceCallback(CreateMainSurfaceCallback, &window)
        .EnableInstanceExtensions(instanceExtensions.data(), ToU32(instanceExtensions.size()))
        .SetPhysicalDeviceFeatures(&physDeviceFeatures);

    VexDevice* pDevice = new VexDevice();
    deviceBuilder.Build(pDevice);

    VexDevice& device = *pDevice;

    VkCommandPool commandPool = CreateCommandPool(device, device.graphicsQueueFamilyIndex);
    ea::vector<VkCommandPool> workerCommandPools = CreateWorkerCommandPools(device, device.graphicsQueueFamilyIndex, jobScheduler.GetWorkerCount() + 1);
    VkDescriptorPool descriptorPool = CreateDescriptorPool(device);

    VkSampler linearRepeatSampler;
    CreateSampler(device, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_REPEAT, &linearRepeatSampler);

    VkSampler linearMirroredRepeatSampler;
    CreateSampler(device, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT, &linearMirroredRepeatSampler);

    VkSampler linearClampToEdgeSampler;
    CreateSampler(device, VK_FILTER_LINEAR, VK_FILTER_LINEAR, VK_SAMPLER_MIPMAP_MODE_LINEAR, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, &linearClampToEdgeSampler);

    m_jobScheduler = &jobScheduler;
    m_window = &window;
    m_device = pDevice;
    m_commandPool = std::move(commandPool);
    m_workerCommandPools = std::move(workerCommandPools);
    m_descriptorPool = std::move(descriptorPool);
    m_linearRepeatSampler = std::move(linearRepeatSampler);
    m_linearMirroredRepeatSampler = std::move(linearMirroredRepeatSampler);
    m_linearClampToEdgeSampler = std::move(linearClampToEdgeSampler);

    m_swapchain = NewUnique<GPUVulkanSwapchain>(*this);
    m_presentPass = CreateRenderPass(ea::array{ GPUFormat::ePresent }, false);
    m_dummyBindSetLayout = CreateDummyDescriptorSetLayout(*this);

    AllocateDescriptorSet(device, GetDescriptorPool(), m_dummyBindSetLayout, &m_dummyBindSet);

    Flush();
}

GPUVulkan::~GPUVulkan()
{
    Flush();

    m_swapchain = nullptr;
    m_presentPass = nullptr;

    for (ea::vector<VkCommandBuffer>& cmdBufferArray : m_cmdBuffers)
    {
        FreeCommandBuffers(*m_device, cmdBufferArray);
    }

    FreeDescriptorSets(*m_device, ea::span(&m_dummyBindSet, 1));
    m_device->DestroyDescriptorSetLayout(m_dummyBindSetLayout);

    m_device->DestroySampler(m_linearRepeatSampler);
    m_device->DestroySampler(m_linearMirroredRepeatSampler);
    m_device->DestroySampler(m_linearClampToEdgeSampler);

    m_device->DestroyDescriptorPool(m_descriptorPool);
    for (VkCommandPool pool : m_workerCommandPools)
    {
        m_device->DestroyCommandPool(pool);
    }
    m_device->DestroyCommandPool(m_commandPool);

    m_device->Destroy();

    delete m_device;
}

bool GPUVulkan::IsNull() const
{
    return m_window == nullptr;
}

GPUBackend GPUVulkan::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPUProperties GPUVulkan::GetProperties() const
{
    AssertNotNull();
    
    VkPhysicalDeviceProperties props = {};
    m_device->GetPhysicalDeviceProperties(&props);

    GPUProperties ret = {};
    ret.deviceName = props.deviceName;

    return ret;
}

GPUSwapchain* GPUVulkan::GetSwapchain()
{
    AssertNotNull();
    return m_swapchain.get();
}

const GPURenderPass& GPUVulkan::GetPresentRenderPass() const
{
    AssertNotNull();
    return *m_presentPass;
}

static std::string LoadGLSLShader(const char* path)
{
    std::ifstream file(path);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

static ea::vector<VkDescriptorSetLayout> CreateDescriptorSetLayouts(VexDevice& device, const VexShaderCompilation& compilation)
{
    const std::vector<VexReflectDescriptorSetBinding>& vexBindings = compilation.reflection.bindings;
    const std::vector<VexReflectDescriptorSet>& vexSets = compilation.reflection.descriptorSets;

    ea::vector<VkDescriptorSetLayout> layouts;
    layouts.resize(vexSets.size());

    for (size_t i = 0; i < vexSets.size(); ++i)
    {
        const VexReflectDescriptorSet& vexSet = vexSets[i];

        uint32 numBinding = vexSet.rangeEnd - vexSet.rangeBegin;

        ea::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.resize(numBinding);

        for (uint32 j = 0; j < numBinding; ++j)
        {
            const VexReflectDescriptorSetBinding& vexBinding = vexBindings[vexSet.rangeBegin + j];

            VkDescriptorSetLayoutBinding& vkBinding = bindings[j];
            vkBinding.binding = vexBinding.binding;
            vkBinding.descriptorCount = vexBinding.count;
            vkBinding.descriptorType = vexBinding.type;
            vkBinding.stageFlags = VK_SHADER_STAGE_ALL;
        }

        VkDescriptorSetLayoutCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        info.bindingCount = ToU32(bindings.size());
        info.pBindings = bindings.data();

        device.CreateDescriptorSetLayout(&info, &layouts[i]);
    }

    return layouts;
}

static VkResult CreatePipelineLayout(VexDevice& device, ea::span<const VkDescriptorSetLayout> setLayouts, VkPipelineLayout* pHandle)
{
    VkPipelineLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.pSetLayouts = setLayouts.data();
    info.setLayoutCount = ToU32(setLayouts.size());
    
    return device.CreatePipelineLayout(&info, pHandle);
}

static VkResult CreateShaderModule(VexDevice& device, const VexShaderStageCompilation& stage, VkShaderModule* pHandle)
{
    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = stage.spirvCodeSize;
    info.pCode = stage.spirvCode.data();

    return device.CreateShaderModule(&info, pHandle);
}

static bool CheckShaderStageCompilation(const char* path, const VexShaderStageCompilation& compilation)
{
    if (compilation.errorCount > 0 || compilation.warningCount > 0 || !compilation.errorMessage.empty())
    {
        fmt::print("Shader compilation failed:\nStage '{}'\nPath '{}'\nError count ({})\nWarning count ({})\n{}", 
            ToString(compilation.stage), 
            path, 
            compilation.errorCount, 
            compilation.warningCount,
            compilation.errorMessage);

        return false;
    }

    return true;
}

static bool CheckShaderStageCompilation(const char* path, const VexShaderStageCompilation& vertex, const VexShaderStageCompilation& frag)
{
    return CheckShaderStageCompilation(path, vertex) && CheckShaderStageCompilation(path, frag);
}

UniqueGPUShaderPtr GPUVulkan::LoadShaderFromFile(const char* path, const GPURenderPass& renderPassLayout, uint32 subpass, const ShaderOptions& options)
{
    std::string glsl = LoadGLSLShader(path);

    ea::array vertexKeywords =
    {
        VexShaderKeyword{"VERTEX_SHADER", "1"},
        VexShaderKeyword{"FRAGMENT_SHADER", "0"}
    };

    ea::array fragKeywords =
    {
        VexShaderKeyword{"VERTEX_SHADER", "0"},
        VexShaderKeyword{"FRAGMENT_SHADER", "1"}
    };

    VexShaderCompilation compilation = VexShaderCompiler()
        .SetVertexSourceGLSL(glsl.c_str(), uint32_t(glsl.size()))
        .SetFragSourceGLSL(glsl.c_str(), uint32_t(glsl.size()))
        .SetVertexKeywords(vertexKeywords.data(), ToU32(vertexKeywords.size()))
        .SetFragKeywords(fragKeywords.data(), ToU32(fragKeywords.size()))
        .Compile();

    if (!CheckShaderStageCompilation(path, compilation.vertex, compilation.frag))
    {
        ASSERT(false);
        return nullptr;
    }

    ea::vector<VkDescriptorSetLayout> dscLayouts = CreateDescriptorSetLayouts(*m_device, compilation);

    VkPipelineLayout pipelineLayout;
    CreatePipelineLayout(*m_device, dscLayouts, &pipelineLayout);

    VkShaderModule vertexShader;
    CreateShaderModule(*m_device, compilation.vertex, &vertexShader);

    VkShaderModule fragShader;
    CreateShaderModule(*m_device, compilation.frag, &fragShader);

    ShaderData data = {};
    data.dscLayouts = std::move(dscLayouts);
    data.pipelineLayout = pipelineLayout;
    data.vertex = vertexShader;
    data.frag = fragShader;

    GPUVulkanShader* shader = new GPUVulkanShader(*this, data, CastVulkan(renderPassLayout), subpass, options);
    return UniqueGPUShaderPtr(shader);
}

void GPUVulkan::DestroyShader(GPUShader* shader)
{
    AssertNotNull();
    ASSERT(shader != nullptr);

    delete shader;
}

UniqueGPURenderPassPtr GPUVulkan::CreateRenderPass(ea::span<const GPUFormat> formats, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks)
{
    AssertNotNull();
    GPUVulkanRenderPass* renderPass = new GPUVulkanRenderPass(*this, formats, subpassLayouts, multiviewMasks);
    return UniqueGPURenderPassPtr(renderPass);
}

UniqueGPURenderPassPtr GPUVulkan::CreateRenderPass(ea::span<const GPUFormat> colorFormats, bool hasDepthStencil)
{
    AssertNotNull();
    GPUVulkanRenderPass* renderPass = new GPUVulkanRenderPass(*this, colorFormats, hasDepthStencil);
    return UniqueGPURenderPassPtr(renderPass);
}

void GPUVulkan::DestroyRenderPass(GPURenderPass* renderPass)
{
    AssertNotNull();
    ASSERT(renderPass != nullptr);

    delete renderPass;
}

UniqueGPUBindSetPtr GPUVulkan::CreateBindSet(const GPUShader& shader, uint32 set)
{
    AssertNotNull();
    GPUVulkanBindSet* bindSet = new GPUVulkanBindSet(*this, CastVulkan(shader), set);
    return UniqueGPUBindSetPtr(bindSet);
}

void GPUVulkan::DestroyBindSet(GPUBindSet* bindSet)
{
    AssertNotNull();
    ASSERT(bindSet != nullptr);

    delete bindSet;
}

UniqueGPUBufferPtr GPUVulkan::CreateBuffer(uint64 allocSize, GPUBufferUsage usage, GPUMemoryType memoryType, GPUMemoryAllocation allocation)
{
    AssertNotNull();
    GPUVulkanBuffer* buffer = new GPUVulkanBuffer(*this, allocSize, usage, memoryType, allocation);
    return UniqueGPUBufferPtr(buffer);
}

void GPUVulkan::DestroyBuffer(GPUBuffer* buffer)
{
    AssertNotNull();
    ASSERT(buffer != nullptr);

    delete buffer;
}

UniqueGPUImagePtr GPUVulkan::CreateImage(uint32 width, uint32 height, uint32 layerCount, GPUFormat format, GPUImageType type, GPUImageUsageFlags usage)
{
    AssertNotNull();
    GPUVulkanImage* image = new GPUVulkanImage(*this, width, height, layerCount, format, type, usage);
    return UniqueGPUImagePtr(image);
}

void GPUVulkan::DestroyImage(GPUImage* image)
{
    AssertNotNull();
    ASSERT(image != nullptr);

    delete image;
}

UniqueGPUImageViewPtr GPUVulkan::CreateImageView(const GPUImage& image, GPUImageViewType viewType, GPUImageComponent component, uint32 firstLayer, uint32 layerCount)
{
    AssertNotNull();
    GPUVulkanImageView* imageView = new GPUVulkanImageView(*this, CastVulkan(image), viewType, component, firstLayer, layerCount);
    return UniqueGPUImageViewPtr(imageView);
}

void GPUVulkan::DestroyImageView(GPUImageView* imageView)
{
    AssertNotNull();
    ASSERT(imageView != nullptr);

    delete imageView;
}

UniqueGPUFramebufferPtr GPUVulkan::CreateFramebuffer(const GPURenderPass* renderPassLayout, ea::span<const GPUImageView* const> attachments)
{
    AssertNotNull();

    ea::vector<const GPUImageView*> copy(attachments.data(), attachments.data() + attachments.size());
    GPUVulkanFramebuffer* framebuffer = new GPUVulkanFramebuffer(*this, &CastVulkan(*renderPassLayout), std::move(copy));
    return UniqueGPUFramebufferPtr(framebuffer);
}

void GPUVulkan::DestroyFramebuffer(GPUFramebuffer* framebuffer)
{
    AssertNotNull();
    ASSERT(framebuffer != nullptr);

    delete framebuffer;
}

ea::span<const Vertex> GPUVulkan::GetScreenQuadVertices()
{
    static const ea::array ScreenQuadVertices
    {
        Vertex{ float3(-1.0f, -1.0f, 0.f),  float3(0, 0, -1),        float2(0.0f, 1.0f),    float3(1, 0, 0) },
        Vertex{ float3(-1.0f, 1.0f, 0.f),   float3(0, 0, -1),        float2(0.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, 1.0f, 0.f),    float3(0, 0, -1),        float2(1.0f, 0.0f),    float3(1, 0, 0) },
        Vertex{ float3(1.0f, -1.0f, 0.f),   float3(0, 0, -1),        float2(1.0f, 1.0f),    float3(1, 0, 0) },
    };

    AssertNotNull();
    return ScreenQuadVertices;
}

ea::span<const uint16> GPUVulkan::GetScreenQuadIndices()
{
    static const ea::array<uint16, 6> ScreenQuadIndices
    {
        0,1,2,
        2,3,0,
    };

    AssertNotNull();
    return ScreenQuadIndices;
}

UniqueGPUFencePtr GPUVulkan::Submit(const GPUFrameExecution& exec)
{
    AssertNotNull();

    ea::vector<VkFence> signalFences = {};
    ea::vector<VkSemaphore> signalSemaphores = {};
    ea::vector<VkCommandBuffer> commandBuffers = RecordRendering(*this, *m_jobScheduler, exec, signalFences, signalSemaphores);

    if (m_cmdBuffers.size() <= exec.frameIndex)
    {
        m_cmdBuffers.resize(exec.frameIndex + 1);
    }

    FreeCommandBuffers(*m_device, m_cmdBuffers[exec.frameIndex]);
    m_cmdBuffers[exec.frameIndex] = std::move(commandBuffers);

    return CreateFence(std::move(signalFences), std::move(signalSemaphores));
}

void GPUVulkan::DestroyFence(GPUFence* fence)
{
    AssertNotNull();
    ASSERT(fence != nullptr);
    delete fence;
}

void GPUVulkan::Flush()
{
    AssertNotNull();
    m_device->DeviceWaitIdle();
}

Window& GPUVulkan::GetWindow()
{
    AssertNotNull();
    return *m_window;
}

VexDevice& GPUVulkan::GetVexDevice()
{
    AssertNotNull();
    return *m_device;
}

VkCommandPool GPUVulkan::GetCommandPool()
{
    AssertNotNull();
    return m_commandPool;
}

VkCommandPool GPUVulkan::GetWorkerCommandPool(uint32 workerIndex)
{
    AssertNotNull();
    return m_workerCommandPools[workerIndex];
}

VkDescriptorPool GPUVulkan::GetDescriptorPool()
{
    AssertNotNull();
    return m_descriptorPool;
}

VkSampler GPUVulkan::GetSampler(GPUSamplerFiltering, GPUSamplerAddressing addressing) const
{
    AssertNotNull();

    if (addressing == GPUSamplerAddressing::eRepeat)
    {
        return m_linearRepeatSampler;
    }
    else if (addressing == GPUSamplerAddressing::eMirroredRepeat)
    {
        return m_linearMirroredRepeatSampler;
    }
    else if (addressing == GPUSamplerAddressing::eClampToEdge)
    {
        return m_linearClampToEdgeSampler;
    }

    ASSERT_MSG(false, "Unimplemented case or invalid arguments");
    std::terminate();
}

VkFormat GPUVulkan::ToVkFormat(GPUFormat format) const
{
    AssertNotNull();
    ASSERT(format != GPUFormat::eNull);
    
    switch (format)
    {
    case GPUFormat::eUNorm8x1: return VK_FORMAT_R8_UNORM;
    case GPUFormat::eUNorm8x4: return VK_FORMAT_R8G8B8A8_UNORM;
    case GPUFormat::eSNorm8x4: return VK_FORMAT_R8G8B8A8_SNORM;
    case GPUFormat::eUNorm16x4: return VK_FORMAT_R16G16B16A16_UNORM;
    case GPUFormat::eSNorm16x4: return VK_FORMAT_R16G16B16A16_SNORM;
    case GPUFormat::eSFloat16x4: return VK_FORMAT_R16G16B16A16_SFLOAT;
    case GPUFormat::eDepth24S8: 
    {
        VkFormatProperties properties;
        m_device->GetPhysicalDeviceFormatProperties(VK_FORMAT_D24_UNORM_S8_UINT, &properties);

        if (properties.optimalTilingFeatures != 0)
        {
            return VK_FORMAT_D24_UNORM_S8_UINT;
        }
        else
        {
            return VK_FORMAT_D32_SFLOAT_S8_UINT;
        }
    }
    break;
    case GPUFormat::eDepth32: return VK_FORMAT_D32_SFLOAT;
    case GPUFormat::eUint16: return VK_FORMAT_R16_UINT;
    case GPUFormat::ePresent: return m_swapchain->GetVexSwapchain().imageFormat;
    }

    ASSERT_MSG(false, "Invalid argument or case not implemented");
    return {};
}

UniqueGPUImagePtr GPUVulkan::CreateImage(VkSwapchainKHR swapchain, const VexImage& vexImage)
{
    AssertNotNull();
    GPUVulkanImage* image = new GPUVulkanImage(*this, swapchain, vexImage);
    return UniqueGPUImagePtr(image);
}

VkDescriptorSet GPUVulkan::GetDummyBindSet() const
{
    AssertNotNull();
    return m_dummyBindSet;
}

UniqueGPUFencePtr GPUVulkan::CreateFence(ea::span<const VkFence> fences, ea::span<const VkSemaphore> semaphores)
{
    AssertNotNull();
    GPUVulkanFence* fence = new GPUVulkanFence(*this, fences, semaphores);
    return UniqueGPUFencePtr(fence);
}

UniqueGPUFencePtr GPUVulkan::CreateFence(VkFence fence, VkSemaphore semaphore)
{
    AssertNotNull();
    return CreateFence(ea::span(&fence, 1), ea::span(&semaphore, 1));
}

void GPUVulkan::AssertNotNull() const
{
    ASSERT(!IsNull());
}