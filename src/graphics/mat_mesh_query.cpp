
#include <pch.hpp>
#include <gyn/graphics/mat_mesh_query.hpp>
#include <gyn/graphics/material.hpp>
#include <gyn/graphics/mesh.hpp>
#include <gyn/graphics/render_asset_manager.hpp>

static void AssertMaterialsBelongToSubpass(ea::span<const SharedPtr<const Material>> materials, GPURenderPassID renderPass, uint32 subpass)
{
    if constexpr (EnableAsserts)
    {
        for (const auto& m : materials)
        {
            const GPUShader& s = *m->GetShader();
            ASSERT(s.GetRenderPass().GetID() == renderPass);
            ASSERT(s.GetSubpass() == subpass);
        }
    }
}

ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex> Gyn::CreateIdentityRemapTable(ea::span<const SharedPtr<const Material>> materials,
    ea::span<const SharedPtr<const Mesh>> meshes)
{
    ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex> remapTable{};

    for (uint32 i = 0; i < uint32(materials.size()); ++i)
    {
        for (uint32 j = 0; j < uint32(meshes.size()); ++j)
        {
            MatMeshLocalIndex keyValue{};
            keyValue.material = i;
            keyValue.mesh = j;

            ASSERT(!HashMapHasValue(remapTable, keyValue));
            remapTable[keyValue] = keyValue;
        }
    }

    return remapTable;
}

MatMeshQuery::MatMeshQuery(const GPURenderPass& renderPass, uint32 subpass,
    ea::vector<SharedPtr<const Material>>&& materials,
    ea::vector<SharedPtr<const Mesh>>&& meshes,
    ea::hash_map<MatMeshLocalIndex, MatMeshLocalIndex>&& remapTable,
    bool isRemappedIdentity)
{
    m_renderPass = &renderPass;
    m_subpass = subpass;
    m_isRemapIdentity = isRemappedIdentity;
    m_materials = std::move(materials);
    m_meshes = std::move(meshes);
    m_remapTable = std::move(remapTable);
}

bool MatMeshQuery::IsNull() const
{
    return m_renderPass == nullptr;
}

bool MatMeshQuery::IsRemapIdentity() const
{
    AssertNotNull();
    return m_isRemapIdentity;
}

GPURenderPassID MatMeshQuery::GetGPURenderPassID() const
{
    AssertNotNull();
    return m_renderPass->GetID();
}

uint32 MatMeshQuery::GetSubpass() const
{
    AssertNotNull();
    return m_subpass;
}

uint32 MatMeshQuery::GetInstanceBatchCount() const
{
    AssertNotNull();
    return uint32(m_materials.size() * m_meshes.size());
}

ea::span<const SharedPtr<const Material>> MatMeshQuery::GetMaterialBatch() const
{
    AssertNotNull();
    return m_materials;
}

uint32 MatMeshQuery::GetMaterialBatchSize() const
{
    AssertNotNull();
    return uint32(m_materials.size());
}

ea::span<const SharedPtr<const Mesh>> MatMeshQuery::GetMeshBatch() const
{
    AssertNotNull();
    return m_meshes;
}

uint32 MatMeshQuery::GetMeshBatchSize() const
{
    AssertNotNull();
    return uint32(m_meshes.size());
}

MatMeshLocalIndex MatMeshQuery::GetRemappedMatMeshIndex(MatMeshLocalIndex index) const
{
    AssertNotNull();
    return m_remapTable.at(index);
}

uint32 MatMeshQuery::GetRemappedInstanceBatchIndex(MatMeshLocalIndex index) const
{
    AssertNotNull();
    MatMeshLocalIndex remapped = GetRemappedMatMeshIndex(index);

    return uint32(remapped.material * m_meshes.size() + remapped.mesh);
}

ea::vector<uint32> MatMeshQuery::CreateRemappedVector(ea::span<const uint32> data,
    ea::span<const SharedPtr<const Material>> materials,
    ea::span<const SharedPtr<const Mesh>> meshes) const
{
    AssertNotNull();
    ASSERT(!materials.empty());
    ASSERT(!meshes.empty());

    if (data.empty()) return {};
    if (IsRemapIdentity())
    {
        return CopyToVector(data);
    }

    ea::vector<uint32> ret(m_materials.size() * m_meshes.size());

    for (uint32 i = 0; i < uint32(materials.size()); ++i)
    {
        for (uint32 j = 0; j < uint32(meshes.size()); ++j)
        {
            MatMeshLocalIndex key{};
            key.material = i;
            key.mesh = j;

            const uint32 k0 = uint32(i * meshes.size() + j);
            const uint32 k1 = GetRemappedInstanceBatchIndex(key);

            ret[k1] += data[k0];
        }
    }

    return ret;
}

ea::vector<uint32> MatMeshQuery::CreateRemappedVector(ea::span<const uint32> data, const RenderAssetManager& renderAssets) const
{
    AssertNotNull();

    return CreateRemappedVector(data, 
        ToSpanConst(renderAssets.GetMaterialBatch(m_renderPass->GetID(), m_subpass)), 
        ToSpanConst(renderAssets.GetMeshBatch()));
}

void MatMeshQuery::AssertNotNull() const
{
    ASSERT(!IsNull());
}

MatMeshQueryBuilder::MatMeshQueryBuilder(const GPURenderPass& renderPass, uint32 subpass,
    ea::span<const SharedPtr<const Material>> materials,
    ea::span<const SharedPtr<const Mesh>> meshes)
{
    ASSERT(subpass < renderPass.GetSubpasses().size());
    AssertMaterialsBelongToSubpass(materials, renderPass.GetID(), subpass);

    auto materialsCopy = CopyToVector(materials);
    auto meshesCopy = CopyToVector(meshes);
    auto remapTable = CreateIdentityRemapTable(materials, meshes);

    m_materials = std::move(materialsCopy);
    m_meshes = std::move(meshesCopy);
    m_remapTable = std::move(remapTable);
    m_renderPass = &renderPass;
    m_subpass = subpass;
    m_isRemapIdentity = true;
    m_hasBuilt = false;
}

MatMeshQueryBuilder::MatMeshQueryBuilder(const GPURenderPass& renderPass, uint32 subpass, const RenderAssetManager& renderAssets) :
    MatMeshQueryBuilder
    (
        renderPass, 
        subpass, 
        ToSpanConst(renderAssets.GetMaterialBatch(renderPass.GetID(), subpass)), 
        ToSpanConst(renderAssets.GetMeshBatch())
    )
{
}

bool MatMeshQueryBuilder::IsNull() const
{
    return m_renderPass == nullptr;
}

bool MatMeshQueryBuilder::HasBuilt() const
{
    AssertNotNull();
    return m_hasBuilt;
}

MatMeshQueryBuilder& MatMeshQueryBuilder::SetGlobalMaterialOverride(SharedPtr<const Material> material)
{
    AssertNotNull();
    AssertHasNotBuilt();
    ASSERT(!m_materials.empty());
    ASSERT(!m_meshes.empty());

    const MaterialID materialOverrideID = material->GetID();

    for (uint32 i = 0; i < uint32(m_materials.size()); ++i)
    {
        for (uint32 j = 0; j < uint32(m_meshes.size()); ++j)
        {
            MatMeshLocalIndex key{};
            key.material = i;
            key.mesh = j;

            MatMeshLocalIndex value{};
            value.material = 0;
            value.mesh = j;

            m_remapTable.at(key) = value;
        }
    }

    m_materials.clear();
    m_materials.emplace_back(std::move(material));

    m_isRemapIdentity = false;
    return *this;
}

MatMeshQuery MatMeshQueryBuilder::Build()
{
    AssertNotNull();
    AssertHasNotBuilt();

    m_hasBuilt = true;
    return MatMeshQuery(*m_renderPass, m_subpass, std::move(m_materials), std::move(m_meshes), std::move(m_remapTable), m_isRemapIdentity);
}

void MatMeshQueryBuilder::AssertNotNull() const
{
    ASSERT(!IsNull());
}

void MatMeshQueryBuilder::AssertHasNotBuilt() const
{
    ASSERT(!HasBuilt());
}