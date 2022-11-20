#pragma once

#include <gyn/common.hpp>
#include <gyn/gpu.hpp>
#include <gyn/vulkan/fwd.hpp>

namespace Gyn
{
    struct ShaderData
    {
        ea::vector<VkDescriptorSetLayout> dscLayouts = {};
        VkShaderModule vertex = {};
        VkShaderModule frag = {};
        VkPipelineLayout pipelineLayout = {};
    };
}