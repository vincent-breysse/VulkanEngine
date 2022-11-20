
#include <pch.hpp>
#include <gyn/entity_archetype.hpp>

static EntityArchetypeID CreateUniqueArchetypeID()
{
    static std::atomic<uint64> id = 1;
    return EntityArchetypeID(id++);
}

EntityArchetype::EntityArchetype(ea::span<const TypeID> types)
{
    m_id = CreateUniqueArchetypeID();
    m_types = CopyToVector(types);
}

bool EntityArchetype::IsNull() const
{
    return ::IsNull(m_id);
}

EntityArchetypeID EntityArchetype::GetID() const
{
    AssertNotNull();
    return m_id;
}

bool EntityArchetype::HasType(TypeID type) const
{
    AssertNotNull();
    for (TypeID myID : m_types)
    {
        if (myID == type) return true;
    }

    return false;
}

ea::span<const TypeID> EntityArchetype::GetTypes() const
{
    AssertNotNull();
    return m_types;
}

bool EntityArchetype::HasAllTypes(ea::span<const TypeID> types) const
{
    AssertNotNull();

    const usize myCount = GetTypes().size();
    const usize count = types.size();

    if (count > myCount) return false;

    for (usize i = 0; i < count; ++i)
    {
        if (!HasType(types[i])) return false;
    }

    return true;
}

bool EntityArchetype::HasExactTypes(ea::span<const TypeID> types) const
{
    AssertNotNull();

    ea::span<const TypeID> myTypes = GetTypes();
    const usize count0 = myTypes.size();
    const usize count1 = types.size();

    if (count0 != count1) return false;

    for (usize i = 0; i < count0; ++i)
    {
        if (myTypes[i] != types[i]) return false;
    }

    return true;
}

void EntityArchetype::AssertNotNull() const
{
    ASSERT(!IsNull());
}