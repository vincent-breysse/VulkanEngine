#pragma once

#include <gyn/common.hpp>

namespace Gyn
{
    class EntityArchetype
    {
    public:

        EntityArchetype() = default;

        EntityArchetype(ea::span<const TypeID> types);

        bool IsNull() const;

        EntityArchetypeID GetID() const;

        ea::span<const TypeID> GetTypes() const;

        bool HasType(TypeID type) const;

        bool HasAllTypes(ea::span<const TypeID> types) const;

        bool HasExactTypes(ea::span<const TypeID> types) const;

    private:

        void AssertNotNull() const;

        EntityArchetypeID m_id = EntityArchetypeNullID;
        ea::vector<TypeID> m_types = {};
    };
}