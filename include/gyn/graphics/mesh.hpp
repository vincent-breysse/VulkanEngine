#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    class Mesh : NoCopyMove
    {
    public:

        Mesh(GPU& gpu, UniqueGPUBufferPtr&& vertexBuffer, UniqueGPUBufferPtr&& index16Buffer, const AABB& bounds);

        bool IsNull() const;

        MeshID GetID() const;

        const GPUBuffer& GetVertexBuffer() const;

        const GPUBuffer& GetIndex16Buffer() const;

        const AABB& GetBounds() const;

    private:

        void AssertNotNull() const;

        GPU* m_gpu = {};
        MeshID m_id = {};
        AABB m_bounds = {};
        UniqueGPUBufferPtr m_vBuffer = {};
        UniqueGPUBufferPtr m_iBuffer = {};
    };

    inline SharedPtr<Mesh> CreateMeshSharedPtr(GPU& gpu, UniqueGPUBufferPtr&& vertexBuffer, UniqueGPUBufferPtr&& index16Buffer, const AABB& bounds)
    {
        return ea::make_shared<Mesh>(gpu, std::move(vertexBuffer), std::move(index16Buffer), bounds);
    }
}