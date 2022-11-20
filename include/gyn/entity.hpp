#pragma once

#include <gyn/common.hpp>
#include <gyn/entity_manager.hpp>

namespace Gyn
{
    class Entity
    {
    public:

        Entity() = default;

        Entity(EntityManager& manager, EntityID id);

        bool IsNull() const;

        EntityArchetype GetArchetype() const;

        template <typename T>
        void SetComponent(const T& component);

        template <typename T>
        T GetComponent() const;

        template <typename T>
        void SetAbstractComponent(UniquePtr<T>&& component);

        template <typename T>
        const T* GetAbstractComponent() const;

        template <typename T>
        T* GetMutableAbstractComponent();

        EntityID GetID() const;

        EntityManager& GetEntityManager();

        const EntityManager& GetEntityManager() const;

    private:

        void AssertNotNull() const;

        EntityID m_id = EntityNullID;
        EntityManager* m_manager = {};
    };

    template <typename T>
    void Entity::SetComponent(const T& component)
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);

        AssertNotNull();
        m_manager->SetComponent<T>(m_id, component);
    }

    template <typename T>
    T Entity::GetComponent() const
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);
        AssertNotNull();

        return m_manager->GetComponent<T>(m_id);
    }

    template <typename T>
    void Entity::SetAbstractComponent(UniquePtr<T>&& component)
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);

        AssertNotNull();
        m_manager->SetAbstractComponent<T>(m_id, std::move(component));
    }

    template <typename T>
    const T* Entity::GetAbstractComponent() const
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        AssertNotNull();

        return m_manager->GetAbstractComponent<T>(m_id);
    }

    template <typename T>
    T* Entity::GetMutableAbstractComponent()
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        AssertNotNull();

        return m_manager->GetMutableAbstractComponent<T>(m_id);
    }
}