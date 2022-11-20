
#include <pch.hpp>
#include <gyn/common/threaded_ring_buffer.hpp>
#include <gyn/common/atomic.hpp>

ThreadedRingBuffer::ThreadedRingBuffer(uint32 capacity) :
    m_items(capacity),
    m_capacity(capacity),
    m_pushLeft(capacity),
    m_popLeft(0)
{
    ASSERT(capacity > 1);
    ASSERT(IsPowerOf2(capacity));
}

bool ThreadedRingBuffer::TryPush(uint32 item)
{
    if (!AtomicFetchSubIfNot0(m_pushLeft))
    {
        return false;
    }

    DoPush(item);

    return true;
}

void ThreadedRingBuffer::Push(uint32 item)
{
    ASSERT(m_pushLeft.load() > 0);
    m_pushLeft.fetch_sub(1);
    ASSERT(m_pushLeft.load() >= 0);

    DoPush(item);
}

bool ThreadedRingBuffer::TryPop(uint32& outItem)
{
    if (!AtomicFetchSubIfNot0(m_popLeft))
    {
        return false;
    }

    outItem = DoPop();
    return true;
}

uint32 ThreadedRingBuffer::Pop()
{
    ASSERT(m_popLeft.load() > 0);
    m_popLeft.fetch_sub(1);
    ASSERT(m_popLeft.load() >= 0);

    return DoPop();
}

uint32 ThreadedRingBuffer::Size() const
{
    return m_popLeft.load();
}

uint32 ThreadedRingBuffer::Capacity() const
{
    return m_capacity.load();
}

bool ThreadedRingBuffer::IsEmpty() const
{
    return Size() == 0;
}

bool ThreadedRingBuffer::IsFull() const
{
    return Size() == Capacity();
}

void ThreadedRingBuffer::DoPush(uint32 item)
{
    uint32 capacity = m_capacity.load();
    uint32 pushOrder = m_nextPushOrder.fetch_add(1);

    while (m_currentPushOrder.load() != pushOrder)
    {
    }

    uint32 push = m_push.fetch_add(1);
    m_items[push & (capacity - 1)] = item;

    m_popLeft.fetch_add(1);
    m_currentPushOrder.fetch_add(1);
}

uint32 ThreadedRingBuffer::DoPop()
{
    uint32 capacity = m_capacity.load();
    uint32 popOrder = m_nextPopOrder.fetch_add(1);

    while (m_currentPopOrder.load() != popOrder)
    {
    }

    uint32 pop = m_pop.fetch_add(1);
    uint32 item = m_items[pop & (capacity - 1)];

    m_pushLeft.fetch_add(1);
    m_currentPopOrder.fetch_add(1);

    return item;
}