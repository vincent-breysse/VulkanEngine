
#include <pch.hpp>
#include <gyn/common/threaded_transient_pool.hpp>
#include <gyn/common/atomic.hpp>

ThreadedTransientPool::ThreadedTransientPool(usize pageSize)
{
    ASSERT(IsPowerOf2(pageSize));

    m_data.resize(pageSize * PageCount);
    m_pageSize.store(pageSize);
    m_endAlloc.store(pageSize);
}

byte* ThreadedTransientPool::Allocate(usize wantedAllocSize)
{
    usize allocSize = RoundUp16(wantedAllocSize);
    usize pageSize = m_pageSize.load();
    ASSERT_MSG(allocSize < (pageSize / 2),
        "An allocation shouldn't exceed half the size of a page");

    usize begin = m_nextAlloc.fetch_add(allocSize);
    usize end = begin + allocSize;
    usize beginPageIndexAbs = begin / pageSize;

    // Overlap between 2 pages. Just reallocate in this case
    while (beginPageIndexAbs != (end / pageSize))
    {
        CASLoop(m_order, begin, end);

        begin = m_nextAlloc.fetch_add(allocSize);
        end = begin + allocSize;
        beginPageIndexAbs = begin / pageSize;
    }

    usize pageIndex = beginPageIndexAbs & (PageCount - 1);
    usize endAlloc = m_endAlloc.load();

    if (end > endAlloc)
    {
        while (begin > m_order.load())
        {
        }

        endAlloc = m_endAlloc.load();

        if (end > endAlloc)
        {
            uint32 pageAllocCount = m_pageAllocCounts[pageIndex].load();

            if (pageAllocCount > 0)
            {
                fmt::print("ThreadedTransientPool Warning :\n"
                    "=================================================\n"
                    "Thread [{}] stalled because it tried to switch to page [{}] that still contains {} allocations.\n"
                    "You should either consider increasing the page size or moving the allocations meant to last to another container.\n"
                    "Current page size is {} bytes\n",
                    std::this_thread::get_id(), pageIndex, pageAllocCount, pageSize);
            }

            pageAllocCount = m_pageAllocCounts[pageIndex].load();

            while (pageAllocCount > 0)
            {
                pageAllocCount = m_pageAllocCounts[pageIndex].load();
            }

            endAlloc = m_endAlloc.fetch_add(pageSize) + pageSize;
        }

        ASSERT(end <= endAlloc);
    }

    m_pageAllocCounts[pageIndex].fetch_add(1);
    m_order.fetch_add(allocSize);

    byte* allocatedPtr = m_data.data() + (begin & (m_data.size() - 1));
    ASSERT(allocatedPtr >= m_data.data() && ((allocatedPtr + allocSize) < m_data.end()));
    ASSERT(IsAligned16(allocatedPtr));

    return allocatedPtr;
}

void ThreadedTransientPool::Deallocate(const byte* ptr)
{
    ASSERT(ptr >= m_data.begin() && ptr < m_data.end());

    usize offset = ptr - m_data.data();
    usize pageIndex = offset / m_pageSize.load();
    m_pageAllocCounts[pageIndex].fetch_sub(1);
}
