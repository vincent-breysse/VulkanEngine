#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/config.hpp>
#include <gyn/common/error.hpp>

namespace Gyn
{
    template <typename TOut, typename TIn>
    TOut* ReinterpretCast(TIn* in)
    {
        static_assert(sizeof(TIn) == sizeof(TOut), "Sizeof mismatch when calling ReinterpretCast");
        static_assert(alignof(TIn) == alignof(TOut), "Alignof mismatch when calling ReinterpretCast");

        return reinterpret_cast<TOut*>(in);
    }

    template <typename TOut, typename TIn>
    const TOut* ReinterpretCast(const TIn* in)
    {
        static_assert(sizeof(TIn) == sizeof(TOut), "Sizeof mismatch when calling ReinterpretCast");
        static_assert(alignof(TIn) == alignof(TOut), "Alignof mismatch when calling ReinterpretCast");

        return reinterpret_cast<const TOut*>(in);
    }

    template <typename EnumT>
    constexpr std::underlying_type_t<EnumT> EnumCast(EnumT x)
    {
        return static_cast<std::underlying_type_t<EnumT>>(x);
    }

    template <typename EnumT>
    constexpr EnumT EnumCast(std::underlying_type_t<EnumT> x)
    {
        return static_cast<EnumT>(x);
    }

    template <typename TOut, typename TIn>
    TOut NarrowingCast(TIn i)
    {
        constexpr auto Min = std::is_unsigned_v<TIn> || std::is_unsigned_v<TOut> ? 0 : std::numeric_limits<TOut>::lowest();
        constexpr auto Max = std::numeric_limits<TOut>::max();

        GYN_ASSERT(i >= Min);
        GYN_ASSERT(i <= Max);

        return static_cast<TOut>(i);
    }

    inline uint64 ToU64(uint64 i)
    {
        return i;
    }

    inline uint64 ToU64(uint32 i)
    {
        return i;
    }

    inline uint64 ToU64(uint16 i)
    {
        return i;
    }

    inline uint64 ToU64(uint8 i)
    {
        return i;
    }

    inline uint64 ToU64(int64 i)
    {
        return NarrowingCast<uint64>(i);
    }

    inline uint64 ToU64(int32 i)
    {
        return NarrowingCast<uint64>(i);
    }

    inline uint64 ToU64(int16 i)
    {
        return NarrowingCast<uint64>(i);
    }

    inline uint64 ToU64(int8 i)
    {
        return NarrowingCast<uint64>(i);
    }

    inline uint64 ToU64(float i)
    {
        return NarrowingCast<uint64>(i);
    }

    inline uint64 ToU64(double i)
    {
        return NarrowingCast<uint64>(i);
    }





    inline uint32 ToU32(uint64 i)
    {
        return NarrowingCast<uint32>(i);
    }

    inline uint32 ToU32(uint32 i)
    {
        return i;
    }

    inline uint32 ToU32(uint16 i)
    {
        return i;
    }

    inline uint32 ToU32(uint8 i)
    {
        return i;
    }

    inline uint32 ToU32(int64 i)
    {
        return NarrowingCast<uint32>(i);
    }

    inline uint32 ToU32(int32 i)
    {
        return NarrowingCast<uint32>(i);
    }

    inline uint32 ToU32(int16 i)
    {
        return NarrowingCast<uint32>(i);
    }

    inline uint32 ToU32(int8 i)
    {
        return NarrowingCast<uint32>(i);
    }

    inline uint32 ToU32(float i)
    {
        return NarrowingCast<uint32>(i);
    }

    inline uint32 ToU32(double i)
    {
        return NarrowingCast<uint32>(i);
    }




    inline uint16 ToU16(uint64 i)
    {
        return NarrowingCast<uint16>(i);
    }

    inline uint16 ToU16(uint32 i)
    {
        return NarrowingCast<uint16>(i);
    }

    inline uint16 ToU16(uint16 i)
    {
        return i;
    }

    inline uint16 ToU16(uint8 i)
    {
        return i;
    }

    inline uint16 ToU16(int64 i)
    {
        return NarrowingCast<uint16>(i);
    }

    inline uint16 ToU16(int32 i)
    {
        return NarrowingCast<uint16>(i);
    }

    inline uint16 ToU16(int16 i)
    {
        return NarrowingCast<uint16>(i);
    }

    inline uint16 ToU16(float i)
    {
        return NarrowingCast<uint16>(i);
    }

    inline uint16 ToU16(double i)
    {
        return NarrowingCast<uint16>(i);
    }




    inline uint8 ToU8(uint64 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(uint32 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(uint16 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(uint8 i)
    {
        return i;
    }

    inline uint8 ToU8(int64 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(int32 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(int16 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(int8 i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(float i)
    {
        return NarrowingCast<uint8>(i);
    }

    inline uint8 ToU8(double i)
    {
        return NarrowingCast<uint8>(i);
    }





    inline int64 ToI64(uint64 i)
    {
        return NarrowingCast<int64>(i);
    }

    inline int64 ToI64(uint32 i)
    {
        return NarrowingCast<int64>(i);
    }

    inline int64 ToI64(uint16 i)
    {
        return NarrowingCast<int64>(i);
    }

    inline int64 ToI64(uint8 i)
    {
        return NarrowingCast<int64>(i);
    }

    inline int64 ToI64(int64 i)
    {
        return i;
    }

    inline int64 ToI64(int32 i)
    {
        return i;
    }

    inline int64 ToI64(int16 i)
    {
        return i;
    }

    inline int64 ToI64(int8 i)
    {
        return i;
    }

    inline int64 ToI64(float i)
    {
        return NarrowingCast<int64>(i);
    }

    inline int64 ToI64(double i)
    {
        return NarrowingCast<int64>(i);
    }





    inline int32 ToI32(uint64 i)
    {
        return NarrowingCast<int32>(i);
    }

    inline int32 ToI32(uint32 i)
    {
        return NarrowingCast<int32>(i);
    }

    inline int32 ToI32(uint16 i)
    {
        return NarrowingCast<int32>(i);
    }

    inline int32 ToI32(uint8 i)
    {
        return NarrowingCast<int32>(i);
    }

    inline int32 ToI32(int64 i)
    {
        return NarrowingCast<int32>(i);
    }

    inline int32 ToI32(int32 i)
    {
        return i;
    }

    inline int32 ToI32(int16 i)
    {
        return i;
    }

    inline int32 ToI32(int8 i)
    {
        return i;
    }

    inline int32 ToI32(float i)
    {
        return NarrowingCast<int32>(i);
    }

    inline int32 ToI32(double i)
    {
        return NarrowingCast<int32>(i);
    }





    inline int16 ToI16(uint64 i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(uint32 i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(uint16 i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(uint8 i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(int64 i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(int32 i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(int16 i)
    {
        return i;
    }

    inline int16 ToI16(int8 i)
    {
        return i;
    }

    inline int16 ToI16(float i)
    {
        return NarrowingCast<int16>(i);
    }

    inline int16 ToI16(double i)
    {
        return NarrowingCast<int16>(i);
    }





    inline int8 ToI8(uint64 i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(uint32 i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(uint16 i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(int64 i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(int32 i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(int16 i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(int8 i)
    {
        return i;
    }

    inline int8 ToI8(float i)
    {
        return NarrowingCast<int8>(i);
    }

    inline int8 ToI8(double i)
    {
        return NarrowingCast<int8>(i);
    }




    inline double ToF64(uint64 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(uint32 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(uint16 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(uint8 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(int64 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(int32 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(int16 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(int8 i)
    {
        return NarrowingCast<double>(i);
    }

    inline double ToF64(float i)
    {
        return i;
    }

    inline double ToF64(double i)
    {
        return i;
    }





    inline float ToF32(uint64 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(uint32 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(uint16 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(uint8 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(int64 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(int32 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(int16 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(int8 i)
    {
        return NarrowingCast<float>(i);
    }

    inline float ToF32(float i)
    {
        return i;
    }

    inline float ToF32(double i)
    {
        return NarrowingCast<float>(i);
    }
}