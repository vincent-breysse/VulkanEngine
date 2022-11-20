
#include <pch.hpp>
#include <gyn/common/eastl.hpp>

static constexpr bool AllocationDebug = false;

void* Gyn::OperatorNewArray(size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    if constexpr (AllocationDebug)
    {
        fmt::print("{} Allocation\n=======================\nBytes : {}\nFlags : {}, Debug flags : {}\nFile : {}\nLine : {}\n\n",
            pName, size, flags, debugFlags, file, line);
    }

    return ::operator new[](size);
}

void* Gyn::OperatorNewArray(size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    static_assert(__STDCPP_DEFAULT_NEW_ALIGNMENT__ == 16);
    ASSERT(alignment <= 16);
    ASSERT(alignmentOffset == 0);
    return ::operator new[](size, pName, flags, debugFlags, file, line);
}