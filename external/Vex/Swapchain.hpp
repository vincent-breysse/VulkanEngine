#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

struct VexDevice;
struct VexSwapchainKHR;

struct VexSwapchainCreateInfo
{
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR desiredSurfaceFormat;
    VkImageUsageFlags imageUsageFlags;
    VkPresentModeKHR desiredPresentMode;
    uint32_t desiredWidth;
    uint32_t desiredHeight;
    uint32_t desiredNumImage;
    VkSurfaceTransformFlagBitsKHR preTransform;
};

bool VexCreateSwpachain(VexDevice* device, const VexSwapchainCreateInfo* info, const VexSwapchainKHR** outSwapchain);

struct VexSwapchainBuilder
{
    VexSwapchainBuilder();

    VexSwapchainBuilder& SetSurface(VkSurfaceKHR surface);

    VexSwapchainBuilder& SetDesiredSurfaceFormat(VkFormat format, VkColorSpaceKHR colorSpace);

    VexSwapchainBuilder& SetImageUsageFlags(VkImageUsageFlags flags);

    VexSwapchainBuilder& SetDesiredPresentMode(VkPresentModeKHR presentMode);

    VexSwapchainBuilder& SetDesiredExtent(uint32_t width, uint32_t height);

    VexSwapchainBuilder& SetDesiredNumImage(uint32_t numImage);

    VexSwapchainBuilder& SetPreTransformFlags(VkSurfaceTransformFlagBitsKHR flags);

    bool Build(VexDevice* device, const VexSwapchainKHR** outSwapchain);

    VexSwapchainCreateInfo info;
};

VkImageCreateInfo VexGetSwapchainImageCreateInfo(const VexSwapchainKHR* swapchain);