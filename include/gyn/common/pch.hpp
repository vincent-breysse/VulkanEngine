#pragma once

#include <stdcpp17.hpp>

#include <EASTL.h>

#include <jsoncpp/json.h>

#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/ostream.h>

#include <VMath/vmath.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define VK_NO_PROTOTYPES
#include <Vex/Loader.hpp>
#include <Vex/Device.hpp>
#include <Vex/ShaderCompiler.hpp>
#include <Vex/Swapchain.hpp>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_vulkan.h>

namespace ea = eastl;

namespace Gyn
{
    using namespace VMath;
}