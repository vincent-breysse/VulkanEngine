
#include <pch.hpp>
#include <gyn/world.hpp>
#include <gyn/entity.hpp>
#include <gyn/entity_manager.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/pvs.hpp>
#include <gyn/graphics/render_component.hpp>
#include <gyn/graphics/model.hpp>
#include <gyn/graphics/point_light.hpp>

ea::span<const TypeID> Gyn::GetPVSComponentTypes()
{
    static const ea::array TypeIDs
    {
        GetTypeID<EntityIDComponent>(),
        GetTypeID<PVSBounds>(),
        GetTypeID<PVSDynamicData>(),
        GetTypeID<PVSDataCache>(),
    };

    return TypeIDs;
}

ea::vector<const ea::vector<EntityID>*> Gyn::GetCombinedPVSVisibleRenderers(const EntityManager& entityManager, const PVSDynamicData& pvsData)
{
    ea::vector<const ea::vector<EntityID>*> combinedRenderers{};
    combinedRenderers.reserve(pvsData.visiblePVSIDs.size() + 1);

    combinedRenderers.push_back(&pvsData.rendererIDs);

    for (EntityID pvsID : pvsData.visiblePVSIDs)
    {
        const PVSDynamicData* visiblePVSData = entityManager.GetAbstractComponent<PVSDynamicData>(pvsID);
        combinedRenderers.push_back(&visiblePVSData->rendererIDs);
    }

    combinedRenderers.shrink_to_fit();
    return combinedRenderers;
}

uint32 Gyn::GetCombinedPVSVisibleRendererCount(const ea::vector<const ea::vector<EntityID>*>& renderers)
{
    usize count = 0;

    for (usize i = 0; i < renderers.size(); ++i)
    {
        count += renderers[i]->size();
    }

    return ToU32(count);
}

ea::vector<EntityID> Gyn::GetCombinedPVSVisiblePointLights(const EntityManager& entityManager, const PVSDynamicData& pvsData)
{
    usize allocSize = pvsData.pointLightIDs.size();

    for (EntityID pvsID : pvsData.visiblePVSIDs)
    {
        const PVSDynamicData* visiblePVSData = entityManager.GetAbstractComponent<PVSDynamicData>(pvsID);
        allocSize += visiblePVSData->pointLightIDs.size();
    }

    ea::vector_set<EntityID> set{};
    set.reserve(allocSize);

    set.insert(pvsData.pointLightIDs.begin(), pvsData.pointLightIDs.end());

    for (EntityID pvsID : pvsData.visiblePVSIDs)
    {
        const PVSDynamicData* visiblePVSData = entityManager.GetAbstractComponent<PVSDynamicData>(pvsID);
        set.insert(visiblePVSData->pointLightIDs.begin(), visiblePVSData->pointLightIDs.end());
    }

    set.shrink_to_fit();
    return set;
}

PVSDataCache Gyn::BuildPVSDataCache(const EntityManager& entityManager, 
    ea::span<const ea::vector<EntityID>* const> rendererIDSlices, 
    usize rendererIDCount, 
    ea::span<const EntityID> pointLightIDs)
{
    ea::vector<uint32> entityGPUIndices{};
    entityGPUIndices.resize(rendererIDCount);

    ea::vector<ModelID> entityModelIDs{};
    entityModelIDs.resize(rendererIDCount);

    ea::vector<AABB> entityWorldBounds{};
    entityWorldBounds.resize(rendererIDCount);

    ea::vector<bool> entityDisableShadows{};
    entityDisableShadows.resize(rendererIDCount);

    for (usize s = 0, i = 0; s < rendererIDSlices.size(); ++s)
    {
        const ea::vector<EntityID>& slice = *rendererIDSlices[s];

        for (usize k = 0; k < slice.size(); ++k, ++i)
        {
            const EntityID entityID = slice[k];

            entityGPUIndices[i] = entityManager.GetComponent<EntityGPUBufferIndex>(entityID).val;
            entityModelIDs[i] = entityManager.GetComponent<EntityModelID>(entityID).val;
            entityWorldBounds[i] = entityManager.GetComponent<WorldRenderBounds>(entityID).val;
            entityDisableShadows[i] = entityManager.GetComponent<DisableShadow>(entityID).val;
        }
    }

    ea::vector<EntityID> pointLightPVSIDs{};
    pointLightPVSIDs.resize(pointLightIDs.size());

    ea::vector<PointLight> pointLightDatas{};
    pointLightDatas.resize(pointLightIDs.size());

    ea::vector<float3> pointLightPositions{};
    pointLightPositions.resize(pointLightIDs.size());

    for (usize i = 0; i < pointLightIDs.size(); ++i)
    {
        const EntityID entityID = pointLightIDs[i];

        pointLightPVSIDs[i] = entityManager.GetComponent<PVSEntityID>(entityID).val;
        pointLightDatas[i] = entityManager.GetComponent<PointLight>(entityID);
        pointLightPositions[i] = entityManager.GetComponent<Position>(entityID).val;
    }

    PVSDataCache cache{};
    cache.entityGPUIndices = std::move(entityGPUIndices);
    cache.modelIDs = std::move(entityModelIDs);
    cache.entityWorldBounds = std::move(entityWorldBounds);
    cache.entityDisableShadows = std::move(entityDisableShadows);

    cache.pointLightDatas = std::move(pointLightDatas);
    cache.pointLightPositions = std::move(pointLightPositions);
    cache.pointLightPVSIDs = std::move(pointLightPVSIDs);

    return cache;
}

PVSDataCache Gyn::BuildPVSDataCache(const EntityManager& entityManager, const PVSDynamicData& data)
{
    return BuildPVSDataCache(entityManager, data.combinedRendererIDs, data.combinedRendererCount, data.combinedPointLightIDs);
}

static bool AABBContains(const AABB& aabb, float3 point)
{
    vfloat4 vCenter = vLoad(&aabb.center);
    vfloat4 vExtents = vLoad(&aabb.extents);
    vfloat4 vPoint = vLoad(&point);

    return v4AllInBoundsAbs(v4Sub(vPoint, vCenter), vExtents);
}

static PVSEntityID FindPVSEntityID(ea::span<const EntityChunk* const> pvsChunks, float3 position)
{
    for (const EntityChunk* chunk : pvsChunks)
    {
        uint32 count = chunk->GetEntityCount();
        const PVSBounds* boundsLane = chunk->GetComponentLane<PVSBounds>();
        const EntityIDComponent* entityIDs = chunk->GetComponentLane<EntityIDComponent>();

        for (uint32 i = 0; i < count; ++i)
        {
            const AABB& bounds = boundsLane[i].val;

            if (AABBContains(bounds, position))
            {
                return PVSEntityID(entityIDs[i].val);
            }
        }
    }

    return PVSEntityID(EntityNullID);
}

void Gyn::UpdateEntityPVSIDs(World& world)
{
    const ea::array PVSComponentTypes
    {
        GetTypeID<EntityIDComponent>(),
        GetTypeID<PVSBounds>(),
    };

    const ea::array PVSChildComponentTypes
    {
        GetTypeID<Position>(),
        GetTypeID<PVSEntityID>(),
    };

    ea::vector<EntityChunk*> pvsChunks = world.entityManager.QueryEntityChunks(PVSComponentTypes);
    ea::vector<EntityChunk*> pvsChildChunks = world.entityManager.QueryEntityChunks(PVSChildComponentTypes);

    for (EntityChunk* pvsChildChunk : pvsChildChunks)
    {
        const Position* positions = pvsChildChunk->GetComponentLane<Position>();
        PVSEntityID* entityIDs = pvsChildChunk->GetMutableComponentLane<PVSEntityID>();

        for (uint32 i = 0; i < pvsChildChunk->GetEntityCount(); ++i)
        {
            entityIDs[i] = FindPVSEntityID(pvsChunks, positions[i].val);
        }
    }
}