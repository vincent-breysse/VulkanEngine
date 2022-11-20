
#include <pch.hpp>
#include <gyn/entity_archetype.hpp>
#include <gyn/entity_chunk.hpp>
#include <gyn/entity.hpp>
#include <gyn/entity_manager.hpp>

static EntityID CreateUniqueEntityID()
{
    static std::atomic<uint64> id = 1;
    return EntityID(id++);
}

static const char* GetTypeName(TypeID id)
{
    if constexpr (GYN_RTTI)
    {
        return GetTypeInfo(id)->name();
    }
    else
    {
        return "Unknown";
    }
}
static auto GetTypeNameMaybeUnused = &GetTypeName; // Avoid warning in release mode

static ea::hash_map<TypeID, ComponentTypeInfo>& GetComponentReflectionMap()
{
    static ea::hash_map<TypeID, ComponentTypeInfo> ret;
    return ret;
}

bool Gyn::IsNull(EntityArchetypeID id)
{
    return id == EntityArchetypeNullID;
}

void Gyn::AssertNotNull(EntityArchetypeID id)
{
    ASSERT_MSG(!IsNull(id), "EntityArchetypeID is null");
}

bool Gyn::IsNull(EntityID id)
{
    return id == EntityNullID;
}

void Gyn::AssertNotNull(EntityID id)
{
    ASSERT_MSG(!IsNull(id), "EntityID is null");
}

bool Gyn::operator==(const EntityMetadata& a, const EntityMetadata& b)
{
    return a.archetypeID == b.archetypeID && a.chunkLocalIndex == b.chunkLocalIndex;
}

bool Gyn::operator!=(const EntityMetadata& a, const EntityMetadata& b)
{
    return !(a == b);
}

bool Gyn::operator==(const ComponentTypeInfo& a, const ComponentTypeInfo& b)
{
    return a.isAbstract == b.isAbstract && a.size == b.size;
}

bool Gyn::operator!=(const ComponentTypeInfo& a, const ComponentTypeInfo& b)
{
    return !(a == b);
}

void Gyn::RegisterComponentType(TypeID id, const ComponentTypeInfo& info)
{
    auto& map = GetComponentReflectionMap();
    ASSERT_MSG(!HashMapHasValue(map, id), fmt::format("Component '{}' already registered", GetTypeName(id)));

    map[id] = info;
}

ComponentTypeInfo Gyn::GetComponentTypeInfo(TypeID id)
{
    auto& map = GetComponentReflectionMap();
    ASSERT_MSG(HashMapHasValue(map, id), fmt::format("Unknow component '{}'", GetTypeName(id)));

    return map.at(id);
}

uint32 Gyn::GetComponentSize(TypeID id)
{
    return GetComponentTypeInfo(id).size;
}

bool Gyn::IsComponentAbstract(TypeID id)
{
    return GetComponentTypeInfo(id).isAbstract;
}

EntityManager::EntityManager(World& world)
{
    m_world = &world;
}

bool EntityManager::IsNull() const
{
    return m_world == nullptr;
}

EntityArchetype EntityManager::CreateArchetype(ea::span<const TypeID> componentTypes)
{
    AssertNotNull();

    if (uint32 index = 0; HasMatchingArchetype(componentTypes, index))
    {
        return m_archetypes[index];
    }
    
    return m_archetypes.emplace_back(EntityArchetype(componentTypes));
}

Entity EntityManager::CreateEntity(const EntityArchetype& archetype)
{
    AssertNotNull();

    EntityChunk& chunk = GetOrCreateChunk(archetype);
    EntityID id = CreateUniqueEntityID();
    EntityArchetypeID archetypeID = archetype.GetID();
    uint32 chunkLocalIndex = chunk.CreateEntity();

    EntityMetadata entityMetadata = {};
    entityMetadata.chunkLocalIndex = chunkLocalIndex;
    entityMetadata.archetypeID = archetypeID;

    ASSERT(!HasAssociatedMetadata(id));
    m_entityMetadata[id] = entityMetadata;

    return Entity(*this, id);
}

Entity EntityManager::CreateEntity(ea::span<const TypeID> componentTypes)
{
    AssertNotNull();
    return CreateEntity(CreateArchetype(componentTypes));
}

const EntityChunk& EntityManager::GetEntityChunk(EntityArchetypeID id) const
{
    AssertNotNull();
    ::AssertNotNull(id);
    AssertHasAssociatedEntityChunk(id);

    return *m_chunks.at(id);
}

EntityChunk& EntityManager::GetEntityChunk(EntityArchetypeID id)
{
    return const_cast<EntityChunk&>(THIS_CONST->GetEntityChunk(id));
}

const EntityChunk& EntityManager::GetEntityChunk(EntityID id) const
{
    AssertNotNull();
    ::AssertNotNull(id);
    AssertHasAssociatedMetadata(id);
    AssertHasAssociatedEntityChunk(id);

    return GetEntityChunk(GetEntityArchetypeID(id));
}

EntityChunk& EntityManager::GetEntityChunk(EntityID id)
{
    return const_cast<EntityChunk&>(THIS_CONST->GetEntityChunk(id));
}

EntityMetadata EntityManager::GetEntityMetadata(EntityID id) const
{
    AssertNotNull();
    ::AssertNotNull(id);
    AssertHasAssociatedMetadata(id);

    return m_entityMetadata.at(id);
}

EntityArchetypeID EntityManager::GetEntityArchetypeID(EntityID id) const
{
    AssertNotNull();
    ::AssertNotNull(id);
    return GetEntityMetadata(id).archetypeID;
}

uint32 EntityManager::GetEntitChunkLocalIndex(EntityID id) const
{
    AssertNotNull();
    ::AssertNotNull(id);
    return GetEntityMetadata(id).chunkLocalIndex;
}

EntityArchetype EntityManager::GetEntityArchetype(EntityID id)
{
    AssertNotNull();
    ::AssertNotNull(id);

    return GetEntityChunk(id).GetArchetype();
}

EntityID EntityManager::GetEntityID(const EntityMetadata& metadata) const
{
    AssertNotNull();

    // TODO : Fix this linear search
    for (auto& kv : m_entityMetadata)
    {
        if (kv.second == metadata)
        {
            return kv.first;
        }
    }

    return EntityNullID;
}

EntityID EntityManager::GetEntityID(const EntityChunk& chunk, uint32 chunkLocalIndex) const
{
    AssertNotNull();

    EntityMetadata metadata{};
    metadata.archetypeID = chunk.GetArchetype().GetID();
    metadata.chunkLocalIndex = chunkLocalIndex;

    return GetEntityID(metadata);
}

ea::vector<const EntityChunk*> EntityManager::QueryEntityChunks(ea::span<const TypeID> all) const
{
    AssertNotNull();

    ea::vector<const EntityChunk*> chunks{};

    for (const EntityArchetype& archetype : m_archetypes)
    {
        if (archetype.HasAllTypes(all))
        {
            chunks.push_back(m_chunks.at(archetype.GetID()).get());
        }
    }

    return chunks;
}

ea::vector<EntityChunk*> EntityManager::QueryEntityChunks(ea::span<const TypeID> all)
{
    AssertNotNull();

    ea::vector<const EntityChunk*> vecConst = THIS_CONST->QueryEntityChunks(all);
    return ConstCastMove(vecConst);
}

Entity EntityManager::GetFirstEnity(EntityChunk& chunk)
{
    AssertNotNull();
    ASSERT_MSG(chunk.GetEntityCount() > 0,
        "The chunk must contain at least one entity");

    EntityMetadata metadata = {};
    metadata.archetypeID = chunk.GetArchetype().GetID();
    metadata.chunkLocalIndex = 0;

    EntityID id = GetEntityID(metadata);
    ASSERT_MSG(id != EntityNullID,
        "Could not find any Entity matching the EntityMetadata");

    return Entity(*this, id);
}

Entity EntityManager::GetFirstEnity(ea::span<const TypeID> all)
{
    AssertNotNull();

    ea::vector<EntityChunk*> chunks = QueryEntityChunks(all);
    ASSERT(!chunks.empty());

    return GetFirstEnity(*chunks.front());
}

Entity EntityManager::GetTheEntity(ea::span<const TypeID> all)
{
    AssertNotNull();

    ea::vector<EntityChunk*> chunks = QueryEntityChunks(all);
    ASSERT_MSG(!chunks.empty(),
        "No matching chunks found");
    ASSERT_MSG(chunks[0]->GetEntityCount() == 1,
        "The chunk must contain exactly one entity");

    return GetFirstEnity(*chunks[0]);
}

bool EntityManager::HasMatchingArchetype(ea::span<const TypeID> componentTypes, uint32& outIndex) const
{
    AssertNotNull();

    for (uint32 i = 0; i < m_archetypes.size(); ++i)
    {
        if (m_archetypes[i].HasExactTypes(componentTypes))
        {
            outIndex = i;
            return true;
        }
    }

    outIndex = 0;
    return false;
}

void EntityManager::SetComponent(EntityID entityID, TypeID type, const byte* data)
{
    AssertNotNull();
    ::AssertNotNull(entityID);
    ASSERT(data);
    ASSERT(GetComponentSize(type) > 0);

    uint32 chunkLocalIndex = GetEntitChunkLocalIndex(entityID);
    EntityChunk& chunk = GetEntityChunk(entityID);

    chunk.SetComponent(chunkLocalIndex, type, data);
}

ea::span<const byte> EntityManager::GetComponent(EntityID entityID, TypeID type) const
{
    AssertNotNull();
    ::AssertNotNull(entityID);
    ASSERT(GetComponentSize(type) > 0);

    uint32 chunkLocalIndex = GetEntitChunkLocalIndex(entityID);
    const EntityChunk& chunk = GetEntityChunk(entityID);

    return chunk.GetComponent(chunkLocalIndex, type);
}

void EntityManager::SetAbstractComponent(EntityID entityID, TypeID type, UniquePtr<ECSAbstractComponent>&& component)
{
    AssertNotNull();
    ::AssertNotNull(entityID);

    uint32 chunkLocalIndex = GetEntitChunkLocalIndex(entityID);
    EntityChunk& chunk = GetEntityChunk(entityID);

    chunk.SetAbstractComponent(chunkLocalIndex, type, std::move(component));
}

const ECSAbstractComponent* EntityManager::GetAbstractComponent(EntityID entityID, TypeID type) const
{
    AssertNotNull();
    ::AssertNotNull(entityID);

    uint32 chunkLocalIndex = GetEntitChunkLocalIndex(entityID);
    const EntityChunk& chunk = GetEntityChunk(entityID);

    return chunk.GetAbstractComponent(chunkLocalIndex, type);
}

ECSAbstractComponent* EntityManager::GetMutableAbstractComponent(EntityID entityID, TypeID type)
{
    AssertNotNull();
    ::AssertNotNull(entityID);

    uint32 chunkLocalIndex = GetEntitChunkLocalIndex(entityID);
    EntityChunk& chunk = GetEntityChunk(entityID);

    return chunk.GetMutableAbstractComponent(chunkLocalIndex, type);
}

EntityChunk& EntityManager::GetOrCreateChunk(const EntityArchetype& archetype)
{
    AssertNotNull();

    const EntityArchetypeID archetypeID = archetype.GetID();
    if (!HasAssociatedEntityChunk(archetypeID))
    {
        m_chunks[archetypeID] = ea::make_unique<EntityChunk>(archetype);
    }

    return GetEntityChunk(archetypeID);
}

bool EntityManager::HasAssociatedEntityChunk(EntityArchetypeID id) const
{
    AssertNotNull();
    return m_chunks.find(id) != m_chunks.end();
}

bool EntityManager::HasAssociatedEntityChunk(EntityID id) const
{
    AssertNotNull();
    AssertHasAssociatedMetadata(id);

    const EntityMetadata& metadata = m_entityMetadata.at(id);
    return HasAssociatedEntityChunk(metadata.archetypeID);
}

bool EntityManager::HasAssociatedMetadata(EntityID id) const
{
    AssertNotNull();
    return m_entityMetadata.find(id) != m_entityMetadata.end();
}

void EntityManager::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void EntityManager::AssertHasAssociatedEntityChunk(EntityArchetypeID id) const
{
    AssertNotNull();
    ASSERT_MSG(HasAssociatedEntityChunk(id),
        "EntityID has no associated EntityChunk");
}

void EntityManager::AssertHasAssociatedEntityChunk(EntityID id) const
{
    AssertNotNull();
    ASSERT_MSG(HasAssociatedEntityChunk(id),
        "EntityID has no associated EntityChunk");
}

void EntityManager::AssertHasAssociatedMetadata(EntityID id) const
{
    AssertNotNull();
    ASSERT_MSG(HasAssociatedMetadata(id), 
        "EntityID has no associated EntityMetadata");
}