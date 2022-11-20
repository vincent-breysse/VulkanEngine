
#include <pch.hpp>
#include <gyn/graphics/common.hpp>

ea::array<float4, SSAOKernelSize> Gyn::CreateSSAOKernel()
{
    ea::array<float4, SSAOKernelSize> kernel{};

    std::default_random_engine generator;
    std::uniform_real_distribution<float> random(0.0, 1.0);

    for (size_t i = 0; i < kernel.size(); ++i)
    {
        float3 sample
        {
            random(generator) * 2.0f - 1.0f,
            random(generator) * 2.0f - 1.0f,
            0.1f + 0.9f * random(generator)
        };

        float i01 = (i + 1) / (float)kernel.size();
        float scale = i01 * random(generator);

        sample = Vec3Normalize(sample);
        sample *= scale;

        kernel[i] = float4(sample, 0);
    }

    return kernel;
}

AllImageViews Gyn::CreateAllAvailableImageViews(GPU& gpu, const GPUImage& image, GPUImageViewType viewType, uint32 firstLayer, uint32 layerCount)
{
    GPUFormat format = image.GetFormat();

    bool isColorFormat = IsColorFormat(format);
    bool isDSFormat = IsDepthStencilFormat(format);
    bool isDepthFormat = IsDepthFormat(format);
    bool isStencilFormat = IsStencilFormat(format);

    AllImageViews views = {};
    views.color = isColorFormat ? gpu.CreateImageView(image, viewType, GPUImageComponent::eColor, firstLayer, layerCount) : nullptr;
    views.depthStencil = isDSFormat ? gpu.CreateImageView(image, viewType, GPUImageComponent::eDepthStencil, firstLayer, layerCount) : nullptr;
    views.depth = isDepthFormat ? gpu.CreateImageView(image, viewType, GPUImageComponent::eDepth, firstLayer, layerCount) : nullptr;
    views.stencil = isStencilFormat ? gpu.CreateImageView(image, viewType, GPUImageComponent::eStencil, firstLayer, layerCount) : nullptr;

    return views;
}

bool Gyn::ImageFormatContainsComponent(GPUFormat format, GPUImageComponent component)
{
    switch (component)
    {
    case GPUImageComponent::eColor: return IsColorFormat(format);
    case GPUImageComponent::eDepth: return IsDepthFormat(format);
    case GPUImageComponent::eStencil: return IsStencilFormat(format);
    case GPUImageComponent::eDepthStencil: return IsDepthStencilFormat(format);
    }

    ASSERT(false);
    return false;
}

const GPUImageView* Gyn::PickImageViewFromComponent(const AllImageViews& views, GPUImageComponent component)
{
    switch (component)
    {
    case GPUImageComponent::eColor: return views.color.get();
    case GPUImageComponent::eDepth: return views.depth.get();
    case GPUImageComponent::eStencil: return views.stencil.get();
    case GPUImageComponent::eDepthStencil: return views.depthStencil.get();
    }

    ASSERT(false);
    return nullptr;
}

bool Gyn::operator==(const MatMeshLocalIndex& a, const MatMeshLocalIndex& b)
{
    return a.material == b.material && a.mesh == b.mesh;
}

bool Gyn::operator!=(const MatMeshLocalIndex& a, const MatMeshLocalIndex& b)
{
    return !(a == b);
}

bool Gyn::operator==(const MatMeshID& a, const MatMeshID& b)
{
    return a.material == b.material && a.mesh == b.mesh;
}

bool Gyn::operator!=(const MatMeshID& a, const MatMeshID& b)
{
    return !(a == b);
}