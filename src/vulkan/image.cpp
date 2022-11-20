
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/image.hpp>

static VkImageUsageFlags ToVkImageUsageFlags(GPUFormat format, GPUImageUsageFlags usage, uint32 mipLevels)
{
    bool sampleBit = Any(usage & GPUImageUsageFlags::eSample);
    bool transferBit = Any(usage & GPUImageUsageFlags::eTransfer);
    bool renderBit = Any(usage & GPUImageUsageFlags::eRender);

    VkImageUsageFlags vkUsage = 0;

    if (sampleBit)
        vkUsage |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    if (transferBit)
        vkUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    if (mipLevels > 1)
        vkUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // Needed to generate mip chains

    if (sampleBit && renderBit)
        vkUsage |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

    if (renderBit)
    {
        if (IsColorFormat(format))
        {
            vkUsage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        else if (IsDepthFormat(format))
        {
            vkUsage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        }
    }

    return vkUsage;
}

static uint32 ComputeMipLevels(VexDevice& device, VkFormat imageFormat, GPUImageType type, GPUImageUsageFlags usage, uint32 size)
{
    VkFormatProperties formatProperties;
    device.GetPhysicalDeviceFormatProperties(imageFormat, &formatProperties);
    if ((formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) == 0)
        return 1;

    if (type != GPUImageType::e2D || !Any(usage & GPUImageUsageFlags::eSample) || Any(usage & GPUImageUsageFlags::eRender))
        return 1;

    return 1u + ToU32(floor(log2f(float(size))));
}

static VkImage CreateVkImage2D(VexDevice& device, uint32 width, uint32 height, uint32 mipLevels, VkFormat format, VkImageUsageFlags usage, VkDeviceMemory* outMemory)
{
    VkImageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = format;
    info.extent.width = width;
    info.extent.height = height;
    info.extent.depth = 1;
    info.arrayLayers = 1;
    info.usage = usage;
    info.mipLevels = mipLevels;
    info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkMemoryPropertyFlags wantedMemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VkMemoryPropertyFlags unwantedMemoryFlags = 0;

    VkDeviceMemory deviceMemory;
    VkImage image = CreateBackedImage(device, info, wantedMemoryFlags, unwantedMemoryFlags, &deviceMemory);

    *outMemory = deviceMemory;
    return image;
}

static VkImage CreateVkImageCubemap(VexDevice& device, uint32 size, uint32 layerCount, uint32 mipLevels, VkFormat format, VkImageUsageFlags usage, VkDeviceMemory* outMemory)
{
    ASSERT(layerCount % 6 == 0);

    VkImageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = format;
    info.extent.width = size;
    info.extent.height = size;
    info.extent.depth = 1;
    info.arrayLayers = layerCount;
    info.usage = usage;
    info.mipLevels = mipLevels;
    info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkMemoryPropertyFlags wantedMemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VkMemoryPropertyFlags unwantedMemoryFlags = 0;

    VkDeviceMemory deviceMemory;
    VkImage image = CreateBackedImage(device, info, wantedMemoryFlags, unwantedMemoryFlags, &deviceMemory);

    *outMemory = deviceMemory;
    return image;
}

static VkImage CreateVkImage(VexDevice& device, uint32 width, uint32 height, uint32 layerCount, uint32 mipLevels, VkFormat format, VkImageUsageFlags usage, GPUImageType type, VkDeviceMemory* outMemory)
{
    switch (type)
    {
    case GPUImageType::e2D: return CreateVkImage2D(device, width, height, mipLevels, format, usage, outMemory);
    case GPUImageType::eCube: return CreateVkImageCubemap(device, width, layerCount, mipLevels, format, usage, outMemory);
    }

    ASSERT_MSG(false, "Invalid GPUImageType");
    *outMemory = VK_NULL_HANDLE;
    return VK_NULL_HANDLE;
}

GPUVulkanImage::GPUVulkanImage()
{
    m_gpu = nullptr;
    m_extent = uint2(0,0);
    m_mipLevels = 0;
    m_imageType = GPUImageType::eNull;
    m_usageFlags = GPUImageUsageFlags::eNull;
    m_format = GPUFormat::eNull;
    m_image = nullptr;
    m_memoryOwner = VK_NULL_HANDLE;
    m_imageOwner = VK_NULL_HANDLE;
}

GPUVulkanImage::GPUVulkanImage(GPUVulkan& gpu, uint32 width, uint32 height, uint32 layerCount, GPUFormat format, GPUImageType type, GPUImageUsageFlags usage) : GPUVulkanImage()
{
    ASSERT(width > 0);
    ASSERT(height > 0);
    ASSERT(format != GPUFormat::eNull);
    ASSERT(usage != GPUImageUsageFlags::eNull);
    ASSERT(type != GPUImageType::eNull);
    if (type == GPUImageType::eCube)
    {
        ASSERT(width == height);
        ASSERT(layerCount % 6 == 0);
    }

    VexDevice& device = gpu.GetVexDevice();

    VkFormat vkFormat = gpu.ToVkFormat(format);
    uint32 mipLevels = ComputeMipLevels(device, vkFormat, type, usage, Max(width, height));
    VkImageUsageFlags vkUsage = ToVkImageUsageFlags(format, usage, mipLevels);

    VkDeviceMemory imageMemory;
    VkImage image = CreateVkImage(device, width, height, layerCount, mipLevels, vkFormat, vkUsage, type, &imageMemory);

    m_gpu = &gpu;
    m_image = &device.GetVexImage(image);
    m_imageType = type;
    m_usageFlags = usage;
    m_format = format;
    m_extent = uint2(width, height);
    m_mipLevels = mipLevels;
    m_memoryOwner = imageMemory;
    m_imageOwner = image;
}

GPUVulkanImage::GPUVulkanImage(GPUVulkan& gpu, VkSwapchainKHR, const VexImage& image) : GPUVulkanImage()
{
    ASSERT(image.imageType == VK_IMAGE_TYPE_2D);
    ASSERT(image.usage == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    
    m_gpu = &gpu;
    m_image = &image;
    m_extent.x = image.extent.width;
    m_extent.y = image.extent.height;
    m_imageType = GPUImageType::e2D;
    m_usageFlags = GPUImageUsageFlags::eRender;
    m_format = GPUFormat::ePresent;
    m_mipLevels = 1;
}

GPUVulkanImage::~GPUVulkanImage()
{
    if (IsNull())
        return;

    if (m_imageOwner != VK_NULL_HANDLE)
    {
        ASSERT(m_memoryOwner != VK_NULL_HANDLE);
        VexDevice& device = m_gpu->GetVexDevice();

        device.DestroyImage(m_imageOwner);
        device.FreeMemory(m_memoryOwner);
    }
}

bool GPUVulkanImage::IsNull() const
{
    return !m_image;
}

GPUBackend GPUVulkanImage::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanImage::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

GPUImageType GPUVulkanImage::GetType() const
{
    AssertNotNull();
    return m_imageType;
}

GPUImageUsageFlags GPUVulkanImage::GetUsageFlags() const 
{
    AssertNotNull();
    return m_usageFlags;
}

GPUMemoryType GPUVulkanImage::GetMemoryType() const
{
    AssertNotNull();
    return GPUMemoryType::eGPULocal;
}

GPUFormat GPUVulkanImage::GetFormat() const
{
    AssertNotNull();
    return m_format;
}

uint2 GPUVulkanImage::GetExtent() const
{
    AssertNotNull();
    return m_extent;
}

void GPUVulkanImage::Upload(ea::span<const byte> bytes)
{
    AssertNotNull();

    ASSERT_MSG(Any(m_usageFlags & GPUImageUsageFlags::eTransfer), 
        "GPUImage must have been created with GPUImageUsageFlags::eTransfer bit to use GPUImage::Upload");

    ASSERT_MSG(IsColorFormat(m_format),
        "Uploading data to depth buffers is not supported");

    ASSERT(m_mipLevels > 0);

    VexDevice& device = m_gpu->GetVexDevice();
    VkQueue queue = device.graphicsQueue;
    VkCommandPool commandPool = m_gpu->GetCommandPool();
    VkDeviceSize allocSize = bytes.size();

    VkDeviceMemory uploadBufferMemory;
    VkBuffer uploadBuffer;
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = allocSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkMemoryPropertyFlags wantedMemoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        VkMemoryPropertyFlags unwantedMemoryFlags = 0;

        uploadBuffer = CreateBackedBuffer(device, bufferInfo, wantedMemoryFlags, unwantedMemoryFlags, &uploadBufferMemory);

        void* map;
        device.MapMemory(uploadBufferMemory, 0, VK_WHOLE_SIZE, 0, &map);
        memcpy(map, bytes.data(), bytes.size());
        device.UnmapMemory(uploadBufferMemory);
    }

    VkCommandBuffer cmdBuffer;
    AllocateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &cmdBuffer);

    BeginCommandBuffer(device, cmdBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = m_image->handle;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = m_mipLevels;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;

        device.CmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    {
        VkBufferImageCopy region = {};
        region.imageExtent = m_image->extent;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageSubresource.mipLevel = 0;

        device.CmdCopyBufferToImage(cmdBuffer, uploadBuffer, m_image->handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }

    for (uint32 i = 1; i < m_mipLevels; ++i)
    {
        uint32 srcMipLevel = i - 1;
        uint32 dstMipLevel = i;

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = m_image->handle;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.baseMipLevel = srcMipLevel;
        barrier.subresourceRange.levelCount = 1;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;

        device.CmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseArrayLayer = 0;
        subresourceRange.layerCount = 1;
        subresourceRange.baseMipLevel = i;
        subresourceRange.levelCount = 1;

        uint32 srcMipWidth = Max(m_extent.x >> srcMipLevel, 1u);
        uint32 srcMipHeight = Max(m_extent.y >> srcMipLevel, 1u);

        uint32 dstMipWidth = Max(m_extent.x >> dstMipLevel, 1u);
        uint32 dstMipHeight = Max(m_extent.y >> dstMipLevel, 1u);

        VkImageBlit blit = {};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { ToI32(srcMipWidth), ToI32(srcMipHeight), 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = srcMipLevel;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { ToI32(dstMipWidth), ToI32(dstMipHeight), 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = dstMipLevel;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        device.CmdBlitImage(cmdBuffer,
            m_image->handle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_image->handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &blit,
            VK_FILTER_LINEAR);
    }

    {
        bool hasMips = m_mipLevels > 1;
        uint32 barrierCount = hasMips ? 2 : 1;
        uint32 lastMipLevel = m_mipLevels - 1;

        VkImageMemoryBarrier barriers[2] = {};

        barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barriers[0].image = m_image->handle;
        barriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barriers[0].subresourceRange.baseArrayLayer = 0;
        barriers[0].subresourceRange.layerCount = 1;
        barriers[0].subresourceRange.baseMipLevel = lastMipLevel;
        barriers[0].subresourceRange.levelCount = 1;
        barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        if (hasMips)
        {
            barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barriers[1].image = m_image->handle;
            barriers[1].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barriers[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barriers[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barriers[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barriers[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barriers[1].subresourceRange.baseArrayLayer = 0;
            barriers[1].subresourceRange.layerCount = 1;
            barriers[1].subresourceRange.baseMipLevel = 0;
            barriers[1].subresourceRange.levelCount = hasMips ? lastMipLevel : m_mipLevels;
            barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        }

        VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        device.CmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, barrierCount, barriers);
    }
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
    device.DestroyBuffer(uploadBuffer);
    device.FreeMemory(uploadBufferMemory);
}

const VexImage& GPUVulkanImage::GetVexImage() const
{
    AssertNotNull();
    return *m_image;
}

VkImage GPUVulkanImage::GetHandle() const
{
    AssertNotNull();
    return m_image->handle;
}

uint32 GPUVulkanImage::GetMipLevels() const
{
    AssertNotNull();
    return m_mipLevels;
}

void GPUVulkanImage::AssertNotNull() const
{
    ASSERT(!IsNull());
}

static VkImageAspectFlags GetVkImageAspectFlags(GPUImageComponent component)
{
    ASSERT(component != GPUImageComponent::eNull);

    const ea::array Table
    {
        0,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        VK_IMAGE_ASPECT_STENCIL_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
    };

    return Table[EnumCast(component)];
}

static VkResult CreateVkImageView(const GPUVulkanImage& image, GPUImageViewType viewType, GPUImageComponent component, uint32 firstLayer, uint32 layerCount, VkImageView* outView)
{
    VexDevice& device = CastVulkan(image.GetGPU()).GetVexDevice();

    VkImageViewCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image.GetHandle();
    info.format = image.GetVexImage().format;
    info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    info.subresourceRange.aspectMask = GetVkImageAspectFlags(component);
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = image.GetMipLevels();
    
    switch (viewType)
    {
    case GPUImageViewType::e2D:
    {
        ASSERT_MSG(layerCount == 1,
            "Texture2DArray are not yet supported");

        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.subresourceRange.baseArrayLayer = firstLayer;
        info.subresourceRange.layerCount = 1;

        return device.CreateImageView(&info, outView);
    }
    break;

    case GPUImageViewType::eCube:
    {
        ASSERT(layerCount == 6);
        ASSERT(image.GetType() == GPUImageType::eCube);

        info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        info.subresourceRange.baseArrayLayer = firstLayer;
        info.subresourceRange.layerCount = 6;

        return device.CreateImageView(&info, outView);
    }
    break;

    case GPUImageViewType::eCubeArray:
    {
        ASSERT(image.GetType() == GPUImageType::eCube);
        ASSERT(layerCount % 6 == 0);
        ASSERT_MSG(firstLayer == 0, "Layer offset in cube array is not yet supported");

        info.viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = layerCount;

        return device.CreateImageView(&info, outView);
    }
    break;
    }

    ASSERT_MSG(false, "Invalid GPUImageViewType");
    return VK_RESULT_MAX_ENUM;
}

GPUVulkanImageView::GPUVulkanImageView()
{
    m_gpu = nullptr;
    m_image = nullptr;
    m_view = nullptr;
    m_viewType = GPUImageViewType::eNull;
}

GPUVulkanImageView::GPUVulkanImageView(GPUVulkan& gpu, const GPUVulkanImage& image, GPUImageViewType viewType, GPUImageComponent component, uint32 firstLayer, uint32 layerCount) : GPUVulkanImageView()
{
    ASSERT(layerCount > 0);
    if (viewType == GPUImageViewType::eCubeArray)
    {
        ASSERT(firstLayer % 6 == 0);
        ASSERT(layerCount % 6 == 0);
    }
    else if (viewType == GPUImageViewType::eCube)
    {
        ASSERT(firstLayer % 6 == 0);
        ASSERT(layerCount == 6);
    }
    else
    {
        ASSERT(layerCount == 1);
    }

    VkImageView imageView;
    CreateVkImageView(image, viewType, component, firstLayer, layerCount, &imageView);

    m_gpu = &gpu;
    m_viewType = viewType;
    m_image = &image;
    m_view = &gpu.GetVexDevice().GetVexImageView(imageView);
}

GPUVulkanImageView::~GPUVulkanImageView()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();
    device.DestroyImageView(m_view->handle);
}

bool GPUVulkanImageView::IsNull() const
{
    return !m_view;
}

GPUBackend GPUVulkanImageView::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanImageView::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

const GPUImage& GPUVulkanImageView::GetImage() const
{
    AssertNotNull();
    return *m_image;
}

GPUImageViewType GPUVulkanImageView::GetType() const
{
    AssertNotNull();
    return m_viewType;
}

uint2 GPUVulkanImageView::GetExtent() const
{
    AssertNotNull();
    return m_image->GetExtent();
}

const VexImageView& GPUVulkanImageView::GetVexView() const
{
    AssertNotNull();
    return *m_view;
}

VkImageView GPUVulkanImageView::GetHandle() const
{
    AssertNotNull();
    return m_view->handle;
}

void GPUVulkanImageView::AssertNotNull() const
{
    ASSERT(!IsNull());
}

ea::vector<const VexImageView*> Gyn::GetVexImageViews(ea::span<const SharedPtr<const GPUImageView>> span)
{
    ea::vector<const VexImageView*> ret;
    if (span.size() > 0)
    {
        ret.resize(span.size());

        for (int i = 0; i < ret.size(); ++i)
        {
            ret[i] = &CastVulkan(*span[i]).GetVexView();
        }
    }

    return ret;
}

ea::vector<const VexImage*> Gyn::GetVexImages(ea::span<const SharedPtr<const GPUImageView>> span)
{
    ea::vector<const VexImage*> ret;
    if (span.size() > 0)
    {
        ret.resize(span.size());

        for (int i = 0; i < ret.size(); ++i)
        {
            ret[i] = &CastVulkan(span[i]->GetImage()).GetVexImage();
        }
    }

    return ret;
}