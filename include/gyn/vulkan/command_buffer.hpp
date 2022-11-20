#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    ea::vector<VkCommandBuffer> RecordRendering(GPUVulkan& gpu,
        JobScheduler& jobScheduler,
        const GPUFrameExecution& frame,
        ea::vector<VkFence>& outSignalFences,
        ea::vector<VkSemaphore>& outSignalSemaphores);
}