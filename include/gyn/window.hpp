#pragma once

#include <vulkan/vulkan.h>
#include <gyn/common.hpp>

namespace Gyn
{
    struct WindowImpl;

    enum class KeyCode
    {
        Null,

        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,
        Semicolon,
        Equal,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LeftBracket,
        Backslash,
        RightBracket,
        GraveAccent,
        World1,
        World2,
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        NumPad0,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad7,
        NumPad8,
        NumPad9,
        NumPadDecimal,
        NumPadDivide,
        NumPadMultiply,
        NumPadSubtract,
        NumPadAdd,
        NumPadEnter,
        NumPadEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu,

        EnumEnd
    };
    constexpr uint32 KeyCodeCount = uint32(KeyCode::EnumEnd);

    enum class KeyState
    {
        Null,

        Released,
        Down,
        Pressed,
        Up,

        EnumEnd,
    };
    constexpr uint32 KeyStateCount = uint32(KeyState::EnumEnd);

    enum class CursorMode
    {
        Null,

        Normal,
        Lock,

        EnumEnd
    };
    constexpr uint32 CursorModeCount = uint32(CursorMode::EnumEnd);

    enum class MouseButton
    {
        Null,

        Left,
        Right,
        Middle,

        EnumEnd
    };
    constexpr uint32 MouseButtonCount = uint32(MouseButton::EnumEnd);

    class Window : NoCopyMove
    {
    public:

        Window() = default;

        Window(uint2 size, const char* name, bool fullScreen);

        ~Window();

        bool IsNull() const;

        bool IsOpen() const;

        uint2 GetSize() const;

        uint2 GetPosition() const;

        void Close();

        void SetCursorMode(CursorMode mode);

        CursorMode GetCursorMode() const;

        void SetMousePos(float2 position);

        void PollEvents();

        float2 GetMousePos() const;

        float2 GetMouseDelta() const;

        bool IsKeyPressed(KeyCode key) const;

        bool IsMouseButtonPressed(MouseButton button) const;

        VkResult CreateVkSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* pHandle) const;

        GLFWwindow* GetGLFWWindow();

        static void InitSharedContext();

        static void UninitSharedContext();

        static ea::span<const char*> GetVkRequiredInstanceExtensions();

    private:

        void AssertNotNull() const;

        WindowImpl* m_impl = {};
    };

    class Keyboard
    {
    public:

        Keyboard();

        void UpdateKeyStates(const Window& window);

        KeyState GetKeyState(KeyCode key) const;

        bool IsReleased(KeyCode key) const;

        bool IsDown(KeyCode key) const;

        bool IsPressed(KeyCode key) const;

        bool IsUp(KeyCode key) const;

    private:

        ea::array<KeyState, KeyCodeCount> m_states = {};
    };
}