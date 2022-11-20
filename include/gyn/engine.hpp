#pragma once

#include <gyn/common.hpp>

namespace Gyn
{
    class RenderAssetManager;
    class MatMeshRefCountManager;

    struct Engine : NoCopyMove
    {
    public:

        Engine() = default;

        UniquePtr<Window> window = {};
        UniquePtr<GPU> gpu = {};
        UniquePtr<Keyboard> keyboard = {};
        UniquePtr<TimeManager> time = {};
        UniquePtr<JobScheduler> jobScheduler = {};
        UniquePtr<RenderAssetManager> renderAssets = {};
        UniquePtr<MatMeshRefCountManager> matMeshRefCounts = {};
    };
}