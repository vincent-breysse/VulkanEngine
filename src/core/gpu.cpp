
#include <pch.hpp>
#include <gyn/gpu.hpp>

bool Gyn::IsDepthStencilFormat(GPUFormat format)
{
    return format == GPUFormat::eDepth24S8;
}

bool Gyn::IsDepthFormat(GPUFormat format)
{
    return format == GPUFormat::eDepth24S8 || format == GPUFormat::eDepth32;
}

bool Gyn::IsStencilFormat(GPUFormat format)
{
    return format == GPUFormat::eDepth24S8;
}

bool Gyn::IsColorFormat(GPUFormat format)
{
    return !IsDepthFormat(format) && !IsStencilFormat(format);
}

UniqueGPUBufferPtr GPU::CreateBuffer(ea::span<const byte> data, GPUBufferUsage usageFlags, GPUMemoryType memoryType)
{
    UniqueGPUBufferPtr buffer = CreateBuffer(data.size(), usageFlags, memoryType, GPUMemoryAllocation::eUninitialized);
    buffer->Upload(data);
    return buffer;
}

void UniqueGPUFencePtrDestroyer::operator()(GPUFence* fence) const
{
    if (fence)
    {
        GPU& gpu = fence->GetGPU();
        gpu.DestroyFence(fence);
    }
}

void UniqueGPURenderPassPtrDestroyer::operator()(GPURenderPass* renderPass) const
{
    if (renderPass)
    {
        GPU& gpu = renderPass->GetGPU();
        gpu.DestroyRenderPass(renderPass);
    }
}

void UniqueGPUFramebufferPtrDestroyer::operator()(GPUFramebuffer* framebuffer) const
{
    if (framebuffer)
    {
        GPU& gpu = framebuffer->GetGPU();
        gpu.DestroyFramebuffer(framebuffer);
    }
}

void UniqueGPUShaderPtrDestroyer::operator()(GPUShader* shader) const
{
    if (shader)
    {
        GPU& gpu = shader->GetGPU();
        gpu.DestroyShader(shader);
    }
}

void UniqueGPUBindSetPtrDestroyer::operator()(GPUBindSet* bindSet) const
{
    if (bindSet)
    {
        GPU& gpu = bindSet->GetGPU();
        gpu.DestroyBindSet(bindSet);
    }
}

void UniqueGPUBufferPtrDestroyer::operator()(GPUBuffer* buffer) const
{
    if (buffer)
    {
        GPU& gpu = buffer->GetGPU();
        gpu.DestroyBuffer(buffer);
    }
}

void UniqueGPUImagePtrDestroyer::operator()(GPUImage* image) const
{
    if (image)
    {
        GPU& gpu = image->GetGPU();
        gpu.DestroyImage(image);
    }
}

void UniqueGPUImageViewPtrDestroyer::operator()(GPUImageView* imageView) const
{
    if (imageView)
    {
        GPU& gpu = imageView->GetGPU();
        gpu.DestroyImageView(imageView);
    }
}