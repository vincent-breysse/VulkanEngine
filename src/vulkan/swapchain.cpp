
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/window.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/fence.hpp>
#include <gyn/vulkan/swapchain.hpp>

constexpr uint32 NullImageID = UINT32_MAX;

static const VexSwapchainKHR* CreateSwapchain(VexDevice& device, Window& window)
{
    VexSwapchainBuilder builder = VexSwapchainBuilder()
        .SetSurface(device.mainSurface)
        .SetDesiredSurfaceFormat(VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        .SetDesiredPresentMode(VK_PRESENT_MODE_IMMEDIATE_KHR)
        .SetDesiredExtent(window.GetSize().x, window.GetSize().y)
        .SetDesiredNumImage(3);

    const VexSwapchainKHR* swapchain;
    builder.Build(&device, &swapchain);

    return swapchain;
}

static ea::vector<UniqueGPUImagePtr> CreateGPUImages(GPUVulkan& gpu, const VexSwapchainKHR* swapchain)
{
    VexDevice& device = gpu.GetVexDevice();

    uint32_t imageCount;
    device.GetSwapchainImagesKHR(swapchain->handle, &imageCount, nullptr);

    ea::vector<VkImage> vkImages;
    vkImages.resize(imageCount);

    device.GetSwapchainImagesKHR(swapchain->handle, &imageCount, vkImages.data());

    VkImageCreateInfo imageCreateInfo = VexGetSwapchainImageCreateInfo(swapchain);

    for (VkImage image : vkImages)
    {
        device.RegisterImage(&imageCreateInfo, image);
    }

    ea::vector<UniqueGPUImagePtr> ret;
    ret.resize(imageCount);

    for (uint32 i = 0; i < imageCount; ++i)
    {
        ret[i] = gpu.CreateImage(swapchain->handle, device.GetVexImage(vkImages[i]));
    }

    return ret;
}

static ea::vector<UniqueGPUImageViewPtr> CreateGPUImagesViews(GPUVulkan& gpu, const ea::vector<UniqueGPUImagePtr>& images)
{
    ea::vector<UniqueGPUImageViewPtr> ret = {};
    ret.resize(images.size());

    for (usize i = 0; i < images.size(); ++i)
    {
        ret[i] = gpu.CreateImageView(*images[i], GPUImageViewType::e2D, GPUImageComponent::eColor, 0, 1);
    }

    return ret;
}

GPUVulkanSwapchain::GPUVulkanSwapchain()
{
    m_gpu = nullptr;
    m_swapchain = nullptr;
    m_currentID = NullImageID;
}

GPUVulkanSwapchain::GPUVulkanSwapchain(GPUVulkan& gpu) : GPUVulkanSwapchain()
{
    VexDevice& device = gpu.GetVexDevice();

    const VexSwapchainKHR* swapchain = CreateSwapchain(device, gpu.GetWindow());
    ea::vector<UniqueGPUImagePtr> images = CreateGPUImages(gpu, swapchain);
    ea::vector<UniqueGPUImageViewPtr> imageViews = CreateGPUImagesViews(gpu, images);

    m_gpu = &gpu;
    m_currentID = NullImageID;
    m_swapchain = swapchain;
    m_image = std::move(images);
    m_imageViews = std::move(imageViews);
}

GPUVulkanSwapchain::~GPUVulkanSwapchain()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();
    
    m_imageViews.clear();
    m_image.clear();
    device.DestroySwapchainKHR(m_swapchain->handle);
}

bool GPUVulkanSwapchain::IsNull() const
{
    return m_gpu == nullptr;
}

GPUBackend GPUVulkanSwapchain::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanSwapchain::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

GPUFormat GPUVulkanSwapchain::GetFormat() const
{
    AssertNotNull();
    return GPUFormat::ePresent;
}

uint2 GPUVulkanSwapchain::GetExtent() const
{
    AssertNotNull();
    return ToVec2u(m_swapchain->imageExtent);
}

const VexSwapchainKHR& GPUVulkanSwapchain::GetVexSwapchain() const
{
    AssertNotNull();
    return *m_swapchain;
}

uint32 GPUVulkanSwapchain::AcquireImage()
{
    AssertNotNull();
    ASSERT_MSG(m_currentID == NullImageID, "Trying to acquire a swapchain image before presenting it");

    VexDevice& device = m_gpu->GetVexDevice();

    VkFence fence;
    CreateFence(device, 0, &fence);

    uint32 imageIndex;
    device.AcquireNextImageKHR(m_swapchain->handle, Uint64Max, VK_NULL_HANDLE, fence, &imageIndex);
    device.WaitForFences(1, &fence, true, Uint64Max);

    device.DestroyFence(fence);

    m_currentID = imageIndex;
    return m_currentID;
}

uint32 GPUVulkanSwapchain::GetImageCount() const
{
    AssertNotNull();
    return uint32(m_imageViews.size());
}

const GPUImageView* GPUVulkanSwapchain::GetView(uint32 index) const
{
    AssertNotNull();
    ASSERT_MSG(m_currentID != NullImageID, "Image must have been acquired first");

    return m_imageViews[index].get();
}

void GPUVulkanSwapchain::Present(const GPUFence* waitFence)
{
    AssertNotNull();
    ASSERT_MSG(m_currentID != NullImageID, "Trying to present a swapchain image that has not been acquired");

    VexDevice& device = m_gpu->GetVexDevice();

    ea::span<const VkSemaphore> waitSemaphores = CastVulkan(*waitFence).GetSemaphores();

    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.swapchainCount = 1;
    info.pSwapchains = &m_swapchain->handle;
    info.waitSemaphoreCount = ToU32(waitSemaphores.size());
    info.pWaitSemaphores = waitSemaphores.data();
    info.pImageIndices = &m_currentID;

    device.QueuePresentKHR(device.presentQueue, &info);

    m_currentID = NullImageID;
}

void GPUVulkanSwapchain::AssertNotNull() const
{
    ASSERT(!IsNull());
}