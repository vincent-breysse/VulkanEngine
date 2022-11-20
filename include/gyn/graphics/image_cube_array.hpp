#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    class ImageCubeArray : NonCopyable
    {
    public:

        ImageCubeArray(GPU& gpu, uint32 size, GPUFormat format, uint32 cubeCount, GPUImageUsageFlags usageFlags);

        bool IsNull() const;

        GPUFormat GetFormat() const;

        uint2 GetExtent() const;

        uint32 GetCubeCount() const;

        const GPUImage& GetGPUImage() const;

        const GPUImageView* GetCubeArrayView(GPUImageComponent component) const;

        const GPUImageView* GetCubeView(uint32 cubeIndex, GPUImageComponent component) const;

    private:

        void AssertNotNull() const;

        GPU* m_gpu = {};
        GPUFormat m_format = {};
        uint32 m_size = {};
        uint32 m_cubeCount = {};
        UniqueGPUImagePtr m_image = {};
        AllImageViews m_cubeArrayView = {};
        ea::vector<AllImageViews> m_cubeViews = {};
    };
}