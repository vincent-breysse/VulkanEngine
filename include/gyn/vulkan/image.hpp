#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkanImage : public GPUImage
    {
    public:

        GPUVulkanImage();

        GPUVulkanImage(GPUVulkan& gpu, uint32 width, uint32 height, uint32 layerCount, GPUFormat format, GPUImageType type, GPUImageUsageFlags usage);

        GPUVulkanImage(GPUVulkan& gpu, VkSwapchainKHR swapchain, const VexImage& image);

        ~GPUVulkanImage() override;

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        GPUImageType GetType() const override;

        GPUImageUsageFlags GetUsageFlags() const override;

        GPUMemoryType GetMemoryType() const override;

        GPUFormat GetFormat() const override;

        uint2 GetExtent() const override;

        void Upload(ea::span<const byte> bytes) override;

        const VexImage& GetVexImage() const;

        VkImage GetHandle() const;

        uint32 GetMipLevels() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        uint2 m_extent;
        uint32 m_mipLevels;
        GPUImageType m_imageType;
        GPUImageUsageFlags m_usageFlags;
        GPUFormat m_format;
        const VexImage* m_image;

        // Those handles can be null if we don't own the image (e.g when using a GPUSwapchain image)
        VkDeviceMemory m_memoryOwner;
        VkImage m_imageOwner;
    };

    inline const GPUVulkanImage& CastVulkan(const GPUImage& image)
    {
        ASSERT(image.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanImage&>(image);
    }

    inline GPUVulkanImage& CastVulkan(GPUImage& image)
    {
        ASSERT(image.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanImage&>(image);
    }

    class GPUVulkanImageView : public GPUImageView
    {
    public:

        GPUVulkanImageView();

        GPUVulkanImageView(GPUVulkan& gpu, const GPUVulkanImage& image, GPUImageViewType viewType, GPUImageComponent component, uint32 firstLayer, uint32 layerCount);

        ~GPUVulkanImageView();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        const GPUImage& GetImage() const override;

        GPUImageViewType GetType() const override;

        uint2 GetExtent() const override;

        const VexImageView& GetVexView() const;

        VkImageView GetHandle() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        const GPUVulkanImage* m_image;
        const VexImageView* m_view;
        GPUImageViewType m_viewType;
    };

    ea::vector<const VexImageView*> GetVexImageViews(ea::span<const SharedPtr<const GPUImageView>> span);

    ea::vector<const VexImage*> GetVexImages(ea::span<const SharedPtr<const GPUImageView>> span);

    inline const GPUVulkanImageView& CastVulkan(const GPUImageView& view)
    {
        ASSERT(view.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanImageView&>(view);
    }

    inline GPUVulkanImageView& CastVulkan(GPUImageView& view)
    {
        ASSERT(view.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanImageView&>(view);
    }
}