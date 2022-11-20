
#include <pch.hpp>
#include <gyn/vulkan/helper.hpp>
#include <gyn/vulkan/command_buffer.hpp>
#include <gyn/vulkan/framebuffer.hpp>
#include <gyn/vulkan/render_pass.hpp>
#include <gyn/vulkan/bind_set.hpp>
#include <gyn/vulkan/shader.hpp>
#include <gyn/vulkan/buffer.hpp>
#include <gyn/vulkan/image.hpp>
#include <gyn/vulkan/fence.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/job_scheduler.hpp>

namespace
{
    struct RecordRenderPassJob : Job
    {
        GPUVulkan* gpu = {};
        bool renderUI = false;
        const GPUFrameExecution* frame = {};
        const GPURenderPassExecution* pass = {};

        VkPipelineStageFlags barrierSrcMask = {};
        VkPipelineStageFlags barrierDstMask = {};
        ea::vector<VkImageMemoryBarrier> imageBarriers = {};

        VkCommandBuffer* outCmdBuffer = {};
        std::atomic_int32_t* signal = {};

        void operator()(uint32 workerIndex);
    };
}

static void BeginRenderPass(VexDevice& device, VkCommandBuffer cmdBuffer, const GPURenderPassExecution& pass)
{
    const GPUVulkanFramebuffer& framebuffer = CastVulkan(*pass.framebuffer);
    const ea::span<const VkClearValue> clearValues = framebuffer.GetClearValues();

    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = CastVulkan(framebuffer.GetRenderPass()).GetHandle();
    info.framebuffer = framebuffer.GetHandle();
    info.renderArea = framebuffer.GetRenderArea();
    info.pClearValues = clearValues.data();
    info.clearValueCount = ToU32(clearValues.size());

    device.CmdBeginRenderPass(cmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

static void EndRenderPass(VexDevice& device, VkCommandBuffer cmdBuffer)
{
    device.CmdEndRenderPass(cmdBuffer);
}

static void RecordInstanceDrawing(GPUVulkan& gpu, VkCommandBuffer vkCmd, const GPUFrameExecution& graph, const GPURenderPassExecution& renderPass, const GPUSupbassExecution& subpass)
{
    VexDevice& device = gpu.GetVexDevice();
    VkDescriptorSet bindSet0 = CastVulkan(*graph.bindSet0).GetHandle();
    const GPUCommandBuffer& cmdBuffer = subpass.commandBuffer;

    ea::span<const GPUShader* const> shaders = cmdBuffer.shaders;
    ea::span<const GPUBindSet* const> bindSets3 = cmdBuffer.bindSets3;
    ea::span<const GPUBuffer* const> vertexBuffers = cmdBuffer.vertexBuffers;
    ea::span<const GPUBuffer* const> indexBuffers = cmdBuffer.indexBuffers;

    ea::span<const uint32> instanceCounts = cmdBuffer.instanceCounts;
    ea::span<const uint32> firstInstances = cmdBuffer.firstInstances;

    uint32 shaderCount = ToU32(shaders.size());
    uint32 vertexBufferCount = ToU32(vertexBuffers.size());
    uint32 cmdCount = shaderCount * vertexBufferCount; MaybeUnused(cmdCount);

    ASSERT(shaderCount == bindSets3.size());
    ASSERT(vertexBufferCount == indexBuffers.size());
    ASSERT(cmdCount == instanceCounts.size());
    ASSERT(cmdCount == firstInstances.size());

    VkDescriptorSet bindSet1 = renderPass.bindSet1 ? CastVulkan(*renderPass.bindSet1).GetHandle() : gpu.GetDummyBindSet();
    VkDescriptorSet bindSet2 = subpass.bindSet2 ? CastVulkan(*subpass.bindSet2).GetHandle() : gpu.GetDummyBindSet();

    uint32 k = 0;
    for (uint32 i = 0; i < shaderCount; ++i)
    {
        const GPUVulkanShader& shader = CastVulkan(*shaders[i]);
        VkPipelineLayout pipelineLayout = shader.GetPipelineLayout();
        VkPipeline pipeline = shader.GetPipeline();

        VkDescriptorSet bindSet3 = CastVulkan(*bindSets3[i]).GetHandle();

        ea::array<VkDescriptorSet, 4> dscSets
        {
            bindSet0,
            bindSet1,
            bindSet2,
            bindSet3
        };

        device.CmdBindPipeline(vkCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        device.CmdBindDescriptorSets(vkCmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, ToU32(dscSets.size()), dscSets.data(), 0, nullptr);

        for (uint32 j = 0; j < vertexBufferCount; ++j, ++k)
        {
            uint32 instanceCount = instanceCounts[k];
            if (instanceCount == 0) continue;

            uint32 firstInstance = firstInstances[k];

            const GPUVulkanBuffer& vertexBuffer = CastVulkan(*vertexBuffers[j]);
            const GPUVulkanBuffer& indexBuffer = CastVulkan(*indexBuffers[j]);

            ea::array<VkBuffer, 1> vertexBufferBindings
            {
                vertexBuffer.GetHandle()
            };

            ea::array<VkDeviceSize, 1> vertexBufferOffsets
            {
                0
            };

            uint32 indexCount = ToU32(indexBuffer.GetAllocSize() / sizeof(uint16));

            device.CmdBindVertexBuffers(vkCmd, 0, ToU32(vertexBufferBindings.size()), vertexBufferBindings.data(), vertexBufferOffsets.data());
            device.CmdBindIndexBuffer(vkCmd, indexBuffer.GetHandle(), 0, VK_INDEX_TYPE_UINT16);
            device.CmdDrawIndexed(vkCmd, indexCount, instanceCount, 0, 0, firstInstance);
        }
    }
}

void RecordRenderPassJob::operator()(uint32 workerIndex)
{
    VexDevice& device = gpu->GetVexDevice();

    const GPUVulkanFramebuffer& vkFramebuffer = CastVulkan(*pass->framebuffer);
    uint32 subpassCount = ToU32(pass->subpasses.size());
    VkViewport viewport = vkFramebuffer.GetViewport();
    VkRect2D renderArea = vkFramebuffer.GetRenderArea();
    VkCommandPool commandPool = gpu->GetWorkerCommandPool(workerIndex);

    VkCommandBuffer vkCmd;
    AllocateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &vkCmd);

    BeginCommandBuffer(device, vkCmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    if (!imageBarriers.empty())
    {
        device.CmdPipelineBarrier(vkCmd, barrierSrcMask, barrierDstMask, 0, 0, nullptr, 0, nullptr, ToU32(imageBarriers.size()), imageBarriers.data());
    }

    BeginRenderPass(device, vkCmd, *pass);
    if (renderUI)
    {
        gpu->ImGuiRecordRendering(vkCmd);
    }
    else
    {
        device.CmdSetViewport(vkCmd, 0, 1, &viewport);
        device.CmdSetScissor(vkCmd, 0, 1, &renderArea);

        for (uint32 s = 0; s < subpassCount; ++s)
        {
            if (s > 0)
            {
                device.CmdNextSubpass(vkCmd, VK_SUBPASS_CONTENTS_INLINE);
            }

            RecordInstanceDrawing(*gpu, vkCmd, *frame, *pass, pass->subpasses[s]);
        }
    }
    EndRenderPass(device, vkCmd);
    device.EndCommandBuffer(vkCmd);

    *outCmdBuffer = vkCmd;
    signal->fetch_sub(1);
}

ea::vector<VkCommandBuffer> Gyn::RecordRendering(GPUVulkan& gpu, 
    JobScheduler& jobScheduler, 
    const GPUFrameExecution& frame,
    ea::vector<VkFence>& outSignalFences, 
    ea::vector<VkSemaphore>& outSignalSemaphores)
{
    ASSERT_MSG(frame.blurPassCount > 0, "0 Bloom blur pass case not handled");

    VexDevice& device = gpu.GetVexDevice();
    VkQueue queue = device.graphicsQueue;
    VkCommandPool commandPool = gpu.GetCommandPool();

    const uint32 blurPassCount = frame.blurPassCount;

    const uint32 shadowPassCount = ToU32(frame.shadowPasses.size());
    const uint32 sunShadowPassIndex = shadowPassCount;
    const uint32 gbufferPassIndex = sunShadowPassIndex + 1;
    const uint32 gbufferBarrierIndex = gbufferPassIndex + 1;
    const uint32 tileCullingPassIndex = gbufferBarrierIndex + 1;
    const uint32 ssaoPassIndex = tileCullingPassIndex + 1;
    const uint32 blurPassBegin = ssaoPassIndex + 1;
    const uint32 lightingBarrierIndex = blurPassBegin + blurPassCount;
    const uint32 lightingPassIndex = lightingBarrierIndex + 1;
    const uint32 presentBarrierIndex = lightingPassIndex + 1;
    const uint32 presentPassIndex = presentBarrierIndex + 1;
    const uint32 cmdBufferCount = presentPassIndex + 1;

    ea::vector<VkCommandBuffer> commandBuffers = {};
    commandBuffers.resize(cmdBufferCount);

    std::atomic_int32_t signal = 0;

    for (uint32 i = 0; i < shadowPassCount; ++i)
    {
        auto& shadowPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
        shadowPassJob.gpu = &gpu;
        shadowPassJob.frame = &frame;
        shadowPassJob.pass = &frame.shadowPasses[i];
        shadowPassJob.outCmdBuffer = &commandBuffers[i];
        shadowPassJob.signal = &signal;

        signal.fetch_add(1);
        jobScheduler.Submit(shadowPassJob);
    }



    auto& sunShadowPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
    sunShadowPassJob.gpu = &gpu;
    sunShadowPassJob.frame = &frame;
    sunShadowPassJob.pass = &frame.sunShadowPass;
    sunShadowPassJob.outCmdBuffer = &commandBuffers[sunShadowPassIndex];
    sunShadowPassJob.signal = &signal;

    signal.fetch_add(1);
    jobScheduler.Submit(sunShadowPassJob);



    auto& gbufferPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
    gbufferPassJob.gpu = &gpu;
    gbufferPassJob.frame = &frame;
    gbufferPassJob.pass = &frame.gbufferPass;
    gbufferPassJob.outCmdBuffer = &commandBuffers[gbufferPassIndex];
    gbufferPassJob.signal = &signal;

    signal.fetch_add(1);
    jobScheduler.Submit(gbufferPassJob);



    auto& tileCullingPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
    tileCullingPassJob.gpu = &gpu;
    tileCullingPassJob.frame = &frame;
    tileCullingPassJob.pass = &frame.tileCullingPass;
    tileCullingPassJob.outCmdBuffer = &commandBuffers[tileCullingPassIndex];
    tileCullingPassJob.signal = &signal;

    signal.fetch_add(1);
    jobScheduler.Submit(tileCullingPassJob);



    auto& ssaoPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
    ssaoPassJob.gpu = &gpu;
    ssaoPassJob.frame = &frame;
    ssaoPassJob.pass = &frame.ssaoPass;
    ssaoPassJob.outCmdBuffer = &commandBuffers[ssaoPassIndex];
    ssaoPassJob.signal = &signal;

    signal.fetch_add(1);
    jobScheduler.Submit(ssaoPassJob);



    for (uint32 i = 0; i < blurPassCount; ++i)
    {
        bool isFirstPass = i == 0;
        bool startBuffer0 = (blurPassCount & 1) == 0;
        bool isBuffer0 = startBuffer0 ^ (i & 1);

        const GPURenderPassExecution* pass = nullptr;
        if (isFirstPass)
        {
            pass = isBuffer0 ? &frame.firstBlurPass0 : &frame.firstBlurPass1;
        }
        else
        {
            pass = isBuffer0 ? &frame.blurPass0 : &frame.blurPass1;
        }

        ea::vector<VkImageMemoryBarrier> imageBarriers{};
        if (!isFirstPass)
        {
            const GPUImage& inputImage = isBuffer0 ? *frame.blurBuffer1 : *frame.blurBuffer0;

            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.image = CastVulkan(inputImage).GetHandle();

            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;


            barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.layerCount = 1;
            barrier.subresourceRange.levelCount = 1;

            imageBarriers.emplace_back(barrier);
        }

        auto& blurPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
        blurPassJob.gpu = &gpu;
        blurPassJob.frame = &frame;
        blurPassJob.pass = pass;

        blurPassJob.barrierSrcMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        blurPassJob.barrierDstMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        blurPassJob.imageBarriers = std::move(imageBarriers);

        blurPassJob.outCmdBuffer = &commandBuffers[blurPassBegin + i];
        blurPassJob.signal = &signal;

        signal.fetch_add(1);
        jobScheduler.Submit(blurPassJob);
    }



    auto& lightingPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
    lightingPassJob.gpu = &gpu;
    lightingPassJob.frame = &frame;
    lightingPassJob.pass = &frame.lightingPass;
    lightingPassJob.outCmdBuffer = &commandBuffers[lightingPassIndex];
    lightingPassJob.signal = &signal;

    signal.fetch_add(1);
    jobScheduler.Submit(lightingPassJob);



    auto& presentPassJob = jobScheduler.CreateJob<RecordRenderPassJob>();
    presentPassJob.gpu = &gpu;
    presentPassJob.frame = &frame;
    presentPassJob.pass = &frame.presentPass;
    presentPassJob.outCmdBuffer = &commandBuffers[presentPassIndex];
    presentPassJob.signal = &signal;
    presentPassJob.renderUI = true;

    signal.fetch_add(1);
    jobScheduler.Submit(presentPassJob);


    VkCommandBuffer& gbufferBarrierCmd = commandBuffers[gbufferBarrierIndex];
    AllocateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &gbufferBarrierCmd);
    BeginCommandBuffer(device, gbufferBarrierCmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        VkImageSubresourceRange depthStencilSubresource = {};
        depthStencilSubresource.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        depthStencilSubresource.baseArrayLayer = 0;
        depthStencilSubresource.baseMipLevel = 0;
        depthStencilSubresource.layerCount = 1;
        depthStencilSubresource.levelCount = 1;

        VkImageSubresourceRange colorSubresource = {};
        colorSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorSubresource.baseArrayLayer = 0;
        colorSubresource.baseMipLevel = 0;
        colorSubresource.layerCount = 1;
        colorSubresource.levelCount = 1;

        ea::array<VkImageMemoryBarrier, 3> barriers = {};
        barriers[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barriers[0].image = CastVulkan(*frame.depthBuffer).GetHandle();

        barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barriers[0].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        barriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        barriers[0].subresourceRange = depthStencilSubresource;


        barriers[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barriers[1].image = CastVulkan(*frame.normalBuffer).GetHandle();

        barriers[1].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barriers[1].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barriers[1].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        barriers[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        barriers[1].subresourceRange = colorSubresource;


        barriers[2].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barriers[2].image = CastVulkan(*frame.emissiveBuffer).GetHandle();

        barriers[2].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barriers[2].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        barriers[2].oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        barriers[2].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barriers[2].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        barriers[2].subresourceRange = colorSubresource;

        VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        device.CmdPipelineBarrier(gbufferBarrierCmd, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, ToU32(barriers.size()), barriers.data());
    }
    device.EndCommandBuffer(gbufferBarrierCmd);

    VkCommandBuffer& lightingBarrierCmd = commandBuffers[lightingBarrierIndex];
    AllocateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &lightingBarrierCmd);
    BeginCommandBuffer(device, lightingBarrierCmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        VkMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        VkPipelineStageFlags srcStageMask =
            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
            | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT
            | VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        device.CmdPipelineBarrier(lightingBarrierCmd, srcStageMask, dstStageMask, 0, 1, &barrier, 0, nullptr, 0, nullptr);
    }
    device.EndCommandBuffer(lightingBarrierCmd);

    VkCommandBuffer& presentBarrierCmd = commandBuffers[presentBarrierIndex];
    AllocateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, &presentBarrierCmd);
    BeginCommandBuffer(device, presentBarrierCmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        VkMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        device.CmdPipelineBarrier(presentBarrierCmd, srcStageMask, dstStageMask, 0, 1, &barrier, 0, nullptr, 0, nullptr);
    }
    device.EndCommandBuffer(presentBarrierCmd);

    ea::vector<VkFence> signalFences = {};
    {
        VkFence& fence = signalFences.emplace_back();
        CreateFence(device, 0, &fence);
    }

    ea::vector<VkSemaphore> signalSemaphores = {};
    signalSemaphores.resize(cmdBufferCount);

    for (uint32 i = 0; i < cmdBufferCount; ++i)
    {
        CreateSemaphore(device, &signalSemaphores[i]);
    }

    jobScheduler.Wait(signal);

    VkSubmitInfo submit = {};
    submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit.commandBufferCount = ToU32(commandBuffers.size());
    submit.pCommandBuffers = commandBuffers.data();
    submit.signalSemaphoreCount = ToU32(signalSemaphores.size());
    submit.pSignalSemaphores = signalSemaphores.data();

    device.QueueSubmit(queue, 1, &submit, signalFences[0]);

    outSignalFences = std::move(signalFences);
    outSignalSemaphores = std::move(signalSemaphores);

    return commandBuffers;
}