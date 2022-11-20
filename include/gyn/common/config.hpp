#pragma once

#include <cstdint>
#include <cstddef>


#ifdef _MSC_VER
#define GYN_MSVC
#endif

#ifdef __clang__
#define GYN_CLANG
#endif

#ifdef __GNUC__
#define GYN_GCC
#endif

#if defined(__MINGW32__) ||  defined(__MINGW64__)
#define GYN_MINGW
#endif



#ifdef GYN_MSVC

#define GYN_DLL_EXPORT __declspec(dllexport)
#define GYN_DLL_IMPORT __declspec(dllimport)

#define GYN_COUNTER __COUNTER__

#define GYN_FUNC __FUNCTION__
#define GYN_FULL_FUNC __FUNCSIG__

#define GYN_LINE __LINE__
#define GYN_FILE __FILE__

#define GYN_FORCE_INLINE __forceinline
#define GYN_NO_INLINE __declspec(noinline)

#elif defined(GYN_CLANG) || defined(GYN_GCC)

#define GYN_DLL_EXPORT
#define GYN_DLL_IMPORT

#define GYN_COUNTER __COUNTER__

#define GYN_FUNC __func__
#define GYN_FULL_FUNC __PRETTY_FUNCTION__

#define GYN_LINE __LINE__
#define GYN_FILE __FILE__

#define GYN_FORCE_INLINE __attribute__((always_inline))
#define GYN_NO_INLINE __attribute__((noinline))

#endif

#if !defined(NDEBUG)

#define GYN_DEBUG 1

#else

#define GYN_DEBUG 0

#endif

#define GYN_RTTI GYN_DEBUG

namespace Gyn
{
    using int8 = int8_t;
    using uint8 = uint8_t;

    using int16 = int16_t;
    using uint16 = uint16_t;

    using int32 = int32_t;
    using uint32 = uint32_t;

    using int64 = int64_t;
    using uint64 = uint64_t;

    using intptr = intptr_t;
    using uintptr = uintptr_t;

    using usize = size_t;

    using schar = signed char;
    using uchar = unsigned char;
    using wchar = wchar_t;
    using char16 = char16_t;
    using char32 = char32_t;
    using ushort = unsigned short;
    using uint = unsigned int;
    using ulong = unsigned long;
    using llong = long long;
    using ullong = unsigned long long;
    using ldouble = long double;

    using byte = uint8;

    constexpr int8 Int8Min = INT8_MIN;
    constexpr int16 Int16Min = INT16_MIN;
    constexpr int32 Int32Min = INT32_MIN;
    constexpr int64 Int64Min = INT64_MIN;

    constexpr int8 Int8Max = INT8_MAX;
    constexpr int16 Int16Max = INT16_MAX;
    constexpr int32 Int32Max = INT32_MAX;
    constexpr int64 Int64Max = INT64_MAX;

    constexpr uint8 Uint8Max = UINT8_MAX;
    constexpr uint16 Uint16Max = UINT16_MAX;
    constexpr uint32 Uint32Max = UINT32_MAX;
    constexpr uint64 Uint64Max = UINT64_MAX;

    constexpr intptr IntptrMin = INTPTR_MIN;
    constexpr intptr IntptrMax = INTPTR_MAX;
    constexpr uintptr UintptrMax = UINTPTR_MAX;

    constexpr usize UsizeMax = SIZE_MAX;

    constexpr float FloatMin = FLT_MIN;
    constexpr float FloatMax = FLT_MAX;
    constexpr float FloatEpsilon = FLT_EPSILON;
}