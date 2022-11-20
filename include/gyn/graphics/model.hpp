#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn
{
    struct TextureLoading
    {
        ea::string filePath;
    };

    struct MaterialLoading
    {
        int albedoMapIndex;
        int normalMapIndex;
        int emissiveMapIndex;
        int maskMapIndex;
        float3 color;
        float shininess;
    };

    struct SubMeshLoading
    {
        ea::vector<Vertex> vertices;
        ea::vector<uint16> indices;
        int materialIndex;
    };

    struct ModelLoading
    {
        ea::vector<TextureLoading> textures;
        ea::vector<MaterialLoading> materials;
        ea::vector<SubMeshLoading> subMeshes;
    };

    class Model : NoCopyMove
    {
    public:

        Model() = default;

        Model(ea::vector<SharedPtr<Material>>&& subMaterials, ea::vector<SharedPtr<Mesh>>&& subMeshes);

        bool IsNull() const;

        ModelID GetID() const;

        ea::span<const SharedPtr<const Mesh>> GetSubMeshes() const;

        const Mesh& GetSubMesh(uint i) const;

        ea::span<const SharedPtr<const Material>> GetSubMaterials() const;

        const Material& GetSubMaterial(uint i) const;

        const AABB& GetBounds() const;

    private:

        void AssertNotNull() const;

        ModelID m_id = {};
        AABB m_bounds = {};
        const GPUShader* m_sharedShader = {};
        ea::vector<SharedPtr<Mesh>> m_subMeshes = {};
        ea::vector<SharedPtr<Material>> m_subMaterials = {};
    };

    void PrintModelInfo(const char* path);

    ModelLoading LoadModelData(const char* path);
}