
#include <pch.hpp>
#include <gyn/entity_archetype.hpp>
#include <gyn/entity_manager.hpp>
#include <gyn/entity.hpp>

Entity::Entity(EntityManager& manager, EntityID id)
{
    ::AssertNotNull(id);

    m_manager = &manager;
    m_id = id;
}

bool Entity::IsNull() const
{
    return ::IsNull(m_id);
}

EntityArchetype Entity::GetArchetype() const
{
    AssertNotNull();
    return m_manager->GetEntityArchetype(m_id);
}

EntityID Entity::GetID() const
{
    AssertNotNull();
    return m_id;
}

EntityManager& Entity::GetEntityManager()
{
    AssertNotNull();
    return *m_manager;
}

const EntityManager& Entity::GetEntityManager() const
{
    AssertNotNull();
    return *m_manager;
}

void Entity::AssertNotNull() const
{
    ASSERT(!IsNull());
}