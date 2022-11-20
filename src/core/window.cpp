
#include <pch.hpp>
#include <gyn/window.hpp>

namespace Gyn
{
    struct WindowImpl
    {
        GLFWwindow* window = nullptr;
        float2 prevMousePos = float2(0, 0);
        float2 mousePos = float2(0, 0);
    };
}

static int ToGlfwKey(KeyCode key)
{
    constexpr ea::array Map
    {
        -1,
        GLFW_KEY_SPACE,
        GLFW_KEY_APOSTROPHE,
        GLFW_KEY_COMMA,
        GLFW_KEY_MINUS,
        GLFW_KEY_PERIOD,
        GLFW_KEY_SLASH,
        GLFW_KEY_0,
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
        GLFW_KEY_4,
        GLFW_KEY_5,
        GLFW_KEY_6,
        GLFW_KEY_7,
        GLFW_KEY_8,
        GLFW_KEY_9,
        GLFW_KEY_SEMICOLON,
        GLFW_KEY_EQUAL,
        GLFW_KEY_A,
        GLFW_KEY_B,
        GLFW_KEY_C,
        GLFW_KEY_D,
        GLFW_KEY_E,
        GLFW_KEY_F,
        GLFW_KEY_G,
        GLFW_KEY_H,
        GLFW_KEY_I,
        GLFW_KEY_J,
        GLFW_KEY_K,
        GLFW_KEY_L,
        GLFW_KEY_M,
        GLFW_KEY_N,
        GLFW_KEY_O,
        GLFW_KEY_P,
        GLFW_KEY_Q,
        GLFW_KEY_R,
        GLFW_KEY_S,
        GLFW_KEY_T,
        GLFW_KEY_U,
        GLFW_KEY_V,
        GLFW_KEY_W,
        GLFW_KEY_X,
        GLFW_KEY_Y,
        GLFW_KEY_Z,
        GLFW_KEY_LEFT_BRACKET,
        GLFW_KEY_BACKSLASH,
        GLFW_KEY_RIGHT_BRACKET,
        GLFW_KEY_GRAVE_ACCENT,
        GLFW_KEY_WORLD_1,
        GLFW_KEY_WORLD_2,
        GLFW_KEY_ESCAPE,
        GLFW_KEY_ENTER,
        GLFW_KEY_TAB,
        GLFW_KEY_BACKSPACE,
        GLFW_KEY_INSERT,
        GLFW_KEY_DELETE,
        GLFW_KEY_RIGHT,
        GLFW_KEY_LEFT,
        GLFW_KEY_DOWN,
        GLFW_KEY_UP,
        GLFW_KEY_PAGE_UP,
        GLFW_KEY_PAGE_DOWN,
        GLFW_KEY_HOME,
        GLFW_KEY_END,
        GLFW_KEY_CAPS_LOCK,
        GLFW_KEY_SCROLL_LOCK,
        GLFW_KEY_NUM_LOCK,
        GLFW_KEY_PRINT_SCREEN,
        GLFW_KEY_PAUSE,
        GLFW_KEY_F1,
        GLFW_KEY_F2,
        GLFW_KEY_F3,
        GLFW_KEY_F4,
        GLFW_KEY_F5,
        GLFW_KEY_F6,
        GLFW_KEY_F7,
        GLFW_KEY_F8,
        GLFW_KEY_F9,
        GLFW_KEY_F10,
        GLFW_KEY_F11,
        GLFW_KEY_F12,
        GLFW_KEY_F13,
        GLFW_KEY_F14,
        GLFW_KEY_F15,
        GLFW_KEY_F16,
        GLFW_KEY_F17,
        GLFW_KEY_F18,
        GLFW_KEY_F19,
        GLFW_KEY_F20,
        GLFW_KEY_F21,
        GLFW_KEY_F22,
        GLFW_KEY_F23,
        GLFW_KEY_F24,
        GLFW_KEY_F25,
        GLFW_KEY_KP_0,
        GLFW_KEY_KP_1,
        GLFW_KEY_KP_2,
        GLFW_KEY_KP_3,
        GLFW_KEY_KP_4,
        GLFW_KEY_KP_5,
        GLFW_KEY_KP_6,
        GLFW_KEY_KP_7,
        GLFW_KEY_KP_8,
        GLFW_KEY_KP_9,
        GLFW_KEY_KP_DECIMAL,
        GLFW_KEY_KP_DIVIDE,
        GLFW_KEY_KP_MULTIPLY,
        GLFW_KEY_KP_SUBTRACT,
        GLFW_KEY_KP_ADD,
        GLFW_KEY_KP_ENTER,
        GLFW_KEY_KP_EQUAL,
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_LEFT_CONTROL,
        GLFW_KEY_LEFT_ALT,
        GLFW_KEY_LEFT_SUPER,
        GLFW_KEY_RIGHT_SHIFT,
        GLFW_KEY_RIGHT_CONTROL,
        GLFW_KEY_RIGHT_ALT,
        GLFW_KEY_RIGHT_SUPER,
        GLFW_KEY_MENU,
    };

    return Map[EnumCast(key)];
}

static int ToGlfwCursorMode(CursorMode mode)
{
    constexpr ea::array Map
    {
        -1,
        GLFW_CURSOR_NORMAL,
        GLFW_CURSOR_DISABLED
    };

    return Map[EnumCast(mode)];
}

static int ToGlfwMouseButton(MouseButton button)
{
    constexpr ea::array Map
    {
        -1,
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
        GLFW_MOUSE_BUTTON_MIDDLE,
    };

    return Map[EnumCast(button)];
}

static CursorMode FromGlfwCursorMode(int mode)
{
    switch (mode)
    {
    case GLFW_CURSOR_NORMAL: return CursorMode::Normal;
    case GLFW_CURSOR_DISABLED: return CursorMode::Lock;
    }
    
    ASSERT(false);
    return CursorMode(0);
}

Window::~Window()
{
    if (m_impl)
    {
        glfwDestroyWindow(m_impl->window);
    }

    delete m_impl;
}

Window::Window(uint2 size, const char* name, bool fullScreen)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    int width = ToI32(size.x);
    int height = ToI32(size.y);
    GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : nullptr;

    auto impl = new WindowImpl();
    impl->window = glfwCreateWindow(width, height, name, monitor, nullptr);

    m_impl = impl;
}

bool Window::IsNull() const
{
    return m_impl == nullptr;
}

bool Window::IsOpen() const
{
    AssertNotNull();
    return !glfwWindowShouldClose(m_impl->window);
}

uint2 Window::GetSize() const
{
    AssertNotNull();

    int x = 0;
    int y = 0;
    glfwGetWindowSize(m_impl->window, &x, &y);

    return uint2(ToU32(x), ToU32(y));
}

uint2 Window::GetPosition() const
{
    AssertNotNull();

    int x = 0;
    int y = 0;
    glfwGetWindowPos(m_impl->window, &x, &y);
    
    return uint2(ToU32(x), ToU32(y));
}

void Window::Close()
{
    AssertNotNull();
    glfwSetWindowShouldClose(m_impl->window, true);
}

void Window::SetCursorMode(CursorMode mode)
{
    AssertNotNull();
    ASSERT(mode != CursorMode::Null);
    glfwSetInputMode(m_impl->window, GLFW_CURSOR, ToGlfwCursorMode(mode));
}

CursorMode Window::GetCursorMode() const
{
    AssertNotNull();
    return FromGlfwCursorMode(glfwGetInputMode(m_impl->window, GLFW_CURSOR));
}

void Window::SetMousePos(float2 position)
{
    AssertNotNull();
    glfwSetCursorPos(m_impl->window, position.x, position.y);
}

void Window::PollEvents(void)
{
    AssertNotNull();

    glfwPollEvents();
    m_impl->prevMousePos = m_impl->mousePos;

    double mouseX = 0;
    double mouseY = 0;
    glfwGetCursorPos(m_impl->window, &mouseX, &mouseY);

    m_impl->mousePos = float2(ToF32(mouseX), ToF32(mouseY));
}

float2 Window::GetMousePos() const
{
    AssertNotNull();
    return m_impl->mousePos;
}

float2 Window::GetMouseDelta() const
{
    AssertNotNull();

    float2 p1 = m_impl->prevMousePos;
    float2 p2 = GetMousePos();

    return p2 - p1;
}

bool Window::IsKeyPressed(KeyCode key) const
{
    AssertNotNull();
    ASSERT(key != KeyCode::Null);
    return glfwGetKey(m_impl->window, ToGlfwKey(key)) == GLFW_PRESS;
}

bool Window::IsMouseButtonPressed(MouseButton button) const
{
    AssertNotNull();
    ASSERT(button != MouseButton::Null);
    return glfwGetMouseButton(m_impl->window, ToGlfwMouseButton(button)) == GLFW_PRESS;
}

VkResult Window::CreateVkSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* pHandle) const
{
    AssertNotNull();

    return glfwCreateWindowSurface(instance, m_impl->window, allocator, pHandle);
}

GLFWwindow* Window::GetGLFWWindow()
{
    AssertNotNull();
    return m_impl->window;
}

static void OnGlfwError(int error, const char* description)
{
    fmt::print("GLFW Error [{}] : {}\n", error, description);
}

void Window::InitSharedContext()
{
    glfwSetErrorCallback(OnGlfwError);

    int result = glfwInit();
    if (!result)
    {
        throw std::runtime_error("Failed to initialize Window shared context");
    }

    if (!glfwVulkanSupported())
    {
        throw std::runtime_error("Failed to initialize Window shared context. Vulkan not supported");
    }
}

void Window::UninitSharedContext()
{
    glfwTerminate();
}

ea::span<const char*> Window::GetVkRequiredInstanceExtensions()
{
    uint32 glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    return ea::span<const char*>(glfwExtensions, glfwExtensionCount);
}

void Window::AssertNotNull() const
{
    ASSERT(!IsNull());
}





static KeyState UpdateKeyReleased(const Window& window, KeyCode key)
{
    if (window.IsKeyPressed(key))
    {
        return KeyState::Down;
    }
    else
    {
        return KeyState::Released;
    }
}

static KeyState UpdateKeyDown(const Window&, KeyCode)
{
    return KeyState::Pressed;
}

static KeyState UpdateKeyPressed(const Window& window, KeyCode key)
{
    if (window.IsKeyPressed(key))
    {
        return KeyState::Pressed;
    }
    else
    {
        return KeyState::Up;
    }
}

static KeyState UpdateKeyUp(const Window&, KeyCode)
{
    return KeyState::Released;
}

using FnUpdateKeyCode = KeyState(*)(const Window&, KeyCode);

constexpr ea::array<FnUpdateKeyCode, KeyStateCount> UpdateKeyCodeFuncs =
{
    nullptr,
    UpdateKeyReleased,
    UpdateKeyDown,
    UpdateKeyPressed,
    UpdateKeyUp,
};

Keyboard::Keyboard()
{
    m_states.fill(KeyState::Released);
}

void Keyboard::UpdateKeyStates(const Window& window)
{
    for (uint32 i = 1; i < KeyCodeCount; i++)
    {
        KeyState state = m_states[i];
        FnUpdateKeyCode fnUpdate = UpdateKeyCodeFuncs[EnumCast(state)];

        m_states[i] = fnUpdate(window, EnumCast<KeyCode>(i));
    }
}

KeyState Keyboard::GetKeyState(KeyCode key) const
{
    return m_states[EnumCast(key)];
}

bool Keyboard::IsReleased(KeyCode key) const
{
    KeyState state = GetKeyState(key);
    return state == KeyState::Released || state == KeyState::Up;
}

bool Keyboard::IsDown(KeyCode key) const
{
    KeyState state = GetKeyState(key);
    return state == KeyState::Down;
}

bool Keyboard::IsPressed(KeyCode key) const
{
    KeyState state = GetKeyState(key);
    return state == KeyState::Pressed || state == KeyState::Down;
}

bool Keyboard::IsUp(KeyCode key) const
{
    KeyState state = GetKeyState(key);
    return state == KeyState::Up;
}