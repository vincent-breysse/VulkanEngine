#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/config.hpp>

namespace Gyn
{
    uint32 AtomicFetchAdd(uint32& x);

    void CASLoop(std::atomic_uint32_t& a, uint32 expected, uint32 desired);

    void CASLoop(std::atomic_uint64_t& a, uint64 expected, uint64 desired);

    bool AtomicFetchSubIfNot0(std::atomic_uint32_t& a);
}