
#include <pch.hpp>
#include <gyn/graphics/image_cube_array.hpp>

ImageCubeArray::ImageCubeArray(GPU& gpu, uint32 size, GPUFormat format, uint32 cubeCount, GPUImageUsageFlags usageFlags)
{
    ASSERT(usageFlags == GPUImageUsageFlags::eRenderAndSample); // Other cases should work but are not tested
    ASSERT_MSG(!IsColorFormat(format), "Color formats not handled yet"); // Color should work but untested
    ASSERT_MSG(!(IsStencilFormat(format) && !IsDepthFormat(format)), "Stencil only formats not handled yet"); // Stencil only should work but untested

    const uint32 layerCount = cubeCount * 6;

    UniqueGPUImagePtr image = gpu.CreateImage(size, size, layerCount, format, GPUImageType::eCube, usageFlags);

    AllImageViews cubeArrayViews = CreateAllAvailableImageViews(gpu, *image, GPUImageViewType::eCubeArray, 0, layerCount);

    ea::vector<AllImageViews> cubeViews{};
    cubeViews.resize(cubeCount);

    for (uint32 i = 0; i < cubeCount; ++i)
    {
        cubeViews[i] = CreateAllAvailableImageViews(gpu, *image, GPUImageViewType::eCube, i * 6, 6);
    }

    m_gpu = &gpu;
    m_format = format;
    m_size = size;
    m_cubeCount = cubeCount;
    m_image = std::move(image);
    m_cubeArrayView = std::move(cubeArrayViews);
    m_cubeViews = std::move(cubeViews);
}

bool ImageCubeArray::IsNull() const
{
    return m_gpu == nullptr;
}

GPUFormat ImageCubeArray::GetFormat() const
{
    AssertNotNull();
    return m_format;
}

uint2 ImageCubeArray::GetExtent() const
{
    AssertNotNull();
    return uint2(m_size, m_size);
}

uint32 ImageCubeArray::GetCubeCount() const
{
    AssertNotNull();
    return m_cubeCount;
}

const GPUImage& ImageCubeArray::GetGPUImage() const
{
    AssertNotNull();
    return *m_image;
}

const GPUImageView* ImageCubeArray::GetCubeArrayView(GPUImageComponent component) const
{
    AssertNotNull();
    ASSERT(ImageFormatContainsComponent(m_format, component));
    return PickImageViewFromComponent(m_cubeArrayView, component);
}

const GPUImageView* ImageCubeArray::GetCubeView(uint32 cubeIndex, GPUImageComponent component) const
{
    AssertNotNull();
    ASSERT(ImageFormatContainsComponent(m_format, component));
    return PickImageViewFromComponent(m_cubeViews[cubeIndex], component);
}

void ImageCubeArray::AssertNotNull() const
{
    ASSERT(!IsNull());
}