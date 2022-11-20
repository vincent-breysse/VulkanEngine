
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/render_pass.hpp>
#include <gyn/vulkan/framebuffer.hpp>
#include <gyn/vulkan/image.hpp>
#include <gyn/vulkan/swapchain.hpp>

static void AssertValidInputs(GPUVulkan&, const GPURenderPass* renderPass, ea::span<const GPUImageView*> attachments)
{
    if constexpr (EnableAsserts)
    {
        ASSERT_MSG(renderPass,
            "GPURenderPass must not be null");
        ASSERT_MSG(attachments.size() > 0,
            "A Framebuffer must come with at least one color attachment");

        for (const auto& attachment : attachments)
        {
            ASSERT_MSG(attachment,
                "Attachment must not be null");
        }

        const uint2 extent = attachments[0]->GetExtent();

        for (const auto& i : attachments)
        {
            const uint2 e = i->GetExtent();
            ASSERT_MSG(All(e == extent),
                "Every attachments must have the same extent");
        }

        ASSERT_MSG(renderPass->GetAttachments().size() == attachments.size(),
            "The number of attachments provied to the Framebuffer must be equal to the number of attachments announced in the GPURenderPass");

        for (int i = 0; i < attachments.size(); ++i)
        {
            auto format0 = renderPass->GetAttachments()[i];
            auto format1 = attachments[i]->GetImage().GetFormat();

            ASSERT_MSG(format0 == format1,
                "For each RenderTexture passed when creating a Framebuffer, the GPUImage[i] must match the GPUFormat[i] announced in the GPURenderPass");
        }
    }
}

static ea::vector<VkClearValue> CreateClearValues(ea::span<const GPUImageView*> attachments)
{
    VkClearValue colorClearValues = {};
    colorClearValues.color.float32[3] = 1;

    VkClearValue depthStencilClearValues = {};
    depthStencilClearValues.depthStencil.depth = 1.f;

    ea::vector<VkClearValue> clearValues{};
    clearValues.resize(attachments.size());

    for (int i = 0; i < attachments.size(); ++i)
    {
        if (IsDepthFormat(attachments[i]->GetImage().GetFormat()))
        {
            clearValues[i] = depthStencilClearValues;
        }
        else
        {
            clearValues[i] = colorClearValues;
        }
    }

    return clearValues;
}

static VkViewport CreateViewport(VkExtent2D extent)
{
    VkViewport viewport = {};
    viewport.width = float(extent.width);
    viewport.height = float(extent.height);
    viewport.maxDepth = 1.0f;
    return viewport;
}

static VkRect2D CreateRenderArea(VkExtent2D extent)
{
    VkRect2D ret = {};
    ret.extent = extent;
    return ret;
}

static VkFramebuffer CreateFramebuffer(VexDevice& device, VkRenderPass renderPass, VkExtent2D extent, ea::span<const GPUImageView* const> attachments)
{
    std::vector<VkImageView> vkAttachments = {};
    vkAttachments.resize(attachments.size());

    for (usize i = 0; i < attachments.size(); ++i)
    {
        vkAttachments[i] = CastVulkan(*attachments[i]).GetHandle();
    }

    VkFramebufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    info.renderPass = renderPass;
    info.attachmentCount = ToU32(vkAttachments.size());
    info.pAttachments = vkAttachments.data();
    info.width = extent.width;
    info.height = extent.height;
    info.layers = 1;

    VkFramebuffer framebuffer;
    device.CreateFramebuffer(&info, &framebuffer);
    return framebuffer;
}

GPUVulkanFramebuffer::GPUVulkanFramebuffer()
{
    m_gpu = nullptr;
    m_renderPassLayout = nullptr;
    m_viewport = {};
    m_renderArea = {};
    m_extent = {};
    m_framebuffer = nullptr;
}

GPUVulkanFramebuffer::GPUVulkanFramebuffer(GPUVulkan& gpu, const GPUVulkanRenderPass* renderPassLayout, ea::vector<const GPUImageView*>&& attachments) : GPUVulkanFramebuffer()
{
    AssertValidInputs(gpu, renderPassLayout, attachments);

    VexDevice& device = gpu.GetVexDevice();
    VkExtent2D extent = ToVkExtent2D(attachments[0]->GetExtent()); // Attachments are assumed to all have the same extent
    VkRect2D renderArea = CreateRenderArea(extent);
    VkViewport viewport = CreateViewport(extent);
    ea::vector<VkClearValue> clearValues = CreateClearValues(attachments);
    VkFramebuffer framebuffer = CreateFramebuffer(device, renderPassLayout->GetHandle(), extent, attachments);

    m_gpu = &gpu;
    m_framebuffer = &device.GetVexFramebuffer(framebuffer);
    m_renderArea = renderArea;
    m_viewport = viewport;
    m_extent = extent;
    m_attachments = std::move(attachments);
    m_renderPassLayout = renderPassLayout;
    m_clearValues = std::move(clearValues);
}

GPUVulkanFramebuffer::~GPUVulkanFramebuffer()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();
    device.DestroyFramebuffer(m_framebuffer->handle);
}

bool GPUVulkanFramebuffer::IsNull() const
{
    return m_gpu == nullptr;
}

GPUBackend GPUVulkanFramebuffer::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanFramebuffer::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

const GPURenderPass& GPUVulkanFramebuffer::GetRenderPass() const
{
    AssertNotNull();
    return *m_renderPassLayout;
}

ea::span<const GPUImageView* const> GPUVulkanFramebuffer::GetAttachments() const
{
    AssertNotNull();
    return m_attachments;
}

const VexFramebuffer& GPUVulkanFramebuffer::GetVexFramebuffer() const
{
    AssertNotNull();
    return *m_framebuffer;
}

VkFramebuffer GPUVulkanFramebuffer::GetHandle() const
{
    AssertNotNull();
    return m_framebuffer->handle;
}

const VkViewport& GPUVulkanFramebuffer::GetViewport() const
{
    AssertNotNull();
    return m_viewport;
}

const VkRect2D& GPUVulkanFramebuffer::GetRenderArea() const
{
    AssertNotNull();
    return m_renderArea;
}

ea::span<const VkClearValue> GPUVulkanFramebuffer::GetClearValues() const
{
    AssertNotNull();
    return m_clearValues;
}

void GPUVulkanFramebuffer::AssertNotNull() const
{
    ASSERT(!IsNull());
}