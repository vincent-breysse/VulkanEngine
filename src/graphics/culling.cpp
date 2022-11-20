
#include <pch.hpp>
#include <gyn/graphics/culling.hpp>
#include <gyn/job_scheduler.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/graphics_engine.hpp>

namespace
{
    struct CullingRange
    {
        const uint32* gpuIndices = {};
        const ModelID* modelIDs = {};
        const AABB* aabbs = {};
        const bool* forceCulls = {};
        uint32 size = {};
    };

    struct CullingContext
    {
        FrustrumPlanesSOA frustrumPlanes = {};
        ea::vector<CullingRange> ranges = {};
        const RenderAssetManager* renderAsset = {};
        const MatMeshQuery* matMeshes = {};
        const InstanceDrawingLayout* drawingLayout = {};
    };

    enum class CullingType
    {
        eCamera,
        eShadows
    };

    struct PointLightCullingSlice
    {
        float3 position = {};
        float shadowFar = {};
        const PVSDataCache* pvsCache = {};
    };

    struct JobForLoopLayout
    {
        uint32 rangeSize = {};
        uint32 rangeCount = {};
        uint32 rest = {};
    };

    struct CullRendererRangeJob : Job
    {
        CullingContext context = {};
        uint32 rangeIndex = {};
        CullingOutput* output = {};
        std::atomic_int32_t* signal = {};

        void operator()(uint32);
    };
}

static void SortPointLightCulling(ea::span<const uint32> sortIndices, PointLightCulling& culling)
{
    constexpr bool SortInline = true;

    if constexpr (SortInline) // Inline version. Supposed to be faster
    {
        usize lightCount = sortIndices.size();

        ea::vector<uint32> who{};
        who.resize(lightCount);

        ea::vector<uint32> where{};
        where.resize(lightCount);

        for (usize i = 0; i < lightCount; ++i)
        {
            who[i] = ToU32(i);
            where[i] = ToU32(i);
        }

        for (usize i = 0; i < lightCount; ++i)
        {
            uint32 a = ToU32(i);
            uint32 b = where[sortIndices[i]];

            ASSERT(who[b] == sortIndices[i]);

            if (a == b) continue;

            ea::swap(culling.flatIndices[a], culling.flatIndices[b]);
            ea::swap(culling.positionAndRanges[a], culling.positionAndRanges[b]);
            ea::swap(culling.colorAndIntensities[a], culling.colorAndIntensities[b]);
            ea::swap(culling.shadowMatrices[a], culling.shadowMatrices[b]);
            ea::swap(culling.shadowParams[a], culling.shadowParams[b]);

            where[who[a]] = b;
            where[who[b]] = a;
            ea::swap(who[a], who[b]);
        }
    }
    else // Naive double buffered version. Supposed to be slower
    {
        PointLightCulling temp = culling;

        for (usize i = 0; i < sortIndices.size(); ++i)
        {
            uint32 src = ToU32(i);
            uint32 dst = sortIndices[i];

            if (src == dst) continue;

            culling.flatIndices[src] = temp.flatIndices[dst];
            culling.positionAndRanges[src] = temp.positionAndRanges[dst];
            culling.colorAndIntensities[src] = temp.colorAndIntensities[dst];
            culling.shadowMatrices[src] = temp.shadowMatrices[dst];
            culling.shadowParams[src] = temp.shadowParams[dst];
        }
    }
}

static vfloat4 v4MulInvSign(vfloat4 v, vfloat4 sign)
{
    vfloat4 test = v4Greater(sign, v4Zero());
    vfloat4 toggleSign = v4And(test, v4SignMask());
    return v4Xor(v, toggleSign);
}

static bool v4AllNotClipped(vfloat4 x, vfloat4 y, vfloat4 z, vfloat4 planeX, vfloat4 planeY, vfloat4 planeZ, vfloat4 planeW)
{
    vfloat4 xx = v4Mul(x, planeX);
    vfloat4 yy = v4Mul(y, planeY);
    vfloat4 zz = v4Mul(z, planeZ);
    vfloat4 dot = v4Add(v4Add(xx, yy), zz);
    return v4AllLessEqual(dot, planeW);
}

static bool v4AllNotClipped(vfloat4 cx, vfloat4 cy, vfloat4 cz, vfloat4 ex, vfloat4 ey, vfloat4 ez, vfloat4 px, vfloat4 py, vfloat4 pz, vfloat4 pw)
{
    vfloat4 localFarestX = v4MulInvSign(ex, px);
    vfloat4 localFarestY = v4MulInvSign(ey, py);
    vfloat4 localFarestZ = v4MulInvSign(ez, pz);

    vfloat4 farestX = v4Add(cx, localFarestX);
    vfloat4 farestY = v4Add(cy, localFarestY);
    vfloat4 farestZ = v4Add(cz, localFarestZ);

    return v4AllNotClipped(farestX, farestY, farestZ, px, py, pz, pw);
}

static bool IsInFrustrum(const AABB& aabb, const FrustrumPlanesSOA& soa)
{
    vfloat4 cx = v4Spread(aabb.center.x);
    vfloat4 cy = v4Spread(aabb.center.y);
    vfloat4 cz = v4Spread(aabb.center.z);

    vfloat4 ex = v4Spread(aabb.extents.x);
    vfloat4 ey = v4Spread(aabb.extents.y);
    vfloat4 ez = v4Spread(aabb.extents.z);

    bool notClipped0 = v4AllNotClipped(cx, cy, cz, ex, ey, ez, soa.px0, soa.py0, soa.pz0, soa.pw0);
    bool notClipped1 = v4AllNotClipped(cx, cy, cz, ex, ey, ez, soa.px1, soa.py1, soa.pz1, soa.pw1);

    return notClipped0 && notClipped1;
}

static bool IsClipped(float3 p, float4 plane)
{
    return Vec3Dot(p, plane.xyz()) > plane.w;
}

static bool IsClipped(const Sphere& sphere, float4 plane)
{
    return IsClipped(sphere.center - plane.xyz() * sphere.radius, plane);
}

static bool IsInFrustrum(const Sphere& sphere, const ea::array<float4, 6>& planes)
{
    return !IsClipped(sphere, planes[0])
        && !IsClipped(sphere, planes[1])
        && !IsClipped(sphere, planes[2])
        && !IsClipped(sphere, planes[3])
        && !IsClipped(sphere, planes[4])
        && !IsClipped(sphere, planes[5]);
}

static PointLightCulling CullPointLights(float3 cameraPosition, const ea::array<float4, 6>& frustrumPlanes, ea::span<const PointLightChunk> chunks, uint32 totalLightCount)
{
    PointLightCulling culling{};
    culling.flatIndices.reserve(totalLightCount);
    culling.colorAndIntensities.reserve(totalLightCount);
    culling.positionAndRanges.reserve(totalLightCount);
    culling.shadowMatrices.reserve(totalLightCount);
    culling.shadowParams.reserve(totalLightCount);

    ea::vector<uint32> sortIndices{};
    sortIndices.reserve(totalLightCount);

    uint32 flatIndex = 0;
    uint32 lightCount = 0;

    for (usize c = 0; c < chunks.size(); ++c)
    {
        const PointLightChunk& range = chunks[c];

        for (usize i = 0; i < range.size; ++i, ++flatIndex)
        {
            const PointLight& light = range.lights[i];
            float3 position = range.positions[i];

            Sphere sphere(position, light.range);

            if (!IsInFrustrum(sphere, frustrumPlanes))
                continue;

            culling.flatIndices.push_back(flatIndex);
            culling.positionAndRanges.push_back(float4(position, light.range));
            culling.colorAndIntensities.push_back(float4(light.color, light.intensity));
            culling.shadowMatrices.push_back(ComputeShadowMatrices(position, light));
            culling.shadowParams.push_back(GetShadowParams(light));

            sortIndices.push_back(lightCount);
            ++lightCount;
        }
    }

    ea::quick_sort(sortIndices.begin(), sortIndices.end(), [&](uint32 i, uint32 j)
        {
            const float4& posAndRangeI = culling.positionAndRanges[i];
            const float4& posAndRangeJ = culling.positionAndRanges[j];

            const auto posI = float3(&posAndRangeI.x);
            const auto posJ = float3(&posAndRangeJ.x);

            const float di = Vec3LengthSq(posI - cameraPosition);
            const float dj = Vec3LengthSq(posJ - cameraPosition);

            return di < dj;
        });

    SortPointLightCulling(sortIndices, culling);

    if (lightCount >= MaxPointLightCount)
    {
        culling.flatIndices.resize(MaxPointLightCount);
        culling.colorAndIntensities.resize(MaxPointLightCount);
        culling.positionAndRanges.resize(MaxPointLightCount);
        culling.shadowMatrices.resize(MaxPointLightCount);
        culling.shadowParams.resize(MaxPointLightCount);

        fmt::print("Visible Light count ({}) exceeded MaxPointLightCount ({}). Some lights were ignored\n", lightCount, MaxPointLightCount);

        lightCount = MaxPointLightCount;
    }

    culling.lightCount = lightCount;
    culling.shadowLightCount = std::min(MaxPointLightCastingShadows, lightCount);

    ASSERT(culling.colorAndIntensities.size() == culling.positionAndRanges.size());

    return culling;
}

static PointLightCulling CullPointLightsWithoutPVS(const RenderContext& context)
{
    return CullPointLights(context.cameraPosition, context.cameraFrustrumPlanes, context.pointLightChunks, context.totalPointLightCount);
}

static PointLightCulling CullPointLightsWithPVS(const RenderContext& context, const PVSDataCache& pvsCache)
{
    uint32 size = ToU32(pvsCache.pointLightDatas.size());
    ASSERT(pvsCache.pointLightPositions.size() == size);
    ASSERT(pvsCache.pointLightPVSIDs.size() == size);
    ASSERT(pvsCache.pointLightDatas.size() == size);

    PointLightChunk chunk = {};
    chunk.lights = pvsCache.pointLightDatas.data();
    chunk.positions = pvsCache.pointLightPositions.data();
    chunk.pvsIDs = pvsCache.pointLightPVSIDs.data();
    chunk.size = size;

    return CullPointLights(context.cameraPosition, context.cameraFrustrumPlanes, ea::span(&chunk, 1), size);
}

static PointLightCulling CullPointLights(const RenderContext& context)
{
    const PVSDataCache* pvsCache = context.global->usePVS ? context.cameraPVSDataCache : nullptr;

    if (!pvsCache)
    {
        return CullPointLightsWithoutPVS(context);
    }

    return CullPointLightsWithPVS(context, *pvsCache);
}

static void CullRenderers(const CullingContext& context, uint32 rangeIndex, CullingOutput& output)
{
    ASSERT(context.renderAsset);
    ASSERT(context.matMeshes);
    ASSERT(context.drawingLayout);

    const RenderAssetManager& renderAssets = *context.renderAsset;
    const MatMeshQuery& matMeshes = *context.matMeshes;
    const InstanceDrawingLayout& drawingLayout = *context.drawingLayout;
    const FrustrumPlanesSOA& frustrumPlanes = context.frustrumPlanes;
    const ea::vector<CullingRange>& ranges = context.ranges;

    ea::vector<uint32>& instanceCounts = output.instanceCounts;
    ea::vector<uint32>& remapTable = output.remappedEntityIDs;
    uint32& visibleRendererCount = output.visibleRendererCount;

    const CullingRange& range = ranges[rangeIndex];

    for (usize i = 0; i < range.size; ++i)
    {
        const bool forceCull = range.forceCulls ? range.forceCulls[i] : false;

        if (forceCull)
            continue;

        const ModelID modelID = range.modelIDs[i];
        const uint32 gpuIndex = range.gpuIndices[i];
        const AABB& aabb = range.aabbs[i];

        if (!IsInFrustrum(aabb, frustrumPlanes))
            continue;

        const ea::span<const MatMeshLocalIndex> matMeshIDs = renderAssets.GetModelLocalIndices(modelID);

        for (usize j = 0; j < matMeshIDs.size(); ++j)
        {
            const MatMeshLocalIndex matMeshID = matMeshIDs[j];

            const uint32 batchIndex = matMeshes.GetRemappedInstanceBatchIndex(matMeshID);
            uint32 instanceCount = AtomicFetchAdd(instanceCounts[batchIndex]);

            const uint32 offset = drawingLayout.firstInstances[batchIndex];
            const uint32 remapIndex = offset + instanceCount;

            remapTable[remapIndex] = gpuIndex;
        }

        AtomicFetchAdd(visibleRendererCount);
    }
}

void CullRendererRangeJob::operator()(uint32)
{
    CullRenderers(context, rangeIndex, *output);
    --(*signal);
}

static JobForLoopLayout ComputeJobForLoopLayout(uint32 totalSize, uint32 wantedRangeSize)
{
    JobForLoopLayout layout = {};

    if (totalSize < wantedRangeSize)
    {
        layout.rangeCount = 1;
        layout.rangeSize = totalSize;
        layout.rest = 0;

        return layout;
    }

    uint32 rangeCount = totalSize / wantedRangeSize;
    uint32 rest = totalSize - rangeCount * wantedRangeSize;

    layout.rangeCount = rangeCount;
    layout.rangeSize = wantedRangeSize;
    layout.rest = rest;

    return layout;
}

static ea::vector<CullingRange> CreateCullingRangesFromScene(const RenderContext& context, CullingType type, uint32 wantedSizePerRange)
{
    const ea::vector<RenderEntityChunk>& entityChunks = context.entityChunks;
    uint32 chunkCount = ToU32(entityChunks.size());

    uint32 estimatedRangeCount = DivideRoundUp(context.totalEntityCount, wantedSizePerRange);

    ea::vector<CullingRange> cullingRanges;
    cullingRanges.reserve(estimatedRangeCount);

    for (uint32 c = 0; c < chunkCount; ++c)
    {
        const RenderEntityChunk& chunk = entityChunks[c];

        JobForLoopLayout layout = ComputeJobForLoopLayout(chunk.size, wantedSizePerRange);

        uint32 begin = 0;
        uint32 end = layout.rangeSize + layout.rest;

        for (uint32 i = 0; i < layout.rangeCount; ++i)
        {
            CullingRange& cullingRange = cullingRanges.emplace_back();
            cullingRange.gpuIndices = chunk.gpuIndices + begin;
            cullingRange.modelIDs = chunk.modelIDs + begin;
            cullingRange.aabbs = chunk.worldAABBs + begin;
            cullingRange.forceCulls = type == CullingType::eShadows ? chunk.disableShadows + begin : nullptr;
            cullingRange.size = end - begin;

            begin = end;
            end += layout.rangeSize;
        }
    }

    return cullingRanges;
}

static ea::vector<CullingRange> CreateCullingRangesFromPVS(const PVSDataCache& pvsCache, CullingType type, uint32 wantedSizePerRange)
{
    const uint32* gpuIndices = pvsCache.entityGPUIndices.data();
    const ModelID* modelIDs = pvsCache.modelIDs.data();
    const AABB* worldBounds = pvsCache.entityWorldBounds.data();
    const bool* disableShadows = pvsCache.entityDisableShadows.data();

    uint32 totalSize = ToU32(pvsCache.entityGPUIndices.size());

    JobForLoopLayout layout = ComputeJobForLoopLayout(totalSize, wantedSizePerRange);

    ea::vector<CullingRange> ranges = {};
    ranges.resize(layout.rangeCount);

    uint32 begin = 0;
    uint32 end = layout.rangeSize + layout.rest;

    for (uint32 i = 0; i < layout.rangeCount; ++i)
    {
        CullingRange& range = ranges[i];
        range.gpuIndices = gpuIndices + begin;
        range.modelIDs = modelIDs + begin;
        range.aabbs = worldBounds + begin;
        range.forceCulls = type == CullingType::eShadows ? disableShadows + begin : nullptr;
        range.size = end - begin;

        begin = end;
        end += layout.rangeSize;
    }

    return ranges;
}

static ea::vector<CullingRange> CreateCullingRanges(const RenderContext& context, const PVSDataCache* pvsCache, CullingType cullingType, uint32 wantedSizePerRange)
{
    if (pvsCache)
    {
        return CreateCullingRangesFromPVS(*pvsCache, cullingType, wantedSizePerRange);
    }
    else
    {
        return CreateCullingRangesFromScene(context, cullingType, wantedSizePerRange);
    }
}

static CullingContext CreateCameraCullingContext(const RenderContext& context, const MatMeshMetadataCache& cache, uint32 wantedSizePerRange)
{
    const PVSDataCache* pvsCache = context.global->usePVS ? context.cameraPVSDataCache : nullptr;

    CullingContext cullingContext = {};
    cullingContext.frustrumPlanes = context.cameraFrustrumPlanesSOA;
    cullingContext.ranges = CreateCullingRanges(context, pvsCache, CullingType::eCamera, wantedSizePerRange);
    cullingContext.renderAsset = context.renderAssets;
    cullingContext.matMeshes = &cache.mainMatMeshQuery;
    cullingContext.drawingLayout = &cache.mainDrawingLayout;

    return cullingContext;
}

static void KickCameraFrustumCulling(JobScheduler& jobScheduler, const RenderContext& context, const MatMeshMetadataCache& cache, uint32 wantedSizePerRange, CullingOutput* output, std::atomic_int32_t* signal)
{
    bool threadedCulling = context.global->threadedCulling;
    const MatMeshQuery& matMeshes = cache.mainMatMeshQuery;
    const InstanceDrawingLayout& drawingLayout = cache.mainDrawingLayout;

    CullingContext cullingContext = CreateCameraCullingContext(context, cache, wantedSizePerRange);

    uint32 rangeCount = ToU32(cullingContext.ranges.size());
    uint32 materialCount = matMeshes.GetMaterialBatchSize();
    uint32 meshCount = matMeshes.GetMeshBatchSize();
    uint32 batchCount = materialCount * meshCount;
    ASSERT(drawingLayout.firstInstances.size() == batchCount);

    output->instanceCounts.resize(batchCount);
    output->remappedEntityIDs.resize(drawingLayout.maxInstanceCount);
    output->visibleRendererCount = 0;

    for (uint32 r = 0; r < rangeCount; ++r)
    {
        CullRendererRangeJob& job = jobScheduler.CreateJob<CullRendererRangeJob>();
        job.context = cullingContext;
        job.output = output;
        job.rangeIndex = r;
        job.signal = signal;

        signal->fetch_add(1);
        if (threadedCulling)
        {
            jobScheduler.Submit(job);
        }
        else
        {
            jobScheduler.Run(job);
        }
    }
}

static CullingContext CreateShadowCullingContext(const RenderContext& context, const MatMeshQuery& matMeshQuery, const InstanceDrawingLayout& drawingLayout, const PointLightCullingSlice& light, uint32 wantedSizePerRange)
{
    ea::vector<CullingRange> cullingRanges = CreateCullingRanges(context, light.pvsCache, CullingType::eShadows, wantedSizePerRange);

    float3 position = light.position;
    float shadowRange = light.shadowFar;

    ea::array<float4, 6> frustrumPlanes
    {
        float4(1, 0, 0, position.x + shadowRange),
        float4(-1, 0, 0, -(position.x - shadowRange)),

        float4(0, 1, 0, position.y + shadowRange),
        float4(0, -1, 0, -(position.y - shadowRange)),

        float4(0, 0, 1, position.z + shadowRange),
        float4(0, 0, -1, -(position.z - shadowRange)),
    };

    CullingContext cullingContext = {};
    cullingContext.frustrumPlanes = BuildFrustrumPlanesSOA(frustrumPlanes);
    cullingContext.ranges = std::move(cullingRanges);
    cullingContext.renderAsset = context.renderAssets;
    cullingContext.matMeshes = &matMeshQuery;
    cullingContext.drawingLayout = &drawingLayout;

    return cullingContext;
}

static void KickPointLightShadowCastersCulling(JobScheduler& jobScheduler, 
    const RenderContext& context, 
    const PointLightCulling& lightCulling, 
    const MatMeshMetadataCache& cache, 
    uint32 wantedSizePerRange,
    ea::span<CullingOutput> outputs,
    std::atomic_int32_t* outSignal)
{
    bool threadedCulling = context.global->threadedCulling;
    const MatMeshQuery& matMeshes = cache.pointShadowMatMeshQuery;
    const InstanceDrawingLayout& drawingLayout = cache.pointShadowDrawingLayout;
    const uint32 lightCount = lightCulling.shadowLightCount;
    uint32 materialCount = matMeshes.GetMaterialBatchSize();
    uint32 meshCount = matMeshes.GetMeshBatchSize();
    uint32 batchCount = materialCount * meshCount;
    ASSERT(drawingLayout.firstInstances.size() == batchCount);

    for (uint32 l = 0; l < lightCount; ++l)
    {
        uint32 flatIndex = lightCulling.flatIndices[l];
        const PVSDataCache* pvsCache = context.pointLightPVSCaches[flatIndex];

        PointLightCullingSlice lightSlice = {};
        lightSlice.position = float3(&lightCulling.positionAndRanges[l].x);
        lightSlice.shadowFar = lightCulling.shadowParams[l].far;
        lightSlice.pvsCache = context.global->usePVS ? pvsCache : nullptr;

        CullingContext cullingContext = CreateShadowCullingContext(context, matMeshes, drawingLayout, lightSlice, wantedSizePerRange);

        CullingOutput& output = outputs[l];
        output.instanceCounts.resize(batchCount);
        output.remappedEntityIDs.resize(drawingLayout.maxInstanceCount);
        output.visibleRendererCount = 0;

        uint32 rangeCount = ToU32(cullingContext.ranges.size());
        for (uint32 r = 0; r < rangeCount; ++r)
        {
            CullRendererRangeJob& job = jobScheduler.CreateJob<CullRendererRangeJob>();
            job.context = cullingContext;
            job.output = &output;
            job.rangeIndex = r;
            job.signal = outSignal;

            outSignal->fetch_add(1);
            if (threadedCulling)
            {
                jobScheduler.Submit(job);
            }
            else
            {
                jobScheduler.Run(job);
            }
        }
    }
}

static CullingContext CreateDirectionalShadowCullingContext(const RenderContext& context, const MatMeshQuery& matMeshQuery, const InstanceDrawingLayout& drawingLayout, uint32 wantedSizePerRange)
{
    ea::vector<CullingRange> cullingRanges = CreateCullingRanges(context, nullptr, CullingType::eShadows, wantedSizePerRange);

    Matrix4x4 cullingMatrix = context.sunShadowsViewMatrix * context.sunShadowsProjMatrix;
    ea::array<float4, 6> frustrumPlanes = ExtractCullingPlanes(cullingMatrix);

    CullingContext cullingContext = {};
    cullingContext.frustrumPlanes = BuildFrustrumPlanesSOA(frustrumPlanes);
    cullingContext.ranges = std::move(cullingRanges);
    cullingContext.renderAsset = context.renderAssets;
    cullingContext.matMeshes = &matMeshQuery;
    cullingContext.drawingLayout = &drawingLayout;

    return cullingContext;
}

static void KicklDirectionalLightShadowCastersCulling(JobScheduler& jobScheduler,
    const RenderContext& context,
    const MatMeshMetadataCache& cache,
    uint32 wantedSizePerRange,
    CullingOutput* output,
    std::atomic_int32_t* signal)
{
    bool threadedCulling = context.global->threadedCulling;
    const MatMeshQuery& matMeshes = cache.sunShadowMatMeshQuery;
    const InstanceDrawingLayout& drawingLayout = cache.sunShadowDrawingLayout;
    uint32 materialCount = matMeshes.GetMaterialBatchSize();
    uint32 meshCount = matMeshes.GetMeshBatchSize();
    uint32 batchCount = materialCount * meshCount;
    ASSERT(drawingLayout.firstInstances.size() == batchCount);

    CullingContext cullingContext = CreateDirectionalShadowCullingContext(context, matMeshes, drawingLayout, wantedSizePerRange);

    output->instanceCounts.resize(batchCount);
    output->remappedEntityIDs.resize(drawingLayout.maxInstanceCount);
    output->visibleRendererCount = 0;

    uint32 rangeCount = ToU32(cullingContext.ranges.size());
    for (uint32 r = 0; r < rangeCount; ++r)
    {
        CullRendererRangeJob& job = jobScheduler.CreateJob<CullRendererRangeJob>();
        job.context = cullingContext;
        job.output = output;
        job.rangeIndex = r;
        job.signal = signal;

        signal->fetch_add(1);
        if (threadedCulling)
        {
            jobScheduler.Submit(job);
        }
        else
        {
            jobScheduler.Run(job);
        }
    }
}

FrustrumPlanesSOA Gyn::BuildFrustrumPlanesSOA(const ea::array<float4, 6>& planes)
{
    FrustrumPlanesSOA soa;
    soa.px0 = v4Set(planes[0].x, planes[1].x, planes[2].x, planes[3].x);
    soa.px1 = v4Set(planes[4].x, planes[5].x, 1, 1);

    soa.py0 = v4Set(planes[0].y, planes[1].y, planes[2].y, planes[3].y);
    soa.py1 = v4Set(planes[4].y, planes[5].y, 0, 0);

    soa.pz0 = v4Set(planes[0].z, planes[1].z, planes[2].z, planes[3].z);
    soa.pz1 = v4Set(planes[4].z, planes[5].z, 0, 0);

    soa.pw0 = v4Set(planes[0].w, planes[1].w, planes[2].w, planes[3].w);
    soa.pw1 = v4Set(planes[4].w, planes[5].w, 1e9f, 1e9f);

    return soa;
}

ea::array<float4, 6> Gyn::ExtractCullingPlanes(const Matrix4x4& m)
{
    // Extract culling planes using Gribb & Hartmann method

    ea::array<float4, 6> planes{};

    // Left clipping plane
    planes[0] = float4(-(m(0, 3) + m(0, 0)), -(m(1, 3) + m(1, 0)), -(m(2, 3) + m(2, 0)), m(3, 3) + m(3, 0));

    // Right clipping plane
    planes[1] = float4(-(m(0, 3) - m(0, 0)), -(m(1, 3) - m(1, 0)), -(m(2, 3) - m(2, 0)), (m(3, 3) - m(3, 0)));

    // Top clipping plane
    planes[2] = float4(-(m(0, 3) - m(0, 1)), -(m(1, 3) - m(1, 1)), -(m(2, 3) - m(2, 1)), (m(3, 3) - m(3, 1)));

    // Bottom clipping plane
    planes[3] = float4(-(m(0, 3) + m(0, 1)), -(m(1, 3) + m(1, 1)), -(m(2, 3) + m(2, 1)), m(3, 3) + m(3, 1));

    // Near clipping plane
    planes[4] = float4(-m(0, 2), -m(1, 2), -m(2, 2), m(3, 2));

    // Far clipping plane
    planes[5] = float4(m(0, 2) - m(0, 3), m(1, 2) - m(1, 3), m(2, 2) - m(2, 3), -(m(3, 2) - m(3, 3)));

    return planes;
}

FrameCulling Gyn::CullScene(JobScheduler& jobScheduler, const RenderContext& context, const MatMeshMetadataCache& matMeshCache, uint32 wantedSizePerRange)
{
    std::atomic_int32_t fence = 0;

    CullingOutput cameraCullingOutput = {};
    KickCameraFrustumCulling(jobScheduler, 
        context, 
        matMeshCache, 
        wantedSizePerRange, 
        &cameraCullingOutput, 
        &fence);

    CullingOutput sunShadowCullingOutput = {};
    KicklDirectionalLightShadowCastersCulling(jobScheduler, 
        context, 
        matMeshCache, 
        wantedSizePerRange, 
        &sunShadowCullingOutput, 
        &fence);

    // TODO: Point light culling is main thread. Kick this as a separate job and make point light shadow caster culling depend on it.
    PointLightCulling pointLightCulling = CullPointLights(context);

    ea::vector<CullingOutput> pointLightShadowCullingOutputs = {};
    pointLightShadowCullingOutputs.resize(pointLightCulling.shadowLightCount);
    KickPointLightShadowCastersCulling(jobScheduler,
        context, 
        pointLightCulling, 
        matMeshCache, 
        wantedSizePerRange, 
        pointLightShadowCullingOutputs, 
        &fence);

    jobScheduler.Wait(fence);

    FrameCulling culling = {};
    culling.pointLights = std::move(pointLightCulling);
    culling.gbuffer = std::move(cameraCullingOutput);
    culling.pointShadows = std::move(pointLightShadowCullingOutputs);
    culling.sunShadows = std::move(sunShadowCullingOutput);

    return culling;
}