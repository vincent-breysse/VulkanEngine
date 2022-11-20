#pragma once

#include <gyn/vulkan/common.hpp>

namespace Gyn
{
    class GPUVulkanShader : public GPUShader
    {
    public:

        GPUVulkanShader();

        GPUVulkanShader(GPUVulkan& gpu, const ShaderData& data, const GPUVulkanRenderPass& renderPass, uint32 subpass, const ShaderOptions& options);

        ~GPUVulkanShader();

        bool IsNull() const override;

        GPUBackend GetGPUBackend() const override;

        GPU& GetGPU() const override;

        const GPURenderPass& GetRenderPass() const override;

        uint32 GetSubpass() const override;

        VkDescriptorSetLayout GetDescriptorSetLayout(uint32 index) const;

        const VexPipelineLayout& GetVexPipelineLayout() const;

        VkPipelineLayout GetPipelineLayout() const;

        VkPipeline GetPipeline() const;

    private:

        void AssertNotNull() const;

        GPUVulkan* m_gpu;
        const GPURenderPass* m_renderPass;
        uint32 m_subpass;
        const VexShaderModule* m_vertex;
        const VexShaderModule* m_frag;
        ea::vector<VkDescriptorSetLayout> m_dscLayouts;
        const VexPipelineLayout* m_pipelineLayout;
        VkPipeline m_pipeline; // TODO: A shader should not be bound to a PSO. Remove this when we have GPUGraphicsPipeline
    };

    inline const GPUVulkanShader& CastVulkan(const GPUShader& shader)
    {
        ASSERT(shader.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<const GPUVulkanShader&>(shader);
    }

    inline GPUVulkanShader& CastVulkan(GPUShader& shader)
    {
        ASSERT(shader.GetGPUBackend() == GPUBackend::eVulkan);
        return static_cast<GPUVulkanShader&>(shader);
    }
}