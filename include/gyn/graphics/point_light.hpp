#pragma once

#include <gyn/graphics/common.hpp>
#include <gyn/entity_manager.hpp>

namespace Gyn
{
    struct PointLightCommon
    {
        UniqueGPUShaderPtr shadowShader;
        SharedPtr<Material> shadowMaterial;
        ea::vector<UniqueGPUBufferPtr> shadowCubeIDUBOs;
        UniqueGPURenderPassPtr shadowPassLayout;
    };

    struct PointLight : public ECSComponent
    {
        float3 color;
        float intensity;
        float range;
        float shadowNear;
        float shadowFar;
    };

    PointLightCommon CreatePointLightCommonData(RenderAssetManager& assets);

    ShadowMatrixUniforms ComputeShadowMatrices(float3 position, const PointLight& pointLight);

    ShadowParamUniforms GetShadowParams(const PointLight& pointLight);

    ShadowUniforms ComputeShadowUniforms(ea::span<const EntityChunk *const> chunks);

    Entity CreateLightEntity(EntityManager& entityManager, float3 position, const PointLight& light, EntityID pvsID = EntityNullID);
}