
#pragma once

#ifndef __cplusplus
#error VMath requires C++
#endif

#define VMATH_DIRECTX_MATH_VERSION 316

#if defined(_MSC_VER) && (_MSC_VER < 1910)
#error VMath requires Visual C++ 2017 or later.
#endif

#if defined(_MSC_VER) && !defined(_M_ARM) && !defined(_M_ARM64) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC) && (!_MANAGED) && (!_M_CEE) && (!defined(_M_IX86_FP) || (_M_IX86_FP > 1)) && !defined(VMATH_NO_INTRINSICS) && !defined(VMATH_VECTORCALL)
#define VMATH_VECTORCALL 1
#endif

#if VMATH_VECTORCALL
#define VMATH_CALLCONV __vectorcall
#elif defined(__GNUC__)
#define VMATH_CALLCONV 
#else
#define VMATH_CALLCONV __fastcall
#endif

#if !defined(VMATH_AVX2_INTRINSICS) && defined(__AVX2__) && !defined(VMATH_NO_INTRINSICS)
#define VMATH_AVX2_INTRINSICS
#endif

#if !defined(VMATH_FMA3_INTRINSICS) && defined(VMATH_AVX2_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
#define VMATH_FMA3_INTRINSICS
#endif

#if !defined(VMATH_F16C_INTRINSICS) && defined(VMATH_AVX2_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
#define VMATH_F16C_INTRINSICS
#endif

#if !defined(VMATH_F16C_INTRINSICS) && defined(__F16C__) && !defined(VMATH_NO_INTRINSICS)
#define VMATH_F16C_INTRINSICS
#endif

#if defined(VMATH_FMA3_INTRINSICS) && !defined(VMATH_AVX_INTRINSICS)
#define VMATH_AVX_INTRINSICS
#endif

#if defined(VMATH_F16C_INTRINSICS) && !defined(VMATH_AVX_INTRINSICS)
#define VMATH_AVX_INTRINSICS
#endif

#if !defined(VMATH_AVX_INTRINSICS) && defined(__AVX__) && !defined(VMATH_NO_INTRINSICS)
#define VMATH_AVX_INTRINSICS
#endif

#if defined(VMATH_AVX_INTRINSICS) && !defined(VMATH_SSE4_INTRINSICS)
#define VMATH_SSE4_INTRINSICS
#endif

#if defined(VMATH_SSE4_INTRINSICS) && !defined(VMATH_SSE3_INTRINSICS)
#define VMATH_SSE3_INTRINSICS
#endif

#if defined(VMATH_SSE3_INTRINSICS) && !defined(VMATH_SSE_INTRINSICS)
#define VMATH_SSE_INTRINSICS
#endif

#if !defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
#if (defined(_M_IX86) || defined(_M_X64) || __i386__ || __x86_64__) && !defined(_M_HYBRID_X86_ARM64) && !defined(_M_ARM64EC)
#define VMATH_SSE_INTRINSICS
#elif defined(_M_ARM) || defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __arm__ || __aarch64__
#define VMATH_ARM_NEON_INTRINSICS
#elif !defined(VMATH_NO_INTRINSICS)
#error VMath does not support this target
#endif
#endif // !VMATH_ARM_NEON_INTRINSICS && !VMATH_SSE_INTRINSICS && !VMATH_NO_INTRINSICS

#if defined(VMATH_SSE_INTRINSICS) && defined(_MSC_VER) && (_MSC_VER >= 1920) && !defined(__clang__) && !defined(VMATH_SVML_INTRINSICS) && !defined(VMATH_DISABLE_INTEL_SVML)
#define VMATH_SVML_INTRINSICS
#endif

#include <math.h>
#include <float.h>

#ifndef VMATH_NO_INTRINSICS

#ifdef _MSC_VER
#include <intrin.h>
#endif

#if (defined(__clang__) || defined(__GNUC__)) && (__x86_64__ || __i386__)
#include <cpuid.h>
#endif

#ifdef VMATH_SSE_INTRINSICS
#include <xmmintrin.h>
#include <emmintrin.h>

#ifdef VMATH_SSE3_INTRINSICS
#include <pmmintrin.h>
#endif

#ifdef VMATH_SSE4_INTRINSICS
#include <smmintrin.h>
#endif

#ifdef VMATH_AVX_INTRINSICS
#include <immintrin.h>
#endif

#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_MSC_VER) && !defined(__clang__) && (defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
#include <arm64_neon.h>
#else
#include <arm_neon.h>
#endif
#endif
#endif // !VMATH_NO_INTRINSICS

#include <assert.h>
#include <stdint.h>

#if __cplusplus >= 201703L
#define VMATH_ALIGNED_DATA(x) alignas(x)
#define VMATH_ALIGNED_STRUCT(x) struct alignas(x)
#elif defined(__GNUC__)
#define VMATH_ALIGNED_DATA(x) __attribute__ ((aligned(x)))
#define VMATH_ALIGNED_STRUCT(x) struct __attribute__ ((aligned(x)))
#else
#define VMATH_ALIGNED_DATA(x) __declspec(align(x))
#define VMATH_ALIGNED_STRUCT(x) __declspec(align(x)) struct
#endif

/****************************************************************************
 *
 * Conditional intrinsics
 *
 ****************************************************************************/

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)

#if defined(VMATH_FMA3_INTRINSICS)
#define VMATH_FMADD_PS( a, b, c ) _mm_fmadd_ps((a), (b), (c))
#define VMATH_FNMADD_PS( a, b, c ) _mm_fnmadd_ps((a), (b), (c))
#else
#define VMATH_FMADD_PS( a, b, c ) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#define VMATH_FNMADD_PS( a, b, c ) _mm_sub_ps((c), _mm_mul_ps((a), (b)))
#endif

#if defined(VMATH_AVX_INTRINSICS) && defined(VMATH_FAVOR_INTEL)
#define VMATH_PERMUTE_PS( v, c ) _mm_permute_ps((v), c )
#else
#define VMATH_PERMUTE_PS( v, c ) _mm_shuffle_ps((v), (v), c )
#endif

#endif // VMATH_SSE_INTRINSICS && !VMATH_NO_INTRINSICS

namespace VMath
{
    constexpr float Pi = 3.141592654f;

    //------------------------------------------------------------------------------
    // Vector intrinsic: Four 32 bit floating point components aligned on a 16 byte
    // boundary and mapped to hardware vector registers

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
    using vfloat4 = __m128;
#elif defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
    using vfloat4 = float32x4_t;
#else
    struct vfloat4
    {
        union
        {
            float       v4f[4];
            uint32_t    v4u[4];
        };
    };
#endif

    //------------------------------------------------------------------------------
    // Matrix type: Sixteen 32 bit floating point components aligned on a
    // 16 byte boundary and mapped to four hardware vector registers

#ifdef VMATH_NO_INTRINSICS
    struct vfloat4x4
#else
    VMATH_ALIGNED_STRUCT(16) vfloat4x4
#endif
    {
        vfloat4x4() = default;

        explicit vfloat4x4(vfloat4 r0, vfloat4 r1, vfloat4 r2, vfloat4 r3);

        explicit vfloat4x4
        (
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33
        );

#ifdef VMATH_NO_INTRINSICS
        union
        {
            vfloat4 r[4];
            float m[4][4];
        };
#else
        vfloat4 r[4];
#endif
    };

    struct vbool4
    {
        vbool4() = default;

        explicit vbool4(uint32_t mask);

        uint32_t mask;
    };


    /****************************************************************************
     *
     * Load operations
     *
     ****************************************************************************/

    vfloat4 VMATH_CALLCONV vLoadBinary32(const uint32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadFloat(const float* pSource);

    vfloat4 VMATH_CALLCONV vLoadBinary32x2(const uint32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadFloat2(const float* pSource);
    vfloat4 VMATH_CALLCONV vLoadCastInt2(const int32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadCastUint2(const uint32_t* pSource);

    vfloat4 VMATH_CALLCONV vLoadBinary32x3(const uint32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadFloat3(const float* pSource);
    vfloat4 VMATH_CALLCONV vLoadCastInt3(const int32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadCastUint3(const uint32_t* pSource);

    vfloat4 VMATH_CALLCONV vLoadBinary32x4(const uint32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadFloat4(const float* pSource);
    vfloat4 VMATH_CALLCONV vLoadCastInt4(const int32_t* pSource);
    vfloat4 VMATH_CALLCONV vLoadCastUint4(const uint32_t* pSource);

    vfloat4x4 VMATH_CALLCONV vLoadFloat4x4(const float* pSource);

    /****************************************************************************
     *
     * Store operations
     *
     ****************************************************************************/

    void VMATH_CALLCONV vStoreBinary32(uint32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreFloat(float* pDestination, vfloat4 V);

    void VMATH_CALLCONV vStoreBinary32x2(uint32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreFloat2(float* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreCastInt2(int32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreCastUint2(uint32_t* pDestination, vfloat4 V);

    void VMATH_CALLCONV vStoreBinary32x3(uint32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreFloat3(float* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreCastInt3(int32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreCastUint3(uint32_t* pDestination, vfloat4 V);

    void VMATH_CALLCONV vStoreBinary32x4(uint32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreFloat4(float* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreCastInt4(int32_t* pDestination, vfloat4 V);
    void VMATH_CALLCONV vStoreCastUint4(uint32_t* pDestination, vfloat4 V);

    void VMATH_CALLCONV vStoreFloat4x4(float* pDestination, vfloat4x4 M);

    /****************************************************************************
     *
     * General vector operations
     *
     ****************************************************************************/

    vfloat4 VMATH_CALLCONV v4Zero();
    vfloat4 VMATH_CALLCONV v4Set(float x, float y, float z, float w);
    vfloat4 VMATH_CALLCONV v4SetBinary(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
    vfloat4 VMATH_CALLCONV v4Spread(float Value);
    vfloat4 VMATH_CALLCONV v4SpreadBinary(uint32_t Value);
    vfloat4 VMATH_CALLCONV v4AllBitsOne();
    vfloat4 VMATH_CALLCONV v4One();
    vfloat4 VMATH_CALLCONV v4Infinity();
    vfloat4 VMATH_CALLCONV v4QNaN();
    vfloat4 VMATH_CALLCONV v4Epsilon();
    vfloat4 VMATH_CALLCONV v4SignMask();

    float VMATH_CALLCONV v4GetByIndex(vfloat4 V, size_t i);
    float VMATH_CALLCONV v4GetX(vfloat4 V);
    float VMATH_CALLCONV v4GetY(vfloat4 V);
    float VMATH_CALLCONV v4GetZ(vfloat4 V);
    float VMATH_CALLCONV v4GetW(vfloat4 V);

    vfloat4 VMATH_CALLCONV v4SetByIndex(vfloat4 V, float f, size_t i);
    vfloat4 VMATH_CALLCONV v4SetX(vfloat4 V, float x);
    vfloat4 VMATH_CALLCONV v4SetY(vfloat4 V, float y);
    vfloat4 VMATH_CALLCONV v4SetZ(vfloat4 V, float z);
    vfloat4 VMATH_CALLCONV v4SetW(vfloat4 V, float w);

    vfloat4 VMATH_CALLCONV v4Swizzle(vfloat4 V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3);
    vfloat4 VMATH_CALLCONV v4Shuffle(vfloat4 V1, vfloat4 V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW);
    vfloat4 VMATH_CALLCONV v4SelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3);
    vfloat4 VMATH_CALLCONV v4Select(vfloat4 V1, vfloat4 V2, vfloat4 Control);

    vfloat4 VMATH_CALLCONV v4PushLeft(vfloat4 V1, vfloat4 V2, uint32_t Elements);
    vfloat4 VMATH_CALLCONV v4RotateLeft(vfloat4 V, uint32_t Elements);
    vfloat4 VMATH_CALLCONV v4RotateRight(vfloat4 V, uint32_t Elements);
    vfloat4 VMATH_CALLCONV v4Insert(vfloat4 VD, vfloat4 VS, uint32_t VSLeftRotateElements, uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3);

    vfloat4 VMATH_CALLCONV v4Equal(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4BinaryEqual(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4NearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon);
    vfloat4 VMATH_CALLCONV v4NotEqual(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4BinaryNotEqual(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Greater(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4GreaterEqual(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Less(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4LessEqual(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4InBoundsAbs(vfloat4 V, vfloat4 Bounds);

    vfloat4 VMATH_CALLCONV v4IsNaN(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4IsInfinite(vfloat4 V);

    vfloat4 VMATH_CALLCONV v4Min(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Max(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Round(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Truncate(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Floor(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Ceiling(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Clamp(vfloat4 V, vfloat4 Min, vfloat4 Max);
    vfloat4 VMATH_CALLCONV v4Clamp01(vfloat4 V);

    vfloat4 VMATH_CALLCONV v4And(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4AndNot(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Or(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Nor(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Xor(vfloat4 V1, vfloat4 V2);

    vfloat4 VMATH_CALLCONV v4Negate(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Add(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Sum(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4AddAngles(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Sub(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4SubAngles(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Mul(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4MulAdd(vfloat4 V1, vfloat4 V2, vfloat4 V3);
    vfloat4 VMATH_CALLCONV v4Div(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4NegateMulAdd(vfloat4 V1, vfloat4 V2, vfloat4 V3);
    vfloat4 VMATH_CALLCONV v4RcpEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Rcp(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4SqrtEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Sqrt(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4RSqrtEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4RSqrt(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Exp2(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Exp10(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ExpE(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Log2(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Log10(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4LogE(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Pow(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Abs(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Mod(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4ModAngles(vfloat4 Angles);
    vfloat4 VMATH_CALLCONV v4Sin(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4SinEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Cos(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4CosEst(vfloat4 V);
    void VMATH_CALLCONV v4SinCos(vfloat4* pSin, vfloat4* pCos, vfloat4 V);
    void VMATH_CALLCONV v4SinCosEst(vfloat4* pSin, vfloat4* pCos, vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Tan(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4TanEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4SinH(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4CosH(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4TanH(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ASin(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ASinEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ACos(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ACosEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ATan(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ATanEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ATan2(vfloat4 Y, vfloat4 X);
    vfloat4 VMATH_CALLCONV v4ATan2Est(vfloat4 Y, vfloat4 X);
    vfloat4 VMATH_CALLCONV v4Lerp(vfloat4 V0, vfloat4 V1, float t);
    vfloat4 VMATH_CALLCONV v4Lerp(vfloat4 V0, vfloat4 V1, vfloat4 T);
    vfloat4 VMATH_CALLCONV v4Hermite(vfloat4 Position0, vfloat4 Tangent0, vfloat4 Position1, vfloat4 Tangent1, float t);
    vfloat4 VMATH_CALLCONV v4Hermite(vfloat4 Position0, vfloat4 Tangent0, vfloat4 Position1, vfloat4 Tangent1, vfloat4 T);
    vfloat4 VMATH_CALLCONV v4CatmullRom(vfloat4 Position0, vfloat4 Position1, vfloat4 Position2, vfloat4 Position3, float t);
    vfloat4 VMATH_CALLCONV v4CatmullRom(vfloat4 Position0, vfloat4 Position1, vfloat4 Position2, vfloat4 Position3, vfloat4 T);


    vfloat4 VMATH_CALLCONV v4xxxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xxww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xywx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xywy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xywz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xyww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xzww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4xwww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yxww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yywx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yywy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yywz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yyww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4yzww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4ywww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zxww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zywx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zywy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zywz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zyww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zzww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4zwww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wxww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wywx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wywy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wywz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wyww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wzww(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwxx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwxy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwxz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwxw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwyx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwyy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwyz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwyw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwzx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwzy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwzz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwzw(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwwx(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwwy(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwwz(vfloat4 v);
    vfloat4 VMATH_CALLCONV v4wwww(vfloat4 v);

    /****************************************************************************
     *
     * 2D vector operations
     *
     ****************************************************************************/

    bool VMATH_CALLCONV v2AllEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllBinaryEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllNearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon);
    bool VMATH_CALLCONV v2AnyNotEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AnyBinaryNotEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllGreater(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllGreaterEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllLess(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllLessEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v2AllInBoundsAbs(vfloat4 V, vfloat4 Bounds);

    bool VMATH_CALLCONV v2AnyIsNaN(vfloat4 V);
    bool VMATH_CALLCONV v2AnyIsInfinite(vfloat4 V);

    vfloat4 VMATH_CALLCONV v2Dot(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v2Cross(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v2LengthSq(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2RLengthEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2RLength(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2LengthEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2Length(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2NormalizeEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2Normalize(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2ClampLength(vfloat4 V, float LengthMin, float LengthMax);
    vfloat4 VMATH_CALLCONV v2ClampLength(vfloat4 V, vfloat4 LengthMin, vfloat4 LengthMax);
    vfloat4 VMATH_CALLCONV v2Reflect(vfloat4 Incident, vfloat4 Normal);
    vfloat4 VMATH_CALLCONV v2Refract(vfloat4 Incident, vfloat4 Normal, float RefractionIndex);
    vfloat4 VMATH_CALLCONV v2Refract(vfloat4 Incident, vfloat4 Normal, vfloat4 RefractionIndex);
    vfloat4 VMATH_CALLCONV v2Orthogonal(vfloat4 V);
    vfloat4 VMATH_CALLCONV v2AngleBetweenNormalsEst(vfloat4 N1, vfloat4 N2);
    vfloat4 VMATH_CALLCONV v2AngleBetweenNormals(vfloat4 N1, vfloat4 N2);
    vfloat4 VMATH_CALLCONV v2AngleBetweenVectors(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v2LinePointDistance(vfloat4 LinePoint1, vfloat4 LinePoint2, vfloat4 Point);
    vfloat4 VMATH_CALLCONV v2IntersectLine(vfloat4 Line1Point1, vfloat4 Line1Point2, vfloat4 Line2Point1, vfloat4 Line2Point2);

    /****************************************************************************
     *
     * 3D vector operations
     *
     ****************************************************************************/

    bool VMATH_CALLCONV v3AllEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllBinaryEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllNearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon);
    bool VMATH_CALLCONV v3AnyNotEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AnyBinaryNotEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllGreater(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllGreaterEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllLess(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllLessEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v3AllInBoundsAbs(vfloat4 V, vfloat4 Bounds);

    bool VMATH_CALLCONV v3AnyIsNaN(vfloat4 V);
    bool VMATH_CALLCONV v3AnyIsInfinite(vfloat4 V);

    vfloat4 VMATH_CALLCONV v3Dot(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v3Cross(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v3LengthSq(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3RLengthEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3RLength(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3LengthEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3Length(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3NormalizeEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3Normalize(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3ClampLength(vfloat4 V, float LengthMin, float LengthMax);
    vfloat4 VMATH_CALLCONV v3ClampLength(vfloat4 V, vfloat4 LengthMin, vfloat4 LengthMax);
    vfloat4 VMATH_CALLCONV v3Reflect(vfloat4 Incident, vfloat4 Normal);
    vfloat4 VMATH_CALLCONV v3Refract(vfloat4 Incident, vfloat4 Normal, float RefractionIndex);
    vfloat4 VMATH_CALLCONV v3Refract(vfloat4 Incident, vfloat4 Normal, vfloat4 RefractionIndex);
    vfloat4 VMATH_CALLCONV v3Orthogonal(vfloat4 V);
    vfloat4 VMATH_CALLCONV v3AngleBetweenNormalsEst(vfloat4 N1, vfloat4 N2);
    vfloat4 VMATH_CALLCONV v3AngleBetweenNormals(vfloat4 N1, vfloat4 N2);
    vfloat4 VMATH_CALLCONV v3AngleBetweenVectors(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v3LinePointDistance(vfloat4 LinePoint1, vfloat4 LinePoint2, vfloat4 Point);
    void VMATH_CALLCONV v3ComponentsFromNormal(vfloat4* pParallel, vfloat4* pPerpendicular, vfloat4 V, vfloat4 Normal);
    vfloat4 VMATH_CALLCONV v3Rotate(vfloat4 V, vfloat4 RotationQuaternion);
    vfloat4 VMATH_CALLCONV v3InverseRotate(vfloat4 V, vfloat4 RotationQuaternion);
    vfloat4 VMATH_CALLCONV v3TransformPoint(vfloat4 V, vfloat4x4 M);
    vfloat4 VMATH_CALLCONV v3TransformPointDivideW(vfloat4 V, vfloat4x4 M);
    vfloat4 VMATH_CALLCONV v3TransformNormal(vfloat4 V, vfloat4x4 M);
    vfloat4 VMATH_CALLCONV v3Project(vfloat4 V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, vfloat4x4 Projection, vfloat4x4 View, vfloat4x4 World);
    vfloat4 VMATH_CALLCONV v3Unproject(vfloat4 V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, vfloat4x4 Projection, vfloat4x4 View, vfloat4x4 World);

    /****************************************************************************
     *
     * 4D vector operations
     *
     ****************************************************************************/

    bool VMATH_CALLCONV v4AllEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllBinaryEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllNearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon);
    bool VMATH_CALLCONV v4AnyNotEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AnyBinaryNotEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllGreater(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllGreaterEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllLess(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllLessEqual(vfloat4 V1, vfloat4 V2);
    bool VMATH_CALLCONV v4AllInBoundsAbs(vfloat4 V, vfloat4 Bounds);

    bool VMATH_CALLCONV v4AnyIsNaN(vfloat4 V);
    bool VMATH_CALLCONV v4AnyIsInfinite(vfloat4 V);

    vfloat4 VMATH_CALLCONV v4Dot(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Cross(vfloat4 V1, vfloat4 V2, vfloat4 V3);
    vfloat4 VMATH_CALLCONV v4LengthSq(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4RLengthEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4RLength(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4LengthEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Length(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4NormalizeEst(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4Normalize(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4ClampLength(vfloat4 V, float LengthMin, float LengthMax);
    vfloat4 VMATH_CALLCONV v4ClampLength(vfloat4 V, vfloat4 LengthMin, vfloat4 LengthMax);
    vfloat4 VMATH_CALLCONV v4Reflect(vfloat4 Incident, vfloat4 Normal);
    vfloat4 VMATH_CALLCONV v4Refract(vfloat4 Incident, vfloat4 Normal, float RefractionIndex);
    vfloat4 VMATH_CALLCONV v4Refract(vfloat4 Incident, vfloat4 Normal, vfloat4 RefractionIndex);
    vfloat4 VMATH_CALLCONV v4Orthogonal(vfloat4 V);
    vfloat4 VMATH_CALLCONV v4AngleBetweenNormalsEst(vfloat4 N1, vfloat4 N2);
    vfloat4 VMATH_CALLCONV v4AngleBetweenNormals(vfloat4 N1, vfloat4 N2);
    vfloat4 VMATH_CALLCONV v4AngleBetweenVectors(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV v4Transform(vfloat4 V, vfloat4x4 M);

    /****************************************************************************
     *
     * Matrix operations
     *
     ****************************************************************************/

    bool VMATH_CALLCONV vMat4IsAnyNaN(vfloat4x4 M);
    bool VMATH_CALLCONV vMat4IsAnyInfinite(vfloat4x4 M);
    bool VMATH_CALLCONV vMat4IsIdentity(vfloat4x4 M);

    vfloat4x4 VMATH_CALLCONV vMat4Mul(vfloat4x4 M1, vfloat4x4 M2);
    vfloat4x4 VMATH_CALLCONV vMat4MulTranspose(vfloat4x4 M1, vfloat4x4 M2);
    vfloat4x4 VMATH_CALLCONV vMat4Transpose(vfloat4x4 M);
    vfloat4x4 VMATH_CALLCONV vMat4Inverse(vfloat4* pDeterminant, vfloat4x4 M);
    vfloat4x4 VMATH_CALLCONV vMat4VectorTensorProduct(vfloat4 V1, vfloat4 V2);
    vfloat4 VMATH_CALLCONV vMat4Determinant(vfloat4x4 M);

    bool VMATH_CALLCONV vMat4Decompose(vfloat4* outScale, vfloat4* outRotQuat, vfloat4* outTrans, vfloat4x4 M);

    vfloat4x4 VMATH_CALLCONV vMat4Zero();
    vfloat4x4 VMATH_CALLCONV vMat4Identity();
    vfloat4x4 VMATH_CALLCONV vMat4Translation(float OffsetX, float OffsetY, float OffsetZ);
    vfloat4x4 VMATH_CALLCONV vMat4Translation(vfloat4 Offset);
    vfloat4x4 VMATH_CALLCONV vMat4Scaling(float ScaleX, float ScaleY, float ScaleZ);
    vfloat4x4 VMATH_CALLCONV vMat4Scaling(vfloat4 Scale);
    vfloat4x4 VMATH_CALLCONV vMat4RotationX(float Angle);
    vfloat4x4 VMATH_CALLCONV vMat4RotationY(float Angle);
    vfloat4x4 VMATH_CALLCONV vMat4RotationZ(float Angle);
    vfloat4x4 VMATH_CALLCONV vMat4RotationRollPitchYaw(float Pitch, float Yaw, float Roll);
    vfloat4x4 VMATH_CALLCONV vMat4RotationRollPitchYawFromVector(vfloat4 Angles);
    vfloat4x4 VMATH_CALLCONV vMat4RotationNormal(vfloat4 NormalAxis, float Angle);
    vfloat4x4 VMATH_CALLCONV vMat4RotationAxis(vfloat4 Axis, float Angle);
    vfloat4x4 VMATH_CALLCONV vMat4RotationQuaternion(vfloat4 Quaternion);
    vfloat4x4 VMATH_CALLCONV vMat4Transformation2D(vfloat4 ScalingOrigin, float ScalingOrientation, vfloat4 Scaling, vfloat4 RotationOrigin, float Rotation, vfloat4 Translation);
    vfloat4x4 VMATH_CALLCONV vMat4Transformation(vfloat4 ScalingOrigin, vfloat4 ScalingOrientationQuaternion, vfloat4 Scaling, vfloat4 RotationOrigin, vfloat4 RotationQuaternion, vfloat4 Translation);
    vfloat4x4 VMATH_CALLCONV vMat4AffineTransformation2D(vfloat4 Scaling, vfloat4 RotationOrigin, float Rotation, vfloat4 Translation);
    vfloat4x4 VMATH_CALLCONV vMat4AffineTransformation(vfloat4 Scaling, vfloat4 RotationOrigin, vfloat4 RotationQuaternion, vfloat4 Translation);
    vfloat4x4 VMATH_CALLCONV vMat4Reflect(vfloat4 ReflectionPlane);

    vfloat4x4 VMATH_CALLCONV vMat4LookAt(vfloat4 EyePosition, vfloat4 FocusPosition, vfloat4 UpDirection);
    vfloat4x4 VMATH_CALLCONV vMat4LookTo(vfloat4 EyePosition, vfloat4 EyeDirection, vfloat4 UpDirection);
    vfloat4x4 VMATH_CALLCONV vMat4Perspective(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
    vfloat4x4 VMATH_CALLCONV vMat4PerspectiveFOV(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
    vfloat4x4 VMATH_CALLCONV vMat4PerspectiveOffCenter(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);
    vfloat4x4 VMATH_CALLCONV vMat4Orthographic(float ViewWidth, float ViewHeight, float NearZ, float FarZ);
    vfloat4x4 VMATH_CALLCONV vMat4OrthographicOffCenter(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ);

    /****************************************************************************
     *
     * Quaternion operations
     *
     ****************************************************************************/

    vfloat4 VMATH_CALLCONV vQuatMul(vfloat4 Q1, vfloat4 Q2);
    vfloat4 VMATH_CALLCONV vQuatLengthSq(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatRLength(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatLength(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatNormalizeEst(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatNormalize(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatConjugate(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatInverse(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatLn(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatExp(vfloat4 Q);
    vfloat4 VMATH_CALLCONV vQuatSlerp(vfloat4 Q0, vfloat4 Q1, float t);
    vfloat4 VMATH_CALLCONV vQuatSlerp(vfloat4 Q0, vfloat4 Q1, vfloat4 T);
    vfloat4 VMATH_CALLCONV vQuatSquad(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 Q3, float t);
    vfloat4 VMATH_CALLCONV vQuatSquad(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 Q3, vfloat4 T);
    void VMATH_CALLCONV vQuatSquadSetup(vfloat4* pA, vfloat4* pB, vfloat4* pC, vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 Q3);
    vfloat4 VMATH_CALLCONV vQuatBaryCentric(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, float f, float g);
    vfloat4 VMATH_CALLCONV vQuatBaryCentric(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 F, vfloat4 G);

    vfloat4 VMATH_CALLCONV vQuatIdentity();
    vfloat4 VMATH_CALLCONV vQuatRotationRollPitchYaw(float Pitch, float Yaw, float Roll);
    vfloat4 VMATH_CALLCONV vQuatRotationRollPitchYawFromVector(vfloat4 Angles);
    vfloat4 VMATH_CALLCONV vQuatNormalAngle(vfloat4 NormalAxis, float Angle);
    vfloat4 VMATH_CALLCONV vQuatAxisAngle(vfloat4 Axis, float Angle);
    vfloat4 VMATH_CALLCONV vQuatRotationMatrix(vfloat4x4 M);

    void VMATH_CALLCONV vQuatToAxisAngle(vfloat4* pAxis, float* pAngle, vfloat4 Q);

    /****************************************************************************
     *
     * Plane operations
     *
     ****************************************************************************/

    vfloat4 VMATH_CALLCONV vPlaneNormalizeEst(vfloat4 P);
    vfloat4 VMATH_CALLCONV vPlaneNormalize(vfloat4 P);
    vfloat4 VMATH_CALLCONV vPlaneIntersectLine(vfloat4 P, vfloat4 LinePoint1, vfloat4 LinePoint2);
    void VMATH_CALLCONV vPlaneIntersectPlane(vfloat4* pLinePoint1, vfloat4* pLinePoint2, vfloat4 P1, vfloat4 P2);
    vfloat4 VMATH_CALLCONV vPlaneTransform(vfloat4 P, vfloat4x4 M);

    vfloat4 VMATH_CALLCONV vPlaneFromPointNormal(vfloat4 Point, vfloat4 Normal);
    vfloat4 VMATH_CALLCONV vPlaneFromPoints(vfloat4 Point1, vfloat4 Point2, vfloat4 Point3);

    /****************************************************************************
     *
     * Color operations
     *
     ****************************************************************************/

    vfloat4 VMATH_CALLCONV vColorNegative(vfloat4 C);
    vfloat4 VMATH_CALLCONV vColorAdjustSaturation(vfloat4 C, float Saturation);
    vfloat4 VMATH_CALLCONV vColorAdjustContrast(vfloat4 C, float Contrast);

    vfloat4 VMATH_CALLCONV vColorRGBToHSL(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorHSLToRGB(vfloat4 hsl);

    vfloat4 VMATH_CALLCONV vColorRGBToHSV(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorHSVToRGB(vfloat4 hsv);

    vfloat4 VMATH_CALLCONV vColorRGBToYUV(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorYUVToRGB(vfloat4 yuv);

    vfloat4 VMATH_CALLCONV vColorRGBToYUV_HD(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorYUVToRGB_HD(vfloat4 yuv);

    vfloat4 VMATH_CALLCONV vColorRGBToYUV_UHD(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorYUVToRGB_UHD(vfloat4 yuv);

    vfloat4 VMATH_CALLCONV vColorRGBToXYZ(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorXYZToRGB(vfloat4 xyz);

    vfloat4 VMATH_CALLCONV vColorXYZToSRGB(vfloat4 xyz);
    vfloat4 VMATH_CALLCONV vColorSRGBToXYZ(vfloat4 srgb);

    vfloat4 VMATH_CALLCONV vColorRGBToSRGB(vfloat4 rgb);
    vfloat4 VMATH_CALLCONV vColorSRGBToRGB(vfloat4 srgb);

    /****************************************************************************
     *
     * vbool4
     *
     ****************************************************************************/

    bool v4bIsValidCompareMask(uint32_t mask);
    bool VMATH_CALLCONV v4IsValidCompareMask(vfloat4 vMask);

    vbool4 VMATH_CALLCONV v4CompressMask128(vfloat4 v);

    vbool4 vLoadBool4(const bool* src);
    void vStoreBool4(bool* dst, vbool4 src);

    vbool4 vLoadBool3(const bool* src);
    void vStoreBool3(bool* dst, vbool4 src);

    vbool4 vLoadBool2(const bool* src);
    void vStoreBool2(bool* dst, vbool4 src);


    bool v4All(vbool4 v);
    bool v4AllFalse(vbool4 v);
    bool v4Any(vbool4 v);
    bool v4AnyFalse(vbool4 v);

    bool v3All(vbool4 v);
    bool v3AllFalse(vbool4 v);
    bool v3Any(vbool4 v);
    bool v3AnyFalse(vbool4 v);

    bool v2All(vbool4 v);
    bool v2AllFalse(vbool4 v);
    bool v2Any(vbool4 v);
    bool v2AnyFalse(vbool4 v);


    bool v4All(vfloat4 vMask);
    bool v4AllFalse(vfloat4 vMask);
    bool v4Any(vfloat4 vMask);
    bool v4AnyFalse(vfloat4 vMask);

    bool v3All(vfloat4 vMask);
    bool v3AllFalse(vfloat4 vMask);
    bool v3Any(vfloat4 vMask);
    bool v3AnyFalse(vfloat4 vMask);

    bool v2All(vfloat4 vMask);
    bool v2AllFalse(vfloat4 vMask);
    bool v2Any(vfloat4 vMask);
    bool v2AnyFalse(vfloat4 vMask);


    /****************************************************************************
     *
     * Miscellaneous operations
     *
     ****************************************************************************/

    vfloat4 VMATH_CALLCONV v4FresnelTerm(vfloat4 CosIncidentAngle, vfloat4 RefractionIndex);

    bool VerifyVMathCPUSupport();

    bool NearEqual(float S1, float S2, float Epsilon);
    float ModAngle(float Value);

    float Sin(float Value);
    float SinEst(float Value);

    float Cos(float Value);
    float CosEst(float Value);

    void SinCos(float* pSin, float* pCos, float Value);
    void SinCosEst(float* pSin, float* pCos, float Value);

    float ASin(float Value);
    float ASinEst(float Value);

    float ACos(float Value);
    float ACosEst(float Value);

    /****************************************************************************
     *
     * Globals
     *
     ****************************************************************************/

     // The purpose of the following global constants is to prevent redundant
     // reloading of the constants when they are referenced by more than one
     // separate inline math routine called within the same function.  Declaring
     // a constant locally within a routine is sufficient to prevent redundant
     // reloads of that constant when that single routine is called multiple
     // times in a function, but if the constant is used (and declared) in a
     // separate math routine it would be reloaded.

#ifndef VMATH_GLOBALCONST
#if defined(__GNUC__) && !defined(__MINGW32__)
#define VMATH_GLOBALCONST extern const __attribute__((weak))
#else
#define VMATH_GLOBALCONST extern const __declspec(selectany)
#endif
#endif

    namespace Internal
    {
        //------------------------------------------------------------------------------
        // Conversion types for constants
        VMATH_ALIGNED_STRUCT(16) vfloat4f32
        {
            union
            {
                float f[4];
                vfloat4 v;
            };

            inline operator vfloat4() const { return v; }
            inline operator const float* () const { return f; }
#ifdef VMATH_NO_INTRINSICS
#elif defined(VMATH_SSE_INTRINSICS)
            inline operator __m128i() const { return _mm_castps_si128(v); }
            inline operator __m128d() const { return _mm_castps_pd(v); }
#elif defined(VMATH_ARM_NEON_INTRINSICS) && defined(__GNUC__)
            inline operator int32x4_t() const { return vreinterpretq_s32_f32(v); }
            inline operator uint32x4_t() const { return vreinterpretq_u32_f32(v); }
#endif
        };

        VMATH_ALIGNED_STRUCT(16) vfloat4i32
        {
            union
            {
                int32_t i[4];
                vfloat4 v;
            };

            inline operator vfloat4() const { return v; }
#ifdef VMATH_NO_INTRINSICS
#elif defined(VMATH_SSE_INTRINSICS)
            inline operator __m128i() const { return _mm_castps_si128(v); }
            inline operator __m128d() const { return _mm_castps_pd(v); }
#elif defined(VMATH_ARM_NEON_INTRINSICS) && defined(__GNUC__)
            inline operator int32x4_t() const { return vreinterpretq_s32_f32(v); }
            inline operator uint32x4_t() const { return vreinterpretq_u32_f32(v); }
#endif
        };

        VMATH_ALIGNED_STRUCT(16) vfloat4u32
        {
            union
            {
                uint32_t u[4];
                vfloat4 v;
            };

            inline operator vfloat4() const { return v; }
#ifdef VMATH_NO_INTRINSICS
#elif defined(VMATH_SSE_INTRINSICS)
            inline operator __m128i() const { return _mm_castps_si128(v); }
            inline operator __m128d() const { return _mm_castps_pd(v); }
#elif defined(VMATH_ARM_NEON_INTRINSICS) && defined(__GNUC__)
            inline operator int32x4_t() const { return vreinterpretq_s32_f32(v); }
            inline operator uint32x4_t() const { return vreinterpretq_u32_f32(v); }
#endif
        };

        VMATH_GLOBALCONST vfloat4f32 kSinCoefficients0 = { { { -0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f } } };
        VMATH_GLOBALCONST vfloat4f32 kSinCoefficients1 = { { { -2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/ } } };
        VMATH_GLOBALCONST vfloat4f32 kCosCoefficients0 = { { { -0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f } } };
        VMATH_GLOBALCONST vfloat4f32 kCosCoefficients1 = { { { -2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/ } } };
        VMATH_GLOBALCONST vfloat4f32 kTanCoefficients0 = { { { 1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f } } };
        VMATH_GLOBALCONST vfloat4f32 kTanCoefficients1 = { { { 2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f } } };
        VMATH_GLOBALCONST vfloat4f32 kArcCoefficients0 = { { { +1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f } } };
        VMATH_GLOBALCONST vfloat4f32 kArcCoefficients1 = { { { +0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f } } };
        VMATH_GLOBALCONST vfloat4f32 kATanCoefficients0 = { { { -0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f } } };
        VMATH_GLOBALCONST vfloat4f32 kATanCoefficients1 = { { { -0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f } } };
        VMATH_GLOBALCONST vfloat4f32 kATanEstCoefficients0 = { { { +0.999866f, +0.999866f, +0.999866f, +0.999866f } } };
        VMATH_GLOBALCONST vfloat4f32 kATanEstCoefficients1 = { { { -0.3302995f, +0.180141f, -0.085133f, +0.0208351f } } };
        VMATH_GLOBALCONST vfloat4f32 kTanEstCoefficients = { { { 2.484f, -1.954923183e-1f, 2.467401101f, (1 / Pi) } } };
        VMATH_GLOBALCONST vfloat4f32 kArcEstCoefficients = { { { +1.5707288f, -0.2121144f, +0.0742610f, -0.0187293f } } };
        VMATH_GLOBALCONST vfloat4f32 kIdentityR0 = { { { 1.0f, 0.0f, 0.0f, 0.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kIdentityR1 = { { { 0.0f, 1.0f, 0.0f, 0.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kIdentityR2 = { { { 0.0f, 0.0f, 1.0f, 0.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kIdentityR3 = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kNegIdentityR3 = { { { 0.0f, 0.0f, 0.0f, -1.0f } } };
        VMATH_GLOBALCONST vfloat4u32 kNegativeZero = { { { 0x80000000, 0x80000000, 0x80000000, 0x80000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kNegate3 = { { { 0x80000000, 0x80000000, 0x80000000, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kMaskXY = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kMask3 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kMaskX = { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kMaskY = { { { 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kMaskZ = { { { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kMaskW = { { { 0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF } } };
        VMATH_GLOBALCONST vfloat4f32 kOne = { { { 1.0f, 1.0f, 1.0f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kOne3 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kZero = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kTwo = { { { 2.f, 2.f, 2.f, 2.f } } };
        VMATH_GLOBALCONST vfloat4f32 kFour = { { { 4.f, 4.f, 4.f, 4.f } } };
        VMATH_GLOBALCONST vfloat4f32 kSix = { { { 6.f, 6.f, 6.f, 6.f } } };
        VMATH_GLOBALCONST vfloat4f32 kNegativeOne = { { { -1.0f, -1.0f, -1.0f, -1.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kOneHalf = { { { 0.5f, 0.5f, 0.5f, 0.5f } } };
        VMATH_GLOBALCONST vfloat4f32 kNegativeTwoPi = { { { -(2 * Pi), -(2 * Pi), -(2 * Pi), -(2 * Pi) } } };
        VMATH_GLOBALCONST vfloat4f32 kNegativePi = { { { -Pi, -Pi, -Pi, -Pi } } };
        VMATH_GLOBALCONST vfloat4f32 kHalfPi = { { { (Pi / 2), (Pi / 2), (Pi / 2), (Pi / 2) } } };
        VMATH_GLOBALCONST vfloat4f32 kPi = { { { Pi, Pi, Pi, Pi } } };
        VMATH_GLOBALCONST vfloat4f32 kTwoPi = { { { (2 * Pi), (2 * Pi), (2 * Pi), (2 * Pi) } } };
        VMATH_GLOBALCONST vfloat4f32 kReciprocalTwoPi = { { { (1 / (2 * Pi)), (1 / (2 * Pi)), (1 / (2 * Pi)), (1 / (2 * Pi)) } } };
        VMATH_GLOBALCONST vfloat4f32 kEpsilon = { { { 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f } } };
        VMATH_GLOBALCONST vfloat4i32 kInfinity = { { { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 } } };
        VMATH_GLOBALCONST vfloat4i32 kQNaN = { { { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 } } };
        VMATH_GLOBALCONST vfloat4i32 kQNaNTest = { { { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF } } };
        VMATH_GLOBALCONST vfloat4i32 kAbsMask = { { { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF } } };
        VMATH_GLOBALCONST vfloat4u32 kNegOneMask = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF } } };
        VMATH_GLOBALCONST vfloat4f32 kNoFraction = { { { 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kNegateX = { { { -1.0f, 1.0f, 1.0f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kNegateY = { { { 1.0f, -1.0f, 1.0f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kNegateZ = { { { 1.0f, 1.0f, -1.0f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4u32 kSelect0101 = { { { 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF } } };
        VMATH_GLOBALCONST vfloat4u32 kSelect1000 = { { { 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kSelect1100 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kSelect1110 = { { { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kSelect1011 = { { { 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF } } };
        VMATH_GLOBALCONST vfloat4u32 kFlipY = { { { 0, 0x80000000, 0, 0 } } };
        VMATH_GLOBALCONST vfloat4u32 kFlipZW = { { { 0, 0, 0x80000000, 0x80000000 } } };
        VMATH_GLOBALCONST vfloat4f32 kFixUnsigned = { { { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kMaxInt = { { { 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kMaxUInt = { { { 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kUnsignedFix = { { { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kSRGBScale = { { { 12.92f, 12.92f, 12.92f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kSRGBA = { { { 0.055f, 0.055f, 0.055f, 0.0f } } };
        VMATH_GLOBALCONST vfloat4f32 kSRGBA1 = { { { 1.055f, 1.055f, 1.055f, 1.0f } } };
        VMATH_GLOBALCONST vfloat4i32 kExponentBias = { { { 127, 127, 127, 127 } } };
        VMATH_GLOBALCONST vfloat4i32 kSubnormalExponent = { { { -126, -126, -126, -126 } } };
        VMATH_GLOBALCONST vfloat4i32 kNumTrailing = { { { 23, 23, 23, 23 } } };
        VMATH_GLOBALCONST vfloat4i32 kMinNormal = { { { 0x00800000, 0x00800000, 0x00800000, 0x00800000 } } };
        VMATH_GLOBALCONST vfloat4u32 kNegInfinity = { { { 0xFF800000, 0xFF800000, 0xFF800000, 0xFF800000 } } };
        VMATH_GLOBALCONST vfloat4u32 kNegQNaN = { { { 0xFFC00000, 0xFFC00000, 0xFFC00000, 0xFFC00000 } } };
        VMATH_GLOBALCONST vfloat4i32 kBin128 = { { { 0x43000000, 0x43000000, 0x43000000, 0x43000000 } } };
        VMATH_GLOBALCONST vfloat4u32 kBinNeg150 = { { { 0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000 } } };
        VMATH_GLOBALCONST vfloat4i32 k253 = { { { 253, 253, 253, 253 } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst1 = { { { -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst2 = { { { +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst3 = { { { -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst4 = { { { +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst5 = { { { -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst6 = { { { +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f } } };
        VMATH_GLOBALCONST vfloat4f32 kExpEst7 = { { { -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst0 = { { { +1.442693f, +1.442693f, +1.442693f, +1.442693f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst1 = { { { -0.721242f, -0.721242f, -0.721242f, -0.721242f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst2 = { { { +0.479384f, +0.479384f, +0.479384f, +0.479384f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst3 = { { { -0.350295f, -0.350295f, -0.350295f, -0.350295f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst4 = { { { +0.248590f, +0.248590f, +0.248590f, +0.248590f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst5 = { { { -0.145700f, -0.145700f, -0.145700f, -0.145700f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst6 = { { { +0.057148f, +0.057148f, +0.057148f, +0.057148f } } };
        VMATH_GLOBALCONST vfloat4f32 kLogEst7 = { { { -0.010578f, -0.010578f, -0.010578f, -0.010578f } } };
        VMATH_GLOBALCONST vfloat4f32 kLgE = { { { +1.442695f, +1.442695f, +1.442695f, +1.442695f } } };
        VMATH_GLOBALCONST vfloat4f32 kInvLgE = { { { +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f } } };
        VMATH_GLOBALCONST vfloat4f32 kLg10 = { { { +3.321928f, +3.321928f, +3.321928f, +3.321928f } } };
        VMATH_GLOBALCONST vfloat4f32 kInvLg10 = { { { +3.010299956e-1f, +3.010299956e-1f, +3.010299956e-1f, +3.010299956e-1f } } };
    }

    //------------------------------------------------------------------------------

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)

// PermuteHelper internal template (SSE only)
    namespace Internal
    {
        // Slow path fallback for permutes that do not map to a single SSE shuffle opcode.
        template <uint32_t Shuffle, bool WhichX, bool WhichY, bool WhichZ, bool WhichW>
        struct PermuteHelper
        {
            static vfloat4 VMATH_CALLCONV Permute(vfloat4 v1, vfloat4 v2)
            {
                static const vfloat4u32 selectMask =
                { { {
                        WhichX ? 0xFFFFFFFF : 0,
                        WhichY ? 0xFFFFFFFF : 0,
                        WhichZ ? 0xFFFFFFFF : 0,
                        WhichW ? 0xFFFFFFFF : 0,
                } } };

                vfloat4 shuffled1 = VMATH_PERMUTE_PS(v1, Shuffle);
                vfloat4 shuffled2 = VMATH_PERMUTE_PS(v2, Shuffle);

                vfloat4 masked1 = _mm_andnot_ps(selectMask, shuffled1);
                vfloat4 masked2 = _mm_and_ps(selectMask, shuffled2);

                return _mm_or_ps(masked1, masked2);
            }
        };

        // Fast path for permutes that only read from the first vector.
        template<uint32_t Shuffle>
        struct PermuteHelper<Shuffle, false, false, false, false>
        {
            static vfloat4 VMATH_CALLCONV Permute(vfloat4 v1, vfloat4) { return VMATH_PERMUTE_PS(v1, Shuffle); }
        };

        // Fast path for permutes that only read from the second vector.
        template<uint32_t Shuffle>
        struct PermuteHelper<Shuffle, true, true, true, true>
        {
            static vfloat4 VMATH_CALLCONV Permute(vfloat4, vfloat4 v2) { return VMATH_PERMUTE_PS(v2, Shuffle); }
        };

        // Fast path for permutes that read XY from the first vector, ZW from the second.
        template<uint32_t Shuffle>
        struct PermuteHelper<Shuffle, false, false, true, true>
        {
            static vfloat4 VMATH_CALLCONV Permute(vfloat4 v1, vfloat4 v2) { return _mm_shuffle_ps(v1, v2, Shuffle); }
        };

        // Fast path for permutes that read XY from the second vector, ZW from the first.
        template<uint32_t Shuffle>
        struct PermuteHelper<Shuffle, true, true, false, false>
        {
            static vfloat4 VMATH_CALLCONV Permute(vfloat4 v1, vfloat4 v2) { return _mm_shuffle_ps(v2, v1, Shuffle); }
        };
    }

#endif // VMATH_SSE_INTRINSICS && !VMATH_NO_INTRINSICS

    // General permute template
    template<uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW>
    inline vfloat4 VMATH_CALLCONV v4Shuffle(vfloat4 V1, vfloat4 V2)
    {
        static_assert(PermuteX <= 7, "PermuteX template parameter out of range");
        static_assert(PermuteY <= 7, "PermuteY template parameter out of range");
        static_assert(PermuteZ <= 7, "PermuteZ template parameter out of range");
        static_assert(PermuteW <= 7, "PermuteW template parameter out of range");

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        constexpr uint32_t Shuffle = _MM_SHUFFLE(PermuteW & 3, PermuteZ & 3, PermuteY & 3, PermuteX & 3);

        constexpr bool WhichX = PermuteX > 3;
        constexpr bool WhichY = PermuteY > 3;
        constexpr bool WhichZ = PermuteZ > 3;
        constexpr bool WhichW = PermuteW > 3;

        return Internal::PermuteHelper<Shuffle, WhichX, WhichY, WhichZ, WhichW>::Permute(V1, V2);
#else

        return v4Shuffle(V1, V2, PermuteX, PermuteY, PermuteZ, PermuteW);

#endif
    }

    // Special-case permute templates
    template<> inline constexpr vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 2, 3>(vfloat4 V1, vfloat4) { return V1; }
    template<> inline constexpr vfloat4 VMATH_CALLCONV v4Shuffle<4, 5, 6, 7>(vfloat4, vfloat4 V2) { return V2; }

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 4, 5>(vfloat4 V1, vfloat4 V2) { return _mm_movelh_ps(V1, V2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<6, 7, 2, 3>(vfloat4 V1, vfloat4 V2) { return _mm_movehl_ps(V1, V2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 4, 1, 5>(vfloat4 V1, vfloat4 V2) { return _mm_unpacklo_ps(V1, V2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 6, 3, 7>(vfloat4 V1, vfloat4 V2) { return _mm_unpackhi_ps(V1, V2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 3, 6, 7>(vfloat4 V1, vfloat4 V2) { return _mm_castpd_ps(_mm_unpackhi_pd(_mm_castps_pd(V1), _mm_castps_pd(V2))); }
#endif

#if defined(VMATH_SSE4_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 1, 2, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x1); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 5, 2, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 5, 2, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x3); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 6, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x4); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 1, 6, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x5); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 5, 6, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x6); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 5, 6, 3>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x7); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 2, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x8); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 1, 2, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0x9); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 5, 2, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0xA); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 5, 2, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0xB); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 6, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0xC); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<4, 1, 6, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0xD); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 5, 6, 7>(vfloat4 V1, vfloat4 V2) { return _mm_blend_ps(V1, V2, 0xE); }
#endif

#if defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)

    // If the indices are all in the range 0-3 or 4-7, then use v4Swizzle instead
    // The mirror cases are not spelled out here as the programmer can always swap the arguments
    // (i.e. prefer permutes where the X element comes from the V1 vector instead of the V2 vector)

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 4, 5>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_low_f32(V1), vget_low_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 0, 4, 5>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vget_low_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 5, 4>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_low_f32(V1), vrev64_f32(vget_low_f32(V2))); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 0, 5, 4>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vrev64_f32(vget_low_f32(V2))); }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 3, 6, 7>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_high_f32(V1), vget_high_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<3, 2, 6, 7>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vget_high_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 3, 7, 6>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_high_f32(V1), vrev64_f32(vget_high_f32(V2))); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<3, 2, 7, 6>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vrev64_f32(vget_high_f32(V2))); }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 6, 7>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_low_f32(V1), vget_high_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 0, 6, 7>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vget_high_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 1, 7, 6>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_low_f32(V1), vrev64_f32(vget_high_f32(V2))); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 0, 7, 6>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_low_f32(V1)), vrev64_f32(vget_high_f32(V2))); }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<3, 2, 4, 5>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vget_low_f32(V2)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 3, 5, 4>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vget_high_f32(V1), vrev64_f32(vget_low_f32(V2))); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<3, 2, 5, 4>(vfloat4 V1, vfloat4 V2) { return vcombine_f32(vrev64_f32(vget_high_f32(V1)), vrev64_f32(vget_low_f32(V2))); }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 4, 2, 6>(vfloat4 V1, vfloat4 V2) { return vtrnq_f32(V1, V2).val[0]; }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 5, 3, 7>(vfloat4 V1, vfloat4 V2) { return vtrnq_f32(V1, V2).val[1]; }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 4, 1, 5>(vfloat4 V1, vfloat4 V2) { return vzipq_f32(V1, V2).val[0]; }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 6, 3, 7>(vfloat4 V1, vfloat4 V2) { return vzipq_f32(V1, V2).val[1]; }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<0, 2, 4, 6>(vfloat4 V1, vfloat4 V2) { return vuzpq_f32(V1, V2).val[0]; }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 3, 5, 7>(vfloat4 V1, vfloat4 V2) { return vuzpq_f32(V1, V2).val[1]; }

    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<1, 2, 3, 4>(vfloat4 V1, vfloat4 V2) { return vextq_f32(V1, V2, 1); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<2, 3, 4, 5>(vfloat4 V1, vfloat4 V2) { return vextq_f32(V1, V2, 2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Shuffle<3, 4, 5, 6>(vfloat4 V1, vfloat4 V2) { return vextq_f32(V1, V2, 3); }

#endif // VMATH_ARM_NEON_INTRINSICS && !VMATH_NO_INTRINSICS

    //------------------------------------------------------------------------------

    // General swizzle template
    template<uint32_t SwizzleX, uint32_t SwizzleY, uint32_t SwizzleZ, uint32_t SwizzleW>
    inline vfloat4 VMATH_CALLCONV v4Swizzle(vfloat4 V)
    {
        static_assert(SwizzleX <= 3, "SwizzleX template parameter out of range");
        static_assert(SwizzleY <= 3, "SwizzleY template parameter out of range");
        static_assert(SwizzleZ <= 3, "SwizzleZ template parameter out of range");
        static_assert(SwizzleW <= 3, "SwizzleW template parameter out of range");

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        return VMATH_PERMUTE_PS(V, _MM_SHUFFLE(SwizzleW, SwizzleZ, SwizzleY, SwizzleX));
#else

        return v4Swizzle(V, SwizzleX, SwizzleY, SwizzleZ, SwizzleW);

#endif
    }

    // Specialized swizzles
    template<> inline constexpr vfloat4 VMATH_CALLCONV v4Swizzle<0, 1, 2, 3>(vfloat4 V) { return V; }

#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 1, 0, 1>(vfloat4 V) { return _mm_movelh_ps(V, V); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 3, 2, 3>(vfloat4 V) { return _mm_movehl_ps(V, V); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 0, 1, 1>(vfloat4 V) { return _mm_unpacklo_ps(V, V); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 2, 3, 3>(vfloat4 V) { return _mm_unpackhi_ps(V, V); }
#endif

#if defined(VMATH_SSE3_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 0, 2, 2>(vfloat4 V) { return _mm_moveldup_ps(V); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 1, 3, 3>(vfloat4 V) { return _mm_movehdup_ps(V); }
#endif

#if defined(VMATH_AVX2_INTRINSICS) && !defined(VMATH_NO_INTRINSICS) && defined(VMATH_FAVOR_INTEL)
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 0, 0, 0>(vfloat4 V) { return _mm_broadcastss_ps(V); }
#endif

#if defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 0, 0, 0>(vfloat4 V) { return vdupq_lane_f32(vget_low_f32(V), 0); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 1, 1, 1>(vfloat4 V) { return vdupq_lane_f32(vget_low_f32(V), 1); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 2, 2, 2>(vfloat4 V) { return vdupq_lane_f32(vget_high_f32(V), 0); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<3, 3, 3, 3>(vfloat4 V) { return vdupq_lane_f32(vget_high_f32(V), 1); }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 0, 3, 2>(vfloat4 V) { return vrev64q_f32(V); }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 1, 0, 1>(vfloat4 V) { float32x2_t vt = vget_low_f32(V); return vcombine_f32(vt, vt); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 3, 2, 3>(vfloat4 V) { float32x2_t vt = vget_high_f32(V); return vcombine_f32(vt, vt); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 0, 1, 0>(vfloat4 V) { float32x2_t vt = vrev64_f32(vget_low_f32(V)); return vcombine_f32(vt, vt); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<3, 2, 3, 2>(vfloat4 V) { float32x2_t vt = vrev64_f32(vget_high_f32(V)); return vcombine_f32(vt, vt); }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 1, 3, 2>(vfloat4 V) { return vcombine_f32(vget_low_f32(V), vrev64_f32(vget_high_f32(V))); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 0, 2, 3>(vfloat4 V) { return vcombine_f32(vrev64_f32(vget_low_f32(V)), vget_high_f32(V)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 3, 1, 0>(vfloat4 V) { return vcombine_f32(vget_high_f32(V), vrev64_f32(vget_low_f32(V))); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<3, 2, 0, 1>(vfloat4 V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vget_low_f32(V)); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<3, 2, 1, 0>(vfloat4 V) { return vcombine_f32(vrev64_f32(vget_high_f32(V)), vrev64_f32(vget_low_f32(V))); }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 0, 2, 2>(vfloat4 V) { return vtrnq_f32(V, V).val[0]; }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 1, 3, 3>(vfloat4 V) { return vtrnq_f32(V, V).val[1]; }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 0, 1, 1>(vfloat4 V) { return vzipq_f32(V, V).val[0]; }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 2, 3, 3>(vfloat4 V) { return vzipq_f32(V, V).val[1]; }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<0, 2, 0, 2>(vfloat4 V) { return vuzpq_f32(V, V).val[0]; }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 3, 1, 3>(vfloat4 V) { return vuzpq_f32(V, V).val[1]; }

    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<1, 2, 3, 0>(vfloat4 V) { return vextq_f32(V, V, 1); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<2, 3, 0, 1>(vfloat4 V) { return vextq_f32(V, V, 2); }
    template<> inline vfloat4 VMATH_CALLCONV v4Swizzle<3, 0, 1, 2>(vfloat4 V) { return vextq_f32(V, V, 3); }

#endif // VMATH_ARM_NEON_INTRINSICS && !VMATH_NO_INTRINSICS

    // XMVectorSwizzle<0, 0, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxxx(vfloat4 v) { return v4Swizzle<0, 0, 0, 0>(v); }

    // XMVectorSwizzle<0, 0, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxxy(vfloat4 v) { return v4Swizzle<0, 0, 0, 1>(v); }

    // XMVectorSwizzle<0, 0, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxxz(vfloat4 v) { return v4Swizzle<0, 0, 0, 2>(v); }

    // XMVectorSwizzle<0, 0, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxxw(vfloat4 v) { return v4Swizzle<0, 0, 0, 3>(v); }

    // XMVectorSwizzle<0, 0, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxyx(vfloat4 v) { return v4Swizzle<0, 0, 1, 0>(v); }

    // XMVectorSwizzle<0, 0, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxyy(vfloat4 v) { return v4Swizzle<0, 0, 1, 1>(v); }

    // XMVectorSwizzle<0, 0, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxyz(vfloat4 v) { return v4Swizzle<0, 0, 1, 2>(v); }

    // XMVectorSwizzle<0, 0, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxyw(vfloat4 v) { return v4Swizzle<0, 0, 1, 3>(v); }

    // XMVectorSwizzle<0, 0, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxzx(vfloat4 v) { return v4Swizzle<0, 0, 2, 0>(v); }

    // XMVectorSwizzle<0, 0, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxzy(vfloat4 v) { return v4Swizzle<0, 0, 2, 1>(v); }

    // XMVectorSwizzle<0, 0, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxzz(vfloat4 v) { return v4Swizzle<0, 0, 2, 2>(v); }

    // XMVectorSwizzle<0, 0, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxzw(vfloat4 v) { return v4Swizzle<0, 0, 2, 3>(v); }

    // XMVectorSwizzle<0, 0, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxwx(vfloat4 v) { return v4Swizzle<0, 0, 3, 0>(v); }

    // XMVectorSwizzle<0, 0, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxwy(vfloat4 v) { return v4Swizzle<0, 0, 3, 1>(v); }

    // XMVectorSwizzle<0, 0, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxwz(vfloat4 v) { return v4Swizzle<0, 0, 3, 2>(v); }

    // XMVectorSwizzle<0, 0, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xxww(vfloat4 v) { return v4Swizzle<0, 0, 3, 3>(v); }

    // XMVectorSwizzle<0, 1, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyxx(vfloat4 v) { return v4Swizzle<0, 1, 0, 0>(v); }

    // XMVectorSwizzle<0, 1, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyxy(vfloat4 v) { return v4Swizzle<0, 1, 0, 1>(v); }

    // XMVectorSwizzle<0, 1, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyxz(vfloat4 v) { return v4Swizzle<0, 1, 0, 2>(v); }

    // XMVectorSwizzle<0, 1, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyxw(vfloat4 v) { return v4Swizzle<0, 1, 0, 3>(v); }

    // XMVectorSwizzle<0, 1, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyyx(vfloat4 v) { return v4Swizzle<0, 1, 1, 0>(v); }

    // XMVectorSwizzle<0, 1, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyyy(vfloat4 v) { return v4Swizzle<0, 1, 1, 1>(v); }

    // XMVectorSwizzle<0, 1, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyyz(vfloat4 v) { return v4Swizzle<0, 1, 1, 2>(v); }

    // XMVectorSwizzle<0, 1, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyyw(vfloat4 v) { return v4Swizzle<0, 1, 1, 3>(v); }

    // XMVectorSwizzle<0, 1, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyzx(vfloat4 v) { return v4Swizzle<0, 1, 2, 0>(v); }

    // XMVectorSwizzle<0, 1, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyzy(vfloat4 v) { return v4Swizzle<0, 1, 2, 1>(v); }

    // XMVectorSwizzle<0, 1, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyzz(vfloat4 v) { return v4Swizzle<0, 1, 2, 2>(v); }

    // XMVectorSwizzle<0, 1, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyzw(vfloat4 v) { return v4Swizzle<0, 1, 2, 3>(v); }

    // XMVectorSwizzle<0, 1, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xywx(vfloat4 v) { return v4Swizzle<0, 1, 3, 0>(v); }

    // XMVectorSwizzle<0, 1, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xywy(vfloat4 v) { return v4Swizzle<0, 1, 3, 1>(v); }

    // XMVectorSwizzle<0, 1, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xywz(vfloat4 v) { return v4Swizzle<0, 1, 3, 2>(v); }

    // XMVectorSwizzle<0, 1, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xyww(vfloat4 v) { return v4Swizzle<0, 1, 3, 3>(v); }

    // XMVectorSwizzle<0, 2, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzxx(vfloat4 v) { return v4Swizzle<0, 2, 0, 0>(v); }

    // XMVectorSwizzle<0, 2, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzxy(vfloat4 v) { return v4Swizzle<0, 2, 0, 1>(v); }

    // XMVectorSwizzle<0, 2, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzxz(vfloat4 v) { return v4Swizzle<0, 2, 0, 2>(v); }

    // XMVectorSwizzle<0, 2, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzxw(vfloat4 v) { return v4Swizzle<0, 2, 0, 3>(v); }

    // XMVectorSwizzle<0, 2, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzyx(vfloat4 v) { return v4Swizzle<0, 2, 1, 0>(v); }

    // XMVectorSwizzle<0, 2, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzyy(vfloat4 v) { return v4Swizzle<0, 2, 1, 1>(v); }

    // XMVectorSwizzle<0, 2, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzyz(vfloat4 v) { return v4Swizzle<0, 2, 1, 2>(v); }

    // XMVectorSwizzle<0, 2, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzyw(vfloat4 v) { return v4Swizzle<0, 2, 1, 3>(v); }

    // XMVectorSwizzle<0, 2, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzzx(vfloat4 v) { return v4Swizzle<0, 2, 2, 0>(v); }

    // XMVectorSwizzle<0, 2, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzzy(vfloat4 v) { return v4Swizzle<0, 2, 2, 1>(v); }

    // XMVectorSwizzle<0, 2, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzzz(vfloat4 v) { return v4Swizzle<0, 2, 2, 2>(v); }

    // XMVectorSwizzle<0, 2, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzzw(vfloat4 v) { return v4Swizzle<0, 2, 2, 3>(v); }

    // XMVectorSwizzle<0, 2, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzwx(vfloat4 v) { return v4Swizzle<0, 2, 3, 0>(v); }

    // XMVectorSwizzle<0, 2, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzwy(vfloat4 v) { return v4Swizzle<0, 2, 3, 1>(v); }

    // XMVectorSwizzle<0, 2, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzwz(vfloat4 v) { return v4Swizzle<0, 2, 3, 2>(v); }

    // XMVectorSwizzle<0, 2, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xzww(vfloat4 v) { return v4Swizzle<0, 2, 3, 3>(v); }

    // XMVectorSwizzle<0, 3, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwxx(vfloat4 v) { return v4Swizzle<0, 3, 0, 0>(v); }

    // XMVectorSwizzle<0, 3, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwxy(vfloat4 v) { return v4Swizzle<0, 3, 0, 1>(v); }

    // XMVectorSwizzle<0, 3, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwxz(vfloat4 v) { return v4Swizzle<0, 3, 0, 2>(v); }

    // XMVectorSwizzle<0, 3, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwxw(vfloat4 v) { return v4Swizzle<0, 3, 0, 3>(v); }

    // XMVectorSwizzle<0, 3, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwyx(vfloat4 v) { return v4Swizzle<0, 3, 1, 0>(v); }

    // XMVectorSwizzle<0, 3, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwyy(vfloat4 v) { return v4Swizzle<0, 3, 1, 1>(v); }

    // XMVectorSwizzle<0, 3, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwyz(vfloat4 v) { return v4Swizzle<0, 3, 1, 2>(v); }

    // XMVectorSwizzle<0, 3, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwyw(vfloat4 v) { return v4Swizzle<0, 3, 1, 3>(v); }

    // XMVectorSwizzle<0, 3, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwzx(vfloat4 v) { return v4Swizzle<0, 3, 2, 0>(v); }

    // XMVectorSwizzle<0, 3, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwzy(vfloat4 v) { return v4Swizzle<0, 3, 2, 1>(v); }

    // XMVectorSwizzle<0, 3, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwzz(vfloat4 v) { return v4Swizzle<0, 3, 2, 2>(v); }

    // XMVectorSwizzle<0, 3, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwzw(vfloat4 v) { return v4Swizzle<0, 3, 2, 3>(v); }

    // XMVectorSwizzle<0, 3, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwwx(vfloat4 v) { return v4Swizzle<0, 3, 3, 0>(v); }

    // XMVectorSwizzle<0, 3, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwwy(vfloat4 v) { return v4Swizzle<0, 3, 3, 1>(v); }

    // XMVectorSwizzle<0, 3, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwwz(vfloat4 v) { return v4Swizzle<0, 3, 3, 2>(v); }

    // XMVectorSwizzle<0, 3, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4xwww(vfloat4 v) { return v4Swizzle<0, 3, 3, 3>(v); }

    // XMVectorSwizzle<1, 0, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxxx(vfloat4 v) { return v4Swizzle<1, 0, 0, 0>(v); }

    // XMVectorSwizzle<1, 0, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxxy(vfloat4 v) { return v4Swizzle<1, 0, 0, 1>(v); }

    // XMVectorSwizzle<1, 0, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxxz(vfloat4 v) { return v4Swizzle<1, 0, 0, 2>(v); }

    // XMVectorSwizzle<1, 0, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxxw(vfloat4 v) { return v4Swizzle<1, 0, 0, 3>(v); }

    // XMVectorSwizzle<1, 0, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxyx(vfloat4 v) { return v4Swizzle<1, 0, 1, 0>(v); }

    // XMVectorSwizzle<1, 0, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxyy(vfloat4 v) { return v4Swizzle<1, 0, 1, 1>(v); }

    // XMVectorSwizzle<1, 0, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxyz(vfloat4 v) { return v4Swizzle<1, 0, 1, 2>(v); }

    // XMVectorSwizzle<1, 0, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxyw(vfloat4 v) { return v4Swizzle<1, 0, 1, 3>(v); }

    // XMVectorSwizzle<1, 0, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxzx(vfloat4 v) { return v4Swizzle<1, 0, 2, 0>(v); }

    // XMVectorSwizzle<1, 0, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxzy(vfloat4 v) { return v4Swizzle<1, 0, 2, 1>(v); }

    // XMVectorSwizzle<1, 0, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxzz(vfloat4 v) { return v4Swizzle<1, 0, 2, 2>(v); }

    // XMVectorSwizzle<1, 0, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxzw(vfloat4 v) { return v4Swizzle<1, 0, 2, 3>(v); }

    // XMVectorSwizzle<1, 0, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxwx(vfloat4 v) { return v4Swizzle<1, 0, 3, 0>(v); }

    // XMVectorSwizzle<1, 0, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxwy(vfloat4 v) { return v4Swizzle<1, 0, 3, 1>(v); }

    // XMVectorSwizzle<1, 0, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxwz(vfloat4 v) { return v4Swizzle<1, 0, 3, 2>(v); }

    // XMVectorSwizzle<1, 0, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yxww(vfloat4 v) { return v4Swizzle<1, 0, 3, 3>(v); }

    // XMVectorSwizzle<1, 1, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyxx(vfloat4 v) { return v4Swizzle<1, 1, 0, 0>(v); }

    // XMVectorSwizzle<1, 1, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyxy(vfloat4 v) { return v4Swizzle<1, 1, 0, 1>(v); }

    // XMVectorSwizzle<1, 1, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyxz(vfloat4 v) { return v4Swizzle<1, 1, 0, 2>(v); }

    // XMVectorSwizzle<1, 1, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyxw(vfloat4 v) { return v4Swizzle<1, 1, 0, 3>(v); }

    // XMVectorSwizzle<1, 1, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyyx(vfloat4 v) { return v4Swizzle<1, 1, 1, 0>(v); }

    // XMVectorSwizzle<1, 1, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyyy(vfloat4 v) { return v4Swizzle<1, 1, 1, 1>(v); }

    // XMVectorSwizzle<1, 1, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyyz(vfloat4 v) { return v4Swizzle<1, 1, 1, 2>(v); }

    // XMVectorSwizzle<1, 1, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyyw(vfloat4 v) { return v4Swizzle<1, 1, 1, 3>(v); }

    // XMVectorSwizzle<1, 1, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyzx(vfloat4 v) { return v4Swizzle<1, 1, 2, 0>(v); }

    // XMVectorSwizzle<1, 1, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyzy(vfloat4 v) { return v4Swizzle<1, 1, 2, 1>(v); }

    // XMVectorSwizzle<1, 1, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyzz(vfloat4 v) { return v4Swizzle<1, 1, 2, 2>(v); }

    // XMVectorSwizzle<1, 1, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyzw(vfloat4 v) { return v4Swizzle<1, 1, 2, 3>(v); }

    // XMVectorSwizzle<1, 1, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yywx(vfloat4 v) { return v4Swizzle<1, 1, 3, 0>(v); }

    // XMVectorSwizzle<1, 1, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yywy(vfloat4 v) { return v4Swizzle<1, 1, 3, 1>(v); }

    // XMVectorSwizzle<1, 1, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yywz(vfloat4 v) { return v4Swizzle<1, 1, 3, 2>(v); }

    // XMVectorSwizzle<1, 1, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yyww(vfloat4 v) { return v4Swizzle<1, 1, 3, 3>(v); }

    // XMVectorSwizzle<1, 2, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzxx(vfloat4 v) { return v4Swizzle<1, 2, 0, 0>(v); }

    // XMVectorSwizzle<1, 2, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzxy(vfloat4 v) { return v4Swizzle<1, 2, 0, 1>(v); }

    // XMVectorSwizzle<1, 2, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzxz(vfloat4 v) { return v4Swizzle<1, 2, 0, 2>(v); }

    // XMVectorSwizzle<1, 2, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzxw(vfloat4 v) { return v4Swizzle<1, 2, 0, 3>(v); }

    // XMVectorSwizzle<1, 2, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzyx(vfloat4 v) { return v4Swizzle<1, 2, 1, 0>(v); }

    // XMVectorSwizzle<1, 2, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzyy(vfloat4 v) { return v4Swizzle<1, 2, 1, 1>(v); }

    // XMVectorSwizzle<1, 2, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzyz(vfloat4 v) { return v4Swizzle<1, 2, 1, 2>(v); }

    // XMVectorSwizzle<1, 2, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzyw(vfloat4 v) { return v4Swizzle<1, 2, 1, 3>(v); }

    // XMVectorSwizzle<1, 2, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzzx(vfloat4 v) { return v4Swizzle<1, 2, 2, 0>(v); }

    // XMVectorSwizzle<1, 2, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzzy(vfloat4 v) { return v4Swizzle<1, 2, 2, 1>(v); }

    // XMVectorSwizzle<1, 2, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzzz(vfloat4 v) { return v4Swizzle<1, 2, 2, 2>(v); }

    // XMVectorSwizzle<1, 2, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzzw(vfloat4 v) { return v4Swizzle<1, 2, 2, 3>(v); }

    // XMVectorSwizzle<1, 2, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzwx(vfloat4 v) { return v4Swizzle<1, 2, 3, 0>(v); }

    // XMVectorSwizzle<1, 2, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzwy(vfloat4 v) { return v4Swizzle<1, 2, 3, 1>(v); }

    // XMVectorSwizzle<1, 2, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzwz(vfloat4 v) { return v4Swizzle<1, 2, 3, 2>(v); }

    // XMVectorSwizzle<1, 2, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4yzww(vfloat4 v) { return v4Swizzle<1, 2, 3, 3>(v); }

    // XMVectorSwizzle<1, 3, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywxx(vfloat4 v) { return v4Swizzle<1, 3, 0, 0>(v); }

    // XMVectorSwizzle<1, 3, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywxy(vfloat4 v) { return v4Swizzle<1, 3, 0, 1>(v); }

    // XMVectorSwizzle<1, 3, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywxz(vfloat4 v) { return v4Swizzle<1, 3, 0, 2>(v); }

    // XMVectorSwizzle<1, 3, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywxw(vfloat4 v) { return v4Swizzle<1, 3, 0, 3>(v); }

    // XMVectorSwizzle<1, 3, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywyx(vfloat4 v) { return v4Swizzle<1, 3, 1, 0>(v); }

    // XMVectorSwizzle<1, 3, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywyy(vfloat4 v) { return v4Swizzle<1, 3, 1, 1>(v); }

    // XMVectorSwizzle<1, 3, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywyz(vfloat4 v) { return v4Swizzle<1, 3, 1, 2>(v); }

    // XMVectorSwizzle<1, 3, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywyw(vfloat4 v) { return v4Swizzle<1, 3, 1, 3>(v); }

    // XMVectorSwizzle<1, 3, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywzx(vfloat4 v) { return v4Swizzle<1, 3, 2, 0>(v); }

    // XMVectorSwizzle<1, 3, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywzy(vfloat4 v) { return v4Swizzle<1, 3, 2, 1>(v); }

    // XMVectorSwizzle<1, 3, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywzz(vfloat4 v) { return v4Swizzle<1, 3, 2, 2>(v); }

    // XMVectorSwizzle<1, 3, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywzw(vfloat4 v) { return v4Swizzle<1, 3, 2, 3>(v); }

    // XMVectorSwizzle<1, 3, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywwx(vfloat4 v) { return v4Swizzle<1, 3, 3, 0>(v); }

    // XMVectorSwizzle<1, 3, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywwy(vfloat4 v) { return v4Swizzle<1, 3, 3, 1>(v); }

    // XMVectorSwizzle<1, 3, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywwz(vfloat4 v) { return v4Swizzle<1, 3, 3, 2>(v); }

    // XMVectorSwizzle<1, 3, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ywww(vfloat4 v) { return v4Swizzle<1, 3, 3, 3>(v); }

    // XMVectorSwizzle<2, 0, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxxx(vfloat4 v) { return v4Swizzle<2, 0, 0, 0>(v); }

    // XMVectorSwizzle<2, 0, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxxy(vfloat4 v) { return v4Swizzle<2, 0, 0, 1>(v); }

    // XMVectorSwizzle<2, 0, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxxz(vfloat4 v) { return v4Swizzle<2, 0, 0, 2>(v); }

    // XMVectorSwizzle<2, 0, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxxw(vfloat4 v) { return v4Swizzle<2, 0, 0, 3>(v); }

    // XMVectorSwizzle<2, 0, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxyx(vfloat4 v) { return v4Swizzle<2, 0, 1, 0>(v); }

    // XMVectorSwizzle<2, 0, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxyy(vfloat4 v) { return v4Swizzle<2, 0, 1, 1>(v); }

    // XMVectorSwizzle<2, 0, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxyz(vfloat4 v) { return v4Swizzle<2, 0, 1, 2>(v); }

    // XMVectorSwizzle<2, 0, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxyw(vfloat4 v) { return v4Swizzle<2, 0, 1, 3>(v); }

    // XMVectorSwizzle<2, 0, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxzx(vfloat4 v) { return v4Swizzle<2, 0, 2, 0>(v); }

    // XMVectorSwizzle<2, 0, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxzy(vfloat4 v) { return v4Swizzle<2, 0, 2, 1>(v); }

    // XMVectorSwizzle<2, 0, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxzz(vfloat4 v) { return v4Swizzle<2, 0, 2, 2>(v); }

    // XMVectorSwizzle<2, 0, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxzw(vfloat4 v) { return v4Swizzle<2, 0, 2, 3>(v); }

    // XMVectorSwizzle<2, 0, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxwx(vfloat4 v) { return v4Swizzle<2, 0, 3, 0>(v); }

    // XMVectorSwizzle<2, 0, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxwy(vfloat4 v) { return v4Swizzle<2, 0, 3, 1>(v); }

    // XMVectorSwizzle<2, 0, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxwz(vfloat4 v) { return v4Swizzle<2, 0, 3, 2>(v); }

    // XMVectorSwizzle<2, 0, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zxww(vfloat4 v) { return v4Swizzle<2, 0, 3, 3>(v); }

    // XMVectorSwizzle<2, 1, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyxx(vfloat4 v) { return v4Swizzle<2, 1, 0, 0>(v); }

    // XMVectorSwizzle<2, 1, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyxy(vfloat4 v) { return v4Swizzle<2, 1, 0, 1>(v); }

    // XMVectorSwizzle<2, 1, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyxz(vfloat4 v) { return v4Swizzle<2, 1, 0, 2>(v); }

    // XMVectorSwizzle<2, 1, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyxw(vfloat4 v) { return v4Swizzle<2, 1, 0, 3>(v); }

    // XMVectorSwizzle<2, 1, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyyx(vfloat4 v) { return v4Swizzle<2, 1, 1, 0>(v); }

    // XMVectorSwizzle<2, 1, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyyy(vfloat4 v) { return v4Swizzle<2, 1, 1, 1>(v); }

    // XMVectorSwizzle<2, 1, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyyz(vfloat4 v) { return v4Swizzle<2, 1, 1, 2>(v); }

    // XMVectorSwizzle<2, 1, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyyw(vfloat4 v) { return v4Swizzle<2, 1, 1, 3>(v); }

    // XMVectorSwizzle<2, 1, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyzx(vfloat4 v) { return v4Swizzle<2, 1, 2, 0>(v); }

    // XMVectorSwizzle<2, 1, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyzy(vfloat4 v) { return v4Swizzle<2, 1, 2, 1>(v); }

    // XMVectorSwizzle<2, 1, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyzz(vfloat4 v) { return v4Swizzle<2, 1, 2, 2>(v); }

    // XMVectorSwizzle<2, 1, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyzw(vfloat4 v) { return v4Swizzle<2, 1, 2, 3>(v); }

    // XMVectorSwizzle<2, 1, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zywx(vfloat4 v) { return v4Swizzle<2, 1, 3, 0>(v); }

    // XMVectorSwizzle<2, 1, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zywy(vfloat4 v) { return v4Swizzle<2, 1, 3, 1>(v); }

    // XMVectorSwizzle<2, 1, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zywz(vfloat4 v) { return v4Swizzle<2, 1, 3, 2>(v); }

    // XMVectorSwizzle<2, 1, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zyww(vfloat4 v) { return v4Swizzle<2, 1, 3, 3>(v); }

    // XMVectorSwizzle<2, 2, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzxx(vfloat4 v) { return v4Swizzle<2, 2, 0, 0>(v); }

    // XMVectorSwizzle<2, 2, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzxy(vfloat4 v) { return v4Swizzle<2, 2, 0, 1>(v); }

    // XMVectorSwizzle<2, 2, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzxz(vfloat4 v) { return v4Swizzle<2, 2, 0, 2>(v); }

    // XMVectorSwizzle<2, 2, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzxw(vfloat4 v) { return v4Swizzle<2, 2, 0, 3>(v); }

    // XMVectorSwizzle<2, 2, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzyx(vfloat4 v) { return v4Swizzle<2, 2, 1, 0>(v); }

    // XMVectorSwizzle<2, 2, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzyy(vfloat4 v) { return v4Swizzle<2, 2, 1, 1>(v); }

    // XMVectorSwizzle<2, 2, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzyz(vfloat4 v) { return v4Swizzle<2, 2, 1, 2>(v); }

    // XMVectorSwizzle<2, 2, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzyw(vfloat4 v) { return v4Swizzle<2, 2, 1, 3>(v); }

    // XMVectorSwizzle<2, 2, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzzx(vfloat4 v) { return v4Swizzle<2, 2, 2, 0>(v); }

    // XMVectorSwizzle<2, 2, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzzy(vfloat4 v) { return v4Swizzle<2, 2, 2, 1>(v); }

    // XMVectorSwizzle<2, 2, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzzz(vfloat4 v) { return v4Swizzle<2, 2, 2, 2>(v); }

    // XMVectorSwizzle<2, 2, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzzw(vfloat4 v) { return v4Swizzle<2, 2, 2, 3>(v); }

    // XMVectorSwizzle<2, 2, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzwx(vfloat4 v) { return v4Swizzle<2, 2, 3, 0>(v); }

    // XMVectorSwizzle<2, 2, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzwy(vfloat4 v) { return v4Swizzle<2, 2, 3, 1>(v); }

    // XMVectorSwizzle<2, 2, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzwz(vfloat4 v) { return v4Swizzle<2, 2, 3, 2>(v); }

    // XMVectorSwizzle<2, 2, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zzww(vfloat4 v) { return v4Swizzle<2, 2, 3, 3>(v); }

    // XMVectorSwizzle<2, 3, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwxx(vfloat4 v) { return v4Swizzle<2, 3, 0, 0>(v); }

    // XMVectorSwizzle<2, 3, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwxy(vfloat4 v) { return v4Swizzle<2, 3, 0, 1>(v); }

    // XMVectorSwizzle<2, 3, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwxz(vfloat4 v) { return v4Swizzle<2, 3, 0, 2>(v); }

    // XMVectorSwizzle<2, 3, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwxw(vfloat4 v) { return v4Swizzle<2, 3, 0, 3>(v); }

    // XMVectorSwizzle<2, 3, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwyx(vfloat4 v) { return v4Swizzle<2, 3, 1, 0>(v); }

    // XMVectorSwizzle<2, 3, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwyy(vfloat4 v) { return v4Swizzle<2, 3, 1, 1>(v); }

    // XMVectorSwizzle<2, 3, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwyz(vfloat4 v) { return v4Swizzle<2, 3, 1, 2>(v); }

    // XMVectorSwizzle<2, 3, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwyw(vfloat4 v) { return v4Swizzle<2, 3, 1, 3>(v); }

    // XMVectorSwizzle<2, 3, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwzx(vfloat4 v) { return v4Swizzle<2, 3, 2, 0>(v); }

    // XMVectorSwizzle<2, 3, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwzy(vfloat4 v) { return v4Swizzle<2, 3, 2, 1>(v); }

    // XMVectorSwizzle<2, 3, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwzz(vfloat4 v) { return v4Swizzle<2, 3, 2, 2>(v); }

    // XMVectorSwizzle<2, 3, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwzw(vfloat4 v) { return v4Swizzle<2, 3, 2, 3>(v); }

    // XMVectorSwizzle<2, 3, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwwx(vfloat4 v) { return v4Swizzle<2, 3, 3, 0>(v); }

    // XMVectorSwizzle<2, 3, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwwy(vfloat4 v) { return v4Swizzle<2, 3, 3, 1>(v); }

    // XMVectorSwizzle<2, 3, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwwz(vfloat4 v) { return v4Swizzle<2, 3, 3, 2>(v); }

    // XMVectorSwizzle<2, 3, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4zwww(vfloat4 v) { return v4Swizzle<2, 3, 3, 3>(v); }

    // XMVectorSwizzle<3, 0, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxxx(vfloat4 v) { return v4Swizzle<3, 0, 0, 0>(v); }

    // XMVectorSwizzle<3, 0, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxxy(vfloat4 v) { return v4Swizzle<3, 0, 0, 1>(v); }

    // XMVectorSwizzle<3, 0, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxxz(vfloat4 v) { return v4Swizzle<3, 0, 0, 2>(v); }

    // XMVectorSwizzle<3, 0, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxxw(vfloat4 v) { return v4Swizzle<3, 0, 0, 3>(v); }

    // XMVectorSwizzle<3, 0, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxyx(vfloat4 v) { return v4Swizzle<3, 0, 1, 0>(v); }

    // XMVectorSwizzle<3, 0, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxyy(vfloat4 v) { return v4Swizzle<3, 0, 1, 1>(v); }

    // XMVectorSwizzle<3, 0, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxyz(vfloat4 v) { return v4Swizzle<3, 0, 1, 2>(v); }

    // XMVectorSwizzle<3, 0, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxyw(vfloat4 v) { return v4Swizzle<3, 0, 1, 3>(v); }

    // XMVectorSwizzle<3, 0, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxzx(vfloat4 v) { return v4Swizzle<3, 0, 2, 0>(v); }

    // XMVectorSwizzle<3, 0, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxzy(vfloat4 v) { return v4Swizzle<3, 0, 2, 1>(v); }

    // XMVectorSwizzle<3, 0, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxzz(vfloat4 v) { return v4Swizzle<3, 0, 2, 2>(v); }

    // XMVectorSwizzle<3, 0, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxzw(vfloat4 v) { return v4Swizzle<3, 0, 2, 3>(v); }

    // XMVectorSwizzle<3, 0, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxwx(vfloat4 v) { return v4Swizzle<3, 0, 3, 0>(v); }

    // XMVectorSwizzle<3, 0, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxwy(vfloat4 v) { return v4Swizzle<3, 0, 3, 1>(v); }

    // XMVectorSwizzle<3, 0, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxwz(vfloat4 v) { return v4Swizzle<3, 0, 3, 2>(v); }

    // XMVectorSwizzle<3, 0, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wxww(vfloat4 v) { return v4Swizzle<3, 0, 3, 3>(v); }

    // XMVectorSwizzle<3, 1, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyxx(vfloat4 v) { return v4Swizzle<3, 1, 0, 0>(v); }

    // XMVectorSwizzle<3, 1, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyxy(vfloat4 v) { return v4Swizzle<3, 1, 0, 1>(v); }

    // XMVectorSwizzle<3, 1, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyxz(vfloat4 v) { return v4Swizzle<3, 1, 0, 2>(v); }

    // XMVectorSwizzle<3, 1, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyxw(vfloat4 v) { return v4Swizzle<3, 1, 0, 3>(v); }

    // XMVectorSwizzle<3, 1, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyyx(vfloat4 v) { return v4Swizzle<3, 1, 1, 0>(v); }

    // XMVectorSwizzle<3, 1, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyyy(vfloat4 v) { return v4Swizzle<3, 1, 1, 1>(v); }

    // XMVectorSwizzle<3, 1, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyyz(vfloat4 v) { return v4Swizzle<3, 1, 1, 2>(v); }

    // XMVectorSwizzle<3, 1, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyyw(vfloat4 v) { return v4Swizzle<3, 1, 1, 3>(v); }

    // XMVectorSwizzle<3, 1, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyzx(vfloat4 v) { return v4Swizzle<3, 1, 2, 0>(v); }

    // XMVectorSwizzle<3, 1, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyzy(vfloat4 v) { return v4Swizzle<3, 1, 2, 1>(v); }

    // XMVectorSwizzle<3, 1, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyzz(vfloat4 v) { return v4Swizzle<3, 1, 2, 2>(v); }

    // XMVectorSwizzle<3, 1, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyzw(vfloat4 v) { return v4Swizzle<3, 1, 2, 3>(v); }

    // XMVectorSwizzle<3, 1, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wywx(vfloat4 v) { return v4Swizzle<3, 1, 3, 0>(v); }

    // XMVectorSwizzle<3, 1, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wywy(vfloat4 v) { return v4Swizzle<3, 1, 3, 1>(v); }

    // XMVectorSwizzle<3, 1, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wywz(vfloat4 v) { return v4Swizzle<3, 1, 3, 2>(v); }

    // XMVectorSwizzle<3, 1, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wyww(vfloat4 v) { return v4Swizzle<3, 1, 3, 3>(v); }

    // XMVectorSwizzle<3, 2, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzxx(vfloat4 v) { return v4Swizzle<3, 2, 0, 0>(v); }

    // XMVectorSwizzle<3, 2, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzxy(vfloat4 v) { return v4Swizzle<3, 2, 0, 1>(v); }

    // XMVectorSwizzle<3, 2, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzxz(vfloat4 v) { return v4Swizzle<3, 2, 0, 2>(v); }

    // XMVectorSwizzle<3, 2, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzxw(vfloat4 v) { return v4Swizzle<3, 2, 0, 3>(v); }

    // XMVectorSwizzle<3, 2, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzyx(vfloat4 v) { return v4Swizzle<3, 2, 1, 0>(v); }

    // XMVectorSwizzle<3, 2, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzyy(vfloat4 v) { return v4Swizzle<3, 2, 1, 1>(v); }

    // XMVectorSwizzle<3, 2, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzyz(vfloat4 v) { return v4Swizzle<3, 2, 1, 2>(v); }

    // XMVectorSwizzle<3, 2, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzyw(vfloat4 v) { return v4Swizzle<3, 2, 1, 3>(v); }

    // XMVectorSwizzle<3, 2, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzzx(vfloat4 v) { return v4Swizzle<3, 2, 2, 0>(v); }

    // XMVectorSwizzle<3, 2, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzzy(vfloat4 v) { return v4Swizzle<3, 2, 2, 1>(v); }

    // XMVectorSwizzle<3, 2, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzzz(vfloat4 v) { return v4Swizzle<3, 2, 2, 2>(v); }

    // XMVectorSwizzle<3, 2, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzzw(vfloat4 v) { return v4Swizzle<3, 2, 2, 3>(v); }

    // XMVectorSwizzle<3, 2, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzwx(vfloat4 v) { return v4Swizzle<3, 2, 3, 0>(v); }

    // XMVectorSwizzle<3, 2, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzwy(vfloat4 v) { return v4Swizzle<3, 2, 3, 1>(v); }

    // XMVectorSwizzle<3, 2, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzwz(vfloat4 v) { return v4Swizzle<3, 2, 3, 2>(v); }

    // XMVectorSwizzle<3, 2, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wzww(vfloat4 v) { return v4Swizzle<3, 2, 3, 3>(v); }

    // XMVectorSwizzle<3, 3, 0, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwxx(vfloat4 v) { return v4Swizzle<3, 3, 0, 0>(v); }

    // XMVectorSwizzle<3, 3, 0, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwxy(vfloat4 v) { return v4Swizzle<3, 3, 0, 1>(v); }

    // XMVectorSwizzle<3, 3, 0, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwxz(vfloat4 v) { return v4Swizzle<3, 3, 0, 2>(v); }

    // XMVectorSwizzle<3, 3, 0, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwxw(vfloat4 v) { return v4Swizzle<3, 3, 0, 3>(v); }

    // XMVectorSwizzle<3, 3, 1, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwyx(vfloat4 v) { return v4Swizzle<3, 3, 1, 0>(v); }

    // XMVectorSwizzle<3, 3, 1, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwyy(vfloat4 v) { return v4Swizzle<3, 3, 1, 1>(v); }

    // XMVectorSwizzle<3, 3, 1, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwyz(vfloat4 v) { return v4Swizzle<3, 3, 1, 2>(v); }

    // XMVectorSwizzle<3, 3, 1, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwyw(vfloat4 v) { return v4Swizzle<3, 3, 1, 3>(v); }

    // XMVectorSwizzle<3, 3, 2, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwzx(vfloat4 v) { return v4Swizzle<3, 3, 2, 0>(v); }

    // XMVectorSwizzle<3, 3, 2, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwzy(vfloat4 v) { return v4Swizzle<3, 3, 2, 1>(v); }

    // XMVectorSwizzle<3, 3, 2, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwzz(vfloat4 v) { return v4Swizzle<3, 3, 2, 2>(v); }

    // XMVectorSwizzle<3, 3, 2, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwzw(vfloat4 v) { return v4Swizzle<3, 3, 2, 3>(v); }

    // XMVectorSwizzle<3, 3, 3, 0>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwwx(vfloat4 v) { return v4Swizzle<3, 3, 3, 0>(v); }

    // XMVectorSwizzle<3, 3, 3, 1>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwwy(vfloat4 v) { return v4Swizzle<3, 3, 3, 1>(v); }

    // XMVectorSwizzle<3, 3, 3, 2>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwwz(vfloat4 v) { return v4Swizzle<3, 3, 3, 2>(v); }

    // XMVectorSwizzle<3, 3, 3, 3>(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4wwww(vfloat4 v) { return v4Swizzle<3, 3, 3, 3>(v); }

    //------------------------------------------------------------------------------

    template<uint32_t Elements>
    inline vfloat4 VMATH_CALLCONV v4PushLeft(vfloat4 V1, vfloat4 V2)
    {
        static_assert(Elements < 4, "Elements template parameter out of range");
        return v4Shuffle<Elements, (Elements + 1), (Elements + 2), (Elements + 3)>(V1, V2);
    }

    template<uint32_t Elements>
    inline vfloat4 VMATH_CALLCONV v4RotateLeft(vfloat4 V)
    {
        static_assert(Elements < 4, "Elements template parameter out of range");
        return v4Swizzle<Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3>(V);
    }

    template<uint32_t Elements>
    inline vfloat4 VMATH_CALLCONV v4RotateRight(vfloat4 V)
    {
        static_assert(Elements < 4, "Elements template parameter out of range");
        return v4Swizzle<(4 - Elements) & 3, (5 - Elements) & 3, (6 - Elements) & 3, (7 - Elements) & 3>(V);
    }

    template<uint32_t VSLeftRotateElements, uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3>
    inline vfloat4 VMATH_CALLCONV v4Insert(vfloat4 VD, vfloat4 VS)
    {
        vfloat4 Control = v4SelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
        return v4Select(VD, v4RotateLeft<VSLeftRotateElements>(VS), Control);
    }

    /****************************************************************************
     *
     * Implementation
     *
     ****************************************************************************/


     //------------------------------------------------------------------------------
     // XMVECTOR XMLoadInt(const uint32_t* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadBinary32(const uint32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4u[0] = *pSource;
        V.v4u[1] = 0;
        V.v4u[2] = 0;
        V.v4u[3] = 0;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t zero = vdupq_n_u32(0);
        return vreinterpretq_f32_u32(vld1q_lane_u32(pSource, zero, 0));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_load_ss(reinterpret_cast<const float*>(pSource));
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadFloat(const float* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadFloat(const float* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = *pSource;
        V.v4f[1] = 0.f;
        V.v4f[2] = 0.f;
        V.v4f[3] = 0.f;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t zero = vdupq_n_f32(0);
        return vld1q_lane_f32(pSource, zero, 0);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_load_ss(pSource);
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadInt2(const uint32_t* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadBinary32x2(const uint32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4u[0] = pSource[0];
        V.v4u[1] = pSource[1];
        V.v4u[2] = 0;
        V.v4u[3] = 0;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t x = vld1_u32(pSource);
        uint32x2_t zero = vdup_n_u32(0);
        return vreinterpretq_f32_u32(vcombine_u32(x, zero));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadFloat2(const XMFLOAT2* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadFloat2(const float* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = pSource[0];
        V.v4f[1] = pSource[1];
        V.v4f[2] = 0.f;
        V.v4f[3] = 0.f;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t x = vld1_f32(pSource);
        float32x2_t zero = vdup_n_f32(0);
        return vcombine_f32(x, zero);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
#endif
    }


    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadSInt2(const XMINT2* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadCastInt2(const int32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = float(pSource[0]);
        V.v4f[1] = float(pSource[1]);
        V.v4f[2] = 0.f;
        V.v4f[3] = 0.f;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x2_t x = vld1_s32(pSource);
        float32x2_t v = vcvt_f32_s32(x);
        float32x2_t zero = vdup_n_f32(0);
        return vcombine_f32(v, zero);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 V = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        return _mm_cvtepi32_ps(_mm_castps_si128(V));
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadUInt2(const XMUINT2* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadCastUint2(const uint32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = float(pSource[0]);
        V.v4f[1] = float(pSource[2]);
        V.v4f[2] = 0.f;
        V.v4f[3] = 0.f;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t x = vld1_u32(pSource);
        float32x2_t v = vcvt_f32_u32(x);
        float32x2_t zero = vdup_n_f32(0);
        return vcombine_f32(v, zero);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 V = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        // For the values that are higher than 0x7FFFFFFF, a fixup is needed
        // Determine which ones need the fix.
        vfloat4 vMask = _mm_and_ps(V, Internal::kNegativeZero);
        // Force all values positive
        vfloat4 vResult = _mm_xor_ps(V, vMask);
        // Convert to floats
        vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
        // Convert 0x80000000 -> 0xFFFFFFFF
        __m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
        // For only the ones that are too big, add the fixup
        vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Internal::kFixUnsigned);
        vResult = _mm_add_ps(vResult, vMask);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadInt3(const uint32_t* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadBinary32x3(const uint32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4u[0] = pSource[0];
        V.v4u[1] = pSource[1];
        V.v4u[2] = pSource[2];
        V.v4u[3] = 0;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t x = vld1_u32(pSource);
        uint32x2_t zero = vdup_n_u32(0);
        uint32x2_t y = vld1_lane_u32(pSource + 2, zero, 0);
        return vreinterpretq_f32_u32(vcombine_u32(x, y));
#elif defined(VMATH_SSE4_INTRINSICS)
        __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        __m128 z = _mm_load_ss(reinterpret_cast<const float*>(pSource + 2));
        return _mm_insert_ps(xy, z, 0x20);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        __m128 z = _mm_load_ss(reinterpret_cast<const float*>(pSource + 2));
        return _mm_movelh_ps(xy, z);
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadFloat3(const XMFLOAT3* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadFloat3(const float* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = pSource[0];
        V.v4f[1] = pSource[1];
        V.v4f[2] = pSource[2];
        V.v4f[3] = 0.f;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t x = vld1_f32(pSource);
        float32x2_t zero = vdup_n_f32(0);
        float32x2_t y = vld1_lane_f32(pSource + 2, zero, 0);
        return vcombine_f32(x, y);
#elif defined(VMATH_SSE4_INTRINSICS)
        __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        __m128 z = _mm_load_ss(pSource + 2);
        return _mm_insert_ps(xy, z, 0x20);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        __m128 z = _mm_load_ss(pSource + 2);
        return _mm_movelh_ps(xy, z);
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadSInt3(const XMINT3* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadCastInt3(const int32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 V;
        V.v4f[0] = float(pSource[0]);
        V.v4f[1] = float(pSource[1]);
        V.v4f[2] = float(pSource[2]);
        V.v4f[3] = 0.f;
        return V;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x2_t x = vld1_s32(pSource);
        int32x2_t zero = vdup_n_s32(0);
        int32x2_t y = vld1_lane_s32(pSource + 2, zero, 0);
        int32x4_t v = vcombine_s32(x, y);
        return vcvtq_f32_s32(v);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        __m128 z = _mm_load_ss(reinterpret_cast<const float*>(pSource + 2));
        __m128 V = _mm_movelh_ps(xy, z);
        return _mm_cvtepi32_ps(_mm_castps_si128(V));
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadUInt3(const XMUINT3* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadCastUint3(const uint32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = float(pSource[0]);
        V.v4f[1] = float(pSource[1]);
        V.v4f[2] = float(pSource[2]);
        V.v4f[3] = 0.f;
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t x = vld1_u32(pSource);
        uint32x2_t zero = vdup_n_u32(0);
        uint32x2_t y = vld1_lane_u32(pSource + 2, zero, 0);
        uint32x4_t v = vcombine_u32(x, y);
        return vcvtq_f32_u32(v);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(pSource)));
        __m128 z = _mm_load_ss(reinterpret_cast<const float*>(pSource + 2));
        __m128 V = _mm_movelh_ps(xy, z);
        // For the values that are higher than 0x7FFFFFFF, a fixup is needed
        // Determine which ones need the fix.
        vfloat4 vMask = _mm_and_ps(V, Internal::kNegativeZero);
        // Force all values positive
        vfloat4 vResult = _mm_xor_ps(V, vMask);
        // Convert to floats
        vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
        // Convert 0x80000000 -> 0xFFFFFFFF
        __m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
        // For only the ones that are too big, add the fixup
        vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Internal::kFixUnsigned);
        vResult = _mm_add_ps(vResult, vMask);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadInt4(const uint32_t* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadBinary32x4(const uint32_t* pSource)
    {
        assert(pSource);

#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4u[0] = pSource[0];
        V.v4u[1] = pSource[1];
        V.v4u[2] = pSource[2];
        V.v4u[3] = pSource[3];
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vld1q_u32(pSource));
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
        return _mm_castsi128_ps(V);
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadFloat4(const XMFLOAT4* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadFloat4(const float* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = pSource[0];
        V.v4f[1] = pSource[1];
        V.v4f[2] = pSource[2];
        V.v4f[3] = pSource[3];
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vld1q_f32(pSource);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_loadu_ps(pSource);
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadSInt4(const XMINT4* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadCastInt4(const int32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 V;
        V.v4f[0] = float(pSource[0]);
        V.v4f[1] = float(pSource[1]);
        V.v4f[2] = float(pSource[2]);
        V.v4f[3] = float(pSource[3]);
        return V;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t v = vld1q_s32(pSource);
        return vcvtq_f32_s32(v);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
        return _mm_cvtepi32_ps(V);
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMLoadUInt4(const XMUINT4* pSource) 
    inline vfloat4 VMATH_CALLCONV vLoadCastUint4(const uint32_t* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 V;
        V.v4f[0] = float(pSource[0]);
        V.v4f[1] = float(pSource[1]);
        V.v4f[2] = float(pSource[2]);
        V.v4f[3] = float(pSource[3]);
        return V;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t v = vld1q_u32(pSource);
        return vcvtq_f32_u32(v);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i V = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource));
        // For the values that are higher than 0x7FFFFFFF, a fixup is needed
        // Determine which ones need the fix.
        vfloat4 vMask = _mm_and_ps(_mm_castsi128_ps(V), Internal::kNegativeZero);
        // Force all values positive
        vfloat4 vResult = _mm_xor_ps(_mm_castsi128_ps(V), vMask);
        // Convert to floats
        vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
        // Convert 0x80000000 -> 0xFFFFFFFF
        __m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
        // For only the ones that are too big, add the fixup
        vMask = _mm_and_ps(_mm_castsi128_ps(iMask), Internal::kFixUnsigned);
        vResult = _mm_add_ps(vResult, vMask);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------
    // XMMATRIX XMLoadFloat4x4(const XMFLOAT4X4* pSource) 
    inline vfloat4x4 VMATH_CALLCONV vLoadFloat4x4(const float* pSource)
    {
        assert(pSource);
#if defined(VMATH_NO_INTRINSICS)

        vfloat4x4 M;
        M.r[0].v4f[0] = pSource[0];
        M.r[0].v4f[1] = pSource[1];
        M.r[0].v4f[2] = pSource[2];
        M.r[0].v4f[3] = pSource[3];

        M.r[1].v4f[0] = pSource[4];
        M.r[1].v4f[1] = pSource[5];
        M.r[1].v4f[2] = pSource[6];
        M.r[1].v4f[3] = pSource[7];

        M.r[2].v4f[0] = pSource[8];
        M.r[2].v4f[1] = pSource[9];
        M.r[2].v4f[2] = pSource[10];
        M.r[2].v4f[3] = pSource[11];

        M.r[3].v4f[0] = pSource[12];
        M.r[3].v4f[1] = pSource[13];
        M.r[3].v4f[2] = pSource[14];
        M.r[3].v4f[3] = pSource[15];
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = vld1q_f32(pSource + 0);
        M.r[1] = vld1q_f32(pSource + 4);
        M.r[2] = vld1q_f32(pSource + 8);
        M.r[3] = vld1q_f32(pSource + 12);
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = _mm_loadu_ps(pSource + 0);
        M.r[1] = _mm_loadu_ps(pSource + 4);
        M.r[2] = _mm_loadu_ps(pSource + 8);
        M.r[3] = _mm_loadu_ps(pSource + 12);
        return M;
#endif
    }

    /****************************************************************************
     *
     * Vector and matrix store operations
     *
     ****************************************************************************/
     // void XMStoreInt(uint32_t* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreBinary32(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        *pDestination = V.v4u[0];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vst1q_lane_u32(pDestination, *reinterpret_cast<const uint32x4_t*>(&V), 0);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_store_ss(reinterpret_cast<float*>(pDestination), V);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreFloat(float* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreFloat(float* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        *pDestination = V.v4f[0];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vst1q_lane_f32(pDestination, V, 0);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_store_ss(pDestination, V);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreInt2(uint32_t* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreBinary32x2(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = V.v4u[0];
        pDestination[1] = V.v4u[1];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t VL = vget_low_u32(vreinterpretq_u32_f32(V));
        vst1_u32(pDestination, VL);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(V));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreFloat2(XMFLOAT2* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreFloat2(float* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = V.v4f[0];
        pDestination[1] = V.v4f[1];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        vst1_f32(pDestination, VL);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(V));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreSInt2(XMINT2* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreCastInt2(int32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = int32_t(V.v4f[0]);
        pDestination[1] = int32_t(V.v4f[1]);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t v = vget_low_f32(V);
        int32x2_t iv = vcvt_s32_f32(v);
        vst1_s32(pDestination, iv);
#elif defined(VMATH_SSE_INTRINSICS)
        // In case of positive overflow, detect it
        vfloat4 vOverflow = _mm_cmpgt_ps(V, Internal::kMaxInt);
        // Float to int conversion
        __m128i vResulti = _mm_cvttps_epi32(V);
        // If there was positive overflow, set to 0x7FFFFFFF
        vfloat4 vResult = _mm_and_ps(vOverflow, Internal::kAbsMask);
        vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
        vOverflow = _mm_or_ps(vOverflow, vResult);
        // Write two ints
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(vOverflow));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreUInt2(XMUINT2* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreCastUint2(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = uint32_t(V.v4f[0]);
        pDestination[1] = uint32_t(V.v4f[1]);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t v = vget_low_f32(V);
        uint32x2_t iv = vcvt_u32_f32(v);
        vst1_u32(pDestination, iv);
#elif defined(VMATH_SSE_INTRINSICS)
        // Clamp to >=0
        vfloat4 vResult = _mm_max_ps(V, Internal::kZero);
        // Any numbers that are too big, set to 0xFFFFFFFFU
        vfloat4 vOverflow = _mm_cmpgt_ps(vResult, Internal::kMaxUInt);
        vfloat4 vValue = Internal::kUnsignedFix;
        // Too large for a signed integer?
        vfloat4 vMask = _mm_cmpge_ps(vResult, vValue);
        // Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
        vValue = _mm_and_ps(vValue, vMask);
        // Perform fixup only on numbers too large (Keeps low bit precision)
        vResult = _mm_sub_ps(vResult, vValue);
        __m128i vResulti = _mm_cvttps_epi32(vResult);
        // Convert from signed to unsigned pnly if greater than 0x80000000
        vMask = _mm_and_ps(vMask, Internal::kNegativeZero);
        vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
        // On those that are too large, set to 0xFFFFFFFF
        vResult = _mm_or_ps(vResult, vOverflow);
        // Write two uints
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(vResult));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreInt3(uint32_t* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreBinary32x3(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = V.v4u[0];
        pDestination[1] = V.v4u[1];
        pDestination[2] = V.v4u[2];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t VL = vget_low_u32(vreinterpretq_u32_f32(V));
        vst1_u32(pDestination, VL);
        vst1q_lane_u32(pDestination + 2, *reinterpret_cast<const uint32x4_t*>(&V), 2);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(V));
        __m128 z = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(&pDestination[2]), z);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreFloat3(XMFLOAT3* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreFloat3(float* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = V.v4f[0];
        pDestination[1] = V.v4f[1];
        pDestination[2] = V.v4f[2];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        vst1_f32(pDestination, VL);
        vst1q_lane_f32(pDestination + 2, V, 2);
#elif defined(VMATH_SSE4_INTRINSICS)
        *reinterpret_cast<int32_t*>(pDestination + 0) = _mm_extract_ps(V, 0);
        *reinterpret_cast<int32_t*>(pDestination + 1) = _mm_extract_ps(V, 1);
        *reinterpret_cast<int32_t*>(pDestination + 2) = _mm_extract_ps(V, 2);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(V));
        __m128 z = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(pDestination + 2, z);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreSInt3(XMINT3* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreCastInt3(int32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = int32_t(V.v4f[0]);
        pDestination[1] = int32_t(V.v4f[1]);
        pDestination[2] = int32_t(V.v4f[2]);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t v = vcvtq_s32_f32(V);
        int32x2_t vL = vget_low_s32(v);
        vst1_s32(pDestination, vL);
        vst1q_lane_s32(pDestination + 2, v, 2);
#elif defined(VMATH_SSE_INTRINSICS)
        // In case of positive overflow, detect it
        vfloat4 vOverflow = _mm_cmpgt_ps(V, Internal::kMaxInt);
        // Float to int conversion
        __m128i vResulti = _mm_cvttps_epi32(V);
        // If there was positive overflow, set to 0x7FFFFFFF
        vfloat4 vResult = _mm_and_ps(vOverflow, Internal::kAbsMask);
        vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
        vOverflow = _mm_or_ps(vOverflow, vResult);
        // Write 3 uints
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(vOverflow));
        __m128 z = VMATH_PERMUTE_PS(vOverflow, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(pDestination + 2), z);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreUInt3(XMUINT3* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreCastUint3(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = uint32_t(V.v4f[0]);
        pDestination[1] = uint32_t(V.v4f[1]);
        pDestination[2] = uint32_t(V.v4f[2]);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t v = vcvtq_u32_f32(V);
        uint32x2_t vL = vget_low_u32(v);
        vst1_u32(pDestination, vL);
        vst1q_lane_u32(pDestination + 2, v, 2);
#elif defined(VMATH_SSE_INTRINSICS)
        // Clamp to >=0
        vfloat4 vResult = _mm_max_ps(V, Internal::kZero);
        // Any numbers that are too big, set to 0xFFFFFFFFU
        vfloat4 vOverflow = _mm_cmpgt_ps(vResult, Internal::kMaxUInt);
        vfloat4 vValue = Internal::kUnsignedFix;
        // Too large for a signed integer?
        vfloat4 vMask = _mm_cmpge_ps(vResult, vValue);
        // Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
        vValue = _mm_and_ps(vValue, vMask);
        // Perform fixup only on numbers too large (Keeps low bit precision)
        vResult = _mm_sub_ps(vResult, vValue);
        __m128i vResulti = _mm_cvttps_epi32(vResult);
        // Convert from signed to unsigned pnly if greater than 0x80000000
        vMask = _mm_and_ps(vMask, Internal::kNegativeZero);
        vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
        // On those that are too large, set to 0xFFFFFFFF
        vResult = _mm_or_ps(vResult, vOverflow);
        // Write 3 uints
        _mm_store_sd(reinterpret_cast<double*>(pDestination), _mm_castps_pd(vResult));
        __m128 z = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(2, 2, 2, 2));
        _mm_store_ss(reinterpret_cast<float*>(pDestination + 2), z);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreInt4(uint32_t* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreBinary32x4(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = V.v4u[0];
        pDestination[1] = V.v4u[1];
        pDestination[2] = V.v4u[2];
        pDestination[3] = V.v4u[3];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vst1q_u32(pDestination, vreinterpretq_u32_f32(V));
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(V));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreFloat4(XMFLOAT4* pDestination, FXMVECTOR  V)
    inline void VMATH_CALLCONV vStoreFloat4(float* pDestination, vfloat4  V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = V.v4f[0];
        pDestination[1] = V.v4f[1];
        pDestination[2] = V.v4f[2];
        pDestination[3] = V.v4f[3];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vst1q_f32(pDestination, V);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_storeu_ps(pDestination, V);
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreSInt4(XMINT4* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreCastInt4(int32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = int32_t(V.v4f[0]);
        pDestination[1] = int32_t(V.v4f[1]);
        pDestination[2] = int32_t(V.v4f[2]);
        pDestination[3] = int32_t(V.v4f[3]);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t v = vcvtq_s32_f32(V);
        vst1q_s32(pDestination, v);
#elif defined(VMATH_SSE_INTRINSICS)
        // In case of positive overflow, detect it
        vfloat4 vOverflow = _mm_cmpgt_ps(V, Internal::kMaxInt);
        // Float to int conversion
        __m128i vResulti = _mm_cvttps_epi32(V);
        // If there was positive overflow, set to 0x7FFFFFFF
        vfloat4 vResult = _mm_and_ps(vOverflow, Internal::kAbsMask);
        vOverflow = _mm_andnot_ps(vOverflow, _mm_castsi128_ps(vResulti));
        vOverflow = _mm_or_ps(vOverflow, vResult);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vOverflow));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreUInt4(XMUINT4* pDestination, FXMVECTOR V)
    inline void VMATH_CALLCONV vStoreCastUint4(uint32_t* pDestination, vfloat4 V)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)
        pDestination[0] = uint32_t(V.v4f[0]);
        pDestination[1] = uint32_t(V.v4f[1]);
        pDestination[2] = uint32_t(V.v4f[2]);
        pDestination[3] = uint32_t(V.v4f[3]);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t v = vcvtq_u32_f32(V);
        vst1q_u32(pDestination, v);
#elif defined(VMATH_SSE_INTRINSICS)
        // Clamp to >=0
        vfloat4 vResult = _mm_max_ps(V, Internal::kZero);
        // Any numbers that are too big, set to 0xFFFFFFFFU
        vfloat4 vOverflow = _mm_cmpgt_ps(vResult, Internal::kMaxUInt);
        vfloat4 vValue = Internal::kUnsignedFix;
        // Too large for a signed integer?
        vfloat4 vMask = _mm_cmpge_ps(vResult, vValue);
        // Zero for number's lower than 0x80000000, 32768.0f*65536.0f otherwise
        vValue = _mm_and_ps(vValue, vMask);
        // Perform fixup only on numbers too large (Keeps low bit precision)
        vResult = _mm_sub_ps(vResult, vValue);
        __m128i vResulti = _mm_cvttps_epi32(vResult);
        // Convert from signed to unsigned pnly if greater than 0x80000000
        vMask = _mm_and_ps(vMask, Internal::kNegativeZero);
        vResult = _mm_xor_ps(_mm_castsi128_ps(vResulti), vMask);
        // On those that are too large, set to 0xFFFFFFFF
        vResult = _mm_or_ps(vResult, vOverflow);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination), _mm_castps_si128(vResult));
#endif
    }

    //------------------------------------------------------------------------------
    // void XMStoreFloat4x4(XMFLOAT4X4* pDestination, FXMMATRIX M)
    inline void VMATH_CALLCONV vStoreFloat4x4(float* pDestination, vfloat4x4 M)
    {
        assert(pDestination);
#if defined(VMATH_NO_INTRINSICS)

        pDestination[0] = M.r[0].v4f[0];
        pDestination[1] = M.r[0].v4f[1];
        pDestination[2] = M.r[0].v4f[2];
        pDestination[3] = M.r[0].v4f[3];

        pDestination[4] = M.r[1].v4f[0];
        pDestination[5] = M.r[1].v4f[1];
        pDestination[6] = M.r[1].v4f[2];
        pDestination[7] = M.r[1].v4f[3];

        pDestination[8] = M.r[2].v4f[0];
        pDestination[9] = M.r[2].v4f[1];
        pDestination[10] = M.r[2].v4f[2];
        pDestination[11] = M.r[2].v4f[3];

        pDestination[12] = M.r[3].v4f[0];
        pDestination[13] = M.r[3].v4f[1];
        pDestination[14] = M.r[3].v4f[2];
        pDestination[15] = M.r[3].v4f[3];

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vst1q_f32(pDestination + 0, M.r[0]);
        vst1q_f32(pDestination + 4, M.r[1]);
        vst1q_f32(pDestination + 8, M.r[2]);
        vst1q_f32(pDestination + 12, M.r[3]);
#elif defined(VMATH_SSE_INTRINSICS)
        _mm_storeu_ps(pDestination + 0, M.r[0]);
        _mm_storeu_ps(pDestination + 4, M.r[1]);
        _mm_storeu_ps(pDestination + 8, M.r[2]);
        _mm_storeu_ps(pDestination + 12, M.r[3]);
#endif
    }

#if defined(VMATH_NO_INTRINSICS)
#define VMATH_ISNAN(x)  isnan(x)
#define VMATH_ISINF(x)  isinf(x)
#endif

#if defined(VMATH_SSE_INTRINSICS)

#define VMATH_3UNPACK3INTO4(l1, l2, l3) \
    vfloat4 V3 = _mm_shuffle_ps(l2, l3, _MM_SHUFFLE(0, 0, 3, 2));\
    vfloat4 V2 = _mm_shuffle_ps(l2, l1, _MM_SHUFFLE(3, 3, 1, 0));\
    V2 = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 0, 2));\
    vfloat4 V4 = _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(L3), 32 / 8))

#define VMATH_3PACK4INTO3(v2x) \
    v2x = _mm_shuffle_ps(V2, V3, _MM_SHUFFLE(1, 0, 2, 1));\
    V2 = _mm_shuffle_ps(V2, V1, _MM_SHUFFLE(2, 2, 0, 0));\
    V1 = _mm_shuffle_ps(V1, V2, _MM_SHUFFLE(0, 2, 1, 0));\
    V3 = _mm_shuffle_ps(V3, V4, _MM_SHUFFLE(0, 0, 2, 2));\
    V3 = _mm_shuffle_ps(V3, V4, _MM_SHUFFLE(2, 1, 2, 0))

#endif


    /****************************************************************************
    *
    * General Vector
    *
    ****************************************************************************/

    //------------------------------------------------------------------------------
    // Return a vector with all elements equaling zero
    // XMVECTOR XMVectorZero()
    inline vfloat4 VMATH_CALLCONV v4Zero()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult = { { { 0.0f, 0.0f, 0.0f, 0.0f } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vdupq_n_f32(0);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_setzero_ps();
#endif
    }

    //------------------------------------------------------------------------------
    // Initialize a vector with four floating point values
    // XMVECTOR XMVectorSet(float x, float y, float z, float w)
    inline vfloat4 VMATH_CALLCONV v4Set(float x, float y, float z, float w)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult = { { { x, y, z, w } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t V0 = vcreate_f32(
            uint64_t(*reinterpret_cast<const uint32_t*>(&x))
            | (uint64_t(*reinterpret_cast<const uint32_t*>(&y)) << 32));
        float32x2_t V1 = vcreate_f32(
            uint64_t(*reinterpret_cast<const uint32_t*>(&z))
            | (uint64_t(*reinterpret_cast<const uint32_t*>(&w)) << 32));
        return vcombine_f32(V0, V1);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_set_ps(w, z, y, x);
#endif
    }

    //------------------------------------------------------------------------------
    // Initialize a vector with four integer values
    // XMVECTOR XMVectorSetInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
    inline vfloat4 VMATH_CALLCONV v4SetBinary(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult = { { { x, y, z, w } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t V0 = vcreate_u32(uint64_t(x) | (uint64_t(y) << 32));
        uint32x2_t V1 = vcreate_u32(uint64_t(z) | (uint64_t(w) << 32));
        return vreinterpretq_f32_u32(vcombine_u32(V0, V1));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set_epi32(int32_t(w), int32_t(z), int32_t(y), int32_t(x)));
#endif
    }

    //------------------------------------------------------------------------------
    // Initialize a vector with a replicated floating point value
    // XMVECTOR XMVectorReplicate(float Value)
    inline vfloat4 VMATH_CALLCONV v4Spread(float Value)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult;
        vResult.f[0] =
            vResult.f[1] =
            vResult.f[2] =
            vResult.f[3] = Value;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vdupq_n_f32(Value);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_set_ps1(Value);
#endif
    }

    //------------------------------------------------------------------------------
    // Initialize a vector with a replicated integer value
    // XMVECTOR XMVectorReplicateInt(uint32_t Value)
    inline vfloat4 VMATH_CALLCONV v4SpreadBinary(uint32_t Value)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult;
        vResult.u[0] =
            vResult.u[1] =
            vResult.u[2] =
            vResult.u[3] = Value;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vdupq_n_u32(Value));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set1_epi32(int32_t(Value)));
#endif
    }

    //------------------------------------------------------------------------------
// Initialize a vector with all bits set (true mask)
// XMVECTOR XMVectorTrueInt()
    inline vfloat4 VMATH_CALLCONV v4AllBitsOne()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult = { { { 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_s32(vdupq_n_s32(-1));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set1_epi32(-1));
#endif
    }

    //------------------------------------------------------------------------------
    // Return a vector of 1.0f,1.0f,1.0f,1.0f
    // XMVECTOR XMVectorSplatOne()
    inline vfloat4 VMATH_CALLCONV v4One()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult;
        vResult.f[0] =
            vResult.f[1] =
            vResult.f[2] =
            vResult.f[3] = 1.0f;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vdupq_n_f32(1.0f);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_set_ps1(1.0f);
#endif
    }

    //------------------------------------------------------------------------------
    // Return a vector of INF,INF,INF,INF
    // XMVECTOR XMVectorSplatInfinity()
    inline vfloat4 VMATH_CALLCONV v4Infinity()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult;
        vResult.u[0] =
            vResult.u[1] =
            vResult.u[2] =
            vResult.u[3] = 0x7F800000;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vdupq_n_u32(0x7F800000));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set1_epi32(0x7F800000));
#endif
    }

    //------------------------------------------------------------------------------
    // Return a vector of Q_NAN,Q_NAN,Q_NAN,Q_NAN
    // XMVECTOR XMVectorSplatQNaN()
    inline vfloat4 VMATH_CALLCONV v4QNaN()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult;
        vResult.u[0] =
            vResult.u[1] =
            vResult.u[2] =
            vResult.u[3] = 0x7FC00000;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vdupq_n_u32(0x7FC00000));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set1_epi32(0x7FC00000));
#endif
    }

    //------------------------------------------------------------------------------
    // Return a vector of 1.192092896e-7f,1.192092896e-7f,1.192092896e-7f,1.192092896e-7f
    // XMVECTOR XMVectorSplatEpsilon()
    inline vfloat4 VMATH_CALLCONV v4Epsilon()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult;
        vResult.u[0] =
            vResult.u[1] =
            vResult.u[2] =
            vResult.u[3] = 0x34000000;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vdupq_n_u32(0x34000000));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set1_epi32(0x34000000));
#endif
    }

    //------------------------------------------------------------------------------
    // Return a vector of -0.0f (0x80000000),-0.0f,-0.0f,-0.0f
    // XMVECTOR XMVectorSplatSignMask()
    inline vfloat4 VMATH_CALLCONV v4SignMask()
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4u32 vResult;
        vResult.u[0] =
            vResult.u[1] =
            vResult.u[2] =
            vResult.u[3] = 0x80000000U;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vdupq_n_u32(0x80000000U));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_castsi128_ps(_mm_set1_epi32(int32_t(0x80000000)));
#endif
    }

    //------------------------------------------------------------------------------
    // Return a floating point value via an index. This is not a recommended
    // function to use due to performance loss.
    // float XMVectorGetByIndex(FXMVECTOR V, size_t i)
    inline float VMATH_CALLCONV v4GetByIndex(vfloat4 V, size_t i)
    {
        assert(i < 4);
#if defined(VMATH_NO_INTRINSICS)
        return V.v4f[i];
#else
        Internal::vfloat4f32 U;
        U.v = V;
        return U.f[i];
#endif
    }

    //------------------------------------------------------------------------------
    // Return the X component in an FPU register.
    // float XMVectorGetX(FXMVECTOR V)
    inline float VMATH_CALLCONV v4GetX(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return V.v4f[0];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vgetq_lane_f32(V, 0);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cvtss_f32(V);
#endif
    }

    // Return the Y component in an FPU register.
    // float XMVectorGetY(FXMVECTOR V)
    inline float VMATH_CALLCONV v4GetY(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return V.v4f[1];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vgetq_lane_f32(V, 1);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
        return _mm_cvtss_f32(vTemp);
#endif
    }

    // Return the Z component in an FPU register.
    // float XMVectorGetZ(FXMVECTOR V)
    inline float VMATH_CALLCONV v4GetZ(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return V.v4f[2];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vgetq_lane_f32(V, 2);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
        return _mm_cvtss_f32(vTemp);
#endif
    }

    // Return the W component in an FPU register.
    // float XMVectorGetW(FXMVECTOR V)
    inline float VMATH_CALLCONV v4GetW(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return V.v4f[3];
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vgetq_lane_f32(V, 3);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
        return _mm_cvtss_f32(vTemp);
#endif
    }

    // Set a single indexed floating point component
    // XMVECTOR XMVectorSetByIndex(FXMVECTOR V, float f, size_t i)
    inline vfloat4 VMATH_CALLCONV v4SetByIndex(vfloat4 V, float f, size_t i)
    {
        assert(i < 4);
        Internal::vfloat4f32 U;
        U.v = V;
        U.f[i] = f;
        return U.v;
    }

    //------------------------------------------------------------------------------

    // Sets the X component of a vector to a passed floating point value
    // XMVECTOR XMVectorSetX(FXMVECTOR V, float x)
    inline vfloat4 VMATH_CALLCONV v4SetX(vfloat4 V, float x)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 U = { { {
                x,
                V.v4f[1],
                V.v4f[2],
                V.v4f[3]
            } } };
        return U.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vsetq_lane_f32(x, V, 0);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = _mm_set_ss(x);
        vResult = _mm_move_ss(V, vResult);
        return vResult;
#endif
    }

    // Sets the Y component of a vector to a passed floating point value
    // XMVECTOR XMVectorSetY(FXMVECTOR V, float y)
    inline vfloat4 VMATH_CALLCONV v4SetY(vfloat4 V, float y)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 U = { { {
                V.v4f[0],
                y,
                V.v4f[2],
                V.v4f[3]
            } } };
        return U.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vsetq_lane_f32(y, V, 1);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vResult = _mm_set_ss(y);
        vResult = _mm_insert_ps(V, vResult, 0x10);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Swap y and x
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
        // Convert input to vector
        vfloat4 vTemp = _mm_set_ss(y);
        // Replace the x component
        vResult = _mm_move_ss(vResult, vTemp);
        // Swap y and x again
        vResult = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 2, 0, 1));
        return vResult;
#endif
    }
    // Sets the Z component of a vector to a passed floating point value
    // XMVECTOR XMVectorSetZ(FXMVECTOR V, float z)
    inline vfloat4 VMATH_CALLCONV v4SetZ(vfloat4 V, float z)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 U = { { {
                V.v4f[0],
                V.v4f[1],
                z,
                V.v4f[3]
            } } };
        return U.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vsetq_lane_f32(z, V, 2);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vResult = _mm_set_ss(z);
        vResult = _mm_insert_ps(V, vResult, 0x20);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Swap z and x
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 0, 1, 2));
        // Convert input to vector
        vfloat4 vTemp = _mm_set_ss(z);
        // Replace the x component
        vResult = _mm_move_ss(vResult, vTemp);
        // Swap z and x again
        vResult = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 0, 1, 2));
        return vResult;
#endif
    }

    // Sets the W component of a vector to a passed floating point value
    // XMVECTOR XMVectorSetW(FXMVECTOR V, float w)
    inline vfloat4 VMATH_CALLCONV v4SetW(vfloat4 V, float w)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 U = { { {
                V.v4f[0],
                V.v4f[1],
                V.v4f[2],
                w
            } } };
        return U.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vsetq_lane_f32(w, V, 3);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vResult = _mm_set_ss(w);
        vResult = _mm_insert_ps(V, vResult, 0x30);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Swap w and x
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 2, 1, 3));
        // Convert input to vector
        vfloat4 vTemp = _mm_set_ss(w);
        // Replace the x component
        vResult = _mm_move_ss(vResult, vTemp);
        // Swap w and x again
        vResult = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 2, 1, 3));
        return vResult;
#endif
    }

    // XMVECTOR XMVectorSwizzle(FXMVECTOR V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3)
    inline vfloat4 VMATH_CALLCONV v4Swizzle(vfloat4 V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3)
    {
        assert((E0 < 4) && (E1 < 4) && (E2 < 4) && (E3 < 4));
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                V.v4f[E0],
                V.v4f[E1],
                V.v4f[E2],
                V.v4f[E3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const uint32_t ControlElement[4] =
        {
            0x03020100, // Swizzle X
            0x07060504, // Swizzle Y
            0x0B0A0908, // Swizzle Z
            0x0F0E0D0C, // Swizzle W
        };

        uint8x8x2_t tbl;
        tbl.val[0] = vreinterpret_u8_f32(vget_low_f32(V));
        tbl.val[1] = vreinterpret_u8_f32(vget_high_f32(V));

        uint32x2_t idx = vcreate_u32(uint64_t(ControlElement[E0]) | (uint64_t(ControlElement[E1]) << 32));
        const uint8x8_t rL = vtbl2_u8(tbl, vreinterpret_u8_u32(idx));

        idx = vcreate_u32(uint64_t(ControlElement[E2]) | (uint64_t(ControlElement[E3]) << 32));
        const uint8x8_t rH = vtbl2_u8(tbl, vreinterpret_u8_u32(idx));

        return vcombine_f32(vreinterpret_f32_u8(rL), vreinterpret_f32_u8(rH));
#elif defined(VMATH_AVX_INTRINSICS)
        uint32_t elem[4] = { E0, E1, E2, E3 };
        __m128i vControl = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&elem[0]));
        return _mm_permutevar_ps(V, vControl);
#else
        auto aPtr = reinterpret_cast<const uint32_t*>(&V);

        vfloat4 Result;
        auto pWork = reinterpret_cast<uint32_t*>(&Result);

        pWork[0] = aPtr[E0];
        pWork[1] = aPtr[E1];
        pWork[2] = aPtr[E2];
        pWork[3] = aPtr[E3];

        return Result;
#endif
    }

    //------------------------------------------------------------------------------
    // XMVECTOR XMVectorPermute(FXMVECTOR V1, FXMVECTOR V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW)
    inline vfloat4 VMATH_CALLCONV v4Shuffle(vfloat4 V1, vfloat4 V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW)
    {
        assert(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);

#if defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        static const uint32_t ControlElement[8] =
        {
            0x03020100, // Permute 0X
            0x07060504, // Permute 0Y
            0x0B0A0908, // Permute 0Z
            0x0F0E0D0C, // Permute 0W
            0x13121110, // Permute 1X
            0x17161514, // Permute 1Y
            0x1B1A1918, // Permute 1Z
            0x1F1E1D1C, // Permute 1W
        };

        uint8x8x4_t tbl;
        tbl.val[0] = vreinterpret_u8_f32(vget_low_f32(V1));
        tbl.val[1] = vreinterpret_u8_f32(vget_high_f32(V1));
        tbl.val[2] = vreinterpret_u8_f32(vget_low_f32(V2));
        tbl.val[3] = vreinterpret_u8_f32(vget_high_f32(V2));

        uint32x2_t idx = vcreate_u32(uint64_t(ControlElement[PermuteX]) | (uint64_t(ControlElement[PermuteY]) << 32));
        const uint8x8_t rL = vtbl4_u8(tbl, vreinterpret_u8_u32(idx));

        idx = vcreate_u32(uint64_t(ControlElement[PermuteZ]) | (uint64_t(ControlElement[PermuteW]) << 32));
        const uint8x8_t rH = vtbl4_u8(tbl, vreinterpret_u8_u32(idx));

        return vcombine_f32(vreinterpret_f32_u8(rL), vreinterpret_f32_u8(rH));
#elif defined(VMATH_AVX_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        static const Internal::vfloat4u32 three = { { { 3, 3, 3, 3 } } };

        VMATH_ALIGNED_DATA(16) uint32_t elem[4] = { PermuteX, PermuteY, PermuteZ, PermuteW };
        __m128i vControl = _mm_load_si128(reinterpret_cast<const __m128i*>(&elem[0]));

        __m128i vSelect = _mm_cmpgt_epi32(vControl, three);
        vControl = _mm_castps_si128(_mm_and_ps(_mm_castsi128_ps(vControl), three));

        __m128 shuffled1 = _mm_permutevar_ps(V1, vControl);
        __m128 shuffled2 = _mm_permutevar_ps(V2, vControl);

        __m128 masked1 = _mm_andnot_ps(_mm_castsi128_ps(vSelect), shuffled1);
        __m128 masked2 = _mm_and_ps(_mm_castsi128_ps(vSelect), shuffled2);

        return _mm_or_ps(masked1, masked2);
#else

        const uint32_t* aPtr[2];
        aPtr[0] = reinterpret_cast<const uint32_t*>(&V1);
        aPtr[1] = reinterpret_cast<const uint32_t*>(&V2);

        vfloat4 Result;
        auto pWork = reinterpret_cast<uint32_t*>(&Result);

        const uint32_t i0 = PermuteX & 3;
        const uint32_t vi0 = PermuteX >> 2;
        pWork[0] = aPtr[vi0][i0];

        const uint32_t i1 = PermuteY & 3;
        const uint32_t vi1 = PermuteY >> 2;
        pWork[1] = aPtr[vi1][i1];

        const uint32_t i2 = PermuteZ & 3;
        const uint32_t vi2 = PermuteZ >> 2;
        pWork[2] = aPtr[vi2][i2];

        const uint32_t i3 = PermuteW & 3;
        const uint32_t vi3 = PermuteW >> 2;
        pWork[3] = aPtr[vi3][i3];

        return Result;
#endif
    }

    //------------------------------------------------------------------------------
    // Define a control vector to be used in v4Select
    // operations.  The four integers specified in v4SelectControl
    // serve as indices to select between components in two vectors.
    // The first index controls selection for the first component of
    // the vectors involved in a select operation, the second index
    // controls selection for the second component etc.  A value of
    // zero for an index causes the corresponding component from the first
    // vector to be selected whereas a one causes the component from the
    // second vector to be selected instead.

    // XMVECTOR XMVectorSelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3)
    inline vfloat4 VMATH_CALLCONV v4SelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3)
    {
#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        // x=Index0,y=Index1,z=Index2,w=Index3
        __m128i vTemp = _mm_set_epi32(int32_t(VectorIndex3), int32_t(VectorIndex2), int32_t(VectorIndex1), int32_t(VectorIndex0));
        // Any non-zero entries become 0xFFFFFFFF else 0
        vTemp = _mm_cmpgt_epi32(vTemp, Internal::kZero);
        return _mm_castsi128_ps(vTemp);
#elif defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        int32x2_t V0 = vcreate_s32(uint64_t(VectorIndex0) | (uint64_t(VectorIndex1) << 32));
        int32x2_t V1 = vcreate_s32(uint64_t(VectorIndex2) | (uint64_t(VectorIndex3) << 32));
        int32x4_t vTemp = vcombine_s32(V0, V1);
        // Any non-zero entries become 0xFFFFFFFF else 0
        return vreinterpretq_f32_u32(vcgtq_s32(vTemp, Internal::kZero));
#else
        vfloat4    ControlVector;
        const uint32_t  ControlElement[] =
        {
            0x00000000,
            0xFFFFFFFF
        };

        assert(VectorIndex0 < 2);
        assert(VectorIndex1 < 2);
        assert(VectorIndex2 < 2);
        assert(VectorIndex3 < 2);

        ControlVector.v4u[0] = ControlElement[VectorIndex0];
        ControlVector.v4u[1] = ControlElement[VectorIndex1];
        ControlVector.v4u[2] = ControlElement[VectorIndex2];
        ControlVector.v4u[3] = ControlElement[VectorIndex3];

        return ControlVector;

#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSelect(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Control)
    inline vfloat4 VMATH_CALLCONV v4Select(vfloat4 V1, vfloat4 V2, vfloat4 Control)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Result = { { {
                (V1.v4u[0] & ~Control.v4u[0]) | (V2.v4u[0] & Control.v4u[0]),
                (V1.v4u[1] & ~Control.v4u[1]) | (V2.v4u[1] & Control.v4u[1]),
                (V1.v4u[2] & ~Control.v4u[2]) | (V2.v4u[2] & Control.v4u[2]),
                (V1.v4u[3] & ~Control.v4u[3]) | (V2.v4u[3] & Control.v4u[3]),
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vbslq_f32(vreinterpretq_u32_f32(Control), V2, V1);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp1 = _mm_andnot_ps(Control, V1);
        vfloat4 vTemp2 = _mm_and_ps(V2, Control);
        return _mm_or_ps(vTemp1, vTemp2);
#endif
    }

    // XMVECTOR XMVectorShiftLeft(FXMVECTOR V1, FXMVECTOR V2, uint32_t Elements)
    inline vfloat4 VMATH_CALLCONV v4PushLeft(vfloat4 V1, vfloat4 V2, uint32_t Elements)
    {
        assert(Elements < 4);
        return v4Shuffle(V1, V2, Elements, ((Elements)+1), ((Elements)+2), ((Elements)+3));
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorRotateLeft(FXMVECTOR V, uint32_t Elements)
    inline vfloat4 VMATH_CALLCONV v4RotateLeft(vfloat4 V, uint32_t Elements)
    {
        assert(Elements < 4);
        return v4Swizzle(V, Elements & 3, (Elements + 1) & 3, (Elements + 2) & 3, (Elements + 3) & 3);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorRotateRight(FXMVECTOR V, uint32_t Elements)
    inline vfloat4 VMATH_CALLCONV v4RotateRight(vfloat4 V, uint32_t Elements)
    {
        assert(Elements < 4);
        return v4Swizzle(V, (4 - (Elements)) & 3, (5 - (Elements)) & 3, (6 - (Elements)) & 3, (7 - (Elements)) & 3);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorInsert(
    inline vfloat4 VMATH_CALLCONV v4Insert(
        vfloat4 VD, vfloat4 VS,
        uint32_t VSLeftRotateElements,
        uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3)
    {
        vfloat4 Control = v4SelectControl(Select0 & 1, Select1 & 1, Select2 & 1, Select3 & 1);
        return v4Select(VD, v4RotateLeft(VS, VSLeftRotateElements), Control);
    }

    //------------------------------------------------------------------------------
    // Comparison operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Equal(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4f[0] == V2.v4f[0]) ? 0xFFFFFFFF : 0,
                (V1.v4f[1] == V2.v4f[1]) ? 0xFFFFFFFF : 0,
                (V1.v4f[2] == V2.v4f[2]) ? 0xFFFFFFFF : 0,
                (V1.v4f[3] == V2.v4f[3]) ? 0xFFFFFFFF : 0,
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vceqq_f32(V1, V2));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cmpeq_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------
    // Treat the components of the vectors as unsigned integers and
    // compare individual bits between the two.  This is useful for
    // comparing control vectors and result vectors returned from
    // other comparison operations.

    // XMVECTOR XMVectorEqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4BinaryEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4u[0] == V2.v4u[0]) ? 0xFFFFFFFF : 0,
                (V1.v4u[1] == V2.v4u[1]) ? 0xFFFFFFFF : 0,
                (V1.v4u[2] == V2.v4u[2]) ? 0xFFFFFFFF : 0,
                (V1.v4u[3] == V2.v4u[3]) ? 0xFFFFFFFF : 0,
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vceqq_s32(vreinterpretq_s32_f32(V1), vreinterpretq_s32_f32(V2)));
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return _mm_castsi128_ps(V);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorNearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon)
    inline vfloat4 VMATH_CALLCONV v4NearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon)
    {
#if defined(VMATH_NO_INTRINSICS)

        float fDeltax = V1.v4f[0] - V2.v4f[0];
        float fDeltay = V1.v4f[1] - V2.v4f[1];
        float fDeltaz = V1.v4f[2] - V2.v4f[2];
        float fDeltaw = V1.v4f[3] - V2.v4f[3];

        fDeltax = fabsf(fDeltax);
        fDeltay = fabsf(fDeltay);
        fDeltaz = fabsf(fDeltaz);
        fDeltaw = fabsf(fDeltaw);

        Internal::vfloat4u32 Control = { { {
                (fDeltax <= Epsilon.v4f[0]) ? 0xFFFFFFFFU : 0,
                (fDeltay <= Epsilon.v4f[1]) ? 0xFFFFFFFFU : 0,
                (fDeltaz <= Epsilon.v4f[2]) ? 0xFFFFFFFFU : 0,
                (fDeltaw <= Epsilon.v4f[3]) ? 0xFFFFFFFFU : 0,
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t vDelta = vsubq_f32(V1, V2);
#if defined(_MSC_VER) && !defined(__clang__)
        return vacleq_f32(vDelta, Epsilon);
#else
        return vreinterpretq_f32_u32(vcleq_f32(vabsq_f32(vDelta), Epsilon));
#endif
#elif defined(VMATH_SSE_INTRINSICS)
        // Get the difference
        vfloat4 vDelta = _mm_sub_ps(V1, V2);
        // Get the absolute value of the difference
        vfloat4 vTemp = _mm_setzero_ps();
        vTemp = _mm_sub_ps(vTemp, vDelta);
        vTemp = _mm_max_ps(vTemp, vDelta);
        vTemp = _mm_cmple_ps(vTemp, Epsilon);
        return vTemp;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorNotEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4NotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4f[0] != V2.v4f[0]) ? 0xFFFFFFFF : 0,
                (V1.v4f[1] != V2.v4f[1]) ? 0xFFFFFFFF : 0,
                (V1.v4f[2] != V2.v4f[2]) ? 0xFFFFFFFF : 0,
                (V1.v4f[3] != V2.v4f[3]) ? 0xFFFFFFFF : 0,
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32(V1, V2)));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cmpneq_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorNotEqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4BinaryNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4u[0] != V2.v4u[0]) ? 0xFFFFFFFFU : 0,
                (V1.v4u[1] != V2.v4u[1]) ? 0xFFFFFFFFU : 0,
                (V1.v4u[2] != V2.v4u[2]) ? 0xFFFFFFFFU : 0,
                (V1.v4u[3] != V2.v4u[3]) ? 0xFFFFFFFFU : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vmvnq_u32(
            vceqq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2))));
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i V = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return _mm_xor_ps(_mm_castsi128_ps(V), Internal::kNegOneMask);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorGreater(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Greater(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4f[0] > V2.v4f[0]) ? 0xFFFFFFFF : 0,
                (V1.v4f[1] > V2.v4f[1]) ? 0xFFFFFFFF : 0,
                (V1.v4f[2] > V2.v4f[2]) ? 0xFFFFFFFF : 0,
                (V1.v4f[3] > V2.v4f[3]) ? 0xFFFFFFFF : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vcgtq_f32(V1, V2));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cmpgt_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorGreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4GreaterEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4f[0] >= V2.v4f[0]) ? 0xFFFFFFFF : 0,
                (V1.v4f[1] >= V2.v4f[1]) ? 0xFFFFFFFF : 0,
                (V1.v4f[2] >= V2.v4f[2]) ? 0xFFFFFFFF : 0,
                (V1.v4f[3] >= V2.v4f[3]) ? 0xFFFFFFFF : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vcgeq_f32(V1, V2));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cmpge_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorLess(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Less(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4f[0] < V2.v4f[0]) ? 0xFFFFFFFF : 0,
                (V1.v4f[1] < V2.v4f[1]) ? 0xFFFFFFFF : 0,
                (V1.v4f[2] < V2.v4f[2]) ? 0xFFFFFFFF : 0,
                (V1.v4f[3] < V2.v4f[3]) ? 0xFFFFFFFF : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vcltq_f32(V1, V2));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cmplt_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorLessOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4LessEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V1.v4f[0] <= V2.v4f[0]) ? 0xFFFFFFFF : 0,
                (V1.v4f[1] <= V2.v4f[1]) ? 0xFFFFFFFF : 0,
                (V1.v4f[2] <= V2.v4f[2]) ? 0xFFFFFFFF : 0,
                (V1.v4f[3] <= V2.v4f[3]) ? 0xFFFFFFFF : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vcleq_f32(V1, V2));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_cmple_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorInBounds(FXMVECTOR V, FXMVECTOR Bounds)
    inline vfloat4 VMATH_CALLCONV v4InBoundsAbs(vfloat4 V, vfloat4 Bounds)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                (V.v4f[0] <= Bounds.v4f[0] && V.v4f[0] >= -Bounds.v4f[0]) ? 0xFFFFFFFF : 0,
                (V.v4f[1] <= Bounds.v4f[1] && V.v4f[1] >= -Bounds.v4f[1]) ? 0xFFFFFFFF : 0,
                (V.v4f[2] <= Bounds.v4f[2] && V.v4f[2] >= -Bounds.v4f[2]) ? 0xFFFFFFFF : 0,
                (V.v4f[3] <= Bounds.v4f[3] && V.v4f[3] >= -Bounds.v4f[3]) ? 0xFFFFFFFF : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Test if less than or equal
        uint32x4_t vTemp1 = vcleq_f32(V, Bounds);
        // Negate the bounds
        uint32x4_t vTemp2 = vreinterpretq_u32_f32(vnegq_f32(Bounds));
        // Test if greater or equal (Reversed)
        vTemp2 = vcleq_f32(vreinterpretq_f32_u32(vTemp2), V);
        // Blend answers
        vTemp1 = vandq_u32(vTemp1, vTemp2);
        return vreinterpretq_f32_u32(vTemp1);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test if less than or equal
        vfloat4 vTemp1 = _mm_cmple_ps(V, Bounds);
        // Negate the bounds
        vfloat4 vTemp2 = _mm_mul_ps(Bounds, Internal::kNegativeOne);
        // Test if greater or equal (Reversed)
        vTemp2 = _mm_cmple_ps(vTemp2, V);
        // Blend answers
        vTemp1 = _mm_and_ps(vTemp1, vTemp2);
        return vTemp1;
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

    // XMVECTOR XMVectorIsNaN(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4IsNaN(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                VMATH_ISNAN(V.v4f[0]) ? 0xFFFFFFFFU : 0,
                VMATH_ISNAN(V.v4f[1]) ? 0xFFFFFFFFU : 0,
                VMATH_ISNAN(V.v4f[2]) ? 0xFFFFFFFFU : 0,
                VMATH_ISNAN(V.v4f[3]) ? 0xFFFFFFFFU : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Test against itself. NaN is always not equal
        uint32x4_t vTempNan = vceqq_f32(V, V);
        // Flip results
        return vreinterpretq_f32_u32(vmvnq_u32(vTempNan));
#elif defined(VMATH_SSE_INTRINSICS)
        // Test against itself. NaN is always not equal
        return _mm_cmpneq_ps(V, V);
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorIsInfinite(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4IsInfinite(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Control = { { {
                VMATH_ISINF(V.v4f[0]) ? 0xFFFFFFFFU : 0,
                VMATH_ISINF(V.v4f[1]) ? 0xFFFFFFFFU : 0,
                VMATH_ISINF(V.v4f[2]) ? 0xFFFFFFFFU : 0,
                VMATH_ISINF(V.v4f[3]) ? 0xFFFFFFFFU : 0
            } } };
        return Control.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Mask off the sign bit
        uint32x4_t vTemp = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        // Compare to infinity
        vTemp = vceqq_f32(vreinterpretq_f32_u32(vTemp), Internal::kInfinity);
        // If any are infinity, the signs are true.
        return vreinterpretq_f32_u32(vTemp);
#elif defined(VMATH_SSE_INTRINSICS)
        // Mask off the sign bit
        __m128 vTemp = _mm_and_ps(V, Internal::kAbsMask);
        // Compare to infinity
        vTemp = _mm_cmpeq_ps(vTemp, Internal::kInfinity);
        // If any are infinity, the signs are true.
        return vTemp;
#endif
    }

    //------------------------------------------------------------------------------
    // Rounding and clamping operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorMin(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Min(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                (V1.v4f[0] < V2.v4f[0]) ? V1.v4f[0] : V2.v4f[0],
                (V1.v4f[1] < V2.v4f[1]) ? V1.v4f[1] : V2.v4f[1],
                (V1.v4f[2] < V2.v4f[2]) ? V1.v4f[2] : V2.v4f[2],
                (V1.v4f[3] < V2.v4f[3]) ? V1.v4f[3] : V2.v4f[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vminq_f32(V1, V2);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_min_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorMax(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Max(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                (V1.v4f[0] > V2.v4f[0]) ? V1.v4f[0] : V2.v4f[0],
                (V1.v4f[1] > V2.v4f[1]) ? V1.v4f[1] : V2.v4f[1],
                (V1.v4f[2] > V2.v4f[2]) ? V1.v4f[2] : V2.v4f[2],
                (V1.v4f[3] > V2.v4f[3]) ? V1.v4f[3] : V2.v4f[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vmaxq_f32(V1, V2);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_max_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    namespace Internal
    {
        // Round to nearest (even) a.k.a. banker's rounding
        // float round_to_nearest(float x)
        inline float round_to_nearest(float x)
        {
            float i = floorf(x);
            x -= i;
            if (x < 0.5f)
                return i;
            if (x > 0.5f)
                return i + 1.f;

            float int_part;
            (void)modff(i / 2.f, &int_part);
            if ((2.f * int_part) == i)
            {
                return i;
            }

            return i + 1.f;
        }
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

    // XMVECTOR XMVectorRound(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Round(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                Internal::round_to_nearest(V.v4f[0]),
                Internal::round_to_nearest(V.v4f[1]),
                Internal::round_to_nearest(V.v4f[2]),
                Internal::round_to_nearest(V.v4f[3])
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vrndnq_f32(V);
#else
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(V), Internal::kNegativeZero);
        float32x4_t sMagic = vreinterpretq_f32_u32(vorrq_u32(Internal::kNoFraction, sign));
        float32x4_t R1 = vaddq_f32(V, sMagic);
        R1 = vsubq_f32(R1, sMagic);
        float32x4_t R2 = vabsq_f32(V);
        uint32x4_t mask = vcleq_f32(R2, Internal::kNoFraction);
        return vbslq_f32(mask, R1, V);
#endif
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_round_ps(V, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 sign = _mm_and_ps(V, Internal::kNegativeZero);
        __m128 sMagic = _mm_or_ps(Internal::kNoFraction, sign);
        __m128 R1 = _mm_add_ps(V, sMagic);
        R1 = _mm_sub_ps(R1, sMagic);
        __m128 R2 = _mm_and_ps(V, Internal::kAbsMask);
        __m128 mask = _mm_cmple_ps(R2, Internal::kNoFraction);
        R2 = _mm_andnot_ps(mask, V);
        R1 = _mm_and_ps(R1, mask);
        vfloat4 vResult = _mm_xor_ps(R1, R2);
        return vResult;
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorTruncate(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Truncate(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        vfloat4 Result;
        uint32_t     i;

        // Avoid C4701
        Result.v4f[0] = 0.0f;

        for (i = 0; i < 4; i++)
        {
            if (VMATH_ISNAN(V.v4f[i]))
            {
                Result.v4u[i] = 0x7FC00000;
            }
            else if (fabsf(V.v4f[i]) < 8388608.0f)
            {
                Result.v4f[i] = float(int32_t(V.v4f[i]));
            }
            else
            {
                Result.v4f[i] = V.v4f[i];
            }
        }
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vrndq_f32(V);
#else
        float32x4_t vTest = vabsq_f32(V);
        vTest = vreinterpretq_f32_u32(vcltq_f32(vTest, Internal::kNoFraction));

        int32x4_t vInt = vcvtq_s32_f32(V);
        float32x4_t vResult = vcvtq_f32_s32(vInt);

        // All numbers less than 8388608 will use the round to int
        // All others, use the ORIGINAL value
        return vbslq_f32(vreinterpretq_u32_f32(vTest), vResult, V);
#endif
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_round_ps(V, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
#elif defined(VMATH_SSE_INTRINSICS)
        // To handle NAN, INF and numbers greater than 8388608, use masking
        // Get the abs value
        __m128i vTest = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        // Test for greater than 8388608 (All floats with NO fractionals, NAN and INF
        vTest = _mm_cmplt_epi32(vTest, Internal::kNoFraction);
        // Convert to int and back to float for rounding with truncation
        __m128i vInt = _mm_cvttps_epi32(V);
        // Convert back to floats
        vfloat4 vResult = _mm_cvtepi32_ps(vInt);
        // All numbers less than 8388608 will use the round to int
        vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
        // All others, use the ORIGINAL value
        vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
        vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorFloor(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Floor(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                floorf(V.v4f[0]),
                floorf(V.v4f[1]),
                floorf(V.v4f[2]),
                floorf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vrndmq_f32(V);
#else
        float32x4_t vTest = vabsq_f32(V);
        vTest = vreinterpretq_f32_u32(vcltq_f32(vTest, Internal::kNoFraction));
        // Truncate
        int32x4_t vInt = vcvtq_s32_f32(V);
        float32x4_t vResult = vcvtq_f32_s32(vInt);
        uint32x4_t vLargerMask = vcgtq_f32(vResult, V);
        // 0 -> 0, 0xffffffff -> -1.0f
        float32x4_t vLarger = vcvtq_f32_s32(vreinterpretq_s32_u32(vLargerMask));
        vResult = vaddq_f32(vResult, vLarger);
        // All numbers less than 8388608 will use the round to int
        // All others, use the ORIGINAL value
        return vbslq_f32(vreinterpretq_u32_f32(vTest), vResult, V);
#endif
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_floor_ps(V);
#elif defined(VMATH_SSE_INTRINSICS)
        // To handle NAN, INF and numbers greater than 8388608, use masking
        __m128i vTest = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        vTest = _mm_cmplt_epi32(vTest, Internal::kNoFraction);
        // Truncate
        __m128i vInt = _mm_cvttps_epi32(V);
        vfloat4 vResult = _mm_cvtepi32_ps(vInt);
        __m128 vLarger = _mm_cmpgt_ps(vResult, V);
        // 0 -> 0, 0xffffffff -> -1.0f
        vLarger = _mm_cvtepi32_ps(_mm_castps_si128(vLarger));
        vResult = _mm_add_ps(vResult, vLarger);
        // All numbers less than 8388608 will use the round to int
        vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
        // All others, use the ORIGINAL value
        vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
        vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorCeiling(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Ceiling(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                ceilf(V.v4f[0]),
                ceilf(V.v4f[1]),
                ceilf(V.v4f[2]),
                ceilf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vrndpq_f32(V);
#else
        float32x4_t vTest = vabsq_f32(V);
        vTest = vreinterpretq_f32_u32(vcltq_f32(vTest, Internal::kNoFraction));
        // Truncate
        int32x4_t vInt = vcvtq_s32_f32(V);
        float32x4_t vResult = vcvtq_f32_s32(vInt);
        uint32x4_t vSmallerMask = vcltq_f32(vResult, V);
        // 0 -> 0, 0xffffffff -> -1.0f
        float32x4_t vSmaller = vcvtq_f32_s32(vreinterpretq_s32_u32(vSmallerMask));
        vResult = vsubq_f32(vResult, vSmaller);
        // All numbers less than 8388608 will use the round to int
        // All others, use the ORIGINAL value
        return vbslq_f32(vreinterpretq_u32_f32(vTest), vResult, V);
#endif
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_ceil_ps(V);
#elif defined(VMATH_SSE_INTRINSICS)
        // To handle NAN, INF and numbers greater than 8388608, use masking
        __m128i vTest = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        vTest = _mm_cmplt_epi32(vTest, Internal::kNoFraction);
        // Truncate
        __m128i vInt = _mm_cvttps_epi32(V);
        vfloat4 vResult = _mm_cvtepi32_ps(vInt);
        __m128 vSmaller = _mm_cmplt_ps(vResult, V);
        // 0 -> 0, 0xffffffff -> -1.0f
        vSmaller = _mm_cvtepi32_ps(_mm_castps_si128(vSmaller));
        vResult = _mm_sub_ps(vResult, vSmaller);
        // All numbers less than 8388608 will use the round to int
        vResult = _mm_and_ps(vResult, _mm_castsi128_ps(vTest));
        // All others, use the ORIGINAL value
        vTest = _mm_andnot_si128(vTest, _mm_castps_si128(V));
        vResult = _mm_or_ps(vResult, _mm_castsi128_ps(vTest));
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorClamp(FXMVECTOR V, FXMVECTOR Min, FXMVECTOR Max)
    inline vfloat4 VMATH_CALLCONV v4Clamp(vfloat4 V, vfloat4 Min, vfloat4 Max)
    {
        assert(v4AllLessEqual(Min, Max));

#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v4Max(Min, V);
        Result = v4Min(Max, Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t vResult = vmaxq_f32(Min, V);
        vResult = vminq_f32(Max, vResult);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult;
        vResult = _mm_max_ps(Min, V);
        vResult = _mm_min_ps(Max, vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSaturate(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Clamp01(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        const vfloat4 Zero = v4Zero();

        return v4Clamp(V, Zero, Internal::kOne.v);

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Set <0 to 0
        float32x4_t vResult = vmaxq_f32(V, vdupq_n_f32(0));
        // Set>1 to 1
        return vminq_f32(vResult, vdupq_n_f32(1.0f));
#elif defined(VMATH_SSE_INTRINSICS)
        // Set <0 to 0
        vfloat4 vResult = _mm_max_ps(V, Internal::kZero);
        // Set>1 to 1
        return _mm_min_ps(vResult, Internal::kOne);
#endif
    }

    //------------------------------------------------------------------------------
    // Bitwise logical operations
    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorAndInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4And(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Result = { { {
                V1.v4u[0] & V2.v4u[0],
                V1.v4u[1] & V2.v4u[1],
                V1.v4u[2] & V2.v4u[2],
                V1.v4u[3] & V2.v4u[3]
            } } };
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2)));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_and_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorAndCInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4AndNot(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Result = { { {
                V1.v4u[0] & ~V2.v4u[0],
                V1.v4u[1] & ~V2.v4u[1],
                V1.v4u[2] & ~V2.v4u[2],
                V1.v4u[3] & ~V2.v4u[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vbicq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2)));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_andnot_ps(V2, V1);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorOrInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Or(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Result = { { {
                V1.v4u[0] | V2.v4u[0],
                V1.v4u[1] | V2.v4u[1],
                V1.v4u[2] | V2.v4u[2],
                V1.v4u[3] | V2.v4u[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2)));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_or_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorNorInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Nor(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Result = { { {
                ~(V1.v4u[0] | V2.v4u[0]),
                ~(V1.v4u[1] | V2.v4u[1]),
                ~(V1.v4u[2] | V2.v4u[2]),
                ~(V1.v4u[3] | V2.v4u[3])
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t Result = vorrq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2));
        return vreinterpretq_f32_u32(vbicq_u32(Internal::kNegOneMask, Result));
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i Result;
        Result = _mm_or_si128(_mm_castps_si128(V1), _mm_castps_si128(V2));
        Result = _mm_andnot_si128(Result, Internal::kNegOneMask);
        return _mm_castsi128_ps(Result);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorXorInt(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Xor(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4u32 Result = { { {
                V1.v4u[0] ^ V2.v4u[0],
                V1.v4u[1] ^ V2.v4u[1],
                V1.v4u[2] ^ V2.v4u[2],
                V1.v4u[3] ^ V2.v4u[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2)));
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_xor_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------
    // Computation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorNegate(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Negate(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                -V.v4f[0],
                -V.v4f[1],
                -V.v4f[2],
                -V.v4f[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vnegq_f32(V);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 Z;

        Z = _mm_setzero_ps();

        return _mm_sub_ps(Z, V);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorAdd(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Add(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                V1.v4f[0] + V2.v4f[0],
                V1.v4f[1] + V2.v4f[1],
                V1.v4f[2] + V2.v4f[2],
                V1.v4f[3] + V2.v4f[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vaddq_f32(V1, V2);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_add_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSum(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Sum(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result;
        Result.f[0] =
            Result.f[1] =
            Result.f[2] =
            Result.f[3] = V.v4f[0] + V.v4f[1] + V.v4f[2] + V.v4f[3];
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        float32x4_t vTemp = vpaddq_f32(V, V);
        return vpaddq_f32(vTemp, vTemp);
#else
        float32x2_t v1 = vget_low_f32(V);
        float32x2_t v2 = vget_high_f32(V);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        return vcombine_f32(v1, v1);
#endif
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vTemp = _mm_hadd_ps(V, V);
        return _mm_hadd_ps(vTemp, vTemp);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 3, 0, 1));
        vfloat4 vTemp2 = _mm_add_ps(V, vTemp);
        vTemp = VMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(1, 0, 3, 2));
        return _mm_add_ps(vTemp, vTemp2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorAddAngles(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4AddAngles(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        const vfloat4 Zero = v4Zero();

        // Add the given angles together.  If the range of V1 is such
        // that -Pi <= V1 < Pi and the range of V2 is such that
        // -2Pi <= V2 <= 2Pi, then the range of the resulting angle
        // will be -Pi <= Result < Pi.
        vfloat4 Result = v4Add(V1, V2);

        vfloat4 Mask = v4Less(Result, Internal::kNegativePi.v);
        vfloat4 Offset = v4Select(Zero, Internal::kTwoPi.v, Mask);

        Mask = v4GreaterEqual(Result, Internal::kPi.v);
        Offset = v4Select(Offset, Internal::kNegativeTwoPi.v, Mask);

        Result = v4Add(Result, Offset);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Adjust the angles
        float32x4_t vResult = vaddq_f32(V1, V2);
        // Less than Pi?
        uint32x4_t vOffset = vcltq_f32(vResult, Internal::kNegativePi);
        vOffset = vandq_u32(vOffset, Internal::kTwoPi);
        // Add 2Pi to all entries less than -Pi
        vResult = vaddq_f32(vResult, vreinterpretq_f32_u32(vOffset));
        // Greater than or equal to Pi?
        vOffset = vcgeq_f32(vResult, Internal::kPi);
        vOffset = vandq_u32(vOffset, Internal::kTwoPi);
        // Sub 2Pi to all entries greater than Pi
        vResult = vsubq_f32(vResult, vreinterpretq_f32_u32(vOffset));
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Adjust the angles
        vfloat4 vResult = _mm_add_ps(V1, V2);
        // Less than Pi?
        vfloat4 vOffset = _mm_cmplt_ps(vResult, Internal::kNegativePi);
        vOffset = _mm_and_ps(vOffset, Internal::kTwoPi);
        // Add 2Pi to all entries less than -Pi
        vResult = _mm_add_ps(vResult, vOffset);
        // Greater than or equal to Pi?
        vOffset = _mm_cmpge_ps(vResult, Internal::kPi);
        vOffset = _mm_and_ps(vOffset, Internal::kTwoPi);
        // Sub 2Pi to all entries greater than Pi
        vResult = _mm_sub_ps(vResult, vOffset);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSubtract(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Sub(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                V1.v4f[0] - V2.v4f[0],
                V1.v4f[1] - V2.v4f[1],
                V1.v4f[2] - V2.v4f[2],
                V1.v4f[3] - V2.v4f[3]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vsubq_f32(V1, V2);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_sub_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSubtractAngles(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4SubAngles(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        const vfloat4 Zero = v4Zero();

        // Subtract the given angles.  If the range of V1 is such
        // that -Pi <= V1 < Pi and the range of V2 is such that
        // -2Pi <= V2 <= 2Pi, then the range of the resulting angle
        // will be -Pi <= Result < Pi.
        vfloat4 Result = v4Sub(V1, V2);

        vfloat4 Mask = v4Less(Result, Internal::kNegativePi.v);
        vfloat4 Offset = v4Select(Zero, Internal::kTwoPi.v, Mask);

        Mask = v4GreaterEqual(Result, Internal::kPi.v);
        Offset = v4Select(Offset, Internal::kNegativeTwoPi.v, Mask);

        Result = v4Add(Result, Offset);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Adjust the angles
        vfloat4 vResult = vsubq_f32(V1, V2);
        // Less than Pi?
        uint32x4_t vOffset = vcltq_f32(vResult, Internal::kNegativePi);
        vOffset = vandq_u32(vOffset, Internal::kTwoPi);
        // Add 2Pi to all entries less than -Pi
        vResult = vaddq_f32(vResult, vreinterpretq_f32_u32(vOffset));
        // Greater than or equal to Pi?
        vOffset = vcgeq_f32(vResult, Internal::kPi);
        vOffset = vandq_u32(vOffset, Internal::kTwoPi);
        // Sub 2Pi to all entries greater than Pi
        vResult = vsubq_f32(vResult, vreinterpretq_f32_u32(vOffset));
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Adjust the angles
        vfloat4 vResult = _mm_sub_ps(V1, V2);
        // Less than Pi?
        vfloat4 vOffset = _mm_cmplt_ps(vResult, Internal::kNegativePi);
        vOffset = _mm_and_ps(vOffset, Internal::kTwoPi);
        // Add 2Pi to all entries less than -Pi
        vResult = _mm_add_ps(vResult, vOffset);
        // Greater than or equal to Pi?
        vOffset = _mm_cmpge_ps(vResult, Internal::kPi);
        vOffset = _mm_and_ps(vOffset, Internal::kTwoPi);
        // Sub 2Pi to all entries greater than Pi
        vResult = _mm_sub_ps(vResult, vOffset);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorMultiply(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Mul(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                V1.v4f[0] * V2.v4f[0],
                V1.v4f[1] * V2.v4f[1],
                V1.v4f[2] * V2.v4f[2],
                V1.v4f[3] * V2.v4f[3]
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vmulq_f32(V1, V2);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_mul_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorMultiplyAdd(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR V3)
    inline vfloat4 VMATH_CALLCONV v4MulAdd(vfloat4 V1, vfloat4 V2, vfloat4 V3)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                V1.v4f[0] * V2.v4f[0] + V3.v4f[0],
                V1.v4f[1] * V2.v4f[1] + V3.v4f[1],
                V1.v4f[2] * V2.v4f[2] + V3.v4f[2],
                V1.v4f[3] * V2.v4f[3] + V3.v4f[3]
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vfmaq_f32(V3, V1, V2);
#else
        return vmlaq_f32(V3, V1, V2);
#endif
#elif defined(VMATH_SSE_INTRINSICS)
        return VMATH_FMADD_PS(V1, V2, V3);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorDivide(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Div(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                V1.v4f[0] / V2.v4f[0],
                V1.v4f[1] / V2.v4f[1],
                V1.v4f[2] / V2.v4f[2],
                V1.v4f[3] / V2.v4f[3]
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vdivq_f32(V1, V2);
#else
        // 2 iterations of Newton-Raphson refinement of reciprocal
        float32x4_t Reciprocal = vrecpeq_f32(V2);
        float32x4_t S = vrecpsq_f32(Reciprocal, V2);
        Reciprocal = vmulq_f32(S, Reciprocal);
        S = vrecpsq_f32(Reciprocal, V2);
        Reciprocal = vmulq_f32(S, Reciprocal);
        return vmulq_f32(V1, Reciprocal);
#endif
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_div_ps(V1, V2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorNegativeMultiplySubtract(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR V3)
    inline vfloat4 VMATH_CALLCONV v4NegateMulAdd(vfloat4 V1, vfloat4 V2, vfloat4 V3)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                V3.v4f[0] - (V1.v4f[0] * V2.v4f[0]),
                V3.v4f[1] - (V1.v4f[1] * V2.v4f[1]),
                V3.v4f[2] - (V1.v4f[2] * V2.v4f[2]),
                V3.v4f[3] - (V1.v4f[3] * V2.v4f[3])
            } } };
        return Result;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        return vfmsq_f32(V3, V1, V2);
#else
        return vmlsq_f32(V3, V1, V2);
#endif
#elif defined(VMATH_SSE_INTRINSICS)
        return VMATH_FNMADD_PS(V1, V2, V3);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorReciprocalEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4RcpEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                1.f / V.v4f[0],
                1.f / V.v4f[1],
                1.f / V.v4f[2],
                1.f / V.v4f[3]
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vrecpeq_f32(V);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_rcp_ps(V);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorReciprocal(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Rcp(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                1.f / V.v4f[0],
                1.f / V.v4f[1],
                1.f / V.v4f[2],
                1.f / V.v4f[3]
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC) || __aarch64__
        float32x4_t one = vdupq_n_f32(1.0f);
        return vdivq_f32(one, V);
#else
        // 2 iterations of Newton-Raphson refinement
        float32x4_t Reciprocal = vrecpeq_f32(V);
        float32x4_t S = vrecpsq_f32(Reciprocal, V);
        Reciprocal = vmulq_f32(S, Reciprocal);
        S = vrecpsq_f32(Reciprocal, V);
        return vmulq_f32(S, Reciprocal);
#endif
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_div_ps(Internal::kOne, V);
#endif
    }

    //------------------------------------------------------------------------------
    // Return an estimated square root
    // XMVECTOR XMVectorSqrtEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4SqrtEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                sqrtf(V.v4f[0]),
                sqrtf(V.v4f[1]),
                sqrtf(V.v4f[2]),
                sqrtf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // 1 iteration of Newton-Raphson refinment of sqrt
        float32x4_t S0 = vrsqrteq_f32(V);
        float32x4_t P0 = vmulq_f32(V, S0);
        float32x4_t R0 = vrsqrtsq_f32(P0, S0);
        float32x4_t S1 = vmulq_f32(S0, R0);

        vfloat4 VEqualsInfinity = v4BinaryEqual(V, Internal::kInfinity.v);
        vfloat4 VEqualsZero = v4Equal(V, vdupq_n_f32(0));
        vfloat4 Result = vmulq_f32(V, S1);
        vfloat4 Select = v4BinaryEqual(VEqualsInfinity, VEqualsZero);
        return v4Select(V, Result, Select);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_sqrt_ps(V);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSqrt(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Sqrt(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                sqrtf(V.v4f[0]),
                sqrtf(V.v4f[1]),
                sqrtf(V.v4f[2]),
                sqrtf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // 3 iterations of Newton-Raphson refinment of sqrt
        float32x4_t S0 = vrsqrteq_f32(V);
        float32x4_t P0 = vmulq_f32(V, S0);
        float32x4_t R0 = vrsqrtsq_f32(P0, S0);
        float32x4_t S1 = vmulq_f32(S0, R0);
        float32x4_t P1 = vmulq_f32(V, S1);
        float32x4_t R1 = vrsqrtsq_f32(P1, S1);
        float32x4_t S2 = vmulq_f32(S1, R1);
        float32x4_t P2 = vmulq_f32(V, S2);
        float32x4_t R2 = vrsqrtsq_f32(P2, S2);
        float32x4_t S3 = vmulq_f32(S2, R2);

        vfloat4 VEqualsInfinity = v4BinaryEqual(V, Internal::kInfinity.v);
        vfloat4 VEqualsZero = v4Equal(V, vdupq_n_f32(0));
        vfloat4 Result = vmulq_f32(V, S3);
        vfloat4 Select = v4BinaryEqual(VEqualsInfinity, VEqualsZero);
        return v4Select(V, Result, Select);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_sqrt_ps(V);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorReciprocalSqrtEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4RSqrtEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                1.f / sqrtf(V.v4f[0]),
                1.f / sqrtf(V.v4f[1]),
                1.f / sqrtf(V.v4f[2]),
                1.f / sqrtf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vrsqrteq_f32(V);
#elif defined(VMATH_SSE_INTRINSICS)
        return _mm_rsqrt_ps(V);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorReciprocalSqrt(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4RSqrt(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                1.f / sqrtf(V.v4f[0]),
                1.f / sqrtf(V.v4f[1]),
                1.f / sqrtf(V.v4f[2]),
                1.f / sqrtf(V.v4f[3])
            } } };
        return Result;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // 2 iterations of Newton-Raphson refinement of reciprocal
        float32x4_t S0 = vrsqrteq_f32(V);

        float32x4_t P0 = vmulq_f32(V, S0);
        float32x4_t R0 = vrsqrtsq_f32(P0, S0);

        float32x4_t S1 = vmulq_f32(S0, R0);
        float32x4_t P1 = vmulq_f32(V, S1);
        float32x4_t R1 = vrsqrtsq_f32(P1, S1);

        return vmulq_f32(S1, R1);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = _mm_sqrt_ps(V);
        vResult = _mm_div_ps(Internal::kOne, vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorExp2(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Exp2(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                exp2f(V.v4f[0]),
                exp2f(V.v4f[1]),
                exp2f(V.v4f[2]),
                exp2f(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t itrunc = vcvtq_s32_f32(V);
        float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
        float32x4_t y = vsubq_f32(V, ftrunc);

        float32x4_t poly = vmlaq_f32(Internal::kExpEst6, Internal::kExpEst7, y);
        poly = vmlaq_f32(Internal::kExpEst5, poly, y);
        poly = vmlaq_f32(Internal::kExpEst4, poly, y);
        poly = vmlaq_f32(Internal::kExpEst3, poly, y);
        poly = vmlaq_f32(Internal::kExpEst2, poly, y);
        poly = vmlaq_f32(Internal::kExpEst1, poly, y);
        poly = vmlaq_f32(Internal::kOne, poly, y);

        int32x4_t biased = vaddq_s32(itrunc, Internal::kExponentBias);
        biased = vshlq_n_s32(biased, 23);
        float32x4_t result0 = v4Div(vreinterpretq_f32_s32(biased), poly);

        biased = vaddq_s32(itrunc, Internal::k253);
        biased = vshlq_n_s32(biased, 23);
        float32x4_t result1 = v4Div(vreinterpretq_f32_s32(biased), poly);
        result1 = vmulq_f32(Internal::kMinNormal.v, result1);

        // Use selection to handle the cases
        //  if (V is NaN) -> QNaN;
        //  else if (V sign bit set)
        //      if (V > -150)
        //         if (V.exponent < -126) -> result1
        //         else -> result0
        //      else -> +0
        //  else
        //      if (V < 128) -> result0
        //      else -> +inf

        uint32x4_t comp = vcltq_s32(vreinterpretq_s32_f32(V), Internal::kBin128);
        float32x4_t result2 = vbslq_f32(comp, result0, Internal::kInfinity);

        comp = vcltq_s32(itrunc, Internal::kSubnormalExponent);
        float32x4_t result3 = vbslq_f32(comp, result1, result0);

        comp = vcltq_s32(vreinterpretq_s32_f32(V), Internal::kBinNeg150);
        float32x4_t result4 = vbslq_f32(comp, result3, Internal::kZero);

        int32x4_t sign = vandq_s32(vreinterpretq_s32_f32(V), Internal::kNegativeZero);
        comp = vceqq_s32(sign, Internal::kNegativeZero);
        float32x4_t result5 = vbslq_f32(comp, result4, result2);

        int32x4_t t0 = vandq_s32(vreinterpretq_s32_f32(V), Internal::kQNaNTest);
        int32x4_t t1 = vandq_s32(vreinterpretq_s32_f32(V), Internal::kInfinity);
        t0 = vreinterpretq_s32_u32(vceqq_s32(t0, Internal::kZero));
        t1 = vreinterpretq_s32_u32(vceqq_s32(t1, Internal::kInfinity));
        int32x4_t isNaN = vbicq_s32(t1, t0);

        float32x4_t vResult = vbslq_f32(vreinterpretq_u32_s32(isNaN), Internal::kQNaN, result5);
        return vResult;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_exp2_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i itrunc = _mm_cvttps_epi32(V);
        __m128 ftrunc = _mm_cvtepi32_ps(itrunc);
        __m128 y = _mm_sub_ps(V, ftrunc);

        __m128 poly = VMATH_FMADD_PS(Internal::kExpEst7, y, Internal::kExpEst6);
        poly = VMATH_FMADD_PS(poly, y, Internal::kExpEst5);
        poly = VMATH_FMADD_PS(poly, y, Internal::kExpEst4);
        poly = VMATH_FMADD_PS(poly, y, Internal::kExpEst3);
        poly = VMATH_FMADD_PS(poly, y, Internal::kExpEst2);
        poly = VMATH_FMADD_PS(poly, y, Internal::kExpEst1);
        poly = VMATH_FMADD_PS(poly, y, Internal::kOne);

        __m128i biased = _mm_add_epi32(itrunc, Internal::kExponentBias);
        biased = _mm_slli_epi32(biased, 23);
        __m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

        biased = _mm_add_epi32(itrunc, Internal::k253);
        biased = _mm_slli_epi32(biased, 23);
        __m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
        result1 = _mm_mul_ps(Internal::kMinNormal.v, result1);

        // Use selection to handle the cases
        //  if (V is NaN) -> QNaN;
        //  else if (V sign bit set)
        //      if (V > -150)
        //         if (V.exponent < -126) -> result1
        //         else -> result0
        //      else -> +0
        //  else
        //      if (V < 128) -> result0
        //      else -> +inf

        __m128i comp = _mm_cmplt_epi32(_mm_castps_si128(V), Internal::kBin128);
        __m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
        __m128i select1 = _mm_andnot_si128(comp, Internal::kInfinity);
        __m128i result2 = _mm_or_si128(select0, select1);

        comp = _mm_cmplt_epi32(itrunc, Internal::kSubnormalExponent);
        select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
        select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
        __m128i result3 = _mm_or_si128(select0, select1);

        comp = _mm_cmplt_epi32(_mm_castps_si128(V), Internal::kBinNeg150);
        select0 = _mm_and_si128(comp, result3);
        select1 = _mm_andnot_si128(comp, Internal::kZero);
        __m128i result4 = _mm_or_si128(select0, select1);

        __m128i sign = _mm_and_si128(_mm_castps_si128(V), Internal::kNegativeZero);
        comp = _mm_cmpeq_epi32(sign, Internal::kNegativeZero);
        select0 = _mm_and_si128(comp, result4);
        select1 = _mm_andnot_si128(comp, result2);
        __m128i result5 = _mm_or_si128(select0, select1);

        __m128i t0 = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i t1 = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        t0 = _mm_cmpeq_epi32(t0, Internal::kZero);
        t1 = _mm_cmpeq_epi32(t1, Internal::kInfinity);
        __m128i isNaN = _mm_andnot_si128(t0, t1);

        select0 = _mm_and_si128(isNaN, Internal::kQNaN);
        select1 = _mm_andnot_si128(isNaN, result5);
        __m128i vResult = _mm_or_si128(select0, select1);

        return _mm_castsi128_ps(vResult);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorExp10(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Exp10(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                powf(10.0f, V.v4f[0]),
                powf(10.0f, V.v4f[1]),
                powf(10.0f, V.v4f[2]),
                powf(10.0f, V.v4f[3])
            } } };
        return Result.v;

#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_exp10_ps(V);
        return Result;
#else
        // exp10(V) = exp2(vin*log2(10))
        vfloat4 Vten = v4Mul(Internal::kLg10, V);
        return v4Exp2(Vten);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorExpE(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ExpE(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                expf(V.v4f[0]),
                expf(V.v4f[1]),
                expf(V.v4f[2]),
                expf(V.v4f[3])
            } } };
        return Result.v;

#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_exp_ps(V);
        return Result;
#else
        // expE(V) = exp2(vin*log2(e))
        vfloat4 Ve = v4Mul(Internal::kLgE, V);
        return v4Exp2(Ve);
#endif
    }

    //------------------------------------------------------------------------------

#if defined(VMATH_SSE_INTRINSICS)

    namespace Internal
    {
        inline __m128i multi_sll_epi32(__m128i value, __m128i count)
        {
            __m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
            __m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r0 = _mm_sll_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r1 = _mm_sll_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r2 = _mm_sll_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r3 = _mm_sll_epi32(v, c);

            // (r0,r0,r1,r1)
            __m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
            // (r2,r2,r3,r3)
            __m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
            // (r0,r1,r2,r3)
            __m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
            return _mm_castps_si128(result);
        }

        inline __m128i multi_srl_epi32(__m128i value, __m128i count)
        {
            __m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
            __m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r0 = _mm_srl_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r1 = _mm_srl_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r2 = _mm_srl_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
            c = _mm_and_si128(c, Internal::kMaskX);
            __m128i r3 = _mm_srl_epi32(v, c);

            // (r0,r0,r1,r1)
            __m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
            // (r2,r2,r3,r3)
            __m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
            // (r0,r1,r2,r3)
            __m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
            return _mm_castps_si128(result);
        }

        inline __m128i GetLeadingBit(const __m128i value)
        {
            static const Internal::vfloat4i32 k0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
            static const Internal::vfloat4i32 k000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
            static const Internal::vfloat4i32 k0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
            static const Internal::vfloat4i32 k00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

            __m128i v = value, r, c, b, s;

            c = _mm_cmpgt_epi32(v, k0000FFFF);   // c = (v > 0xFFFF)
            b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
            r = _mm_slli_epi32(b, 4);               // r = (b << 4)
            v = multi_srl_epi32(v, r);              // v = (v >> r)

            c = _mm_cmpgt_epi32(v, k000000FF);   // c = (v > 0xFF)
            b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
            s = _mm_slli_epi32(b, 3);               // s = (b << 3)
            v = multi_srl_epi32(v, s);              // v = (v >> s)
            r = _mm_or_si128(r, s);                 // r = (r | s)

            c = _mm_cmpgt_epi32(v, k0000000F);   // c = (v > 0xF)
            b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
            s = _mm_slli_epi32(b, 2);               // s = (b << 2)
            v = multi_srl_epi32(v, s);              // v = (v >> s)
            r = _mm_or_si128(r, s);                 // r = (r | s)

            c = _mm_cmpgt_epi32(v, k00000003);   // c = (v > 0x3)
            b = _mm_srli_epi32(c, 31);              // b = (c ? 1 : 0)
            s = _mm_slli_epi32(b, 1);               // s = (b << 1)
            v = multi_srl_epi32(v, s);              // v = (v >> s)
            r = _mm_or_si128(r, s);                 // r = (r | s)

            s = _mm_srli_epi32(v, 1);
            r = _mm_or_si128(r, s);
            return r;
        }
    } // namespace Internal

#endif // VMATH_SSE_INTRINSICS

#if defined(VMATH_ARM_NEON_INTRINSICS)

    namespace Internal
    {
        inline int32x4_t GetLeadingBit(const int32x4_t value)
        {
            static const Internal::vfloat4i32 k0000FFFF = { { { 0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF } } };
            static const Internal::vfloat4i32 k000000FF = { { { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF } } };
            static const Internal::vfloat4i32 k0000000F = { { { 0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F } } };
            static const Internal::vfloat4i32 k00000003 = { { { 0x00000003, 0x00000003, 0x00000003, 0x00000003 } } };

            uint32x4_t c = vcgtq_s32(value, k0000FFFF);              // c = (v > 0xFFFF)
            int32x4_t b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31);    // b = (c ? 1 : 0)
            int32x4_t r = vshlq_n_s32(b, 4);                            // r = (b << 4)
            r = vnegq_s32(r);
            int32x4_t v = vshlq_s32(value, r);                          // v = (v >> r)

            c = vcgtq_s32(v, k000000FF);                             // c = (v > 0xFF)
            b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31);              // b = (c ? 1 : 0)
            int32x4_t s = vshlq_n_s32(b, 3);                            // s = (b << 3)
            s = vnegq_s32(s);
            v = vshlq_s32(v, s);                                        // v = (v >> s)
            r = vorrq_s32(r, s);                                        // r = (r | s)

            c = vcgtq_s32(v, k0000000F);                             // c = (v > 0xF)
            b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31);              // b = (c ? 1 : 0)
            s = vshlq_n_s32(b, 2);                                      // s = (b << 2)
            s = vnegq_s32(s);
            v = vshlq_s32(v, s);                                        // v = (v >> s)
            r = vorrq_s32(r, s);                                        // r = (r | s)

            c = vcgtq_s32(v, k00000003);                             // c = (v > 0x3)
            b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31);              // b = (c ? 1 : 0)
            s = vshlq_n_s32(b, 1);                                      // s = (b << 1)
            s = vnegq_s32(s);
            v = vshlq_s32(v, s);                                        // v = (v >> s)
            r = vorrq_s32(r, s);                                        // r = (r | s)

            s = vshrq_n_s32(v, 1);
            r = vorrq_s32(r, s);
            return r;
        }

    } // namespace Internal

#endif

//------------------------------------------------------------------------------

    // XMVECTOR XMVectorLog2(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Log2(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                log2f(V.v4f[0]),
                log2f(V.v4f[1]),
                log2f(V.v4f[2]),
                log2f(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t rawBiased = vandq_s32(vreinterpretq_s32_f32(V), Internal::kInfinity);
        int32x4_t trailing = vandq_s32(vreinterpretq_s32_f32(V), Internal::kQNaNTest);
        uint32x4_t isExponentZero = vceqq_s32(vreinterpretq_s32_f32(Internal::kZero), rawBiased);

        // Compute exponent and significand for normals.
        int32x4_t biased = vshrq_n_s32(rawBiased, 23);
        int32x4_t exponentNor = vsubq_s32(biased, Internal::kExponentBias);
        int32x4_t trailingNor = trailing;

        // Compute exponent and significand for subnormals.
        int32x4_t leading = Internal::GetLeadingBit(trailing);
        int32x4_t shift = vsubq_s32(Internal::kNumTrailing, leading);
        int32x4_t exponentSub = vsubq_s32(Internal::kSubnormalExponent, shift);
        int32x4_t trailingSub = vshlq_s32(trailing, shift);
        trailingSub = vandq_s32(trailingSub, Internal::kQNaNTest);
        int32x4_t e = vbslq_s32(isExponentZero, exponentSub, exponentNor);
        int32x4_t t = vbslq_s32(isExponentZero, trailingSub, trailingNor);

        // Compute the approximation.
        int32x4_t tmp = vorrq_s32(vreinterpretq_s32_f32(Internal::kOne), t);
        float32x4_t y = vsubq_f32(vreinterpretq_f32_s32(tmp), Internal::kOne);

        float32x4_t log2 = vmlaq_f32(Internal::kLogEst6, Internal::kLogEst7, y);
        log2 = vmlaq_f32(Internal::kLogEst5, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst4, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst3, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst2, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst1, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst0, log2, y);
        log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

        //  if (x is NaN) -> QNaN
        //  else if (V is positive)
        //      if (V is infinite) -> +inf
        //      else -> log2(V)
        //  else
        //      if (V is zero) -> -inf
        //      else -> -QNaN

        uint32x4_t isInfinite = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        isInfinite = vceqq_u32(isInfinite, Internal::kInfinity);

        uint32x4_t isGreaterZero = vcgtq_f32(V, Internal::kZero);
        uint32x4_t isNotFinite = vcgtq_f32(V, Internal::kInfinity);
        uint32x4_t isPositive = vbicq_u32(isGreaterZero, isNotFinite);

        uint32x4_t isZero = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        isZero = vceqq_u32(isZero, Internal::kZero);

        uint32x4_t t0 = vandq_u32(vreinterpretq_u32_f32(V), Internal::kQNaNTest);
        uint32x4_t t1 = vandq_u32(vreinterpretq_u32_f32(V), Internal::kInfinity);
        t0 = vceqq_u32(t0, Internal::kZero);
        t1 = vceqq_u32(t1, Internal::kInfinity);
        uint32x4_t isNaN = vbicq_u32(t1, t0);

        float32x4_t result = vbslq_f32(isInfinite, Internal::kInfinity, log2);
        float32x4_t tmp2 = vbslq_f32(isZero, Internal::kNegInfinity, Internal::kNegQNaN);
        result = vbslq_f32(isPositive, result, tmp2);
        result = vbslq_f32(isNaN, Internal::kQNaN, result);
        return result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_log2_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        __m128i trailing = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i isExponentZero = _mm_cmpeq_epi32(Internal::kZero, rawBiased);

        // Compute exponent and significand for normals.
        __m128i biased = _mm_srli_epi32(rawBiased, 23);
        __m128i exponentNor = _mm_sub_epi32(biased, Internal::kExponentBias);
        __m128i trailingNor = trailing;

        // Compute exponent and significand for subnormals.
        __m128i leading = Internal::GetLeadingBit(trailing);
        __m128i shift = _mm_sub_epi32(Internal::kNumTrailing, leading);
        __m128i exponentSub = _mm_sub_epi32(Internal::kSubnormalExponent, shift);
        __m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
        trailingSub = _mm_and_si128(trailingSub, Internal::kQNaNTest);

        __m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
        __m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
        __m128i e = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isExponentZero, trailingSub);
        select1 = _mm_andnot_si128(isExponentZero, trailingNor);
        __m128i t = _mm_or_si128(select0, select1);

        // Compute the approximation.
        __m128i tmp = _mm_or_si128(Internal::kOne, t);
        __m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), Internal::kOne);

        __m128 log2 = VMATH_FMADD_PS(Internal::kLogEst7, y, Internal::kLogEst6);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst5);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst4);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst3);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst2);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst1);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst0);
        log2 = VMATH_FMADD_PS(log2, y, _mm_cvtepi32_ps(e));

        //  if (x is NaN) -> QNaN
        //  else if (V is positive)
        //      if (V is infinite) -> +inf
        //      else -> log2(V)
        //  else
        //      if (V is zero) -> -inf
        //      else -> -QNaN

        __m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        isInfinite = _mm_cmpeq_epi32(isInfinite, Internal::kInfinity);

        __m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), Internal::kZero);
        __m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), Internal::kInfinity);
        __m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

        __m128i isZero = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        isZero = _mm_cmpeq_epi32(isZero, Internal::kZero);

        __m128i t0 = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i t1 = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        t0 = _mm_cmpeq_epi32(t0, Internal::kZero);
        t1 = _mm_cmpeq_epi32(t1, Internal::kInfinity);
        __m128i isNaN = _mm_andnot_si128(t0, t1);

        select0 = _mm_and_si128(isInfinite, Internal::kInfinity);
        select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
        __m128i result = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isZero, Internal::kNegInfinity);
        select1 = _mm_andnot_si128(isZero, Internal::kNegQNaN);
        tmp = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isPositive, result);
        select1 = _mm_andnot_si128(isPositive, tmp);
        result = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isNaN, Internal::kQNaN);
        select1 = _mm_andnot_si128(isNaN, result);
        result = _mm_or_si128(select0, select1);

        return _mm_castsi128_ps(result);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorLog10(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Log10(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                log10f(V.v4f[0]),
                log10f(V.v4f[1]),
                log10f(V.v4f[2]),
                log10f(V.v4f[3])
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t rawBiased = vandq_s32(vreinterpretq_s32_f32(V), Internal::kInfinity);
        int32x4_t trailing = vandq_s32(vreinterpretq_s32_f32(V), Internal::kQNaNTest);
        uint32x4_t isExponentZero = vceqq_s32(Internal::kZero, rawBiased);

        // Compute exponent and significand for normals.
        int32x4_t biased = vshrq_n_s32(rawBiased, 23);
        int32x4_t exponentNor = vsubq_s32(biased, Internal::kExponentBias);
        int32x4_t trailingNor = trailing;

        // Compute exponent and significand for subnormals.
        int32x4_t leading = Internal::GetLeadingBit(trailing);
        int32x4_t shift = vsubq_s32(Internal::kNumTrailing, leading);
        int32x4_t exponentSub = vsubq_s32(Internal::kSubnormalExponent, shift);
        int32x4_t trailingSub = vshlq_s32(trailing, shift);
        trailingSub = vandq_s32(trailingSub, Internal::kQNaNTest);
        int32x4_t e = vbslq_s32(isExponentZero, exponentSub, exponentNor);
        int32x4_t t = vbslq_s32(isExponentZero, trailingSub, trailingNor);

        // Compute the approximation.
        int32x4_t tmp = vorrq_s32(Internal::kOne, t);
        float32x4_t y = vsubq_f32(vreinterpretq_f32_s32(tmp), Internal::kOne);

        float32x4_t log2 = vmlaq_f32(Internal::kLogEst6, Internal::kLogEst7, y);
        log2 = vmlaq_f32(Internal::kLogEst5, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst4, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst3, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst2, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst1, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst0, log2, y);
        log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

        log2 = vmulq_f32(Internal::kInvLg10, log2);

        //  if (x is NaN) -> QNaN
        //  else if (V is positive)
        //      if (V is infinite) -> +inf
        //      else -> log2(V)
        //  else
        //      if (V is zero) -> -inf
        //      else -> -QNaN

        uint32x4_t isInfinite = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        isInfinite = vceqq_u32(isInfinite, Internal::kInfinity);

        uint32x4_t isGreaterZero = vcgtq_s32(vreinterpretq_s32_f32(V), Internal::kZero);
        uint32x4_t isNotFinite = vcgtq_s32(vreinterpretq_s32_f32(V), Internal::kInfinity);
        uint32x4_t isPositive = vbicq_u32(isGreaterZero, isNotFinite);

        uint32x4_t isZero = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        isZero = vceqq_u32(isZero, Internal::kZero);

        uint32x4_t t0 = vandq_u32(vreinterpretq_u32_f32(V), Internal::kQNaNTest);
        uint32x4_t t1 = vandq_u32(vreinterpretq_u32_f32(V), Internal::kInfinity);
        t0 = vceqq_u32(t0, Internal::kZero);
        t1 = vceqq_u32(t1, Internal::kInfinity);
        uint32x4_t isNaN = vbicq_u32(t1, t0);

        float32x4_t result = vbslq_f32(isInfinite, Internal::kInfinity, log2);
        float32x4_t tmp2 = vbslq_f32(isZero, Internal::kNegInfinity, Internal::kNegQNaN);
        result = vbslq_f32(isPositive, result, tmp2);
        result = vbslq_f32(isNaN, Internal::kQNaN, result);
        return result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_log10_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        __m128i trailing = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i isExponentZero = _mm_cmpeq_epi32(Internal::kZero, rawBiased);

        // Compute exponent and significand for normals.
        __m128i biased = _mm_srli_epi32(rawBiased, 23);
        __m128i exponentNor = _mm_sub_epi32(biased, Internal::kExponentBias);
        __m128i trailingNor = trailing;

        // Compute exponent and significand for subnormals.
        __m128i leading = Internal::GetLeadingBit(trailing);
        __m128i shift = _mm_sub_epi32(Internal::kNumTrailing, leading);
        __m128i exponentSub = _mm_sub_epi32(Internal::kSubnormalExponent, shift);
        __m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
        trailingSub = _mm_and_si128(trailingSub, Internal::kQNaNTest);

        __m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
        __m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
        __m128i e = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isExponentZero, trailingSub);
        select1 = _mm_andnot_si128(isExponentZero, trailingNor);
        __m128i t = _mm_or_si128(select0, select1);

        // Compute the approximation.
        __m128i tmp = _mm_or_si128(Internal::kOne, t);
        __m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), Internal::kOne);

        __m128 log2 = VMATH_FMADD_PS(Internal::kLogEst7, y, Internal::kLogEst6);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst5);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst4);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst3);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst2);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst1);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst0);
        log2 = VMATH_FMADD_PS(log2, y, _mm_cvtepi32_ps(e));

        log2 = _mm_mul_ps(Internal::kInvLg10, log2);

        //  if (x is NaN) -> QNaN
        //  else if (V is positive)
        //      if (V is infinite) -> +inf
        //      else -> log2(V)
        //  else
        //      if (V is zero) -> -inf
        //      else -> -QNaN

        __m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        isInfinite = _mm_cmpeq_epi32(isInfinite, Internal::kInfinity);

        __m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), Internal::kZero);
        __m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), Internal::kInfinity);
        __m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

        __m128i isZero = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        isZero = _mm_cmpeq_epi32(isZero, Internal::kZero);

        __m128i t0 = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i t1 = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        t0 = _mm_cmpeq_epi32(t0, Internal::kZero);
        t1 = _mm_cmpeq_epi32(t1, Internal::kInfinity);
        __m128i isNaN = _mm_andnot_si128(t0, t1);

        select0 = _mm_and_si128(isInfinite, Internal::kInfinity);
        select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
        __m128i result = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isZero, Internal::kNegInfinity);
        select1 = _mm_andnot_si128(isZero, Internal::kNegQNaN);
        tmp = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isPositive, result);
        select1 = _mm_andnot_si128(isPositive, tmp);
        result = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isNaN, Internal::kQNaN);
        select1 = _mm_andnot_si128(isNaN, result);
        result = _mm_or_si128(select0, select1);

        return _mm_castsi128_ps(result);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorLogE(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4LogE(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                logf(V.v4f[0]),
                logf(V.v4f[1]),
                logf(V.v4f[2]),
                logf(V.v4f[3])
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        int32x4_t rawBiased = vandq_s32(vreinterpretq_s32_f32(V), Internal::kInfinity);
        int32x4_t trailing = vandq_s32(vreinterpretq_s32_f32(V), Internal::kQNaNTest);
        uint32x4_t isExponentZero = vceqq_s32(Internal::kZero, rawBiased);

        // Compute exponent and significand for normals.
        int32x4_t biased = vshrq_n_s32(rawBiased, 23);
        int32x4_t exponentNor = vsubq_s32(biased, Internal::kExponentBias);
        int32x4_t trailingNor = trailing;

        // Compute exponent and significand for subnormals.
        int32x4_t leading = Internal::GetLeadingBit(trailing);
        int32x4_t shift = vsubq_s32(Internal::kNumTrailing, leading);
        int32x4_t exponentSub = vsubq_s32(Internal::kSubnormalExponent, shift);
        int32x4_t trailingSub = vshlq_s32(trailing, shift);
        trailingSub = vandq_s32(trailingSub, Internal::kQNaNTest);
        int32x4_t e = vbslq_s32(isExponentZero, exponentSub, exponentNor);
        int32x4_t t = vbslq_s32(isExponentZero, trailingSub, trailingNor);

        // Compute the approximation.
        int32x4_t tmp = vorrq_s32(Internal::kOne, t);
        float32x4_t y = vsubq_f32(vreinterpretq_f32_s32(tmp), Internal::kOne);

        float32x4_t log2 = vmlaq_f32(Internal::kLogEst6, Internal::kLogEst7, y);
        log2 = vmlaq_f32(Internal::kLogEst5, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst4, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst3, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst2, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst1, log2, y);
        log2 = vmlaq_f32(Internal::kLogEst0, log2, y);
        log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

        log2 = vmulq_f32(Internal::kInvLgE, log2);

        //  if (x is NaN) -> QNaN
        //  else if (V is positive)
        //      if (V is infinite) -> +inf
        //      else -> log2(V)
        //  else
        //      if (V is zero) -> -inf
        //      else -> -QNaN

        uint32x4_t isInfinite = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        isInfinite = vceqq_u32(isInfinite, Internal::kInfinity);

        uint32x4_t isGreaterZero = vcgtq_s32(vreinterpretq_s32_f32(V), Internal::kZero);
        uint32x4_t isNotFinite = vcgtq_s32(vreinterpretq_s32_f32(V), Internal::kInfinity);
        uint32x4_t isPositive = vbicq_u32(isGreaterZero, isNotFinite);

        uint32x4_t isZero = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        isZero = vceqq_u32(isZero, Internal::kZero);

        uint32x4_t t0 = vandq_u32(vreinterpretq_u32_f32(V), Internal::kQNaNTest);
        uint32x4_t t1 = vandq_u32(vreinterpretq_u32_f32(V), Internal::kInfinity);
        t0 = vceqq_u32(t0, Internal::kZero);
        t1 = vceqq_u32(t1, Internal::kInfinity);
        uint32x4_t isNaN = vbicq_u32(t1, t0);

        float32x4_t result = vbslq_f32(isInfinite, Internal::kInfinity, log2);
        float32x4_t tmp2 = vbslq_f32(isZero, Internal::kNegInfinity, Internal::kNegQNaN);
        result = vbslq_f32(isPositive, result, tmp2);
        result = vbslq_f32(isNaN, Internal::kQNaN, result);
        return result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_log_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i rawBiased = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        __m128i trailing = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i isExponentZero = _mm_cmpeq_epi32(Internal::kZero, rawBiased);

        // Compute exponent and significand for normals.
        __m128i biased = _mm_srli_epi32(rawBiased, 23);
        __m128i exponentNor = _mm_sub_epi32(biased, Internal::kExponentBias);
        __m128i trailingNor = trailing;

        // Compute exponent and significand for subnormals.
        __m128i leading = Internal::GetLeadingBit(trailing);
        __m128i shift = _mm_sub_epi32(Internal::kNumTrailing, leading);
        __m128i exponentSub = _mm_sub_epi32(Internal::kSubnormalExponent, shift);
        __m128i trailingSub = Internal::multi_sll_epi32(trailing, shift);
        trailingSub = _mm_and_si128(trailingSub, Internal::kQNaNTest);

        __m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
        __m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
        __m128i e = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isExponentZero, trailingSub);
        select1 = _mm_andnot_si128(isExponentZero, trailingNor);
        __m128i t = _mm_or_si128(select0, select1);

        // Compute the approximation.
        __m128i tmp = _mm_or_si128(Internal::kOne, t);
        __m128 y = _mm_sub_ps(_mm_castsi128_ps(tmp), Internal::kOne);

        __m128 log2 = VMATH_FMADD_PS(Internal::kLogEst7, y, Internal::kLogEst6);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst5);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst4);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst3);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst2);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst1);
        log2 = VMATH_FMADD_PS(log2, y, Internal::kLogEst0);
        log2 = VMATH_FMADD_PS(log2, y, _mm_cvtepi32_ps(e));

        log2 = _mm_mul_ps(Internal::kInvLgE, log2);

        //  if (x is NaN) -> QNaN
        //  else if (V is positive)
        //      if (V is infinite) -> +inf
        //      else -> log2(V)
        //  else
        //      if (V is zero) -> -inf
        //      else -> -QNaN

        __m128i isInfinite = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        isInfinite = _mm_cmpeq_epi32(isInfinite, Internal::kInfinity);

        __m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(V), Internal::kZero);
        __m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(V), Internal::kInfinity);
        __m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

        __m128i isZero = _mm_and_si128(_mm_castps_si128(V), Internal::kAbsMask);
        isZero = _mm_cmpeq_epi32(isZero, Internal::kZero);

        __m128i t0 = _mm_and_si128(_mm_castps_si128(V), Internal::kQNaNTest);
        __m128i t1 = _mm_and_si128(_mm_castps_si128(V), Internal::kInfinity);
        t0 = _mm_cmpeq_epi32(t0, Internal::kZero);
        t1 = _mm_cmpeq_epi32(t1, Internal::kInfinity);
        __m128i isNaN = _mm_andnot_si128(t0, t1);

        select0 = _mm_and_si128(isInfinite, Internal::kInfinity);
        select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
        __m128i result = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isZero, Internal::kNegInfinity);
        select1 = _mm_andnot_si128(isZero, Internal::kNegQNaN);
        tmp = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isPositive, result);
        select1 = _mm_andnot_si128(isPositive, tmp);
        result = _mm_or_si128(select0, select1);

        select0 = _mm_and_si128(isNaN, Internal::kQNaN);
        select1 = _mm_andnot_si128(isNaN, result);
        result = _mm_or_si128(select0, select1);

        return _mm_castsi128_ps(result);
#endif
    }

    // XMVECTOR XMVectorPow(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Pow(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                powf(V1.v4f[0], V2.v4f[0]),
                powf(V1.v4f[1], V2.v4f[1]),
                powf(V1.v4f[2], V2.v4f[2]),
                powf(V1.v4f[3], V2.v4f[3])
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        Internal::vfloat4f32 vResult = { { {
                powf(vgetq_lane_f32(V1, 0), vgetq_lane_f32(V2, 0)),
                powf(vgetq_lane_f32(V1, 1), vgetq_lane_f32(V2, 1)),
                powf(vgetq_lane_f32(V1, 2), vgetq_lane_f32(V2, 2)),
                powf(vgetq_lane_f32(V1, 3), vgetq_lane_f32(V2, 3))
            } } };
        return vResult.v;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_pow_ps(V1, V2);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        VMATH_ALIGNED_DATA(16) float a[4];
        VMATH_ALIGNED_DATA(16) float b[4];
        _mm_store_ps(a, V1);
        _mm_store_ps(b, V2);
        vfloat4 vResult = _mm_setr_ps(
            powf(a[0], b[0]),
            powf(a[1], b[1]),
            powf(a[2], b[2]),
            powf(a[3], b[3]));
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorAbs(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Abs(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult = { { {
                fabsf(V.v4f[0]),
                fabsf(V.v4f[1]),
                fabsf(V.v4f[2]),
                fabsf(V.v4f[3])
            } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return vabsq_f32(V);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = _mm_setzero_ps();
        vResult = _mm_sub_ps(vResult, V);
        vResult = _mm_max_ps(vResult, V);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorMod(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Mod(vfloat4 V1, vfloat4 V2)
    {
        // V1 % V2 = V1 - V2 * truncate(V1 / V2)

#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Quotient = v4Div(V1, V2);
        Quotient = v4Truncate(Quotient);
        vfloat4 Result = v4NegateMulAdd(V2, Quotient, V1);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 vResult = v4Div(V1, V2);
        vResult = v4Truncate(vResult);
        return vmlsq_f32(V1, vResult, V2);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = _mm_div_ps(V1, V2);
        vResult = v4Truncate(vResult);
        return VMATH_FNMADD_PS(vResult, V2, V1);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorModAngles(FXMVECTOR Angles)
    inline vfloat4 VMATH_CALLCONV v4ModAngles(vfloat4 Angles)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 V;
        vfloat4 Result;

        // Modulo the range of the given angles such that -Pi <= Angles < Pi
        V = v4Mul(Angles, Internal::kReciprocalTwoPi.v);
        V = v4Round(V);
        Result = v4NegateMulAdd(Internal::kTwoPi.v, V, Angles);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Modulo the range of the given angles such that -Pi <= Angles < Pi
        vfloat4 vResult = vmulq_f32(Angles, Internal::kReciprocalTwoPi);
        // Use the inline function due to complexity for rounding
        vResult = v4Round(vResult);
        return vmlsq_f32(Angles, vResult, Internal::kTwoPi);
#elif defined(VMATH_SSE_INTRINSICS)
        // Modulo the range of the given angles such that -Pi <= Angles < Pi
        vfloat4 vResult = _mm_mul_ps(Angles, Internal::kReciprocalTwoPi);
        // Use the inline function due to complexity for rounding
        vResult = v4Round(vResult);
        return VMATH_FNMADD_PS(vResult, Internal::kTwoPi, Angles);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSin(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Sin(vfloat4 V)
    {
        // 11-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                sinf(V.v4f[0]),
                sinf(V.v4f[1]),
                sinf(V.v4f[2]),
                sinf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with sin(y) = sin(x).
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), Internal::kNegativeZero);
        uint32x4_t c = vorrq_u32(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        float32x4_t absx = vabsq_f32(x);
        float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
        uint32x4_t comp = vcleq_f32(absx, Internal::kHalfPi);
        x = vbslq_f32(comp, x, rflx);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation
        const vfloat4 SC1 = Internal::kSinCoefficients1;
        const vfloat4 SC0 = Internal::kSinCoefficients0;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

        vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        Result = vmulq_f32(Result, x);
        return Result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_sin_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with sin(y) = sin(x).
        __m128 sign = _mm_and_ps(x, Internal::kNegativeZero);
        __m128 c = _mm_or_ps(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        __m128 absx = _mm_andnot_ps(sign, x);  // |x|
        __m128 rflx = _mm_sub_ps(c, x);
        __m128 comp = _mm_cmple_ps(absx, Internal::kHalfPi);
        __m128 select0 = _mm_and_ps(comp, x);
        __m128 select1 = _mm_andnot_ps(comp, rflx);
        x = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation
        const vfloat4 SC1 = Internal::kSinCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
        const vfloat4 SC0 = Internal::kSinCoefficients0;
        __m128 vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, x);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorCos(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Cos(vfloat4 V)
    {
        // 10-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                cosf(V.v4f[0]),
                cosf(V.v4f[1]),
                cosf(V.v4f[2]),
                cosf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
    // Map V to x in [-pi,pi].
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), Internal::kNegativeZero);
        uint32x4_t c = vorrq_u32(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        float32x4_t absx = vabsq_f32(x);
        float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
        uint32x4_t comp = vcleq_f32(absx, Internal::kHalfPi);
        x = vbslq_f32(comp, x, rflx);
        float32x4_t fsign = vbslq_f32(comp, Internal::kOne, Internal::kNegativeOne);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation
        const vfloat4 CC1 = Internal::kCosCoefficients1;
        const vfloat4 CC0 = Internal::kCosCoefficients0;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

        vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        Result = vmulq_f32(Result, fsign);
        return Result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_cos_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
    // Map V to x in [-pi,pi].
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
        vfloat4 sign = _mm_and_ps(x, Internal::kNegativeZero);
        __m128 c = _mm_or_ps(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        __m128 absx = _mm_andnot_ps(sign, x);  // |x|
        __m128 rflx = _mm_sub_ps(c, x);
        __m128 comp = _mm_cmple_ps(absx, Internal::kHalfPi);
        __m128 select0 = _mm_and_ps(comp, x);
        __m128 select1 = _mm_andnot_ps(comp, rflx);
        x = _mm_or_ps(select0, select1);
        select0 = _mm_and_ps(comp, Internal::kOne);
        select1 = _mm_andnot_ps(comp, Internal::kNegativeOne);
        sign = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation
        const vfloat4 CC1 = Internal::kCosCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
        const vfloat4 CC0 = Internal::kCosCoefficients0;
        __m128 vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, sign);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // void XMVectorSinCos(XMVECTOR* pSin, XMVECTOR* pCos, FXMVECTOR V)
    inline void VMATH_CALLCONV v4SinCos(vfloat4* pSin, vfloat4* pCos, vfloat4 V)
    {
        assert(pSin != nullptr);
        assert(pCos != nullptr);

        // 11/10-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Sin = { { {
                sinf(V.v4f[0]),
                sinf(V.v4f[1]),
                sinf(V.v4f[2]),
                sinf(V.v4f[3])
            } } };

        Internal::vfloat4f32 Cos = { { {
                cosf(V.v4f[0]),
                cosf(V.v4f[1]),
                cosf(V.v4f[2]),
                cosf(V.v4f[3])
            } } };

        *pSin = Sin.v;
        *pCos = Cos.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), Internal::kNegativeZero);
        uint32x4_t c = vorrq_u32(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        float32x4_t absx = vabsq_f32(x);
        float32x4_t  rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
        uint32x4_t comp = vcleq_f32(absx, Internal::kHalfPi);
        x = vbslq_f32(comp, x, rflx);
        float32x4_t fsign = vbslq_f32(comp, Internal::kOne, Internal::kNegativeOne);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation for sine
        const vfloat4 SC1 = Internal::kSinCoefficients1;
        const vfloat4 SC0 = Internal::kSinCoefficients0;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

        vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        *pSin = vmulq_f32(Result, x);

        // Compute polynomial approximation for cosine
        const vfloat4 CC1 = Internal::kCosCoefficients1;
        const vfloat4 CC0 = Internal::kCosCoefficients0;
        vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
        Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

        vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        *pCos = vmulq_f32(Result, fsign);
#elif defined(VMATH_SVML_INTRINSICS)
        *pSin = _mm_sincos_ps(pCos, V);
#elif defined(VMATH_SSE_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
        vfloat4 sign = _mm_and_ps(x, Internal::kNegativeZero);
        __m128 c = _mm_or_ps(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        __m128 absx = _mm_andnot_ps(sign, x);  // |x|
        __m128 rflx = _mm_sub_ps(c, x);
        __m128 comp = _mm_cmple_ps(absx, Internal::kHalfPi);
        __m128 select0 = _mm_and_ps(comp, x);
        __m128 select1 = _mm_andnot_ps(comp, rflx);
        x = _mm_or_ps(select0, select1);
        select0 = _mm_and_ps(comp, Internal::kOne);
        select1 = _mm_andnot_ps(comp, Internal::kNegativeOne);
        sign = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation of sine
        const vfloat4 SC1 = Internal::kSinCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
        const vfloat4 SC0 = Internal::kSinCoefficients0;
        __m128 vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, x);
        *pSin = Result;

        // Compute polynomial approximation of cosine
        const vfloat4 CC1 = Internal::kCosCoefficients1;
        vConstantsB = VMATH_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
        const vfloat4 CC0 = Internal::kCosCoefficients0;
        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
        Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, sign);
        *pCos = Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorTan(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Tan(vfloat4 V)
    {
        // Cody and Waite algorithm to compute tangent.

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                tanf(V.v4f[0]),
                tanf(V.v4f[1]),
                tanf(V.v4f[2]),
                tanf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_tan_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        static const Internal::vfloat4f32 TanCoefficients0 = { { { 1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f } } };
        static const Internal::vfloat4f32 TanCoefficients1 = { { { 4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f } } };
        static const Internal::vfloat4f32 TanConstants = { { { 1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/ } } };
        static const Internal::vfloat4u32 Mask = { { { 0x1, 0x1, 0x1, 0x1 } } };

        vfloat4 TwoDivPi = v4wwww(TanConstants.v);

        vfloat4 Zero = v4Zero();

        vfloat4 C0 = v4xxxx(TanConstants.v);
        vfloat4 C1 = v4yyyy(TanConstants.v);
        vfloat4 Epsilon = v4zzzz(TanConstants.v);

        vfloat4 VA = v4Mul(V, TwoDivPi);

        VA = v4Round(VA);

        vfloat4 VC = v4NegateMulAdd(VA, C0, V);

        vfloat4 VB = v4Abs(VA);

        VC = v4NegateMulAdd(VA, C1, VC);

#if defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        VB = vreinterpretq_f32_u32(vcvtq_u32_f32(VB));
#elif defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        reinterpret_cast<__m128i*>(&VB)[0] = _mm_cvttps_epi32(VB);
#else
        for (size_t i = 0; i < 4; i++)
        {
            VB.v4u[i] = uint32_t(VB.v4f[i]);
        }
#endif

        vfloat4 VC2 = v4Mul(VC, VC);

        vfloat4 T7 = v4wwww(TanCoefficients1.v);
        vfloat4 T6 = v4zzzz(TanCoefficients1.v);
        vfloat4 T4 = v4xxxx(TanCoefficients1.v);
        vfloat4 T3 = v4wwww(TanCoefficients0.v);
        vfloat4 T5 = v4yyyy(TanCoefficients1.v);
        vfloat4 T2 = v4zzzz(TanCoefficients0.v);
        vfloat4 T1 = v4yyyy(TanCoefficients0.v);
        vfloat4 T0 = v4xxxx(TanCoefficients0.v);

        vfloat4 VBIsEven = v4And(VB, Mask.v);
        VBIsEven = v4BinaryEqual(VBIsEven, Zero);

        vfloat4 N = v4MulAdd(VC2, T7, T6);
        vfloat4 D = v4MulAdd(VC2, T4, T3);
        N = v4MulAdd(VC2, N, T5);
        D = v4MulAdd(VC2, D, T2);
        N = v4Mul(VC2, N);
        D = v4MulAdd(VC2, D, T1);
        N = v4MulAdd(VC, N, VC);
        vfloat4 VCNearZero = v4InBoundsAbs(VC, Epsilon);
        D = v4MulAdd(VC2, D, T0);

        N = v4Select(N, VC, VCNearZero);
        D = v4Select(D, Internal::kOne.v, VCNearZero);

        vfloat4 R0 = v4Negate(N);
        vfloat4 R1 = v4Div(N, D);
        R0 = v4Div(D, R0);

        vfloat4 VIsZero = v4Equal(V, Zero);

        vfloat4 Result = v4Select(R0, R1, VBIsEven);

        Result = v4Select(Result, Zero, VIsZero);

        return Result;

#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSinH(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4SinH(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                sinhf(V.v4f[0]),
                sinhf(V.v4f[1]),
                sinhf(V.v4f[2]),
                sinhf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

        vfloat4 V1 = vmlaq_f32(Internal::kNegativeOne.v, V, Scale.v);
        vfloat4 V2 = vmlsq_f32(Internal::kNegativeOne.v, V, Scale.v);
        vfloat4 E1 = v4Exp2(V1);
        vfloat4 E2 = v4Exp2(V2);

        return vsubq_f32(E1, E2);
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_sinh_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

        vfloat4 V1 = VMATH_FMADD_PS(V, Scale, Internal::kNegativeOne);
        vfloat4 V2 = VMATH_FNMADD_PS(V, Scale, Internal::kNegativeOne);
        vfloat4 E1 = v4Exp2(V1);
        vfloat4 E2 = v4Exp2(V2);

        return _mm_sub_ps(E1, E2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorCosH(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4CosH(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                coshf(V.v4f[0]),
                coshf(V.v4f[1]),
                coshf(V.v4f[2]),
                coshf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

        vfloat4 V1 = vmlaq_f32(Internal::kNegativeOne.v, V, Scale.v);
        vfloat4 V2 = vmlsq_f32(Internal::kNegativeOne.v, V, Scale.v);
        vfloat4 E1 = v4Exp2(V1);
        vfloat4 E2 = v4Exp2(V2);
        return vaddq_f32(E1, E2);
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_cosh_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 Scale = { { { 1.442695040888963f, 1.442695040888963f, 1.442695040888963f, 1.442695040888963f } } }; // 1.0f / ln(2.0f)

        vfloat4 V1 = VMATH_FMADD_PS(V, Scale.v, Internal::kNegativeOne.v);
        vfloat4 V2 = VMATH_FNMADD_PS(V, Scale.v, Internal::kNegativeOne.v);
        vfloat4 E1 = v4Exp2(V1);
        vfloat4 E2 = v4Exp2(V2);
        return _mm_add_ps(E1, E2);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorTanH(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4TanH(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                tanhf(V.v4f[0]),
                tanhf(V.v4f[1]),
                tanhf(V.v4f[2]),
                tanhf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

        vfloat4 E = vmulq_f32(V, Scale.v);
        E = v4Exp2(E);
        E = vmlaq_f32(Internal::kOneHalf.v, E, Internal::kOneHalf.v);
        E = v4Rcp(E);
        return vsubq_f32(Internal::kOne.v, E);
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_tanh_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 Scale = { { { 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f, 2.8853900817779268f } } }; // 2.0f / ln(2.0f)

        vfloat4 E = _mm_mul_ps(V, Scale.v);
        E = v4Exp2(E);
        E = VMATH_FMADD_PS(E, Internal::kOneHalf.v, Internal::kOneHalf.v);
        E = _mm_div_ps(Internal::kOne.v, E);
        return _mm_sub_ps(Internal::kOne.v, E);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorASin(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ASin(vfloat4 V)
    {
        // 7-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                asinf(V.v4f[0]),
                asinf(V.v4f[1]),
                asinf(V.v4f[2]),
                asinf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t nonnegative = vcgeq_f32(V, Internal::kZero);
        float32x4_t x = vabsq_f32(V);

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        float32x4_t oneMValue = vsubq_f32(Internal::kOne, x);
        float32x4_t clampOneMValue = vmaxq_f32(Internal::kZero, oneMValue);
        float32x4_t root = v4Sqrt(clampOneMValue);

        // Compute polynomial approximation
        const vfloat4 AC1 = Internal::kArcCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
        vfloat4 t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
        t0 = vmlaq_f32(vConstants, t0, x);

        const vfloat4 AC0 = Internal::kArcCoefficients0;
        vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
        t0 = vmlaq_f32(vConstants, t0, x);
        t0 = vmulq_f32(t0, root);

        float32x4_t t1 = vsubq_f32(Internal::kPi, t0);
        t0 = vbslq_f32(nonnegative, t0, t1);
        t0 = vsubq_f32(Internal::kHalfPi, t0);
        return t0;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_asin_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 nonnegative = _mm_cmpge_ps(V, Internal::kZero);
        __m128 mvalue = _mm_sub_ps(Internal::kZero, V);
        __m128 x = _mm_max_ps(V, mvalue);  // |V|

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        __m128 oneMValue = _mm_sub_ps(Internal::kOne, x);
        __m128 clampOneMValue = _mm_max_ps(Internal::kZero, oneMValue);
        __m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

        // Compute polynomial approximation
        const vfloat4 AC1 = Internal::kArcCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 t0 = VMATH_FMADD_PS(vConstantsB, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        const vfloat4 AC0 = Internal::kArcCoefficients0;
        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);
        t0 = _mm_mul_ps(t0, root);

        __m128 t1 = _mm_sub_ps(Internal::kPi, t0);
        t0 = _mm_and_ps(nonnegative, t0);
        t1 = _mm_andnot_ps(nonnegative, t1);
        t0 = _mm_or_ps(t0, t1);
        t0 = _mm_sub_ps(Internal::kHalfPi, t0);
        return t0;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorACos(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ACos(vfloat4 V)
    {
        // 7-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                acosf(V.v4f[0]),
                acosf(V.v4f[1]),
                acosf(V.v4f[2]),
                acosf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t nonnegative = vcgeq_f32(V, Internal::kZero);
        float32x4_t x = vabsq_f32(V);

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        float32x4_t oneMValue = vsubq_f32(Internal::kOne, x);
        float32x4_t clampOneMValue = vmaxq_f32(Internal::kZero, oneMValue);
        float32x4_t root = v4Sqrt(clampOneMValue);

        // Compute polynomial approximation
        const vfloat4 AC1 = Internal::kArcCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
        vfloat4 t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
        t0 = vmlaq_f32(vConstants, t0, x);

        const vfloat4 AC0 = Internal::kArcCoefficients0;
        vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
        t0 = vmlaq_f32(vConstants, t0, x);
        t0 = vmulq_f32(t0, root);

        float32x4_t t1 = vsubq_f32(Internal::kPi, t0);
        t0 = vbslq_f32(nonnegative, t0, t1);
        return t0;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_acos_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 nonnegative = _mm_cmpge_ps(V, Internal::kZero);
        __m128 mvalue = _mm_sub_ps(Internal::kZero, V);
        __m128 x = _mm_max_ps(V, mvalue);  // |V|

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        __m128 oneMValue = _mm_sub_ps(Internal::kOne, x);
        __m128 clampOneMValue = _mm_max_ps(Internal::kZero, oneMValue);
        __m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

        // Compute polynomial approximation
        const vfloat4 AC1 = Internal::kArcCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 t0 = VMATH_FMADD_PS(vConstantsB, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(1, 1, 1, 1));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC1, _MM_SHUFFLE(0, 0, 0, 0));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        const vfloat4 AC0 = Internal::kArcCoefficients0;
        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(3, 3, 3, 3));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(2, 2, 2, 2));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(1, 1, 1, 1));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AC0, _MM_SHUFFLE(0, 0, 0, 0));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);
        t0 = _mm_mul_ps(t0, root);

        __m128 t1 = _mm_sub_ps(Internal::kPi, t0);
        t0 = _mm_and_ps(nonnegative, t0);
        t1 = _mm_andnot_ps(nonnegative, t1);
        t0 = _mm_or_ps(t0, t1);
        return t0;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorATan(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ATan(vfloat4 V)
    {
        // 17-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                atanf(V.v4f[0]),
                atanf(V.v4f[1]),
                atanf(V.v4f[2]),
                atanf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t absV = vabsq_f32(V);
        float32x4_t invV = v4Rcp(V);
        uint32x4_t comp = vcgtq_f32(V, Internal::kOne);
        float32x4_t sign = vbslq_f32(comp, Internal::kOne, Internal::kNegativeOne);
        comp = vcleq_f32(absV, Internal::kOne);
        sign = vbslq_f32(comp, Internal::kZero, sign);
        float32x4_t x = vbslq_f32(comp, V, invV);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation
        const vfloat4 TC1 = Internal::kATanCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(TC1), 0);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(TC1), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(TC1), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(TC1), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        const vfloat4 TC0 = Internal::kATanCoefficients0;
        vConstants = vdupq_lane_f32(vget_high_f32(TC0), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_high_f32(TC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(TC0), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(TC0), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        Result = vmulq_f32(Result, x);

        float32x4_t result1 = vmulq_f32(sign, Internal::kHalfPi);
        result1 = vsubq_f32(result1, Result);

        comp = vceqq_f32(sign, Internal::kZero);
        Result = vbslq_f32(comp, Result, result1);
        return Result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_atan_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 absV = v4Abs(V);
        __m128 invV = _mm_div_ps(Internal::kOne, V);
        __m128 comp = _mm_cmpgt_ps(V, Internal::kOne);
        __m128 select0 = _mm_and_ps(comp, Internal::kOne);
        __m128 select1 = _mm_andnot_ps(comp, Internal::kNegativeOne);
        __m128 sign = _mm_or_ps(select0, select1);
        comp = _mm_cmple_ps(absV, Internal::kOne);
        select0 = _mm_and_ps(comp, Internal::kZero);
        select1 = _mm_andnot_ps(comp, sign);
        sign = _mm_or_ps(select0, select1);
        select0 = _mm_and_ps(comp, V);
        select1 = _mm_andnot_ps(comp, invV);
        __m128 x = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation
        const vfloat4 TC1 = Internal::kATanCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(TC1, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        const vfloat4 TC0 = Internal::kATanCoefficients0;
        vConstants = VMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(3, 3, 3, 3));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(2, 2, 2, 2));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(TC0, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);

        Result = _mm_mul_ps(Result, x);
        __m128 result1 = _mm_mul_ps(sign, Internal::kHalfPi);
        result1 = _mm_sub_ps(result1, Result);

        comp = _mm_cmpeq_ps(sign, Internal::kZero);
        select0 = _mm_and_ps(comp, Result);
        select1 = _mm_andnot_ps(comp, result1);
        Result = _mm_or_ps(select0, select1);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorATan2(FXMVECTOR Y, FXMVECTOR X)
    inline vfloat4 VMATH_CALLCONV v4ATan2(vfloat4 Y, vfloat4 X)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                atan2f(Y.v4f[0], X.v4f[0]),
                atan2f(Y.v4f[1], X.v4f[1]),
                atan2f(Y.v4f[2], X.v4f[2]),
                atan2f(Y.v4f[3], X.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_atan2_ps(Y, X);
        return Result;
#else

        // Return the inverse tangent of Y / X in the range of -Pi to Pi with the following exceptions:

        //     Y == 0 and X is Negative         -> Pi with the sign of Y
        //     y == 0 and x is positive         -> 0 with the sign of y
        //     Y != 0 and X == 0                -> Pi / 2 with the sign of Y
        //     Y != 0 and X is Negative         -> atan(y/x) + (PI with the sign of Y)
        //     X == -Infinity and Finite Y      -> Pi with the sign of Y
        //     X == +Infinity and Finite Y      -> 0 with the sign of Y
        //     Y == Infinity and X is Finite    -> Pi / 2 with the sign of Y
        //     Y == Infinity and X == -Infinity -> 3Pi / 4 with the sign of Y
        //     Y == Infinity and X == +Infinity -> Pi / 4 with the sign of Y

        static const Internal::vfloat4f32 ATan2Constants = { { { Pi, (Pi / 2), (Pi / 4), Pi * 3.0f / 4.0f } } };

        vfloat4 Zero = v4Zero();
        vfloat4 ATanResultValid = v4AllBitsOne();

        vfloat4 Pi = v4xxxx(ATan2Constants);
        vfloat4 PiOverTwo = v4yyyy(ATan2Constants);
        vfloat4 PiOverFour = v4zzzz(ATan2Constants);
        vfloat4 ThreePiOverFour = v4wwww(ATan2Constants);

        vfloat4 YEqualsZero = v4Equal(Y, Zero);
        vfloat4 XEqualsZero = v4Equal(X, Zero);
        vfloat4 XIsPositive = v4And(X, Internal::kNegativeZero.v);
        XIsPositive = v4BinaryEqual(XIsPositive, Zero);
        vfloat4 YEqualsInfinity = v4IsInfinite(Y);
        vfloat4 XEqualsInfinity = v4IsInfinite(X);

        vfloat4 YSign = v4And(Y, Internal::kNegativeZero.v);
        Pi = v4Or(Pi, YSign);
        PiOverTwo = v4Or(PiOverTwo, YSign);
        PiOverFour = v4Or(PiOverFour, YSign);
        ThreePiOverFour = v4Or(ThreePiOverFour, YSign);

        vfloat4 R1 = v4Select(Pi, YSign, XIsPositive);
        vfloat4 R2 = v4Select(ATanResultValid, PiOverTwo, XEqualsZero);
        vfloat4 R3 = v4Select(R2, R1, YEqualsZero);
        vfloat4 R4 = v4Select(ThreePiOverFour, PiOverFour, XIsPositive);
        vfloat4 R5 = v4Select(PiOverTwo, R4, XEqualsInfinity);
        vfloat4 Result = v4Select(R3, R5, YEqualsInfinity);
        ATanResultValid = v4BinaryEqual(Result, ATanResultValid);

        vfloat4 V = v4Div(Y, X);

        vfloat4 R0 = v4ATan(V);

        R1 = v4Select(Pi, Internal::kNegativeZero, XIsPositive);
        R2 = v4Add(R0, R1);

        return v4Select(Result, R2, ATanResultValid);

#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorSinEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4SinEst(vfloat4 V)
    {
        // 7-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                sinf(V.v4f[0]),
                sinf(V.v4f[1]),
                sinf(V.v4f[2]),
                sinf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with sin(y) = sin(x).
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), Internal::kNegativeZero);
        uint32x4_t c = vorrq_u32(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        float32x4_t absx = vabsq_f32(x);
        float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
        uint32x4_t comp = vcleq_f32(absx, Internal::kHalfPi);
        x = vbslq_f32(comp, x, rflx);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation
        const vfloat4 SEC = Internal::kSinCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        Result = vmulq_f32(Result, x);
        return Result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_sin_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with sin(y) = sin(x).
        __m128 sign = _mm_and_ps(x, Internal::kNegativeZero);
        __m128 c = _mm_or_ps(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        __m128 absx = _mm_andnot_ps(sign, x);  // |x|
        __m128 rflx = _mm_sub_ps(c, x);
        __m128 comp = _mm_cmple_ps(absx, Internal::kHalfPi);
        __m128 select0 = _mm_and_ps(comp, x);
        __m128 select1 = _mm_andnot_ps(comp, rflx);
        x = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation
        const vfloat4 SEC = Internal::kSinCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);
        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, x);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorCosEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4CosEst(vfloat4 V)
    {
        // 6-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                cosf(V.v4f[0]),
                cosf(V.v4f[1]),
                cosf(V.v4f[2]),
                cosf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
    // Map V to x in [-pi,pi].
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), Internal::kNegativeZero);
        uint32x4_t c = vorrq_u32(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        float32x4_t absx = vabsq_f32(x);
        float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
        uint32x4_t comp = vcleq_f32(absx, Internal::kHalfPi);
        x = vbslq_f32(comp, x, rflx);
        float32x4_t fsign = vbslq_f32(comp, Internal::kOne, Internal::kNegativeOne);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation
        const vfloat4 CEC = Internal::kCosCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        Result = vmulq_f32(Result, fsign);
        return Result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_cos_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
    // Map V to x in [-pi,pi].
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
        vfloat4 sign = _mm_and_ps(x, Internal::kNegativeZero);
        __m128 c = _mm_or_ps(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        __m128 absx = _mm_andnot_ps(sign, x);  // |x|
        __m128 rflx = _mm_sub_ps(c, x);
        __m128 comp = _mm_cmple_ps(absx, Internal::kHalfPi);
        __m128 select0 = _mm_and_ps(comp, x);
        __m128 select1 = _mm_andnot_ps(comp, rflx);
        x = _mm_or_ps(select0, select1);
        select0 = _mm_and_ps(comp, Internal::kOne);
        select1 = _mm_andnot_ps(comp, Internal::kNegativeOne);
        sign = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation
        const vfloat4 CEC = Internal::kCosCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);
        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, sign);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // void XMVectorSinCosEst(XMVECTOR* pSin, XMVECTOR* pCos, FXMVECTOR  V)
    inline void VMATH_CALLCONV v4SinCosEst(vfloat4* pSin, vfloat4* pCos, vfloat4  V)
    {
        assert(pSin != nullptr);
        assert(pCos != nullptr);

        // 7/6-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Sin = { { {
                sinf(V.v4f[0]),
                sinf(V.v4f[1]),
                sinf(V.v4f[2]),
                sinf(V.v4f[3])
            } } };

        Internal::vfloat4f32 Cos = { { {
                cosf(V.v4f[0]),
                cosf(V.v4f[1]),
                cosf(V.v4f[2]),
                cosf(V.v4f[3])
            } } };

        *pSin = Sin.v;
        *pCos = Cos.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
        uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), Internal::kNegativeZero);
        uint32x4_t c = vorrq_u32(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        float32x4_t absx = vabsq_f32(x);
        float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
        uint32x4_t comp = vcleq_f32(absx, Internal::kHalfPi);
        x = vbslq_f32(comp, x, rflx);
        float32x4_t fsign = vbslq_f32(comp, Internal::kOne, Internal::kNegativeOne);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation for sine
        const vfloat4 SEC = Internal::kSinCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(SEC), 0);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(SEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(SEC), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        *pSin = vmulq_f32(Result, x);

        // Compute polynomial approximation
        const vfloat4 CEC = Internal::kCosCoefficients1;
        vConstants = vdupq_lane_f32(vget_high_f32(CEC), 0);
        Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(CEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(CEC), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        Result = vmlaq_f32(Internal::kOne, Result, x2);
        *pCos = vmulq_f32(Result, fsign);
#elif defined(VMATH_SSE_INTRINSICS)
    // Force the value within the bounds of pi
        vfloat4 x = v4ModAngles(V);

        // Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
        vfloat4 sign = _mm_and_ps(x, Internal::kNegativeZero);
        __m128 c = _mm_or_ps(Internal::kPi, sign);  // pi when x >= 0, -pi when x < 0
        __m128 absx = _mm_andnot_ps(sign, x);  // |x|
        __m128 rflx = _mm_sub_ps(c, x);
        __m128 comp = _mm_cmple_ps(absx, Internal::kHalfPi);
        __m128 select0 = _mm_and_ps(comp, x);
        __m128 select1 = _mm_andnot_ps(comp, rflx);
        x = _mm_or_ps(select0, select1);
        select0 = _mm_and_ps(comp, Internal::kOne);
        select1 = _mm_andnot_ps(comp, Internal::kNegativeOne);
        sign = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation for sine
        const vfloat4 SEC = Internal::kSinCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(SEC, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);
        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, x);
        *pSin = Result;

        // Compute polynomial approximation for cosine
        const vfloat4 CEC = Internal::kCosCoefficients1;
        vConstantsB = VMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(3, 3, 3, 3));
        vConstants = VMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(2, 2, 2, 2));
        Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(CEC, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);
        Result = VMATH_FMADD_PS(Result, x2, Internal::kOne);
        Result = _mm_mul_ps(Result, sign);
        *pCos = Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorTanEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4TanEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                tanf(V.v4f[0]),
                tanf(V.v4f[1]),
                tanf(V.v4f[2]),
                tanf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_tan_ps(V);
        return Result;
#else

        vfloat4 OneOverPi = v4wwww(Internal::kTanEstCoefficients.v);

        vfloat4 V1 = v4Mul(V, OneOverPi);
        V1 = v4Round(V1);

        V1 = v4NegateMulAdd(Internal::kPi.v, V1, V);

        vfloat4 T0 = v4xxxx(Internal::kTanEstCoefficients.v);
        vfloat4 T1 = v4yyyy(Internal::kTanEstCoefficients.v);
        vfloat4 T2 = v4zzzz(Internal::kTanEstCoefficients.v);

        vfloat4 V2T2 = v4NegateMulAdd(V1, V1, T2);
        vfloat4 V2 = v4Mul(V1, V1);
        vfloat4 V1T0 = v4Mul(V1, T0);
        vfloat4 V1T1 = v4Mul(V1, T1);

        vfloat4 D = v4RcpEst(V2T2);
        vfloat4 N = v4MulAdd(V2, V1T1, V1T0);

        return v4Mul(N, D);

#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorASinEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ASinEst(vfloat4 V)
    {
        // 3-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result;
        Result.f[0] = asinf(V.v4f[0]);
        Result.f[1] = asinf(V.v4f[1]);
        Result.f[2] = asinf(V.v4f[2]);
        Result.f[3] = asinf(V.v4f[3]);
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t nonnegative = vcgeq_f32(V, Internal::kZero);
        float32x4_t x = vabsq_f32(V);

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        float32x4_t oneMValue = vsubq_f32(Internal::kOne, x);
        float32x4_t clampOneMValue = vmaxq_f32(Internal::kZero, oneMValue);
        float32x4_t root = v4Sqrt(clampOneMValue);

        // Compute polynomial approximation
        const vfloat4 AEC = Internal::kArcEstCoefficients;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
        vfloat4 t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
        t0 = vmlaq_f32(vConstants, t0, x);
        t0 = vmulq_f32(t0, root);

        float32x4_t t1 = vsubq_f32(Internal::kPi, t0);
        t0 = vbslq_f32(nonnegative, t0, t1);
        t0 = vsubq_f32(Internal::kHalfPi, t0);
        return t0;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_asin_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 nonnegative = _mm_cmpge_ps(V, Internal::kZero);
        __m128 mvalue = _mm_sub_ps(Internal::kZero, V);
        __m128 x = _mm_max_ps(V, mvalue);  // |V|

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        __m128 oneMValue = _mm_sub_ps(Internal::kOne, x);
        __m128 clampOneMValue = _mm_max_ps(Internal::kZero, oneMValue);
        __m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

        // Compute polynomial approximation
        const vfloat4 AEC = Internal::kArcEstCoefficients;
        __m128 vConstantsB = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 t0 = VMATH_FMADD_PS(vConstantsB, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);
        t0 = _mm_mul_ps(t0, root);

        __m128 t1 = _mm_sub_ps(Internal::kPi, t0);
        t0 = _mm_and_ps(nonnegative, t0);
        t1 = _mm_andnot_ps(nonnegative, t1);
        t0 = _mm_or_ps(t0, t1);
        t0 = _mm_sub_ps(Internal::kHalfPi, t0);
        return t0;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorACosEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ACosEst(vfloat4 V)
    {
        // 3-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                acosf(V.v4f[0]),
                acosf(V.v4f[1]),
                acosf(V.v4f[2]),
                acosf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t nonnegative = vcgeq_f32(V, Internal::kZero);
        float32x4_t x = vabsq_f32(V);

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        float32x4_t oneMValue = vsubq_f32(Internal::kOne, x);
        float32x4_t clampOneMValue = vmaxq_f32(Internal::kZero, oneMValue);
        float32x4_t root = v4Sqrt(clampOneMValue);

        // Compute polynomial approximation
        const vfloat4 AEC = Internal::kArcEstCoefficients;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
        vfloat4 t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
        t0 = vmlaq_f32(vConstants, t0, x);

        vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
        t0 = vmlaq_f32(vConstants, t0, x);
        t0 = vmulq_f32(t0, root);

        float32x4_t t1 = vsubq_f32(Internal::kPi, t0);
        t0 = vbslq_f32(nonnegative, t0, t1);
        return t0;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_acos_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 nonnegative = _mm_cmpge_ps(V, Internal::kZero);
        __m128 mvalue = _mm_sub_ps(Internal::kZero, V);
        __m128 x = _mm_max_ps(V, mvalue);  // |V|

        // Compute (1-|V|), clamp to zero to avoid sqrt of negative number.
        __m128 oneMValue = _mm_sub_ps(Internal::kOne, x);
        __m128 clampOneMValue = _mm_max_ps(Internal::kZero, oneMValue);
        __m128 root = _mm_sqrt_ps(clampOneMValue);  // sqrt(1-|V|)

        // Compute polynomial approximation
        const vfloat4 AEC = Internal::kArcEstCoefficients;
        __m128 vConstantsB = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 t0 = VMATH_FMADD_PS(vConstantsB, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);

        vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
        t0 = VMATH_FMADD_PS(t0, x, vConstants);
        t0 = _mm_mul_ps(t0, root);

        __m128 t1 = _mm_sub_ps(Internal::kPi, t0);
        t0 = _mm_and_ps(nonnegative, t0);
        t1 = _mm_andnot_ps(nonnegative, t1);
        t0 = _mm_or_ps(t0, t1);
        return t0;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorATanEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4ATanEst(vfloat4 V)
    {
        // 9-degree minimax approximation

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                atanf(V.v4f[0]),
                atanf(V.v4f[1]),
                atanf(V.v4f[2]),
                atanf(V.v4f[3])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t absV = vabsq_f32(V);
        float32x4_t invV = v4RcpEst(V);
        uint32x4_t comp = vcgtq_f32(V, Internal::kOne);
        float32x4_t sign = vbslq_f32(comp, Internal::kOne, Internal::kNegativeOne);
        comp = vcleq_f32(absV, Internal::kOne);
        sign = vbslq_f32(comp, Internal::kZero, sign);
        float32x4_t x = vbslq_f32(comp, V, invV);

        float32x4_t x2 = vmulq_f32(x, x);

        // Compute polynomial approximation
        const vfloat4 AEC = Internal::kATanEstCoefficients1;
        vfloat4 vConstants = vdupq_lane_f32(vget_high_f32(AEC), 0);
        vfloat4 Result = vmlaq_lane_f32(vConstants, x2, vget_high_f32(AEC), 1);

        vConstants = vdupq_lane_f32(vget_low_f32(AEC), 1);
        Result = vmlaq_f32(vConstants, Result, x2);

        vConstants = vdupq_lane_f32(vget_low_f32(AEC), 0);
        Result = vmlaq_f32(vConstants, Result, x2);

        // ATanEstCoefficients0 is already splatted
        Result = vmlaq_f32(Internal::kATanEstCoefficients0, Result, x2);
        Result = vmulq_f32(Result, x);

        float32x4_t result1 = vmulq_f32(sign, Internal::kHalfPi);
        result1 = vsubq_f32(result1, Result);

        comp = vceqq_f32(sign, Internal::kZero);
        Result = vbslq_f32(comp, Result, result1);
        return Result;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_atan_ps(V);
        return Result;
#elif defined(VMATH_SSE_INTRINSICS)
        __m128 absV = v4Abs(V);
        __m128 invV = _mm_div_ps(Internal::kOne, V);
        __m128 comp = _mm_cmpgt_ps(V, Internal::kOne);
        __m128 select0 = _mm_and_ps(comp, Internal::kOne);
        __m128 select1 = _mm_andnot_ps(comp, Internal::kNegativeOne);
        __m128 sign = _mm_or_ps(select0, select1);
        comp = _mm_cmple_ps(absV, Internal::kOne);
        select0 = _mm_and_ps(comp, Internal::kZero);
        select1 = _mm_andnot_ps(comp, sign);
        sign = _mm_or_ps(select0, select1);
        select0 = _mm_and_ps(comp, V);
        select1 = _mm_andnot_ps(comp, invV);
        __m128 x = _mm_or_ps(select0, select1);

        __m128 x2 = _mm_mul_ps(x, x);

        // Compute polynomial approximation
        const vfloat4 AEC = Internal::kATanEstCoefficients1;
        __m128 vConstantsB = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(3, 3, 3, 3));
        __m128 vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Result = VMATH_FMADD_PS(vConstantsB, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(1, 1, 1, 1));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);

        vConstants = VMATH_PERMUTE_PS(AEC, _MM_SHUFFLE(0, 0, 0, 0));
        Result = VMATH_FMADD_PS(Result, x2, vConstants);
        // ATanEstCoefficients0 is already splatted
        Result = VMATH_FMADD_PS(Result, x2, Internal::kATanEstCoefficients0);
        Result = _mm_mul_ps(Result, x);
        __m128 result1 = _mm_mul_ps(sign, Internal::kHalfPi);
        result1 = _mm_sub_ps(result1, Result);

        comp = _mm_cmpeq_ps(sign, Internal::kZero);
        select0 = _mm_and_ps(comp, Result);
        select1 = _mm_andnot_ps(comp, result1);
        Result = _mm_or_ps(select0, select1);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorATan2Est(FXMVECTOR Y, FXMVECTOR X)
    inline vfloat4 VMATH_CALLCONV v4ATan2Est(vfloat4 Y, vfloat4 X)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                atan2f(Y.v4f[0], X.v4f[0]),
                atan2f(Y.v4f[1], X.v4f[1]),
                atan2f(Y.v4f[2], X.v4f[2]),
                atan2f(Y.v4f[3], X.v4f[3]),
            } } };
        return Result.v;
#elif defined(VMATH_SVML_INTRINSICS)
        vfloat4 Result = _mm_atan2_ps(Y, X);
        return Result;
#else

        static const Internal::vfloat4f32 ATan2Constants = { { { Pi, (Pi / 2), (Pi / 4), 2.3561944905f /* Pi*3/4 */ } } };

        const vfloat4 Zero = v4Zero();
        vfloat4 ATanResultValid = v4AllBitsOne();

        vfloat4 Pi = v4xxxx(ATan2Constants);
        vfloat4 PiOverTwo = v4yyyy(ATan2Constants);
        vfloat4 PiOverFour = v4zzzz(ATan2Constants);
        vfloat4 ThreePiOverFour = v4wwww(ATan2Constants);

        vfloat4 YEqualsZero = v4Equal(Y, Zero);
        vfloat4 XEqualsZero = v4Equal(X, Zero);
        vfloat4 XIsPositive = v4And(X, Internal::kNegativeZero.v);
        XIsPositive = v4BinaryEqual(XIsPositive, Zero);
        vfloat4 YEqualsInfinity = v4IsInfinite(Y);
        vfloat4 XEqualsInfinity = v4IsInfinite(X);

        vfloat4 YSign = v4And(Y, Internal::kNegativeZero.v);
        Pi = v4Or(Pi, YSign);
        PiOverTwo = v4Or(PiOverTwo, YSign);
        PiOverFour = v4Or(PiOverFour, YSign);
        ThreePiOverFour = v4Or(ThreePiOverFour, YSign);

        vfloat4 R1 = v4Select(Pi, YSign, XIsPositive);
        vfloat4 R2 = v4Select(ATanResultValid, PiOverTwo, XEqualsZero);
        vfloat4 R3 = v4Select(R2, R1, YEqualsZero);
        vfloat4 R4 = v4Select(ThreePiOverFour, PiOverFour, XIsPositive);
        vfloat4 R5 = v4Select(PiOverTwo, R4, XEqualsInfinity);
        vfloat4 Result = v4Select(R3, R5, YEqualsInfinity);
        ATanResultValid = v4BinaryEqual(Result, ATanResultValid);

        vfloat4 Reciprocal = v4RcpEst(X);
        vfloat4 V = v4Mul(Y, Reciprocal);
        vfloat4 R0 = v4ATanEst(V);

        R1 = v4Select(Pi, Internal::kNegativeZero, XIsPositive);
        R2 = v4Add(R0, R1);

        Result = v4Select(Result, R2, ATanResultValid);

        return Result;

#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorLerp(FXMVECTOR V0, FXMVECTOR V1, float t)
    inline vfloat4 VMATH_CALLCONV v4Lerp(vfloat4 V0, vfloat4 V1, float t)
    {
        // V0 + t * (V1 - V0)

#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Scale = v4Spread(t);
        vfloat4 Length = v4Sub(V1, V0);
        return v4MulAdd(Length, Scale, V0);

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 L = vsubq_f32(V1, V0);
        return vmlaq_n_f32(V0, L, t);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 L = _mm_sub_ps(V1, V0);
        vfloat4 S = _mm_set_ps1(t);
        return VMATH_FMADD_PS(L, S, V0);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorLerpV(FXMVECTOR V0, FXMVECTOR V1, FXMVECTOR T)
    inline vfloat4 VMATH_CALLCONV v4Lerp(vfloat4 V0, vfloat4 V1, vfloat4 T)
    {
        // V0 + T * (V1 - V0)

#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Length = v4Sub(V1, V0);
        return v4MulAdd(Length, T, V0);

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 L = vsubq_f32(V1, V0);
        return vmlaq_f32(V0, L, T);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 Length = _mm_sub_ps(V1, V0);
        return VMATH_FMADD_PS(Length, T, V0);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorHermite(FXMVECTOR Position0, FXMVECTOR Tangent0, FXMVECTOR Position1, GXMVECTOR Tangent1, float t)
    inline vfloat4 VMATH_CALLCONV v4Hermite(vfloat4 Position0, vfloat4 Tangent0, vfloat4 Position1, vfloat4 Tangent1, float t)
    {
        // Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
        //          (t^3 - 2 * t^2 + t) * Tangent0 +
        //          (-2 * t^3 + 3 * t^2) * Position1 +
        //          (t^3 - t^2) * Tangent1

#if defined(VMATH_NO_INTRINSICS)

        float t2 = t * t;
        float t3 = t * t2;

        vfloat4 P0 = v4Spread(2.0f * t3 - 3.0f * t2 + 1.0f);
        vfloat4 T0 = v4Spread(t3 - 2.0f * t2 + t);
        vfloat4 P1 = v4Spread(-2.0f * t3 + 3.0f * t2);
        vfloat4 T1 = v4Spread(t3 - t2);

        vfloat4 Result = v4Mul(P0, Position0);
        Result = v4MulAdd(T0, Tangent0, Result);
        Result = v4MulAdd(P1, Position1, Result);
        Result = v4MulAdd(T1, Tangent1, Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float t2 = t * t;
        float t3 = t * t2;

        float p0 = 2.0f * t3 - 3.0f * t2 + 1.0f;
        float t0 = t3 - 2.0f * t2 + t;
        float p1 = -2.0f * t3 + 3.0f * t2;
        float t1 = t3 - t2;

        vfloat4 vResult = vmulq_n_f32(Position0, p0);
        vResult = vmlaq_n_f32(vResult, Tangent0, t0);
        vResult = vmlaq_n_f32(vResult, Position1, p1);
        vResult = vmlaq_n_f32(vResult, Tangent1, t1);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        float t2 = t * t;
        float t3 = t * t2;

        vfloat4 P0 = _mm_set_ps1(2.0f * t3 - 3.0f * t2 + 1.0f);
        vfloat4 T0 = _mm_set_ps1(t3 - 2.0f * t2 + t);
        vfloat4 P1 = _mm_set_ps1(-2.0f * t3 + 3.0f * t2);
        vfloat4 T1 = _mm_set_ps1(t3 - t2);

        vfloat4 vResult = _mm_mul_ps(P0, Position0);
        vResult = VMATH_FMADD_PS(Tangent0, T0, vResult);
        vResult = VMATH_FMADD_PS(Position1, P1, vResult);
        vResult = VMATH_FMADD_PS(Tangent1, T1, vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorHermiteV(FXMVECTOR Position0, FXMVECTOR Tangent0, FXMVECTOR Position1, GXMVECTOR Tangent1, HXMVECTOR T)
    inline vfloat4 VMATH_CALLCONV v4Hermite(vfloat4 Position0, vfloat4 Tangent0, vfloat4 Position1, vfloat4 Tangent1, vfloat4 T)
    {
        // Result = (2 * t^3 - 3 * t^2 + 1) * Position0 +
        //          (t^3 - 2 * t^2 + t) * Tangent0 +
        //          (-2 * t^3 + 3 * t^2) * Position1 +
        //          (t^3 - t^2) * Tangent1

#if defined(VMATH_NO_INTRINSICS)

        vfloat4 T2 = v4Mul(T, T);
        vfloat4 T3 = v4Mul(T, T2);

        vfloat4 P0 = v4Spread(2.0f * T3.v4f[0] - 3.0f * T2.v4f[0] + 1.0f);
        vfloat4 T0 = v4Spread(T3.v4f[1] - 2.0f * T2.v4f[1] + T.v4f[1]);
        vfloat4 P1 = v4Spread(-2.0f * T3.v4f[2] + 3.0f * T2.v4f[2]);
        vfloat4 T1 = v4Spread(T3.v4f[3] - T2.v4f[3]);

        vfloat4 Result = v4Mul(P0, Position0);
        Result = v4MulAdd(T0, Tangent0, Result);
        Result = v4MulAdd(P1, Position1, Result);
        Result = v4MulAdd(T1, Tangent1, Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
        static const Internal::vfloat4f32 CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

        vfloat4 T2 = vmulq_f32(T, T);
        vfloat4 T3 = vmulq_f32(T, T2);
        // Mul by the constants against t^2
        T2 = vmulq_f32(T2, CatMulT2);
        // Mul by the constants against t^3
        T3 = vmlaq_f32(T2, T3, CatMulT3);
        // T3 now has the pre-result.
        // I need to add t.y only
        T2 = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(T), Internal::kMaskY));
        T3 = vaddq_f32(T3, T2);
        // Add 1.0f to x
        T3 = vaddq_f32(T3, Internal::kIdentityR0);
        // Now, I have the constants created
        // Mul the x constant to Position0
        vfloat4 vResult = vmulq_lane_f32(Position0, vget_low_f32(T3), 0); // T3[0]
        // Mul the y constant to Tangent0
        vResult = vmlaq_lane_f32(vResult, Tangent0, vget_low_f32(T3), 1); // T3[1]
        // Mul the z constant to Position1
        vResult = vmlaq_lane_f32(vResult, Position1, vget_high_f32(T3), 0); // T3[2]
        // Mul the w constant to Tangent1
        vResult = vmlaq_lane_f32(vResult, Tangent1, vget_high_f32(T3), 1); // T3[3]
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 CatMulT2 = { { { -3.0f, -2.0f, 3.0f, -1.0f } } };
        static const Internal::vfloat4f32 CatMulT3 = { { { 2.0f, 1.0f, -2.0f, 1.0f } } };

        vfloat4 T2 = _mm_mul_ps(T, T);
        vfloat4 T3 = _mm_mul_ps(T, T2);
        // Mul by the constants against t^2
        T2 = _mm_mul_ps(T2, CatMulT2);
        // Mul by the constants against t^3
        T3 = VMATH_FMADD_PS(T3, CatMulT3, T2);
        // T3 now has the pre-result.
        // I need to add t.y only
        T2 = _mm_and_ps(T, Internal::kMaskY);
        T3 = _mm_add_ps(T3, T2);
        // Add 1.0f to x
        T3 = _mm_add_ps(T3, Internal::kIdentityR0);
        // Now, I have the constants created
        // Mul the x constant to Position0
        vfloat4 vResult = VMATH_PERMUTE_PS(T3, _MM_SHUFFLE(0, 0, 0, 0));
        vResult = _mm_mul_ps(vResult, Position0);
        // Mul the y constant to Tangent0
        T2 = VMATH_PERMUTE_PS(T3, _MM_SHUFFLE(1, 1, 1, 1));
        vResult = VMATH_FMADD_PS(T2, Tangent0, vResult);
        // Mul the z constant to Position1
        T2 = VMATH_PERMUTE_PS(T3, _MM_SHUFFLE(2, 2, 2, 2));
        vResult = VMATH_FMADD_PS(T2, Position1, vResult);
        // Mul the w constant to Tangent1
        T3 = VMATH_PERMUTE_PS(T3, _MM_SHUFFLE(3, 3, 3, 3));
        vResult = VMATH_FMADD_PS(T3, Tangent1, vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorCatmullRom(FXMVECTOR Position0, FXMVECTOR Position1, FXMVECTOR Position2, GXMVECTOR Position3, float t)
    inline vfloat4 VMATH_CALLCONV v4CatmullRom(vfloat4 Position0, vfloat4 Position1, vfloat4 Position2, vfloat4 Position3, float t)
    {
        // Result = ((-t^3 + 2 * t^2 - t) * Position0 +
        //           (3 * t^3 - 5 * t^2 + 2) * Position1 +
        //           (-3 * t^3 + 4 * t^2 + t) * Position2 +
        //           (t^3 - t^2) * Position3) * 0.5

#if defined(VMATH_NO_INTRINSICS)

        float t2 = t * t;
        float t3 = t * t2;

        vfloat4 P0 = v4Spread((-t3 + 2.0f * t2 - t) * 0.5f);
        vfloat4 P1 = v4Spread((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
        vfloat4 P2 = v4Spread((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
        vfloat4 P3 = v4Spread((t3 - t2) * 0.5f);

        vfloat4 Result = v4Mul(P0, Position0);
        Result = v4MulAdd(P1, Position1, Result);
        Result = v4MulAdd(P2, Position2, Result);
        Result = v4MulAdd(P3, Position3, Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float t2 = t * t;
        float t3 = t * t2;

        float p0 = (-t3 + 2.0f * t2 - t) * 0.5f;
        float p1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
        float p2 = (-3.0f * t3 + 4.0f * t2 + t) * 0.5f;
        float p3 = (t3 - t2) * 0.5f;

        vfloat4 P1 = vmulq_n_f32(Position1, p1);
        vfloat4 P0 = vmlaq_n_f32(P1, Position0, p0);
        vfloat4 P3 = vmulq_n_f32(Position3, p3);
        vfloat4 P2 = vmlaq_n_f32(P3, Position2, p2);
        P0 = vaddq_f32(P0, P2);
        return P0;
#elif defined(VMATH_SSE_INTRINSICS)
        float t2 = t * t;
        float t3 = t * t2;

        vfloat4 P0 = _mm_set_ps1((-t3 + 2.0f * t2 - t) * 0.5f);
        vfloat4 P1 = _mm_set_ps1((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f);
        vfloat4 P2 = _mm_set_ps1((-3.0f * t3 + 4.0f * t2 + t) * 0.5f);
        vfloat4 P3 = _mm_set_ps1((t3 - t2) * 0.5f);

        P1 = _mm_mul_ps(Position1, P1);
        P0 = VMATH_FMADD_PS(Position0, P0, P1);
        P3 = _mm_mul_ps(Position3, P3);
        P2 = VMATH_FMADD_PS(Position2, P2, P3);
        P0 = _mm_add_ps(P0, P2);
        return P0;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVectorCatmullRomV(FXMVECTOR Position0, FXMVECTOR Position1, FXMVECTOR Position2, GXMVECTOR Position3, HXMVECTOR T)
    inline vfloat4 VMATH_CALLCONV v4CatmullRom(vfloat4 Position0, vfloat4 Position1, vfloat4 Position2, vfloat4 Position3, vfloat4 T)
    {
#if defined(VMATH_NO_INTRINSICS)
        float fx = T.v4f[0];
        float fy = T.v4f[1];
        float fz = T.v4f[2];
        float fw = T.v4f[3];
        Internal::vfloat4f32 vResult = { { {
                0.5f * ((-fx * fx * fx + 2 * fx * fx - fx) * Position0.v4f[0]
                + (3 * fx * fx * fx - 5 * fx * fx + 2) * Position1.v4f[0]
                + (-3 * fx * fx * fx + 4 * fx * fx + fx) * Position2.v4f[0]
                + (fx * fx * fx - fx * fx) * Position3.v4f[0]),

                0.5f * ((-fy * fy * fy + 2 * fy * fy - fy) * Position0.v4f[1]
                + (3 * fy * fy * fy - 5 * fy * fy + 2) * Position1.v4f[1]
                + (-3 * fy * fy * fy + 4 * fy * fy + fy) * Position2.v4f[1]
                + (fy * fy * fy - fy * fy) * Position3.v4f[1]),

                0.5f * ((-fz * fz * fz + 2 * fz * fz - fz) * Position0.v4f[2]
                + (3 * fz * fz * fz - 5 * fz * fz + 2) * Position1.v4f[2]
                + (-3 * fz * fz * fz + 4 * fz * fz + fz) * Position2.v4f[2]
                + (fz * fz * fz - fz * fz) * Position3.v4f[2]),

                0.5f * ((-fw * fw * fw + 2 * fw * fw - fw) * Position0.v4f[3]
                + (3 * fw * fw * fw - 5 * fw * fw + 2) * Position1.v4f[3]
                + (-3 * fw * fw * fw + 4 * fw * fw + fw) * Position2.v4f[3]
                + (fw * fw * fw - fw * fw) * Position3.v4f[3])
            } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
        static const Internal::vfloat4f32 Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
        static const Internal::vfloat4f32 Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
        static const Internal::vfloat4f32 Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
        // Cache T^2 and T^3
        vfloat4 T2 = vmulq_f32(T, T);
        vfloat4 T3 = vmulq_f32(T, T2);
        // Perform the Position0 term
        vfloat4 vResult = vaddq_f32(T2, T2);
        vResult = vsubq_f32(vResult, T);
        vResult = vsubq_f32(vResult, T3);
        vResult = vmulq_f32(vResult, Position0);
        // Perform the Position1 term and add
        vfloat4 vTemp = vmulq_f32(T3, Catmul3);
        vTemp = vmlsq_f32(vTemp, T2, Catmul5);
        vTemp = vaddq_f32(vTemp, Catmul2);
        vResult = vmlaq_f32(vResult, vTemp, Position1);
        // Perform the Position2 term and add
        vTemp = vmulq_f32(T2, Catmul4);
        vTemp = vmlsq_f32(vTemp, T3, Catmul3);
        vTemp = vaddq_f32(vTemp, T);
        vResult = vmlaq_f32(vResult, vTemp, Position2);
        // Position3 is the last term
        T3 = vsubq_f32(T3, T2);
        vResult = vmlaq_f32(vResult, T3, Position3);
        // Multiply by 0.5f and exit
        vResult = vmulq_f32(vResult, Internal::kOneHalf);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 Catmul2 = { { { 2.0f, 2.0f, 2.0f, 2.0f } } };
        static const Internal::vfloat4f32 Catmul3 = { { { 3.0f, 3.0f, 3.0f, 3.0f } } };
        static const Internal::vfloat4f32 Catmul4 = { { { 4.0f, 4.0f, 4.0f, 4.0f } } };
        static const Internal::vfloat4f32 Catmul5 = { { { 5.0f, 5.0f, 5.0f, 5.0f } } };
        // Cache T^2 and T^3
        vfloat4 T2 = _mm_mul_ps(T, T);
        vfloat4 T3 = _mm_mul_ps(T, T2);
        // Perform the Position0 term
        vfloat4 vResult = _mm_add_ps(T2, T2);
        vResult = _mm_sub_ps(vResult, T);
        vResult = _mm_sub_ps(vResult, T3);
        vResult = _mm_mul_ps(vResult, Position0);
        // Perform the Position1 term and add
        vfloat4 vTemp = _mm_mul_ps(T3, Catmul3);
        vTemp = VMATH_FNMADD_PS(T2, Catmul5, vTemp);
        vTemp = _mm_add_ps(vTemp, Catmul2);
        vResult = VMATH_FMADD_PS(vTemp, Position1, vResult);
        // Perform the Position2 term and add
        vTemp = _mm_mul_ps(T2, Catmul4);
        vTemp = VMATH_FNMADD_PS(T3, Catmul3, vTemp);
        vTemp = _mm_add_ps(vTemp, T);
        vResult = VMATH_FMADD_PS(vTemp, Position2, vResult);
        // Position3 is the last term
        T3 = _mm_sub_ps(T3, T2);
        vResult = VMATH_FMADD_PS(T3, Position3, vResult);
        // Multiply by 0.5f and exit
        vResult = _mm_mul_ps(vResult, Internal::kOneHalf);
        return vResult;
#endif
    }

    /****************************************************************************
     *
     * 2D Vector
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------
     // Comparison operations
     //------------------------------------------------------------------------------

     //------------------------------------------------------------------------------

    // bool XMVector2Equal(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AllEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] == V2.v4f[0]) && (V1.v4f[1] == V2.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpeq_ps(V1, V2);
        // z and w are don't care
        return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2EqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AllBinaryEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4u[0] == V2.v4u[0]) && (V1.v4u[1] == V2.v4u[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vceq_u32(vget_low_u32(vreinterpretq_u32_f32(V1)), vget_low_u32(vreinterpretq_u32_f32(V2)));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) == 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2NearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon)
    inline bool VMATH_CALLCONV v2AllNearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon)
    {
#if defined(VMATH_NO_INTRINSICS)
        float dx = fabsf(V1.v4f[0] - V2.v4f[0]);
        float dy = fabsf(V1.v4f[1] - V2.v4f[1]);
        return ((dx <= Epsilon.v4f[0]) &&
            (dy <= Epsilon.v4f[1]));
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t vDelta = vsub_f32(vget_low_f32(V1), vget_low_f32(V2));
#if defined(_MSC_VER) && !defined(__clang__)
        uint32x2_t vTemp = vacle_f32(vDelta, vget_low_u32(Epsilon));
#else
        uint32x2_t vTemp = vcle_f32(vabs_f32(vDelta), vget_low_f32(Epsilon));
#endif
        uint64_t r = vget_lane_u64(vreinterpret_u64_u32(vTemp), 0);
        return (r == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Get the difference
        vfloat4 vDelta = _mm_sub_ps(V1, V2);
        // Get the absolute value of the difference
        vfloat4 vTemp = _mm_setzero_ps();
        vTemp = _mm_sub_ps(vTemp, vDelta);
        vTemp = _mm_max_ps(vTemp, vDelta);
        vTemp = _mm_cmple_ps(vTemp, Epsilon);
        // z and w are don't care
        return (((_mm_movemask_ps(vTemp) & 3) == 0x3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2NotEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AnyNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] != V2.v4f[0]) || (V1.v4f[1] != V2.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vceq_f32(vget_low_f32(V1), vget_low_f32(V2));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpeq_ps(V1, V2);
        // z and w are don't care
        return (((_mm_movemask_ps(vTemp) & 3) != 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2NotEqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AnyBinaryNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4u[0] != V2.v4u[0]) || (V1.v4u[1] != V2.v4u[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vceq_u32(vget_low_u32(vreinterpretq_u32_f32(V1)), vget_low_u32(vreinterpretq_u32_f32(V2)));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 3) != 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2Greater(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AllGreater(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] > V2.v4f[0]) && (V1.v4f[1] > V2.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vcgt_f32(vget_low_f32(V1), vget_low_f32(V2));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpgt_ps(V1, V2);
        // z and w are don't care
        return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2GreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AllGreaterEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] >= V2.v4f[0]) && (V1.v4f[1] >= V2.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vcge_f32(vget_low_f32(V1), vget_low_f32(V2));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpge_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2Less(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AllLess(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] < V2.v4f[0]) && (V1.v4f[1] < V2.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vclt_f32(vget_low_f32(V1), vget_low_f32(V2));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmplt_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2LessOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v2AllLessEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] <= V2.v4f[0]) && (V1.v4f[1] <= V2.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x2_t vTemp = vcle_f32(vget_low_f32(V1), vget_low_f32(V2));
        return (vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmple_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 3) == 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector2InBounds(FXMVECTOR V, FXMVECTOR Bounds)
    inline bool VMATH_CALLCONV v2AllInBoundsAbs(vfloat4 V, vfloat4 Bounds)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V.v4f[0] <= Bounds.v4f[0] && V.v4f[0] >= -Bounds.v4f[0]) &&
            (V.v4f[1] <= Bounds.v4f[1] && V.v4f[1] >= -Bounds.v4f[1])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        float32x2_t B = vget_low_f32(Bounds);
        // Test if less than or equal
        uint32x2_t ivTemp1 = vcle_f32(VL, B);
        // Negate the bounds
        float32x2_t vTemp2 = vneg_f32(B);
        // Test if greater or equal (Reversed)
        uint32x2_t ivTemp2 = vcle_f32(vTemp2, VL);
        // Blend answers
        ivTemp1 = vand_u32(ivTemp1, ivTemp2);
        // x and y in bounds?
        return (vget_lane_u64(vreinterpret_u64_u32(ivTemp1), 0) == 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test if less than or equal
        vfloat4 vTemp1 = _mm_cmple_ps(V, Bounds);
        // Negate the bounds
        vfloat4 vTemp2 = _mm_mul_ps(Bounds, Internal::kNegativeOne);
        // Test if greater or equal (Reversed)
        vTemp2 = _mm_cmple_ps(vTemp2, V);
        // Blend answers
        vTemp1 = _mm_and_ps(vTemp1, vTemp2);
        // x and y in bounds? (z and w are don't care)
        return (((_mm_movemask_ps(vTemp1) & 0x3) == 0x3) != 0);
#endif
    }

    //------------------------------------------------------------------------------

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

    // bool XMVector2IsNaN(FXMVECTOR V)
    inline bool VMATH_CALLCONV v2AnyIsNaN(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (VMATH_ISNAN(V.v4f[0]) ||
            VMATH_ISNAN(V.v4f[1]));
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Test against itself. NaN is always not equal
        uint32x2_t vTempNan = vceq_f32(VL, VL);
        // If x or y are NaN, the mask is zero
        return (vget_lane_u64(vreinterpret_u64_u32(vTempNan), 0) != 0xFFFFFFFFFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test against itself. NaN is always not equal
        vfloat4 vTempNan = _mm_cmpneq_ps(V, V);
        // If x or y are NaN, the mask is non-zero
        return ((_mm_movemask_ps(vTempNan) & 3) != 0);
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

    //------------------------------------------------------------------------------

    // bool XMVector2IsInfinite(FXMVECTOR V)
    inline bool VMATH_CALLCONV v2AnyIsInfinite(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        return (VMATH_ISINF(V.v4f[0]) ||
            VMATH_ISINF(V.v4f[1]));
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Mask off the sign bit
        uint32x2_t vTemp = vand_u32(vget_low_u32(vreinterpretq_u32_f32(V)), vget_low_u32(Internal::kAbsMask));
        // Compare to infinity
        vTemp = vceq_f32(vreinterpret_f32_u32(vTemp), vget_low_f32(Internal::kInfinity));
        // If any are infinity, the signs are true.
        return vget_lane_u64(vreinterpret_u64_u32(vTemp), 0) != 0;
#elif defined(VMATH_SSE_INTRINSICS)
        // Mask off the sign bit
        __m128 vTemp = _mm_and_ps(V, Internal::kAbsMask);
        // Compare to infinity
        vTemp = _mm_cmpeq_ps(vTemp, Internal::kInfinity);
        // If x or z are infinity, the signs are true.
        return ((_mm_movemask_ps(vTemp) & 3) != 0);
#endif
    }

    //------------------------------------------------------------------------------
    // Computation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Dot(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v2Dot(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result;
        Result.f[0] =
            Result.f[1] =
            Result.f[2] =
            Result.f[3] = V1.v4f[0] * V2.v4f[0] + V1.v4f[1] * V2.v4f[1];
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Perform the dot product on x and y
        float32x2_t vTemp = vmul_f32(vget_low_f32(V1), vget_low_f32(V2));
        vTemp = vpadd_f32(vTemp, vTemp);
        return vcombine_f32(vTemp, vTemp);
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_dp_ps(V1, V2, 0x3f);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vDot = _mm_mul_ps(V1, V2);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_moveldup_ps(vDot);
        return vDot;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y
        vfloat4 vLengthSq = _mm_mul_ps(V1, V2);
        // vTemp has y splatted
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        // x+y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Cross(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v2Cross(vfloat4 V1, vfloat4 V2)
    {
        // [ V1.x*V2.y - V1.y*V2.x, V1.x*V2.y - V1.y*V2.x ]

#if defined(VMATH_NO_INTRINSICS)
        float fCross = (V1.v4f[0] * V2.v4f[1]) - (V1.v4f[1] * V2.v4f[0]);
        Internal::vfloat4f32 vResult;
        vResult.f[0] =
            vResult.f[1] =
            vResult.f[2] =
            vResult.f[3] = fCross;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Negate = { { { 1.f, -1.f, 0, 0 } } };

        float32x2_t vTemp = vmul_f32(vget_low_f32(V1), vrev64_f32(vget_low_f32(V2)));
        vTemp = vmul_f32(vTemp, vget_low_f32(Negate));
        vTemp = vpadd_f32(vTemp, vTemp);
        return vcombine_f32(vTemp, vTemp);
#elif defined(VMATH_SSE_INTRINSICS)
    // Swap x and y
        vfloat4 vResult = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(0, 1, 0, 1));
        // Perform the muls
        vResult = _mm_mul_ps(vResult, V1);
        // Splat y
        vfloat4 vTemp = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(1, 1, 1, 1));
        // Sub the values
        vResult = _mm_sub_ss(vResult, vTemp);
        // Splat the cross product
        vResult = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(0, 0, 0, 0));
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2LengthSq(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2LengthSq(vfloat4 V)
    {
        return v2Dot(V, V);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2ReciprocalLengthEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2RLengthEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v2LengthSq(V);
        Result = v4RSqrtEst(Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Dot2
        float32x2_t vTemp = vmul_f32(VL, VL);
        vTemp = vpadd_f32(vTemp, vTemp);
        // Reciprocal sqrt (estimate)
        vTemp = vrsqrte_f32(vTemp);
        return vcombine_f32(vTemp, vTemp);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x3f);
        return _mm_rsqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vfloat4 vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_rsqrt_ss(vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has y splatted
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        // x+y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = _mm_rsqrt_ss(vLengthSq);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2ReciprocalLength(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2RLength(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v2LengthSq(V);
        Result = v4RSqrt(Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Dot2
        float32x2_t vTemp = vmul_f32(VL, VL);
        vTemp = vpadd_f32(vTemp, vTemp);
        // Reciprocal sqrt
        float32x2_t  S0 = vrsqrte_f32(vTemp);
        float32x2_t  P0 = vmul_f32(vTemp, S0);
        float32x2_t  R0 = vrsqrts_f32(P0, S0);
        float32x2_t  S1 = vmul_f32(S0, R0);
        float32x2_t  P1 = vmul_f32(vTemp, S1);
        float32x2_t  R1 = vrsqrts_f32(P1, S1);
        float32x2_t Result = vmul_f32(S1, R1);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x3f);
        vfloat4 vLengthSq = _mm_sqrt_ps(vTemp);
        return _mm_div_ps(Internal::kOne, vLengthSq);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vfloat4 vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ss(vTemp);
        vLengthSq = _mm_div_ss(Internal::kOne, vLengthSq);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has y splatted
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        // x+y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = _mm_sqrt_ss(vLengthSq);
        vLengthSq = _mm_div_ss(Internal::kOne, vLengthSq);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2LengthEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2LengthEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v2LengthSq(V);
        Result = v4SqrtEst(Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Dot2
        float32x2_t vTemp = vmul_f32(VL, VL);
        vTemp = vpadd_f32(vTemp, vTemp);
        const float32x2_t zero = vdup_n_f32(0);
        uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
        // Sqrt (estimate)
        float32x2_t Result = vrsqrte_f32(vTemp);
        Result = vmul_f32(vTemp, Result);
        Result = vbsl_f32(VEqualsZero, zero, Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x3f);
        return _mm_sqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vfloat4 vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ss(vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has y splatted
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        // x+y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = _mm_sqrt_ss(vLengthSq);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Length(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2Length(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v2LengthSq(V);
        Result = v4Sqrt(Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Dot2
        float32x2_t vTemp = vmul_f32(VL, VL);
        vTemp = vpadd_f32(vTemp, vTemp);
        const float32x2_t zero = vdup_n_f32(0);
        uint32x2_t VEqualsZero = vceq_f32(vTemp, zero);
        // Sqrt
        float32x2_t S0 = vrsqrte_f32(vTemp);
        float32x2_t P0 = vmul_f32(vTemp, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(vTemp, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        float32x2_t Result = vmul_f32(S1, R1);
        Result = vmul_f32(vTemp, Result);
        Result = vbsl_f32(VEqualsZero, zero, Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x3f);
        return _mm_sqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vfloat4 vTemp = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ss(vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has y splatted
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        // x+y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------
    // XMVector2NormalizeEst uses a reciprocal estimate and
    // returns QNaN on zero and infinite vectors.
    // XMVECTOR XMVector2NormalizeEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2NormalizeEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v2RLength(V);
        Result = v4Mul(V, Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Dot2
        float32x2_t vTemp = vmul_f32(VL, VL);
        vTemp = vpadd_f32(vTemp, vTemp);
        // Reciprocal sqrt (estimate)
        vTemp = vrsqrte_f32(vTemp);
        // Normalize
        float32x2_t Result = vmul_f32(VL, vTemp);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x3f);
        vfloat4 vResult = _mm_rsqrt_ps(vTemp);
        return _mm_mul_ps(vResult, V);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_rsqrt_ss(vLengthSq);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        vLengthSq = _mm_mul_ps(vLengthSq, V);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has y splatted
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        // x+y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = _mm_rsqrt_ss(vLengthSq);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        vLengthSq = _mm_mul_ps(vLengthSq, V);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Normalize(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2Normalize(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 vResult = v2Length(V);
        float fLength = vResult.v4f[0];

        // Prevent divide by zero
        if (fLength > 0)
        {
            fLength = 1.0f / fLength;
        }

        vResult.v4f[0] = V.v4f[0] * fLength;
        vResult.v4f[1] = V.v4f[1] * fLength;
        vResult.v4f[2] = V.v4f[2] * fLength;
        vResult.v4f[3] = V.v4f[3] * fLength;
        return vResult;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        // Dot2
        float32x2_t vTemp = vmul_f32(VL, VL);
        vTemp = vpadd_f32(vTemp, vTemp);
        uint32x2_t VEqualsZero = vceq_f32(vTemp, vdup_n_f32(0));
        uint32x2_t VEqualsInf = vceq_f32(vTemp, vget_low_f32(Internal::kInfinity));
        // Reciprocal sqrt (2 iterations of Newton-Raphson)
        float32x2_t S0 = vrsqrte_f32(vTemp);
        float32x2_t P0 = vmul_f32(vTemp, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(vTemp, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        vTemp = vmul_f32(S1, R1);
        // Normalize
        float32x2_t Result = vmul_f32(VL, vTemp);
        Result = vbsl_f32(VEqualsZero, vdup_n_f32(0), Result);
        Result = vbsl_f32(VEqualsInf, vget_low_f32(Internal::kQNaN), Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vLengthSq = _mm_dp_ps(V, V, 0x3f);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Reciprocal mul to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#elif defined(VMATH_SSE3_INTRINSICS)
        // Perform the dot product on x and y only
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_moveldup_ps(vLengthSq);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Reciprocal mul to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x and y only
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Reciprocal mul to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2ClampLength(FXMVECTOR V, float LengthMin, float LengthMax)
    inline vfloat4 VMATH_CALLCONV v2ClampLength(vfloat4 V, float LengthMin, float LengthMax)
    {
        vfloat4 ClampMax = v4Spread(LengthMax);
        vfloat4 ClampMin = v4Spread(LengthMin);
        return v2ClampLength(V, ClampMin, ClampMax);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2ClampLengthV(FXMVECTOR V, FXMVECTOR LengthMin, FXMVECTOR LengthMax)
    inline vfloat4 VMATH_CALLCONV v2ClampLength(vfloat4 V, vfloat4 LengthMin, vfloat4 LengthMax)
    {
        assert((v4GetY(LengthMin) == v4GetX(LengthMin)));
        assert((v4GetY(LengthMax) == v4GetX(LengthMax)));
        assert(v2AllGreaterEqual(LengthMin, Internal::kZero));
        assert(v2AllGreaterEqual(LengthMax, Internal::kZero));
        assert(v2AllGreaterEqual(LengthMax, LengthMin));

        vfloat4 LengthSq = v2LengthSq(V);

        const vfloat4 Zero = v4Zero();

        vfloat4 RcpLength = v4RSqrt(LengthSq);

        vfloat4 InfiniteLength = v4BinaryEqual(LengthSq, Internal::kInfinity.v);
        vfloat4 ZeroLength = v4Equal(LengthSq, Zero);

        vfloat4 Length = v4Mul(LengthSq, RcpLength);

        vfloat4 Normal = v4Mul(V, RcpLength);

        vfloat4 Select = v4BinaryEqual(InfiniteLength, ZeroLength);
        Length = v4Select(LengthSq, Length, Select);
        Normal = v4Select(LengthSq, Normal, Select);

        vfloat4 ControlMax = v4Greater(Length, LengthMax);
        vfloat4 ControlMin = v4Less(Length, LengthMin);

        vfloat4 ClampLength = v4Select(Length, LengthMax, ControlMax);
        ClampLength = v4Select(ClampLength, LengthMin, ControlMin);

        vfloat4 Result = v4Mul(Normal, ClampLength);

        // Preserve the original vector (with no precision loss) if the length falls within the given range
        vfloat4 Control = v4BinaryEqual(ControlMax, ControlMin);
        Result = v4Select(Result, V, Control);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Reflect(FXMVECTOR Incident, FXMVECTOR Normal)
    inline vfloat4 VMATH_CALLCONV v2Reflect(vfloat4 Incident, vfloat4 Normal)
    {
        // Result = Incident - (2 * dot(Incident, Normal)) * Normal

        vfloat4 Result;
        Result = v2Dot(Incident, Normal);
        Result = v4Add(Result, Result);
        Result = v4NegateMulAdd(Result, Normal, Incident);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Refract(FXMVECTOR Incident, FXMVECTOR Normal, float RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v2Refract(vfloat4 Incident, vfloat4 Normal, float RefractionIndex)
    {
        vfloat4 Index = v4Spread(RefractionIndex);
        return v2Refract(Incident, Normal, Index);
    }

    //------------------------------------------------------------------------------

    // Return the refraction of a 2D vector
    // XMVECTOR XMVector2RefractV(FXMVECTOR Incident, FXMVECTOR Normal, FXMVECTOR RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v2Refract(vfloat4 Incident, vfloat4 Normal, vfloat4 RefractionIndex)
    {
        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

#if defined(VMATH_NO_INTRINSICS)

        float IDotN = (Incident.v4f[0] * Normal.v4f[0]) + (Incident.v4f[1] * Normal.v4f[1]);
        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        float RY = 1.0f - (IDotN * IDotN);
        float RX = 1.0f - (RY * RefractionIndex.v4f[0] * RefractionIndex.v4f[0]);
        RY = 1.0f - (RY * RefractionIndex.v4f[1] * RefractionIndex.v4f[1]);
        if (RX >= 0.0f)
        {
            RX = (RefractionIndex.v4f[0] * Incident.v4f[0]) - (Normal.v4f[0] * ((RefractionIndex.v4f[0] * IDotN) + sqrtf(RX)));
        }
        else
        {
            RX = 0.0f;
        }
        if (RY >= 0.0f)
        {
            RY = (RefractionIndex.v4f[1] * Incident.v4f[1]) - (Normal.v4f[1] * ((RefractionIndex.v4f[1] * IDotN) + sqrtf(RY)));
        }
        else
        {
            RY = 0.0f;
        }

        vfloat4 vResult;
        vResult.v4f[0] = RX;
        vResult.v4f[1] = RY;
        vResult.v4f[2] = 0.0f;
        vResult.v4f[3] = 0.0f;
        return vResult;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t IL = vget_low_f32(Incident);
        float32x2_t NL = vget_low_f32(Normal);
        float32x2_t RIL = vget_low_f32(RefractionIndex);
        // Get the 2D Dot product of Incident-Normal
        float32x2_t vTemp = vmul_f32(IL, NL);
        float32x2_t IDotN = vpadd_f32(vTemp, vTemp);
        // vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        vTemp = vmls_f32(vget_low_f32(Internal::kOne), IDotN, IDotN);
        vTemp = vmul_f32(vTemp, RIL);
        vTemp = vmls_f32(vget_low_f32(Internal::kOne), vTemp, RIL);
        // If any terms are <=0, sqrt() will fail, punt to zero
        uint32x2_t vMask = vcgt_f32(vTemp, vget_low_f32(Internal::kZero));
        // Sqrt(vTemp)
        float32x2_t S0 = vrsqrte_f32(vTemp);
        float32x2_t P0 = vmul_f32(vTemp, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(vTemp, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        float32x2_t S2 = vmul_f32(S1, R1);
        vTemp = vmul_f32(vTemp, S2);
        // R = RefractionIndex * IDotN + sqrt(R)
        vTemp = vmla_f32(vTemp, RIL, IDotN);
        // Result = RefractionIndex * Incident - Normal * R
        float32x2_t vResult = vmul_f32(RIL, IL);
        vResult = vmls_f32(vResult, vTemp, NL);
        vResult = vreinterpret_f32_u32(vand_u32(vreinterpret_u32_f32(vResult), vMask));
        return vcombine_f32(vResult, vResult);
#elif defined(VMATH_SSE_INTRINSICS)
    // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
    // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
    // Get the 2D Dot product of Incident-Normal
        vfloat4 IDotN = v2Dot(Incident, Normal);
        // vTemp = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        vfloat4 vTemp = VMATH_FNMADD_PS(IDotN, IDotN, Internal::kOne);
        vTemp = _mm_mul_ps(vTemp, RefractionIndex);
        vTemp = VMATH_FNMADD_PS(vTemp, RefractionIndex, Internal::kOne);
        // If any terms are <=0, sqrt() will fail, punt to zero
        vfloat4 vMask = _mm_cmpgt_ps(vTemp, Internal::kZero);
        // R = RefractionIndex * IDotN + sqrt(R)
        vTemp = _mm_sqrt_ps(vTemp);
        vTemp = VMATH_FMADD_PS(RefractionIndex, IDotN, vTemp);
        // Result = RefractionIndex * Incident - Normal * R
        vfloat4 vResult = _mm_mul_ps(RefractionIndex, Incident);
        vResult = VMATH_FNMADD_PS(vTemp, Normal, vResult);
        vResult = _mm_and_ps(vResult, vMask);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2Orthogonal(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v2Orthogonal(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                -V.v4f[1],
                V.v4f[0],
                0.f,
                0.f
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Negate = { { { -1.f, 1.f, 0, 0 } } };
        const float32x2_t zero = vdup_n_f32(0);

        float32x2_t VL = vget_low_f32(V);
        float32x2_t Result = vmul_f32(vrev64_f32(VL), vget_low_f32(Negate));
        return vcombine_f32(Result, zero);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 2, 0, 1));
        vResult = _mm_mul_ps(vResult, Internal::kNegateX);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2AngleBetweenNormalsEst(FXMVECTOR N1, FXMVECTOR N2)
    inline vfloat4 VMATH_CALLCONV v2AngleBetweenNormalsEst(vfloat4 N1, vfloat4 N2)
    {
        vfloat4 Result = v2Dot(N1, N2);
        Result = v4Clamp(Result, Internal::kNegativeOne.v, Internal::kOne.v);
        Result = v4ACosEst(Result);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2AngleBetweenNormals(FXMVECTOR N1, FXMVECTOR N2)
    inline vfloat4 VMATH_CALLCONV v2AngleBetweenNormals(vfloat4 N1, vfloat4 N2)
    {
        vfloat4 Result = v2Dot(N1, N2);
        Result = v4Clamp(Result, Internal::kNegativeOne, Internal::kOne);
        Result = v4ACos(Result);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2AngleBetweenVectors(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v2AngleBetweenVectors(vfloat4 V1, vfloat4 V2)
    {
        vfloat4 L1 = v2RLength(V1);
        vfloat4 L2 = v2RLength(V2);

        vfloat4 Dot = v2Dot(V1, V2);

        L1 = v4Mul(L1, L2);

        vfloat4 CosAngle = v4Mul(Dot, L1);
        CosAngle = v4Clamp(CosAngle, Internal::kNegativeOne.v, Internal::kOne.v);

        return v4ACos(CosAngle);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2LinePointDistance(FXMVECTOR LinePoint1, FXMVECTOR LinePoint2, FXMVECTOR Point)
    inline vfloat4 VMATH_CALLCONV v2LinePointDistance(vfloat4 LinePoint1, vfloat4 LinePoint2, vfloat4 Point)
    {
        // Given a vector PointVector from LinePoint1 to Point and a vector
        // LineVector from LinePoint1 to LinePoint2, the scaled distance
        // PointProjectionScale from LinePoint1 to the perpendicular projection
        // of PointVector onto the line is defined as:
        //
        //     PointProjectionScale = dot(PointVector, LineVector) / LengthSq(LineVector)

        vfloat4 PointVector = v4Sub(Point, LinePoint1);
        vfloat4 LineVector = v4Sub(LinePoint2, LinePoint1);

        vfloat4 LengthSq = v2LengthSq(LineVector);

        vfloat4 PointProjectionScale = v2Dot(PointVector, LineVector);
        PointProjectionScale = v4Div(PointProjectionScale, LengthSq);

        vfloat4 DistanceVector = v4Mul(LineVector, PointProjectionScale);
        DistanceVector = v4Sub(PointVector, DistanceVector);

        return v2Length(DistanceVector);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector2IntersectLine(FXMVECTOR Line1Point1, FXMVECTOR Line1Point2, FXMVECTOR Line2Point1, GXMVECTOR Line2Point2)
    inline vfloat4 VMATH_CALLCONV v2IntersectLine(vfloat4 Line1Point1, vfloat4 Line1Point2, vfloat4 Line2Point1, vfloat4 Line2Point2)
    {
#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        vfloat4 V1 = v4Sub(Line1Point2, Line1Point1);
        vfloat4 V2 = v4Sub(Line2Point2, Line2Point1);
        vfloat4 V3 = v4Sub(Line1Point1, Line2Point1);

        vfloat4 C1 = v2Cross(V1, V2);
        vfloat4 C2 = v2Cross(V2, V3);

        vfloat4 Result;
        const vfloat4 Zero = v4Zero();
        if (v2AllNearEqual(C1, Zero, Internal::kEpsilon.v))
        {
            if (v2AllNearEqual(C2, Zero, Internal::kEpsilon.v))
            {
                // Coincident
                Result = Internal::kInfinity.v;
            }
            else
            {
                // Parallel
                Result = Internal::kQNaN.v;
            }
        }
        else
        {
            // Intersection point = Line1Point1 + V1 * (C2 / C1)
            vfloat4 Scale = v4Rcp(C1);
            Scale = v4Mul(C2, Scale);
            Result = v4MulAdd(V1, Scale, Line1Point1);
        }

        return Result;

#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 V1 = _mm_sub_ps(Line1Point2, Line1Point1);
        vfloat4 V2 = _mm_sub_ps(Line2Point2, Line2Point1);
        vfloat4 V3 = _mm_sub_ps(Line1Point1, Line2Point1);
        // Generate the cross products
        vfloat4 C1 = v2Cross(V1, V2);
        vfloat4 C2 = v2Cross(V2, V3);
        // If C1 is not close to epsilon, use the calculated value
        vfloat4 vResultMask = _mm_setzero_ps();
        vResultMask = _mm_sub_ps(vResultMask, C1);
        vResultMask = _mm_max_ps(vResultMask, C1);
        // 0xFFFFFFFF if the calculated value is to be used
        vResultMask = _mm_cmpgt_ps(vResultMask, Internal::kEpsilon);
        // If C1 is close to epsilon, which fail type is it? INFINITY or NAN?
        vfloat4 vFailMask = _mm_setzero_ps();
        vFailMask = _mm_sub_ps(vFailMask, C2);
        vFailMask = _mm_max_ps(vFailMask, C2);
        vFailMask = _mm_cmple_ps(vFailMask, Internal::kEpsilon);
        vfloat4 vFail = _mm_and_ps(vFailMask, Internal::kInfinity);
        vFailMask = _mm_andnot_ps(vFailMask, Internal::kQNaN);
        // vFail is NAN or INF
        vFail = _mm_or_ps(vFail, vFailMask);
        // Intersection point = Line1Point1 + V1 * (C2 / C1)
        vfloat4 vResult = _mm_div_ps(C2, C1);
        vResult = VMATH_FMADD_PS(vResult, V1, Line1Point1);
        // Use result, or failure value
        vResult = _mm_and_ps(vResult, vResultMask);
        vResultMask = _mm_andnot_ps(vResultMask, vFail);
        vResult = _mm_or_ps(vResult, vResultMask);
        return vResult;
#endif
    }

    /****************************************************************************
     *
     * 3D Vector
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------
     // Comparison operations
     //------------------------------------------------------------------------------

     //------------------------------------------------------------------------------

    // bool XMVector3Equal(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AllEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] == V2.v4f[0]) && (V1.v4f[1] == V2.v4f[1]) && (V1.v4f[2] == V2.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpeq_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3EqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AllBinaryEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4u[0] == V2.v4u[0]) && (V1.v4u[1] == V2.v4u[1]) && (V1.v4u[2] == V2.v4u[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2));
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) == 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3NearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon)
    inline bool VMATH_CALLCONV v3AllNearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon)
    {
#if defined(VMATH_NO_INTRINSICS)
        float dx, dy, dz;

        dx = fabsf(V1.v4f[0] - V2.v4f[0]);
        dy = fabsf(V1.v4f[1] - V2.v4f[1]);
        dz = fabsf(V1.v4f[2] - V2.v4f[2]);
        return (((dx <= Epsilon.v4f[0]) &&
            (dy <= Epsilon.v4f[1]) &&
            (dz <= Epsilon.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t vDelta = vsubq_f32(V1, V2);
#if defined(_MSC_VER) && !defined(__clang__)
        uint32x4_t vResult = vacleq_f32(vDelta, Epsilon);
#else
        uint32x4_t vResult = vcleq_f32(vabsq_f32(vDelta), Epsilon);
#endif
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Get the difference
        vfloat4 vDelta = _mm_sub_ps(V1, V2);
        // Get the absolute value of the difference
        vfloat4 vTemp = _mm_setzero_ps();
        vTemp = _mm_sub_ps(vTemp, vDelta);
        vTemp = _mm_max_ps(vTemp, vDelta);
        vTemp = _mm_cmple_ps(vTemp, Epsilon);
        // w is don't care
        return (((_mm_movemask_ps(vTemp) & 7) == 0x7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3NotEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AnyNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] != V2.v4f[0]) || (V1.v4f[1] != V2.v4f[1]) || (V1.v4f[2] != V2.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpeq_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 7) != 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3NotEqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AnyBinaryNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4u[0] != V2.v4u[0]) || (V1.v4u[1] != V2.v4u[1]) || (V1.v4u[2] != V2.v4u[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2));
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return (((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) & 7) != 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3Greater(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AllGreater(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] > V2.v4f[0]) && (V1.v4f[1] > V2.v4f[1]) && (V1.v4f[2] > V2.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcgtq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpgt_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3GreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AllGreaterEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] >= V2.v4f[0]) && (V1.v4f[1] >= V2.v4f[1]) && (V1.v4f[2] >= V2.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcgeq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpge_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3Less(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AllLess(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] < V2.v4f[0]) && (V1.v4f[1] < V2.v4f[1]) && (V1.v4f[2] < V2.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcltq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmplt_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3LessOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v3AllLessEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] <= V2.v4f[0]) && (V1.v4f[1] <= V2.v4f[1]) && (V1.v4f[2] <= V2.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcleq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmple_ps(V1, V2);
        return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector3InBounds(FXMVECTOR V, FXMVECTOR Bounds)
    inline bool VMATH_CALLCONV v3AllInBoundsAbs(vfloat4 V, vfloat4 Bounds)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V.v4f[0] <= Bounds.v4f[0] && V.v4f[0] >= -Bounds.v4f[0]) &&
            (V.v4f[1] <= Bounds.v4f[1] && V.v4f[1] >= -Bounds.v4f[1]) &&
            (V.v4f[2] <= Bounds.v4f[2] && V.v4f[2] >= -Bounds.v4f[2])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Test if less than or equal
        uint32x4_t ivTemp1 = vcleq_f32(V, Bounds);
        // Negate the bounds
        float32x4_t vTemp2 = vnegq_f32(Bounds);
        // Test if greater or equal (Reversed)
        uint32x4_t ivTemp2 = vcleq_f32(vTemp2, V);
        // Blend answers
        ivTemp1 = vandq_u32(ivTemp1, ivTemp2);
        // in bounds?
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(ivTemp1)), vget_high_u8(vreinterpretq_u8_u32(ivTemp1)));
        uint16x4x2_t vTemp3 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp3.val[1]), 1) & 0xFFFFFFU) == 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test if less than or equal
        vfloat4 vTemp1 = _mm_cmple_ps(V, Bounds);
        // Negate the bounds
        vfloat4 vTemp2 = _mm_mul_ps(Bounds, Internal::kNegativeOne);
        // Test if greater or equal (Reversed)
        vTemp2 = _mm_cmple_ps(vTemp2, V);
        // Blend answers
        vTemp1 = _mm_and_ps(vTemp1, vTemp2);
        // x,y and z in bounds? (w is don't care)
        return (((_mm_movemask_ps(vTemp1) & 0x7) == 0x7) != 0);
#endif
    }

    //------------------------------------------------------------------------------

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

    // bool XMVector3IsNaN(FXMVECTOR V)
    inline bool VMATH_CALLCONV v3AnyIsNaN(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        return (VMATH_ISNAN(V.v4f[0]) ||
            VMATH_ISNAN(V.v4f[1]) ||
            VMATH_ISNAN(V.v4f[2]));

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Test against itself. NaN is always not equal
        uint32x4_t vTempNan = vceqq_f32(V, V);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vTempNan)), vget_high_u8(vreinterpretq_u8_u32(vTempNan)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        // If x or y or z are NaN, the mask is zero
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) != 0xFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test against itself. NaN is always not equal
        vfloat4 vTempNan = _mm_cmpneq_ps(V, V);
        // If x or y or z are NaN, the mask is non-zero
        return ((_mm_movemask_ps(vTempNan) & 7) != 0);
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

    //------------------------------------------------------------------------------

    // bool XMVector3IsInfinite(FXMVECTOR V)
    inline bool VMATH_CALLCONV v3AnyIsInfinite(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (VMATH_ISINF(V.v4f[0]) ||
            VMATH_ISINF(V.v4f[1]) ||
            VMATH_ISINF(V.v4f[2]));
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Mask off the sign bit
        uint32x4_t vTempInf = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        // Compare to infinity
        vTempInf = vceqq_f32(vreinterpretq_f32_u32(vTempInf), Internal::kInfinity);
        // If any are infinity, the signs are true.
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vTempInf)), vget_high_u8(vreinterpretq_u8_u32(vTempInf)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return ((vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) & 0xFFFFFFU) != 0);
#elif defined(VMATH_SSE_INTRINSICS)
        // Mask off the sign bit
        __m128 vTemp = _mm_and_ps(V, Internal::kAbsMask);
        // Compare to infinity
        vTemp = _mm_cmpeq_ps(vTemp, Internal::kInfinity);
        // If x,y or z are infinity, the signs are true.
        return ((_mm_movemask_ps(vTemp) & 7) != 0);
#endif
    }

    //------------------------------------------------------------------------------
    // Computation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Dot(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v3Dot(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        float fValue = V1.v4f[0] * V2.v4f[0] + V1.v4f[1] * V2.v4f[1] + V1.v4f[2] * V2.v4f[2];
        Internal::vfloat4f32 vResult;
        vResult.f[0] =
            vResult.f[1] =
            vResult.f[2] =
            vResult.f[3] = fValue;
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t vTemp = vmulq_f32(V1, V2);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        return vcombine_f32(v1, v1);
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_dp_ps(V1, V2, 0x7f);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vTemp = _mm_mul_ps(V1, V2);
        vTemp = _mm_and_ps(vTemp, Internal::kMask3);
        vTemp = _mm_hadd_ps(vTemp, vTemp);
        return _mm_hadd_ps(vTemp, vTemp);
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product
        vfloat4 vDot = _mm_mul_ps(V1, V2);
        // x=Dot.v4f[1], y=Dot.v4f[2]
        vfloat4 vTemp = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
        // Result.v4f[0] = x+y
        vDot = _mm_add_ss(vDot, vTemp);
        // x=Dot.v4f[2]
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // Result.v4f[0] = (x+y)+z
        vDot = _mm_add_ss(vDot, vTemp);
        // Splat x
        return VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Cross(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v3Cross(vfloat4 V1, vfloat4 V2)
    {
        // [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult = { { {
                (V1.v4f[1] * V2.v4f[2]) - (V1.v4f[2] * V2.v4f[1]),
                (V1.v4f[2] * V2.v4f[0]) - (V1.v4f[0] * V2.v4f[2]),
                (V1.v4f[0] * V2.v4f[1]) - (V1.v4f[1] * V2.v4f[0]),
                0.0f
            } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t v1xy = vget_low_f32(V1);
        float32x2_t v2xy = vget_low_f32(V2);

        float32x2_t v1yx = vrev64_f32(v1xy);
        float32x2_t v2yx = vrev64_f32(v2xy);

        float32x2_t v1zz = vdup_lane_f32(vget_high_f32(V1), 0);
        float32x2_t v2zz = vdup_lane_f32(vget_high_f32(V2), 0);

        vfloat4 vResult = vmulq_f32(vcombine_f32(v1yx, v1xy), vcombine_f32(v2zz, v2yx));
        vResult = vmlsq_f32(vResult, vcombine_f32(v1zz, v1yx), vcombine_f32(v2yx, v2xy));
        vResult = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(vResult), Internal::kFlipY));
        return vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(vResult), Internal::kMask3));
#elif defined(VMATH_SSE_INTRINSICS)
    // y1,z1,x1,w1
        vfloat4 vTemp1 = VMATH_PERMUTE_PS(V1, _MM_SHUFFLE(3, 0, 2, 1));
        // z2,x2,y2,w2
        vfloat4 vTemp2 = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(3, 1, 0, 2));
        // Perform the left operation
        vfloat4 vResult = _mm_mul_ps(vTemp1, vTemp2);
        // z1,x1,y1,w1
        vTemp1 = VMATH_PERMUTE_PS(vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
        // y2,z2,x2,w2
        vTemp2 = VMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
        // Perform the right operation
        vResult = VMATH_FNMADD_PS(vTemp1, vTemp2, vResult);
        // Set w to zero
        return _mm_and_ps(vResult, Internal::kMask3);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3LengthSq(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3LengthSq(vfloat4 V)
    {
        return v3Dot(V, V);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3ReciprocalLengthEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3RLengthEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v3LengthSq(V);
        Result = v4RSqrtEst(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot3
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        // Reciprocal sqrt (estimate)
        v2 = vrsqrte_f32(v1);
        return vcombine_f32(v2, v2);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x7f);
        return _mm_rsqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_and_ps(vLengthSq, Internal::kMask3);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_rsqrt_ps(vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y and z
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and y
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
        // x+z, y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        // y,y,y,y
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // x+z+y,??,??,??
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        // Splat the length squared
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        // Get the reciprocal
        vLengthSq = _mm_rsqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3ReciprocalLength(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3RLength(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v3LengthSq(V);
        Result = v4RSqrt(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot3
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        // Reciprocal sqrt
        float32x2_t  S0 = vrsqrte_f32(v1);
        float32x2_t  P0 = vmul_f32(v1, S0);
        float32x2_t  R0 = vrsqrts_f32(P0, S0);
        float32x2_t  S1 = vmul_f32(S0, R0);
        float32x2_t  P1 = vmul_f32(v1, S1);
        float32x2_t  R1 = vrsqrts_f32(P1, S1);
        float32x2_t Result = vmul_f32(S1, R1);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x7f);
        vfloat4 vLengthSq = _mm_sqrt_ps(vTemp);
        return _mm_div_ps(Internal::kOne, vLengthSq);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vDot = _mm_mul_ps(V, V);
        vDot = _mm_and_ps(vDot, Internal::kMask3);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_sqrt_ps(vDot);
        vDot = _mm_div_ps(Internal::kOne, vDot);
        return vDot;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product
        vfloat4 vDot = _mm_mul_ps(V, V);
        // x=Dot.y, y=Dot.z
        vfloat4 vTemp = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
        // Result.x = x+y
        vDot = _mm_add_ss(vDot, vTemp);
        // x=Dot.z
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // Result.x = (x+y)+z
        vDot = _mm_add_ss(vDot, vTemp);
        // Splat x
        vDot = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
        // Get the reciprocal
        vDot = _mm_sqrt_ps(vDot);
        // Get the reciprocal
        vDot = _mm_div_ps(Internal::kOne, vDot);
        return vDot;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3LengthEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3LengthEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v3LengthSq(V);
        Result = v4SqrtEst(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot3
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        const float32x2_t zero = vdup_n_f32(0);
        uint32x2_t VEqualsZero = vceq_f32(v1, zero);
        // Sqrt (estimate)
        float32x2_t Result = vrsqrte_f32(v1);
        Result = vmul_f32(v1, Result);
        Result = vbsl_f32(VEqualsZero, zero, Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x7f);
        return _mm_sqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_and_ps(vLengthSq, Internal::kMask3);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y and z
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and y
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
        // x+z, y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        // y,y,y,y
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // x+z+y,??,??,??
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        // Splat the length squared
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        // Get the length
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Length(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3Length(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v3LengthSq(V);
        Result = v4Sqrt(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot3
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        const float32x2_t zero = vdup_n_f32(0);
        uint32x2_t VEqualsZero = vceq_f32(v1, zero);
        // Sqrt
        float32x2_t S0 = vrsqrte_f32(v1);
        float32x2_t P0 = vmul_f32(v1, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(v1, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        float32x2_t Result = vmul_f32(S1, R1);
        Result = vmul_f32(v1, Result);
        Result = vbsl_f32(VEqualsZero, zero, Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x7f);
        return _mm_sqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_and_ps(vLengthSq, Internal::kMask3);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y and z
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and y
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
        // x+z, y
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        // y,y,y,y
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // x+z+y,??,??,??
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        // Splat the length squared
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        // Get the length
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------
    // XMVector3NormalizeEst uses a reciprocal estimate and
    // returns QNaN on zero and infinite vectors.
    // XMVECTOR XMVector3NormalizeEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3NormalizeEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v3RLength(V);
        Result = v4Mul(V, Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot3
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        // Reciprocal sqrt (estimate)
        v2 = vrsqrte_f32(v1);
        // Normalize
        return vmulq_f32(V, vcombine_f32(v2, v2));
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0x7f);
        vfloat4 vResult = _mm_rsqrt_ps(vTemp);
        return _mm_mul_ps(vResult, V);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vDot = _mm_mul_ps(V, V);
        vDot = _mm_and_ps(vDot, Internal::kMask3);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_rsqrt_ps(vDot);
        vDot = _mm_mul_ps(vDot, V);
        return vDot;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product
        vfloat4 vDot = _mm_mul_ps(V, V);
        // x=Dot.y, y=Dot.z
        vfloat4 vTemp = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
        // Result.x = x+y
        vDot = _mm_add_ss(vDot, vTemp);
        // x=Dot.z
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // Result.x = (x+y)+z
        vDot = _mm_add_ss(vDot, vTemp);
        // Splat x
        vDot = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
        // Get the reciprocal
        vDot = _mm_rsqrt_ps(vDot);
        // Perform the normalization
        vDot = _mm_mul_ps(vDot, V);
        return vDot;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Normalize(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3Normalize(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        float fLength;
        vfloat4 vResult;

        vResult = v3Length(V);
        fLength = vResult.v4f[0];

        // Prevent divide by zero
        if (fLength > 0)
        {
            fLength = 1.0f / fLength;
        }

        vResult.v4f[0] = V.v4f[0] * fLength;
        vResult.v4f[1] = V.v4f[1] * fLength;
        vResult.v4f[2] = V.v4f[2] * fLength;
        vResult.v4f[3] = V.v4f[3] * fLength;
        return vResult;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot3
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vpadd_f32(v1, v1);
        v2 = vdup_lane_f32(v2, 0);
        v1 = vadd_f32(v1, v2);
        uint32x2_t VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
        uint32x2_t VEqualsInf = vceq_f32(v1, vget_low_f32(Internal::kInfinity));
        // Reciprocal sqrt (2 iterations of Newton-Raphson)
        float32x2_t S0 = vrsqrte_f32(v1);
        float32x2_t P0 = vmul_f32(v1, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(v1, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        v2 = vmul_f32(S1, R1);
        // Normalize
        vfloat4 vResult = vmulq_f32(V, vcombine_f32(v2, v2));
        vResult = vbslq_f32(vcombine_u32(VEqualsZero, VEqualsZero), vdupq_n_f32(0), vResult);
        return vbslq_f32(vcombine_u32(VEqualsInf, VEqualsInf), Internal::kQNaN, vResult);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vLengthSq = _mm_dp_ps(V, V, 0x7f);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Divide to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#elif defined(VMATH_SSE3_INTRINSICS)
        // Perform the dot product on x,y and z only
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_and_ps(vLengthSq, Internal::kMask3);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Divide to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y and z only
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Divide to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3ClampLength(FXMVECTOR V, float LengthMin, float LengthMax)
    inline vfloat4 VMATH_CALLCONV v3ClampLength(vfloat4 V, float LengthMin, float LengthMax)
    {
        vfloat4 ClampMax = v4Spread(LengthMax);
        vfloat4 ClampMin = v4Spread(LengthMin);

        return v3ClampLength(V, ClampMin, ClampMax);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3ClampLengthV(FXMVECTOR V, FXMVECTOR LengthMin, FXMVECTOR LengthMax)
    inline vfloat4 VMATH_CALLCONV v3ClampLength(vfloat4 V, vfloat4 LengthMin, vfloat4 LengthMax)
    {
        assert((v4GetY(LengthMin) == v4GetX(LengthMin)) && (v4GetZ(LengthMin) == v4GetX(LengthMin)));
        assert((v4GetY(LengthMax) == v4GetX(LengthMax)) && (v4GetZ(LengthMax) == v4GetX(LengthMax)));
        assert(v3AllGreaterEqual(LengthMin, v4Zero()));
        assert(v3AllGreaterEqual(LengthMax, v4Zero()));
        assert(v3AllGreaterEqual(LengthMax, LengthMin));

        vfloat4 LengthSq = v3LengthSq(V);

        const vfloat4 Zero = v4Zero();

        vfloat4 RcpLength = v4RSqrt(LengthSq);

        vfloat4 InfiniteLength = v4BinaryEqual(LengthSq, Internal::kInfinity.v);
        vfloat4 ZeroLength = v4Equal(LengthSq, Zero);

        vfloat4 Normal = v4Mul(V, RcpLength);

        vfloat4 Length = v4Mul(LengthSq, RcpLength);

        vfloat4 Select = v4BinaryEqual(InfiniteLength, ZeroLength);
        Length = v4Select(LengthSq, Length, Select);
        Normal = v4Select(LengthSq, Normal, Select);

        vfloat4 ControlMax = v4Greater(Length, LengthMax);
        vfloat4 ControlMin = v4Less(Length, LengthMin);

        vfloat4 ClampLength = v4Select(Length, LengthMax, ControlMax);
        ClampLength = v4Select(ClampLength, LengthMin, ControlMin);

        vfloat4 Result = v4Mul(Normal, ClampLength);

        // Preserve the original vector (with no precision loss) if the length falls within the given range
        vfloat4 Control = v4BinaryEqual(ControlMax, ControlMin);
        Result = v4Select(Result, V, Control);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Reflect(FXMVECTOR Incident, FXMVECTOR Normal)
    inline vfloat4 VMATH_CALLCONV v3Reflect(vfloat4 Incident, vfloat4 Normal)
    {
        // Result = Incident - (2 * dot(Incident, Normal)) * Normal

        vfloat4 Result = v3Dot(Incident, Normal);
        Result = v4Add(Result, Result);
        Result = v4NegateMulAdd(Result, Normal, Incident);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Refract(FXMVECTOR Incident, FXMVECTOR Normal, float RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v3Refract(vfloat4 Incident, vfloat4 Normal, float RefractionIndex)
    {
        vfloat4 Index = v4Spread(RefractionIndex);
        return v3Refract(Incident, Normal, Index);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3RefractV(FXMVECTOR Incident, FXMVECTOR Normal, FXMVECTOR RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v3Refract(vfloat4 Incident, vfloat4 Normal, vfloat4 RefractionIndex)
    {
        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

#if defined(VMATH_NO_INTRINSICS)

        const vfloat4  Zero = v4Zero();

        vfloat4 IDotN = v3Dot(Incident, Normal);

        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        vfloat4 R = v4NegateMulAdd(IDotN, IDotN, Internal::kOne.v);
        R = v4Mul(R, RefractionIndex);
        R = v4NegateMulAdd(R, RefractionIndex, Internal::kOne.v);

        if (v4AllLessEqual(R, Zero))
        {
            // Total internal reflection
            return Zero;
        }
        else
        {
            // R = RefractionIndex * IDotN + sqrt(R)
            R = v4Sqrt(R);
            R = v4MulAdd(RefractionIndex, IDotN, R);

            // Result = RefractionIndex * Incident - Normal * R
            vfloat4 Result = v4Mul(RefractionIndex, Incident);
            Result = v4NegateMulAdd(Normal, R, Result);

            return Result;
        }

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 IDotN = v3Dot(Incident, Normal);

        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        float32x4_t R = vmlsq_f32(Internal::kOne, IDotN, IDotN);
        R = vmulq_f32(R, RefractionIndex);
        R = vmlsq_f32(Internal::kOne, R, RefractionIndex);

        uint32x4_t isrzero = vcleq_f32(R, Internal::kZero);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(isrzero)), vget_high_u8(vreinterpretq_u8_u32(isrzero)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));

        float32x4_t vResult;
        if (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU)
        {
            // Total internal reflection
            vResult = Internal::kZero;
        }
        else
        {
            // Sqrt(R)
            float32x4_t S0 = vrsqrteq_f32(R);
            float32x4_t P0 = vmulq_f32(R, S0);
            float32x4_t R0 = vrsqrtsq_f32(P0, S0);
            float32x4_t S1 = vmulq_f32(S0, R0);
            float32x4_t P1 = vmulq_f32(R, S1);
            float32x4_t R1 = vrsqrtsq_f32(P1, S1);
            float32x4_t S2 = vmulq_f32(S1, R1);
            R = vmulq_f32(R, S2);
            // R = RefractionIndex * IDotN + sqrt(R)
            R = vmlaq_f32(R, RefractionIndex, IDotN);
            // Result = RefractionIndex * Incident - Normal * R
            vResult = vmulq_f32(RefractionIndex, Incident);
            vResult = vmlsq_f32(vResult, R, Normal);
        }
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
    // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
    // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
        vfloat4 IDotN = v3Dot(Incident, Normal);
        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        vfloat4 R = VMATH_FNMADD_PS(IDotN, IDotN, Internal::kOne);
        vfloat4 R2 = _mm_mul_ps(RefractionIndex, RefractionIndex);
        R = VMATH_FNMADD_PS(R, R2, Internal::kOne);

        vfloat4 vResult = _mm_cmple_ps(R, Internal::kZero);
        if (_mm_movemask_ps(vResult) == 0x0f)
        {
            // Total internal reflection
            vResult = Internal::kZero;
        }
        else
        {
            // R = RefractionIndex * IDotN + sqrt(R)
            R = _mm_sqrt_ps(R);
            R = VMATH_FMADD_PS(RefractionIndex, IDotN, R);
            // Result = RefractionIndex * Incident - Normal * R
            vResult = _mm_mul_ps(RefractionIndex, Incident);
            vResult = VMATH_FNMADD_PS(R, Normal, vResult);
        }
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Orthogonal(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v3Orthogonal(vfloat4 V)
    {
        vfloat4 Zero = v4Zero();
        vfloat4 Z = v4zzzz(V);
        vfloat4 YZYY = v4Swizzle<1, 2, 1, 1>(V);

        vfloat4 NegativeV = v4Sub(Zero, V);

        vfloat4 ZIsNegative = v4Less(Z, Zero);
        vfloat4 YZYYIsNegative = v4Less(YZYY, Zero);

        vfloat4 S = v4Add(YZYY, Z);
        vfloat4 D = v4Sub(YZYY, Z);

        vfloat4 Select = v4BinaryEqual(ZIsNegative, YZYYIsNegative);

        vfloat4 R0 = v4Shuffle<4, 0, 0, 0>(NegativeV, S);
        vfloat4 R1 = v4Shuffle<4, 0, 0, 0>(V, D);

        return v4Select(R1, R0, Select);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3AngleBetweenNormalsEst(FXMVECTOR N1, FXMVECTOR N2)
    inline vfloat4 VMATH_CALLCONV v3AngleBetweenNormalsEst(vfloat4 N1, vfloat4 N2)
    {
        vfloat4 Result = v3Dot(N1, N2);
        Result = v4Clamp(Result, Internal::kNegativeOne.v, Internal::kOne.v);
        Result = v4ACosEst(Result);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3AngleBetweenNormals(FXMVECTOR N1, FXMVECTOR N2)
    inline vfloat4 VMATH_CALLCONV v3AngleBetweenNormals(vfloat4 N1, vfloat4 N2)
    {
        vfloat4 Result = v3Dot(N1, N2);
        Result = v4Clamp(Result, Internal::kNegativeOne.v, Internal::kOne.v);
        Result = v4ACos(Result);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3AngleBetweenVectors(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v3AngleBetweenVectors(vfloat4 V1, vfloat4 V2)
    {
        vfloat4 L1 = v3RLength(V1);
        vfloat4 L2 = v3RLength(V2);

        vfloat4 Dot = v3Dot(V1, V2);

        L1 = v4Mul(L1, L2);

        vfloat4 CosAngle = v4Mul(Dot, L1);
        CosAngle = v4Clamp(CosAngle, Internal::kNegativeOne.v, Internal::kOne.v);

        return v4ACos(CosAngle);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3LinePointDistance(FXMVECTOR LinePoint1, FXMVECTOR LinePoint2, FXMVECTOR Point)
    inline vfloat4 VMATH_CALLCONV v3LinePointDistance(vfloat4 LinePoint1, vfloat4 LinePoint2, vfloat4 Point)
    {
        // Given a vector PointVector from LinePoint1 to Point and a vector
        // LineVector from LinePoint1 to LinePoint2, the scaled distance
        // PointProjectionScale from LinePoint1 to the perpendicular projection
        // of PointVector onto the line is defined as:
        //
        //     PointProjectionScale = dot(PointVector, LineVector) / LengthSq(LineVector)

        vfloat4 PointVector = v4Sub(Point, LinePoint1);
        vfloat4 LineVector = v4Sub(LinePoint2, LinePoint1);

        vfloat4 LengthSq = v3LengthSq(LineVector);

        vfloat4 PointProjectionScale = v3Dot(PointVector, LineVector);
        PointProjectionScale = v4Div(PointProjectionScale, LengthSq);

        vfloat4 DistanceVector = v4Mul(LineVector, PointProjectionScale);
        DistanceVector = v4Sub(PointVector, DistanceVector);

        return v3Length(DistanceVector);
    }

    //------------------------------------------------------------------------------

    // void XMVector3ComponentsFromNormal(XMVECTOR* pParallel, XMVECTOR* pPerpendicular, FXMVECTOR V, FXMVECTOR Normal)
    inline void VMATH_CALLCONV v3ComponentsFromNormal(vfloat4* pParallel, vfloat4* pPerpendicular, vfloat4 V, vfloat4 Normal)
    {
        assert(pParallel != nullptr);
        assert(pPerpendicular != nullptr);

        vfloat4 Scale = v3Dot(V, Normal);

        vfloat4 Parallel = v4Mul(Normal, Scale);

        *pParallel = Parallel;
        *pPerpendicular = v4Sub(V, Parallel);
    }

    //------------------------------------------------------------------------------
    // Transform a vector using a rotation expressed as a unit quaternion
    // XMVECTOR XMVector3Rotate(FXMVECTOR V, FXMVECTOR RotationQuaternion)
    inline vfloat4 VMATH_CALLCONV v3Rotate(vfloat4 V, vfloat4 RotationQuaternion)
    {
        vfloat4 A = v4Select(Internal::kSelect1110.v, V, Internal::kSelect1110.v);
        vfloat4 Q = vQuatConjugate(RotationQuaternion);
        vfloat4 Result = vQuatMul(Q, A);
        return vQuatMul(Result, RotationQuaternion);
    }

    //------------------------------------------------------------------------------
    // Transform a vector using the inverse of a rotation expressed as a unit quaternion
    // XMVECTOR XMVector3InverseRotate(FXMVECTOR V, FXMVECTOR RotationQuaternion)
    inline vfloat4 VMATH_CALLCONV v3InverseRotate(vfloat4 V, vfloat4 RotationQuaternion)
    {
        vfloat4 A = v4Select(Internal::kSelect1110.v, V, Internal::kSelect1110.v);
        vfloat4 Result = vQuatMul(RotationQuaternion, A);
        vfloat4 Q = vQuatConjugate(RotationQuaternion);
        return vQuatMul(Result, Q);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Transform(FXMVECTOR V, FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV v3TransformPoint(vfloat4 V, vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Z = v4zzzz(V);
        vfloat4 Y = v4yyyy(V);
        vfloat4 X = v4xxxx(V);

        vfloat4 Result = v4MulAdd(Z, M.r[2], M.r[3]);
        Result = v4MulAdd(Y, M.r[1], Result);
        Result = v4MulAdd(X, M.r[0], Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        vfloat4 vResult = vmlaq_lane_f32(M.r[3], M.r[0], VL, 0); // X
        vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
        return vmlaq_lane_f32(vResult, M.r[2], vget_high_f32(V), 0); // Z
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2)); // Z
        vResult = VMATH_FMADD_PS(vResult, M.r[2], M.r[3]);
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1)); // Y
        vResult = VMATH_FMADD_PS(vTemp, M.r[1], vResult);
        vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0)); // X
        vResult = VMATH_FMADD_PS(vTemp, M.r[0], vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3TransformCoord(FXMVECTOR V, FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV v3TransformPointDivideW(vfloat4 V, vfloat4x4 M)
    {
        vfloat4 Z = v4zzzz(V);
        vfloat4 Y = v4yyyy(V);
        vfloat4 X = v4xxxx(V);

        vfloat4 Result = v4MulAdd(Z, M.r[2], M.r[3]);
        Result = v4MulAdd(Y, M.r[1], Result);
        Result = v4MulAdd(X, M.r[0], Result);

        vfloat4 W = v4wwww(Result);
        return v4Div(Result, W);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3TransformNormal(FXMVECTOR V, FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV v3TransformNormal(vfloat4 V, vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Z = v4zzzz(V);
        vfloat4 Y = v4yyyy(V);
        vfloat4 X = v4xxxx(V);

        vfloat4 Result = v4Mul(Z, M.r[2]);
        Result = v4MulAdd(Y, M.r[1], Result);
        Result = v4MulAdd(X, M.r[0], Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        vfloat4 vResult = vmulq_lane_f32(M.r[0], VL, 0); // X
        vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
        return vmlaq_lane_f32(vResult, M.r[2], vget_high_f32(V), 0); // Z
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2)); // Z
        vResult = _mm_mul_ps(vResult, M.r[2]);
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1)); // Y
        vResult = VMATH_FMADD_PS(vTemp, M.r[1], vResult);
        vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0)); // X
        vResult = VMATH_FMADD_PS(vTemp, M.r[0], vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Project(FXMVECTOR V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, FXMMATRIX Projection, CXMMATRIX View, CXMMATRIX World)
    inline vfloat4 VMATH_CALLCONV v3Project(vfloat4 V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, vfloat4x4 Projection, vfloat4x4 View, vfloat4x4 World)
    {
        const float HalfViewportWidth = ViewportWidth * 0.5f;
        const float HalfViewportHeight = ViewportHeight * 0.5f;

        vfloat4 Scale = v4Set(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 0.0f);
        vfloat4 Offset = v4Set(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

        vfloat4x4 Transform = vMat4Mul(World, View);
        Transform = vMat4Mul(Transform, Projection);

        vfloat4 Result = v3TransformPointDivideW(V, Transform);

        Result = v4MulAdd(Result, Scale, Offset);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector3Unproject(FXMVECTOR V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, FXMMATRIX Projection, CXMMATRIX View, CXMMATRIX World)
    inline vfloat4 VMATH_CALLCONV v3Unproject(vfloat4 V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, vfloat4x4 Projection, vfloat4x4 View, vfloat4x4 World)
    {
        static const Internal::vfloat4f32 D = { { { -1.0f, 1.0f, 0.0f, 0.0f } } };

        vfloat4 Scale = v4Set(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
        Scale = v4Rcp(Scale);

        vfloat4 Offset = v4Set(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
        Offset = v4MulAdd(Scale, Offset, D.v);

        vfloat4x4 Transform = vMat4Mul(World, View);
        Transform = vMat4Mul(Transform, Projection);
        Transform = vMat4Inverse(nullptr, Transform);

        vfloat4 Result = v4MulAdd(V, Scale, Offset);

        return v3TransformPointDivideW(Result, Transform);
    }

    /****************************************************************************
     *
     * 4D Vector
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------
     // Comparison operations
     //------------------------------------------------------------------------------

     //------------------------------------------------------------------------------

    // bool XMVector4Equal(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AllEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] == V2.v4f[0]) && (V1.v4f[1] == V2.v4f[1]) && (V1.v4f[2] == V2.v4f[2]) && (V1.v4f[3] == V2.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpeq_ps(V1, V2);
        return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4EqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AllBinaryEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4u[0] == V2.v4u[0]) && (V1.v4u[1] == V2.v4u[1]) && (V1.v4u[2] == V2.v4u[2]) && (V1.v4u[3] == V2.v4u[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2));
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) == 0xf) != 0);
#endif
    }

    // bool XMVector4NearEqual(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR Epsilon)
    inline bool VMATH_CALLCONV v4AllNearEqual(vfloat4 V1, vfloat4 V2, vfloat4 Epsilon)
    {
#if defined(VMATH_NO_INTRINSICS)
        float dx, dy, dz, dw;

        dx = fabsf(V1.v4f[0] - V2.v4f[0]);
        dy = fabsf(V1.v4f[1] - V2.v4f[1]);
        dz = fabsf(V1.v4f[2] - V2.v4f[2]);
        dw = fabsf(V1.v4f[3] - V2.v4f[3]);
        return (((dx <= Epsilon.v4f[0]) &&
            (dy <= Epsilon.v4f[1]) &&
            (dz <= Epsilon.v4f[2]) &&
            (dw <= Epsilon.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t vDelta = vsubq_f32(V1, V2);
#if defined(_MSC_VER) && !defined(__clang__)
        uint32x4_t vResult = vacleq_f32(vDelta, Epsilon);
#else
        uint32x4_t vResult = vcleq_f32(vabsq_f32(vDelta), Epsilon);
#endif
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Get the difference
        vfloat4 vDelta = _mm_sub_ps(V1, V2);
        // Get the absolute value of the difference
        vfloat4 vTemp = _mm_setzero_ps();
        vTemp = _mm_sub_ps(vTemp, vDelta);
        vTemp = _mm_max_ps(vTemp, vDelta);
        vTemp = _mm_cmple_ps(vTemp, Epsilon);
        return ((_mm_movemask_ps(vTemp) == 0xf) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4NotEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AnyNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] != V2.v4f[0]) || (V1.v4f[1] != V2.v4f[1]) || (V1.v4f[2] != V2.v4f[2]) || (V1.v4f[3] != V2.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) != 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpneq_ps(V1, V2);
        return ((_mm_movemask_ps(vTemp)) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4NotEqualInt(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AnyBinaryNotEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4u[0] != V2.v4u[0]) || (V1.v4u[1] != V2.v4u[1]) || (V1.v4u[2] != V2.v4u[2]) || (V1.v4u[3] != V2.v4u[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vceqq_u32(vreinterpretq_u32_f32(V1), vreinterpretq_u32_f32(V2));
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) != 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        __m128i vTemp = _mm_cmpeq_epi32(_mm_castps_si128(V1), _mm_castps_si128(V2));
        return ((_mm_movemask_ps(_mm_castsi128_ps(vTemp)) != 0xF) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4Greater(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AllGreater(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] > V2.v4f[0]) && (V1.v4f[1] > V2.v4f[1]) && (V1.v4f[2] > V2.v4f[2]) && (V1.v4f[3] > V2.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcgtq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpgt_ps(V1, V2);
        return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4GreaterOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AllGreaterEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] >= V2.v4f[0]) && (V1.v4f[1] >= V2.v4f[1]) && (V1.v4f[2] >= V2.v4f[2]) && (V1.v4f[3] >= V2.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcgeq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmpge_ps(V1, V2);
        return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4Less(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AllLess(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] < V2.v4f[0]) && (V1.v4f[1] < V2.v4f[1]) && (V1.v4f[2] < V2.v4f[2]) && (V1.v4f[3] < V2.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcltq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmplt_ps(V1, V2);
        return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4LessOrEqual(FXMVECTOR V1, FXMVECTOR V2)
    inline bool VMATH_CALLCONV v4AllLessEqual(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V1.v4f[0] <= V2.v4f[0]) && (V1.v4f[1] <= V2.v4f[1]) && (V1.v4f[2] <= V2.v4f[2]) && (V1.v4f[3] <= V2.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vResult = vcleq_f32(V1, V2);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vResult)), vget_high_u8(vreinterpretq_u8_u32(vResult)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp = _mm_cmple_ps(V1, V2);
        return ((_mm_movemask_ps(vTemp) == 0x0f) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMVector4InBounds(FXMVECTOR V, FXMVECTOR Bounds)
    inline bool VMATH_CALLCONV v4AllInBoundsAbs(vfloat4 V, vfloat4 Bounds)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (((V.v4f[0] <= Bounds.v4f[0] && V.v4f[0] >= -Bounds.v4f[0]) &&
            (V.v4f[1] <= Bounds.v4f[1] && V.v4f[1] >= -Bounds.v4f[1]) &&
            (V.v4f[2] <= Bounds.v4f[2] && V.v4f[2] >= -Bounds.v4f[2]) &&
            (V.v4f[3] <= Bounds.v4f[3] && V.v4f[3] >= -Bounds.v4f[3])) != 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Test if less than or equal
        uint32x4_t ivTemp1 = vcleq_f32(V, Bounds);
        // Negate the bounds
        float32x4_t vTemp2 = vnegq_f32(Bounds);
        // Test if greater or equal (Reversed)
        uint32x4_t ivTemp2 = vcleq_f32(vTemp2, V);
        // Blend answers
        ivTemp1 = vandq_u32(ivTemp1, ivTemp2);
        // in bounds?
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(ivTemp1)), vget_high_u8(vreinterpretq_u8_u32(ivTemp1)));
        uint16x4x2_t vTemp3 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp3.val[1]), 1) == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test if less than or equal
        vfloat4 vTemp1 = _mm_cmple_ps(V, Bounds);
        // Negate the bounds
        vfloat4 vTemp2 = _mm_mul_ps(Bounds, Internal::kNegativeOne);
        // Test if greater or equal (Reversed)
        vTemp2 = _mm_cmple_ps(vTemp2, V);
        // Blend answers
        vTemp1 = _mm_and_ps(vTemp1, vTemp2);
        // All in bounds?
        return ((_mm_movemask_ps(vTemp1) == 0x0f) != 0);
#endif
    }

    //------------------------------------------------------------------------------

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

    // bool XMVector4IsNaN(FXMVECTOR V)
    inline bool VMATH_CALLCONV v4AnyIsNaN(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (VMATH_ISNAN(V.v4f[0]) ||
            VMATH_ISNAN(V.v4f[1]) ||
            VMATH_ISNAN(V.v4f[2]) ||
            VMATH_ISNAN(V.v4f[3]));
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Test against itself. NaN is always not equal
        uint32x4_t vTempNan = vceqq_f32(V, V);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vTempNan)), vget_high_u8(vreinterpretq_u8_u32(vTempNan)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        // If any are NaN, the mask is zero
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) != 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        // Test against itself. NaN is always not equal
        vfloat4 vTempNan = _mm_cmpneq_ps(V, V);
        // If any are NaN, the mask is non-zero
        return (_mm_movemask_ps(vTempNan) != 0);
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

    //------------------------------------------------------------------------------

    // bool XMVector4IsInfinite(FXMVECTOR V)
    inline bool VMATH_CALLCONV v4AnyIsInfinite(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        return (VMATH_ISINF(V.v4f[0]) ||
            VMATH_ISINF(V.v4f[1]) ||
            VMATH_ISINF(V.v4f[2]) ||
            VMATH_ISINF(V.v4f[3]));

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Mask off the sign bit
        uint32x4_t vTempInf = vandq_u32(vreinterpretq_u32_f32(V), Internal::kAbsMask);
        // Compare to infinity
        vTempInf = vceqq_f32(vreinterpretq_f32_u32(vTempInf), Internal::kInfinity);
        // If any are infinity, the signs are true.
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(vTempInf)), vget_high_u8(vreinterpretq_u8_u32(vTempInf)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) != 0);
#elif defined(VMATH_SSE_INTRINSICS)
        // Mask off the sign bit
        vfloat4 vTemp = _mm_and_ps(V, Internal::kAbsMask);
        // Compare to infinity
        vTemp = _mm_cmpeq_ps(vTemp, Internal::kInfinity);
        // If any are infinity, the signs are true.
        return (_mm_movemask_ps(vTemp) != 0);
#endif
    }

    //------------------------------------------------------------------------------
    // Computation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Dot(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4Dot(vfloat4 V1, vfloat4 V2)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result;
        Result.f[0] =
            Result.f[1] =
            Result.f[2] =
            Result.f[3] = V1.v4f[0] * V2.v4f[0] + V1.v4f[1] * V2.v4f[1] + V1.v4f[2] * V2.v4f[2] + V1.v4f[3] * V2.v4f[3];
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4_t vTemp = vmulq_f32(V1, V2);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        return vcombine_f32(v1, v1);
#elif defined(VMATH_SSE4_INTRINSICS)
        return _mm_dp_ps(V1, V2, 0xff);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vTemp = _mm_mul_ps(V1, V2);
        vTemp = _mm_hadd_ps(vTemp, vTemp);
        return _mm_hadd_ps(vTemp, vTemp);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp2 = V2;
        vfloat4 vTemp = _mm_mul_ps(V1, vTemp2);
        vTemp2 = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0)); // Copy X to the Z position and Y to the W position
        vTemp2 = _mm_add_ps(vTemp2, vTemp);          // Add Z = X+Z; W = Y+W;
        vTemp = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));  // Copy W to the Z position
        vTemp = _mm_add_ps(vTemp, vTemp2);           // Add Z and W together
        return VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(2, 2, 2, 2));    // Splat Z and return
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Cross(FXMVECTOR V1, FXMVECTOR V2, FXMVECTOR V3)
    inline vfloat4 VMATH_CALLCONV v4Cross(vfloat4 V1, vfloat4 V2, vfloat4 V3)
    {
        // [ ((v2.z*v3.w-v2.w*v3.z)*v1.y)-((v2.y*v3.w-v2.w*v3.y)*v1.z)+((v2.y*v3.z-v2.z*v3.y)*v1.w),
        //   ((v2.w*v3.z-v2.z*v3.w)*v1.x)-((v2.w*v3.x-v2.x*v3.w)*v1.z)+((v2.z*v3.x-v2.x*v3.z)*v1.w),
        //   ((v2.y*v3.w-v2.w*v3.y)*v1.x)-((v2.x*v3.w-v2.w*v3.x)*v1.y)+((v2.x*v3.y-v2.y*v3.x)*v1.w),
        //   ((v2.z*v3.y-v2.y*v3.z)*v1.x)-((v2.z*v3.x-v2.x*v3.z)*v1.y)+((v2.y*v3.x-v2.x*v3.y)*v1.z) ]

#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                (((V2.v4f[2] * V3.v4f[3]) - (V2.v4f[3] * V3.v4f[2])) * V1.v4f[1]) - (((V2.v4f[1] * V3.v4f[3]) - (V2.v4f[3] * V3.v4f[1])) * V1.v4f[2]) + (((V2.v4f[1] * V3.v4f[2]) - (V2.v4f[2] * V3.v4f[1])) * V1.v4f[3]),
                (((V2.v4f[3] * V3.v4f[2]) - (V2.v4f[2] * V3.v4f[3])) * V1.v4f[0]) - (((V2.v4f[3] * V3.v4f[0]) - (V2.v4f[0] * V3.v4f[3])) * V1.v4f[2]) + (((V2.v4f[2] * V3.v4f[0]) - (V2.v4f[0] * V3.v4f[2])) * V1.v4f[3]),
                (((V2.v4f[1] * V3.v4f[3]) - (V2.v4f[3] * V3.v4f[1])) * V1.v4f[0]) - (((V2.v4f[0] * V3.v4f[3]) - (V2.v4f[3] * V3.v4f[0])) * V1.v4f[1]) + (((V2.v4f[0] * V3.v4f[1]) - (V2.v4f[1] * V3.v4f[0])) * V1.v4f[3]),
                (((V2.v4f[2] * V3.v4f[1]) - (V2.v4f[1] * V3.v4f[2])) * V1.v4f[0]) - (((V2.v4f[2] * V3.v4f[0]) - (V2.v4f[0] * V3.v4f[2])) * V1.v4f[1]) + (((V2.v4f[1] * V3.v4f[0]) - (V2.v4f[0] * V3.v4f[1])) * V1.v4f[2]),
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        const uint32x2_t select = vget_low_u32(Internal::kMaskX);

        // Term1: V2zwyz * V3wzwy
        const float32x2_t v2xy = vget_low_f32(V2);
        const float32x2_t v2zw = vget_high_f32(V2);
        const float32x2_t v2yx = vrev64_f32(v2xy);
        const float32x2_t v2wz = vrev64_f32(v2zw);
        const float32x2_t v2yz = vbsl_f32(select, v2yx, v2wz);

        const float32x2_t v3zw = vget_high_f32(V3);
        const float32x2_t v3wz = vrev64_f32(v3zw);
        const float32x2_t v3xy = vget_low_f32(V3);
        const float32x2_t v3wy = vbsl_f32(select, v3wz, v3xy);

        float32x4_t vTemp1 = vcombine_f32(v2zw, v2yz);
        float32x4_t vTemp2 = vcombine_f32(v3wz, v3wy);
        vfloat4 vResult = vmulq_f32(vTemp1, vTemp2);

        // - V2wzwy * V3zwyz
        const float32x2_t v2wy = vbsl_f32(select, v2wz, v2xy);

        const float32x2_t v3yx = vrev64_f32(v3xy);
        const float32x2_t v3yz = vbsl_f32(select, v3yx, v3wz);

        vTemp1 = vcombine_f32(v2wz, v2wy);
        vTemp2 = vcombine_f32(v3zw, v3yz);
        vResult = vmlsq_f32(vResult, vTemp1, vTemp2);

        // term1 * V1yxxx
        const float32x2_t v1xy = vget_low_f32(V1);
        const float32x2_t v1yx = vrev64_f32(v1xy);

        vTemp1 = vcombine_f32(v1yx, vdup_lane_f32(v1yx, 1));
        vResult = vmulq_f32(vResult, vTemp1);

        // Term2: V2ywxz * V3wxwx
        const float32x2_t v2yw = vrev64_f32(v2wy);
        const float32x2_t v2xz = vbsl_f32(select, v2xy, v2wz);

        const float32x2_t v3wx = vbsl_f32(select, v3wz, v3yx);

        vTemp1 = vcombine_f32(v2yw, v2xz);
        vTemp2 = vcombine_f32(v3wx, v3wx);
        float32x4_t vTerm = vmulq_f32(vTemp1, vTemp2);

        // - V2wxwx * V3ywxz
        const float32x2_t v2wx = vbsl_f32(select, v2wz, v2yx);

        const float32x2_t v3yw = vrev64_f32(v3wy);
        const float32x2_t v3xz = vbsl_f32(select, v3xy, v3wz);

        vTemp1 = vcombine_f32(v2wx, v2wx);
        vTemp2 = vcombine_f32(v3yw, v3xz);
        vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);

        // vResult - term2 * V1zzyy
        const float32x2_t v1zw = vget_high_f32(V1);

        vTemp1 = vcombine_f32(vdup_lane_f32(v1zw, 0), vdup_lane_f32(v1yx, 0));
        vResult = vmlsq_f32(vResult, vTerm, vTemp1);

        // Term3: V2yzxy * V3zxyx
        const float32x2_t v3zx = vrev64_f32(v3xz);

        vTemp1 = vcombine_f32(v2yz, v2xy);
        vTemp2 = vcombine_f32(v3zx, v3yx);
        vTerm = vmulq_f32(vTemp1, vTemp2);

        // - V2zxyx * V3yzxy
        const float32x2_t v2zx = vrev64_f32(v2xz);

        vTemp1 = vcombine_f32(v2zx, v2yx);
        vTemp2 = vcombine_f32(v3yz, v3xy);
        vTerm = vmlsq_f32(vTerm, vTemp1, vTemp2);

        // vResult + term3 * V1wwwz
        const float32x2_t v1wz = vrev64_f32(v1zw);

        vTemp1 = vcombine_f32(vdup_lane_f32(v1wz, 0), v1wz);
        return vmlaq_f32(vResult, vTerm, vTemp1);
#elif defined(VMATH_SSE_INTRINSICS)
    // V2zwyz * V3wzwy
        vfloat4 vResult = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 1, 3, 2));
        vfloat4 vTemp3 = VMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 3, 2, 3));
        vResult = _mm_mul_ps(vResult, vTemp3);
        // - V2wzwy * V3zwyz
        vfloat4 vTemp2 = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 3, 2, 3));
        vTemp3 = VMATH_PERMUTE_PS(vTemp3, _MM_SHUFFLE(1, 3, 0, 1));
        vResult = VMATH_FNMADD_PS(vTemp2, vTemp3, vResult);
        // term1 * V1yxxx
        vfloat4 vTemp1 = VMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 1));
        vResult = _mm_mul_ps(vResult, vTemp1);

        // V2ywxz * V3wxwx
        vTemp2 = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 3, 1));
        vTemp3 = VMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 3, 0, 3));
        vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
        // - V2wxwx * V3ywxz
        vTemp2 = VMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 1, 2, 1));
        vTemp1 = VMATH_PERMUTE_PS(V3, _MM_SHUFFLE(2, 0, 3, 1));
        vTemp3 = VMATH_FNMADD_PS(vTemp2, vTemp1, vTemp3);
        // vResult - temp * V1zzyy
        vTemp1 = VMATH_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 2, 2));
        vResult = VMATH_FNMADD_PS(vTemp1, vTemp3, vResult);

        // V2yzxy * V3zxyx
        vTemp2 = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(1, 0, 2, 1));
        vTemp3 = VMATH_PERMUTE_PS(V3, _MM_SHUFFLE(0, 1, 0, 2));
        vTemp3 = _mm_mul_ps(vTemp3, vTemp2);
        // - V2zxyx * V3yzxy
        vTemp2 = VMATH_PERMUTE_PS(vTemp2, _MM_SHUFFLE(2, 0, 2, 1));
        vTemp1 = VMATH_PERMUTE_PS(V3, _MM_SHUFFLE(1, 0, 2, 1));
        vTemp3 = VMATH_FNMADD_PS(vTemp1, vTemp2, vTemp3);
        // vResult + term * V1wwwz
        vTemp1 = VMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 3, 3, 3));
        vResult = VMATH_FMADD_PS(vTemp3, vTemp1, vResult);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4LengthSq(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4LengthSq(vfloat4 V)
    {
        return v4Dot(V, V);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4ReciprocalLengthEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4RLengthEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v4LengthSq(V);
        Result = v4RSqrtEst(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot4
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        // Reciprocal sqrt (estimate)
        v2 = vrsqrte_f32(v1);
        return vcombine_f32(v2, v2);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0xff);
        return _mm_rsqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_rsqrt_ps(vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and w
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
        // x+z, y+w
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // x+z,x+z,x+z,y+w
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
        // ??,??,y+w,y+w
        vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
        // ??,??,x+z+y+w,??
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // Splat the length
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
        // Get the reciprocal
        vLengthSq = _mm_rsqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4ReciprocalLength(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4RLength(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v4LengthSq(V);
        Result = v4RSqrt(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot4
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        // Reciprocal sqrt
        float32x2_t  S0 = vrsqrte_f32(v1);
        float32x2_t  P0 = vmul_f32(v1, S0);
        float32x2_t  R0 = vrsqrts_f32(P0, S0);
        float32x2_t  S1 = vmul_f32(S0, R0);
        float32x2_t  P1 = vmul_f32(v1, S1);
        float32x2_t  R1 = vrsqrts_f32(P1, S1);
        float32x2_t Result = vmul_f32(S1, R1);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0xff);
        vfloat4 vLengthSq = _mm_sqrt_ps(vTemp);
        return _mm_div_ps(Internal::kOne, vLengthSq);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        vLengthSq = _mm_div_ps(Internal::kOne, vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and w
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
        // x+z, y+w
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // x+z,x+z,x+z,y+w
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
        // ??,??,y+w,y+w
        vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
        // ??,??,x+z+y+w,??
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // Splat the length
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
        // Get the reciprocal
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        // Accurate!
        vLengthSq = _mm_div_ps(Internal::kOne, vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4LengthEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4LengthEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v4LengthSq(V);
        Result = v4SqrtEst(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot4
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        const float32x2_t zero = vdup_n_f32(0);
        uint32x2_t VEqualsZero = vceq_f32(v1, zero);
        // Sqrt (estimate)
        float32x2_t Result = vrsqrte_f32(v1);
        Result = vmul_f32(v1, Result);
        Result = vbsl_f32(VEqualsZero, zero, Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0xff);
        return _mm_sqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and w
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
        // x+z, y+w
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // x+z,x+z,x+z,y+w
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
        // ??,??,y+w,y+w
        vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
        // ??,??,x+z+y+w,??
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // Splat the length
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
        // Get the length
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Length(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Length(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;

        Result = v4LengthSq(V);
        Result = v4Sqrt(Result);

        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot4
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        const float32x2_t zero = vdup_n_f32(0);
        uint32x2_t VEqualsZero = vceq_f32(v1, zero);
        // Sqrt
        float32x2_t S0 = vrsqrte_f32(v1);
        float32x2_t P0 = vmul_f32(v1, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(v1, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        float32x2_t Result = vmul_f32(S1, R1);
        Result = vmul_f32(v1, Result);
        Result = vbsl_f32(VEqualsZero, zero, Result);
        return vcombine_f32(Result, Result);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0xff);
        return _mm_sqrt_ps(vTemp);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and w
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
        // x+z, y+w
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // x+z,x+z,x+z,y+w
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
        // ??,??,y+w,y+w
        vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
        // ??,??,x+z+y+w,??
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // Splat the length
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
        // Get the length
        vLengthSq = _mm_sqrt_ps(vLengthSq);
        return vLengthSq;
#endif
    }

    //------------------------------------------------------------------------------
    // XMVector4NormalizeEst uses a reciprocal estimate and returns QNaN on zero and infinite vectors.
    // XMVECTOR XMVector4NormalizeEst(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4NormalizeEst(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4 Result;
        Result = v4RLength(V);
        Result = v4Mul(V, Result);
        return Result;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot4
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        // Reciprocal sqrt (estimate)
        v2 = vrsqrte_f32(v1);
        // Normalize
        return vmulq_f32(V, vcombine_f32(v2, v2));
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(V, V, 0xff);
        vfloat4 vResult = _mm_rsqrt_ps(vTemp);
        return _mm_mul_ps(vResult, V);
#elif defined(VMATH_SSE3_INTRINSICS)
        vfloat4 vDot = _mm_mul_ps(V, V);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_hadd_ps(vDot, vDot);
        vDot = _mm_rsqrt_ps(vDot);
        vDot = _mm_mul_ps(vDot, V);
        return vDot;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and w
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
        // x+z, y+w
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // x+z,x+z,x+z,y+w
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
        // ??,??,y+w,y+w
        vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
        // ??,??,x+z+y+w,??
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // Splat the length
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
        // Get the reciprocal
        vfloat4 vResult = _mm_rsqrt_ps(vLengthSq);
        // Reciprocal mul to perform the normalization
        vResult = _mm_mul_ps(vResult, V);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Normalize(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Normalize(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)
        float fLength;
        vfloat4 vResult;

        vResult = v4Length(V);
        fLength = vResult.v4f[0];

        // Prevent divide by zero
        if (fLength > 0)
        {
            fLength = 1.0f / fLength;
        }

        vResult.v4f[0] = V.v4f[0] * fLength;
        vResult.v4f[1] = V.v4f[1] * fLength;
        vResult.v4f[2] = V.v4f[2] * fLength;
        vResult.v4f[3] = V.v4f[3] * fLength;
        return vResult;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Dot4
        float32x4_t vTemp = vmulq_f32(V, V);
        float32x2_t v1 = vget_low_f32(vTemp);
        float32x2_t v2 = vget_high_f32(vTemp);
        v1 = vadd_f32(v1, v2);
        v1 = vpadd_f32(v1, v1);
        uint32x2_t VEqualsZero = vceq_f32(v1, vdup_n_f32(0));
        uint32x2_t VEqualsInf = vceq_f32(v1, vget_low_f32(Internal::kInfinity));
        // Reciprocal sqrt (2 iterations of Newton-Raphson)
        float32x2_t S0 = vrsqrte_f32(v1);
        float32x2_t P0 = vmul_f32(v1, S0);
        float32x2_t R0 = vrsqrts_f32(P0, S0);
        float32x2_t S1 = vmul_f32(S0, R0);
        float32x2_t P1 = vmul_f32(v1, S1);
        float32x2_t R1 = vrsqrts_f32(P1, S1);
        v2 = vmul_f32(S1, R1);
        // Normalize
        vfloat4 vResult = vmulq_f32(V, vcombine_f32(v2, v2));
        vResult = vbslq_f32(vcombine_u32(VEqualsZero, VEqualsZero), vdupq_n_f32(0), vResult);
        return vbslq_f32(vcombine_u32(VEqualsInf, VEqualsInf), Internal::kQNaN, vResult);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vLengthSq = _mm_dp_ps(V, V, 0xff);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Divide to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#elif defined(VMATH_SSE3_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        vLengthSq = _mm_hadd_ps(vLengthSq, vLengthSq);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Divide to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y,z and w
        vfloat4 vLengthSq = _mm_mul_ps(V, V);
        // vTemp has z and w
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
        // x+z, y+w
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // x+z,x+z,x+z,y+w
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
        // ??,??,y+w,y+w
        vTemp = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
        // ??,??,x+z+y+w,??
        vLengthSq = _mm_add_ps(vLengthSq, vTemp);
        // Splat the length
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Create zero with a single instruction
        vfloat4 vZeroMask = _mm_setzero_ps();
        // Test for a divide by zero (Must be FP to detect -0.0)
        vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Divide to perform the normalization
        vResult = _mm_div_ps(V, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vZeroMask);
        // Select qnan or result based on infinite length
        vfloat4 vTemp1 = _mm_andnot_ps(vLengthSq, Internal::kQNaN);
        vfloat4 vTemp2 = _mm_and_ps(vResult, vLengthSq);
        vResult = _mm_or_ps(vTemp1, vTemp2);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4ClampLength(FXMVECTOR V, float LengthMin, float LengthMax)
    inline vfloat4 VMATH_CALLCONV v4ClampLength(vfloat4 V, float LengthMin, float LengthMax)
    {
        vfloat4 ClampMax = v4Spread(LengthMax);
        vfloat4 ClampMin = v4Spread(LengthMin);

        return v4ClampLength(V, ClampMin, ClampMax);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4ClampLengthV(FXMVECTOR V, FXMVECTOR LengthMin, FXMVECTOR LengthMax)
    inline vfloat4 VMATH_CALLCONV v4ClampLength(vfloat4 V, vfloat4 LengthMin, vfloat4 LengthMax)
    {
        assert((v4GetY(LengthMin) == v4GetX(LengthMin)) && (v4GetZ(LengthMin) == v4GetX(LengthMin)) && (v4GetW(LengthMin) == v4GetX(LengthMin)));
        assert((v4GetY(LengthMax) == v4GetX(LengthMax)) && (v4GetZ(LengthMax) == v4GetX(LengthMax)) && (v4GetW(LengthMax) == v4GetX(LengthMax)));
        assert(v4AllGreaterEqual(LengthMin, v4Zero()));
        assert(v4AllGreaterEqual(LengthMax, v4Zero()));
        assert(v4AllGreaterEqual(LengthMax, LengthMin));

        vfloat4 LengthSq = v4LengthSq(V);

        const vfloat4 Zero = v4Zero();

        vfloat4 RcpLength = v4RSqrt(LengthSq);

        vfloat4 InfiniteLength = v4BinaryEqual(LengthSq, Internal::kInfinity.v);
        vfloat4 ZeroLength = v4Equal(LengthSq, Zero);

        vfloat4 Normal = v4Mul(V, RcpLength);

        vfloat4 Length = v4Mul(LengthSq, RcpLength);

        vfloat4 Select = v4BinaryEqual(InfiniteLength, ZeroLength);
        Length = v4Select(LengthSq, Length, Select);
        Normal = v4Select(LengthSq, Normal, Select);

        vfloat4 ControlMax = v4Greater(Length, LengthMax);
        vfloat4 ControlMin = v4Less(Length, LengthMin);

        vfloat4 ClampLength = v4Select(Length, LengthMax, ControlMax);
        ClampLength = v4Select(ClampLength, LengthMin, ControlMin);

        vfloat4 Result = v4Mul(Normal, ClampLength);

        // Preserve the original vector (with no precision loss) if the length falls within the given range
        vfloat4 Control = v4BinaryEqual(ControlMax, ControlMin);
        Result = v4Select(Result, V, Control);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Reflect(FXMVECTOR Incident, FXMVECTOR Normal)
    inline vfloat4 VMATH_CALLCONV v4Reflect(vfloat4 Incident, vfloat4 Normal)
    {
        // Result = Incident - (2 * dot(Incident, Normal)) * Normal

        vfloat4 Result = v4Dot(Incident, Normal);
        Result = v4Add(Result, Result);
        Result = v4NegateMulAdd(Result, Normal, Incident);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Refract(FXMVECTOR Incident, FXMVECTOR Normal, float RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v4Refract(vfloat4 Incident, vfloat4 Normal, float RefractionIndex)
    {
        vfloat4 Index = v4Spread(RefractionIndex);
        return v4Refract(Incident, Normal, Index);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4RefractV(FXMVECTOR Incident, FXMVECTOR Normal, FXMVECTOR RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v4Refract(vfloat4 Incident, vfloat4 Normal, vfloat4 RefractionIndex)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4        IDotN;
        vfloat4        R;
        const vfloat4  Zero = v4Zero();

        // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) +
        // sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))

        IDotN = v4Dot(Incident, Normal);

        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        R = v4NegateMulAdd(IDotN, IDotN, Internal::kOne.v);
        R = v4Mul(R, RefractionIndex);
        R = v4NegateMulAdd(R, RefractionIndex, Internal::kOne.v);

        if (v4AllLessEqual(R, Zero))
        {
            // Total internal reflection
            return Zero;
        }
        else
        {
            vfloat4 Result;

            // R = RefractionIndex * IDotN + sqrt(R)
            R = v4Sqrt(R);
            R = v4MulAdd(RefractionIndex, IDotN, R);

            // Result = RefractionIndex * Incident - Normal * R
            Result = v4Mul(RefractionIndex, Incident);
            Result = v4NegateMulAdd(Normal, R, Result);

            return Result;
        }

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 IDotN = v4Dot(Incident, Normal);

        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        float32x4_t R = vmlsq_f32(Internal::kOne, IDotN, IDotN);
        R = vmulq_f32(R, RefractionIndex);
        R = vmlsq_f32(Internal::kOne, R, RefractionIndex);

        uint32x4_t isrzero = vcleq_f32(R, Internal::kZero);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(isrzero)), vget_high_u8(vreinterpretq_u8_u32(isrzero)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));

        float32x4_t vResult;
        if (vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1) == 0xFFFFFFFFU)
        {
            // Total internal reflection
            vResult = Internal::kZero;
        }
        else
        {
            // Sqrt(R)
            float32x4_t S0 = vrsqrteq_f32(R);
            float32x4_t P0 = vmulq_f32(R, S0);
            float32x4_t R0 = vrsqrtsq_f32(P0, S0);
            float32x4_t S1 = vmulq_f32(S0, R0);
            float32x4_t P1 = vmulq_f32(R, S1);
            float32x4_t R1 = vrsqrtsq_f32(P1, S1);
            float32x4_t S2 = vmulq_f32(S1, R1);
            R = vmulq_f32(R, S2);
            // R = RefractionIndex * IDotN + sqrt(R)
            R = vmlaq_f32(R, RefractionIndex, IDotN);
            // Result = RefractionIndex * Incident - Normal * R
            vResult = vmulq_f32(RefractionIndex, Incident);
            vResult = vmlsq_f32(vResult, R, Normal);
        }
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 IDotN = v4Dot(Incident, Normal);

        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        vfloat4 R = VMATH_FNMADD_PS(IDotN, IDotN, Internal::kOne);
        vfloat4 R2 = _mm_mul_ps(RefractionIndex, RefractionIndex);
        R = VMATH_FNMADD_PS(R, R2, Internal::kOne);

        vfloat4 vResult = _mm_cmple_ps(R, Internal::kZero);
        if (_mm_movemask_ps(vResult) == 0x0f)
        {
            // Total internal reflection
            vResult = Internal::kZero;
        }
        else
        {
            // R = RefractionIndex * IDotN + sqrt(R)
            R = _mm_sqrt_ps(R);
            R = VMATH_FMADD_PS(RefractionIndex, IDotN, R);
            // Result = RefractionIndex * Incident - Normal * R
            vResult = _mm_mul_ps(RefractionIndex, Incident);
            vResult = VMATH_FNMADD_PS(R, Normal, vResult);
        }
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Orthogonal(FXMVECTOR V)
    inline vfloat4 VMATH_CALLCONV v4Orthogonal(vfloat4 V)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 Result = { { {
                V.v4f[2],
                V.v4f[3],
                -V.v4f[0],
                -V.v4f[1]
            } } };
        return Result.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 Negate = { { { 1.f, 1.f, -1.f, -1.f } } };

        float32x4_t Result = vcombine_f32(vget_high_f32(V), vget_low_f32(V));
        return vmulq_f32(Result, Negate);
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 FlipZW = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 0, 3, 2));
        vResult = _mm_mul_ps(vResult, FlipZW);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4AngleBetweenNormalsEst(FXMVECTOR N1, FXMVECTOR N2)
    inline vfloat4 VMATH_CALLCONV v4AngleBetweenNormalsEst(vfloat4 N1, vfloat4 N2)
    {
        vfloat4 Result = v4Dot(N1, N2);
        Result = v4Clamp(Result, Internal::kNegativeOne.v, Internal::kOne.v);
        Result = v4ACosEst(Result);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4AngleBetweenNormals(FXMVECTOR N1, FXMVECTOR N2)
    inline vfloat4 VMATH_CALLCONV v4AngleBetweenNormals(vfloat4 N1, vfloat4 N2)
    {
        vfloat4 Result = v4Dot(N1, N2);
        Result = v4Clamp(Result, Internal::kNegativeOne.v, Internal::kOne.v);
        Result = v4ACos(Result);
        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4AngleBetweenVectors(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4 VMATH_CALLCONV v4AngleBetweenVectors(vfloat4 V1, vfloat4 V2)
    {
        vfloat4 L1 = v4RLength(V1);
        vfloat4 L2 = v4RLength(V2);

        vfloat4 Dot = v4Dot(V1, V2);

        L1 = v4Mul(L1, L2);

        vfloat4 CosAngle = v4Mul(Dot, L1);
        CosAngle = v4Clamp(CosAngle, Internal::kNegativeOne.v, Internal::kOne.v);

        return v4ACos(CosAngle);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMVector4Transform(FXMVECTOR V, FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV v4Transform(vfloat4 V, vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)

        float fX = (M.m[0][0] * V.v4f[0]) + (M.m[1][0] * V.v4f[1]) + (M.m[2][0] * V.v4f[2]) + (M.m[3][0] * V.v4f[3]);
        float fY = (M.m[0][1] * V.v4f[0]) + (M.m[1][1] * V.v4f[1]) + (M.m[2][1] * V.v4f[2]) + (M.m[3][1] * V.v4f[3]);
        float fZ = (M.m[0][2] * V.v4f[0]) + (M.m[1][2] * V.v4f[1]) + (M.m[2][2] * V.v4f[2]) + (M.m[3][2] * V.v4f[3]);
        float fW = (M.m[0][3] * V.v4f[0]) + (M.m[1][3] * V.v4f[1]) + (M.m[2][3] * V.v4f[2]) + (M.m[3][3] * V.v4f[3]);
        Internal::vfloat4f32 vResult = { { { fX, fY, fZ, fW } } };
        return vResult.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(V);
        vfloat4 vResult = vmulq_lane_f32(M.r[0], VL, 0); // X
        vResult = vmlaq_lane_f32(vResult, M.r[1], VL, 1); // Y
        float32x2_t VH = vget_high_f32(V);
        vResult = vmlaq_lane_f32(vResult, M.r[2], VH, 0); // Z
        return vmlaq_lane_f32(vResult, M.r[3], VH, 1); // W
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vResult = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3)); // W
        vResult = _mm_mul_ps(vResult, M.r[3]);
        vfloat4 vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2)); // Z
        vResult = VMATH_FMADD_PS(vTemp, M.r[2], vResult);
        vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1)); // Y
        vResult = VMATH_FMADD_PS(vTemp, M.r[1], vResult);
        vTemp = VMATH_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0)); // X
        vResult = VMATH_FMADD_PS(vTemp, M.r[0], vResult);
        return vResult;
#endif
    }

#if defined(VMATH_NO_INTRINSICS)
#undef VMATH_ISNAN
#undef VMATH_ISINF
#endif

#if defined(VMATH_SSE_INTRINSICS)
#undef VMATH_3UNPACK3INTO4
#undef VMATH_3PACK4INTO3
#endif

    /****************************************************************************
     *
     * Matrix
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------
     // Comparison operations
     //------------------------------------------------------------------------------

     //------------------------------------------------------------------------------

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

    // Return true if any entry in the matrix is NaN
    // bool XMMatrixIsNaN(FXMMATRIX M)
    inline bool VMATH_CALLCONV vMat4IsAnyNaN(vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)
        size_t i = 16;
        auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
        do {
            // Fetch value into integer unit
            uint32_t uTest = pWork[0];
            // Remove sign
            uTest &= 0x7FFFFFFFU;
            // NaN is 0x7F800001 through 0x7FFFFFFF inclusive
            uTest -= 0x7F800001U;
            if (uTest < 0x007FFFFFU)
            {
                break;      // NaN found
            }
            ++pWork;        // Next entry
        } while (--i);
        return (i != 0);      // i == 0 if nothing matched
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Load in registers
        float32x4_t vX = M.r[0];
        float32x4_t vY = M.r[1];
        float32x4_t vZ = M.r[2];
        float32x4_t vW = M.r[3];
        // Test themselves to check for NaN
        uint32x4_t xmask = vmvnq_u32(vceqq_f32(vX, vX));
        uint32x4_t ymask = vmvnq_u32(vceqq_f32(vY, vY));
        uint32x4_t zmask = vmvnq_u32(vceqq_f32(vZ, vZ));
        uint32x4_t wmask = vmvnq_u32(vceqq_f32(vW, vW));
        // Or all the results
        xmask = vorrq_u32(xmask, zmask);
        ymask = vorrq_u32(ymask, wmask);
        xmask = vorrq_u32(xmask, ymask);
        // If any tested true, return true
        uint8x8x2_t vTemp = vzip_u8(
            vget_low_u8(vreinterpretq_u8_u32(xmask)),
            vget_high_u8(vreinterpretq_u8_u32(xmask)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        uint32_t r = vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1);
        return (r != 0);
#elif defined(VMATH_SSE_INTRINSICS)
        // Load in registers
        vfloat4 vX = M.r[0];
        vfloat4 vY = M.r[1];
        vfloat4 vZ = M.r[2];
        vfloat4 vW = M.r[3];
        // Test themselves to check for NaN
        vX = _mm_cmpneq_ps(vX, vX);
        vY = _mm_cmpneq_ps(vY, vY);
        vZ = _mm_cmpneq_ps(vZ, vZ);
        vW = _mm_cmpneq_ps(vW, vW);
        // Or all the results
        vX = _mm_or_ps(vX, vZ);
        vY = _mm_or_ps(vY, vW);
        vX = _mm_or_ps(vX, vY);
        // If any tested true, return true
        return (_mm_movemask_ps(vX) != 0);
#else
#endif
    }

#if !defined(VMATH_NO_INTRINSICS) && defined(_MSC_VER) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#pragma float_control(pop)
#endif

    //------------------------------------------------------------------------------

    // Return true if any entry in the matrix is +/-INF
    // bool XMMatrixIsInfinite(FXMMATRIX M)
    inline bool VMATH_CALLCONV vMat4IsAnyInfinite(vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)
        size_t i = 16;
        auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
        do {
            // Fetch value into integer unit
            uint32_t uTest = pWork[0];
            // Remove sign
            uTest &= 0x7FFFFFFFU;
            // INF is 0x7F800000
            if (uTest == 0x7F800000U)
            {
                break;      // INF found
            }
            ++pWork;        // Next entry
        } while (--i);
        return (i != 0);      // i == 0 if nothing matched
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        // Load in registers
        float32x4_t vX = M.r[0];
        float32x4_t vY = M.r[1];
        float32x4_t vZ = M.r[2];
        float32x4_t vW = M.r[3];
        // Mask off the sign bits
        vX = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(vX), Internal::kAbsMask));
        vY = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(vY), Internal::kAbsMask));
        vZ = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(vZ), Internal::kAbsMask));
        vW = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(vW), Internal::kAbsMask));
        // Compare to infinity
        uint32x4_t xmask = vceqq_f32(vX, Internal::kInfinity);
        uint32x4_t ymask = vceqq_f32(vY, Internal::kInfinity);
        uint32x4_t zmask = vceqq_f32(vZ, Internal::kInfinity);
        uint32x4_t wmask = vceqq_f32(vW, Internal::kInfinity);
        // Or the answers together
        xmask = vorrq_u32(xmask, zmask);
        ymask = vorrq_u32(ymask, wmask);
        xmask = vorrq_u32(xmask, ymask);
        // If any tested true, return true
        uint8x8x2_t vTemp = vzip_u8(
            vget_low_u8(vreinterpretq_u8_u32(xmask)),
            vget_high_u8(vreinterpretq_u8_u32(xmask)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        uint32_t r = vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1);
        return (r != 0);
#elif defined(VMATH_SSE_INTRINSICS)
        // Mask off the sign bits
        vfloat4 vTemp1 = _mm_and_ps(M.r[0], Internal::kAbsMask);
        vfloat4 vTemp2 = _mm_and_ps(M.r[1], Internal::kAbsMask);
        vfloat4 vTemp3 = _mm_and_ps(M.r[2], Internal::kAbsMask);
        vfloat4 vTemp4 = _mm_and_ps(M.r[3], Internal::kAbsMask);
        // Compare to infinity
        vTemp1 = _mm_cmpeq_ps(vTemp1, Internal::kInfinity);
        vTemp2 = _mm_cmpeq_ps(vTemp2, Internal::kInfinity);
        vTemp3 = _mm_cmpeq_ps(vTemp3, Internal::kInfinity);
        vTemp4 = _mm_cmpeq_ps(vTemp4, Internal::kInfinity);
        // Or the answers together
        vTemp1 = _mm_or_ps(vTemp1, vTemp2);
        vTemp3 = _mm_or_ps(vTemp3, vTemp4);
        vTemp1 = _mm_or_ps(vTemp1, vTemp3);
        // If any are infinity, the signs are true.
        return (_mm_movemask_ps(vTemp1) != 0);
#endif
    }

    //------------------------------------------------------------------------------

    // Return true if the XMMatrix is equal to identity
    // bool XMMatrixIsIdentity(FXMMATRIX M)
    inline bool VMATH_CALLCONV vMat4IsIdentity(vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)
        // Use the integer pipeline to reduce branching to a minimum
        auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
        // Convert 1.0f to zero and or them together
        uint32_t uOne = pWork[0] ^ 0x3F800000U;
        // Or all the 0.0f entries together
        uint32_t uZero = pWork[1];
        uZero |= pWork[2];
        uZero |= pWork[3];
        // 2nd row
        uZero |= pWork[4];
        uOne |= pWork[5] ^ 0x3F800000U;
        uZero |= pWork[6];
        uZero |= pWork[7];
        // 3rd row
        uZero |= pWork[8];
        uZero |= pWork[9];
        uOne |= pWork[10] ^ 0x3F800000U;
        uZero |= pWork[11];
        // 4th row
        uZero |= pWork[12];
        uZero |= pWork[13];
        uZero |= pWork[14];
        uOne |= pWork[15] ^ 0x3F800000U;
        // If all zero entries are zero, the uZero==0
        uZero &= 0x7FFFFFFF;    // Allow -0.0f
        // If all 1.0f entries are 1.0f, then uOne==0
        uOne |= uZero;
        return (uOne == 0);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t xmask = vceqq_f32(M.r[0], Internal::kIdentityR0);
        uint32x4_t ymask = vceqq_f32(M.r[1], Internal::kIdentityR1);
        uint32x4_t zmask = vceqq_f32(M.r[2], Internal::kIdentityR2);
        uint32x4_t wmask = vceqq_f32(M.r[3], Internal::kIdentityR3);
        xmask = vandq_u32(xmask, zmask);
        ymask = vandq_u32(ymask, wmask);
        xmask = vandq_u32(xmask, ymask);
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vreinterpretq_u8_u32(xmask)), vget_high_u8(vreinterpretq_u8_u32(xmask)));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        uint32_t r = vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1);
        return (r == 0xFFFFFFFFU);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vTemp1 = _mm_cmpeq_ps(M.r[0], Internal::kIdentityR0);
        vfloat4 vTemp2 = _mm_cmpeq_ps(M.r[1], Internal::kIdentityR1);
        vfloat4 vTemp3 = _mm_cmpeq_ps(M.r[2], Internal::kIdentityR2);
        vfloat4 vTemp4 = _mm_cmpeq_ps(M.r[3], Internal::kIdentityR3);
        vTemp1 = _mm_and_ps(vTemp1, vTemp2);
        vTemp3 = _mm_and_ps(vTemp3, vTemp4);
        vTemp1 = _mm_and_ps(vTemp1, vTemp3);
        return (_mm_movemask_ps(vTemp1) == 0x0f);
#endif
    }

    //------------------------------------------------------------------------------
    // Computation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // Perform a 4x4 matrix multiply by a 4x4 matrix
    // XMMATRIX XMMatrixMultiply(FXMMATRIX M1, CXMMATRIX M2)
    inline vfloat4x4 VMATH_CALLCONV vMat4Mul(vfloat4x4 M1, vfloat4x4 M2)
    {
#if defined(VMATH_NO_INTRINSICS)
        vfloat4x4 mResult;
        // Cache the invariants in registers
        float x = M1.m[0][0];
        float y = M1.m[0][1];
        float z = M1.m[0][2];
        float w = M1.m[0][3];
        // Perform the operation on the first row
        mResult.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
        mResult.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
        mResult.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
        mResult.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
        // Repeat for all the other rows
        x = M1.m[1][0];
        y = M1.m[1][1];
        z = M1.m[1][2];
        w = M1.m[1][3];
        mResult.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
        mResult.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
        mResult.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
        mResult.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
        x = M1.m[2][0];
        y = M1.m[2][1];
        z = M1.m[2][2];
        w = M1.m[2][3];
        mResult.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
        mResult.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
        mResult.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
        mResult.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
        x = M1.m[3][0];
        y = M1.m[3][1];
        z = M1.m[3][2];
        w = M1.m[3][3];
        mResult.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
        mResult.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
        mResult.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
        mResult.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
        return mResult;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4x4 mResult;
        float32x2_t VL = vget_low_f32(M1.r[0]);
        float32x2_t VH = vget_high_f32(M1.r[0]);
        // Perform the operation on the first row
        float32x4_t vX = vmulq_lane_f32(M2.r[0], VL, 0);
        float32x4_t vY = vmulq_lane_f32(M2.r[1], VL, 1);
        float32x4_t vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        float32x4_t vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        mResult.r[0] = vaddq_f32(vZ, vW);
        // Repeat for the other 3 rows
        VL = vget_low_f32(M1.r[1]);
        VH = vget_high_f32(M1.r[1]);
        vX = vmulq_lane_f32(M2.r[0], VL, 0);
        vY = vmulq_lane_f32(M2.r[1], VL, 1);
        vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        mResult.r[1] = vaddq_f32(vZ, vW);
        VL = vget_low_f32(M1.r[2]);
        VH = vget_high_f32(M1.r[2]);
        vX = vmulq_lane_f32(M2.r[0], VL, 0);
        vY = vmulq_lane_f32(M2.r[1], VL, 1);
        vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        mResult.r[2] = vaddq_f32(vZ, vW);
        VL = vget_low_f32(M1.r[3]);
        VH = vget_high_f32(M1.r[3]);
        vX = vmulq_lane_f32(M2.r[0], VL, 0);
        vY = vmulq_lane_f32(M2.r[1], VL, 1);
        vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        mResult.r[3] = vaddq_f32(vZ, vW);
        return mResult;
#elif defined(VMATH_AVX2_INTRINSICS)
        __m256 t0 = _mm256_castps128_ps256(M1.r[0]);
        t0 = _mm256_insertf128_ps(t0, M1.r[1], 1);
        __m256 t1 = _mm256_castps128_ps256(M1.r[2]);
        t1 = _mm256_insertf128_ps(t1, M1.r[3], 1);

        __m256 u0 = _mm256_castps128_ps256(M2.r[0]);
        u0 = _mm256_insertf128_ps(u0, M2.r[1], 1);
        __m256 u1 = _mm256_castps128_ps256(M2.r[2]);
        u1 = _mm256_insertf128_ps(u1, M2.r[3], 1);

        __m256 a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0));
        __m256 a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0));
        __m256 b0 = _mm256_permute2f128_ps(u0, u0, 0x00);
        __m256 c0 = _mm256_mul_ps(a0, b0);
        __m256 c1 = _mm256_mul_ps(a1, b0);

        a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1));
        a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1));
        b0 = _mm256_permute2f128_ps(u0, u0, 0x11);
        __m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
        __m256 c3 = _mm256_fmadd_ps(a1, b0, c1);

        a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2));
        a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2));
        __m256 b1 = _mm256_permute2f128_ps(u1, u1, 0x00);
        __m256 c4 = _mm256_mul_ps(a0, b1);
        __m256 c5 = _mm256_mul_ps(a1, b1);

        a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3));
        a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3));
        b1 = _mm256_permute2f128_ps(u1, u1, 0x11);
        __m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
        __m256 c7 = _mm256_fmadd_ps(a1, b1, c5);

        t0 = _mm256_add_ps(c2, c6);
        t1 = _mm256_add_ps(c3, c7);

        vfloat4x4 mResult;
        mResult.r[0] = _mm256_castps256_ps128(t0);
        mResult.r[1] = _mm256_extractf128_ps(t0, 1);
        mResult.r[2] = _mm256_castps256_ps128(t1);
        mResult.r[3] = _mm256_extractf128_ps(t1, 1);
        return mResult;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 mResult;
        // Splat the component X,Y,Z then W
#if defined(VMATH_AVX_INTRINSICS)
        vfloat4 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
        vfloat4 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
        vfloat4 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
        vfloat4 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
    // Use vW to hold the original row
        vfloat4 vW = M1.r[0];
        vfloat4 vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vfloat4 vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vfloat4 vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        // Perform the operation on the first row
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        // Perform a binary add to reduce cumulative errors
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        mResult.r[0] = vX;
        // Repeat for the other 3 rows
#if defined(VMATH_AVX_INTRINSICS)
        vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
        vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
        vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
        vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
        vW = M1.r[1];
        vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        mResult.r[1] = vX;
#if defined(VMATH_AVX_INTRINSICS)
        vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
        vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
        vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
        vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
        vW = M1.r[2];
        vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        mResult.r[2] = vX;
#if defined(VMATH_AVX_INTRINSICS)
        vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
        vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
        vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
        vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
        vW = M1.r[3];
        vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        mResult.r[3] = vX;
        return mResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixMultiplyTranspose(FXMMATRIX M1, CXMMATRIX M2)
    inline vfloat4x4 VMATH_CALLCONV vMat4MulTranspose(vfloat4x4 M1, vfloat4x4 M2)
    {
#if defined(VMATH_NO_INTRINSICS)
        vfloat4x4 mResult;
        // Cache the invariants in registers
        float x = M2.m[0][0];
        float y = M2.m[1][0];
        float z = M2.m[2][0];
        float w = M2.m[3][0];
        // Perform the operation on the first row
        mResult.m[0][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
        mResult.m[0][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
        mResult.m[0][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
        mResult.m[0][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
        // Repeat for all the other rows
        x = M2.m[0][1];
        y = M2.m[1][1];
        z = M2.m[2][1];
        w = M2.m[3][1];
        mResult.m[1][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
        mResult.m[1][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
        mResult.m[1][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
        mResult.m[1][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
        x = M2.m[0][2];
        y = M2.m[1][2];
        z = M2.m[2][2];
        w = M2.m[3][2];
        mResult.m[2][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
        mResult.m[2][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
        mResult.m[2][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
        mResult.m[2][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
        x = M2.m[0][3];
        y = M2.m[1][3];
        z = M2.m[2][3];
        w = M2.m[3][3];
        mResult.m[3][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
        mResult.m[3][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
        mResult.m[3][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
        mResult.m[3][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
        return mResult;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x2_t VL = vget_low_f32(M1.r[0]);
        float32x2_t VH = vget_high_f32(M1.r[0]);
        // Perform the operation on the first row
        float32x4_t vX = vmulq_lane_f32(M2.r[0], VL, 0);
        float32x4_t vY = vmulq_lane_f32(M2.r[1], VL, 1);
        float32x4_t vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        float32x4_t vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        float32x4_t r0 = vaddq_f32(vZ, vW);
        // Repeat for the other 3 rows
        VL = vget_low_f32(M1.r[1]);
        VH = vget_high_f32(M1.r[1]);
        vX = vmulq_lane_f32(M2.r[0], VL, 0);
        vY = vmulq_lane_f32(M2.r[1], VL, 1);
        vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        float32x4_t r1 = vaddq_f32(vZ, vW);
        VL = vget_low_f32(M1.r[2]);
        VH = vget_high_f32(M1.r[2]);
        vX = vmulq_lane_f32(M2.r[0], VL, 0);
        vY = vmulq_lane_f32(M2.r[1], VL, 1);
        vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        float32x4_t r2 = vaddq_f32(vZ, vW);
        VL = vget_low_f32(M1.r[3]);
        VH = vget_high_f32(M1.r[3]);
        vX = vmulq_lane_f32(M2.r[0], VL, 0);
        vY = vmulq_lane_f32(M2.r[1], VL, 1);
        vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
        vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
        float32x4_t r3 = vaddq_f32(vZ, vW);

        // Transpose result
        float32x4x2_t P0 = vzipq_f32(r0, r2);
        float32x4x2_t P1 = vzipq_f32(r1, r3);

        float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
        float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

        vfloat4x4 mResult;
        mResult.r[0] = T0.val[0];
        mResult.r[1] = T0.val[1];
        mResult.r[2] = T1.val[0];
        mResult.r[3] = T1.val[1];
        return mResult;
#elif defined(VMATH_AVX2_INTRINSICS)
        __m256 t0 = _mm256_castps128_ps256(M1.r[0]);
        t0 = _mm256_insertf128_ps(t0, M1.r[1], 1);
        __m256 t1 = _mm256_castps128_ps256(M1.r[2]);
        t1 = _mm256_insertf128_ps(t1, M1.r[3], 1);

        __m256 u0 = _mm256_castps128_ps256(M2.r[0]);
        u0 = _mm256_insertf128_ps(u0, M2.r[1], 1);
        __m256 u1 = _mm256_castps128_ps256(M2.r[2]);
        u1 = _mm256_insertf128_ps(u1, M2.r[3], 1);

        __m256 a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0));
        __m256 a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0));
        __m256 b0 = _mm256_permute2f128_ps(u0, u0, 0x00);
        __m256 c0 = _mm256_mul_ps(a0, b0);
        __m256 c1 = _mm256_mul_ps(a1, b0);

        a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1));
        a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1));
        b0 = _mm256_permute2f128_ps(u0, u0, 0x11);
        __m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
        __m256 c3 = _mm256_fmadd_ps(a1, b0, c1);

        a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2));
        a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2));
        __m256 b1 = _mm256_permute2f128_ps(u1, u1, 0x00);
        __m256 c4 = _mm256_mul_ps(a0, b1);
        __m256 c5 = _mm256_mul_ps(a1, b1);

        a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3));
        a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3));
        b1 = _mm256_permute2f128_ps(u1, u1, 0x11);
        __m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
        __m256 c7 = _mm256_fmadd_ps(a1, b1, c5);

        t0 = _mm256_add_ps(c2, c6);
        t1 = _mm256_add_ps(c3, c7);

        // Transpose result
        __m256 vTemp = _mm256_unpacklo_ps(t0, t1);
        __m256 vTemp2 = _mm256_unpackhi_ps(t0, t1);
        __m256 vTemp3 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x20);
        __m256 vTemp4 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x31);
        vTemp = _mm256_unpacklo_ps(vTemp3, vTemp4);
        vTemp2 = _mm256_unpackhi_ps(vTemp3, vTemp4);
        t0 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x20);
        t1 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x31);

        vfloat4x4 mResult;
        mResult.r[0] = _mm256_castps256_ps128(t0);
        mResult.r[1] = _mm256_extractf128_ps(t0, 1);
        mResult.r[2] = _mm256_castps256_ps128(t1);
        mResult.r[3] = _mm256_extractf128_ps(t1, 1);
        return mResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Splat the component X,Y,Z then W
#if defined(VMATH_AVX_INTRINSICS)
        vfloat4 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
        vfloat4 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
        vfloat4 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
        vfloat4 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
    // Use vW to hold the original row
        vfloat4 vW = M1.r[0];
        vfloat4 vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vfloat4 vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vfloat4 vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        // Perform the operation on the first row
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        // Perform a binary add to reduce cumulative errors
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        vfloat4 r0 = vX;
        // Repeat for the other 3 rows
#if defined(VMATH_AVX_INTRINSICS)
        vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
        vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
        vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
        vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
        vW = M1.r[1];
        vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        vfloat4 r1 = vX;
#if defined(VMATH_AVX_INTRINSICS)
        vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
        vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
        vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
        vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
        vW = M1.r[2];
        vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        vfloat4 r2 = vX;
#if defined(VMATH_AVX_INTRINSICS)
        vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
        vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
        vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
        vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
        vW = M1.r[3];
        vX = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
        vY = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
        vZ = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
        vW = VMATH_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
        vX = _mm_mul_ps(vX, M2.r[0]);
        vY = _mm_mul_ps(vY, M2.r[1]);
        vZ = _mm_mul_ps(vZ, M2.r[2]);
        vW = _mm_mul_ps(vW, M2.r[3]);
        vX = _mm_add_ps(vX, vZ);
        vY = _mm_add_ps(vY, vW);
        vX = _mm_add_ps(vX, vY);
        vfloat4 r3 = vX;

        // Transpose result
        // x.x,x.y,y.x,y.y
        vfloat4 vTemp1 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 1, 0));
        // x.z,x.w,y.z,y.w
        vfloat4 vTemp3 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(3, 2, 3, 2));
        // z.x,z.y,w.x,w.y
        vfloat4 vTemp2 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(1, 0, 1, 0));
        // z.z,z.w,w.z,w.w
        vfloat4 vTemp4 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(3, 2, 3, 2));

        vfloat4x4 mResult;
        // x.x,y.x,z.x,w.x
        mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
        // x.y,y.y,z.y,w.y
        mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
        // x.z,y.z,z.z,w.z
        mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
        // x.w,y.w,z.w,w.w
        mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
        return mResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixTranspose(FXMMATRIX M)
    inline vfloat4x4 VMATH_CALLCONV vMat4Transpose(vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)

        // Original matrix:
        //
        //     m00m01m02m03
        //     m10m11m12m13
        //     m20m21m22m23
        //     m30m31m32m33

        vfloat4x4 P;
        P.r[0] = v4Shuffle<0, 4, 1, 5>(M.r[0], M.r[2]); // m00m20m01m21
        P.r[1] = v4Shuffle<0, 4, 1, 5>(M.r[1], M.r[3]); // m10m30m11m31
        P.r[2] = v4Shuffle<2, 6, 3, 7>(M.r[0], M.r[2]); // m02m22m03m23
        P.r[3] = v4Shuffle<2, 6, 3, 7>(M.r[1], M.r[3]); // m12m32m13m33

        vfloat4x4 MT;
        MT.r[0] = v4Shuffle<0, 4, 1, 5>(P.r[0], P.r[1]); // m00m10m20m30
        MT.r[1] = v4Shuffle<2, 6, 3, 7>(P.r[0], P.r[1]); // m01m11m21m31
        MT.r[2] = v4Shuffle<0, 4, 1, 5>(P.r[2], P.r[3]); // m02m12m22m32
        MT.r[3] = v4Shuffle<2, 6, 3, 7>(P.r[2], P.r[3]); // m03m13m23m33
        return MT;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
        float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

        float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
        float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

        vfloat4x4 mResult;
        mResult.r[0] = T0.val[0];
        mResult.r[1] = T0.val[1];
        mResult.r[2] = T1.val[0];
        mResult.r[3] = T1.val[1];
        return mResult;
#elif defined(VMATH_AVX2_INTRINSICS)
        __m256 t0 = _mm256_castps128_ps256(M.r[0]);
        t0 = _mm256_insertf128_ps(t0, M.r[1], 1);
        __m256 t1 = _mm256_castps128_ps256(M.r[2]);
        t1 = _mm256_insertf128_ps(t1, M.r[3], 1);

        __m256 vTemp = _mm256_unpacklo_ps(t0, t1);
        __m256 vTemp2 = _mm256_unpackhi_ps(t0, t1);
        __m256 vTemp3 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x20);
        __m256 vTemp4 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x31);
        vTemp = _mm256_unpacklo_ps(vTemp3, vTemp4);
        vTemp2 = _mm256_unpackhi_ps(vTemp3, vTemp4);
        t0 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x20);
        t1 = _mm256_permute2f128_ps(vTemp, vTemp2, 0x31);

        vfloat4x4 mResult;
        mResult.r[0] = _mm256_castps256_ps128(t0);
        mResult.r[1] = _mm256_extractf128_ps(t0, 1);
        mResult.r[2] = _mm256_castps256_ps128(t1);
        mResult.r[3] = _mm256_extractf128_ps(t1, 1);
        return mResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // x.x,x.y,y.x,y.y
        vfloat4 vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
        // x.z,x.w,y.z,y.w
        vfloat4 vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
        // z.x,z.y,w.x,w.y
        vfloat4 vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
        // z.z,z.w,w.z,w.w
        vfloat4 vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

        vfloat4x4 mResult;
        // x.x,y.x,z.x,w.x
        mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
        // x.y,y.y,z.y,w.y
        mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
        // x.z,y.z,z.z,w.z
        mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
        // x.w,y.w,z.w,w.w
        mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
        return mResult;
#endif
    }

    //------------------------------------------------------------------------------
    // Return the inverse and the determinant of a 4x4 matrix
    // XMMATRIX XMMatrixInverse(XMVECTOR* pDeterminant, FXMMATRIX M)
    inline vfloat4x4 VMATH_CALLCONV vMat4Inverse(vfloat4* pDeterminant, vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        vfloat4x4 MT = vMat4Transpose(M);

        vfloat4 V0[4], V1[4];
        V0[0] = v4Swizzle<0, 0, 1, 1>(MT.r[2]);
        V1[0] = v4Swizzle<2, 3, 2, 3>(MT.r[3]);
        V0[1] = v4Swizzle<0, 0, 1, 1>(MT.r[0]);
        V1[1] = v4Swizzle<2, 3, 2, 3>(MT.r[1]);
        V0[2] = v4Shuffle<0, 2, 4, 6>(MT.r[2], MT.r[0]);
        V1[2] = v4Shuffle<1, 3, 5, 7>(MT.r[3], MT.r[1]);

        vfloat4 D0 = v4Mul(V0[0], V1[0]);
        vfloat4 D1 = v4Mul(V0[1], V1[1]);
        vfloat4 D2 = v4Mul(V0[2], V1[2]);

        V0[0] = v4Swizzle<2, 3, 2, 3>(MT.r[2]);
        V1[0] = v4Swizzle<0, 0, 1, 1>(MT.r[3]);
        V0[1] = v4Swizzle<2, 3, 2, 3>(MT.r[0]);
        V1[1] = v4Swizzle<0, 0, 1, 1>(MT.r[1]);
        V0[2] = v4Shuffle<1, 3, 5, 7>(MT.r[2], MT.r[0]);
        V1[2] = v4Shuffle<0, 2, 4, 6>(MT.r[3], MT.r[1]);

        D0 = v4NegateMulAdd(V0[0], V1[0], D0);
        D1 = v4NegateMulAdd(V0[1], V1[1], D1);
        D2 = v4NegateMulAdd(V0[2], V1[2], D2);

        V0[0] = v4Swizzle<1, 2, 0, 1>(MT.r[1]);
        V1[0] = v4Shuffle<5, 1, 3, 0>(D0, D2);
        V0[1] = v4Swizzle<2, 0, 1, 0>(MT.r[0]);
        V1[1] = v4Shuffle<3, 5, 1, 2>(D0, D2);
        V0[2] = v4Swizzle<1, 2, 0, 1>(MT.r[3]);
        V1[2] = v4Shuffle<7, 1, 3, 0>(D1, D2);
        V0[3] = v4Swizzle<2, 0, 1, 0>(MT.r[2]);
        V1[3] = v4Shuffle<3, 7, 1, 2>(D1, D2);

        vfloat4 C0 = v4Mul(V0[0], V1[0]);
        vfloat4 C2 = v4Mul(V0[1], V1[1]);
        vfloat4 C4 = v4Mul(V0[2], V1[2]);
        vfloat4 C6 = v4Mul(V0[3], V1[3]);

        V0[0] = v4Swizzle<2, 3, 1, 2>(MT.r[1]);
        V1[0] = v4Shuffle<3, 0, 1, 4>(D0, D2);
        V0[1] = v4Swizzle<3, 2, 3, 1>(MT.r[0]);
        V1[1] = v4Shuffle<2, 1, 4, 0>(D0, D2);
        V0[2] = v4Swizzle<2, 3, 1, 2>(MT.r[3]);
        V1[2] = v4Shuffle<3, 0, 1, 6>(D1, D2);
        V0[3] = v4Swizzle<3, 2, 3, 1>(MT.r[2]);
        V1[3] = v4Shuffle<2, 1, 6, 0>(D1, D2);

        C0 = v4NegateMulAdd(V0[0], V1[0], C0);
        C2 = v4NegateMulAdd(V0[1], V1[1], C2);
        C4 = v4NegateMulAdd(V0[2], V1[2], C4);
        C6 = v4NegateMulAdd(V0[3], V1[3], C6);

        V0[0] = v4Swizzle<3, 0, 3, 0>(MT.r[1]);
        V1[0] = v4Shuffle<2, 5, 4, 2>(D0, D2);
        V0[1] = v4Swizzle<1, 3, 0, 2>(MT.r[0]);
        V1[1] = v4Shuffle<5, 0, 3, 4>(D0, D2);
        V0[2] = v4Swizzle<3, 0, 3, 0>(MT.r[3]);
        V1[2] = v4Shuffle<2, 7, 6, 2>(D1, D2);
        V0[3] = v4Swizzle<1, 3, 0, 2>(MT.r[2]);
        V1[3] = v4Shuffle<7, 0, 3, 6>(D1, D2);

        vfloat4 C1 = v4NegateMulAdd(V0[0], V1[0], C0);
        C0 = v4MulAdd(V0[0], V1[0], C0);
        vfloat4 C3 = v4MulAdd(V0[1], V1[1], C2);
        C2 = v4NegateMulAdd(V0[1], V1[1], C2);
        vfloat4 C5 = v4NegateMulAdd(V0[2], V1[2], C4);
        C4 = v4MulAdd(V0[2], V1[2], C4);
        vfloat4 C7 = v4MulAdd(V0[3], V1[3], C6);
        C6 = v4NegateMulAdd(V0[3], V1[3], C6);

        vfloat4x4 R;
        R.r[0] = v4Select(C0, C1, Internal::kSelect0101.v);
        R.r[1] = v4Select(C2, C3, Internal::kSelect0101.v);
        R.r[2] = v4Select(C4, C5, Internal::kSelect0101.v);
        R.r[3] = v4Select(C6, C7, Internal::kSelect0101.v);

        vfloat4 Determinant = v4Dot(R.r[0], MT.r[0]);

        if (pDeterminant != nullptr)
            *pDeterminant = Determinant;

        vfloat4 Reciprocal = v4Rcp(Determinant);

        vfloat4x4 Result;
        Result.r[0] = v4Mul(R.r[0], Reciprocal);
        Result.r[1] = v4Mul(R.r[1], Reciprocal);
        Result.r[2] = v4Mul(R.r[2], Reciprocal);
        Result.r[3] = v4Mul(R.r[3], Reciprocal);
        return Result;

#elif defined(VMATH_SSE_INTRINSICS)
        // Transpose matrix
        vfloat4 vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
        vfloat4 vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
        vfloat4 vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
        vfloat4 vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));

        vfloat4x4 MT;
        MT.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
        MT.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
        MT.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
        MT.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

        vfloat4 V00 = VMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
        vfloat4 V10 = VMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
        vfloat4 V01 = VMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
        vfloat4 V11 = VMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
        vfloat4 V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
        vfloat4 V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

        vfloat4 D0 = _mm_mul_ps(V00, V10);
        vfloat4 D1 = _mm_mul_ps(V01, V11);
        vfloat4 D2 = _mm_mul_ps(V02, V12);

        V00 = VMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
        V10 = VMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
        V01 = VMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
        V11 = VMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
        V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
        V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

        D0 = VMATH_FNMADD_PS(V00, V10, D0);
        D1 = VMATH_FNMADD_PS(V01, V11, D1);
        D2 = VMATH_FNMADD_PS(V02, V12, D2);
        // V11 = D0Y,D0W,D2Y,D2Y
        V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
        V00 = VMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
        V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
        V01 = VMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
        V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
        // V13 = D1Y,D1W,D2W,D2W
        vfloat4 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
        V02 = VMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
        V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
        vfloat4 V03 = VMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
        V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

        vfloat4 C0 = _mm_mul_ps(V00, V10);
        vfloat4 C2 = _mm_mul_ps(V01, V11);
        vfloat4 C4 = _mm_mul_ps(V02, V12);
        vfloat4 C6 = _mm_mul_ps(V03, V13);

        // V11 = D0X,D0Y,D2X,D2X
        V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
        V00 = VMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
        V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
        V01 = VMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
        V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
        // V13 = D1X,D1Y,D2Z,D2Z
        V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
        V02 = VMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
        V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
        V03 = VMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
        V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

        C0 = VMATH_FNMADD_PS(V00, V10, C0);
        C2 = VMATH_FNMADD_PS(V01, V11, C2);
        C4 = VMATH_FNMADD_PS(V02, V12, C4);
        C6 = VMATH_FNMADD_PS(V03, V13, C6);

        V00 = VMATH_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
        // V10 = D0Z,D0Z,D2X,D2Y
        V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
        V10 = VMATH_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
        V01 = VMATH_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
        // V11 = D0X,D0W,D2X,D2Y
        V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
        V11 = VMATH_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
        V02 = VMATH_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
        // V12 = D1Z,D1Z,D2Z,D2W
        V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
        V12 = VMATH_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
        V03 = VMATH_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
        // V13 = D1X,D1W,D2Z,D2W
        V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
        V13 = VMATH_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

        V00 = _mm_mul_ps(V00, V10);
        V01 = _mm_mul_ps(V01, V11);
        V02 = _mm_mul_ps(V02, V12);
        V03 = _mm_mul_ps(V03, V13);
        vfloat4 C1 = _mm_sub_ps(C0, V00);
        C0 = _mm_add_ps(C0, V00);
        vfloat4 C3 = _mm_add_ps(C2, V01);
        C2 = _mm_sub_ps(C2, V01);
        vfloat4 C5 = _mm_sub_ps(C4, V02);
        C4 = _mm_add_ps(C4, V02);
        vfloat4 C7 = _mm_add_ps(C6, V03);
        C6 = _mm_sub_ps(C6, V03);

        C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
        C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
        C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
        C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
        C0 = VMATH_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
        C2 = VMATH_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
        C4 = VMATH_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
        C6 = VMATH_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
        // Get the determinant
        vfloat4 vTemp = v4Dot(C0, MT.r[0]);
        if (pDeterminant != nullptr)
            *pDeterminant = vTemp;
        vTemp = _mm_div_ps(Internal::kOne, vTemp);
        vfloat4x4 mResult;
        mResult.r[0] = _mm_mul_ps(C0, vTemp);
        mResult.r[1] = _mm_mul_ps(C2, vTemp);
        mResult.r[2] = _mm_mul_ps(C4, vTemp);
        mResult.r[3] = _mm_mul_ps(C6, vTemp);
        return mResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixVectorTensorProduct(FXMVECTOR V1, FXMVECTOR V2)
    inline vfloat4x4 VMATH_CALLCONV vMat4VectorTensorProduct(vfloat4 V1, vfloat4 V2)
    {
        vfloat4x4 mResult;
        mResult.r[0] = v4Mul(v4Swizzle<0, 0, 0, 0>(V1), V2);
        mResult.r[1] = v4Mul(v4Swizzle<1, 1, 1, 1>(V1), V2);
        mResult.r[2] = v4Mul(v4Swizzle<2, 2, 2, 2>(V1), V2);
        mResult.r[3] = v4Mul(v4Swizzle<3, 3, 3, 3>(V1), V2);
        return mResult;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMMatrixDeterminant(FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV vMat4Determinant(vfloat4x4 M)
    {
        static const Internal::vfloat4f32 Sign = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };

        vfloat4 V0 = v4Swizzle<1, 0, 0, 0>(M.r[2]);
        vfloat4 V1 = v4Swizzle<2, 2, 1, 1>(M.r[3]);
        vfloat4 V2 = v4Swizzle<1, 0, 0, 0>(M.r[2]);
        vfloat4 V3 = v4Swizzle<3, 3, 3, 2>(M.r[3]);
        vfloat4 V4 = v4Swizzle<2, 2, 1, 1>(M.r[2]);
        vfloat4 V5 = v4Swizzle<3, 3, 3, 2>(M.r[3]);

        vfloat4 P0 = v4Mul(V0, V1);
        vfloat4 P1 = v4Mul(V2, V3);
        vfloat4 P2 = v4Mul(V4, V5);

        V0 = v4Swizzle<2, 2, 1, 1>(M.r[2]);
        V1 = v4Swizzle<1, 0, 0, 0>(M.r[3]);
        V2 = v4Swizzle<3, 3, 3, 2>(M.r[2]);
        V3 = v4Swizzle<1, 0, 0, 0>(M.r[3]);
        V4 = v4Swizzle<3, 3, 3, 2>(M.r[2]);
        V5 = v4Swizzle<2, 2, 1, 1>(M.r[3]);

        P0 = v4NegateMulAdd(V0, V1, P0);
        P1 = v4NegateMulAdd(V2, V3, P1);
        P2 = v4NegateMulAdd(V4, V5, P2);

        V0 = v4Swizzle<3, 3, 3, 2>(M.r[1]);
        V1 = v4Swizzle<2, 2, 1, 1>(M.r[1]);
        V2 = v4Swizzle<1, 0, 0, 0>(M.r[1]);

        vfloat4 S = v4Mul(M.r[0], Sign.v);
        vfloat4 R = v4Mul(V0, P0);
        R = v4NegateMulAdd(V1, P1, R);
        R = v4MulAdd(V2, P2, R);

        return v4Dot(S, R);
    }
    // bool XMMatrixDecompose(XMVECTOR* outScale, XMVECTOR* outRotQuat, XMVECTOR* outTrans, FXMMATRIX M)
    inline bool VMATH_CALLCONV vMat4Decompose(vfloat4* outScale, vfloat4* outRotQuat, vfloat4* outTrans, vfloat4x4 M)
    {
        constexpr float Epsilon = 0.0001f;

        static const vfloat4* pvCanonicalBasis[3] = {
            &Internal::kIdentityR0.v,
            &Internal::kIdentityR1.v,
            &Internal::kIdentityR2.v
        };

        assert(outScale != nullptr);
        assert(outRotQuat != nullptr);
        assert(outTrans != nullptr);

        // Get the translation
        outTrans[0] = M.r[3];

        vfloat4* ppvBasis[3];
        vfloat4x4 matTemp;
        ppvBasis[0] = &matTemp.r[0];
        ppvBasis[1] = &matTemp.r[1];
        ppvBasis[2] = &matTemp.r[2];

        matTemp.r[0] = M.r[0];
        matTemp.r[1] = M.r[1];
        matTemp.r[2] = M.r[2];
        matTemp.r[3] = Internal::kIdentityR3.v;

        auto pfScales = reinterpret_cast<float*>(outScale);

        pfScales[0] = v4GetX(v3Length(ppvBasis[0][0]));
        pfScales[1] = v4GetX(v3Length(ppvBasis[1][0]));
        pfScales[2] = v4GetX(v3Length(ppvBasis[2][0]));
        pfScales[3] = 0.f;

        size_t a, b, c;
        if (pfScales[0] < pfScales[1])
        {
            if (pfScales[1] < pfScales[2])
            {
                a = 2;
                b = 1;
                c = 0;
            }
            else
            {
                a = 1;

                if (pfScales[0] < pfScales[2])
                {
                    b = 2;
                    c = 0;
                }
                else
                {
                    b = 0;
                    c = 2;
                }
            }
        }
        else
        {
            if (pfScales[0] < pfScales[2])
            {
                a = 2;
                b = 0;
                c = 1;
            }
            else
            {
                a = 0;

                if (pfScales[1] < pfScales[2])
                {
                    b = 2;
                    c = 1;
                }
                else
                {
                    b = 1;
                    c = 2;
                }
            }
        }

        if (pfScales[a] < Epsilon)
        {
            ppvBasis[a][0] = pvCanonicalBasis[a][0];
        }
        ppvBasis[a][0] = v3Normalize(ppvBasis[a][0]);

        if (pfScales[b] < Epsilon)
        {
            float fAbsX = fabsf(v4GetX(ppvBasis[a][0]));
            float fAbsY = fabsf(v4GetY(ppvBasis[a][0]));
            float fAbsZ = fabsf(v4GetZ(ppvBasis[a][0]));

            size_t cc;
            if (fAbsX < fAbsY)
            {
                if (fAbsY < fAbsZ)
                {
                    cc = 0;
                }
                else
                {
                    if (fAbsX < fAbsZ) cc = 0;
                    else cc = 2;
                }
            }
            else
            {
                if (fAbsX < fAbsZ)
                {
                    cc = 1;
                }
                else
                {
                    if (fAbsY < fAbsZ) cc = 1;
                    else cc = 2;
                }
            }

            ppvBasis[b][0] = v3Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
        }

        ppvBasis[b][0] = v3Normalize(ppvBasis[b][0]);

        if (pfScales[c] < Epsilon)
        {
            ppvBasis[c][0] = v3Cross(ppvBasis[a][0], ppvBasis[b][0]);
        }

        ppvBasis[c][0] = v3Normalize(ppvBasis[c][0]);

        float fDet = v4GetX(vMat4Determinant(matTemp));

        // use Kramer's rule to check for handedness of coordinate system
        if (fDet < 0.0f)
        {
            // switch coordinate system by negating the scale and inverting the basis vector on the x-axis
            pfScales[a] = -pfScales[a];
            ppvBasis[a][0] = v4Negate(ppvBasis[a][0]);

            fDet = -fDet;
        }

        fDet -= 1.0f;
        fDet *= fDet;

        if (Epsilon < fDet)
        {
            // Non-SRT matrix encountered
            return false;
        }

        // generate the quaternion from the matrix
        outRotQuat[0] = vQuatRotationMatrix(matTemp);
        return true;
    }

    //------------------------------------------------------------------------------
    // Transformation operations
    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixSet(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
    inline vfloat4x4::vfloat4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
    {
#if defined(VMATH_NO_INTRINSICS)
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
        m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
#else
        r[0] = v4Set(m00, m01, m02, m03);
        r[1] = v4Set(m10, m11, m12, m13);
        r[2] = v4Set(m20, m21, m22, m23);
        r[3] = v4Set(m30, m31, m32, m33);
#endif
    }

    //------------------------------------------------------------------------------

    inline vfloat4x4::vfloat4x4(vfloat4 r0, vfloat4 r1, vfloat4 r2, vfloat4 r3)
    {
        r[0] = r0;
        r[1] = r1;
        r[2] = r2;
        r[3] = r3;
    }

    //------------------------------------------------------------------------------

    inline vfloat4x4 VMATH_CALLCONV vMat4Zero()
    {
        vfloat4x4 M;
        M.r[0] = v4Zero();
        M.r[1] = v4Zero();
        M.r[2] = v4Zero();
        M.r[3] = v4Zero();
        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixIdentity()
    inline vfloat4x4 VMATH_CALLCONV vMat4Identity()
    {
        vfloat4x4 M;
        M.r[0] = v4Set(1, 0, 0, 0);
        M.r[1] = v4Set(0, 1, 0, 0);
        M.r[2] = v4Set(0, 0, 1, 0);
        M.r[3] = v4Set(0, 0, 0, 1);
        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixTranslation(float OffsetX, float OffsetY, float OffsetZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4Translation(float OffsetX, float OffsetY, float OffsetZ)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4x4 M;
        M.m[0][0] = 1.0f;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = 1.0f;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = 1.0f;
        M.m[2][3] = 0.0f;

        M.m[3][0] = OffsetX;
        M.m[3][1] = OffsetY;
        M.m[3][2] = OffsetZ;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_SSE_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = Internal::kIdentityR0.v;
        M.r[1] = Internal::kIdentityR1.v;
        M.r[2] = Internal::kIdentityR2.v;
        M.r[3] = v4Set(OffsetX, OffsetY, OffsetZ, 1.f);
        return M;
#endif
    }


    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixTranslationFromVector(FXMVECTOR Offset)
    inline vfloat4x4 VMATH_CALLCONV vMat4Translation(vfloat4 Offset)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4x4 M;
        M.m[0][0] = 1.0f;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = 1.0f;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = 1.0f;
        M.m[2][3] = 0.0f;

        M.m[3][0] = Offset.v4f[0];
        M.m[3][1] = Offset.v4f[1];
        M.m[3][2] = Offset.v4f[2];
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_SSE_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = Internal::kIdentityR0.v;
        M.r[1] = Internal::kIdentityR1.v;
        M.r[2] = Internal::kIdentityR2.v;
        M.r[3] = v4Select(Internal::kIdentityR3.v, Offset, Internal::kSelect1110.v);
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixScaling(float ScaleX, float ScaleY, float ScaleZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4Scaling(float ScaleX, float ScaleY, float ScaleZ)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4x4 M;
        M.m[0][0] = ScaleX;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = ScaleY;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = ScaleZ;
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = 0.0f;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        const vfloat4 Zero = vdupq_n_f32(0);
        vfloat4x4 M;
        M.r[0] = vsetq_lane_f32(ScaleX, Zero, 0);
        M.r[1] = vsetq_lane_f32(ScaleY, Zero, 1);
        M.r[2] = vsetq_lane_f32(ScaleZ, Zero, 2);
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = _mm_set_ps(0, 0, 0, ScaleX);
        M.r[1] = _mm_set_ps(0, 0, ScaleY, 0);
        M.r[2] = _mm_set_ps(0, ScaleZ, 0, 0);
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixScalingFromVector(FXMVECTOR Scale)
    inline vfloat4x4 VMATH_CALLCONV vMat4Scaling(vfloat4 Scale)
    {
#if defined(VMATH_NO_INTRINSICS)

        vfloat4x4 M;
        M.m[0][0] = Scale.v4f[0];
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = Scale.v4f[1];
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = Scale.v4f[2];
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = 0.0f;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(Scale), Internal::kMaskX));
        M.r[1] = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(Scale), Internal::kMaskY));
        M.r[2] = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(Scale), Internal::kMaskZ));
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        M.r[0] = _mm_and_ps(Scale, Internal::kMaskX);
        M.r[1] = _mm_and_ps(Scale, Internal::kMaskY);
        M.r[2] = _mm_and_ps(Scale, Internal::kMaskZ);
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationX(float Angle)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationX(float Angle)
    {
#if defined(VMATH_NO_INTRINSICS)

        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        vfloat4x4 M;
        M.m[0][0] = 1.0f;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = fCosAngle;
        M.m[1][2] = fSinAngle;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = -fSinAngle;
        M.m[2][2] = fCosAngle;
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = 0.0f;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        const float32x4_t Zero = vdupq_n_f32(0);

        float32x4_t T1 = vsetq_lane_f32(fCosAngle, Zero, 1);
        T1 = vsetq_lane_f32(fSinAngle, T1, 2);

        float32x4_t T2 = vsetq_lane_f32(-fSinAngle, Zero, 1);
        T2 = vsetq_lane_f32(fCosAngle, T2, 2);

        vfloat4x4 M;
        M.r[0] = Internal::kIdentityR0.v;
        M.r[1] = T1;
        M.r[2] = T2;
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        float    SinAngle;
        float    CosAngle;
        SinCos(&SinAngle, &CosAngle, Angle);

        vfloat4 vSin = _mm_set_ss(SinAngle);
        vfloat4 vCos = _mm_set_ss(CosAngle);
        // x = 0,y = cos,z = sin, w = 0
        vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
        vfloat4x4 M;
        M.r[0] = Internal::kIdentityR0;
        M.r[1] = vCos;
        // x = 0,y = sin,z = cos, w = 0
        vCos = VMATH_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
        // x = 0,y = -sin,z = cos, w = 0
        vCos = _mm_mul_ps(vCos, Internal::kNegateY);
        M.r[2] = vCos;
        M.r[3] = Internal::kIdentityR3;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationY(float Angle)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationY(float Angle)
    {
#if defined(VMATH_NO_INTRINSICS)

        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        vfloat4x4 M;
        M.m[0][0] = fCosAngle;
        M.m[0][1] = 0.0f;
        M.m[0][2] = -fSinAngle;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = 1.0f;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = fSinAngle;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fCosAngle;
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = 0.0f;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        const float32x4_t Zero = vdupq_n_f32(0);

        float32x4_t T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
        T0 = vsetq_lane_f32(-fSinAngle, T0, 2);

        float32x4_t T2 = vsetq_lane_f32(fSinAngle, Zero, 0);
        T2 = vsetq_lane_f32(fCosAngle, T2, 2);

        vfloat4x4 M;
        M.r[0] = T0;
        M.r[1] = Internal::kIdentityR1.v;
        M.r[2] = T2;
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        float    SinAngle;
        float    CosAngle;
        SinCos(&SinAngle, &CosAngle, Angle);

        vfloat4 vSin = _mm_set_ss(SinAngle);
        vfloat4 vCos = _mm_set_ss(CosAngle);
        // x = sin,y = 0,z = cos, w = 0
        vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
        vfloat4x4 M;
        M.r[2] = vSin;
        M.r[1] = Internal::kIdentityR1;
        // x = cos,y = 0,z = sin, w = 0
        vSin = VMATH_PERMUTE_PS(vSin, _MM_SHUFFLE(3, 0, 1, 2));
        // x = cos,y = 0,z = -sin, w = 0
        vSin = _mm_mul_ps(vSin, Internal::kNegateZ);
        M.r[0] = vSin;
        M.r[3] = Internal::kIdentityR3;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationZ(float Angle)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationZ(float Angle)
    {
#if defined(VMATH_NO_INTRINSICS)

        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        vfloat4x4 M;
        M.m[0][0] = fCosAngle;
        M.m[0][1] = fSinAngle;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = -fSinAngle;
        M.m[1][1] = fCosAngle;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = 1.0f;
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = 0.0f;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        const float32x4_t Zero = vdupq_n_f32(0);

        float32x4_t T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
        T0 = vsetq_lane_f32(fSinAngle, T0, 1);

        float32x4_t T1 = vsetq_lane_f32(-fSinAngle, Zero, 0);
        T1 = vsetq_lane_f32(fCosAngle, T1, 1);

        vfloat4x4 M;
        M.r[0] = T0;
        M.r[1] = T1;
        M.r[2] = Internal::kIdentityR2.v;
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        float    SinAngle;
        float    CosAngle;
        SinCos(&SinAngle, &CosAngle, Angle);

        vfloat4 vSin = _mm_set_ss(SinAngle);
        vfloat4 vCos = _mm_set_ss(CosAngle);
        // x = cos,y = sin,z = 0, w = 0
        vCos = _mm_unpacklo_ps(vCos, vSin);
        vfloat4x4 M;
        M.r[0] = vCos;
        // x = sin,y = cos,z = 0, w = 0
        vCos = VMATH_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 2, 0, 1));
        // x = cos,y = -sin,z = 0, w = 0
        vCos = _mm_mul_ps(vCos, Internal::kNegateX);
        M.r[1] = vCos;
        M.r[2] = Internal::kIdentityR2;
        M.r[3] = Internal::kIdentityR3;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    {
        vfloat4 Angles = v4Set(Pitch, Yaw, Roll, 0.0f);
        return vMat4RotationRollPitchYawFromVector(Angles);
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationRollPitchYawFromVector(FXMVECTOR Angles)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationRollPitchYawFromVector(vfloat4 Angles)
    {
        vfloat4 Q = vQuatRotationRollPitchYawFromVector(Angles);
        return vMat4RotationQuaternion(Q);
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationNormal(FXMVECTOR NormalAxis, float Angle)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationNormal(vfloat4 NormalAxis, float Angle)
    {
#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        vfloat4 A = v4Set(fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f);

        vfloat4 C2 = v4zzzz(A);
        vfloat4 C1 = v4yyyy(A);
        vfloat4 C0 = v4xxxx(A);

        vfloat4 N0 = v4Swizzle<1, 2, 0, 3>(NormalAxis);
        vfloat4 N1 = v4Swizzle<2, 0, 1, 3>(NormalAxis);

        vfloat4 V0 = v4Mul(C2, N0);
        V0 = v4Mul(V0, N1);

        vfloat4 R0 = v4Mul(C2, NormalAxis);
        R0 = v4MulAdd(R0, NormalAxis, C1);

        vfloat4 R1 = v4MulAdd(C0, NormalAxis, V0);
        vfloat4 R2 = v4NegateMulAdd(C0, NormalAxis, V0);

        V0 = v4Select(A, R0, Internal::kSelect1110.v);
        vfloat4 V1 = v4Shuffle<2, 5, 6, 0>(R1, R2);
        vfloat4 V2 = v4Shuffle<1, 4, 1, 4>(R1, R2);

        vfloat4x4 M;
        M.r[0] = v4Shuffle<0, 4, 5, 3>(V0, V1);
        M.r[1] = v4Shuffle<6, 1, 7, 3>(V0, V1);
        M.r[2] = v4Shuffle<4, 5, 2, 3>(V0, V2);
        M.r[3] = Internal::kIdentityR3.v;
        return M;

#elif defined(VMATH_SSE_INTRINSICS)
        float    fSinAngle;
        float    fCosAngle;
        SinCos(&fSinAngle, &fCosAngle, Angle);

        vfloat4 C2 = _mm_set_ps1(1.0f - fCosAngle);
        vfloat4 C1 = _mm_set_ps1(fCosAngle);
        vfloat4 C0 = _mm_set_ps1(fSinAngle);

        vfloat4 N0 = VMATH_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
        vfloat4 N1 = VMATH_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));

        vfloat4 V0 = _mm_mul_ps(C2, N0);
        V0 = _mm_mul_ps(V0, N1);

        vfloat4 R0 = _mm_mul_ps(C2, NormalAxis);
        R0 = _mm_mul_ps(R0, NormalAxis);
        R0 = _mm_add_ps(R0, C1);

        vfloat4 R1 = _mm_mul_ps(C0, NormalAxis);
        R1 = _mm_add_ps(R1, V0);
        vfloat4 R2 = _mm_mul_ps(C0, NormalAxis);
        R2 = _mm_sub_ps(V0, R2);

        V0 = _mm_and_ps(R0, Internal::kMask3);
        vfloat4 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
        V1 = VMATH_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
        vfloat4 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
        V2 = VMATH_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

        R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
        R2 = VMATH_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

        vfloat4x4 M;
        M.r[0] = R2;

        R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
        R2 = VMATH_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
        M.r[1] = R2;

        V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
        M.r[2] = V2;
        M.r[3] = Internal::kIdentityR3.v;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationAxis(FXMVECTOR Axis, float Angle)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationAxis(vfloat4 Axis, float Angle)
    {
        assert(!v3AllEqual(Axis, v4Zero()));
        assert(!v3AnyIsInfinite(Axis));

        vfloat4 Normal = v3Normalize(Axis);
        return vMat4RotationNormal(Normal, Angle);
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixRotationQuaternion(FXMVECTOR Quaternion)
    inline vfloat4x4 VMATH_CALLCONV vMat4RotationQuaternion(vfloat4 Quaternion)
    {
#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        static const Internal::vfloat4f32 Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

        vfloat4 Q0 = v4Add(Quaternion, Quaternion);
        vfloat4 Q1 = v4Mul(Quaternion, Q0);

        vfloat4 V0 = v4Shuffle<1, 0, 0, 7>(Q1, Constant1110.v);
        vfloat4 V1 = v4Shuffle<2, 2, 1, 7>(Q1, Constant1110.v);
        vfloat4 R0 = v4Sub(Constant1110, V0);
        R0 = v4Sub(R0, V1);

        V0 = v4Swizzle<0, 0, 1, 3>(Quaternion);
        V1 = v4Swizzle<2, 1, 2, 3>(Q0);
        V0 = v4Mul(V0, V1);

        V1 = v4wwww(Quaternion);
        vfloat4 V2 = v4Swizzle<1, 2, 0, 3>(Q0);
        V1 = v4Mul(V1, V2);

        vfloat4 R1 = v4Add(V0, V1);
        vfloat4 R2 = v4Sub(V0, V1);

        V0 = v4Shuffle<1, 4, 5, 2>(R1, R2);
        V1 = v4Shuffle<0, 6, 0, 6>(R1, R2);

        vfloat4x4 M;
        M.r[0] = v4Shuffle<0, 4, 5, 3>(R0, V0);
        M.r[1] = v4Shuffle<6, 1, 7, 3>(R0, V0);
        M.r[2] = v4Shuffle<4, 5, 2, 3>(R0, V1);
        M.r[3] = Internal::kIdentityR3.v;
        return M;

#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32  Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

        vfloat4 Q0 = _mm_add_ps(Quaternion, Quaternion);
        vfloat4 Q1 = _mm_mul_ps(Quaternion, Q0);

        vfloat4 V0 = VMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
        V0 = _mm_and_ps(V0, Internal::kMask3);
        vfloat4 V1 = VMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
        V1 = _mm_and_ps(V1, Internal::kMask3);
        vfloat4 R0 = _mm_sub_ps(Constant1110, V0);
        R0 = _mm_sub_ps(R0, V1);

        V0 = VMATH_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 1, 0, 0));
        V1 = VMATH_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
        V0 = _mm_mul_ps(V0, V1);

        V1 = VMATH_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 3, 3, 3));
        vfloat4 V2 = VMATH_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
        V1 = _mm_mul_ps(V1, V2);

        vfloat4 R1 = _mm_add_ps(V0, V1);
        vfloat4 R2 = _mm_sub_ps(V0, V1);

        V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
        V0 = VMATH_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
        V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
        V1 = VMATH_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

        Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
        Q1 = VMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

        vfloat4x4 M;
        M.r[0] = Q1;

        Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
        Q1 = VMATH_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
        M.r[1] = Q1;

        Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
        M.r[2] = Q1;
        M.r[3] = Internal::kIdentityR3;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixTransformation2D(FXMVECTOR ScalingOrigin, float ScalingOrientation, FXMVECTOR Scaling, FXMVECTOR RotationOrigin, float Rotation, GXMVECTOR Translation)
    inline vfloat4x4 VMATH_CALLCONV vMat4Transformation2D(vfloat4 ScalingOrigin, float ScalingOrientation, vfloat4 Scaling, vfloat4 RotationOrigin, float Rotation, vfloat4 Translation)
    {
        // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
        //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

        vfloat4 VScalingOrigin = v4Select(Internal::kSelect1100.v, ScalingOrigin, Internal::kSelect1100.v);
        vfloat4 NegScalingOrigin = v4Negate(VScalingOrigin);

        vfloat4x4 MScalingOriginI = vMat4Translation(NegScalingOrigin);
        vfloat4x4 MScalingOrientation = vMat4RotationZ(ScalingOrientation);
        vfloat4x4 MScalingOrientationT = vMat4Transpose(MScalingOrientation);
        vfloat4 VScaling = v4Select(Internal::kOne.v, Scaling, Internal::kSelect1100.v);
        vfloat4x4 MScaling = vMat4Scaling(VScaling);
        vfloat4 VRotationOrigin = v4Select(Internal::kSelect1100.v, RotationOrigin, Internal::kSelect1100.v);
        vfloat4x4 MRotation = vMat4RotationZ(Rotation);
        vfloat4 VTranslation = v4Select(Internal::kSelect1100.v, Translation, Internal::kSelect1100.v);

        vfloat4x4 M = vMat4Mul(MScalingOriginI, MScalingOrientationT);
        M = vMat4Mul(M, MScaling);
        M = vMat4Mul(M, MScalingOrientation);
        M.r[3] = v4Add(M.r[3], VScalingOrigin);
        M.r[3] = v4Sub(M.r[3], VRotationOrigin);
        M = vMat4Mul(M, MRotation);
        M.r[3] = v4Add(M.r[3], VRotationOrigin);
        M.r[3] = v4Add(M.r[3], VTranslation);

        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixTransformation(FXMVECTOR ScalingOrigin, FXMVECTOR ScalingOrientationQuaternion, FXMVECTOR Scaling, GXMVECTOR RotationOrigin, HXMVECTOR RotationQuaternion, HXMVECTOR Translation)
    inline vfloat4x4 VMATH_CALLCONV vMat4Transformation(vfloat4 ScalingOrigin, vfloat4 ScalingOrientationQuaternion, vfloat4 Scaling, vfloat4 RotationOrigin, vfloat4 RotationQuaternion, vfloat4 Translation)
    {
        // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
        //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

        vfloat4 VScalingOrigin = v4Select(Internal::kSelect1110.v, ScalingOrigin, Internal::kSelect1110.v);
        vfloat4 NegScalingOrigin = v4Negate(ScalingOrigin);

        vfloat4x4 MScalingOriginI = vMat4Translation(NegScalingOrigin);
        vfloat4x4 MScalingOrientation = vMat4RotationQuaternion(ScalingOrientationQuaternion);
        vfloat4x4 MScalingOrientationT = vMat4Transpose(MScalingOrientation);
        vfloat4x4 MScaling = vMat4Scaling(Scaling);
        vfloat4 VRotationOrigin = v4Select(Internal::kSelect1110.v, RotationOrigin, Internal::kSelect1110.v);
        vfloat4x4 MRotation = vMat4RotationQuaternion(RotationQuaternion);
        vfloat4 VTranslation = v4Select(Internal::kSelect1110.v, Translation, Internal::kSelect1110.v);

        vfloat4x4 M;
        M = vMat4Mul(MScalingOriginI, MScalingOrientationT);
        M = vMat4Mul(M, MScaling);
        M = vMat4Mul(M, MScalingOrientation);
        M.r[3] = v4Add(M.r[3], VScalingOrigin);
        M.r[3] = v4Sub(M.r[3], VRotationOrigin);
        M = vMat4Mul(M, MRotation);
        M.r[3] = v4Add(M.r[3], VRotationOrigin);
        M.r[3] = v4Add(M.r[3], VTranslation);
        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixAffineTransformation2D(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, float Rotation, FXMVECTOR Translation)
    inline vfloat4x4 VMATH_CALLCONV vMat4AffineTransformation2D(vfloat4 Scaling, vfloat4 RotationOrigin, float Rotation, vfloat4 Translation)
    {
        // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

        vfloat4 VScaling = v4Select(Internal::kOne.v, Scaling, Internal::kSelect1100.v);
        vfloat4x4 MScaling = vMat4Scaling(VScaling);
        vfloat4 VRotationOrigin = v4Select(Internal::kSelect1100.v, RotationOrigin, Internal::kSelect1100.v);
        vfloat4x4 MRotation = vMat4RotationZ(Rotation);
        vfloat4 VTranslation = v4Select(Internal::kSelect1100.v, Translation, Internal::kSelect1100.v);

        vfloat4x4 M;
        M = MScaling;
        M.r[3] = v4Sub(M.r[3], VRotationOrigin);
        M = vMat4Mul(M, MRotation);
        M.r[3] = v4Add(M.r[3], VRotationOrigin);
        M.r[3] = v4Add(M.r[3], VTranslation);
        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixAffineTransformation(FXMVECTOR Scaling, FXMVECTOR RotationOrigin, FXMVECTOR RotationQuaternion, GXMVECTOR Translation)
    inline vfloat4x4 VMATH_CALLCONV vMat4AffineTransformation(vfloat4 Scaling, vfloat4 RotationOrigin, vfloat4 RotationQuaternion, vfloat4 Translation)
    {
        // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

        vfloat4x4 MScaling = vMat4Scaling(Scaling);
        vfloat4 VRotationOrigin = v4Select(Internal::kSelect1110.v, RotationOrigin, Internal::kSelect1110.v);
        vfloat4x4 MRotation = vMat4RotationQuaternion(RotationQuaternion);
        vfloat4 VTranslation = v4Select(Internal::kSelect1110.v, Translation, Internal::kSelect1110.v);

        vfloat4x4 M;
        M = MScaling;
        M.r[3] = v4Sub(M.r[3], VRotationOrigin);
        M = vMat4Mul(M, MRotation);
        M.r[3] = v4Add(M.r[3], VRotationOrigin);
        M.r[3] = v4Add(M.r[3], VTranslation);
        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixReflect(FXMVECTOR ReflectionPlane)
    inline vfloat4x4 VMATH_CALLCONV vMat4Reflect(vfloat4 ReflectionPlane)
    {
        assert(!v3AllEqual(ReflectionPlane, v4Zero()));
        assert(!v4AnyIsInfinite(ReflectionPlane));

        static const Internal::vfloat4f32 NegativeTwo = { { { -2.0f, -2.0f, -2.0f, 0.0f } } };

        vfloat4 P = vPlaneNormalize(ReflectionPlane);
        vfloat4 S = v4Mul(P, NegativeTwo);

        vfloat4 A = v4xxxx(P);
        vfloat4 B = v4yyyy(P);
        vfloat4 C = v4zzzz(P);
        vfloat4 D = v4wwww(P);

        vfloat4x4 M;
        M.r[0] = v4MulAdd(A, S, Internal::kIdentityR0.v);
        M.r[1] = v4MulAdd(B, S, Internal::kIdentityR1.v);
        M.r[2] = v4MulAdd(C, S, Internal::kIdentityR2.v);
        M.r[3] = v4MulAdd(D, S, Internal::kIdentityR3.v);
        return M;
    }

    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // View and projection initialization operations
    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixLookAtLH(FXMVECTOR EyePosition, FXMVECTOR FocusPosition, FXMVECTOR UpDirection)
    inline vfloat4x4 VMATH_CALLCONV vMat4LookAt(vfloat4 EyePosition, vfloat4 FocusPosition, vfloat4 UpDirection)
    {
        vfloat4 EyeDirection = v4Sub(FocusPosition, EyePosition);
        return vMat4LookTo(EyePosition, EyeDirection, UpDirection);
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixLookToLH(FXMVECTOR EyePosition, FXMVECTOR EyeDirection, FXMVECTOR UpDirection)
    inline vfloat4x4 VMATH_CALLCONV vMat4LookTo(vfloat4 EyePosition, vfloat4 EyeDirection, vfloat4 UpDirection)
    {
        assert(!v3AllEqual(EyeDirection, v4Zero()));
        assert(!v3AnyIsInfinite(EyeDirection));
        assert(!v3AllEqual(UpDirection, v4Zero()));
        assert(!v3AnyIsInfinite(UpDirection));

        vfloat4 R2 = v3Normalize(EyeDirection);

        vfloat4 R0 = v3Cross(UpDirection, R2);
        R0 = v3Normalize(R0);

        vfloat4 R1 = v3Cross(R2, R0);

        vfloat4 NegEyePosition = v4Negate(EyePosition);

        vfloat4 D0 = v3Dot(R0, NegEyePosition);
        vfloat4 D1 = v3Dot(R1, NegEyePosition);
        vfloat4 D2 = v3Dot(R2, NegEyePosition);

        vfloat4x4 M;
        M.r[0] = v4Select(D0, R0, Internal::kSelect1110.v);
        M.r[1] = v4Select(D1, R1, Internal::kSelect1110.v);
        M.r[2] = v4Select(D2, R2, Internal::kSelect1110.v);
        M.r[3] = Internal::kIdentityR3.v;

        M = vMat4Transpose(M);

        return M;
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4Perspective(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
    {
        assert(NearZ > 0.f && FarZ > 0.f);
        assert(!NearEqual(ViewWidth, 0.0f, 0.00001f));
        assert(!NearEqual(ViewHeight, 0.0f, 0.00001f));
        assert(!NearEqual(FarZ, NearZ, 0.00001f));

#if defined(VMATH_NO_INTRINSICS)

        float TwoNearZ = NearZ + NearZ;
        float fRange = FarZ / (FarZ - NearZ);

        vfloat4x4 M;
        M.m[0][0] = TwoNearZ / ViewWidth;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = TwoNearZ / ViewHeight;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = 1.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = -fRange * NearZ;
        M.m[3][3] = 0.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float TwoNearZ = NearZ + NearZ;
        float fRange = FarZ / (FarZ - NearZ);
        const float32x4_t Zero = vdupq_n_f32(0);
        vfloat4x4 M;
        M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
        M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
        M.r[2] = vsetq_lane_f32(fRange, Internal::kIdentityR3.v, 2);
        M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        float TwoNearZ = NearZ + NearZ;
        float fRange = FarZ / (FarZ - NearZ);
        // Note: This is recorded on the stack
        vfloat4 rMem = {
            TwoNearZ / ViewWidth,
            TwoNearZ / ViewHeight,
            fRange,
            -fRange * NearZ
        };
        // Copy from memory to SSE register
        vfloat4 vValues = rMem;
        vfloat4 vTemp = _mm_setzero_ps();
        // Copy x only
        vTemp = _mm_move_ss(vTemp, vValues);
        // TwoNearZ / ViewWidth,0,0,0
        M.r[0] = vTemp;
        // 0,TwoNearZ / ViewHeight,0,0
        vTemp = vValues;
        vTemp = _mm_and_ps(vTemp, Internal::kMaskY);
        M.r[1] = vTemp;
        // x=fRange,y=-fRange * NearZ,0,1.0f
        vValues = _mm_shuffle_ps(vValues, Internal::kIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
        // 0,0,fRange,1.0f
        vTemp = _mm_setzero_ps();
        vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
        M.r[2] = vTemp;
        // 0,0,-fRange * NearZ,0
        vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
        M.r[3] = vTemp;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4PerspectiveFOV(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
    {
        assert(NearZ > 0.f && FarZ > 0.f);
        assert(!NearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
        assert(!NearEqual(AspectRatio, 0.0f, 0.00001f));
        assert(!NearEqual(FarZ, NearZ, 0.00001f));

#if defined(VMATH_NO_INTRINSICS)

        float    SinFov;
        float    CosFov;
        SinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

        float Height = CosFov / SinFov;
        float Width = Height / AspectRatio;
        float fRange = FarZ / (FarZ - NearZ);

        vfloat4x4 M;
        M.m[0][0] = Width;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = Height;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = 1.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = -fRange * NearZ;
        M.m[3][3] = 0.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float    SinFov;
        float    CosFov;
        SinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

        float fRange = FarZ / (FarZ - NearZ);
        float Height = CosFov / SinFov;
        float Width = Height / AspectRatio;
        const float32x4_t Zero = vdupq_n_f32(0);

        vfloat4x4 M;
        M.r[0] = vsetq_lane_f32(Width, Zero, 0);
        M.r[1] = vsetq_lane_f32(Height, Zero, 1);
        M.r[2] = vsetq_lane_f32(fRange, Internal::kIdentityR3.v, 2);
        M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        float    SinFov;
        float    CosFov;
        SinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

        float fRange = FarZ / (FarZ - NearZ);
        // Note: This is recorded on the stack
        float Height = CosFov / SinFov;
        vfloat4 rMem = {
            Height / AspectRatio,
            Height,
            fRange,
            -fRange * NearZ
        };
        // Copy from memory to SSE register
        vfloat4 vValues = rMem;
        vfloat4 vTemp = _mm_setzero_ps();
        // Copy x only
        vTemp = _mm_move_ss(vTemp, vValues);
        // CosFov / SinFov,0,0,0
        vfloat4x4 M;
        M.r[0] = vTemp;
        // 0,Height / AspectRatio,0,0
        vTemp = vValues;
        vTemp = _mm_and_ps(vTemp, Internal::kMaskY);
        M.r[1] = vTemp;
        // x=fRange,y=-fRange * NearZ,0,1.0f
        vTemp = _mm_setzero_ps();
        vValues = _mm_shuffle_ps(vValues, Internal::kIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
        // 0,0,fRange,1.0f
        vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
        M.r[2] = vTemp;
        // 0,0,-fRange * NearZ,0.0f
        vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
        M.r[3] = vTemp;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixPerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4PerspectiveOffCenter(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
    {
        assert(NearZ > 0.f && FarZ > 0.f);
        assert(!NearEqual(ViewRight, ViewLeft, 0.00001f));
        assert(!NearEqual(ViewTop, ViewBottom, 0.00001f));
        assert(!NearEqual(FarZ, NearZ, 0.00001f));

#if defined(VMATH_NO_INTRINSICS)

        float TwoNearZ = NearZ + NearZ;
        float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
        float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
        float fRange = FarZ / (FarZ - NearZ);

        vfloat4x4 M;
        M.m[0][0] = TwoNearZ * ReciprocalWidth;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = TwoNearZ * ReciprocalHeight;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
        M.m[2][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
        M.m[2][2] = fRange;
        M.m[2][3] = 1.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = -fRange * NearZ;
        M.m[3][3] = 0.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float TwoNearZ = NearZ + NearZ;
        float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
        float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
        float fRange = FarZ / (FarZ - NearZ);
        const float32x4_t Zero = vdupq_n_f32(0);

        vfloat4x4 M;
        M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
        M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
        M.r[2] = v4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
            -(ViewTop + ViewBottom) * ReciprocalHeight,
            fRange,
            1.0f);
        M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        float TwoNearZ = NearZ + NearZ;
        float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
        float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
        float fRange = FarZ / (FarZ - NearZ);
        // Note: This is recorded on the stack
        vfloat4 rMem = {
            TwoNearZ * ReciprocalWidth,
            TwoNearZ * ReciprocalHeight,
            -fRange * NearZ,
            0
        };
        // Copy from memory to SSE register
        vfloat4 vValues = rMem;
        vfloat4 vTemp = _mm_setzero_ps();
        // Copy x only
        vTemp = _mm_move_ss(vTemp, vValues);
        // TwoNearZ*ReciprocalWidth,0,0,0
        M.r[0] = vTemp;
        // 0,TwoNearZ*ReciprocalHeight,0,0
        vTemp = vValues;
        vTemp = _mm_and_ps(vTemp, Internal::kMaskY);
        M.r[1] = vTemp;
        // 0,0,fRange,1.0f
        M.r[2] = v4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
            -(ViewTop + ViewBottom) * ReciprocalHeight,
            fRange,
            1.0f);
        // 0,0,-fRange * NearZ,0.0f
        vValues = _mm_and_ps(vValues, Internal::kMaskZ);
        M.r[3] = vValues;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4Orthographic(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
    {
        assert(!NearEqual(ViewWidth, 0.0f, 0.00001f));
        assert(!NearEqual(ViewHeight, 0.0f, 0.00001f));
        assert(!NearEqual(FarZ, NearZ, 0.00001f));

#if defined(VMATH_NO_INTRINSICS)

        float fRange = 1.0f / (FarZ - NearZ);

        vfloat4x4 M;
        M.m[0][0] = 2.0f / ViewWidth;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = 2.0f / ViewHeight;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = -fRange * NearZ;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float fRange = 1.0f / (FarZ - NearZ);

        const float32x4_t Zero = vdupq_n_f32(0);
        vfloat4x4 M;
        M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
        M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
        M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
        M.r[3] = vsetq_lane_f32(-fRange * NearZ, Internal::kIdentityR3.v, 2);
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        float fRange = 1.0f / (FarZ - NearZ);
        // Note: This is recorded on the stack
        vfloat4 rMem = {
            2.0f / ViewWidth,
            2.0f / ViewHeight,
            fRange,
            -fRange * NearZ
        };
        // Copy from memory to SSE register
        vfloat4 vValues = rMem;
        vfloat4 vTemp = _mm_setzero_ps();
        // Copy x only
        vTemp = _mm_move_ss(vTemp, vValues);
        // 2.0f / ViewWidth,0,0,0
        M.r[0] = vTemp;
        // 0,2.0f / ViewHeight,0,0
        vTemp = vValues;
        vTemp = _mm_and_ps(vTemp, Internal::kMaskY);
        M.r[1] = vTemp;
        // x=fRange,y=-fRange * NearZ,0,1.0f
        vTemp = _mm_setzero_ps();
        vValues = _mm_shuffle_ps(vValues, Internal::kIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
        // 0,0,fRange,0.0f
        vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
        M.r[2] = vTemp;
        // 0,0,-fRange * NearZ,1.0f
        vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
        M.r[3] = vTemp;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMMATRIX XMMatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
    inline vfloat4x4 VMATH_CALLCONV vMat4OrthographicOffCenter(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ)
    {
        assert(!NearEqual(ViewRight, ViewLeft, 0.00001f));
        assert(!NearEqual(ViewTop, ViewBottom, 0.00001f));
        assert(!NearEqual(FarZ, NearZ, 0.00001f));

#if defined(VMATH_NO_INTRINSICS)

        float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
        float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
        float fRange = 1.0f / (FarZ - NearZ);

        vfloat4x4 M;
        M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = 0.0f;

        M.m[3][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
        M.m[3][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
        M.m[3][2] = -fRange * NearZ;
        M.m[3][3] = 1.0f;
        return M;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
        float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
        float fRange = 1.0f / (FarZ - NearZ);
        const float32x4_t Zero = vdupq_n_f32(0);
        vfloat4x4 M;
        M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
        M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
        M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
        M.r[3] = v4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
            -(ViewTop + ViewBottom) * ReciprocalHeight,
            -fRange * NearZ,
            1.0f);
        return M;
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4x4 M;
        float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
        float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
        float fRange = 1.0f / (FarZ - NearZ);
        // Note: This is recorded on the stack
        vfloat4 rMem = {
            fReciprocalWidth,
            fReciprocalHeight,
            fRange,
            1.0f
        };
        vfloat4 rMem2 = {
            -(ViewLeft + ViewRight),
            -(ViewTop + ViewBottom),
            -NearZ,
            1.0f
        };
        // Copy from memory to SSE register
        vfloat4 vValues = rMem;
        vfloat4 vTemp = _mm_setzero_ps();
        // Copy x only
        vTemp = _mm_move_ss(vTemp, vValues);
        // fReciprocalWidth*2,0,0,0
        vTemp = _mm_add_ss(vTemp, vTemp);
        M.r[0] = vTemp;
        // 0,fReciprocalHeight*2,0,0
        vTemp = vValues;
        vTemp = _mm_and_ps(vTemp, Internal::kMaskY);
        vTemp = _mm_add_ps(vTemp, vTemp);
        M.r[1] = vTemp;
        // 0,0,fRange,0.0f
        vTemp = vValues;
        vTemp = _mm_and_ps(vTemp, Internal::kMaskZ);
        M.r[2] = vTemp;
        // -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
        vValues = _mm_mul_ps(vValues, rMem2);
        M.r[3] = vValues;
        return M;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionMultiply(FXMVECTOR Q1, FXMVECTOR Q2)
    inline vfloat4 VMATH_CALLCONV vQuatMul(vfloat4 Q1, vfloat4 Q2)
    {
        // Returns the product Q2*Q1 (which is the concatenation of a rotation Q1 followed by the rotation Q2)

        // [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
        //   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
        //   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
        //   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                (Q2.v4f[3] * Q1.v4f[0]) + (Q2.v4f[0] * Q1.v4f[3]) + (Q2.v4f[1] * Q1.v4f[2]) - (Q2.v4f[2] * Q1.v4f[1]),
                (Q2.v4f[3] * Q1.v4f[1]) - (Q2.v4f[0] * Q1.v4f[2]) + (Q2.v4f[1] * Q1.v4f[3]) + (Q2.v4f[2] * Q1.v4f[0]),
                (Q2.v4f[3] * Q1.v4f[2]) + (Q2.v4f[0] * Q1.v4f[1]) - (Q2.v4f[1] * Q1.v4f[0]) + (Q2.v4f[2] * Q1.v4f[3]),
                (Q2.v4f[3] * Q1.v4f[3]) - (Q2.v4f[0] * Q1.v4f[0]) - (Q2.v4f[1] * Q1.v4f[1]) - (Q2.v4f[2] * Q1.v4f[2])
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
        static const Internal::vfloat4f32 ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
        static const Internal::vfloat4f32 ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };

        float32x2_t Q2L = vget_low_f32(Q2);
        float32x2_t Q2H = vget_high_f32(Q2);

        float32x4_t Q2X = vdupq_lane_f32(Q2L, 0);
        float32x4_t Q2Y = vdupq_lane_f32(Q2L, 1);
        float32x4_t Q2Z = vdupq_lane_f32(Q2H, 0);
        vfloat4 vResult = vmulq_lane_f32(Q1, Q2H, 1);

        // Mul by Q1WZYX
        float32x4_t vTemp = vrev64q_f32(Q1);
        vTemp = vcombine_f32(vget_high_f32(vTemp), vget_low_f32(vTemp));
        Q2X = vmulq_f32(Q2X, vTemp);
        vResult = vmlaq_f32(vResult, Q2X, ControlWZYX);

        // Mul by Q1ZWXY
        vTemp = vreinterpretq_f32_u32(vrev64q_u32(vreinterpretq_u32_f32(vTemp)));
        Q2Y = vmulq_f32(Q2Y, vTemp);
        vResult = vmlaq_f32(vResult, Q2Y, ControlZWXY);

        // Mul by Q1YXWZ
        vTemp = vreinterpretq_f32_u32(vrev64q_u32(vreinterpretq_u32_f32(vTemp)));
        vTemp = vcombine_f32(vget_high_f32(vTemp), vget_low_f32(vTemp));
        Q2Z = vmulq_f32(Q2Z, vTemp);
        vResult = vmlaq_f32(vResult, Q2Z, ControlYXWZ);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 ControlWZYX = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };
        static const Internal::vfloat4f32 ControlZWXY = { { { 1.0f, 1.0f, -1.0f, -1.0f } } };
        static const Internal::vfloat4f32 ControlYXWZ = { { { -1.0f, 1.0f, 1.0f, -1.0f } } };
        // Copy to SSE registers and use as few as possible for x86
        vfloat4 Q2X = Q2;
        vfloat4 Q2Y = Q2;
        vfloat4 Q2Z = Q2;
        vfloat4 vResult = Q2;
        // Splat with one instruction
        vResult = VMATH_PERMUTE_PS(vResult, _MM_SHUFFLE(3, 3, 3, 3));
        Q2X = VMATH_PERMUTE_PS(Q2X, _MM_SHUFFLE(0, 0, 0, 0));
        Q2Y = VMATH_PERMUTE_PS(Q2Y, _MM_SHUFFLE(1, 1, 1, 1));
        Q2Z = VMATH_PERMUTE_PS(Q2Z, _MM_SHUFFLE(2, 2, 2, 2));
        // Retire Q1 and perform Q1*Q2W
        vResult = _mm_mul_ps(vResult, Q1);
        vfloat4 Q1Shuffle = Q1;
        // Shuffle the copies of Q1
        Q1Shuffle = VMATH_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
        // Mul by Q1WZYX
        Q2X = _mm_mul_ps(Q2X, Q1Shuffle);
        Q1Shuffle = VMATH_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(2, 3, 0, 1));
        // Flip the signs on y and z
        vResult = VMATH_FMADD_PS(Q2X, ControlWZYX, vResult);
        // Mul by Q1ZWXY
        Q2Y = _mm_mul_ps(Q2Y, Q1Shuffle);
        Q1Shuffle = VMATH_PERMUTE_PS(Q1Shuffle, _MM_SHUFFLE(0, 1, 2, 3));
        // Flip the signs on z and w
        Q2Y = _mm_mul_ps(Q2Y, ControlZWXY);
        // Mul by Q1YXWZ
        Q2Z = _mm_mul_ps(Q2Z, Q1Shuffle);
        // Flip the signs on x and w
        Q2Y = VMATH_FMADD_PS(Q2Z, ControlYXWZ, Q2Y);
        vResult = _mm_add_ps(vResult, Q2Y);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionLengthSq(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatLengthSq(vfloat4 Q)
    {
        return v4LengthSq(Q);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionReciprocalLength(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatRLength(vfloat4 Q)
    {
        return v4RLength(Q);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionLength(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatLength(vfloat4 Q)
    {
        return v4Length(Q);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionNormalizeEst(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatNormalizeEst(vfloat4 Q)
    {
        return v4NormalizeEst(Q);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionNormalize(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatNormalize(vfloat4 Q)
    {
        return v4Normalize(Q);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionConjugate(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatConjugate(vfloat4 Q)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 Result = { { {
                -Q.v4f[0],
                -Q.v4f[1],
                -Q.v4f[2],
                Q.v4f[3]
            } } };
        return Result.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
        return vmulq_f32(Q, NegativeOne3.v);
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 NegativeOne3 = { { { -1.0f, -1.0f, -1.0f, 1.0f } } };
        return _mm_mul_ps(Q, NegativeOne3);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionInverse(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatInverse(vfloat4 Q)
    {
        const vfloat4  Zero = v4Zero();

        vfloat4 L = v4LengthSq(Q);
        vfloat4 Conjugate = vQuatConjugate(Q);

        vfloat4 Control = v4LessEqual(L, Internal::kEpsilon.v);

        vfloat4 Result = v4Div(Conjugate, L);

        Result = v4Select(Result, Zero, Control);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionLn(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatLn(vfloat4 Q)
    {
        static const Internal::vfloat4f32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

        vfloat4 QW = v4wwww(Q);
        vfloat4 Q0 = v4Select(Internal::kSelect1110.v, Q, Internal::kSelect1110.v);

        vfloat4 ControlW = v4InBoundsAbs(QW, OneMinusEpsilon.v);

        vfloat4 Theta = v4ACos(QW);
        vfloat4 SinTheta = v4Sin(Theta);

        vfloat4 S = v4Div(Theta, SinTheta);

        vfloat4 Result = v4Mul(Q0, S);
        Result = v4Select(Q0, Result, ControlW);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionExp(FXMVECTOR Q)
    inline vfloat4 VMATH_CALLCONV vQuatExp(vfloat4 Q)
    {
        vfloat4 Theta = v3Length(Q);

        vfloat4 SinTheta, CosTheta;
        v4SinCos(&SinTheta, &CosTheta, Theta);

        vfloat4 S = v4Div(SinTheta, Theta);

        vfloat4 Result = v4Mul(Q, S);

        const vfloat4 Zero = v4Zero();
        vfloat4 Control = v4NearEqual(Theta, Zero, Internal::kEpsilon.v);
        Result = v4Select(Result, Q, Control);

        Result = v4Select(CosTheta, Result, Internal::kSelect1110.v);

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionSlerp(FXMVECTOR Q0, FXMVECTOR Q1, float t)
    inline vfloat4 VMATH_CALLCONV vQuatSlerp(vfloat4 Q0, vfloat4 Q1, float t)
    {
        vfloat4 T = v4Spread(t);
        return vQuatSlerp(Q0, Q1, T);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionSlerpV(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR T)
    inline vfloat4 VMATH_CALLCONV vQuatSlerp(vfloat4 Q0, vfloat4 Q1, vfloat4 T)
    {
        assert((v4GetY(T) == v4GetX(T)) && (v4GetZ(T) == v4GetX(T)) && (v4GetW(T) == v4GetX(T)));

        // Result = Q0 * sin((1.0 - t) * Omega) / sin(Omega) + Q1 * sin(t * Omega) / sin(Omega)

#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        const Internal::vfloat4f32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };

        vfloat4 CosOmega = v4Dot(Q0, Q1);

        const vfloat4 Zero = v4Zero();
        vfloat4 Control = v4Less(CosOmega, Zero);
        vfloat4 Sign = v4Select(Internal::kOne.v, Internal::kNegativeOne.v, Control);

        CosOmega = v4Mul(CosOmega, Sign);

        Control = v4Less(CosOmega, OneMinusEpsilon);

        vfloat4 SinOmega = v4NegateMulAdd(CosOmega, CosOmega, Internal::kOne.v);
        SinOmega = v4Sqrt(SinOmega);

        vfloat4 Omega = v4ATan2(SinOmega, CosOmega);

        vfloat4 SignMask = v4SignMask();
        vfloat4 V01 = v4PushLeft(T, Zero, 2);
        SignMask = v4PushLeft(SignMask, Zero, 3);
        V01 = v4Xor(V01, SignMask);
        V01 = v4Add(Internal::kIdentityR0.v, V01);

        vfloat4 InvSinOmega = v4Rcp(SinOmega);

        vfloat4 S0 = v4Mul(V01, Omega);
        S0 = v4Sin(S0);
        S0 = v4Mul(S0, InvSinOmega);

        S0 = v4Select(V01, S0, Control);

        vfloat4 S1 = v4yyyy(S0);
        S0 = v4xxxx(S0);

        S1 = v4Mul(S1, Sign);

        vfloat4 Result = v4Mul(Q0, S0);
        Result = v4MulAdd(Q1, S1, Result);

        return Result;

#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 OneMinusEpsilon = { { { 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f } } };
        static const Internal::vfloat4u32 SignMask2 = { { { 0x80000000, 0x00000000, 0x00000000, 0x00000000 } } };

        vfloat4 CosOmega = v4Dot(Q0, Q1);

        const vfloat4 Zero = v4Zero();
        vfloat4 Control = v4Less(CosOmega, Zero);
        vfloat4 Sign = v4Select(Internal::kOne, Internal::kNegativeOne, Control);

        CosOmega = _mm_mul_ps(CosOmega, Sign);

        Control = v4Less(CosOmega, OneMinusEpsilon);

        vfloat4 SinOmega = _mm_mul_ps(CosOmega, CosOmega);
        SinOmega = _mm_sub_ps(Internal::kOne, SinOmega);
        SinOmega = _mm_sqrt_ps(SinOmega);

        vfloat4 Omega = v4ATan2(SinOmega, CosOmega);

        vfloat4 V01 = VMATH_PERMUTE_PS(T, _MM_SHUFFLE(2, 3, 0, 1));
        V01 = _mm_and_ps(V01, Internal::kMaskXY);
        V01 = _mm_xor_ps(V01, SignMask2);
        V01 = _mm_add_ps(Internal::kIdentityR0, V01);

        vfloat4 S0 = _mm_mul_ps(V01, Omega);
        S0 = v4Sin(S0);
        S0 = _mm_div_ps(S0, SinOmega);

        S0 = v4Select(V01, S0, Control);

        vfloat4 S1 = v4yyyy(S0);
        S0 = v4xxxx(S0);

        S1 = _mm_mul_ps(S1, Sign);
        vfloat4 Result = _mm_mul_ps(Q0, S0);
        S1 = _mm_mul_ps(S1, Q1);
        Result = _mm_add_ps(Result, S1);
        return Result;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionSquad(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, GXMVECTOR Q3, float t)
    inline vfloat4 VMATH_CALLCONV vQuatSquad(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 Q3, float t)
    {
        vfloat4 T = v4Spread(t);
        return vQuatSquad(Q0, Q1, Q2, Q3, T);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionSquadV(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, GXMVECTOR Q3, HXMVECTOR T)
    inline vfloat4 VMATH_CALLCONV vQuatSquad(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 Q3, vfloat4 T)
    {
        assert((v4GetY(T) == v4GetX(T)) && (v4GetZ(T) == v4GetX(T)) && (v4GetW(T) == v4GetX(T)));

        vfloat4 TP = T;
        const vfloat4 Two = v4Spread(2);

        vfloat4 Q03 = vQuatSlerp(Q0, Q3, T);
        vfloat4 Q12 = vQuatSlerp(Q1, Q2, T);

        TP = v4NegateMulAdd(TP, TP, TP);
        TP = v4Mul(TP, Two);

        vfloat4 Result = vQuatSlerp(Q03, Q12, TP);

        return Result;
    }

    //------------------------------------------------------------------------------
    // void XMQuaternionSquadSetup(XMVECTOR* pA, XMVECTOR* pB, XMVECTOR* pC, FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, GXMVECTOR Q3)
    inline void VMATH_CALLCONV vQuatSquadSetup(vfloat4* pA, vfloat4* pB, vfloat4* pC, vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 Q3)
    {
        assert(pA);
        assert(pB);
        assert(pC);

        vfloat4 LS12 = vQuatLengthSq(v4Add(Q1, Q2));
        vfloat4 LD12 = vQuatLengthSq(v4Sub(Q1, Q2));
        vfloat4 SQ2 = v4Negate(Q2);

        vfloat4 Control1 = v4Less(LS12, LD12);
        SQ2 = v4Select(Q2, SQ2, Control1);

        vfloat4 LS01 = vQuatLengthSq(v4Add(Q0, Q1));
        vfloat4 LD01 = vQuatLengthSq(v4Sub(Q0, Q1));
        vfloat4 SQ0 = v4Negate(Q0);

        vfloat4 LS23 = vQuatLengthSq(v4Add(SQ2, Q3));
        vfloat4 LD23 = vQuatLengthSq(v4Sub(SQ2, Q3));
        vfloat4 SQ3 = v4Negate(Q3);

        vfloat4 Control0 = v4Less(LS01, LD01);
        vfloat4 Control2 = v4Less(LS23, LD23);

        SQ0 = v4Select(Q0, SQ0, Control0);
        SQ3 = v4Select(Q3, SQ3, Control2);

        vfloat4 InvQ1 = vQuatInverse(Q1);
        vfloat4 InvQ2 = vQuatInverse(SQ2);

        vfloat4 LnQ0 = vQuatLn(vQuatMul(InvQ1, SQ0));
        vfloat4 LnQ2 = vQuatLn(vQuatMul(InvQ1, SQ2));
        vfloat4 LnQ1 = vQuatLn(vQuatMul(InvQ2, Q1));
        vfloat4 LnQ3 = vQuatLn(vQuatMul(InvQ2, SQ3));

        const vfloat4 NegativeOneQuarter = v4Spread(-0.25f);

        vfloat4 ExpQ02 = v4Mul(v4Add(LnQ0, LnQ2), NegativeOneQuarter);
        vfloat4 ExpQ13 = v4Mul(v4Add(LnQ1, LnQ3), NegativeOneQuarter);
        ExpQ02 = vQuatExp(ExpQ02);
        ExpQ13 = vQuatExp(ExpQ13);

        *pA = vQuatMul(Q1, ExpQ02);
        *pB = vQuatMul(SQ2, ExpQ13);
        *pC = SQ2;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionBaryCentric(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, float f, float g)
    inline vfloat4 VMATH_CALLCONV vQuatBaryCentric(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, float f, float g)
    {
        float s = f + g;

        vfloat4 Result;
        if ((s < 0.00001f) && (s > -0.00001f))
        {
            Result = Q0;
        }
        else
        {
            vfloat4 Q01 = vQuatSlerp(Q0, Q1, s);
            vfloat4 Q02 = vQuatSlerp(Q0, Q2, s);

            Result = vQuatSlerp(Q01, Q02, g / s);
        }

        return Result;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionBaryCentricV(FXMVECTOR Q0, FXMVECTOR Q1, FXMVECTOR Q2, GXMVECTOR F, HXMVECTOR G)
    inline vfloat4 VMATH_CALLCONV vQuatBaryCentric(vfloat4 Q0, vfloat4 Q1, vfloat4 Q2, vfloat4 F, vfloat4 G)
    {
        assert((v4GetY(F) == v4GetX(F)) && (v4GetZ(F) == v4GetX(F)) && (v4GetW(F) == v4GetX(F)));
        assert((v4GetY(G) == v4GetX(G)) && (v4GetZ(G) == v4GetX(G)) && (v4GetW(G) == v4GetX(G)));

        const vfloat4 Epsilon = v4Spread(1.0f / (1 << 16));

        vfloat4 S = v4Add(F, G);

        vfloat4 Result;
        if (v4AllInBoundsAbs(S, Epsilon))
        {
            Result = Q0;
        }
        else
        {
            vfloat4 Q01 = vQuatSlerp(Q0, Q1, S);
            vfloat4 Q02 = vQuatSlerp(Q0, Q2, S);
            vfloat4 GS = v4Rcp(S);
            GS = v4Mul(G, GS);

            Result = vQuatSlerp(Q01, Q02, GS);
        }

        return Result;
    }

    //------------------------------------------------------------------------------
    // Transformation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionIdentity()
    inline vfloat4 VMATH_CALLCONV vQuatIdentity()
    {
        return v4Set(0, 0, 0, 1);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    inline vfloat4 VMATH_CALLCONV vQuatRotationRollPitchYaw(float Pitch, float Yaw, float Roll)
    {
        vfloat4 Angles = v4Set(Pitch, Yaw, Roll, 0.0f);
        vfloat4 Q = vQuatRotationRollPitchYawFromVector(Angles);
        return Q;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionRotationRollPitchYawFromVector(FXMVECTOR Angles)
    inline vfloat4 VMATH_CALLCONV vQuatRotationRollPitchYawFromVector(vfloat4 Angles)
    {
        static const Internal::vfloat4f32  Sign = { { { 1.0f, -1.0f, -1.0f, 1.0f } } };

        vfloat4 HalfAngles = v4Mul(Angles, Internal::kOneHalf.v);

        vfloat4 SinAngles, CosAngles;
        v4SinCos(&SinAngles, &CosAngles, HalfAngles);

        vfloat4 P0 = v4Shuffle<0, 4, 4, 4>(SinAngles, CosAngles);
        vfloat4 Y0 = v4Shuffle<5, 1, 5, 5>(SinAngles, CosAngles);
        vfloat4 R0 = v4Shuffle<6, 6, 2, 6>(SinAngles, CosAngles);
        vfloat4 P1 = v4Shuffle<0, 4, 4, 4>(CosAngles, SinAngles);
        vfloat4 Y1 = v4Shuffle<5, 1, 5, 5>(CosAngles, SinAngles);
        vfloat4 R1 = v4Shuffle<6, 6, 2, 6>(CosAngles, SinAngles);

        vfloat4 Q1 = v4Mul(P1, Sign.v);
        vfloat4 Q0 = v4Mul(P0, Y0);
        Q1 = v4Mul(Q1, Y1);
        Q0 = v4Mul(Q0, R0);
        vfloat4 Q = v4MulAdd(Q1, R1, Q0);

        return Q;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionRotationNormal(FXMVECTOR NormalAxis, float Angle)
    inline vfloat4 VMATH_CALLCONV vQuatNormalAngle(vfloat4 NormalAxis, float Angle)
    {
#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        vfloat4 N = v4Select(Internal::kOne.v, NormalAxis, Internal::kSelect1110.v);

        float SinV, CosV;
        SinCos(&SinV, &CosV, 0.5f * Angle);

        vfloat4 Scale = v4Set(SinV, SinV, SinV, CosV);
        return v4Mul(N, Scale);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 N = _mm_and_ps(NormalAxis, Internal::kMask3);
        N = _mm_or_ps(N, Internal::kIdentityR3);
        vfloat4 Scale = _mm_set_ps1(0.5f * Angle);
        vfloat4 vSine;
        vfloat4 vCosine;
        v4SinCos(&vSine, &vCosine, Scale);
        Scale = _mm_and_ps(vSine, Internal::kMask3);
        vCosine = _mm_and_ps(vCosine, Internal::kMaskW);
        Scale = _mm_or_ps(Scale, vCosine);
        N = _mm_mul_ps(N, Scale);
        return N;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionRotationAxis(FXMVECTOR Axis, float Angle)
    inline vfloat4 VMATH_CALLCONV vQuatAxisAngle(vfloat4 Axis, float Angle)
    {
        assert(!v3AllEqual(Axis, v4Zero()));
        assert(!v3AnyIsInfinite(Axis));

        vfloat4 Normal = v3Normalize(Axis);
        vfloat4 Q = vQuatNormalAngle(Normal, Angle);
        return Q;
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMQuaternionRotationMatrix(FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV vQuatRotationMatrix(vfloat4x4 M)
    {
#if defined(VMATH_NO_INTRINSICS)

        Internal::vfloat4f32 q;
        float r22 = M.m[2][2];
        if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
        {
            float dif10 = M.m[1][1] - M.m[0][0];
            float omr22 = 1.f - r22;
            if (dif10 <= 0.f)  // x^2 >= y^2
            {
                float fourXSqr = omr22 - dif10;
                float inv4x = 0.5f / sqrtf(fourXSqr);
                q.f[0] = fourXSqr * inv4x;
                q.f[1] = (M.m[0][1] + M.m[1][0]) * inv4x;
                q.f[2] = (M.m[0][2] + M.m[2][0]) * inv4x;
                q.f[3] = (M.m[1][2] - M.m[2][1]) * inv4x;
            }
            else  // y^2 >= x^2
            {
                float fourYSqr = omr22 + dif10;
                float inv4y = 0.5f / sqrtf(fourYSqr);
                q.f[0] = (M.m[0][1] + M.m[1][0]) * inv4y;
                q.f[1] = fourYSqr * inv4y;
                q.f[2] = (M.m[1][2] + M.m[2][1]) * inv4y;
                q.f[3] = (M.m[2][0] - M.m[0][2]) * inv4y;
            }
        }
        else  // z^2 + w^2 >= x^2 + y^2
        {
            float sum10 = M.m[1][1] + M.m[0][0];
            float opr22 = 1.f + r22;
            if (sum10 <= 0.f)  // z^2 >= w^2
            {
                float fourZSqr = opr22 - sum10;
                float inv4z = 0.5f / sqrtf(fourZSqr);
                q.f[0] = (M.m[0][2] + M.m[2][0]) * inv4z;
                q.f[1] = (M.m[1][2] + M.m[2][1]) * inv4z;
                q.f[2] = fourZSqr * inv4z;
                q.f[3] = (M.m[0][1] - M.m[1][0]) * inv4z;
            }
            else  // w^2 >= z^2
            {
                float fourWSqr = opr22 + sum10;
                float inv4w = 0.5f / sqrtf(fourWSqr);
                q.f[0] = (M.m[1][2] - M.m[2][1]) * inv4w;
                q.f[1] = (M.m[2][0] - M.m[0][2]) * inv4w;
                q.f[2] = (M.m[0][1] - M.m[1][0]) * inv4w;
                q.f[3] = fourWSqr * inv4w;
            }
        }
        return q.v;

#elif defined(VMATH_ARM_NEON_INTRINSICS)
        static const Internal::vfloat4f32 PMMP = { { { +1.0f, -1.0f, -1.0f, +1.0f } } };
        static const Internal::vfloat4f32 MPMP = { { { -1.0f, +1.0f, -1.0f, +1.0f } } };
        static const Internal::vfloat4f32 MMPP = { { { -1.0f, -1.0f, +1.0f, +1.0f } } };
        static const Internal::vfloat4u32 Select0110 = { { { 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000 } } };
        static const Internal::vfloat4u32 Select0010 = { { { 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000 } } };

        float32x4_t r0 = M.r[0];
        float32x4_t r1 = M.r[1];
        float32x4_t r2 = M.r[2];

        float32x4_t r00 = vdupq_lane_f32(vget_low_f32(r0), 0);
        float32x4_t r11 = vdupq_lane_f32(vget_low_f32(r1), 1);
        float32x4_t r22 = vdupq_lane_f32(vget_high_f32(r2), 0);

        // x^2 >= y^2 equivalent to r11 - r00 <= 0
        float32x4_t r11mr00 = vsubq_f32(r11, r00);
        uint32x4_t x2gey2 = vcleq_f32(r11mr00, Internal::kZero);

        // z^2 >= w^2 equivalent to r11 + r00 <= 0
        float32x4_t r11pr00 = vaddq_f32(r11, r00);
        uint32x4_t z2gew2 = vcleq_f32(r11pr00, Internal::kZero);

        // x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
        uint32x4_t x2py2gez2pw2 = vcleq_f32(r22, Internal::kZero);

        // (4*x^2, 4*y^2, 4*z^2, 4*w^2)
        float32x4_t t0 = vmulq_f32(PMMP, r00);
        float32x4_t x2y2z2w2 = vmlaq_f32(t0, MPMP, r11);
        x2y2z2w2 = vmlaq_f32(x2y2z2w2, MMPP, r22);
        x2y2z2w2 = vaddq_f32(x2y2z2w2, Internal::kOne);

        // (r01, r02, r12, r11)
        t0 = vextq_f32(r0, r0, 1);
        float32x4_t t1 = vextq_f32(r1, r1, 1);
        t0 = vcombine_f32(vget_low_f32(t0), vrev64_f32(vget_low_f32(t1)));

        // (r10, r20, r21, r10)
        t1 = vextq_f32(r2, r2, 3);
        float32x4_t r10 = vdupq_lane_f32(vget_low_f32(r1), 0);
        t1 = vbslq_f32(Select0110, t1, r10);

        // (4*x*y, 4*x*z, 4*y*z, unused)
        float32x4_t xyxzyz = vaddq_f32(t0, t1);

        // (r21, r20, r10, r10)
        t0 = vcombine_f32(vrev64_f32(vget_low_f32(r2)), vget_low_f32(r10));

        // (r12, r02, r01, r12)
        float32x4_t t2 = vcombine_f32(vrev64_f32(vget_high_f32(r0)), vrev64_f32(vget_low_f32(r0)));
        float32x4_t t3 = vdupq_lane_f32(vget_high_f32(r1), 0);
        t1 = vbslq_f32(Select0110, t2, t3);

        // (4*x*w, 4*y*w, 4*z*w, unused)
        float32x4_t xwywzw = vsubq_f32(t0, t1);
        xwywzw = vmulq_f32(MPMP, xwywzw);

        // (4*x*x, 4*x*y, 4*x*z, 4*x*w)
        t0 = vextq_f32(xyxzyz, xyxzyz, 3);
        t1 = vbslq_f32(Select0110, t0, x2y2z2w2);
        t2 = vdupq_lane_f32(vget_low_f32(xwywzw), 0);
        float32x4_t tensor0 = vbslq_f32(Internal::kSelect1110, t1, t2);

        // (4*y*x, 4*y*y, 4*y*z, 4*y*w)
        t0 = vbslq_f32(Internal::kSelect1011, xyxzyz, x2y2z2w2);
        t1 = vdupq_lane_f32(vget_low_f32(xwywzw), 1);
        float32x4_t tensor1 = vbslq_f32(Internal::kSelect1110, t0, t1);

        // (4*z*x, 4*z*y, 4*z*z, 4*z*w)
        t0 = vextq_f32(xyxzyz, xyxzyz, 1);
        t1 = vcombine_f32(vget_low_f32(t0), vrev64_f32(vget_high_f32(xwywzw)));
        float32x4_t tensor2 = vbslq_f32(Select0010, x2y2z2w2, t1);

        // (4*w*x, 4*w*y, 4*w*z, 4*w*w)
        float32x4_t tensor3 = vbslq_f32(Internal::kSelect1110, xwywzw, x2y2z2w2);

        // Select the row of the tensor-product matrix that has the largest
        // magnitude.
        t0 = vbslq_f32(x2gey2, tensor0, tensor1);
        t1 = vbslq_f32(z2gew2, tensor2, tensor3);
        t2 = vbslq_f32(x2py2gez2pw2, t0, t1);

        // Normalize the row.  No division by zero is possible because the
        // quaternion is unit-length (and the row is a nonzero multiple of
        // the quaternion).
        t0 = v4Length(t2);
        return v4Div(t2, t0);
#elif defined(VMATH_SSE_INTRINSICS)
        static const Internal::vfloat4f32 PMMP = { { { +1.0f, -1.0f, -1.0f, +1.0f } } };
        static const Internal::vfloat4f32 MPMP = { { { -1.0f, +1.0f, -1.0f, +1.0f } } };
        static const Internal::vfloat4f32 MMPP = { { { -1.0f, -1.0f, +1.0f, +1.0f } } };

        vfloat4 r0 = M.r[0];  // (r00, r01, r02, 0)
        vfloat4 r1 = M.r[1];  // (r10, r11, r12, 0)
        vfloat4 r2 = M.r[2];  // (r20, r21, r22, 0)

        // (r00, r00, r00, r00)
        vfloat4 r00 = VMATH_PERMUTE_PS(r0, _MM_SHUFFLE(0, 0, 0, 0));
        // (r11, r11, r11, r11)
        vfloat4 r11 = VMATH_PERMUTE_PS(r1, _MM_SHUFFLE(1, 1, 1, 1));
        // (r22, r22, r22, r22)
        vfloat4 r22 = VMATH_PERMUTE_PS(r2, _MM_SHUFFLE(2, 2, 2, 2));

        // x^2 >= y^2 equivalent to r11 - r00 <= 0
        // (r11 - r00, r11 - r00, r11 - r00, r11 - r00)
        vfloat4 r11mr00 = _mm_sub_ps(r11, r00);
        vfloat4 x2gey2 = _mm_cmple_ps(r11mr00, Internal::kZero);

        // z^2 >= w^2 equivalent to r11 + r00 <= 0
        // (r11 + r00, r11 + r00, r11 + r00, r11 + r00)
        vfloat4 r11pr00 = _mm_add_ps(r11, r00);
        vfloat4 z2gew2 = _mm_cmple_ps(r11pr00, Internal::kZero);

        // x^2 + y^2 >= z^2 + w^2 equivalent to r22 <= 0
        vfloat4 x2py2gez2pw2 = _mm_cmple_ps(r22, Internal::kZero);

        // (4*x^2, 4*y^2, 4*z^2, 4*w^2)
        vfloat4 t0 = VMATH_FMADD_PS(PMMP, r00, Internal::kOne);
        vfloat4 t1 = _mm_mul_ps(MPMP, r11);
        vfloat4 t2 = VMATH_FMADD_PS(MMPP, r22, t0);
        vfloat4 x2y2z2w2 = _mm_add_ps(t1, t2);

        // (r01, r02, r12, r11)
        t0 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 2, 2, 1));
        // (r10, r10, r20, r21)
        t1 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 0, 0));
        // (r10, r20, r21, r10)
        t1 = VMATH_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
        // (4*x*y, 4*x*z, 4*y*z, unused)
        vfloat4 xyxzyz = _mm_add_ps(t0, t1);

        // (r21, r20, r10, r10)
        t0 = _mm_shuffle_ps(r2, r1, _MM_SHUFFLE(0, 0, 0, 1));
        // (r12, r12, r02, r01)
        t1 = _mm_shuffle_ps(r1, r0, _MM_SHUFFLE(1, 2, 2, 2));
        // (r12, r02, r01, r12)
        t1 = VMATH_PERMUTE_PS(t1, _MM_SHUFFLE(1, 3, 2, 0));
        // (4*x*w, 4*y*w, 4*z*w, unused)
        vfloat4 xwywzw = _mm_sub_ps(t0, t1);
        xwywzw = _mm_mul_ps(MPMP, xwywzw);

        // (4*x^2, 4*y^2, 4*x*y, unused)
        t0 = _mm_shuffle_ps(x2y2z2w2, xyxzyz, _MM_SHUFFLE(0, 0, 1, 0));
        // (4*z^2, 4*w^2, 4*z*w, unused)
        t1 = _mm_shuffle_ps(x2y2z2w2, xwywzw, _MM_SHUFFLE(0, 2, 3, 2));
        // (4*x*z, 4*y*z, 4*x*w, 4*y*w)
        t2 = _mm_shuffle_ps(xyxzyz, xwywzw, _MM_SHUFFLE(1, 0, 2, 1));

        // (4*x*x, 4*x*y, 4*x*z, 4*x*w)
        vfloat4 tensor0 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(2, 0, 2, 0));
        // (4*y*x, 4*y*y, 4*y*z, 4*y*w)
        vfloat4 tensor1 = _mm_shuffle_ps(t0, t2, _MM_SHUFFLE(3, 1, 1, 2));
        // (4*z*x, 4*z*y, 4*z*z, 4*z*w)
        vfloat4 tensor2 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(2, 0, 1, 0));
        // (4*w*x, 4*w*y, 4*w*z, 4*w*w)
        vfloat4 tensor3 = _mm_shuffle_ps(t2, t1, _MM_SHUFFLE(1, 2, 3, 2));

        // Select the row of the tensor-product matrix that has the largest
        // magnitude.
        t0 = _mm_and_ps(x2gey2, tensor0);
        t1 = _mm_andnot_ps(x2gey2, tensor1);
        t0 = _mm_or_ps(t0, t1);
        t1 = _mm_and_ps(z2gew2, tensor2);
        t2 = _mm_andnot_ps(z2gew2, tensor3);
        t1 = _mm_or_ps(t1, t2);
        t0 = _mm_and_ps(x2py2gez2pw2, t0);
        t1 = _mm_andnot_ps(x2py2gez2pw2, t1);
        t2 = _mm_or_ps(t0, t1);

        // Normalize the row.  No division by zero is possible because the
        // quaternion is unit-length (and the row is a nonzero multiple of
        // the quaternion).
        t0 = v4Length(t2);
        return _mm_div_ps(t2, t0);
#endif
    }

    //------------------------------------------------------------------------------
    // Conversion operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------
    // void XMQuaternionToAxisAngle(XMVECTOR* pAxis, float* pAngle, FXMVECTOR Q)
    inline void VMATH_CALLCONV vQuatToAxisAngle(vfloat4* pAxis, float* pAngle, vfloat4 Q)
    {
        assert(pAxis);
        assert(pAngle);

        *pAxis = Q;

        *pAngle = 2.0f * ACos(v4GetW(Q));
    }

    /****************************************************************************
     *
     * Plane
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------
    // XMPlaneNormalizeEst uses a reciprocal estimate and
    // returns QNaN on zero and infinite vectors.

    // XMVECTOR XMPlaneNormalizeEst(FXMVECTOR P)
    inline vfloat4 VMATH_CALLCONV vPlaneNormalizeEst(vfloat4 P)
    {
#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        vfloat4 Result = v3RLengthEst(P);
        return v4Mul(P, Result);

#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vTemp = _mm_dp_ps(P, P, 0x7f);
        vfloat4 vResult = _mm_rsqrt_ps(vTemp);
        return _mm_mul_ps(vResult, P);
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product
        vfloat4 vDot = _mm_mul_ps(P, P);
        // x=Dot.y, y=Dot.z
        vfloat4 vTemp = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
        // Result.x = x+y
        vDot = _mm_add_ss(vDot, vTemp);
        // x=Dot.z
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        // Result.x = (x+y)+z
        vDot = _mm_add_ss(vDot, vTemp);
        // Splat x
        vDot = VMATH_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
        // Get the reciprocal
        vDot = _mm_rsqrt_ps(vDot);
        // Get the reciprocal
        vDot = _mm_mul_ps(vDot, P);
        return vDot;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMPlaneNormalize(FXMVECTOR P)
    inline vfloat4 VMATH_CALLCONV vPlaneNormalize(vfloat4 P)
    {
#if defined(VMATH_NO_INTRINSICS)
        float fLengthSq = sqrtf((P.v4f[0] * P.v4f[0]) + (P.v4f[1] * P.v4f[1]) + (P.v4f[2] * P.v4f[2]));
        // Prevent divide by zero
        if (fLengthSq > 0)
        {
            fLengthSq = 1.0f / fLengthSq;
        }
        Internal::vfloat4f32 vResult = { { {
                P.v4f[0] * fLengthSq,
                P.v4f[1] * fLengthSq,
                P.v4f[2] * fLengthSq,
                P.v4f[3] * fLengthSq
            } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 vLength = v3RLength(P);
        return v4Mul(P, vLength);
#elif defined(VMATH_SSE4_INTRINSICS)
        vfloat4 vLengthSq = _mm_dp_ps(P, P, 0x7f);
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Reciprocal mul to perform the normalization
        vResult = _mm_div_ps(P, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vLengthSq);
        return vResult;
#elif defined(VMATH_SSE_INTRINSICS)
        // Perform the dot product on x,y and z only
        vfloat4 vLengthSq = _mm_mul_ps(P, P);
        vfloat4 vTemp = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vTemp = VMATH_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
        vLengthSq = _mm_add_ss(vLengthSq, vTemp);
        vLengthSq = VMATH_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
        // Prepare for the division
        vfloat4 vResult = _mm_sqrt_ps(vLengthSq);
        // Failsafe on zero (Or epsilon) length planes
        // If the length is infinity, set the elements to zero
        vLengthSq = _mm_cmpneq_ps(vLengthSq, Internal::kInfinity);
        // Reciprocal mul to perform the normalization
        vResult = _mm_div_ps(P, vResult);
        // Any that are infinity, set to zero
        vResult = _mm_and_ps(vResult, vLengthSq);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMPlaneIntersectLine(FXMVECTOR P, FXMVECTOR LinePoint1, FXMVECTOR LinePoint2)
    inline vfloat4 VMATH_CALLCONV vPlaneIntersectLine(vfloat4 P, vfloat4 LinePoint1, vfloat4 LinePoint2)
    {
        vfloat4 V1 = v3Dot(P, LinePoint1);
        vfloat4 V2 = v3Dot(P, LinePoint2);
        vfloat4 D = v4Sub(V1, V2);

        vfloat4 VT = v4Dot(P, v4Select(Internal::kOne.v, LinePoint1, Internal::kSelect1110.v));
        VT = v4Div(VT, D);

        vfloat4 Point = v4Sub(LinePoint2, LinePoint1);
        Point = v4MulAdd(Point, VT, LinePoint1);

        const vfloat4 Zero = v4Zero();
        vfloat4 Control = v4NearEqual(D, Zero, Internal::kEpsilon.v);

        return v4Select(Point, Internal::kQNaN.v, Control);
    }

    //------------------------------------------------------------------------------
    // void XMPlaneIntersectPlane(XMVECTOR* pLinePoint1, XMVECTOR* pLinePoint2, FXMVECTOR P1, FXMVECTOR P2)
    inline void VMATH_CALLCONV vPlaneIntersectPlane(vfloat4* pLinePoint1, vfloat4* pLinePoint2, vfloat4 P1, vfloat4 P2)
    {
        assert(pLinePoint1);
        assert(pLinePoint2);

        vfloat4 V1 = v3Cross(P2, P1);

        vfloat4 LengthSq = v3LengthSq(V1);

        vfloat4 V2 = v3Cross(P2, V1);

        vfloat4 P1W = v4wwww(P1);
        vfloat4 Point = v4Mul(V2, P1W);

        vfloat4 V3 = v3Cross(V1, P1);

        vfloat4 P2W = v4wwww(P2);
        Point = v4MulAdd(V3, P2W, Point);

        vfloat4 LinePoint1 = v4Div(Point, LengthSq);

        vfloat4 LinePoint2 = v4Add(LinePoint1, V1);

        vfloat4 Control = v4LessEqual(LengthSq, Internal::kEpsilon.v);
        *pLinePoint1 = v4Select(LinePoint1, Internal::kQNaN.v, Control);
        *pLinePoint2 = v4Select(LinePoint2, Internal::kQNaN.v, Control);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMPlaneTransform(FXMVECTOR P, FXMMATRIX M)
    inline vfloat4 VMATH_CALLCONV vPlaneTransform(vfloat4 P, vfloat4x4 M)
    {
        vfloat4 W = v4wwww(P);
        vfloat4 Z = v4zzzz(P);
        vfloat4 Y = v4yyyy(P);
        vfloat4 X = v4xxxx(P);

        vfloat4 Result = v4Mul(W, M.r[3]);
        Result = v4MulAdd(Z, M.r[2], Result);
        Result = v4MulAdd(Y, M.r[1], Result);
        Result = v4MulAdd(X, M.r[0], Result);
        return Result;
    }

    //------------------------------------------------------------------------------
    // Conversion operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMPlaneFromPointNormal(FXMVECTOR Point, FXMVECTOR Normal)
    inline vfloat4 VMATH_CALLCONV vPlaneFromPointNormal(vfloat4 Point, vfloat4 Normal)
    {
        vfloat4 W = v3Dot(Point, Normal);
        W = v4Negate(W);
        return v4Select(W, Normal, Internal::kSelect1110.v);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMPlaneFromPoints(FXMVECTOR Point1, FXMVECTOR Point2, FXMVECTOR Point3)
    inline vfloat4 VMATH_CALLCONV vPlaneFromPoints(vfloat4 Point1, vfloat4 Point2, vfloat4 Point3)
    {
        vfloat4 V21 = v4Sub(Point1, Point2);
        vfloat4 V31 = v4Sub(Point1, Point3);

        vfloat4 N = v3Cross(V21, V31);
        N = v3Normalize(N);

        vfloat4 D = v3Dot(N, Point1);
        D = v4Negate(D);

        vfloat4 Result = v4Select(D, N, Internal::kSelect1110.v);

        return Result;
    }

    /****************************************************************************
     *
     * Color
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------
    // Computation operations
    //------------------------------------------------------------------------------

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorNegative(FXMVECTOR vColor)
    inline vfloat4 VMATH_CALLCONV vColorNegative(vfloat4 vColor)
    {
#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult = { { {
                1.0f - vColor.v4f[0],
                1.0f - vColor.v4f[1],
                1.0f - vColor.v4f[2],
                vColor.v4f[3]
            } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32x4_t vTemp = veorq_u32(vreinterpretq_u32_f32(vColor), Internal::kNegate3);
        return vaddq_f32(vreinterpretq_f32_u32(vTemp), Internal::kOne3);
#elif defined(VMATH_SSE_INTRINSICS)
        // Negate only x,y and z.
        vfloat4 vTemp = _mm_xor_ps(vColor, Internal::kNegate3);
        // Add 1,1,1,0 to -x,-y,-z,w
        return _mm_add_ps(vTemp, Internal::kOne3);
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorAdjustSaturation(FXMVECTOR vColor, float fSaturation)
    inline vfloat4 VMATH_CALLCONV vColorAdjustSaturation(vfloat4 vColor, float fSaturation)
    {
        // Luminance = 0.2125f * C[0] + 0.7154f * C[1] + 0.0721f * C[2];
        // Result = (C - Luminance) * Saturation + Luminance;

        const Internal::vfloat4f32 gvLuminance = { { { 0.2125f, 0.7154f, 0.0721f, 0.0f } } };
#if defined(VMATH_NO_INTRINSICS)
        float fLuminance = (vColor.v4f[0] * gvLuminance.f[0]) + (vColor.v4f[1] * gvLuminance.f[1]) + (vColor.v4f[2] * gvLuminance.f[2]);
        vfloat4 vResult;
        vResult.v4f[0] = ((vColor.v4f[0] - fLuminance) * fSaturation) + fLuminance;
        vResult.v4f[1] = ((vColor.v4f[1] - fLuminance) * fSaturation) + fLuminance;
        vResult.v4f[2] = ((vColor.v4f[2] - fLuminance) * fSaturation) + fLuminance;
        vResult.v4f[3] = vColor.v4f[3];
        return vResult;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 vLuminance = v3Dot(vColor, gvLuminance);
        vfloat4 vResult = vsubq_f32(vColor, vLuminance);
        vResult = vmlaq_n_f32(vLuminance, vResult, fSaturation);
        return vbslq_f32(Internal::kSelect1110, vResult, vColor);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vLuminance = v3Dot(vColor, gvLuminance);
        // Splat fSaturation
        vfloat4 vSaturation = _mm_set_ps1(fSaturation);
        // vResult = ((vColor-vLuminance)*vSaturation)+vLuminance;
        vfloat4 vResult = _mm_sub_ps(vColor, vLuminance);
        vResult = VMATH_FMADD_PS(vResult, vSaturation, vLuminance);
        // Retain w from the source color
        vLuminance = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));   // x = vResult.z,y = vResult.z,z = vColor.z,w=vColor.w
        vResult = _mm_shuffle_ps(vResult, vLuminance, _MM_SHUFFLE(3, 0, 1, 0));  // x = vResult.x,y = vResult.y,z = vResult.z,w=vColor.w
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorAdjustContrast(FXMVECTOR vColor, float fContrast)
    inline vfloat4 VMATH_CALLCONV vColorAdjustContrast(vfloat4 vColor, float fContrast)
    {
        // Result = (vColor - 0.5f) * fContrast + 0.5f;

#if defined(VMATH_NO_INTRINSICS)
        Internal::vfloat4f32 vResult = { { {
                ((vColor.v4f[0] - 0.5f) * fContrast) + 0.5f,
                ((vColor.v4f[1] - 0.5f) * fContrast) + 0.5f,
                ((vColor.v4f[2] - 0.5f) * fContrast) + 0.5f,
                vColor.v4f[3]        // Leave W untouched
            } } };
        return vResult.v;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        vfloat4 vResult = vsubq_f32(vColor, Internal::kOneHalf.v);
        vResult = vmlaq_n_f32(Internal::kOneHalf.v, vResult, fContrast);
        return vbslq_f32(Internal::kSelect1110, vResult, vColor);
#elif defined(VMATH_SSE_INTRINSICS)
        vfloat4 vScale = _mm_set_ps1(fContrast);           // Splat the scale
        vfloat4 vResult = _mm_sub_ps(vColor, Internal::kOneHalf);  // Subtract 0.5f from the source (Saving source)
        vResult = VMATH_FMADD_PS(vResult, vScale, Internal::kOneHalf);
        // Retain w from the source color
        vScale = _mm_shuffle_ps(vResult, vColor, _MM_SHUFFLE(3, 2, 2, 2));   // x = vResult.z,y = vResult.z,z = vColor.z,w=vColor.w
        vResult = _mm_shuffle_ps(vResult, vScale, _MM_SHUFFLE(3, 0, 1, 0));  // x = vResult.x,y = vResult.y,z = vResult.z,w=vColor.w
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToHSL(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToHSL(vfloat4 rgb)
    {
        vfloat4 r = v4xxxx(rgb);
        vfloat4 g = v4yyyy(rgb);
        vfloat4 b = v4zzzz(rgb);

        vfloat4 min = v4Min(r, v4Min(g, b));
        vfloat4 max = v4Max(r, v4Max(g, b));

        vfloat4 l = v4Mul(v4Add(min, max), Internal::kOneHalf);

        vfloat4 d = v4Sub(max, min);

        vfloat4 la = v4Select(rgb, l, Internal::kSelect1110);

        if (v3AllLess(d, Internal::kEpsilon))
        {
            // Achromatic, assume H and S of 0
            return v4Select(la, Internal::kZero, Internal::kSelect1100);
        }
        else
        {
            vfloat4 s, h;

            vfloat4 d2 = v4Add(min, max);

            if (v3AllGreater(l, Internal::kOneHalf))
            {
                // d / (2-max-min)
                s = v4Div(d, v4Sub(Internal::kTwo, d2));
            }
            else
            {
                // d / (max+min)
                s = v4Div(d, d2);
            }

            if (v3AllEqual(r, max))
            {
                // Red is max
                h = v4Div(v4Sub(g, b), d);
            }
            else if (v3AllEqual(g, max))
            {
                // Green is max
                h = v4Div(v4Sub(b, r), d);
                h = v4Add(h, Internal::kTwo);
            }
            else
            {
                // Blue is max
                h = v4Div(v4Sub(r, g), d);
                h = v4Add(h, Internal::kFour);
            }

            h = v4Div(h, Internal::kSix);

            if (v3AllLess(h, Internal::kZero))
                h = v4Add(h, Internal::kOne);

            vfloat4 lha = v4Select(la, h, Internal::kSelect1100);
            return v4Select(s, lha, Internal::kSelect1011);
        }
    }

    //------------------------------------------------------------------------------

    namespace Internal
    {

        // XMVECTOR XMColorHue2Clr(FXMVECTOR p, FXMVECTOR q, FXMVECTOR h)
        inline vfloat4 VMATH_CALLCONV vColorHue2Clr(vfloat4 p, vfloat4 q, vfloat4 h)
        {
            static const Internal::vfloat4f32 oneSixth = { { { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f } } };
            static const Internal::vfloat4f32 twoThirds = { { { 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f } } };

            vfloat4 t = h;

            if (v3AllLess(t, Internal::kZero))
                t = v4Add(t, Internal::kOne);

            if (v3AllGreater(t, Internal::kOne))
                t = v4Sub(t, Internal::kOne);

            if (v3AllLess(t, oneSixth))
            {
                // p + (q - p) * 6 * t
                vfloat4 t1 = v4Sub(q, p);
                vfloat4 t2 = v4Mul(Internal::kSix, t);
                return v4MulAdd(t1, t2, p);
            }

            if (v3AllLess(t, Internal::kOneHalf))
                return q;

            if (v3AllLess(t, twoThirds))
            {
                // p + (q - p) * 6 * (2/3 - t)
                vfloat4 t1 = v4Sub(q, p);
                vfloat4 t2 = v4Mul(Internal::kSix, v4Sub(twoThirds, t));
                return v4MulAdd(t1, t2, p);
            }

            return p;
        }

    } // namespace Internal

    // XMVECTOR XMColorHSLToRGB(FXMVECTOR hsl)
    inline vfloat4 VMATH_CALLCONV vColorHSLToRGB(vfloat4 hsl)
    {
        static const Internal::vfloat4f32 oneThird = { { { 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f } } };

        vfloat4 s = v4yyyy(hsl);
        vfloat4 l = v4zzzz(hsl);

        if (v3AllNearEqual(s, Internal::kZero, Internal::kEpsilon))
        {
            // Achromatic
            return v4Select(hsl, l, Internal::kSelect1110);
        }
        else
        {
            vfloat4 h = v4xxxx(hsl);

            vfloat4 q;
            if (v3AllLess(l, Internal::kOneHalf))
            {
                q = v4Mul(l, v4Add(Internal::kOne, s));
            }
            else
            {
                q = v4Sub(v4Add(l, s), v4Mul(l, s));
            }

            vfloat4 p = v4Sub(v4Mul(Internal::kTwo, l), q);

            vfloat4 r = Internal::vColorHue2Clr(p, q, v4Add(h, oneThird));
            vfloat4 g = Internal::vColorHue2Clr(p, q, h);
            vfloat4 b = Internal::vColorHue2Clr(p, q, v4Sub(h, oneThird));

            vfloat4 rg = v4Select(g, r, Internal::kSelect1000);
            vfloat4 ba = v4Select(hsl, b, Internal::kSelect1110);

            return v4Select(ba, rg, Internal::kSelect1100);
        }
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToHSV(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToHSV(vfloat4 rgb)
    {
        vfloat4 r = v4xxxx(rgb);
        vfloat4 g = v4yyyy(rgb);
        vfloat4 b = v4zzzz(rgb);

        vfloat4 min = v4Min(r, v4Min(g, b));
        vfloat4 v = v4Max(r, v4Max(g, b));

        vfloat4 d = v4Sub(v, min);

        vfloat4 s = (v3AllNearEqual(v, Internal::kZero, Internal::kEpsilon)) ? Internal::kZero : v4Div(d, v);

        if (v3AllLess(d, Internal::kEpsilon))
        {
            // Achromatic, assume H of 0
            vfloat4 hv = v4Select(v, Internal::kZero, Internal::kSelect1000);
            vfloat4 hva = v4Select(rgb, hv, Internal::kSelect1110);
            return v4Select(s, hva, Internal::kSelect1011);
        }
        else
        {
            vfloat4 h;

            if (v3AllEqual(r, v))
            {
                // Red is max
                h = v4Div(v4Sub(g, b), d);

                if (v3AllLess(g, b))
                    h = v4Add(h, Internal::kSix);
            }
            else if (v3AllEqual(g, v))
            {
                // Green is max
                h = v4Div(v4Sub(b, r), d);
                h = v4Add(h, Internal::kTwo);
            }
            else
            {
                // Blue is max
                h = v4Div(v4Sub(r, g), d);
                h = v4Add(h, Internal::kFour);
            }

            h = v4Div(h, Internal::kSix);

            vfloat4 hv = v4Select(v, h, Internal::kSelect1000);
            vfloat4 hva = v4Select(rgb, hv, Internal::kSelect1110);
            return v4Select(s, hva, Internal::kSelect1011);
        }
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorHSVToRGB(FXMVECTOR hsv)
    inline vfloat4 VMATH_CALLCONV vColorHSVToRGB(vfloat4 hsv)
    {
        vfloat4 h = v4xxxx(hsv);
        vfloat4 s = v4yyyy(hsv);
        vfloat4 v = v4zzzz(hsv);

        vfloat4 h6 = v4Mul(h, Internal::kSix);

        vfloat4 i = v4Floor(h6);
        vfloat4 f = v4Sub(h6, i);

        // p = v* (1-s)
        vfloat4 p = v4Mul(v, v4Sub(Internal::kOne, s));

        // q = v*(1-f*s)
        vfloat4 q = v4Mul(v, v4Sub(Internal::kOne, v4Mul(f, s)));

        // t = v*(1 - (1-f)*s)
        vfloat4 t = v4Mul(v, v4Sub(Internal::kOne, v4Mul(v4Sub(Internal::kOne, f), s)));

        auto ii = int32_t(v4GetX(v4Mod(i, Internal::kSix)));

        vfloat4 _rgb;

        switch (ii)
        {
        case 0: // rgb = vtp
        {
            vfloat4 vt = v4Select(t, v, Internal::kSelect1000);
            _rgb = v4Select(p, vt, Internal::kSelect1100);
        }
        break;
        case 1: // rgb = qvp
        {
            vfloat4 qv = v4Select(v, q, Internal::kSelect1000);
            _rgb = v4Select(p, qv, Internal::kSelect1100);
        }
        break;
        case 2: // rgb = pvt
        {
            vfloat4 pv = v4Select(v, p, Internal::kSelect1000);
            _rgb = v4Select(t, pv, Internal::kSelect1100);
        }
        break;
        case 3: // rgb = pqv
        {
            vfloat4 pq = v4Select(q, p, Internal::kSelect1000);
            _rgb = v4Select(v, pq, Internal::kSelect1100);
        }
        break;
        case 4: // rgb = tpv
        {
            vfloat4 tp = v4Select(p, t, Internal::kSelect1000);
            _rgb = v4Select(v, tp, Internal::kSelect1100);
        }
        break;
        default: // rgb = vpq
        {
            vfloat4 vp = v4Select(p, v, Internal::kSelect1000);
            _rgb = v4Select(q, vp, Internal::kSelect1100);
        }
        break;
        }

        return v4Select(hsv, _rgb, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToYUV(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToYUV(vfloat4 rgb)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 0.299f, -0.147f, 0.615f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { 0.587f, -0.289f, -0.515f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 0.114f, 0.436f, -0.100f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(rgb, M);

        return v4Select(rgb, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorYUVToRGB(FXMVECTOR yuv)
    inline vfloat4 VMATH_CALLCONV vColorYUVToRGB(vfloat4 yuv)
    {
        static const Internal::vfloat4f32 Scale1 = { { { 0.0f, -0.395f, 2.032f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 1.140f, -0.581f, 0.0f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Internal::kOne, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(yuv, M);

        return v4Select(yuv, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToYUV_HD(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToYUV_HD(vfloat4 rgb)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 0.2126f, -0.0997f, 0.6150f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { 0.7152f, -0.3354f, -0.5586f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 0.0722f, 0.4351f, -0.0564f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(rgb, M);

        return v4Select(rgb, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorYUVToRGB_HD(FXMVECTOR yuv)
    inline vfloat4 VMATH_CALLCONV vColorYUVToRGB_HD(vfloat4 yuv)
    {
        static const Internal::vfloat4f32 Scale1 = { { { 0.0f, -0.2153f, 2.1324f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 1.2803f, -0.3806f, 0.0f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Internal::kOne, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(yuv, M);

        return v4Select(yuv, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToYUV_UHD(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToYUV_UHD(vfloat4 rgb)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 0.2627f, -0.1215f,  0.6150f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { 0.6780f, -0.3136f, -0.5655f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 0.0593f,  0.4351f, -0.0495f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(rgb, M);

        return v4Select(rgb, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorYUVToRGB_UHD(FXMVECTOR yuv)
    inline vfloat4 VMATH_CALLCONV vColorYUVToRGB_UHD(vfloat4 yuv)
    {
        static const Internal::vfloat4f32 Scale1 = { { {    0.0f, -0.1891f, 2.1620f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 1.1989f, -0.4645f,    0.0f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Internal::kOne, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(yuv, M);

        return v4Select(yuv, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToXYZ(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToXYZ(vfloat4 rgb)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 0.4887180f, 0.1762044f, 0.0000000f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { 0.3106803f, 0.8129847f, 0.0102048f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 0.2006017f, 0.0108109f, 0.9897952f, 0.0f } } };
        static const Internal::vfloat4f32 Scale = { { { 1.f / 0.17697f, 1.f / 0.17697f, 1.f / 0.17697f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v4Mul(v3TransformPoint(rgb, M), Scale);

        return v4Select(rgb, clr, Internal::kSelect1110);
    }

    // XMVECTOR XMColorXYZToRGB(FXMVECTOR xyz)
    inline vfloat4 VMATH_CALLCONV vColorXYZToRGB(vfloat4 xyz)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 2.3706743f, -0.5138850f, 0.0052982f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { -0.9000405f, 1.4253036f, -0.0146949f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { -0.4706338f, 0.0885814f, 1.0093968f, 0.0f } } };
        static const Internal::vfloat4f32 Scale = { { { 0.17697f, 0.17697f, 0.17697f, 0.0f } } };

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(v4Mul(xyz, Scale), M);

        return v4Select(xyz, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorXYZToSRGB(FXMVECTOR xyz)
    inline vfloat4 VMATH_CALLCONV vColorXYZToSRGB(vfloat4 xyz)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 3.2406f, -0.9689f, 0.0557f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { -1.5372f, 1.8758f, -0.2040f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { -0.4986f, 0.0415f, 1.0570f, 0.0f } } };
        static const Internal::vfloat4f32 Cutoff = { { { 0.0031308f, 0.0031308f, 0.0031308f, 0.0f } } };
        static const Internal::vfloat4f32 Exp = { { { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.0f } } };

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 lclr = v3TransformPoint(xyz, M);

        vfloat4 sel = v4Greater(lclr, Cutoff);

        // clr = 12.92 * lclr for lclr <= 0.0031308f
        vfloat4 smallC = v4Mul(lclr, Internal::kSRGBScale);

        // clr = (1+a)*pow(lclr, 1/2.4) - a for lclr > 0.0031308 (where a = 0.055)
        vfloat4 largeC = v4Sub(v4Mul(Internal::kSRGBA1, v4Pow(lclr, Exp)), Internal::kSRGBA);

        vfloat4 clr = v4Select(smallC, largeC, sel);

        return v4Select(xyz, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorSRGBToXYZ(FXMVECTOR srgb)
    inline vfloat4 VMATH_CALLCONV vColorSRGBToXYZ(vfloat4 srgb)
    {
        static const Internal::vfloat4f32 Scale0 = { { { 0.4124f, 0.2126f, 0.0193f, 0.0f } } };
        static const Internal::vfloat4f32 Scale1 = { { { 0.3576f, 0.7152f, 0.1192f, 0.0f } } };
        static const Internal::vfloat4f32 Scale2 = { { { 0.1805f, 0.0722f, 0.9505f, 0.0f } } };
        static const Internal::vfloat4f32 Cutoff = { { { 0.04045f, 0.04045f, 0.04045f, 0.0f } } };
        static const Internal::vfloat4f32 Exp = { { { 2.4f, 2.4f, 2.4f, 1.0f } } };

        vfloat4 sel = v4Greater(srgb, Cutoff);

        // lclr = clr / 12.92
        vfloat4 smallC = v4Div(srgb, Internal::kSRGBScale);

        // lclr = pow( (clr + a) / (1+a), 2.4 )
        vfloat4 largeC = v4Pow(v4Div(v4Add(srgb, Internal::kSRGBA), Internal::kSRGBA1), Exp);

        vfloat4 lclr = v4Select(smallC, largeC, sel);

        vfloat4x4 M = vfloat4x4(Scale0, Scale1, Scale2, Internal::kZero);
        vfloat4 clr = v3TransformPoint(lclr, M);

        return v4Select(srgb, clr, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorRGBToSRGB(FXMVECTOR rgb)
    inline vfloat4 VMATH_CALLCONV vColorRGBToSRGB(vfloat4 rgb)
    {
        static const Internal::vfloat4f32 Cutoff = { { { 0.0031308f, 0.0031308f, 0.0031308f, 1.f } } };
        static const Internal::vfloat4f32 Linear = { { { 12.92f, 12.92f, 12.92f, 1.f } } };
        static const Internal::vfloat4f32 Scale = { { { 1.055f, 1.055f, 1.055f, 1.f } } };
        static const Internal::vfloat4f32 Bias = { { { 0.055f, 0.055f, 0.055f, 0.f } } };
        static const Internal::vfloat4f32 InvGamma = { { { 1.0f / 2.4f, 1.0f / 2.4f, 1.0f / 2.4f, 1.f } } };

        vfloat4 V = v4Clamp01(rgb);
        vfloat4 V0 = v4Mul(V, Linear);
        vfloat4 V1 = v4Sub(v4Mul(Scale, v4Pow(V, InvGamma)), Bias);
        vfloat4 select = v4Less(V, Cutoff);
        V = v4Select(V1, V0, select);
        return v4Select(rgb, V, Internal::kSelect1110);
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMColorSRGBToRGB(FXMVECTOR srgb)
    inline vfloat4 VMATH_CALLCONV vColorSRGBToRGB(vfloat4 srgb)
    {
        static const Internal::vfloat4f32 Cutoff = { { { 0.04045f, 0.04045f, 0.04045f, 1.f } } };
        static const Internal::vfloat4f32 ILinear = { { { 1.f / 12.92f, 1.f / 12.92f, 1.f / 12.92f, 1.f } } };
        static const Internal::vfloat4f32 Scale = { { { 1.f / 1.055f, 1.f / 1.055f, 1.f / 1.055f, 1.f } } };
        static const Internal::vfloat4f32 Bias = { { { 0.055f, 0.055f, 0.055f, 0.f } } };
        static const Internal::vfloat4f32 Gamma = { { { 2.4f, 2.4f, 2.4f, 1.f } } };

        vfloat4 V = v4Clamp01(srgb);
        vfloat4 V0 = v4Mul(V, ILinear);
        vfloat4 V1 = v4Pow(v4Mul(v4Add(V, Bias), Scale), Gamma);
        vfloat4 select = v4Greater(V, Cutoff);
        V = v4Select(V0, V1, select);
        return v4Select(srgb, V, Internal::kSelect1110);
    }

    /****************************************************************************
     *
     * vbool4
     *
     ****************************************************************************/

    inline vbool4::vbool4(uint32_t mask) : mask(mask)
    {
        assert(v4bIsValidCompareMask(mask) && "Unexpected vbool4 mask layout");
    }

    inline bool v4bIsValidCompareMask(uint32_t mask)
    {
#if defined(VMATH_NO_INTRINSICS)
        return (mask & ~0x01010101) == 0u;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint32_t x = mask & 0xFF000000u;
        uint32_t y = mask & 0x0000FF00u;
        uint32_t z = mask & 0x00FF0000u;
        uint32_t w = mask & 0x000000FFu;

        return ((x == 0xFF000000u) || (x == 0u))
            && ((y == 0x0000FF00u) || (y == 0u))
            && ((z == 0x00FF0000u) || (z == 0u))
            && ((w == 0x000000FFu) || (w == 0u));
#elif defined(VMATH_SSE_INTRINSICS)
        return (mask & ~0xFu) == 0u;
#endif
    }

    inline bool VMATH_CALLCONV v4IsValidCompareMask(vfloat4 vMask)
    {
        union U32Float
        {
            float f32;
            uint32_t u32;
        };

        constexpr uint32_t FalseMask = 0x0u;
        constexpr uint32_t TrueMask = 0xFFFFFFFFu;

        U32Float x;
        x.f32 = v4GetX(vMask);

        U32Float y;
        y.f32 = v4GetY(vMask);

        U32Float z;
        z.f32 = v4GetZ(vMask);

        U32Float w;
        w.f32 = v4GetW(vMask);

        return (x.u32 == FalseMask || x.u32 == TrueMask)
            && (y.u32 == FalseMask || y.u32 == TrueMask)
            && (z.u32 == FalseMask || z.u32 == TrueMask)
            && (w.u32 == FalseMask || w.u32 == TrueMask);
    }

    inline vbool4 VMATH_CALLCONV v4CompressMask128(vfloat4 v)
    {
        assert(v4IsValidCompareMask(v) && "Unexpected vfloat4 compare mask layout");

#if defined(VMATH_NO_INTRINSICS)

        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = uint8_t(v.v4u[0] == 0xFFFFFFFFu);
        u.u8[1] = uint8_t(v.v4u[1] == 0xFFFFFFFFu);
        u.u8[2] = uint8_t(v.v4u[2] == 0xFFFFFFFFu);
        u.u8[3] = uint8_t(v.v4u[3] == 0xFFFFFFFFu);
        return vbool4(u.u32);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        uint8x16_t vU8 = vreinterpretq_u8_u32(vreinterpretq_u32_f32(v));
        uint8x8x2_t vTemp = vzip_u8(vget_low_u8(vU8), vget_high_u8(vU8));
        uint16x4x2_t vTemp2 = vzip_u16(vreinterpret_u16_u8(vTemp.val[0]), vreinterpret_u16_u8(vTemp.val[1]));
        return vbool4(vget_lane_u32(vreinterpret_u32_u16(vTemp2.val[1]), 1));
#elif defined(VMATH_SSE_INTRINSICS)
        return vbool4(uint32_t(_mm_movemask_ps(v)));
#endif
    }

    inline vbool4 vLoadBool4(const bool* src)
    {
#if defined(VMATH_NO_INTRINSICS)
        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = uint8_t(src[0]);
        u.u8[1] = uint8_t(src[1]);
        u.u8[2] = uint8_t(src[2]);
        u.u8[3] = uint8_t(src[3]);

        return vbool4(u.u32);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = src[0] ? 0xFF : 0u;
        u.u8[2] = src[1] ? 0xFF : 0u;
        u.u8[1] = src[2] ? 0xFF : 0u;
        u.u8[3] = src[3] ? 0xFF : 0u;

        return vbool4(u.u32);
#elif defined(VMATH_SSE_INTRINSICS)
        uint32_t mask = 0;
        mask |= uint8_t(src[0]) << 0;
        mask |= uint8_t(src[1]) << 1;
        mask |= uint8_t(src[2]) << 2;
        mask |= uint8_t(src[3]) << 3;

        return vbool4(mask);
#endif
    }

    inline void vStoreBool4(bool* dst, vbool4 src)
    {
        assert(v4bIsValidCompareMask(src.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS)
        dst[0] = bool(src.mask & 0x00000001u);
        dst[1] = bool(src.mask & 0x00000100u);
        dst[2] = bool(src.mask & 0x00010000u);
        dst[3] = bool(src.mask & 0x01000000u);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        dst[0] = bool(src.mask & 0x000000FFu);
        dst[1] = bool(src.mask & 0x00FF0000u);
        dst[2] = bool(src.mask & 0x0000FF00u);
        dst[3] = bool(src.mask & 0xFF000000u);
#elif defined(VMATH_SSE_INTRINSICS)
        dst[0] = bool(src.mask & (1 << 0u));
        dst[1] = bool(src.mask & (1 << 1u));
        dst[2] = bool(src.mask & (1 << 2u));
        dst[3] = bool(src.mask & (1 << 3u));
#endif
    }

    inline vbool4 vLoadBool3(const bool* src)
    {
#if defined(VMATH_NO_INTRINSICS)
        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = uint8_t(src[0]);
        u.u8[1] = uint8_t(src[1]);
        u.u8[2] = uint8_t(src[2]);
        u.u8[3] = 0u;

        return vbool4(u.u32);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = src[0] ? 0xFF : 0u;
        u.u8[2] = src[1] ? 0xFF : 0u;
        u.u8[1] = src[2] ? 0xFF : 0u;
        u.u8[3] = 0u;

        return vbool4(u.u32);
#elif defined(VMATH_SSE_INTRINSICS)
        uint32_t mask = 0;
        mask |= uint32_t(src[0]) << 0;
        mask |= uint32_t(src[1]) << 1;
        mask |= uint32_t(src[2]) << 2;

        return vbool4(mask);
#endif
    }

    inline void vStoreBool3(bool* dst, vbool4 src)
    {
        assert(v4bIsValidCompareMask(src.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS)
        dst[0] = bool(src.mask & 0x00000001u);
        dst[1] = bool(src.mask & 0x00000100u);
        dst[2] = bool(src.mask & 0x00010000u);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        dst[0] = bool(src.mask & 0x000000FFu);
        dst[1] = bool(src.mask & 0x00FF0000u);
        dst[2] = bool(src.mask & 0x0000FF00u);
#elif defined(VMATH_SSE_INTRINSICS)
        dst[0] = bool(src.mask & (1 << 0u));
        dst[1] = bool(src.mask & (1 << 1u));
        dst[2] = bool(src.mask & (1 << 2u));
#endif
    }

    inline vbool4 vLoadBool2(const bool* src)
    {
#if defined(VMATH_NO_INTRINSICS)
        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = uint8_t(src[0]);
        u.u8[1] = uint8_t(src[1]);
        u.u8[2] = 0u;
        u.u8[3] = 0u;

        return vbool4(u.u32);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        union UnionUint32Uint8x4
        {
            uint32_t u32;
            uint8_t u8[4];
        };

        UnionUint32Uint8x4 u;
        u.u8[0] = src[0] ? 0xFF : 0u;
        u.u8[2] = src[1] ? 0xFF : 0u;
        u.u8[1] = 0u;
        u.u8[3] = 0u;

        return vbool4(u.u32);
#elif defined(VMATH_SSE_INTRINSICS)
        uint32_t mask = 0;
        mask |= uint32_t(src[0]) << 0;
        mask |= uint32_t(src[1]) << 1;

        return vbool4(mask);
#endif
    }

    inline void vStoreBool2(bool* dst, vbool4 src)
    {
        assert(v4bIsValidCompareMask(src.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS)
        dst[0] = bool(src.mask & 0x00000001u);
        dst[1] = bool(src.mask & 0x00000100u);
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        dst[0] = bool(src.mask & 0x000000FFu);
        dst[1] = bool(src.mask & 0x00FF0000u);
#elif defined(VMATH_SSE_INTRINSICS)
        dst[0] = bool(src.mask & (1 << 0u));
        dst[1] = bool(src.mask & (1 << 1u));
#endif
    }

    inline bool v4All(vbool4 v)
    {
        assert(v4bIsValidCompareMask(v.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS) 
        return v.mask == 0x01010101;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return v.mask == 0xFFFFFFFFu;
#elif defined(VMATH_SSE_INTRINSICS)
        return v.mask == 0xFu;
#endif
    }

    inline bool v4AllFalse(vbool4 v)
    {
        assert(v4bIsValidCompareMask(v.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS) || defined(VMATH_SSE_INTRINSICS)
        return v.mask == 0u;
#endif
    }

    inline bool v4Any(vbool4 v)
    {
        return !v4AllFalse(v);
    }

    inline bool v4AnyFalse(vbool4 v)
    {
        return !v4All(v);
    }

    inline bool v3All(vbool4 v)
    {
        assert(v4bIsValidCompareMask(v.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS) 
        return (v.mask & 0x00010101) == 0x00010101;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return (v.mask & 0x00FFFFFFu) == 0x00FFFFFFu;
#elif defined(VMATH_SSE_INTRINSICS)
        return (v.mask & 0x7u) == 0x7u;
#endif
    }

    inline bool v3AllFalse(vbool4 v)
    {
        assert(v4bIsValidCompareMask(v.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS) 
        return (v.mask & 0x00010101) == 0u;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return (v.mask & 0x00FFFFFFu) == 0u;
#elif defined(VMATH_SSE_INTRINSICS)
        return (v.mask & 0x7u) == 0u;
#endif
    }

    inline bool v3Any(vbool4 v)
    {
        return !v3AllFalse(v);
    }

    inline bool v3AnyFalse(vbool4 v)

    {
        return !v3All(v);
    }



    inline bool v2All(vbool4 v)
    {
        assert(v4bIsValidCompareMask(v.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS) 
        return (v.mask & 0x00000101) == 0x00000101;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return (v.mask & 0x00FF00FFu) == 0x00FF00FFu;
#elif defined(VMATH_SSE_INTRINSICS)
        return (v.mask & 0x3u) == 0x3u;
#endif
    }

    inline bool v2AllFalse(vbool4 v)
    {
        assert(v4bIsValidCompareMask(v.mask) && "Unexpected vbool4 mask layout");

#if defined(VMATH_NO_INTRINSICS) 
        return (v.mask & 0x00000101) == 0u;
#elif defined(VMATH_ARM_NEON_INTRINSICS)
        return (v.mask & 0x00FF00FFu) == 0u;
#elif defined(VMATH_SSE_INTRINSICS)
        return (v.mask & 0x3u) == 0u;
#endif
    }

    inline bool v2Any(vbool4 v)
    {
        return !v2AllFalse(v);
    }

    inline bool v2AnyFalse(vbool4 v)
    {
        return !v2All(v);
    }



    inline bool v4All(vfloat4 vMask)
    {
        return v4All(v4CompressMask128(vMask));
    }

    inline bool v4AllFalse(vfloat4 vMask)
    {
        return v4AllFalse(v4CompressMask128(vMask));
    }

    inline bool v4Any(vfloat4 vMask)
    {
        return v4Any(v4CompressMask128(vMask));
    }

    inline bool v4AnyFalse(vfloat4 vMask)
    {
        return v4AnyFalse(v4CompressMask128(vMask));
    }


    inline bool v3All(vfloat4 vMask)
    {
        return v3All(v4CompressMask128(vMask));
    }

    inline bool v3AllFalse(vfloat4 vMask)
    {
        return v3AllFalse(v4CompressMask128(vMask));
    }

    inline bool v3Any(vfloat4 vMask)
    {
        return v3Any(v4CompressMask128(vMask));
    }

    inline bool v3AnyFalse(vfloat4 vMask)
    {
        return v3AnyFalse(v4CompressMask128(vMask));
    }


    inline bool v2All(vfloat4 vMask)
    {
        return v2All(v4CompressMask128(vMask));
    }

    inline bool v2AllFalse(vfloat4 vMask)
    {
        return v2AllFalse(v4CompressMask128(vMask));
    }

    inline bool v2Any(vfloat4 vMask)
    {
        return v2Any(v4CompressMask128(vMask));
    }

    inline bool v2AnyFalse(vfloat4 vMask)
    {
        return v2AnyFalse(v4CompressMask128(vMask));
    }

    /****************************************************************************
     *
     * Miscellaneous
     *
     ****************************************************************************/

     //------------------------------------------------------------------------------

    // bool XMVerifyCPUSupport()
    inline bool VerifyVMathCPUSupport()
    {
#if defined(VMATH_SSE_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        int32_t CPUInfo[4] = { -1 };
#if defined(__clang__) || defined(__GNUC__)
        __cpuid(0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
        __cpuid(CPUInfo, 0);
#endif

#ifdef __AVX2__
        if (CPUInfo[0] < 7)
            return false;
#else
        if (CPUInfo[0] < 1)
            return false;
#endif

#if defined(__clang__) || defined(__GNUC__)
        __cpuid(1, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
        __cpuid(CPUInfo, 1);
#endif

#if defined(__AVX2__) || defined(VMATH_AVX2_INTRINSICS)
        // The compiler can emit FMA3 instructions even without explicit intrinsics use
        if ((CPUInfo[2] & 0x38081001) != 0x38081001)
            return false; // No F16C/AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(VMATH_FMA3_INTRINSICS) && defined(VMATH_F16C_INTRINSICS)
        if ((CPUInfo[2] & 0x38081001) != 0x38081001)
            return false; // No F16C/AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(VMATH_FMA3_INTRINSICS)
        if ((CPUInfo[2] & 0x18081001) != 0x18081001)
            return false; // No AVX/OSXSAVE/SSE4.1/FMA3/SSE3 support
#elif defined(VMATH_F16C_INTRINSICS)
        if ((CPUInfo[2] & 0x38080001) != 0x38080001)
            return false; // No F16C/AVX/OSXSAVE/SSE4.1/SSE3 support
#elif defined(__AVX__) || defined(VMATH_AVX_INTRINSICS)
        if ((CPUInfo[2] & 0x18080001) != 0x18080001)
            return false; // No AVX/OSXSAVE/SSE4.1/SSE3 support
#elif defined(VMATH_SSE4_INTRINSICS)
        if ((CPUInfo[2] & 0x80001) != 0x80001)
            return false; // No SSE3/SSE4.1 support
#elif defined(VMATH_SSE3_INTRINSICS)
        if (!(CPUInfo[2] & 0x1))
            return false; // No SSE3 support
#endif

    // The x64 processor model requires SSE2 support, but no harm in checking
        if ((CPUInfo[3] & 0x6000000) != 0x6000000)
            return false; // No SSE2/SSE support

#if defined(__AVX2__) || defined(VMATH_AVX2_INTRINSICS)
#if defined(__clang__) || defined(__GNUC__)
        __cpuid_count(7, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#else
        __cpuidex(CPUInfo, 7, 0);
#endif
        if (!(CPUInfo[1] & 0x20))
            return false; // No AVX2 support
#endif

        return true;
#elif defined(VMATH_ARM_NEON_INTRINSICS) && !defined(VMATH_NO_INTRINSICS)
        // ARM-NEON support is required for the Windows on ARM platform
        return true;
#else
        // No intrinsics path always supported
        return true;
#endif
    }

    //------------------------------------------------------------------------------

    // XMVECTOR XMFresnelTerm(FXMVECTOR CosIncidentAngle, FXMVECTOR RefractionIndex)
    inline vfloat4 VMATH_CALLCONV v4FresnelTerm(vfloat4 CosIncidentAngle, vfloat4 RefractionIndex)
    {
        assert(!v4AnyIsInfinite(CosIncidentAngle));

        // Result = 0.5f * (g - c)^2 / (g + c)^2 * ((c * (g + c) - 1)^2 / (c * (g - c) + 1)^2 + 1) where
        // c = CosIncidentAngle
        // g = sqrt(c^2 + RefractionIndex^2 - 1)

#if defined(VMATH_NO_INTRINSICS) || defined(VMATH_ARM_NEON_INTRINSICS)

        vfloat4 G = v4MulAdd(RefractionIndex, RefractionIndex, Internal::kNegativeOne.v);
        G = v4MulAdd(CosIncidentAngle, CosIncidentAngle, G);
        G = v4Abs(G);
        G = v4Sqrt(G);

        vfloat4 S = v4Add(G, CosIncidentAngle);
        vfloat4 D = v4Sub(G, CosIncidentAngle);

        vfloat4 V0 = v4Mul(D, D);
        vfloat4 V1 = v4Mul(S, S);
        V1 = v4Rcp(V1);
        V0 = v4Mul(Internal::kOneHalf.v, V0);
        V0 = v4Mul(V0, V1);

        vfloat4 V2 = v4MulAdd(CosIncidentAngle, S, Internal::kNegativeOne.v);
        vfloat4 V3 = v4MulAdd(CosIncidentAngle, D, Internal::kOne.v);
        V2 = v4Mul(V2, V2);
        V3 = v4Mul(V3, V3);
        V3 = v4Rcp(V3);
        V2 = v4MulAdd(V2, V3, Internal::kOne.v);

        vfloat4 Result = v4Mul(V0, V2);

        Result = v4Clamp01(Result);

        return Result;

#elif defined(VMATH_SSE_INTRINSICS)
    // G = sqrt(abs((RefractionIndex^2-1) + CosIncidentAngle^2))
        vfloat4 G = _mm_mul_ps(RefractionIndex, RefractionIndex);
        vfloat4 vTemp = _mm_mul_ps(CosIncidentAngle, CosIncidentAngle);
        G = _mm_sub_ps(G, Internal::kOne);
        vTemp = _mm_add_ps(vTemp, G);
        // max((0-vTemp),vTemp) == abs(vTemp)
        // The abs is needed to deal with refraction and cosine being zero
        G = _mm_setzero_ps();
        G = _mm_sub_ps(G, vTemp);
        G = _mm_max_ps(G, vTemp);
        // Last operation, the sqrt()
        G = _mm_sqrt_ps(G);

        // Calc G-C and G+C
        vfloat4 GAddC = _mm_add_ps(G, CosIncidentAngle);
        vfloat4 GSubC = _mm_sub_ps(G, CosIncidentAngle);
        // Perform the term (0.5f *(g - c)^2) / (g + c)^2
        vfloat4 vResult = _mm_mul_ps(GSubC, GSubC);
        vTemp = _mm_mul_ps(GAddC, GAddC);
        vResult = _mm_mul_ps(vResult, Internal::kOneHalf);
        vResult = _mm_div_ps(vResult, vTemp);
        // Perform the term ((c * (g + c) - 1)^2 / (c * (g - c) + 1)^2 + 1)
        GAddC = _mm_mul_ps(GAddC, CosIncidentAngle);
        GSubC = _mm_mul_ps(GSubC, CosIncidentAngle);
        GAddC = _mm_sub_ps(GAddC, Internal::kOne);
        GSubC = _mm_add_ps(GSubC, Internal::kOne);
        GAddC = _mm_mul_ps(GAddC, GAddC);
        GSubC = _mm_mul_ps(GSubC, GSubC);
        GAddC = _mm_div_ps(GAddC, GSubC);
        GAddC = _mm_add_ps(GAddC, Internal::kOne);
        // Multiply the two term parts
        vResult = _mm_mul_ps(vResult, GAddC);
        // Clamp to 0.0 - 1.0f
        vResult = _mm_max_ps(vResult, Internal::kZero);
        vResult = _mm_min_ps(vResult, Internal::kOne);
        return vResult;
#endif
    }

    //------------------------------------------------------------------------------

    // bool XMScalarNearEqual(float S1, float S2, float Epsilon)
    inline bool NearEqual(float S1, float S2, float Epsilon)
    {
        float Delta = S1 - S2;
        return (fabsf(Delta) <= Epsilon);
    }

    //------------------------------------------------------------------------------
    // Modulo the range of the given angle such that -Pi <= Angle < Pi
    // float XMScalarModAngle(float Angle)
    inline float ModAngle(float Angle)
    {
        // Note: The modulo is performed with unsigned math only to work
        // around a precision error on numbers that are close to PI

        // Normalize the range from 0.0f to (2 * Pi)
        Angle = Angle + Pi;
        // Perform the modulo, unsigned
        float fTemp = fabsf(Angle);
        fTemp = fTemp - ((2 * Pi) * float(int32_t(fTemp / (2 * Pi))));
        // Restore the number to the range of -Pi to Pi-epsilon
        fTemp = fTemp - Pi;
        // If the modulo'd value was negative, restore negation
        if (Angle < 0.0f)
        {
            fTemp = -fTemp;
        }
        return fTemp;
    }

    //------------------------------------------------------------------------------

    // float XMScalarSin(float Value)
    inline float Sin(float Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1 / (2 * Pi)) * Value;
        if (Value >= 0.0f)
        {
            quotient = float(int32_t(quotient + 0.5f));
        }
        else
        {
            quotient = float(int32_t(quotient - 0.5f));
        }
        float y = Value - (2 * Pi) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        if (y > (Pi / 2))
        {
            y = Pi - y;
        }
        else if (y < -(Pi / 2))
        {
            y = -Pi - y;
        }

        // 11-degree minimax approximation
        float y2 = y * y;
        return (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
    }

    //------------------------------------------------------------------------------

    // float XMScalarSinEst(float Value)
    inline float SinEst(float Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1 / (2 * Pi)) * Value;
        if (Value >= 0.0f)
        {
            quotient = float(int32_t(quotient + 0.5f));
        }
        else
        {
            quotient = float(int32_t(quotient - 0.5f));
        }
        float y = Value - (2 * Pi) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        if (y > (Pi / 2))
        {
            y = Pi - y;
        }
        else if (y < -(Pi / 2))
        {
            y = -Pi - y;
        }

        // 7-degree minimax approximation
        float y2 = y * y;
        return (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;
    }

    //------------------------------------------------------------------------------

    // float XMScalarCos(float Value)
    inline float Cos(float Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1 / (2 * Pi)) * Value;
        if (Value >= 0.0f)
        {
            quotient = float(int32_t(quotient + 0.5f));
        }
        else
        {
            quotient = float(int32_t(quotient - 0.5f));
        }
        float y = Value - (2 * Pi) * quotient;

        // Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
        float sign;
        if (y > (Pi / 2))
        {
            y = Pi - y;
            sign = -1.0f;
        }
        else if (y < -(Pi / 2))
        {
            y = -Pi - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        // 10-degree minimax approximation
        float y2 = y * y;
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        return sign * p;
    }

    //------------------------------------------------------------------------------

    // float XMScalarCosEst(float Value)
    inline float CosEst(float Value)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1 / (2 * Pi)) * Value;
        if (Value >= 0.0f)
        {
            quotient = float(int32_t(quotient + 0.5f));
        }
        else
        {
            quotient = float(int32_t(quotient - 0.5f));
        }
        float y = Value - (2 * Pi) * quotient;

        // Map y to [-pi/2,pi/2] with cos(y) = sign*cos(x).
        float sign;
        if (y > (Pi / 2))
        {
            y = Pi - y;
            sign = -1.0f;
        }
        else if (y < -(Pi / 2))
        {
            y = -Pi - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        // 6-degree minimax approximation
        float y2 = y * y;
        float p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
        return sign * p;
    }

    //------------------------------------------------------------------------------

    // void XMScalarSinCos(float* pSin, float* pCos, float Value)
    inline void SinCos(float* pSin, float* pCos, float Value)
    {
        assert(pSin);
        assert(pCos);

        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1 / (2 * Pi)) * Value;
        if (Value >= 0.0f)
        {
            quotient = float(int32_t(quotient + 0.5f));
        }
        else
        {
            quotient = float(int32_t(quotient - 0.5f));
        }
        float y = Value - (2 * Pi) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > (Pi / 2))
        {
            y = Pi - y;
            sign = -1.0f;
        }
        else if (y < -(Pi / 2))
        {
            y = -Pi - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 11-degree minimax approximation
        *pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

        // 10-degree minimax approximation
        float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
        *pCos = sign * p;
    }

    //------------------------------------------------------------------------------

    // void XMScalarSinCosEst(float* pSin, float* pCos, float Value)
    inline void SinCosEst(float* pSin, float* pCos, float Value)
    {
        assert(pSin);
        assert(pCos);

        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1 / (2 * Pi)) * Value;
        if (Value >= 0.0f)
        {
            quotient = float(int32_t(quotient + 0.5f));
        }
        else
        {
            quotient = float(int32_t(quotient - 0.5f));
        }
        float y = Value - (2 * Pi) * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;
        if (y > (Pi / 2))
        {
            y = Pi - y;
            sign = -1.0f;
        }
        else if (y < -(Pi / 2))
        {
            y = -Pi - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 7-degree minimax approximation
        *pSin = (((-0.00018524670f * y2 + 0.0083139502f) * y2 - 0.16665852f) * y2 + 1.0f) * y;

        // 6-degree minimax approximation
        float p = ((-0.0012712436f * y2 + 0.041493919f) * y2 - 0.49992746f) * y2 + 1.0f;
        *pCos = sign * p;
    }

    //------------------------------------------------------------------------------

    // float XMScalarASin(float Value)
    inline float ASin(float Value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (Value >= 0.0f);
        float x = fabsf(Value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
        result *= root;  // acos(|x|)

        // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
        return (nonnegative ? (Pi / 2) - result : result - (Pi / 2));
    }

    //------------------------------------------------------------------------------

    // float XMScalarASinEst(float Value)
    inline float ASinEst(float Value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (Value >= 0.0f);
        float x = fabsf(Value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 3-degree minimax approximation
        float result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
        result *= root;  // acos(|x|)

        // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
        return (nonnegative ? (Pi / 2) - result : result - (Pi / 2));
    }

    //------------------------------------------------------------------------------

    // float XMScalarACos(float Value)
    inline float ACos(float Value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (Value >= 0.0f);
        float x = fabsf(Value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 7-degree minimax approximation
        float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
        result *= root;

        // acos(x) = pi - acos(-x) when x < 0
        return (nonnegative ? result : Pi - result);
    }

    //------------------------------------------------------------------------------

    // float XMScalarACosEst(float Value)
    inline float ACosEst(float Value)
    {
        // Clamp input to [-1,1].
        bool nonnegative = (Value >= 0.0f);
        float x = fabsf(Value);
        float omx = 1.0f - x;
        if (omx < 0.0f)
        {
            omx = 0.0f;
        }
        float root = sqrtf(omx);

        // 3-degree minimax approximation
        float result = ((-0.0187293f * x + 0.0742610f) * x - 0.2121144f) * x + 1.5707288f;
        result *= root;

        // acos(x) = pi - acos(-x) when x < 0
        return (nonnegative ? result : Pi - result);
    }

} // namespace VMath
