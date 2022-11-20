#pragma once

#include <gyn/graphics/common.hpp>
#include <gyn/entity_manager.hpp>
#include <gyn/graphics/mat_mesh_ref_count.hpp>

namespace Gyn
{
    class RenderComponent : public ECSAbstractComponent
    {
    public:

        RenderComponent() = default;

        RenderComponent(MatMeshRefCountManager& refCountManager, SharedPtr<const Model> model);

        bool IsNull() const;

        const Model& GetModel() const;

    private:

        void AssertNotNull() const;

        SharedPtr<const Model> m_model = {};
        ea::vector<MatMeshRefCounter> m_refCounters = {};
    };

    UniquePtr<RenderComponent> CreateRenderComponent(MatMeshRefCountManager& refCountManager, SharedPtr<const Model> model);

    Entity CreateRenderEntity(EntityManager& entityManager, 
        MatMeshRefCountManager& matMeshRefCounts,
        SharedPtr<const Model> model, 
        bool castShadows, 
        float3 position, 
        Quaternion rotation, 
        float3 scale);

    Entity CreateLightRenderEntity(EntityManager& entityManager, 
        MatMeshRefCountManager& matMeshRefCounts,
        SharedPtr<const Model> model, 
        bool castShadows, 
        float3 position, 
        Quaternion rotation, 
        float3 scale,
        const PointLight& light);
}