#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex> CreateIdentityRemapTable(ea::span<const SharedPtr<const Material>> materials,
        ea::span<const SharedPtr<const Mesh>> meshes);

    class MatMeshQuery
    {
        friend MatMeshQueryBuilder;

    public:

        MatMeshQuery() = default;

        MatMeshQuery(const GPURenderPass& renderPass, uint32 subpass,
            ea::vector<SharedPtr<const Material>>&& materials,
            ea::vector<SharedPtr<const Mesh>>&& meshes,
            ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex>&& remapTable,
            bool isRemappedIdentity);

        bool IsNull() const;

        bool IsRemapIdentity() const;

        GPURenderPassID GetGPURenderPassID() const;

        uint32 GetSubpass() const;

        uint32 GetInstanceBatchCount() const;

        ea::span<const SharedPtr<const Material>> GetMaterialBatch() const;

        uint32 GetMaterialBatchSize() const;

        ea::span<const SharedPtr<const Mesh>> GetMeshBatch() const;

        uint32 GetMeshBatchSize() const;

        MatMeshLocalIndex GetRemappedMatMeshIndex(MatMeshLocalIndex index) const;

        uint32 GetRemappedInstanceBatchIndex(MatMeshLocalIndex index) const;

        ea::vector<uint32> CreateRemappedVector(ea::span<const uint32> data, 
            ea::span<const SharedPtr<const Material>> materials,
            ea::span<const SharedPtr<const Mesh>> meshes) const;

        ea::vector<uint32> CreateRemappedVector(ea::span<const uint32> data, const RenderAssetManager& renderAssets) const;

    private:

        void AssertNotNull() const;

        const GPURenderPass* m_renderPass = {};
        uint32 m_subpass = {};
        ea::vector<SharedPtr<const Material>> m_materials = {};
        ea::vector<SharedPtr<const Mesh>> m_meshes = {};
        ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex> m_remapTable = {};
        bool m_isRemapIdentity = {};
    };

    class MatMeshQueryBuilder
    {
    public:

        MatMeshQueryBuilder() = default;

        MatMeshQueryBuilder(const GPURenderPass& renderPass, uint32 subpass, 
            ea::span<const SharedPtr<const Material>> materials, 
            ea::span<const SharedPtr<const Mesh>> meshes);

        MatMeshQueryBuilder(const GPURenderPass& renderPass, uint32 subpass, const RenderAssetManager& renderAssets);

        bool IsNull() const;

        bool HasBuilt() const;

        MatMeshQueryBuilder& SetGlobalMaterialOverride(SharedPtr<const Material> material);

        MatMeshQuery Build();

    private:

        void AssertNotNull() const;

        void AssertHasNotBuilt() const;

        bool m_hasBuilt = {};
        bool m_isRemapIdentity = {};
        const GPURenderPass* m_renderPass = {};
        uint32 m_subpass = {};
        ea::vector<SharedPtr<const Material>> m_materials = {};
        ea::vector<SharedPtr<const Mesh>> m_meshes = {};
        ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex> m_remapTable = {};
    };
}