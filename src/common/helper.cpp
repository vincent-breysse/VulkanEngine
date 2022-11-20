
#include <pch.hpp>
#include <gyn/common.hpp>

template <typename T>
static void CASLoopGeneric(std::atomic<T>& a, T expected, T desired)
{
    T x;
    do
    {
        x = expected;

    } while (!a.compare_exchange_weak(x, desired));
}

uint32 Gyn::AtomicFetchAdd(uint32& x)
{
    return reinterpret_cast<std::atomic_uint32_t&>(x)++;
}

void Gyn::CASLoop(std::atomic_uint32_t& a, uint32 expected, uint32 desired)
{
    CASLoopGeneric<uint32>(a, expected, desired);
}

void Gyn::CASLoop(std::atomic_uint64_t& a, uint64 expected, uint64 desired)
{
    CASLoopGeneric<uint64>(a, expected, desired);
}

bool Gyn::AtomicFetchSubIfNot0(std::atomic_uint32_t& a)
{
    uint32 x;
    do
    {
        x = a.load();

        if (x == 0) return false;

    } while (!a.compare_exchange_weak(x, x - 1));

    return true;
}