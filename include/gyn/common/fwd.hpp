#pragma once

#include <gyn/common/pch.hpp>

namespace eastl
{
    template <typename T>
    struct hash;

    template <typename T>
    struct default_delete;

    template <typename T>
    class shared_ptr;

    template <typename T, typename Deleter = eastl::default_delete<T> >
    class unique_ptr;

    template <typename T, size_t Extent = eastl::dynamic_extent>
    class span;

    template <typename T, typename Allocator = EASTLAllocatorType>
    class vector;

    template <typename T, typename Allocator = EASTLAllocatorType>
    class basic_string;

    typedef basic_string<char>    string;
    typedef basic_string<wchar_t> wstring;

    /// custom string8 / string16 / string32
    typedef basic_string<char>     string8;
    typedef basic_string<char16_t> string16;
    typedef basic_string<char32_t> string32;

    /// ISO mandated string types
    typedef basic_string<char8_t>  u8string;    // Actually not a C++11 type, but added for consistency.
    typedef basic_string<char16_t> u16string;
    typedef basic_string<char32_t> u32string;
}

namespace Gyn
{
    class Window;
    class Keyboard;
    class TimeManager;
    struct Engine;
    struct World;

    class GPU;
    class GPUBuffer;
    class GPUImage;
    class GPUImageView;
    class GPUBindSet;
    class GPUFramebuffer;
    class GPUShader;
    class GPURenderPass;
    class GPUSwapchain;
    class GPUFence;
    struct Vertex;

    class EntityManager;
    class EntityArchetype;
    class EntityChunk;
    class Entity;
    class ECSSystem;
    class JobScheduler;
    struct Job;

    struct LocalToWorld;

    template <typename T>
    using SharedPtr = ea::shared_ptr<T>;

    template <typename T, typename Deleter = ea::default_delete<T>>
    using UniquePtr = ea::unique_ptr<T, Deleter>;
}