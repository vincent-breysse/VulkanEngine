
#include <pch.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/mat_mesh_ref_count.hpp>
#include <gyn/graphics/render_asset_manager.hpp>

MatMeshRefCountManager::MatMeshRefCountManager(const RenderAssetManager& renderAssets)
{
    m_renderAssets = &renderAssets;
}

bool MatMeshRefCountManager::IsNull() const
{
    return m_renderAssets == nullptr;
}

const RenderAssetManager& MatMeshRefCountManager::GetRenderAssetManager() const
{
    AssertNotNull();
    return *m_renderAssets;
}

uint32 MatMeshRefCountManager::GetRefCount(MatMeshID id) const
{
    AssertNotNull();

    if (HashMapHasValue(m_map, id))
    {
        return m_map.at(id);
    }
    else
    {
        return 0;
    }
}

ea::vector<uint32> MatMeshRefCountManager::CreateRefCountArray(GPURenderPassID renderPass, uint32 subpass) const
{
    AssertNotNull();

    const uint32 batchCount = m_renderAssets->GetInstanceBatchCount(renderPass, subpass);
    const ea::span<const SharedPtr<Material>> materials = m_renderAssets->GetMaterialBatch(renderPass, subpass);
    const ea::span<const SharedPtr<Mesh>> meshes = m_renderAssets->GetMeshBatch();

    ea::vector<uint32> refCounts(batchCount);

    for (uint32 i = 0; i < uint32(materials.size()); ++i)
    {
        for (uint32 j = 0; j < uint32(meshes.size()); ++j)
        {
            MatMeshID id{};
            id.material = materials[i]->GetID();
            id.mesh = meshes[j]->GetID();

            const uint32 k = uint32(i * meshes.size() + j);

            refCounts[k] = GetRefCount(id);
        }
    }

    return refCounts;
}

void MatMeshRefCountManager::IncrementRefCount(MatMeshID id)
{
    AssertNotNull();

    ++m_map[id];
}

void MatMeshRefCountManager::DecrementRefCount(MatMeshID id)
{
    AssertNotNull();

    uint32& refCount = m_map[id];
    ASSERT(refCount > 0);

    --refCount;
}

void MatMeshRefCountManager::AssertNotNull() const
{
    ASSERT(!IsNull());
}


MatMeshRefCounter::MatMeshRefCounter(MatMeshRefCountManager& manager, MatMeshID id)
{
    ASSERT(id.material != MaterialNullID);
    ASSERT(id.mesh != MeshNullID);

    m_manager = &manager;
    m_id = id;

    IncrementRefCount();
}

MatMeshRefCounter::MatMeshRefCounter(MatMeshRefCountManager& manager, MaterialID materialID, MeshID meshID) :
    MatMeshRefCounter(manager, MatMeshID{ materialID, meshID })
{
}

MatMeshRefCounter::MatMeshRefCounter(const MatMeshRefCounter& other) :
    MatMeshRefCounterData(other)
{
    if (!IsNull())
    {
        IncrementRefCount();
    }
}

MatMeshRefCounter::MatMeshRefCounter(MatMeshRefCounter&& other) :
    MatMeshRefCounterData(std::move(other))
{
    other.Nullify();
}

MatMeshRefCounter::~MatMeshRefCounter()
{
    if (!IsNull())
    {
        DecrementRefCount();
    }
}

MatMeshRefCounter& MatMeshRefCounter::operator=(const MatMeshRefCounter& other)
{
    if (!IsNull())
    {
        DecrementRefCount();
    }

    MatMeshRefCounterData::operator=(other);

    if (!IsNull())
    {
        IncrementRefCount();
    }

    return *this;
}

MatMeshRefCounter& MatMeshRefCounter::operator=(MatMeshRefCounter&& other)
{
    if (!IsNull())
    {
        DecrementRefCount();
    }

    MatMeshRefCounterData::operator=(std::move(other));
    other.Nullify();

    return *this;
}

bool MatMeshRefCounter::IsNull() const
{
    return m_manager == nullptr;
}

uint32 MatMeshRefCounter::GetRefCount() const
{
    AssertNotNull();
    return m_manager->GetRefCount(m_id);
}

const MatMeshRefCountManager& MatMeshRefCounter::GetRefCountManager() const
{
    AssertNotNull();
    return *m_manager;
}

MatMeshRefCountManager& MatMeshRefCounter::GetRefCountManager()
{
    AssertNotNull();
    return *m_manager;
}

void MatMeshRefCounter::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void MatMeshRefCounter::IncrementRefCount() const
{
    AssertNotNull();
    m_manager->IncrementRefCount(m_id);
}

void MatMeshRefCounter::DecrementRefCount() const
{
    AssertNotNull();
    m_manager->DecrementRefCount(m_id);
}

void MatMeshRefCounter::Nullify()
{
    m_id = {};
    m_manager = {};
}