
#include <pch.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/point_light.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/pvs.hpp>

static Matrix4x4 CreateShadowMatrix(float3 eye, float3 lookTo, float3 up, const Matrix4x4& proj)
{
    Matrix4x4 mat = Mat4LookAt(eye, eye + lookTo, up) * proj;
    return Mat4Transpose(mat);
}

PointLightCommon Gyn::CreatePointLightCommonData(RenderAssetManager& assets)
{
    GPU& gpu = assets.GetGPU();

    UniqueGPURenderPassPtr shadowPassLayout = {};
    {
        GPUFormat format = GPUFormat::eDepth24S8;

        SubpassLayout subpassLayout = {};
        subpassLayout.depthStencilRef = 0;

        uint32 multiviewMask = 0b111'111;

        shadowPassLayout = assets.CreateRenderPass(ea::span(&format, 1), ea::span(&subpassLayout, 1), ea::span(&multiviewMask, 1));
    }

    ea::vector<UniqueGPUBufferPtr> shadowCubeIDUBOs = {};
    shadowCubeIDUBOs.resize(MaxPointLightCastingShadows);

    for (uint32 i = 0; i < MaxPointLightCastingShadows; ++i)
    {
        UniqueGPUBufferPtr buffer = gpu.CreateBuffer(sizeof(uint32), GPUBufferUsage::eUniformBuffer, GPUMemoryType::eCPUVisible);
        buffer->Upload(AsByteSpan(i));

        shadowCubeIDUBOs[i] = std::move(buffer);
    }

    ShaderOptions shaderOptions = {};
    shaderOptions.backFaceCulling = true;

    UniqueGPUShaderPtr shadowShader = assets.GetGPU().LoadShaderFromFile("shader/shadow.glsl", *shadowPassLayout, 0, shaderOptions);
    SharedPtr<Material> shadowMaterial = assets.CreateMaterial<MaterialNoUniforms>(shadowShader.get());

    PointLightCommon common = {};
    common.shadowCubeIDUBOs = std::move(shadowCubeIDUBOs);
    common.shadowShader = std::move(shadowShader);
    common.shadowMaterial = std::move(shadowMaterial);
    common.shadowPassLayout = std::move(shadowPassLayout);

    return common;
}

ShadowMatrixUniforms Gyn::ComputeShadowMatrices(float3 position, const PointLight& light)
{
    const Matrix4x4 shadowProj = Mat4ProjToVulkanProj(Mat4PerspectiveFOV(DegToRad(90), 1.f, light.shadowNear, light.shadowFar));

    ShadowMatrixUniforms out{};
    out.right = CreateShadowMatrix(position,  float3(1, 0, 0),   float3(0, 1, 0), shadowProj);
    out.left = CreateShadowMatrix(position,   float3(-1, 0, 0),  float3(0, 1, 0), shadowProj);
    out.up = CreateShadowMatrix(position,     float3(0, 1, 0),   float3(0, 0, -1), shadowProj);
    out.down = CreateShadowMatrix(position,   float3(0, -1, 0),  float3(0, 0, 1), shadowProj);
    out.front = CreateShadowMatrix(position,  float3(0, 0, 1),   float3(0, 1, 0), shadowProj);
    out.back = CreateShadowMatrix(position,   float3(0, 0, -1),  float3(0, 1, 0), shadowProj);

    return out;
}

ShadowParamUniforms Gyn::GetShadowParams(const PointLight& pointLight)
{
    ShadowParamUniforms params{};
    params.near = pointLight.shadowNear;
    params.far = pointLight.shadowFar;

    return params;
}

ShadowUniforms Gyn::ComputeShadowUniforms(ea::span<const EntityChunk *const> chunks)
{
    ShadowUniforms uniforms{};

    uint64 i = 0;
    for (const EntityChunk* chunk : chunks)
    {
        const Position* positions = chunk->GetComponentLane<Position>();
        const PointLight* lights = chunk->GetComponentLane<PointLight>();

        for (uint32 k = 0; k < chunk->GetEntityCount() && i < MaxPointLightCastingShadows; ++k, ++i)
        {
            float3 position = positions[k].val;
            const PointLight& light = lights[k];

            uniforms.matrices[i] = ComputeShadowMatrices(position, light);
            uniforms.params[i] = GetShadowParams(light);
        }
    }

    return uniforms;
}

Entity Gyn::CreateLightEntity(EntityManager& entityManager, float3 position, const PointLight& light, EntityID pvsID)
{
    static const ea::array LightComponentTypes
    {
        GetTypeID<Position>(),
        GetTypeID<PointLight>(),
        GetTypeID<PVSEntityID>()
    };

    Entity entity = entityManager.CreateEntity(LightComponentTypes);

    entity.SetComponent(Position(position));
    entity.SetComponent(light);
    entity.SetComponent(PVSEntityID(pvsID));

    return entity;
}