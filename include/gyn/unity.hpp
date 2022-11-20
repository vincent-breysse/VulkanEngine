#pragma once

#include <gyn/graphics/common.hpp>

namespace Gyn::Unity
{
    enum class TextureType
    {
        Null,
        Albedo,
        Normal,
        Emissive
    };

    struct Entity
    {
        float3 position;
        Quaternion rotation;
        float3 scale;
        AABB worldBounds;
        int32 modelID;
        bool castShadows;
    };

    struct Mesh
    {
        ea::vector<Vertex> vertices;
        ea::vector<uint16> indices;
        AABB bounds;
    };

    struct Texture
    {
        ea::string path;
        TextureType type;
    };

    struct Material
    {
        float4 color;
        float3 emissiveColor;
        float2 uvScale;
        float normalScale;
        float smoothness;
        float metallic;
        int albedoMapIndex;
        int normalMapIndex;
        int emissiveMapIndex;
        int maskMapIndex;
    };

    struct Model
    {
        ea::vector<uint32> subMaterialIDs;
        ea::vector<uint32> subMeshIDs;
    };

    enum class LightBakeType
    {
        Mixed = 1,
        Baked = 2,
        Realtime = 4
    };

    enum class ShadowType
    {
        None,
        Hard,
        Soft
    };

    struct PointLight
    {
        float3 position;
        float3 color;
        LightBakeType bakeType;
        ShadowType shadowType;
        float intensity;
        float range;
    };

    struct PVS
    {
        AABB bounds;
        ea::vector<uint32> pointLightIndices;
        ea::vector<uint32> renderersIndices;
        ea::vector<uint32> visiblePVSIndices;
    };

    struct Scene
    {
        ea::vector<Mesh> meshes;
        ea::vector<Texture> textures;
        ea::vector<Material> materials;
        ea::vector<Model> models;
        ea::vector<Entity> entities;
        ea::vector<PointLight> pointLights;
        ea::vector<PVS> pvsArray;
    };
     
    Scene LoadScene(const char* path);
}

namespace Gyn
{
    SceneStats SetupWorldFromUnityScene(World& world, const char* path);

    TransformRecord LoadRecord(const char* path);
}