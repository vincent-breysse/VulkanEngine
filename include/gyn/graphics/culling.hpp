#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    struct PointLightCulling
    {
        ea::vector<uint32> flatIndices;
        ea::vector<float4> positionAndRanges;
        ea::vector<float4> colorAndIntensities;
        ea::vector<ShadowMatrixUniforms> shadowMatrices;
        ea::vector<ShadowParamUniforms> shadowParams;
        uint32 lightCount;
        uint32 shadowLightCount;
    };

    struct CullingOutput
    {
        ea::vector<uint32> instanceCounts;
        ea::vector<uint32> remappedEntityIDs;
        uint32 visibleRendererCount;
    };

    struct FrameCulling
    {
        PointLightCulling pointLights;
        CullingOutput gbuffer;
        ea::vector<CullingOutput> pointShadows;
        CullingOutput sunShadows;
    };

    FrustrumPlanesSOA BuildFrustrumPlanesSOA(const ea::array<float4, 6>& planes);

    ea::array<float4, 6> ExtractCullingPlanes(const Matrix4x4& m);

    FrameCulling CullScene(JobScheduler& jobScheduler, const RenderContext& context, const MatMeshMetadataCache& matMeshCache, uint32 wantedSizePerRange);
}