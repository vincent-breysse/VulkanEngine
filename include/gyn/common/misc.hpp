#pragma once

#include <gyn/common/pch.hpp>
#include <gyn/common/config.hpp>
#include <gyn/common/error.hpp>
#include <gyn/common/cast.hpp>

namespace Gyn
{
    struct NonCopyable
    {
        NonCopyable(void) = default;
        ~NonCopyable(void) = default;

        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = default;

        NonCopyable& operator = (const NonCopyable&) = delete;
        NonCopyable& operator = (NonCopyable&&) = default;
    };

    struct NoCopyMove
    {
        NoCopyMove(void) = default;
        ~NoCopyMove(void) = default;

        NoCopyMove(const NoCopyMove&) = delete;
        NoCopyMove(NoCopyMove&&) = delete;

        NoCopyMove& operator = (const NoCopyMove&) = delete;
        NoCopyMove& operator = (NoCopyMove&&) = delete;
    };

    enum class TypeID : uintptr {};

    template <typename T>
    TypeID GetTypeID()
    {
        return TypeID(reinterpret_cast<uintptr>(&typeid(T)));
    }

    inline const std::type_info* GetTypeInfo(TypeID id)
    {
        return reinterpret_cast<const std::type_info*>(id);
    }

    template <uint64 P>
    constexpr bool IsPowerOf2()
    {
        return (P & (P - 1)) == 0;
    }

    template <typename T>
    bool IsPowerOf2Generic(T p)
    {
        return (p & (p - 1)) == 0;
    }

    inline bool IsPowerOf2(uint32 p)
    {
        return IsPowerOf2Generic(p);
    }

    inline bool IsPowerOf2(uint64 p)
    {
        return IsPowerOf2Generic(p);
    }

    template <usize N>
    bool IsAlignedPowerOf2(const byte* ptr)
    {
        static_assert(N > 0, "P must not be 0");
        static_assert(IsPowerOf2<N>(), "P must be a power of 2");

        return (reinterpret_cast<uintptr>(ptr) & (N - 1)) == 0;
    }

    inline bool IsAligned16(const byte* ptr)
    {
        return IsAlignedPowerOf2<16>(ptr);
    }

    template <int64 P>
    uint64 RoundUpPowerOf2(uint64 x)
    {
        static_assert(P > 0, "P must not be 0");
        static_assert(IsPowerOf2<P>(), "P must be a power of 2");

        return (x + P - 1) & -P;
    }

    inline uint64 RoundUp16(uint64 x)
    {
        return RoundUpPowerOf2<16>(x);
    }

    inline uint32 DivideRoundUp(uint32 numerator, uint32 denominator)
    {
        return (numerator + denominator - 1) / denominator;
    }

    inline void ThreadYield()
    {
        std::this_thread::yield();
    }

    inline void ThreadSleep1s()
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }

    inline void ThreadSleep1ms()
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1e-3s);
    }

    inline void ThreadSleep1us()
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1e-6s);
    }

    inline void ThreadSleep1ns()
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1e-9s);
    }

    inline sf::Color ToSfColor(float4 v)
    {
        v *= 255.f;
        sf::Color color(ToU8(v.x), ToU8(v.y), ToU8(v.z), ToU8(v.w));

        return color;
    }

    inline sf::Color ToSfColorOpaque(float3 v)
    {
        return ToSfColor(float4(v, 1.f));
    }

    template <typename T>
    void MaybeUnused(const T&) {}
}

#define GYN_ENUM_CLASS_FLAG_MANIPS(Enum) \
inline Enum operator~(Enum e) \
{ \
    using ::Gyn::EnumCast; \
    return EnumCast<Enum>(~EnumCast(e)); \
} \
\
inline Enum operator&(Enum l, Enum r) \
{ \
    using ::Gyn::EnumCast; \
    return EnumCast<Enum>(EnumCast(l) & EnumCast(r)); \
} \
inline Enum operator|(Enum l, Enum r) \
{ \
    using ::Gyn::EnumCast; \
    return EnumCast<Enum>(EnumCast(l) | EnumCast(r)); \
} \
inline Enum operator^(Enum l, Enum r) \
{ \
    using ::Gyn::EnumCast; \
    return EnumCast<Enum>(EnumCast(l) ^ EnumCast(r)); \
} \
inline Enum operator&=(Enum& l, Enum r) \
{ \
    return l = l & r; \
} \
inline Enum operator|=(Enum& l, Enum r) \
{ \
    return l = l | r; \
} \
inline Enum operator^=(Enum& l, Enum r) \
{ \
    return l = l ^ r; \
} \
inline bool Any(Enum e) \
{ \
    return !!EnumCast(e); \
}

#ifdef GYN_MACROS_CONVENIENCE
#define ENUM_CLASS_FLAG_MANIPS GYN_ENUM_CLASS_FLAG_MANIPS
#endif

#define GYN_STATIC_ASSERT_FAILURE(message) static_assert(false, "From " GYN_FUNC " : " message)

#define GYN_STATIC_ASSERT(condition, message) static_assert(!!(condition), "Assertion [" #condition"] failed From " GYN_FUNC " : " message)

#define GYN_STATIC_ASSERT_TRIVIALLY_COPYABLE(T) static_assert(std::is_trivially_copyable_v<T>, \
    "Type " #T " provided to " GYN_FUNC " must be trivially copyable")

#define GYN_STATIC_ASSERT_DEFAULT_CONSTRUCTIBLE(T) static_assert(std::is_default_constructible_v<T>, \
    "Type " #T " provided to " GYN_FUNC " must be default constructible")

#define GYN_STATIC_ASSERT_IS_CLASS(T) \
GYN_STATIC_ASSERT(std::is_class_v<T>, #T" must be either a class or a struct.")

#define GYN_STATIC_ASSERT_IS_BASE_OF(Base, Derived) \
GYN_STATIC_ASSERT((std::is_base_of_v<Base, Derived>), #Derived" must inherit from " #Base)
