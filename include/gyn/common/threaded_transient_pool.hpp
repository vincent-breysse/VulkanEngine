#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/misc.hpp>

namespace Gyn
{
    class ThreadedTransientPool : NoCopyMove
    {
    public:

        static constexpr usize PageCount = 16;

        ThreadedTransientPool() = default;

        ThreadedTransientPool(usize pageSize);

        byte* Allocate(usize wantedAllocSize);

        void Deallocate(const byte* ptr);

        template <typename T>
        T& Create();

        template <typename T>
        void Destroy(T* ptr);

    private:

        ea::vector<byte> m_data = {};
        std::atomic_size_t m_pageSize = {};
        std::atomic_size_t m_nextAlloc = {};
        std::atomic_size_t m_endAlloc = {};
        std::atomic_size_t m_order = {};
        std::atomic_uint32_t m_pageAllocCounts[PageCount] = {};
    };

    template <typename T>
    T& ThreadedTransientPool::Create()
    {
        byte* ptr = Allocate(sizeof(T));
        new (ptr) T();

        return reinterpret_cast<T&>(*ptr);
    }

    template <typename T>
    void ThreadedTransientPool::Destroy(T* ptr)
    {
        ASSERT(ptr);

        ptr->~T();
        Deallocate(reinterpret_cast<byte*>(ptr));
    }
}