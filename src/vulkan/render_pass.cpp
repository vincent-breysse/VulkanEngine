
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/vulkan/render_pass.hpp>

namespace
{
    enum AttachmentState
    {
        eNone,
        eRead = 1,
        eWrite = 1 << 1
    };
    ENUM_CLASS_FLAG_MANIPS(AttachmentState)
}

static GPURenderPassID CreateUniqueRenderPassID()
{
    static std::atomic<uint64> id = 1;
    return GPURenderPassID(id++);
}

static void AssertValidInputs(GPUVulkan&, ea::span<const GPUFormat> attachments, ea::span<const SubpassLayout> subpasses, ea::span<const uint32> multiviewMasks)
{
    if constexpr (EnableAsserts)
    {
        ASSERT_MSG(attachments.size() > 0,
            "The number of color attachments must be more than 0");
        ASSERT_MSG(subpasses.size() > 0,
            "The number of subpass must be more than 0");

        if (!multiviewMasks.empty())
        {
            ASSERT(multiviewMasks.size() == subpasses.size());
        }

        for (const GPUFormat format : attachments)
        {
            ASSERT_MSG(format != GPUFormat::eNull,
                "Attachment format must not be null");
        }

        ASSERT_MSG(subpasses[0].supassInputRefs.size() == 0,
            "SubpassLayout 0 can't contain any SubpassInput");

        ea::vector<uint32> ids{};
        for (const auto& subpass : subpasses)
        {
            ids.clear();
            ids.reserve(subpass.colorBufferRefs.size());
            for (const uint32 colorRef : subpass.colorBufferRefs)
            {
                ASSERT_MSG(colorRef != NullSubpassRef,
                    "Color buffer references must not be null");

                ASSERT_MSG(colorRef < attachments.size(),
                    "Color buffer reference is out of bounds");

                ASSERT_MSG(IsColorFormat(attachments[colorRef]),
                    "A ColorBufferRef must reference an attachment with a color GPUFormat");

                const auto end = ids.end();
                const auto it = ea::find(ids.begin(), end, colorRef);
                ASSERT_MSG(it == end,
                    "SubpassLayout::colorBufferRefs array must not contain more than one reference to the same attachment");

                ids.push_back(colorRef);
            }

            ids.clear();
            ids.reserve(subpass.supassInputRefs.size());
            for (const uint32 inputRef : subpass.supassInputRefs)
            {
                ASSERT_MSG(inputRef != NullSubpassRef,
                    "SubpassLayout Input references must not be null");

                ASSERT_MSG(inputRef < attachments.size(),
                    "SubpassLayout Input reference is out of bounds");

                const auto end = ids.end();
                const auto it = ea::find(ids.begin(), end, inputRef);
                ASSERT_MSG(it == end,
                    "SubpassLayout::supassInputRefs array must not contain more than one reference to the same attachment");

                ids.push_back(inputRef);
            }

            const uint32 depthStencilRef = subpass.depthStencilRef;

            if (depthStencilRef != NullSubpassRef)
            {
                ASSERT_MSG(IsDepthFormat(attachments[depthStencilRef]),
                    "If SubpassLayout::depthStencilRef != NullSubpassRef, it must reference an attachment with a depth-stencil format");

                ASSERT_MSG(depthStencilRef < attachments.size(),
                    "DepthStencil buffer reference is out of bounds");
            }

            const bool isSorted = ea::is_sorted(subpass.colorBufferRefs.begin(), subpass.colorBufferRefs.end());

            ASSERT_MSG(isSorted,
                "Each attachment array in each SubpassLayout must be sorted from the lowest to the highest attachment ID");
        }
    }
}

static ea::vector<VkAttachmentDescription> CreateVkAttachments(GPUVulkan& gpu, ea::span<const GPUFormat> attachments)
{
    usize attachmentCount = attachments.size();

    ea::vector<VkAttachmentDescription> vkAttachments{};
    vkAttachments.resize(attachmentCount);

    for (usize i = 0; i < attachmentCount; ++i)
    {
        GPUFormat format = attachments[i];
        VkFormat vkFormat = gpu.ToVkFormat(format);
        VkImageLayout layout = format == GPUFormat::ePresent ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentDescription& vkAttachment = vkAttachments[i];
        vkAttachment.format = vkFormat;
        vkAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        vkAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        vkAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        vkAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        vkAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        vkAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        vkAttachment.finalLayout = layout;
    }

    return vkAttachments;
}

static ea::vector<VkAttachmentReference> CreateVkColorBufferReferences(ea::span<const uint32> colorBufferRefs)
{
    ea::vector<VkAttachmentReference> vkColorRefs;
    vkColorRefs.resize(colorBufferRefs.size());

    for (usize i = 0; i < colorBufferRefs.size(); ++i)
    {
        uint32 ref = colorBufferRefs[i];

        vkColorRefs[i].attachment = ref;
        vkColorRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    return vkColorRefs;
}

static ea::vector<VkAttachmentReference> CreateVkSubpassInputReferences(ea::span<const uint32> inputRefs)
{
    ea::vector<VkAttachmentReference> vkInputRefs;
    vkInputRefs.resize(inputRefs.size());

    for (int i = 0; i < inputRefs.size(); ++i)
    {
        uint32 ref = inputRefs[i];

        vkInputRefs[i].attachment = ref;
        vkInputRefs[i].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    return vkInputRefs;
}

static VkAttachmentReference CreateVkDepthStencilRef(uint32 depthStencilRef)
{
    if (depthStencilRef == NullSubpassRef)
        return {};

    VkAttachmentReference vkRef = {};
    vkRef.attachment = depthStencilRef;
    vkRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    return vkRef;
}

static VkSubpassDescription CreateVkSubpass(ea::span<const VkAttachmentReference> vkColorRefs, ea::span<const VkAttachmentReference> vkInputRefs, const VkAttachmentReference* vkDepthStencilRef)
{
    VkSubpassDescription vkSubpass = {};
    vkSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    vkSubpass.colorAttachmentCount = ToU32(vkColorRefs.size());
    vkSubpass.pColorAttachments = vkColorRefs.data();
    vkSubpass.inputAttachmentCount = ToU32(vkInputRefs.size());
    vkSubpass.pInputAttachments = vkInputRefs.data();
    vkSubpass.pDepthStencilAttachment = vkDepthStencilRef;

    return vkSubpass;
}



static void OutputAttachmentState(uint32 attachmentCount, const SubpassLayout& subpass, ea::vector<AttachmentState>& out)
{
    out.resize(attachmentCount, AttachmentState::eNone);
    memset(out.data(), AttachmentState::eNone, out.size() * sizeof(out[0]));

    for (uint32 r : subpass.colorBufferRefs)
    {
        out[r] |= AttachmentState::eWrite;
    }

    if (subpass.depthStencilRef != NullSubpassRef)
    {
        out[subpass.depthStencilRef] |= AttachmentState::eWrite;
    }

    for (uint32 r : subpass.supassInputRefs)
    {
        out[r] |= AttachmentState::eRead;
    }
}

static ea::vector<VkSubpassDependency> CreateSubpassDependencies(ea::span<const GPUFormat> attachments, ea::span<const SubpassLayout> subpasses)
{
    uint32 attachmentCount = ToU32(attachments.size());
    uint32 subpassCount = ToU32(subpasses.size());

    ea::vector<VkSubpassDependency> vkDependencies{};
    vkDependencies.reserve(subpassCount);

    VkPipelineStageFlags colorWriteStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkAccessFlags colorWriteAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkPipelineStageFlags dsWriteStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    VkAccessFlags dsWriteAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkPipelineStageFlags readStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    VkAccessFlags readAccessMask = VK_ACCESS_SHADER_READ_BIT;

    ea::vector<AttachmentState> prevStates;

    for (uint d = 1; d < subpassCount; ++d)
    {
        uint s = d - 1;
        OutputAttachmentState(attachmentCount, subpasses[s], prevStates);

        const SubpassLayout& dst = subpasses[d];

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = s;
        dependency.dstSubpass = d;

        for (uint32 r : dst.colorBufferRefs)
        {
            // Color attachment WAW
            if (prevStates[r] & AttachmentState::eWrite)
            {
                dependency.srcStageMask |= colorWriteStageMask;
                dependency.srcAccessMask |= colorWriteAccessMask;

                dependency.dstStageMask |= colorWriteStageMask;
                dependency.dstAccessMask |= colorWriteAccessMask;
            }
            // Color attachment WAR
            if (prevStates[r] & AttachmentState::eRead)
            {
                dependency.srcStageMask |= readStageMask;
                dependency.srcAccessMask |= readAccessMask;

                dependency.dstStageMask |= colorWriteStageMask;
                dependency.dstAccessMask |= colorWriteAccessMask;
            }
        }

        if (dst.depthStencilRef != NullSubpassRef)
        {
            uint32 r = dst.depthStencilRef;

            // Depth-Stencil attachment WAW
            if (prevStates[r] & AttachmentState::eWrite)
            {
                dependency.srcStageMask |= dsWriteStageMask;
                dependency.srcAccessMask |= dsWriteAccessMask;

                dependency.dstStageMask |= dsWriteStageMask;
                dependency.dstAccessMask |= dsWriteAccessMask;
            }
            // Depth-Stencil attachment WAR
            if (prevStates[r] & AttachmentState::eRead)
            {
                dependency.srcStageMask |= readStageMask;
                dependency.srcAccessMask |= readAccessMask;

                dependency.dstStageMask |= dsWriteStageMask;
                dependency.dstAccessMask |= dsWriteAccessMask;
            }
        }

        for (uint32 r : dst.supassInputRefs)
        {
            // RAW
            if (prevStates[r] & AttachmentState::eWrite)
            {
                // Color attachment RAW
                if (IsColorFormat(attachments[r]))
                {
                    dependency.srcStageMask |= colorWriteStageMask;
                    dependency.srcAccessMask |= colorWriteAccessMask;
                }
                // Depth attachment RAW
                else
                {
                    ASSERT(IsDepthFormat(attachments[r]));

                    dependency.srcStageMask |= dsWriteStageMask;
                    dependency.srcAccessMask |= dsWriteAccessMask;
                }

                dependency.dstStageMask |= readStageMask;
                dependency.dstAccessMask |= readAccessMask;
            }
            // Nothing to do (RAR)
            if (prevStates[r] & AttachmentState::eRead)
            {

            }
        }

        bool srcAccessMaskSet = dependency.srcAccessMask != 0;
        bool srcStageMaskSet = dependency.srcStageMask != 0; MaybeUnused(srcStageMaskSet);

        bool dstAccessMaskSet = dependency.dstAccessMask != 0; MaybeUnused(dstAccessMaskSet);
        bool dstStageMaskSet = dependency.dstStageMask != 0; MaybeUnused(dstStageMaskSet);

        // Assert that masks are either all true, or all false
        ASSERT(srcAccessMaskSet == dstAccessMaskSet);
        ASSERT(srcStageMaskSet == dstStageMaskSet);

        // Since all masks are either all true or all false, we can assume the dependency is empty if one is false
        const bool isDependencyEmpty = !srcAccessMaskSet;
        if (isDependencyEmpty)
            continue;

        vkDependencies.push_back(dependency);
    }

    return vkDependencies;
}

static VkRenderPass CreateRenderPass(GPUVulkan& gpu, ea::span<const GPUFormat> attachments, ea::span<const SubpassLayout> subpasses, ea::span<const uint32> multiviewMasks)
{
    VexDevice& device = gpu.GetVexDevice();
    usize subpassCount = subpasses.size();

    ea::vector<VkAttachmentDescription> vkAttachments = CreateVkAttachments(gpu, attachments);

    ea::vector<ea::vector<VkAttachmentReference>> vkSubpassColorRefs = {};
    ea::vector<ea::vector<VkAttachmentReference>> vkSubpassInputRefs = {};
    ea::vector<VkAttachmentReference> vkSubpassDepthStencilRefs = {};

    vkSubpassColorRefs.resize(subpassCount);
    vkSubpassInputRefs.resize(subpassCount);
    vkSubpassDepthStencilRefs.resize(subpassCount);

    for (usize i = 0; i < subpassCount; ++i)
    {
        const SubpassLayout& subpass = subpasses[i];

        vkSubpassColorRefs[i] = CreateVkColorBufferReferences(subpass.colorBufferRefs);
        vkSubpassInputRefs[i] = CreateVkSubpassInputReferences(subpass.supassInputRefs);
        vkSubpassDepthStencilRefs[i] = CreateVkDepthStencilRef(subpass.depthStencilRef);
    }

    ea::vector<VkSubpassDescription> vkSubpasses{};
    vkSubpasses.resize(subpassCount);

    for (usize i = 0; i < vkSubpasses.size(); ++i)
    {
        ea::vector<VkAttachmentReference>& vkColorRefs = vkSubpassColorRefs[i];
        ea::vector<VkAttachmentReference>& vkInputRefs = vkSubpassInputRefs[i];
        uint32 depthStencilRef = subpasses[i].depthStencilRef;
        const VkAttachmentReference* vkDepthStencilRef = depthStencilRef != NullSubpassRef ? &vkSubpassDepthStencilRefs[i] : nullptr;

        vkSubpasses[i] = CreateVkSubpass(vkColorRefs, vkInputRefs, vkDepthStencilRef);
    }

    ea::vector<VkSubpassDependency> vkDependencies = CreateSubpassDependencies(attachments, subpasses);

    VkRenderPassMultiviewCreateInfo multiviewInfo = {};
    multiviewInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
    multiviewInfo.subpassCount = ToU32(subpassCount);
    multiviewInfo.pViewMasks = multiviewMasks.data();

    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = multiviewMasks.empty() ? nullptr : &multiviewInfo;
    info.attachmentCount = ToU32(vkAttachments.size());
    info.pAttachments = vkAttachments.data();
    info.subpassCount = ToU32(vkSubpasses.size());
    info.pSubpasses = vkSubpasses.data();
    info.pDependencies = vkDependencies.data();
    info.dependencyCount = ToU32(vkDependencies.size());

    VkRenderPass renderPass;
    device.CreateRenderPass(&info, &renderPass);
    return renderPass;
}

static ea::vector<GPUFormat> CreateAttachmentsForSinglePass(ea::span<const GPUFormat> colorAttachments, bool hasDepthStencil)
{
    ea::vector<GPUFormat> attachements{};
    attachements.assign(colorAttachments.begin(), colorAttachments.end());
    if (hasDepthStencil)
    {
        attachements.push_back(GPUFormat::eDepth24S8);
    }

    return attachements;
}

static ea::vector<SubpassLayout> CreateSubpassesForSinglePass(ea::span<const GPUFormat> colorAttachments, bool hasDepthStencil)
{
    ea::vector<uint32> colorAttachmentRefs{};
    colorAttachmentRefs.resize(colorAttachments.size());

    for (int i = 0; i < colorAttachmentRefs.size(); ++i)
    {
        colorAttachmentRefs[i] = i;
    }

    SubpassLayout subpass = {};
    subpass.colorBufferRefs = std::move(colorAttachmentRefs);
    subpass.depthStencilRef = NullSubpassRef;

    if (hasDepthStencil)
    {
        subpass.depthStencilRef = uint32(colorAttachments.size());
    }

    return ea::vector<SubpassLayout>(1, subpass);
}

GPUVulkanRenderPass::GPUVulkanRenderPass()
{
    m_gpu = nullptr;
    m_id = GPURenderPassNullID;
    m_renderPass = nullptr;
}

GPUVulkanRenderPass::GPUVulkanRenderPass(GPUVulkan& gpu, ea::span<const GPUFormat> attachments, ea::span<const SubpassLayout> subpassLayouts, ea::span<const uint32> multiviewMasks) : GPUVulkanRenderPass()
{
    AssertValidInputs(gpu, attachments, subpassLayouts, multiviewMasks);
    VexDevice& device = gpu.GetVexDevice();

    GPURenderPassID id = CreateUniqueRenderPassID();
    VkRenderPass renderPass = CreateRenderPass(gpu, attachments, subpassLayouts, multiviewMasks);
    ea::vector<GPUFormat> attachmentsCopy(attachments.begin(), attachments.end());
    ea::vector<SubpassLayout> subpassesCopy(subpassLayouts.begin(), subpassLayouts.end());

    m_gpu = &gpu;
    m_id = id;
    m_renderPass = &device.GetVexRenderPass(renderPass);
    m_attachments = std::move(attachmentsCopy);
    m_subpasses = std::move(subpassesCopy);
}

GPUVulkanRenderPass::GPUVulkanRenderPass(GPUVulkan& gpu, ea::span<const GPUFormat> colorAttachments, bool hasDepthStencil) :
    GPUVulkanRenderPass(gpu,
        CreateAttachmentsForSinglePass(colorAttachments, hasDepthStencil),
        CreateSubpassesForSinglePass(colorAttachments, hasDepthStencil),
        {})
{
}

GPUVulkanRenderPass::~GPUVulkanRenderPass()
{
    if (IsNull())
        return;

    VexDevice& device = m_gpu->GetVexDevice();
    device.DestroyRenderPass(m_renderPass->handle);
}

bool GPUVulkanRenderPass::IsNull() const
{
    return m_id == GPURenderPassNullID;
}

GPUBackend GPUVulkanRenderPass::GetGPUBackend() const
{
    return GPUBackend::eVulkan;
}

GPU& GPUVulkanRenderPass::GetGPU() const
{
    AssertNotNull();
    return *m_gpu;
}

GPURenderPassID GPUVulkanRenderPass::GetID() const
{
    AssertNotNull();
    return m_id;
}

uint32 GPUVulkanRenderPass::GetColorAttachmentCount(uint32 subpass) const
{
    AssertNotNull();
    return uint32(m_subpasses[subpass].colorBufferRefs.size());
}

ea::span<const GPUFormat> GPUVulkanRenderPass::GetAttachments() const
{
    AssertNotNull();
    return m_attachments;
}

ea::span<const SubpassLayout> GPUVulkanRenderPass::GetSubpasses() const
{
    AssertNotNull();
    return m_subpasses;
}

const VexRenderPass& GPUVulkanRenderPass::GetVexRenderPass() const
{
    AssertNotNull();
    return *m_renderPass;
}

VkRenderPass GPUVulkanRenderPass::GetHandle() const
{
    AssertNotNull();
    return m_renderPass->handle;
}

void GPUVulkanRenderPass::AssertNotNull() const
{
    ASSERT(!IsNull());
}