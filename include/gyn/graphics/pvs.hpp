#pragma once

#include <gyn/graphics/common.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/point_light.hpp>

namespace Gyn
{
    struct PVSEntityID : public ECSComponent
    {
        PVSEntityID() = default;

        PVSEntityID(EntityID id) : val(id) {}

        EntityID val;
    };

    struct PVSBounds : public ECSComponent
    {
        PVSBounds() = default;

        PVSBounds(const AABB& aabb) : val(aabb) {}

        AABB val;
    };

    struct PVSDynamicData : public ECSAbstractComponent
    {
        ea::vector<EntityID> pointLightIDs;
        ea::vector<EntityID> rendererIDs;
        ea::vector<EntityID> visiblePVSIDs;

        ea::vector<EntityID> combinedPointLightIDs;
        ea::vector<const ea::vector<EntityID>*> combinedRendererIDs;
        uint32 combinedRendererCount;
    };

    struct PVSDataCache : public ECSAbstractComponent
    {
        ea::vector<ModelID> modelIDs;
        ea::vector<uint32> entityGPUIndices;
        ea::vector<AABB> entityWorldBounds;
        ea::vector<bool> entityDisableShadows;

        ea::vector<PointLight> pointLightDatas;
        ea::vector<float3> pointLightPositions;
        ea::vector<EntityID> pointLightPVSIDs;
    };

    ea::span<const TypeID> GetPVSComponentTypes();

    ea::vector<const ea::vector<EntityID>*> GetCombinedPVSVisibleRenderers(const EntityManager& entityManager, const  PVSDynamicData& pvsData);

    uint32 GetCombinedPVSVisibleRendererCount(const ea::vector<const ea::vector<EntityID>*>& renderers);

    ea::vector<EntityID> GetCombinedPVSVisiblePointLights(const EntityManager& entityManager, const PVSDynamicData& pvsData);

    PVSDataCache BuildPVSDataCache(const EntityManager& entityManager, 
        ea::span<const ea::vector<EntityID> *const> rendererIDSlices, 
        usize rendererIDCount, 
        ea::span<const EntityID> pointLightIDs);

    PVSDataCache BuildPVSDataCache(const EntityManager& entityManager, const PVSDynamicData& data);

    void UpdateEntityPVSIDs(World& world);
}