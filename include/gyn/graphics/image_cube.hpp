#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    class ImageCube : NonCopyable
    {
    public:

        ImageCube() = default;

        ImageCube(GPU& gpu, uint32 size, GPUFormat format, GPUImageUsageFlags usageFlags);

        bool IsNull() const;

        GPUFormat GetFormat() const;

        uint2 GetExtent() const;

        const GPUImage& GetGPUImage() const;

        const GPUImageView* GetCubeView(GPUImageComponent component) const;

        const GPUImageView* GetFaceView(uint32 face, GPUImageComponent component) const;

    private:

        void AssertNotNull() const;

        GPU* m_gpu = {};
        GPUFormat m_format = {};
        uint32 m_size = {};
        UniqueGPUImagePtr m_image = {};
        AllImageViews m_cubeViews = {};
        ea::array<AllImageViews, 6> m_faceViews = {};
    };
}