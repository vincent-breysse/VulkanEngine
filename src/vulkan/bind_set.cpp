
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/shader.hpp>
#include <gyn/vulkan/bind_set.hpp>
#include <gyn/vulkan/buffer.hpp>
#include <gyn/vulkan/image.hpp>

static VkDescriptorType ToVkDescriptorType(GPUBufferUsage usage)
{
    ASSERT(usage == GPUBufferUsage::eSSBO || usage == GPUBufferUsage::eUniformBuffer);

    if (usage == GPUBufferUsage::eSSBO)
    {
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }
    else if (usage == GPUBufferUsage::eUniformBuffer)
    {
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }

    ASSERT_MSG(false, "Invalid GPUBufferUsage");
    return VK_DESCRIPTOR_TYPE_MAX_ENUM;
}

GPUVulkanBindSet::GPUVulkanBindSet()
{
    m_gpu = nullptr;
    m_shader = nullptr;
    m_setIndex = ~0u;
    m_dscSet = nullptr;
}

GPUVulkanBindSet::GPUVulkanBindSet(GPUVulkan& gpu, const GPUVulkanShader& shader, uint32 setIndex) : GPUVulkanBindSet()
{
    VexDevice& device = gpu.GetVexDevice();

    VkDescriptorSet dscSet;
    AllocateDescriptorSet(device, gpu.GetDescriptorPool(), shader.GetDescriptorSetLayout(setIndex), &dscSet);

    m_gpu = &gpu;
    m_shader = &shader;
    m_setIndex = setIndex;
    m_dscSet = &device.GetVexDescriptorSet(dscSet);
}

GPUVulkanBindSet::~GPUVulkanBindSet()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();
    device.FreeDescriptorSets(m_dscSet->descriptorPool, 1, &m_dscSet->handle);
}

bool GPUVulkanBindSet::IsNull() const
{
    return !m_dscSet;
}

GPUBackend GPUVulkanBindSet::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanBindSet::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

const GPUShader& GPUVulkanBindSet::GetShader()
{
    AssertNotNull();
    return *m_shader;
}

uint32 GPUVulkanBindSet::GetSetIndex() const
{
    AssertNotNull();
    return m_setIndex;
}

void GPUVulkanBindSet::SetBinding(const GPUImageView* imageView, uint32 binding, GPUSamplerFiltering filtering, GPUSamplerAddressing addressing)
{
    AssertNotNull();
    ASSERT(imageView);

    VexDevice& device = m_gpu->GetVexDevice();

    VkDescriptorImageInfo imageInfo = {};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = CastVulkan(*imageView).GetHandle();
    imageInfo.sampler = m_gpu->GetSampler(filtering, addressing);

    VkWriteDescriptorSet info = {};
    info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    info.dstBinding = binding;
    info.descriptorCount = 1;
    info.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    info.pImageInfo = &imageInfo;
    info.dstSet = m_dscSet->handle;

    device.UpdateDescriptorSets(1, &info, 0, nullptr);
}

void GPUVulkanBindSet::SetBinding(const GPUBuffer* buffer, uint32 binding)
{
    AssertNotNull();
    ASSERT(buffer);
    const GPUBufferUsage bufferUsage = buffer->GetUsage();
    ASSERT_MSG(bufferUsage == GPUBufferUsage::eUniformBuffer || bufferUsage == GPUBufferUsage::eSSBO,
        "A GPUBuffer must be either an UniformBuffer or a SSBO to be bound to a GPUBindSet");

    VexDevice& device = m_gpu->GetVexDevice();

    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = CastVulkan(*buffer).GetHandle();
    bufferInfo.range = VK_WHOLE_SIZE;

    VkWriteDescriptorSet info = {};
    info.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    info.dstBinding = binding;
    info.descriptorCount = 1;
    info.descriptorType = ToVkDescriptorType(buffer->GetUsage());
    info.pBufferInfo = &bufferInfo;
    info.dstSet = m_dscSet->handle;

    device.UpdateDescriptorSets(1, &info, 0, nullptr);
}

const VexDescriptorSet& GPUVulkanBindSet::GetVexDescriptorSet() const
{
    AssertNotNull();
    return *m_dscSet;
}

VkDescriptorSet GPUVulkanBindSet::GetHandle() const
{
    AssertNotNull();
    return m_dscSet->handle;
}

void GPUVulkanBindSet::AssertNotNull() const
{
    ASSERT(!IsNull());
}