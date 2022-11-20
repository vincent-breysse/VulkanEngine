
#include <pch.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/render_asset_manager.hpp>

static MeshID CreateUniqueMeshID()
{
    static std::atomic<uint64> id = 1;
    return MeshID(id++);
}

Mesh::Mesh(GPU& gpu, UniqueGPUBufferPtr&& vertexBuffer, UniqueGPUBufferPtr&& index16Buffer, const AABB& bounds)
{
    const MeshID id = CreateUniqueMeshID();

    m_gpu = &gpu;
    m_id = id;
    m_bounds = bounds;
    m_vBuffer = std::move(vertexBuffer);
    m_iBuffer = std::move(index16Buffer);
}

bool Mesh::IsNull() const
{
    return m_id == MeshNullID;
}

MeshID Mesh::GetID() const
{
    AssertNotNull();
    return m_id;
}

const GPUBuffer& Mesh::GetVertexBuffer() const
{
    AssertNotNull();
    return *m_vBuffer;
}

const GPUBuffer& Mesh::GetIndex16Buffer() const
{
    AssertNotNull();
    return *m_iBuffer;
}

const AABB& Mesh::GetBounds() const
{
    AssertNotNull();
    return m_bounds;
}

void Mesh::AssertNotNull() const
{
    ASSERT(!IsNull());
}