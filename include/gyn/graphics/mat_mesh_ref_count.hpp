#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    class MatMeshRefCountManager
    {
    public:

        MatMeshRefCountManager() = default;

        MatMeshRefCountManager(const RenderAssetManager& renderAssets);

        bool IsNull() const;

        const RenderAssetManager& GetRenderAssetManager() const;

        uint32 GetRefCount(MatMeshID id) const;

        ea::vector<uint32> CreateRefCountArray(GPURenderPassID renderPass, uint32 subpass) const;

        void IncrementRefCount(MatMeshID id);

        void DecrementRefCount(MatMeshID id);

    private:

        void AssertNotNull() const;

        const RenderAssetManager* m_renderAssets = {};
        ea::hash_map<MatMeshID, uint32> m_map = {};
    };

    // Separate POD to perform trivial data copy/move automicatically
    struct MatMeshRefCounterData
    {
        MatMeshRefCountManager* m_manager;
        MatMeshID m_id;
    };

    class MatMeshRefCounter : private MatMeshRefCounterData
    {
    public:

        MatMeshRefCounter() = default;

        MatMeshRefCounter(const MatMeshRefCounter& other);

        MatMeshRefCounter(MatMeshRefCounter&& other);

        ~MatMeshRefCounter();

        MatMeshRefCounter& operator=(const MatMeshRefCounter& other);

        MatMeshRefCounter& operator=(MatMeshRefCounter&& other);



        MatMeshRefCounter(MatMeshRefCountManager& manager, MatMeshID id);

        MatMeshRefCounter(MatMeshRefCountManager& manager, MaterialID materialID, MeshID meshID);

        bool IsNull() const;

        uint32 GetRefCount() const;

        const MatMeshRefCountManager& GetRefCountManager() const;

        MatMeshRefCountManager& GetRefCountManager();

    private:

        void AssertNotNull() const;

        void IncrementRefCount() const;

        void DecrementRefCount() const;

        void Nullify();
    };
}