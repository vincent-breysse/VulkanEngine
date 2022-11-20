
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/render_pass.hpp>
#include <gyn/vulkan/shader.hpp>

namespace
{
    // TODO : Remove Vertex layout hardcoding
    struct VertexInputLayout
    {
        VkVertexInputBindingDescription binding;
        ea::array<VkVertexInputAttributeDescription, 4> attributes;
    };
}

static VertexInputLayout GetVertexInputLayout()
{
    VertexInputLayout layout = {};

    layout.binding.binding = 0;
    layout.binding.stride = sizeof(Vertex);
    layout.binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    layout.attributes[0].binding = 0;
    layout.attributes[0].location = 0;
    layout.attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    layout.attributes[0].offset = offsetof(Vertex, position);

    layout.attributes[1].binding = 0;
    layout.attributes[1].location = 1;
    layout.attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    layout.attributes[1].offset = offsetof(Vertex, normal);

    layout.attributes[2].binding = 0;
    layout.attributes[2].location = 2;
    layout.attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
    layout.attributes[2].offset = offsetof(Vertex, uv);

    layout.attributes[3].binding = 0;
    layout.attributes[3].location = 3;
    layout.attributes[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    layout.attributes[3].offset = offsetof(Vertex, tangent);

    return layout;
}

static ea::array<VkPipelineShaderStageCreateInfo, 2> CreateShaderStages(VkShaderModule vertexShader, VkShaderModule fragShader)
{
    ea::array<VkPipelineShaderStageCreateInfo, 2> ret = {};

    VkPipelineShaderStageCreateInfo& vertexStage = ret[0];
    vertexStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexStage.pName = "main";
    vertexStage.module = vertexShader;
    vertexStage.stage = VK_SHADER_STAGE_VERTEX_BIT;

    VkPipelineShaderStageCreateInfo& fragStage = ret[1];
    fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragStage.pName = "main";
    fragStage.module = fragShader;
    fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    return ret;
}

static VkPipeline CreateGraphicsPipeline(VexDevice& device, const GPUVulkanRenderPass& renderPass, uint32_t subpass,
    VkPipelineLayout layout, ea::span<const VkPipelineShaderStageCreateInfo> shaderStages, const ShaderOptions& options)
{
    VertexInputLayout vertexInputLayout = GetVertexInputLayout();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pVertexAttributeDescriptions = vertexInputLayout.attributes.data();
    vertexInputInfo.vertexAttributeDescriptionCount = uint32(vertexInputLayout.attributes.size());
    vertexInputInfo.pVertexBindingDescriptions = &vertexInputLayout.binding;
    vertexInputInfo.vertexBindingDescriptionCount = 1u;

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.pNext = nullptr;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = false;

    VkPipelineViewportStateCreateInfo viewportInfo = {};
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.pNext = nullptr;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = nullptr;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = nullptr;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.cullMode = options.backFaceCulling ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_NONE;
    rasterizerInfo.depthBiasClamp = 0;
    rasterizerInfo.depthBiasConstantFactor = 0;
    rasterizerInfo.depthBiasEnable = false;
    rasterizerInfo.depthBiasSlopeFactor = 0;
    rasterizerInfo.depthClampEnable = false;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.pNext = nullptr;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.rasterizerDiscardEnable = false;

    VkPipelineMultisampleStateCreateInfo multisamplingInfo = {};
    multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisamplingInfo.alphaToCoverageEnable = false;
    multisamplingInfo.alphaToOneEnable = false;
    multisamplingInfo.minSampleShading = 0;
    multisamplingInfo.pNext = nullptr;
    multisamplingInfo.pSampleMask = nullptr;
    multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisamplingInfo.sampleShadingEnable = false;

    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilInfo.depthTestEnable = true;
    depthStencilInfo.depthWriteEnable = true;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.blendEnable = false;
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT;

    ea::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
    colorBlendAttachments.resize(renderPass.GetColorAttachmentCount(subpass), colorBlendAttachment);

    VkPipelineColorBlendStateCreateInfo colorBlendingInfo = {};
    colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendingInfo.logicOpEnable = false;
    colorBlendingInfo.attachmentCount = ToU32(colorBlendAttachments.size());
    colorBlendingInfo.pAttachments = colorBlendAttachments.data();

    ea::array dynStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo dynStateInfo = {};
    dynStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynStateInfo.dynamicStateCount = ToU32(dynStates.size());
    dynStateInfo.pDynamicStates = dynStates.data();

    VkGraphicsPipelineCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pStages = shaderStages.data();
    info.stageCount = ToU32(shaderStages.size());
    info.renderPass = renderPass.GetHandle();
    info.pVertexInputState = &vertexInputInfo;
    info.pInputAssemblyState = &inputAssemblyInfo;
    info.pViewportState = &viewportInfo;
    info.pRasterizationState = &rasterizerInfo;
    info.pMultisampleState = &multisamplingInfo;
    info.pColorBlendState = &colorBlendingInfo;
    info.pDynamicState = &dynStateInfo;
    info.pDepthStencilState = &depthStencilInfo;
    info.layout = layout;
    info.subpass = subpass;

    VkPipeline pipeline;
    device.CreateGraphicsPipelines(VK_NULL_HANDLE, 1, &info, &pipeline);
    return pipeline;
}

GPUVulkanShader::GPUVulkanShader()
{
    m_gpu = nullptr;
    m_renderPass = nullptr;
    m_subpass = 0;
    m_vertex = nullptr;
    m_frag = nullptr;
    m_pipelineLayout = nullptr;
    m_pipeline = VK_NULL_HANDLE;
}

GPUVulkanShader::GPUVulkanShader(GPUVulkan& gpu, const ShaderData& data, const GPUVulkanRenderPass& renderPass, uint32 subpass, const ShaderOptions& options) : GPUVulkanShader()
{
    VexDevice& device = gpu.GetVexDevice();
    ea::array<VkPipelineShaderStageCreateInfo, 2> shaderStageCreateInfos = CreateShaderStages(data.vertex, data.frag);
    VkPipeline pipeline = CreateGraphicsPipeline(device, renderPass, subpass, data.pipelineLayout, shaderStageCreateInfos, options);
    
    m_gpu = &gpu;
    m_renderPass = &renderPass;
    m_subpass = subpass;
    m_vertex = &device.GetVexShaderModule(data.vertex);
    m_frag = &device.GetVexShaderModule(data.frag);
    m_dscLayouts = data.dscLayouts;
    m_pipelineLayout = &device.GetVexPipelineLayout(data.pipelineLayout);
    m_pipeline = std::move(pipeline);
}

GPUVulkanShader::~GPUVulkanShader()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();

    device.DestroyPipeline(m_pipeline);
    device.DestroyPipelineLayout(m_pipelineLayout->handle);

    for (VkDescriptorSetLayout layout : m_dscLayouts)
    {
        device.DestroyDescriptorSetLayout(layout);
    }

    device.DestroyShaderModule(m_frag->handle);
    device.DestroyShaderModule(m_vertex->handle);
}

bool GPUVulkanShader::IsNull() const
{
    return m_gpu == nullptr;
}

GPUBackend GPUVulkanShader::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanShader::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

const GPURenderPass& GPUVulkanShader::GetRenderPass() const
{
    AssertNotNull();
    return *m_renderPass;
}

uint32 GPUVulkanShader::GetSubpass() const
{
    AssertNotNull();
    return m_subpass;
}

VkDescriptorSetLayout GPUVulkanShader::GetDescriptorSetLayout(uint32 index) const
{
    AssertNotNull();
    return m_dscLayouts[index];
}

const VexPipelineLayout& GPUVulkanShader::GetVexPipelineLayout() const
{
    AssertNotNull();
    return *m_pipelineLayout;
}

VkPipelineLayout GPUVulkanShader::GetPipelineLayout() const
{
    AssertNotNull();
    return m_pipelineLayout->handle;
}

VkPipeline GPUVulkanShader::GetPipeline() const
{
    AssertNotNull();
    return m_pipeline;
}

void GPUVulkanShader::AssertNotNull() const
{
    ASSERT(!IsNull());
}