
#include <pch.hpp>
#include <VkBootstrap/VkBootstrap.h>
#include <Vex/Device.hpp>
#include <Vex/Swapchain.hpp>

bool VexCreateSwpachain(VexDevice* device, const VexSwapchainCreateInfo* info, const VexSwapchainKHR** outSwapchain)
{
    *outSwapchain = nullptr;

    vkb::SwapchainBuilder builder
    (
        device->physDevice,
        device->device,
        info->surface,
        device->graphicsQueueFamilyIndex,
        device->presentQueueFamilyIndex
    );

    VkSwapchainCreateInfoKHR swapchainInfo;

    const auto ret = builder
        .set_desired_format(info->desiredSurfaceFormat)
        .set_image_usage_flags(info->imageUsageFlags)
        .set_desired_present_mode(info->desiredPresentMode)
        .set_desired_extent(info->desiredWidth, info->desiredHeight)
        .set_desired_image_count(info->desiredNumImage)
        .set_pre_transform_flags(info->preTransform)
        .build(&swapchainInfo);

    if (!ret)
    {
        printf("Failed to initialize Vulkan Swapchain: %s\n", ret.error().message().c_str());
        return false;
    }

    VkSwapchainKHR swapchain = ret.value().swapchain;
    device->RegisterSwapchainKHR(&swapchainInfo, swapchain);

    *outSwapchain = &device->GetVexSwapchainKHR(swapchain);
    return true;
}

VexSwapchainBuilder::VexSwapchainBuilder()
{
    memset(this, 0, sizeof(*this));

    this->SetDesiredSurfaceFormat(VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
    this->SetImageUsageFlags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    this->SetDesiredPresentMode(VK_PRESENT_MODE_MAILBOX_KHR);
    this->SetDesiredExtent(512, 512);
    this->SetDesiredNumImage(2);
}

VexSwapchainBuilder& VexSwapchainBuilder::SetSurface(VkSurfaceKHR surface)
{
    this->info.surface = surface;
    return *this;
}

VexSwapchainBuilder& VexSwapchainBuilder::SetDesiredSurfaceFormat(VkFormat format, VkColorSpaceKHR colorSpace)
{
    this->info.desiredSurfaceFormat.format = format;
    this->info.desiredSurfaceFormat.colorSpace = colorSpace;
    return *this;
}

VexSwapchainBuilder& VexSwapchainBuilder::SetImageUsageFlags(VkImageUsageFlags flags)
{
    this->info.imageUsageFlags = flags;
    return *this;
}

VexSwapchainBuilder& VexSwapchainBuilder::SetDesiredPresentMode(VkPresentModeKHR presentMode)
{
    this->info.desiredPresentMode = presentMode;
    return *this;
}

VexSwapchainBuilder& VexSwapchainBuilder::SetDesiredExtent(uint32_t width, uint32_t height)
{
    this->info.desiredWidth = width;
    this->info.desiredHeight = height;
    return *this;
}

VexSwapchainBuilder& VexSwapchainBuilder::SetDesiredNumImage(uint32_t numImage)
{
    this->info.desiredNumImage = numImage;
    return *this;
}

VexSwapchainBuilder& VexSwapchainBuilder::SetPreTransformFlags(VkSurfaceTransformFlagBitsKHR flags)
{
    this->info.preTransform = flags;
    return *this;
}

bool VexSwapchainBuilder::Build(VexDevice* device, const VexSwapchainKHR** outSwapchain)
{
    return VexCreateSwpachain(device, &this->info, outSwapchain);
}

VkImageCreateInfo VexGetSwapchainImageCreateInfo(const VexSwapchainKHR* swapchain)
{
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = nullptr;
    imageInfo.flags = 0;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = swapchain->imageFormat;
    imageInfo.extent.width = swapchain->imageExtent.width;
    imageInfo.extent.height = swapchain->imageExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = swapchain->imageArrayLayers;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = swapchain->imageUsage;
    imageInfo.sharingMode = swapchain->imageSharingMode;
    imageInfo.queueFamilyIndexCount = uint32_t(swapchain->queueFamilyIndices.size());
    imageInfo.pQueueFamilyIndices = swapchain->queueFamilyIndices.data();
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    return imageInfo;
}