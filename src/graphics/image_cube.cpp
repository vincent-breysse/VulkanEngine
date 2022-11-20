
#include <pch.hpp>
#include <gyn/graphics/image_cube.hpp>

ImageCube::ImageCube(GPU& gpu, uint32 size, GPUFormat format, GPUImageUsageFlags usageFlags)
{
    ASSERT(usageFlags == GPUImageUsageFlags::eRenderAndSample); // Other cases should work but are not tested
    ASSERT_MSG(!IsColorFormat(format), "Color formats not handled yet"); // Color should work but untested
    ASSERT_MSG(!(IsStencilFormat(format) && !IsDepthFormat(format)), "Stencil only formats not handled yet"); // Stencil only should work but untested

    UniqueGPUImagePtr image = gpu.CreateImage(size, size, 6, format, GPUImageType::eCube, usageFlags);

    AllImageViews cubeViews = CreateAllAvailableImageViews(gpu, *image, GPUImageViewType::eCube, 0, 6);

    ea::array<AllImageViews, 6> faceViews = {};

    for (int i = 0; i < 6; ++i)
    {
        faceViews[i] = CreateAllAvailableImageViews(gpu, *image, GPUImageViewType::e2D, i, 1);
    }
    
    m_gpu = &gpu;
    m_format = format;
    m_size = size;
    m_image = std::move(image);
    m_cubeViews = std::move(cubeViews);
    m_faceViews = std::move(faceViews);
}

bool ImageCube::IsNull() const
{
    return m_gpu == nullptr;
}

GPUFormat ImageCube::GetFormat() const
{
    AssertNotNull();
    return m_format;
}

uint2 ImageCube::GetExtent() const
{
    AssertNotNull();
    return uint2(m_size, m_size);
}

const GPUImage& ImageCube::GetGPUImage() const
{
    AssertNotNull();
    return *m_image;
}

const GPUImageView* ImageCube::GetCubeView(GPUImageComponent component) const
{
    AssertNotNull();
    ASSERT(ImageFormatContainsComponent(m_format, component));
    return PickImageViewFromComponent(m_cubeViews, component);
}

const GPUImageView* ImageCube::GetFaceView(uint32 face, GPUImageComponent component) const
{
    AssertNotNull();
    ASSERT(face < 6);
    ASSERT(ImageFormatContainsComponent(m_format, component));
    return PickImageViewFromComponent(m_faceViews[face], component);
}

void ImageCube::AssertNotNull() const
{
    ASSERT(!IsNull());
}