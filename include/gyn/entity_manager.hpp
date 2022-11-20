#pragma once

#include <gyn/common.hpp>
#include <gyn/entity_chunk.hpp>

namespace Gyn
{
    class EntityManager : NonCopyable
    {
    public:

        EntityManager() = default;

        EntityManager(World& world);

        bool IsNull() const;
        
        EntityArchetype CreateArchetype(ea::span<const TypeID> componentTypes);

        Entity CreateEntity(const EntityArchetype& archetype);

        Entity CreateEntity(ea::span<const TypeID> componentTypes);

        const EntityChunk& GetEntityChunk(EntityArchetypeID id) const;

        EntityChunk& GetEntityChunk(EntityArchetypeID id);

        const EntityChunk& GetEntityChunk(EntityID id) const;

        EntityChunk& GetEntityChunk(EntityID id);

        EntityMetadata GetEntityMetadata(EntityID id) const;

        EntityArchetypeID GetEntityArchetypeID(EntityID id) const;

        uint32 GetEntitChunkLocalIndex(EntityID id) const;

        EntityArchetype GetEntityArchetype(EntityID id);

        EntityID GetEntityID(const EntityMetadata& metadata) const;

        EntityID GetEntityID(const EntityChunk& chunk, uint32 chunkLocalIndex) const;

        ea::vector<const EntityChunk*> QueryEntityChunks(ea::span<const TypeID> all) const;

        ea::vector<EntityChunk*> QueryEntityChunks(ea::span<const TypeID> all);

        Entity GetFirstEnity(EntityChunk& chunk);

        Entity GetFirstEnity(ea::span<const TypeID> all);

        Entity GetTheEntity(ea::span<const TypeID> all);

        template <typename T>
        ea::vector<ea::span<const T>> QueryComponentLanes(ea::span<const TypeID> all) const;

        template <typename T>
        void SetComponent(EntityID entityID, const T& component);

        template <typename T>
        T GetComponent(EntityID entityID) const;

        template <typename T>
        void SetAbstractComponent(EntityID entityID, UniquePtr<T>&& component);

        template <typename T>
        const T* GetAbstractComponent(EntityID entityID) const;

        template <typename T>
        T* GetMutableAbstractComponent(EntityID entityID);

    private:

        bool HasMatchingArchetype(ea::span<const TypeID> componentTypes, uint32& outIndex) const;

        void SetComponent(EntityID entityID, TypeID type, const byte* data);

        ea::span<const byte> GetComponent(EntityID entityID, TypeID info) const;

        void SetAbstractComponent(EntityID entityID, TypeID type, UniquePtr<ECSAbstractComponent>&& component);

        const ECSAbstractComponent* GetAbstractComponent(EntityID entityID, TypeID type) const;

        ECSAbstractComponent* GetMutableAbstractComponent(EntityID entityID, TypeID type);

        EntityChunk& GetOrCreateChunk(const EntityArchetype& archetype);

        bool HasAssociatedEntityChunk(EntityArchetypeID id) const;

        bool HasAssociatedEntityChunk(EntityID id) const;

        bool HasAssociatedMetadata(EntityID id) const;

        void AssertNotNull() const;

        void AssertHasAssociatedEntityChunk(EntityArchetypeID id) const;

        void AssertHasAssociatedEntityChunk(EntityID id) const;

        void AssertHasAssociatedMetadata(EntityID  id) const;

        World* m_world = {};
        ea::vector<EntityArchetype> m_archetypes = {};
        ea::hash_map<EntityArchetypeID, UniquePtr<EntityChunk>> m_chunks = {};
        ea::hash_map<EntityID, EntityMetadata> m_entityMetadata = {};
    };

    template <typename T>
    ea::vector<ea::span<const T>> EntityManager::QueryComponentLanes(ea::span<const TypeID> all) const
    {
        AssertNotNull();

        ea::vector<const EntityChunk*> chunks = QueryEntityChunks(all);

        ea::vector<ea::span<const T>> lanes{};
        lanes.reserve(chunks.size());

        for (const EntityChunk* chunk : chunks)
        {
            ea::span<const byte> byteLane = chunk->GetComponentLane(GetTypeID<T>());

            lanes.push_back(SpanCast<const T>(byteLane));
        }

        return lanes;
    }

    template <typename T>
    void EntityManager::SetComponent(EntityID entityID, const T& component)
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);

        AssertNotNull();

        const auto* p = reinterpret_cast<const byte*>(&component);
        SetComponent(entityID, GetTypeID<T>(), p);
    }

    template <typename T>
    T EntityManager::GetComponent(EntityID entityID) const
    {
        GYN_ECS_STATIC_ASSERT_VALID_COMPONENT(T);

        ea::span<const byte> bytes = GetComponent(entityID, GetTypeID<T>());
        const auto* p = reinterpret_cast<const T*>(bytes.data());

        return *p;
    }

    template <typename T>
    void EntityManager::SetAbstractComponent(EntityID entityID, UniquePtr<T>&& component)
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        SetAbstractComponent(entityID, GetTypeID<T>(), std::move(component));
    }

    template <typename T>
    const T* EntityManager::GetAbstractComponent(EntityID entityID) const
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        return static_cast<const T*>(GetAbstractComponent(entityID, GetTypeID<T>()));
    }

    template <typename T>
    T* EntityManager::GetMutableAbstractComponent(EntityID entityID)
    {
        GYN_ECS_STATIC_ASSERT_VALID_ABSTRACT_COMPONENT(T);
        return static_cast<T*>(GetMutableAbstractComponent(entityID, GetTypeID<T>()));
    }
}