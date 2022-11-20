
#include <pch.hpp>
#include <gyn/entity_chunk.hpp>

EntityChunk::EntityChunk(const EntityArchetype& archetype)
{
    const ea::span<const TypeID> types = archetype.GetTypes();

    for (const TypeID& type : types)
    {
        if (IsComponentAbstract(type))
        {
            m_abstractLanes[type] = {};
        }
        else
        {
            Lane& lane = m_lanes[type];
            lane.bytes = {};
            lane.version = 1;
        }
    }

    m_archetype = archetype;
}

bool EntityChunk::IsNull() const
{
    return m_archetype.IsNull();
}

uint32 EntityChunk::CreateEntity()
{
    AssertNotNull();

    for (TypeID type : m_archetype.GetTypes())
    {
        const ComponentTypeInfo& info = GetComponentTypeInfo(type);

        if (info.isAbstract)
        {
            ea::vector<UniquePtr<ECSAbstractComponent>>& data = m_abstractLanes.at(type).ptrs;
            data.push_back(nullptr);
        }
        else
        {
            ea::vector<byte>& bytes = m_lanes.at(type).bytes;
            bytes.resize(bytes.size() + info.size);
        }
    }

    return m_entityCount++;
}

uint32 EntityChunk::GetEntityCount() const
{
    AssertNotNull();
    return m_entityCount;
}

void EntityChunk::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void EntityChunk::AssertHasComponentLane(TypeID type) const
{
    AssertNotNull();
    if (IsComponentAbstract(type))
    {
        ASSERT_MSG(HasAbstractComponentLane(type), "ECSAbstractComponent lane does not exist");
    }
    else
    {
        ASSERT_MSG(HasComponentLane(type), "ECSComponentBase lane does not exist");
    }
}

void EntityChunk::SetComponent(uint32 index, TypeID type, const byte* data)
{
    AssertNotNull();
    AssertHasComponentLane(type);
    ASSERT(data);

    uint32 componentSize = GetComponentSize(type);
    ASSERT(componentSize > 0);

    byte* lane = GetMutableComponentLane(type);
    uint64 byteIndex = uint64(index) * componentSize;
    uint64 laneByteSize = uint64(componentSize) * GetEntityCount(); MaybeUnused(laneByteSize);

    ASSERT(byteIndex + componentSize <= laneByteSize);
    memcpy(&lane[byteIndex], data, componentSize);
}

ea::span<const byte> EntityChunk::GetComponent(uint32 index, TypeID type) const
{
    AssertNotNull();
    AssertHasComponentLane(type);

    uint32 componentSize = GetComponentSize(type);
    ASSERT(componentSize > 0);

    const byte* lane = GetComponentLane(type);
    uint64 byteIndex = uint64(index) * componentSize;
    uint64 laneByteSize = uint64(componentSize) * GetEntityCount(); MaybeUnused(laneByteSize);

    ASSERT(byteIndex + componentSize <= laneByteSize);
    return ea::span<const byte>(&lane[byteIndex], componentSize);
}

void EntityChunk::SetAbstractComponent(uint32 index, TypeID type, UniquePtr<ECSAbstractComponent>&& component)
{
    AssertNotNull();
    AssertHasComponentLane(type);

    UniquePtr<ECSAbstractComponent>* lane = GetMutableAbstractComponentLane(type);

    ASSERT(index < GetEntityCount());
    lane[index] = std::move(component);
}

const ECSAbstractComponent* EntityChunk::GetAbstractComponent(uint32 index, TypeID type) const
{
    AssertNotNull();
    AssertHasComponentLane(type);

    const UniquePtr<const ECSAbstractComponent>* lane = GetAbstractComponentLane(type);

    ASSERT(index < GetEntityCount());
    return lane[index].get();
}

ECSAbstractComponent* EntityChunk::GetMutableAbstractComponent(uint32 index, TypeID type)
{
    AssertNotNull();
    AssertHasComponentLane(type);

    UniquePtr<ECSAbstractComponent>* lane = GetMutableAbstractComponentLane(type);

    ASSERT(index < GetEntityCount());
    return lane[index].get();
}

const EntityArchetype& EntityChunk::GetArchetype() const
{
    AssertNotNull();
    return m_archetype;
}

bool EntityChunk::HasComponentLane(TypeID type) const
{
    AssertNotNull();
    ASSERT(!IsComponentAbstract(type));

    return m_lanes.find(type) != m_lanes.end();
}

bool EntityChunk::HasAbstractComponentLane(TypeID type) const
{
    AssertNotNull();
    ASSERT(IsComponentAbstract(type));

    return m_abstractLanes.find(type) != m_abstractLanes.end();
}

ComponentLaneVersion EntityChunk::GetLaneVersion(TypeID type) const
{
    AssertNotNull();
    AssertHasComponentLane(type);
    ASSERT(!IsComponentAbstract(type));

    return ComponentLaneVersion(m_lanes.at(type).version);
}

byte* EntityChunk::GetMutableComponentLane(TypeID type)
{
    AssertNotNull();
    AssertHasComponentLane(type);

    Lane& lane = m_lanes.at(type);
    ++lane.version;

    return lane.bytes.data();
}

const byte* EntityChunk::GetComponentLane(TypeID type) const
{
    AssertNotNull();
    AssertHasComponentLane(type);

    return m_lanes.at(type).bytes.data();
}

UniquePtr<ECSAbstractComponent>* EntityChunk::GetMutableAbstractComponentLane(TypeID type)
{
    AssertNotNull();
    AssertHasComponentLane(type);

    AbstractLane& lane = m_abstractLanes.at(type);
    return lane.ptrs.data();
}

const UniquePtr<const ECSAbstractComponent>* EntityChunk::GetAbstractComponentLane(TypeID type) const
{
    AssertNotNull();
    AssertHasComponentLane(type);

    const AbstractLane& lane = m_abstractLanes.at(type);
    return reinterpret_cast<const UniquePtr<const ECSAbstractComponent>*>(lane.ptrs.data());
}