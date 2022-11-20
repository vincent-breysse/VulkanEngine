#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/misc.hpp>

namespace Gyn
{
    class ThreadedRingBuffer : NoCopyMove
    {
    public:

        ThreadedRingBuffer() = default;

        ~ThreadedRingBuffer() = default;

        ThreadedRingBuffer(uint32 capacity);

        bool TryPush(uint32 item);

        void Push(uint32 item);

        bool TryPop(uint32& outItem);

        uint32 Pop();

        uint32 Size() const;

        uint32 Capacity() const;

        bool IsEmpty() const;

        bool IsFull() const;

    private:

        void DoPush(uint32 item);

        uint32 DoPop();

        ea::vector<std::atomic_uint32_t> m_items = {};
        std::atomic_uint32_t m_capacity = {};
        std::atomic_uint32_t m_push = {};
        std::atomic_uint32_t m_pop = {};

        std::atomic_uint32_t m_pushLeft = {};
        std::atomic_uint32_t m_popLeft = {};

        std::atomic_uint32_t m_nextPopOrder = {};
        std::atomic_uint32_t m_currentPopOrder = {};

        std::atomic_uint32_t m_nextPushOrder = {};
        std::atomic_uint32_t m_currentPushOrder = {};
    };
}