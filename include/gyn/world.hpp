#pragma once

#include <gyn/graphics/common.hpp>
#include <gyn/time_manager.hpp>
#include <gyn/entity_manager.hpp>
#include <gyn/graphics/point_light.hpp>
#include <gyn/graphics/graphics_engine.hpp>

namespace Gyn
{
    struct World : NoCopyMove
    {
        World() = default;

        Engine* engine = {};
        GlobalData global = {};
        TimeManager timeManager = {};
        PointLightCommon pointLightCommon = {};
        EntityManager entityManager = {};
        FrameDataBuffer frameData = {};
    };
}