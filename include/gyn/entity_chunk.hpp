#pragma once

#include <gyn/common.hpp>
#include <gyn/entity_archetype.hpp>

namespace Gyn
{
    class EntityChunk : NonCopyable
    {
    public:

        EntityChunk() = default;

        EntityChunk(const EntityArchetype& archetype);

        bool IsNull() const;

        uint32 CreateEntity();

        uint32 GetEntityCount() const;

        void SetComponent(uint32 index, TypeID type, const byte* data);

        ea::span<const byte> GetComponent(uint32 index, TypeID type) const;

        void SetAbstractComponent(uint32 index, TypeID type, UniquePtr<ECSAbstractComponent>&& component);

        const ECSAbstractComponent* GetAbstractComponent(uint32 index, TypeID type) const;

        ECSAbstractComponent* GetMutableAbstractComponent(uint32 index, TypeID type);

        const EntityArchetype& GetArchetype() const;

        bool HasComponentLane(TypeID type) const;

        bool HasAbstractComponentLane(TypeID type) const;

        ComponentLaneVersion GetLaneVersion(TypeID type) const;

        template <typename T>
        ComponentLaneVersion GetLaneVersion() const;

        byte* GetMutableComponentLane(TypeID type);

        const byte* GetComponentLane(TypeID type) const;

        template <typename T>
        const T* GetComponentLane() const;

        template <typename T>
        T* GetMutableComponentLane();

        UniquePtr<ECSAbstractComponent>* GetMutableAbstractComponentLane(TypeID type);

        const UniquePtr<const ECSAbstractComponent>* GetAbstractComponentLane(TypeID type) const;

        template <typename T>
        const UniquePtr<const T>* GetAbstractComponentLane() const;

        template <typename T>
        UniquePtr<T>* GetMutableAbstractComponentLane();

    private:

        struct Lane
        {
            ea::vector<byte> bytes;
            uint64 version;
        };

        struct AbstractLane : NonCopyable
        {
            ea::vector<UniquePtr<ECSAbstractComponent>> ptrs;
        };

        void AssertNotNull() const;

        void AssertHasComponentLane(TypeID type) const;

        EntityArchetype m_archetype = {};
        ea::hash_map<TypeID, Lane> m_lanes = {};
        ea::hash_map<TypeID, AbstractLane> m_abstractLanes = {};
        uint32 m_entityCount = {};
    };

    template <typename T>
    ComponentLaneVersion EntityChunk::GetLaneVersion() const
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);
        AssertNotNull();

        return GetLaneVersion(GetTypeID<T>());
    }

    template <typename T>
    const T* EntityChunk::GetComponentLane() const
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);
        AssertNotNull();

        return reinterpret_cast<const T*>(GetComponentLane(GetTypeID<T>()));
    }

    template <typename T>
    T* EntityChunk::GetMutableComponentLane()
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);
        AssertNotNull();

        return reinterpret_cast<T*>(GetMutableComponentLane(GetTypeID<T>()));
    }

    template <typename T>
    const UniquePtr<const T>* EntityChunk::GetAbstractComponentLane() const
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        AssertNotNull();

        return reinterpret_cast<const UniquePtr<const T>*>(GetAbstractComponentLane(GetTypeID<T>()));
    }

    template <typename T>
    UniquePtr<T>* EntityChunk::GetMutableAbstractComponentLane()
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        AssertNotNull();

        return reinterpret_cast<UniquePtr<T>*>(GetMutableAbstractComponentLane(GetTypeID<T>()));
    }
}