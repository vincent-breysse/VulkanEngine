#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkanBindSet : public GPUBindSet
    {
    public:

        GPUVulkanBindSet();

        GPUVulkanBindSet(GPUVulkan& gpu, const GPUVulkanShader& shader, uint32 setIndex);

        ~GPUVulkanBindSet();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        const GPUShader& GetShader() override;

        uint32 GetSetIndex() const override;

        void SetBinding(const GPUImageView* imageView, uint32 binding, GPUSamplerFiltering filtering, GPUSamplerAddressing addressing) override;

        void SetBinding(const GPUBuffer* buffer, uint32 binding) override;

        const VexDescriptorSet& GetVexDescriptorSet() const;

        VkDescriptorSet GetHandle() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        const GPUShader* m_shader;
        uint32 m_setIndex;
        const VexDescriptorSet* m_dscSet;
    };

    inline GPUVulkanBindSet& CastVulkan(GPUBindSet& bindSet)
    {
        ASSERT(bindSet.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanBindSet&>(bindSet);
    }

    inline const GPUVulkanBindSet& CastVulkan(const GPUBindSet& bindSet)
    {
        ASSERT(bindSet.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanBindSet&>(bindSet);
    }

    inline GPUVulkanBindSet* CastVulkan(GPUBindSet* bindSet)
    {
        ASSERT(bindSet->GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanBindSet*>(bindSet);
    }

    inline const GPUVulkanBindSet* CastVulkan(const GPUBindSet* bindSet)
    {
        ASSERT(bindSet->GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanBindSet*>(bindSet);
    }
}