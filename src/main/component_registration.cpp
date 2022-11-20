
#include <pch.hpp>
#include <gyn/common.hpp>
#include <gyn/transform.hpp>
#include <gyn/graphics/point_light.hpp>
#include <gyn/graphics/graphics_engine.hpp>
#include <gyn/graphics/render_component.hpp>
#include <gyn/graphics/pvs.hpp>

void Gyn::RegisterAllComponents()
{
    RegisterComponentType<EntityIDComponent>();
    RegisterComponentType<PointLight>();
    RegisterComponentType<RenderComponent>();
    RegisterComponentType<Position>();
    RegisterComponentType<Rotation>();
    RegisterComponentType<Scale>();
    RegisterComponentType<LocalToWorld>();
    RegisterComponentType<Resolution>();
    RegisterComponentType<PerspectiveProjection>();
    RegisterComponentType<WorldToView>();
    RegisterComponentType<ViewToClip>();
    RegisterComponentType<WorldToClip>();
    RegisterComponentType<PVSBounds>();
    RegisterComponentType<PVSDynamicData>();
    RegisterComponentType<PVSDataCache>();
    RegisterComponentType<PVSEntityID>();
    RegisterComponentType<EntityGPUBufferIndex>();
    RegisterComponentType<EntityModelID>();
    RegisterComponentType<LocalRenderBounds>();
    RegisterComponentType<WorldRenderBounds>();
    RegisterComponentType<DisableShadow>();
}