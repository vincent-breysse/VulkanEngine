
#include <pch.hpp>
#include <gyn/window.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/swapchain.hpp>
#include <gyn/vulkan/render_pass.hpp>
#include <gyn/vulkan/image.hpp>

using namespace Gyn;

namespace
{
    struct FrameRenderBuffers
    {
        VkDeviceMemory      VertexBufferMemory;
        VkDeviceMemory      IndexBufferMemory;
        VkDeviceSize        VertexBufferSize;
        VkDeviceSize        IndexBufferSize;
        VkBuffer            VertexBuffer;
        VkBuffer            IndexBuffer;
    };

    struct WindowRenderBuffers
    {
        uint32_t            Index;
        uint32_t            Count;
        FrameRenderBuffers* FrameRenderBuffers;
    };

    struct ImGuiData
    {
        VkDescriptorPool descriptorPool;
        VkRenderPass renderPass;
        VkCommandPool commandPool;

        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorSet descriptorSet;
        VkPipelineLayout pipelineLayout;
        VkShaderModule vertexShader;
        VkShaderModule fragShader;
        VkPipeline pipeline;
        WindowRenderBuffers windowRenderBuffers;
        uint32 imageCount;

        VkDeviceMemory vertexBufferMemory;
        VkBuffer vertexBuffer;
        VkDeviceSize vertexBufferSize;

        VkDeviceMemory indexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceSize indexBufferSize;

        VkSampler fontSampler;
        VkDeviceMemory fontImageMemory;
        VkImage fontImage;
        VkImageView fontImageView;

        ea::hash_map<const GPUImageView*, VkDescriptorSet> imageDscSets;
    };
}

//-----------------------------------------------------------------------------
// SHADERS
//-----------------------------------------------------------------------------

// glsl_shader.vert, compiled with:
// # glslangValidator -V -x -o glsl_shader.vert.u32 glsl_shader.vert
/*
#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;
layout(push_constant) uniform uPushConstant { vec2 uScale; vec2 uTranslate; } pc;

out gl_PerVertex { vec4 gl_Position; };
layout(location = 0) out struct { vec4 Color; vec2 UV; } Out;

void main()
{
    Out.Color = aColor;
    Out.UV = aUV;
    gl_Position = vec4(aPos * pc.uScale + pc.uTranslate, 0, 1);
}
*/
static uint32_t glsl_shader_vert_spv[] =
{
    0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
    0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
    0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
    0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
    0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
    0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
    0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
    0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
    0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
    0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
    0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
    0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
    0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
    0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
    0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
    0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
    0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
    0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
    0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
    0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
    0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
    0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
    0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
    0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
    0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
    0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
    0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
    0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
    0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
    0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
    0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
    0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
    0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
    0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
    0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
    0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
    0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
    0x0000002d,0x0000002c,0x000100fd,0x00010038
};

// glsl_shader.frag, compiled with:
// # glslangValidator -V -x -o glsl_shader.frag.u32 glsl_shader.frag
/*
#version 450 core
layout(location = 0) out vec4 fColor;
layout(set=0, binding=0) uniform sampler2D sTexture;
layout(location = 0) in struct { vec4 Color; vec2 UV; } In;
void main()
{
    fColor = In.Color * texture(sTexture, In.UV.st);
}
*/
static uint32_t glsl_shader_frag_spv[] =
{
    0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
    0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
    0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
    0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
    0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
    0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
    0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
    0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
    0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
    0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
    0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
    0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
    0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
    0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
    0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
    0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
    0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
    0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
    0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
    0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
    0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
    0x00010038
};

static ImGuiData g_imGuiData;

static void CheckVkResult(VkResult err)
{
    if (err == 0)
        return;

    fmt::print("ImGui Error: VkResult = %d\n");

    if (err < 0)
        std::terminate();
}

static VkSampler CreateFontSampler(VexDevice* device)
{
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = VK_FILTER_LINEAR;
    info.minFilter = VK_FILTER_LINEAR;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.minLod = -1000;
    info.maxLod = 1000;
    info.maxAnisotropy = 1.0f;

    VkSampler fontSampler;
    device->CreateSampler(&info, &fontSampler);
    return fontSampler;
}

static VkDescriptorSetLayout CreateDescriptorSetLayout(VexDevice* device, VkSampler fontSampler)
{
    VkSampler sampler[1] = { fontSampler };

    VkDescriptorSetLayoutBinding binding[1] = {};
    binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    binding[0].descriptorCount = 1;
    binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    binding[0].pImmutableSamplers = sampler;

    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = 1;
    info.pBindings = binding;

    VkDescriptorSetLayout dscSetLayout;
    device->CreateDescriptorSetLayout(&info, &dscSetLayout);
    return dscSetLayout;
}

static VkDescriptorSet CreateDescriptorSet(VexDevice* device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout)
{
    VkDescriptorSetAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = descriptorPool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &layout;

    VkDescriptorSet dscSet;
    device->AllocateDescriptorSets(&alloc_info, &dscSet);
    return dscSet;
}

static VkPipelineLayout CreatePipelineLayout(VexDevice* device, VkDescriptorSetLayout dscSetLayout)
{
    // Constants: we are using 'vec2 offset' and 'vec2 scale' instead of a full 3d projection matrix
    VkPushConstantRange push_constants[1] = {};
    push_constants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    push_constants[0].offset = sizeof(float) * 0;
    push_constants[0].size = sizeof(float) * 4;

    VkDescriptorSetLayout set_layout[1] = { dscSetLayout };

    VkPipelineLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.setLayoutCount = 1;
    layout_info.pSetLayouts = set_layout;
    layout_info.pushConstantRangeCount = 1;
    layout_info.pPushConstantRanges = push_constants;
    
    VkPipelineLayout pipelineLayout;
    device->CreatePipelineLayout(&layout_info, &pipelineLayout);
    return pipelineLayout;
}

static VkShaderModule CreateVertexShader(VexDevice* device)
{
    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = sizeof(glsl_shader_vert_spv);
    info.pCode = glsl_shader_vert_spv;

    VkShaderModule shader;
    device->CreateShaderModule(&info, &shader);
    return shader;
}

static VkShaderModule CreateFragShader(VexDevice* device)
{
    VkShaderModuleCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = sizeof(glsl_shader_frag_spv);
    info.pCode = glsl_shader_frag_spv;

    VkShaderModule shader;
    device->CreateShaderModule(&info, &shader);
    return shader;
}

static VkPipeline CreatePipeline(VexDevice* device,
    VkShaderModule vertexShader, 
    VkShaderModule fragShader, 
    VkPipelineLayout pipelineLayout,
    VkRenderPass renderPass, 
    VkSampleCountFlagBits MSAASamples, 
    uint32_t subpass)
{
    VkPipelineShaderStageCreateInfo stage[2] = {};
    stage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stage[0].module = vertexShader;
    stage[0].pName = "main";
    stage[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stage[1].module = fragShader;
    stage[1].pName = "main";

    VkVertexInputBindingDescription binding_desc[1] = {};
    binding_desc[0].stride = sizeof(ImDrawVert);
    binding_desc[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attribute_desc[3] = {};
    attribute_desc[0].location = 0;
    attribute_desc[0].binding = binding_desc[0].binding;
    attribute_desc[0].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_desc[0].offset = IM_OFFSETOF(ImDrawVert, pos);
    attribute_desc[1].location = 1;
    attribute_desc[1].binding = binding_desc[0].binding;
    attribute_desc[1].format = VK_FORMAT_R32G32_SFLOAT;
    attribute_desc[1].offset = IM_OFFSETOF(ImDrawVert, uv);
    attribute_desc[2].location = 2;
    attribute_desc[2].binding = binding_desc[0].binding;
    attribute_desc[2].format = VK_FORMAT_R8G8B8A8_UNORM;
    attribute_desc[2].offset = IM_OFFSETOF(ImDrawVert, col);

    VkPipelineVertexInputStateCreateInfo vertex_info = {};
    vertex_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_info.vertexBindingDescriptionCount = 1;
    vertex_info.pVertexBindingDescriptions = binding_desc;
    vertex_info.vertexAttributeDescriptionCount = 3;
    vertex_info.pVertexAttributeDescriptions = attribute_desc;

    VkPipelineInputAssemblyStateCreateInfo ia_info = {};
    ia_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewport_info = {};
    viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_info.viewportCount = 1;
    viewport_info.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo raster_info = {};
    raster_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_info.polygonMode = VK_POLYGON_MODE_FILL;
    raster_info.cullMode = VK_CULL_MODE_NONE;
    raster_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    raster_info.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo ms_info = {};
    ms_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms_info.rasterizationSamples = (MSAASamples != 0) ? MSAASamples : VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState color_attachment[1] = {};
    color_attachment[0].blendEnable = VK_TRUE;
    color_attachment[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_attachment[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_attachment[0].colorBlendOp = VK_BLEND_OP_ADD;
    color_attachment[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_attachment[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_attachment[0].alphaBlendOp = VK_BLEND_OP_ADD;
    color_attachment[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineDepthStencilStateCreateInfo depth_info = {};
    depth_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    VkPipelineColorBlendStateCreateInfo blend_info = {};
    blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    blend_info.attachmentCount = 1;
    blend_info.pAttachments = color_attachment;

    VkDynamicState dynamic_states[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamic_state = {};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = (uint32_t)IM_ARRAYSIZE(dynamic_states);
    dynamic_state.pDynamicStates = dynamic_states;

    VkGraphicsPipelineCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.flags = 0;
    info.stageCount = 2;
    info.pStages = stage;
    info.pVertexInputState = &vertex_info;
    info.pInputAssemblyState = &ia_info;
    info.pViewportState = &viewport_info;
    info.pRasterizationState = &raster_info;
    info.pMultisampleState = &ms_info;
    info.pDepthStencilState = &depth_info;
    info.pColorBlendState = &blend_info;
    info.pDynamicState = &dynamic_state;
    info.layout = pipelineLayout;
    info.renderPass = renderPass;
    info.subpass = subpass;

    VkPipeline pipeline;
    device->CreateGraphicsPipelines(VK_NULL_HANDLE, 1, &info, &pipeline);
    return pipeline;
}

static void CreateFontImageData(VexDevice* device, VkDeviceMemory* outMemory, VkImage* outImage, VkImageView* outImageView)
{
    ImGuiIO& io = ImGui::GetIO();

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    VkImageCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = VK_FORMAT_R8G8B8A8_UNORM;
    info.extent.width = width;
    info.extent.height = height;
    info.extent.depth = 1;
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkDeviceMemory memory;
    VkMemoryPropertyFlags wantedMemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VkImage image = CreateBackedImage(*device, info, wantedMemoryFlags, 0, &memory);

    VkImageViewCreateInfo imageViewInfo = {};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = image;
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewInfo.subresourceRange.levelCount = 1;
    imageViewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    device->CreateImageView(&imageViewInfo, &imageView);

    *outMemory = memory;
    *outImage = image;
    *outImageView = imageView;
}

static void UploadFontsTexture(VexDevice* device, VkImageView fontImageView, VkImage fontImage, VkSampler fontSampler, VkCommandPool commandPool, VkDescriptorSet dscSet)
{
    ImGuiIO& io = ImGui::GetIO();

    VkCommandBuffer cmdBuffer;
    AllocateCommandBuffer(*device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &cmdBuffer);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    device->BeginCommandBuffer(cmdBuffer, &begin_info);
    
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    size_t upload_size = width * height * 4 * sizeof(char);

    {
        VkDescriptorImageInfo desc_image[1] = {};
        desc_image[0].sampler = fontSampler;
        desc_image[0].imageView = fontImageView;
        desc_image[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkWriteDescriptorSet write_desc[1] = {};
        write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_desc[0].dstSet = dscSet;
        write_desc[0].descriptorCount = 1;
        write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write_desc[0].pImageInfo = desc_image;

        device->UpdateDescriptorSets(1, write_desc, 0, nullptr);
    }

    VkDeviceMemory uploadBufferMemory;
    VkBuffer uploadBuffer;
    {
        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size = upload_size;
        buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        uploadBuffer = CreateBackedBuffer(*device, buffer_info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 0, &uploadBufferMemory);
    }

    // Upload to Buffer:
    {
        char* map = NULL;
        device->MapMemory(uploadBufferMemory, 0, upload_size, 0, (void**)(&map));
        memcpy(map, pixels, upload_size);

        VkMappedMemoryRange range[1] = {};
        range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range[0].memory = uploadBufferMemory;
        range[0].size = upload_size;

        device->FlushMappedMemoryRanges(1, range);

        device->UnmapMemory(uploadBufferMemory);
    }

    // Copy to Image:
    {
        VkImageMemoryBarrier copy_barrier[1] = {};
        copy_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        copy_barrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        copy_barrier[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        copy_barrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        copy_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        copy_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        copy_barrier[0].image = fontImage;
        copy_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copy_barrier[0].subresourceRange.levelCount = 1;
        copy_barrier[0].subresourceRange.layerCount = 1;
        device->CmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, copy_barrier);

        VkBufferImageCopy region = {};
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.layerCount = 1;
        region.imageExtent.width = width;
        region.imageExtent.height = height;
        region.imageExtent.depth = 1;
        device->CmdCopyBufferToImage(cmdBuffer, uploadBuffer, fontImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        VkImageMemoryBarrier use_barrier[1] = {};
        use_barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        use_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        use_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        use_barrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        use_barrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        use_barrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        use_barrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        use_barrier[0].image = fontImage;
        use_barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        use_barrier[0].subresourceRange.levelCount = 1;
        use_barrier[0].subresourceRange.layerCount = 1;
        device->CmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, use_barrier);
    }
    device->EndCommandBuffer(cmdBuffer);

    VkSubmitInfo end_info = {};
    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    end_info.commandBufferCount = 1;
    end_info.pCommandBuffers = &cmdBuffer;
    device->QueueSubmit(device->graphicsQueue, 1, &end_info, VK_NULL_HANDLE);

    device->DeviceWaitIdle();

    device->FreeCommandBuffers(commandPool, 1, &cmdBuffer);
    device->DestroyBuffer(uploadBuffer);
    device->FreeMemory(uploadBufferMemory);
}

static void InitImGuiData(Window* window, VexDevice* device, const VexSwapchainKHR* swapchain, VkDescriptorPool descriptorPool, VkRenderPass renderPass, VkCommandPool commandPool, ImGuiData* imGuiData)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().FontGlobalScale *= 1.5f;
    ImGui_ImplGlfw_InitForVulkan(window->GetGLFWWindow(), true);

    uint32 swapchainImageCount;
    device->GetSwapchainImagesKHR(swapchain->handle, &swapchainImageCount, nullptr);

    VkSampler fontSampler = CreateFontSampler(device);
    VkDescriptorSetLayout dscSetLayout = CreateDescriptorSetLayout(device, fontSampler);
    VkDescriptorSet dscSet = CreateDescriptorSet(device, descriptorPool, dscSetLayout);
    VkPipelineLayout pipelineLayout = CreatePipelineLayout(device, dscSetLayout);
    VkShaderModule vertexShader = CreateVertexShader(device);
    VkShaderModule fragShader = CreateFragShader(device);
    VkPipeline pipeline = CreatePipeline(device, vertexShader, fragShader, pipelineLayout, renderPass, VK_SAMPLE_COUNT_1_BIT, 0);

    VkDeviceMemory fontImageMemory;
    VkImage fontImage;
    VkImageView fontImageView;
    CreateFontImageData(device, &fontImageMemory, &fontImage, &fontImageView);

    UploadFontsTexture(device, fontImageView, fontImage, fontSampler, commandPool, dscSet);

    ImGuiIO& io = ImGui::GetIO();
    // Store our identifier
    io.Fonts->SetTexID((ImTextureID)dscSet);

    imGuiData->descriptorPool = descriptorPool;
    imGuiData->renderPass = renderPass;
    imGuiData->commandPool = commandPool;
    imGuiData->descriptorSetLayout = dscSetLayout;
    imGuiData->descriptorSet = dscSet;
    imGuiData->vertexShader = vertexShader;
    imGuiData->fragShader = fragShader;
    imGuiData->pipelineLayout = pipelineLayout;
    imGuiData->pipeline = pipeline;
    imGuiData->imageCount = swapchainImageCount;
    imGuiData->fontSampler = fontSampler;
    imGuiData->fontImageMemory = fontImageMemory;
    imGuiData->fontImage = fontImage;
    imGuiData->fontImageView = fontImageView;
}

static void DestroyImGuiData(VexDevice* device, ImGuiData* data)
{
    device->FreeDescriptorSets(data->descriptorPool, 1, &data->descriptorSet);

    for (auto& kv : data->imageDscSets)
    {
        device->FreeDescriptorSets(data->descriptorPool, 1, &kv.second);
    }

    device->DestroyPipeline(data->pipeline);
    device->DestroyPipelineLayout(data->pipelineLayout);
    device->DestroyDescriptorSetLayout(data->descriptorSetLayout);
    device->DestroySampler(data->fontSampler);
    device->DestroyShaderModule(data->vertexShader);
    device->DestroyShaderModule(data->fragShader);

    device->DestroyBuffer(data->vertexBuffer);
    device->FreeMemory(data->vertexBufferMemory);

    device->DestroyBuffer(data->indexBuffer);
    device->FreeMemory(data->indexBufferMemory);

    device->DestroyImageView(data->fontImageView);
    device->DestroyImage(data->fontImage);
    device->FreeMemory(data->fontImageMemory);

    for (uint32 i = 0; i < data->windowRenderBuffers.Count; ++i)
    {
        FrameRenderBuffers* frameRenderBuffers = &data->windowRenderBuffers.FrameRenderBuffers[i];

        device->DestroyBuffer(frameRenderBuffers->VertexBuffer);
        device->FreeMemory(frameRenderBuffers->VertexBufferMemory);

        device->DestroyBuffer(frameRenderBuffers->IndexBuffer);
        device->FreeMemory(frameRenderBuffers->IndexBufferMemory);
    }
}

static void SetupRenderState(VexDevice* device, ImDrawData* draw_data, const FrameRenderBuffers* rb, int fb_width, int fb_height, VkPipelineLayout pipelineLayout, VkPipeline pipeline, VkCommandBuffer command_buffer)
{
    // Bind pipeline:
    device->CmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    // Bind Vertex And Index Buffer:
    if (draw_data->TotalVtxCount > 0)
    {
        VkBuffer vertex_buffers[1] = { rb->VertexBuffer };
        VkDeviceSize vertex_offset[1] = { 0 };
        device->CmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, vertex_offset);
        device->CmdBindIndexBuffer(command_buffer, rb->IndexBuffer, 0, sizeof(ImDrawIdx) == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32);
    }

    // Setup viewport:
    {
        VkViewport viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = (float)fb_width;
        viewport.height = (float)fb_height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        device->CmdSetViewport(command_buffer, 0, 1, &viewport);
    }

    // Setup scale and translation:
    // Our visible imgui space lies from draw_data->DisplayPps (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
    {
        float scale[2];
        scale[0] = 2.0f / draw_data->DisplaySize.x;
        scale[1] = 2.0f / draw_data->DisplaySize.y;
        float translate[2];
        translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
        translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];
        device->CmdPushConstants(command_buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(float) * 0, sizeof(float) * 2, scale);
        device->CmdPushConstants(command_buffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(float) * 2, sizeof(float) * 2, translate);
    }
}

static void CreateHostVisibleBuffer(VexDevice* device, VkDeviceSize newSize, VkBufferUsageFlagBits usage, VkBuffer* outBuffer, VkDeviceMemory* outBufferMemory, VkDeviceSize* outBufferSize)
{
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = newSize;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkMemoryRequirements memoryRequirements;
    VkDeviceMemory memory;
    VkBuffer buffer = CreateBackedBuffer(*device, buffer_info, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 0, &memory, &memoryRequirements);

    *outBuffer = buffer;
    *outBufferMemory = memory;
    *outBufferSize = memoryRequirements.size;
}

static void RenderDrawData(VexDevice* device, ImDrawData* draw_data, ImGuiData* data, VkCommandBuffer command_buffer)
{
    WindowRenderBuffers* wrb = &data->windowRenderBuffers;
    VkDescriptorSet descriptorSet = data->descriptorSet;
    VkPipelineLayout pipelineLayout = data->pipelineLayout;
    VkPipeline pipeline = data->pipeline; 
    uint32 imageCount = data->imageCount;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    // Allocate array to store enough vertex/index buffers
    if (wrb->FrameRenderBuffers == NULL)
    {
        wrb->Index = 0;
        wrb->Count = imageCount;
        wrb->FrameRenderBuffers = (FrameRenderBuffers*)IM_ALLOC(sizeof(FrameRenderBuffers) * wrb->Count);
        memset(wrb->FrameRenderBuffers, 0, sizeof(FrameRenderBuffers) * wrb->Count);
    }
    IM_ASSERT(wrb->Count == imageCount);
    wrb->Index = (wrb->Index + 1) % wrb->Count;
    FrameRenderBuffers* rb = &wrb->FrameRenderBuffers[wrb->Index];

    if (draw_data->TotalVtxCount > 0)
    {
        // Create or resize the vertex/index buffers
        size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
        size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);
        if (rb->VertexBuffer == VK_NULL_HANDLE || rb->VertexBufferSize < vertex_size)
        {
            device->DestroyBuffer(rb->VertexBuffer);
            device->FreeMemory(rb->VertexBufferMemory);

            CreateHostVisibleBuffer(device, vertex_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &rb->VertexBuffer, &rb->VertexBufferMemory, &rb->VertexBufferSize);
        }
        if (rb->IndexBuffer == VK_NULL_HANDLE || rb->IndexBufferSize < index_size)
        {
            device->DestroyBuffer(rb->IndexBuffer);
            device->FreeMemory(rb->IndexBufferMemory);

            CreateHostVisibleBuffer(device, vertex_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &rb->IndexBuffer, &rb->IndexBufferMemory, &rb->IndexBufferSize);
        }

        // Upload vertex/index data into a single contiguous GPU buffer
        ImDrawVert* vtx_dst = NULL;
        ImDrawIdx* idx_dst = NULL;
        VkResult err = device->MapMemory(rb->VertexBufferMemory, 0, rb->VertexBufferSize, 0, (void**)(&vtx_dst));
        CheckVkResult(err);
        err = device->MapMemory(rb->IndexBufferMemory, 0, rb->IndexBufferSize, 0, (void**)(&idx_dst));
        CheckVkResult(err);
        for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];
            memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
            vtx_dst += cmd_list->VtxBuffer.Size;
            idx_dst += cmd_list->IdxBuffer.Size;
        }
        VkMappedMemoryRange range[2] = {};
        range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range[0].memory = rb->VertexBufferMemory;
        range[0].size = VK_WHOLE_SIZE;
        range[1].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        range[1].memory = rb->IndexBufferMemory;
        range[1].size = VK_WHOLE_SIZE;
        err = device->FlushMappedMemoryRanges(2, range);
        CheckVkResult(err);
        device->UnmapMemory(rb->VertexBufferMemory);
        device->UnmapMemory(rb->IndexBufferMemory);
    }

    // Setup desired Vulkan state
    SetupRenderState(device, draw_data, rb, fb_width, fb_height, pipelineLayout, pipeline, command_buffer);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    // (Because we merged all buffers into a single one, we maintain our own offset into them)
    int global_vtx_offset = 0;
    int global_idx_offset = 0;
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL)
            {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                {
                    SetupRenderState(device, draw_data, rb, fb_width, fb_height, pipelineLayout, pipeline, command_buffer);
                }
                else
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
            }
            else
            {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                // Clamp to viewport as vkCmdSetScissor() won't accept values that are off bounds
                if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
                if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
                if (clip_max.x > fb_width) { clip_max.x = (float)fb_width; }
                if (clip_max.y > fb_height) { clip_max.y = (float)fb_height; }
                if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    continue;

                // Apply scissor/clipping rectangle
                VkRect2D scissor;
                scissor.offset.x = (int32_t)(clip_min.x);
                scissor.offset.y = (int32_t)(clip_min.y);
                scissor.extent.width = (uint32_t)(clip_max.x - clip_min.x);
                scissor.extent.height = (uint32_t)(clip_max.y - clip_min.y);
                device->CmdSetScissor(command_buffer, 0, 1, &scissor);

                // Bind DescriptorSet with font or user texture
                VkDescriptorSet desc_set[1] = { (VkDescriptorSet)pcmd->TextureId };
                if (sizeof(ImTextureID) < sizeof(ImU64))
                {
                    // We don't support texture switches if ImTextureID hasn't been redefined to be 64-bit. Do a flaky check that other textures haven't been used.
                    IM_ASSERT(pcmd->TextureId == (ImTextureID)descriptorSet);
                    desc_set[0] = descriptorSet;
                }
                device->CmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, desc_set, 0, NULL);

                // Draw
                device->CmdDrawIndexed(command_buffer, pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
            }
        }
        global_idx_offset += cmd_list->IdxBuffer.Size;
        global_vtx_offset += cmd_list->VtxBuffer.Size;
    }

    // Note: at this point both vkCmdSetViewport() and vkCmdSetScissor() have been called.
    // Our last values will leak into user/application rendering IF:
    // - Your app uses a pipeline with VK_DYNAMIC_STATE_VIEWPORT or VK_DYNAMIC_STATE_SCISSOR dynamic state
    // - And you forgot to call vkCmdSetViewport() and vkCmdSetScissor() yourself to explicitely set that state.
    // If you use VK_DYNAMIC_STATE_VIEWPORT or VK_DYNAMIC_STATE_SCISSOR you are responsible for setting the values before rendering.
    // In theory we should aim to backup/restore those values but I am not sure this is possible.
    // We perform a call to vkCmdSetScissor() to set back a full viewport which is likely to fix things for 99% users but technically this is not perfect. (See github #4644)
    VkRect2D scissor = { { 0, 0 }, { (uint32_t)fb_width, (uint32_t)fb_height } };
    device->CmdSetScissor(command_buffer, 0, 1, &scissor);
}

static VkDescriptorSet CreateImageDescriptorSet(VexDevice* device, VkSampler sampler, VkDescriptorPool pool, VkDescriptorSetLayout layout, VkImageView imageView, VkImageLayout imageLayout)
{
    // Create Descriptor Set:
    VkDescriptorSet descriptor_set;
    {
        VkDescriptorSetAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = pool;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &layout;

        VkResult err = device->AllocateDescriptorSets(&alloc_info, &descriptor_set);
        CheckVkResult(err);
    }

    // Update the Descriptor Set:
    {
        VkDescriptorImageInfo desc_image[1] = {};
        desc_image[0].sampler = sampler;
        desc_image[0].imageView = imageView;
        desc_image[0].imageLayout = imageLayout;
        VkWriteDescriptorSet write_desc[1] = {};
        write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write_desc[0].dstSet = descriptor_set;
        write_desc[0].descriptorCount = 1;
        write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write_desc[0].pImageInfo = desc_image;

        device->UpdateDescriptorSets(1, write_desc, 0, NULL);
    }
    return descriptor_set;
}

void GPUVulkan::ImGuiInit()
{
    AssertNotNull();

    VexDevice& device = GetVexDevice();
    Window& window = GetWindow();
    const VexSwapchainKHR* swapchain = &CastVulkan(GetSwapchain())->GetVexSwapchain();
    VkDescriptorPool descriptorPool = GetDescriptorPool();
    VkRenderPass renderPass = CastVulkan(GetPresentRenderPass()).GetVexRenderPass().handle;
    VkCommandPool commandPool = GetCommandPool();

    InitImGuiData(&window, &device, swapchain, descriptorPool, renderPass, commandPool, &g_imGuiData);
}

void GPUVulkan::ImGuiNewFrame()
{
    AssertNotNull();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GPUVulkan::ImGuiImage(const GPUImageView* imageView,
    float2 size,
    float2 uv0,
    float2 uv1,
    float4 tint_col,
    float4 border_col)
{
    AssertNotNull();

    VexDevice& device = GetVexDevice();

    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

    if (HashMapHasValue(g_imGuiData.imageDscSets, imageView))
    {
        descriptorSet = g_imGuiData.imageDscSets.at(imageView);
    }
    else
    {
        VkImageView imageViewHandle = CastVulkan(*imageView).GetVexView().handle;
        descriptorSet = CreateImageDescriptorSet(&device, g_imGuiData.fontSampler, GetDescriptorPool(), g_imGuiData.descriptorSetLayout, imageViewHandle, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        g_imGuiData.imageDscSets[imageView] = descriptorSet;
    }

    ImGui::Image(descriptorSet, 
        ImVec2(size.x, size.y), 
        ImVec2(uv0.x, uv0.y), 
        ImVec2(uv1.x, uv1.y), 
        ImVec4(tint_col.x, tint_col.y, tint_col.z, tint_col.w), 
        ImVec4(border_col.x, border_col.y, border_col.z, border_col.w));
}

void GPUVulkan::ImGuiRecordRendering(VkCommandBuffer cmdBuffer)
{
    AssertNotNull();

    VexDevice& device = GetVexDevice();

    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();

    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

    if (!is_minimized)
        RenderDrawData(&device, draw_data, &g_imGuiData, cmdBuffer);
}

void GPUVulkan::ImGUIUninit()
{
    AssertNotNull();

    VexDevice* device = &GetVexDevice();

    Flush();

    DestroyImGuiData(device, &g_imGuiData);
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}