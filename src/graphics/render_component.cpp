
#include <pch.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/render_component.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/model.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/pvs.hpp>

RenderComponent::RenderComponent(MatMeshRefCountManager& refCountManager, SharedPtr<const Model> model)
{
    auto subMaterials = model->GetSubMaterials();
    auto subMeshes = model->GetSubMeshes();

    ASSERT(subMaterials.size() == subMeshes.size());
    const usize count = subMeshes.size();

    ea::vector<MatMeshRefCounter> refCounters;
    refCounters.resize(count);

    for (usize i = 0; i < count; ++i)
    {
        refCounters[i] = MatMeshRefCounter(refCountManager, subMaterials[i]->GetID(), subMeshes[i]->GetID());
    }

    m_model = std::move(model);
    m_refCounters = std::move(refCounters);
}

bool RenderComponent::IsNull() const
{
    return m_refCounters.empty();
}

const Model& RenderComponent::GetModel() const
{
    AssertNotNull();
    return *m_model;
}

void RenderComponent::AssertNotNull() const
{
    ASSERT(!IsNull());
}

UniquePtr<RenderComponent> Gyn::CreateRenderComponent(MatMeshRefCountManager& refCountManager, SharedPtr<const Model> model)
{
    return ea::make_unique<RenderComponent>(refCountManager, std::move(model));
}

Entity Gyn::CreateRenderEntity(EntityManager& entityManager, 
    MatMeshRefCountManager& matMeshRefCounts,
    SharedPtr<const Model> model, 
    bool castShadows, 
    float3 position, 
    Quaternion rotation, 
    float3 scale)
{
    const ea::array ComponentTypes
    {
        GetTypeID<Position>(),
        GetTypeID<Rotation>(),
        GetTypeID<Scale>(),
        GetTypeID<LocalToWorld>(),
        GetTypeID<RenderComponent>(),
        GetTypeID<EntityGPUBufferIndex>(),
        GetTypeID<EntityModelID>(),
        GetTypeID<LocalRenderBounds>(),
        GetTypeID<WorldRenderBounds>(),
        GetTypeID<DisableShadow>(),
    };

    Entity entity = entityManager.CreateEntity(ComponentTypes);
    entity.SetComponent<DisableShadow>(DisableShadow(!castShadows));
    entity.SetComponent<LocalRenderBounds>(LocalRenderBounds(model->GetBounds()));
    entity.SetAbstractComponent<RenderComponent>(CreateRenderComponent(matMeshRefCounts, std::move(model)));

    Transform transform(entity);
    transform.SetPosition(position);
    transform.SetRotation(rotation);
    transform.SetScale(scale);

    return entity;
}

Entity Gyn::CreateLightRenderEntity(EntityManager& entityManager, 
    MatMeshRefCountManager& matMeshRefCounts,
    SharedPtr<const Model> model, 
    bool castShadows, 
    float3 position, 
    Quaternion rotation, 
    float3 scale, 
    const PointLight& light)
{
    const ea::array ComponentTypes
    {
        GetTypeID<Position>(),
        GetTypeID<Rotation>(),
        GetTypeID<Scale>(),
        GetTypeID<LocalToWorld>(),
        GetTypeID<RenderComponent>(),
        GetTypeID<EntityGPUBufferIndex>(),
        GetTypeID<EntityModelID>(),
        GetTypeID<LocalRenderBounds>(),
        GetTypeID<WorldRenderBounds>(),
        GetTypeID<DisableShadow>(),
        GetTypeID<PointLight>(),
        GetTypeID<PVSEntityID>(),
    };

    Entity entity = entityManager.CreateEntity(ComponentTypes);
    entity.SetComponent<DisableShadow>(DisableShadow(!castShadows));
    entity.SetComponent<LocalRenderBounds>(LocalRenderBounds(model->GetBounds()));
    entity.SetAbstractComponent<RenderComponent>(CreateRenderComponent(matMeshRefCounts, std::move(model)));
    entity.SetComponent(light);
    entity.SetComponent(PVSEntityID(EntityNullID));

    Transform transform(entity);
    transform.SetPosition(position);
    transform.SetRotation(rotation);
    transform.SetScale(scale);

    return entity;
}