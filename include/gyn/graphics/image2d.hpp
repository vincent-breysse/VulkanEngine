#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    class Image2D : NonCopyable
    {
    public:

        Image2D() = default;

        Image2D(GPU& gpu, uint32 width, uint32 height, GPUFormat format, GPUImageUsageFlags usageFlags);

        // Construct readonly texture from CPU data. Image data upload is not yet supported except at creation time, so leave it as a constructor for now.
        Image2D(GPU& gpu, const sf::Image& data);

        bool IsNull() const;

        GPUFormat GetFormat() const;

        uint2 GetExtent() const;

        const GPUImage& GetGPUImage() const;

        const GPUImageView* GetView(GPUImageComponent component) const;

    private:

        void AssertNotNull() const;

        GPUFormat m_format = {};
        UniqueGPUImagePtr m_image = {};
        AllImageViews m_views = {};
    };
}