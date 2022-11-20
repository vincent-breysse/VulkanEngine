
#include <pch.hpp>
#include <gyn/graphics/image2d.hpp>

Image2D::Image2D(GPU& gpu, uint32 width, uint32 height, GPUFormat format, GPUImageUsageFlags usageFlags)
{
    UniqueGPUImagePtr image = gpu.CreateImage(width, height, 1, format, GPUImageType::e2D, usageFlags);
    AllImageViews views = CreateAllAvailableImageViews(gpu, *image, GPUImageViewType::e2D, 0, 1);

    m_format = format;
    m_image = std::move(image);
    m_views = std::move(views);
}

Image2D::Image2D(GPU& gpu, const sf::Image& data)
{
    const uint bytePerTexel = 4; // sf::Image always contains RGBA values
    const sf::Vector2u extent = data.getSize();
    const uint byteCount = extent.x * extent.y * bytePerTexel;

    UniqueGPUImagePtr image = gpu.CreateImage(extent.x, extent.y, 1, GPUFormat::eRGBA8, GPUImageType::e2D, GPUImageUsageFlags::eSample | GPUImageUsageFlags::eTransfer);
    image->Upload(ea::span(data.getPixelsPtr(), byteCount));

    AllImageViews views = CreateAllAvailableImageViews(gpu, *image, GPUImageViewType::e2D, 0, 1);

    m_image = std::move(image);
    m_views = std::move(views);
}

bool Image2D::IsNull() const
{
    return m_image == nullptr;
}

GPUFormat Image2D::GetFormat() const
{
    AssertNotNull();
    return m_format;
}

uint2 Image2D::GetExtent() const
{
    AssertNotNull();
    return m_image->GetExtent();
}

const GPUImage& Image2D::GetGPUImage() const
{
    AssertNotNull();
    return *m_image;
}

const GPUImageView* Image2D::GetView(GPUImageComponent component) const
{
    AssertNotNull();
    ASSERT(ImageFormatContainsComponent(m_format, component));
    return PickImageViewFromComponent(m_views, component);
}

void Image2D::AssertNotNull() const
{
    ASSERT(!IsNull());
}