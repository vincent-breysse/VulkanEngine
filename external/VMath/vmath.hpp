#pragma once

#include "vmath-simd.hpp"
#include <cstring>

namespace VMath
{
    struct float2;
    struct float3;
    struct float4;
    struct int2;
    struct int3;
    struct int4;
    struct uint2;
    struct uint3;
    struct uint4;
    struct bool2;
    struct bool3;
    struct bool4;
    struct Matrix4x4;
    struct Plane;
    struct Quaternion;

    struct float2
    {
        float2() = default;

        explicit float2(const float* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit float2(float x, float y) :
            x(x), y(y)
        {
        }

        float& operator[](uint32_t i)
        {
            assert(i < 2);
            return (&x)[i];
        }

        float operator[](uint32_t i) const
        {
            assert(i < 2);
            return (&x)[i];
        }

        float2 xx() const;
        float2 xy() const;
        float2 yx() const;
        float2 yy() const;
        float3 xxx() const;
        float3 xxy() const;
        float3 xyx() const;
        float3 xyy() const;
        float3 yxx() const;
        float3 yxy() const;
        float3 yyx() const;
        float3 yyy() const;
        float4 xxxx() const;
        float4 xxxy() const;
        float4 xxyx() const;
        float4 xxyy() const;
        float4 xyxx() const;
        float4 xyxy() const;
        float4 xyyx() const;
        float4 xyyy() const;
        float4 yxxx() const;
        float4 yxxy() const;
        float4 yxyx() const;
        float4 yxyy() const;
        float4 yyxx() const;
        float4 yyxy() const;
        float4 yyyx() const;
        float4 yyyy() const;

        float x;
        float y;
    };

    struct float3
    {
        float3() = default;

        explicit float3(const float* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit float3(float x, float y, float z) :
            x(x), y(y), z(z)
        {
        }

        constexpr explicit float3(float2 xy, float z) :
            x(xy.x), y(xy.y), z(z)
        {
        }

        constexpr explicit float3(float x, float2 yz) :
            x(x), y(yz.x), z(yz.y)
        {
        }

        float& operator[](uint32_t i)
        {
            assert(i < 3);
            return (&x)[i];
        }

        float operator[](uint32_t i) const
        {
            assert(i < 3);
            return (&x)[i];
        }

        float2 xx() const;
        float2 xy() const;
        float2 xz() const;
        float2 yx() const;
        float2 yy() const;
        float2 yz() const;
        float2 zx() const;
        float2 zy() const;
        float2 zz() const;
        float3 xxx() const;
        float3 xxy() const;
        float3 xxz() const;
        float3 xyx() const;
        float3 xyy() const;
        float3 xyz() const;
        float3 xzx() const;
        float3 xzy() const;
        float3 xzz() const;
        float3 yxx() const;
        float3 yxy() const;
        float3 yxz() const;
        float3 yyx() const;
        float3 yyy() const;
        float3 yyz() const;
        float3 yzx() const;
        float3 yzy() const;
        float3 yzz() const;
        float3 zxx() const;
        float3 zxy() const;
        float3 zxz() const;
        float3 zyx() const;
        float3 zyy() const;
        float3 zyz() const;
        float3 zzx() const;
        float3 zzy() const;
        float3 zzz() const;
        float4 xxxx() const;
        float4 xxxy() const;
        float4 xxxz() const;
        float4 xxyx() const;
        float4 xxyy() const;
        float4 xxyz() const;
        float4 xxzx() const;
        float4 xxzy() const;
        float4 xxzz() const;
        float4 xyxx() const;
        float4 xyxy() const;
        float4 xyxz() const;
        float4 xyyx() const;
        float4 xyyy() const;
        float4 xyyz() const;
        float4 xyzx() const;
        float4 xyzy() const;
        float4 xyzz() const;
        float4 xzxx() const;
        float4 xzxy() const;
        float4 xzxz() const;
        float4 xzyx() const;
        float4 xzyy() const;
        float4 xzyz() const;
        float4 xzzx() const;
        float4 xzzy() const;
        float4 xzzz() const;
        float4 yxxx() const;
        float4 yxxy() const;
        float4 yxxz() const;
        float4 yxyx() const;
        float4 yxyy() const;
        float4 yxyz() const;
        float4 yxzx() const;
        float4 yxzy() const;
        float4 yxzz() const;
        float4 yyxx() const;
        float4 yyxy() const;
        float4 yyxz() const;
        float4 yyyx() const;
        float4 yyyy() const;
        float4 yyyz() const;
        float4 yyzx() const;
        float4 yyzy() const;
        float4 yyzz() const;
        float4 yzxx() const;
        float4 yzxy() const;
        float4 yzxz() const;
        float4 yzyx() const;
        float4 yzyy() const;
        float4 yzyz() const;
        float4 yzzx() const;
        float4 yzzy() const;
        float4 yzzz() const;
        float4 zxxx() const;
        float4 zxxy() const;
        float4 zxxz() const;
        float4 zxyx() const;
        float4 zxyy() const;
        float4 zxyz() const;
        float4 zxzx() const;
        float4 zxzy() const;
        float4 zxzz() const;
        float4 zyxx() const;
        float4 zyxy() const;
        float4 zyxz() const;
        float4 zyyx() const;
        float4 zyyy() const;
        float4 zyyz() const;
        float4 zyzx() const;
        float4 zyzy() const;
        float4 zyzz() const;
        float4 zzxx() const;
        float4 zzxy() const;
        float4 zzxz() const;
        float4 zzyx() const;
        float4 zzyy() const;
        float4 zzyz() const;
        float4 zzzx() const;
        float4 zzzy() const;
        float4 zzzz() const;

        float x;
        float y;
        float z;
    };

    struct float4
    {
        float4() = default;

        explicit float4(const float* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit float4(float x, float y, float z, float w) :
            x(x), y(y), z(z), w(w)
        {
        }

        constexpr explicit float4(float2 xy, float z, float w) :
            x(xy.x), y(xy.y), z(z), w(w)
        {
        }

        constexpr explicit float4(float x, float2 yz, float w) :
            x(x), y(yz.x), z(yz.y), w(w)
        {
        }

        constexpr explicit float4(float x, float y, float2 zw) :
            x(x), y(y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit float4(float2 xy, float2 zw) :
            x(xy.x), y(xy.y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit float4(float3 xyz, float w) :
            x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {
        }

        constexpr explicit float4(float x, float3 yzw) :
            x(x), y(yzw.x), z(yzw.y), w(yzw.z)
        {
        }

        float& operator[](uint32_t i)
        {
            assert(i < 4);
            return (&x)[i];
        }

        float operator[](uint32_t i) const
        {
            assert(i < 4);
            return (&x)[i];
        }

        float2 xx() const;
        float2 xy() const;
        float2 xz() const;
        float2 xw() const;
        float2 yx() const;
        float2 yy() const;
        float2 yz() const;
        float2 yw() const;
        float2 zx() const;
        float2 zy() const;
        float2 zz() const;
        float2 zw() const;
        float2 wx() const;
        float2 wy() const;
        float2 wz() const;
        float2 ww() const;
        float3 xxx() const;
        float3 xxy() const;
        float3 xxz() const;
        float3 xxw() const;
        float3 xyx() const;
        float3 xyy() const;
        float3 xyz() const;
        float3 xyw() const;
        float3 xzx() const;
        float3 xzy() const;
        float3 xzz() const;
        float3 xzw() const;
        float3 xwx() const;
        float3 xwy() const;
        float3 xwz() const;
        float3 xww() const;
        float3 yxx() const;
        float3 yxy() const;
        float3 yxz() const;
        float3 yxw() const;
        float3 yyx() const;
        float3 yyy() const;
        float3 yyz() const;
        float3 yyw() const;
        float3 yzx() const;
        float3 yzy() const;
        float3 yzz() const;
        float3 yzw() const;
        float3 ywx() const;
        float3 ywy() const;
        float3 ywz() const;
        float3 yww() const;
        float3 zxx() const;
        float3 zxy() const;
        float3 zxz() const;
        float3 zxw() const;
        float3 zyx() const;
        float3 zyy() const;
        float3 zyz() const;
        float3 zyw() const;
        float3 zzx() const;
        float3 zzy() const;
        float3 zzz() const;
        float3 zzw() const;
        float3 zwx() const;
        float3 zwy() const;
        float3 zwz() const;
        float3 zww() const;
        float3 wxx() const;
        float3 wxy() const;
        float3 wxz() const;
        float3 wxw() const;
        float3 wyx() const;
        float3 wyy() const;
        float3 wyz() const;
        float3 wyw() const;
        float3 wzx() const;
        float3 wzy() const;
        float3 wzz() const;
        float3 wzw() const;
        float3 wwx() const;
        float3 wwy() const;
        float3 wwz() const;
        float3 www() const;
        float4 xxxx() const;
        float4 xxxy() const;
        float4 xxxz() const;
        float4 xxxw() const;
        float4 xxyx() const;
        float4 xxyy() const;
        float4 xxyz() const;
        float4 xxyw() const;
        float4 xxzx() const;
        float4 xxzy() const;
        float4 xxzz() const;
        float4 xxzw() const;
        float4 xxwx() const;
        float4 xxwy() const;
        float4 xxwz() const;
        float4 xxww() const;
        float4 xyxx() const;
        float4 xyxy() const;
        float4 xyxz() const;
        float4 xyxw() const;
        float4 xyyx() const;
        float4 xyyy() const;
        float4 xyyz() const;
        float4 xyyw() const;
        float4 xyzx() const;
        float4 xyzy() const;
        float4 xyzz() const;
        float4 xyzw() const;
        float4 xywx() const;
        float4 xywy() const;
        float4 xywz() const;
        float4 xyww() const;
        float4 xzxx() const;
        float4 xzxy() const;
        float4 xzxz() const;
        float4 xzxw() const;
        float4 xzyx() const;
        float4 xzyy() const;
        float4 xzyz() const;
        float4 xzyw() const;
        float4 xzzx() const;
        float4 xzzy() const;
        float4 xzzz() const;
        float4 xzzw() const;
        float4 xzwx() const;
        float4 xzwy() const;
        float4 xzwz() const;
        float4 xzww() const;
        float4 xwxx() const;
        float4 xwxy() const;
        float4 xwxz() const;
        float4 xwxw() const;
        float4 xwyx() const;
        float4 xwyy() const;
        float4 xwyz() const;
        float4 xwyw() const;
        float4 xwzx() const;
        float4 xwzy() const;
        float4 xwzz() const;
        float4 xwzw() const;
        float4 xwwx() const;
        float4 xwwy() const;
        float4 xwwz() const;
        float4 xwww() const;
        float4 yxxx() const;
        float4 yxxy() const;
        float4 yxxz() const;
        float4 yxxw() const;
        float4 yxyx() const;
        float4 yxyy() const;
        float4 yxyz() const;
        float4 yxyw() const;
        float4 yxzx() const;
        float4 yxzy() const;
        float4 yxzz() const;
        float4 yxzw() const;
        float4 yxwx() const;
        float4 yxwy() const;
        float4 yxwz() const;
        float4 yxww() const;
        float4 yyxx() const;
        float4 yyxy() const;
        float4 yyxz() const;
        float4 yyxw() const;
        float4 yyyx() const;
        float4 yyyy() const;
        float4 yyyz() const;
        float4 yyyw() const;
        float4 yyzx() const;
        float4 yyzy() const;
        float4 yyzz() const;
        float4 yyzw() const;
        float4 yywx() const;
        float4 yywy() const;
        float4 yywz() const;
        float4 yyww() const;
        float4 yzxx() const;
        float4 yzxy() const;
        float4 yzxz() const;
        float4 yzxw() const;
        float4 yzyx() const;
        float4 yzyy() const;
        float4 yzyz() const;
        float4 yzyw() const;
        float4 yzzx() const;
        float4 yzzy() const;
        float4 yzzz() const;
        float4 yzzw() const;
        float4 yzwx() const;
        float4 yzwy() const;
        float4 yzwz() const;
        float4 yzww() const;
        float4 ywxx() const;
        float4 ywxy() const;
        float4 ywxz() const;
        float4 ywxw() const;
        float4 ywyx() const;
        float4 ywyy() const;
        float4 ywyz() const;
        float4 ywyw() const;
        float4 ywzx() const;
        float4 ywzy() const;
        float4 ywzz() const;
        float4 ywzw() const;
        float4 ywwx() const;
        float4 ywwy() const;
        float4 ywwz() const;
        float4 ywww() const;
        float4 zxxx() const;
        float4 zxxy() const;
        float4 zxxz() const;
        float4 zxxw() const;
        float4 zxyx() const;
        float4 zxyy() const;
        float4 zxyz() const;
        float4 zxyw() const;
        float4 zxzx() const;
        float4 zxzy() const;
        float4 zxzz() const;
        float4 zxzw() const;
        float4 zxwx() const;
        float4 zxwy() const;
        float4 zxwz() const;
        float4 zxww() const;
        float4 zyxx() const;
        float4 zyxy() const;
        float4 zyxz() const;
        float4 zyxw() const;
        float4 zyyx() const;
        float4 zyyy() const;
        float4 zyyz() const;
        float4 zyyw() const;
        float4 zyzx() const;
        float4 zyzy() const;
        float4 zyzz() const;
        float4 zyzw() const;
        float4 zywx() const;
        float4 zywy() const;
        float4 zywz() const;
        float4 zyww() const;
        float4 zzxx() const;
        float4 zzxy() const;
        float4 zzxz() const;
        float4 zzxw() const;
        float4 zzyx() const;
        float4 zzyy() const;
        float4 zzyz() const;
        float4 zzyw() const;
        float4 zzzx() const;
        float4 zzzy() const;
        float4 zzzz() const;
        float4 zzzw() const;
        float4 zzwx() const;
        float4 zzwy() const;
        float4 zzwz() const;
        float4 zzww() const;
        float4 zwxx() const;
        float4 zwxy() const;
        float4 zwxz() const;
        float4 zwxw() const;
        float4 zwyx() const;
        float4 zwyy() const;
        float4 zwyz() const;
        float4 zwyw() const;
        float4 zwzx() const;
        float4 zwzy() const;
        float4 zwzz() const;
        float4 zwzw() const;
        float4 zwwx() const;
        float4 zwwy() const;
        float4 zwwz() const;
        float4 zwww() const;
        float4 wxxx() const;
        float4 wxxy() const;
        float4 wxxz() const;
        float4 wxxw() const;
        float4 wxyx() const;
        float4 wxyy() const;
        float4 wxyz() const;
        float4 wxyw() const;
        float4 wxzx() const;
        float4 wxzy() const;
        float4 wxzz() const;
        float4 wxzw() const;
        float4 wxwx() const;
        float4 wxwy() const;
        float4 wxwz() const;
        float4 wxww() const;
        float4 wyxx() const;
        float4 wyxy() const;
        float4 wyxz() const;
        float4 wyxw() const;
        float4 wyyx() const;
        float4 wyyy() const;
        float4 wyyz() const;
        float4 wyyw() const;
        float4 wyzx() const;
        float4 wyzy() const;
        float4 wyzz() const;
        float4 wyzw() const;
        float4 wywx() const;
        float4 wywy() const;
        float4 wywz() const;
        float4 wyww() const;
        float4 wzxx() const;
        float4 wzxy() const;
        float4 wzxz() const;
        float4 wzxw() const;
        float4 wzyx() const;
        float4 wzyy() const;
        float4 wzyz() const;
        float4 wzyw() const;
        float4 wzzx() const;
        float4 wzzy() const;
        float4 wzzz() const;
        float4 wzzw() const;
        float4 wzwx() const;
        float4 wzwy() const;
        float4 wzwz() const;
        float4 wzww() const;
        float4 wwxx() const;
        float4 wwxy() const;
        float4 wwxz() const;
        float4 wwxw() const;
        float4 wwyx() const;
        float4 wwyy() const;
        float4 wwyz() const;
        float4 wwyw() const;
        float4 wwzx() const;
        float4 wwzy() const;
        float4 wwzz() const;
        float4 wwzw() const;
        float4 wwwx() const;
        float4 wwwy() const;
        float4 wwwz() const;
        float4 wwww() const;

        float x;
        float y;
        float z;
        float w;
    };

    struct int2
    {
        int2() = default;

        explicit int2(const int32_t* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit int2(int32_t x, int32_t y) :
            x(x), y(y)
        {
        }

        int32_t& operator[](uint32_t i)
        {
            assert(i < 2);
            return (&x)[i];
        }

        int32_t operator[](uint32_t i) const
        {
            assert(i < 2);
            return (&x)[i];
        }

        int2 xx() const;
        int2 xy() const;
        int2 yx() const;
        int2 yy() const;
        int3 xxx() const;
        int3 xxy() const;
        int3 xyx() const;
        int3 xyy() const;
        int3 yxx() const;
        int3 yxy() const;
        int3 yyx() const;
        int3 yyy() const;
        int4 xxxx() const;
        int4 xxxy() const;
        int4 xxyx() const;
        int4 xxyy() const;
        int4 xyxx() const;
        int4 xyxy() const;
        int4 xyyx() const;
        int4 xyyy() const;
        int4 yxxx() const;
        int4 yxxy() const;
        int4 yxyx() const;
        int4 yxyy() const;
        int4 yyxx() const;
        int4 yyxy() const;
        int4 yyyx() const;
        int4 yyyy() const;

        int32_t x;
        int32_t y;
    };

    struct int3
    {
        int3() = default;

        explicit int3(const int32_t* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit  int3(int32_t x, int32_t y, int32_t z) :
            x(x), y(y), z(z)
        {
        }

        constexpr explicit  int3(int2 xy, int32_t z) :
            x(xy.x), y(xy.y), z(z)
        {
        }

        constexpr explicit  int3(int32_t x, int2 yz) :
            x(x), y(yz.x), z(yz.y)
        {
        }

        int32_t& operator[](uint32_t i)
        {
            assert(i < 3);
            return (&x)[i];
        }

        int32_t operator[](uint32_t i) const
        {
            assert(i < 3);
            return (&x)[i];
        }

        int2 xx() const;
        int2 xy() const;
        int2 xz() const;
        int2 yx() const;
        int2 yy() const;
        int2 yz() const;
        int2 zx() const;
        int2 zy() const;
        int2 zz() const;
        int3 xxx() const;
        int3 xxy() const;
        int3 xxz() const;
        int3 xyx() const;
        int3 xyy() const;
        int3 xyz() const;
        int3 xzx() const;
        int3 xzy() const;
        int3 xzz() const;
        int3 yxx() const;
        int3 yxy() const;
        int3 yxz() const;
        int3 yyx() const;
        int3 yyy() const;
        int3 yyz() const;
        int3 yzx() const;
        int3 yzy() const;
        int3 yzz() const;
        int3 zxx() const;
        int3 zxy() const;
        int3 zxz() const;
        int3 zyx() const;
        int3 zyy() const;
        int3 zyz() const;
        int3 zzx() const;
        int3 zzy() const;
        int3 zzz() const;
        int4 xxxx() const;
        int4 xxxy() const;
        int4 xxxz() const;
        int4 xxyx() const;
        int4 xxyy() const;
        int4 xxyz() const;
        int4 xxzx() const;
        int4 xxzy() const;
        int4 xxzz() const;
        int4 xyxx() const;
        int4 xyxy() const;
        int4 xyxz() const;
        int4 xyyx() const;
        int4 xyyy() const;
        int4 xyyz() const;
        int4 xyzx() const;
        int4 xyzy() const;
        int4 xyzz() const;
        int4 xzxx() const;
        int4 xzxy() const;
        int4 xzxz() const;
        int4 xzyx() const;
        int4 xzyy() const;
        int4 xzyz() const;
        int4 xzzx() const;
        int4 xzzy() const;
        int4 xzzz() const;
        int4 yxxx() const;
        int4 yxxy() const;
        int4 yxxz() const;
        int4 yxyx() const;
        int4 yxyy() const;
        int4 yxyz() const;
        int4 yxzx() const;
        int4 yxzy() const;
        int4 yxzz() const;
        int4 yyxx() const;
        int4 yyxy() const;
        int4 yyxz() const;
        int4 yyyx() const;
        int4 yyyy() const;
        int4 yyyz() const;
        int4 yyzx() const;
        int4 yyzy() const;
        int4 yyzz() const;
        int4 yzxx() const;
        int4 yzxy() const;
        int4 yzxz() const;
        int4 yzyx() const;
        int4 yzyy() const;
        int4 yzyz() const;
        int4 yzzx() const;
        int4 yzzy() const;
        int4 yzzz() const;
        int4 zxxx() const;
        int4 zxxy() const;
        int4 zxxz() const;
        int4 zxyx() const;
        int4 zxyy() const;
        int4 zxyz() const;
        int4 zxzx() const;
        int4 zxzy() const;
        int4 zxzz() const;
        int4 zyxx() const;
        int4 zyxy() const;
        int4 zyxz() const;
        int4 zyyx() const;
        int4 zyyy() const;
        int4 zyyz() const;
        int4 zyzx() const;
        int4 zyzy() const;
        int4 zyzz() const;
        int4 zzxx() const;
        int4 zzxy() const;
        int4 zzxz() const;
        int4 zzyx() const;
        int4 zzyy() const;
        int4 zzyz() const;
        int4 zzzx() const;
        int4 zzzy() const;
        int4 zzzz() const;

        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct int4
    {
        int4() = default;

        explicit int4(const int32_t* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit int4(int32_t x, int32_t y, int32_t z, int32_t w) :
            x(x), y(y), z(z), w(w)
        {
        }

        constexpr explicit int4(int2 xy, int32_t z, int32_t w) :
            x(xy.x), y(xy.y), z(z), w(w)
        {
        }

        constexpr explicit int4(int32_t x, int2 yz, int32_t w) :
            x(x), y(yz.x), z(yz.y), w(w)
        {
        }

        constexpr explicit int4(int32_t x, int32_t y, int2 zw) :
            x(x), y(y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit int4(int2 xy, int2 zw) :
            x(xy.x), y(xy.y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit int4(int3 xyz, int32_t w) :
            x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {
        }

        constexpr explicit int4(int32_t x, int3 yzw) :
            x(x), y(yzw.x), z(yzw.y), w(yzw.z)
        {
        }

        int32_t& operator[](uint32_t i)
        {
            assert(i < 4);
            return (&x)[i];
        }

        int32_t operator[](uint32_t i) const
        {
            assert(i < 4);
            return (&x)[i];
        }

        int2 xx() const;
        int2 xy() const;
        int2 xz() const;
        int2 xw() const;
        int2 yx() const;
        int2 yy() const;
        int2 yz() const;
        int2 yw() const;
        int2 zx() const;
        int2 zy() const;
        int2 zz() const;
        int2 zw() const;
        int2 wx() const;
        int2 wy() const;
        int2 wz() const;
        int2 ww() const;
        int3 xxx() const;
        int3 xxy() const;
        int3 xxz() const;
        int3 xxw() const;
        int3 xyx() const;
        int3 xyy() const;
        int3 xyz() const;
        int3 xyw() const;
        int3 xzx() const;
        int3 xzy() const;
        int3 xzz() const;
        int3 xzw() const;
        int3 xwx() const;
        int3 xwy() const;
        int3 xwz() const;
        int3 xww() const;
        int3 yxx() const;
        int3 yxy() const;
        int3 yxz() const;
        int3 yxw() const;
        int3 yyx() const;
        int3 yyy() const;
        int3 yyz() const;
        int3 yyw() const;
        int3 yzx() const;
        int3 yzy() const;
        int3 yzz() const;
        int3 yzw() const;
        int3 ywx() const;
        int3 ywy() const;
        int3 ywz() const;
        int3 yww() const;
        int3 zxx() const;
        int3 zxy() const;
        int3 zxz() const;
        int3 zxw() const;
        int3 zyx() const;
        int3 zyy() const;
        int3 zyz() const;
        int3 zyw() const;
        int3 zzx() const;
        int3 zzy() const;
        int3 zzz() const;
        int3 zzw() const;
        int3 zwx() const;
        int3 zwy() const;
        int3 zwz() const;
        int3 zww() const;
        int3 wxx() const;
        int3 wxy() const;
        int3 wxz() const;
        int3 wxw() const;
        int3 wyx() const;
        int3 wyy() const;
        int3 wyz() const;
        int3 wyw() const;
        int3 wzx() const;
        int3 wzy() const;
        int3 wzz() const;
        int3 wzw() const;
        int3 wwx() const;
        int3 wwy() const;
        int3 wwz() const;
        int3 www() const;
        int4 xxxx() const;
        int4 xxxy() const;
        int4 xxxz() const;
        int4 xxxw() const;
        int4 xxyx() const;
        int4 xxyy() const;
        int4 xxyz() const;
        int4 xxyw() const;
        int4 xxzx() const;
        int4 xxzy() const;
        int4 xxzz() const;
        int4 xxzw() const;
        int4 xxwx() const;
        int4 xxwy() const;
        int4 xxwz() const;
        int4 xxww() const;
        int4 xyxx() const;
        int4 xyxy() const;
        int4 xyxz() const;
        int4 xyxw() const;
        int4 xyyx() const;
        int4 xyyy() const;
        int4 xyyz() const;
        int4 xyyw() const;
        int4 xyzx() const;
        int4 xyzy() const;
        int4 xyzz() const;
        int4 xyzw() const;
        int4 xywx() const;
        int4 xywy() const;
        int4 xywz() const;
        int4 xyww() const;
        int4 xzxx() const;
        int4 xzxy() const;
        int4 xzxz() const;
        int4 xzxw() const;
        int4 xzyx() const;
        int4 xzyy() const;
        int4 xzyz() const;
        int4 xzyw() const;
        int4 xzzx() const;
        int4 xzzy() const;
        int4 xzzz() const;
        int4 xzzw() const;
        int4 xzwx() const;
        int4 xzwy() const;
        int4 xzwz() const;
        int4 xzww() const;
        int4 xwxx() const;
        int4 xwxy() const;
        int4 xwxz() const;
        int4 xwxw() const;
        int4 xwyx() const;
        int4 xwyy() const;
        int4 xwyz() const;
        int4 xwyw() const;
        int4 xwzx() const;
        int4 xwzy() const;
        int4 xwzz() const;
        int4 xwzw() const;
        int4 xwwx() const;
        int4 xwwy() const;
        int4 xwwz() const;
        int4 xwww() const;
        int4 yxxx() const;
        int4 yxxy() const;
        int4 yxxz() const;
        int4 yxxw() const;
        int4 yxyx() const;
        int4 yxyy() const;
        int4 yxyz() const;
        int4 yxyw() const;
        int4 yxzx() const;
        int4 yxzy() const;
        int4 yxzz() const;
        int4 yxzw() const;
        int4 yxwx() const;
        int4 yxwy() const;
        int4 yxwz() const;
        int4 yxww() const;
        int4 yyxx() const;
        int4 yyxy() const;
        int4 yyxz() const;
        int4 yyxw() const;
        int4 yyyx() const;
        int4 yyyy() const;
        int4 yyyz() const;
        int4 yyyw() const;
        int4 yyzx() const;
        int4 yyzy() const;
        int4 yyzz() const;
        int4 yyzw() const;
        int4 yywx() const;
        int4 yywy() const;
        int4 yywz() const;
        int4 yyww() const;
        int4 yzxx() const;
        int4 yzxy() const;
        int4 yzxz() const;
        int4 yzxw() const;
        int4 yzyx() const;
        int4 yzyy() const;
        int4 yzyz() const;
        int4 yzyw() const;
        int4 yzzx() const;
        int4 yzzy() const;
        int4 yzzz() const;
        int4 yzzw() const;
        int4 yzwx() const;
        int4 yzwy() const;
        int4 yzwz() const;
        int4 yzww() const;
        int4 ywxx() const;
        int4 ywxy() const;
        int4 ywxz() const;
        int4 ywxw() const;
        int4 ywyx() const;
        int4 ywyy() const;
        int4 ywyz() const;
        int4 ywyw() const;
        int4 ywzx() const;
        int4 ywzy() const;
        int4 ywzz() const;
        int4 ywzw() const;
        int4 ywwx() const;
        int4 ywwy() const;
        int4 ywwz() const;
        int4 ywww() const;
        int4 zxxx() const;
        int4 zxxy() const;
        int4 zxxz() const;
        int4 zxxw() const;
        int4 zxyx() const;
        int4 zxyy() const;
        int4 zxyz() const;
        int4 zxyw() const;
        int4 zxzx() const;
        int4 zxzy() const;
        int4 zxzz() const;
        int4 zxzw() const;
        int4 zxwx() const;
        int4 zxwy() const;
        int4 zxwz() const;
        int4 zxww() const;
        int4 zyxx() const;
        int4 zyxy() const;
        int4 zyxz() const;
        int4 zyxw() const;
        int4 zyyx() const;
        int4 zyyy() const;
        int4 zyyz() const;
        int4 zyyw() const;
        int4 zyzx() const;
        int4 zyzy() const;
        int4 zyzz() const;
        int4 zyzw() const;
        int4 zywx() const;
        int4 zywy() const;
        int4 zywz() const;
        int4 zyww() const;
        int4 zzxx() const;
        int4 zzxy() const;
        int4 zzxz() const;
        int4 zzxw() const;
        int4 zzyx() const;
        int4 zzyy() const;
        int4 zzyz() const;
        int4 zzyw() const;
        int4 zzzx() const;
        int4 zzzy() const;
        int4 zzzz() const;
        int4 zzzw() const;
        int4 zzwx() const;
        int4 zzwy() const;
        int4 zzwz() const;
        int4 zzww() const;
        int4 zwxx() const;
        int4 zwxy() const;
        int4 zwxz() const;
        int4 zwxw() const;
        int4 zwyx() const;
        int4 zwyy() const;
        int4 zwyz() const;
        int4 zwyw() const;
        int4 zwzx() const;
        int4 zwzy() const;
        int4 zwzz() const;
        int4 zwzw() const;
        int4 zwwx() const;
        int4 zwwy() const;
        int4 zwwz() const;
        int4 zwww() const;
        int4 wxxx() const;
        int4 wxxy() const;
        int4 wxxz() const;
        int4 wxxw() const;
        int4 wxyx() const;
        int4 wxyy() const;
        int4 wxyz() const;
        int4 wxyw() const;
        int4 wxzx() const;
        int4 wxzy() const;
        int4 wxzz() const;
        int4 wxzw() const;
        int4 wxwx() const;
        int4 wxwy() const;
        int4 wxwz() const;
        int4 wxww() const;
        int4 wyxx() const;
        int4 wyxy() const;
        int4 wyxz() const;
        int4 wyxw() const;
        int4 wyyx() const;
        int4 wyyy() const;
        int4 wyyz() const;
        int4 wyyw() const;
        int4 wyzx() const;
        int4 wyzy() const;
        int4 wyzz() const;
        int4 wyzw() const;
        int4 wywx() const;
        int4 wywy() const;
        int4 wywz() const;
        int4 wyww() const;
        int4 wzxx() const;
        int4 wzxy() const;
        int4 wzxz() const;
        int4 wzxw() const;
        int4 wzyx() const;
        int4 wzyy() const;
        int4 wzyz() const;
        int4 wzyw() const;
        int4 wzzx() const;
        int4 wzzy() const;
        int4 wzzz() const;
        int4 wzzw() const;
        int4 wzwx() const;
        int4 wzwy() const;
        int4 wzwz() const;
        int4 wzww() const;
        int4 wwxx() const;
        int4 wwxy() const;
        int4 wwxz() const;
        int4 wwxw() const;
        int4 wwyx() const;
        int4 wwyy() const;
        int4 wwyz() const;
        int4 wwyw() const;
        int4 wwzx() const;
        int4 wwzy() const;
        int4 wwzz() const;
        int4 wwzw() const;
        int4 wwwx() const;
        int4 wwwy() const;
        int4 wwwz() const;
        int4 wwww() const;

        int32_t x;
        int32_t y;
        int32_t z;
        int32_t w;
    };

    struct uint2
    {
        uint2() = default;

        explicit uint2(const uint32_t* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit uint2(uint32_t x, uint32_t y) :
            x(x), y(y)
        {
        }

        uint32_t& operator[](uint32_t i)
        {
            assert(i < 2);
            return (&x)[i];
        }

        uint32_t operator[](uint32_t i) const
        {
            assert(i < 2);
            return (&x)[i];
        }

        uint2 xx() const;
        uint2 xy() const;
        uint2 yx() const;
        uint2 yy() const;
        uint3 xxx() const;
        uint3 xxy() const;
        uint3 xyx() const;
        uint3 xyy() const;
        uint3 yxx() const;
        uint3 yxy() const;
        uint3 yyx() const;
        uint3 yyy() const;
        uint4 xxxx() const;
        uint4 xxxy() const;
        uint4 xxyx() const;
        uint4 xxyy() const;
        uint4 xyxx() const;
        uint4 xyxy() const;
        uint4 xyyx() const;
        uint4 xyyy() const;
        uint4 yxxx() const;
        uint4 yxxy() const;
        uint4 yxyx() const;
        uint4 yxyy() const;
        uint4 yyxx() const;
        uint4 yyxy() const;
        uint4 yyyx() const;
        uint4 yyyy() const;

        uint32_t x;
        uint32_t y;
    };

    struct uint3
    {
        uint3() = default;

        explicit uint3(const uint32_t* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit uint3(uint32_t x, uint32_t y, uint32_t z) :
            x(x), y(y), z(z)
        {
        }

        constexpr explicit uint3(uint2 xy, uint32_t z) :
            x(xy.x), y(xy.y), z(z)
        {
        }

        constexpr explicit uint3(uint32_t x, uint2 yz) :
            x(x), y(yz.x), z(yz.y)
        {
        }

        uint32_t& operator[](uint32_t i)
        {
            assert(i < 3);
            return (&x)[i];
        }

        uint32_t operator[](uint32_t i) const
        {
            assert(i < 3);
            return (&x)[i];
        }

        uint2 xx() const;
        uint2 xy() const;
        uint2 xz() const;
        uint2 yx() const;
        uint2 yy() const;
        uint2 yz() const;
        uint2 zx() const;
        uint2 zy() const;
        uint2 zz() const;
        uint3 xxx() const;
        uint3 xxy() const;
        uint3 xxz() const;
        uint3 xyx() const;
        uint3 xyy() const;
        uint3 xyz() const;
        uint3 xzx() const;
        uint3 xzy() const;
        uint3 xzz() const;
        uint3 yxx() const;
        uint3 yxy() const;
        uint3 yxz() const;
        uint3 yyx() const;
        uint3 yyy() const;
        uint3 yyz() const;
        uint3 yzx() const;
        uint3 yzy() const;
        uint3 yzz() const;
        uint3 zxx() const;
        uint3 zxy() const;
        uint3 zxz() const;
        uint3 zyx() const;
        uint3 zyy() const;
        uint3 zyz() const;
        uint3 zzx() const;
        uint3 zzy() const;
        uint3 zzz() const;
        uint4 xxxx() const;
        uint4 xxxy() const;
        uint4 xxxz() const;
        uint4 xxyx() const;
        uint4 xxyy() const;
        uint4 xxyz() const;
        uint4 xxzx() const;
        uint4 xxzy() const;
        uint4 xxzz() const;
        uint4 xyxx() const;
        uint4 xyxy() const;
        uint4 xyxz() const;
        uint4 xyyx() const;
        uint4 xyyy() const;
        uint4 xyyz() const;
        uint4 xyzx() const;
        uint4 xyzy() const;
        uint4 xyzz() const;
        uint4 xzxx() const;
        uint4 xzxy() const;
        uint4 xzxz() const;
        uint4 xzyx() const;
        uint4 xzyy() const;
        uint4 xzyz() const;
        uint4 xzzx() const;
        uint4 xzzy() const;
        uint4 xzzz() const;
        uint4 yxxx() const;
        uint4 yxxy() const;
        uint4 yxxz() const;
        uint4 yxyx() const;
        uint4 yxyy() const;
        uint4 yxyz() const;
        uint4 yxzx() const;
        uint4 yxzy() const;
        uint4 yxzz() const;
        uint4 yyxx() const;
        uint4 yyxy() const;
        uint4 yyxz() const;
        uint4 yyyx() const;
        uint4 yyyy() const;
        uint4 yyyz() const;
        uint4 yyzx() const;
        uint4 yyzy() const;
        uint4 yyzz() const;
        uint4 yzxx() const;
        uint4 yzxy() const;
        uint4 yzxz() const;
        uint4 yzyx() const;
        uint4 yzyy() const;
        uint4 yzyz() const;
        uint4 yzzx() const;
        uint4 yzzy() const;
        uint4 yzzz() const;
        uint4 zxxx() const;
        uint4 zxxy() const;
        uint4 zxxz() const;
        uint4 zxyx() const;
        uint4 zxyy() const;
        uint4 zxyz() const;
        uint4 zxzx() const;
        uint4 zxzy() const;
        uint4 zxzz() const;
        uint4 zyxx() const;
        uint4 zyxy() const;
        uint4 zyxz() const;
        uint4 zyyx() const;
        uint4 zyyy() const;
        uint4 zyyz() const;
        uint4 zyzx() const;
        uint4 zyzy() const;
        uint4 zyzz() const;
        uint4 zzxx() const;
        uint4 zzxy() const;
        uint4 zzxz() const;
        uint4 zzyx() const;
        uint4 zzyy() const;
        uint4 zzyz() const;
        uint4 zzzx() const;
        uint4 zzzy() const;
        uint4 zzzz() const;

        uint32_t x;
        uint32_t y;
        uint32_t z;
    };

    struct uint4
    {
        uint4() = default;

        explicit uint4(const uint32_t* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit uint4(uint32_t x, uint32_t y, uint32_t z, uint32_t w) :
            x(x), y(y), z(z), w(w)
        {
        }

        constexpr explicit uint4(uint2 xy, uint32_t z, uint32_t w) :
            x(xy.x), y(xy.y), z(z), w(w)
        {
        }

        constexpr explicit uint4(uint32_t x, uint2 yz, uint32_t w) :
            x(x), y(yz.x), z(yz.y), w(w)
        {
        }

        constexpr explicit uint4(uint32_t x, uint32_t y, uint2 zw) :
            x(x), y(y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit uint4(uint2 xy, uint2 zw) :
            x(xy.x), y(xy.y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit uint4(uint3 xyz, uint32_t w) :
            x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {
        }

        constexpr explicit uint4(uint32_t x, uint3 yzw) :
            x(x), y(yzw.x), z(yzw.y), w(yzw.z)
        {
        }

        uint32_t& operator[](uint32_t i)
        {
            assert(i < 4);
            return (&x)[i];
        }

        uint32_t operator[](uint32_t i) const
        {
            assert(i < 4);
            return (&x)[i];
        }

        uint2 xx() const;
        uint2 xy() const;
        uint2 xz() const;
        uint2 xw() const;
        uint2 yx() const;
        uint2 yy() const;
        uint2 yz() const;
        uint2 yw() const;
        uint2 zx() const;
        uint2 zy() const;
        uint2 zz() const;
        uint2 zw() const;
        uint2 wx() const;
        uint2 wy() const;
        uint2 wz() const;
        uint2 ww() const;
        uint3 xxx() const;
        uint3 xxy() const;
        uint3 xxz() const;
        uint3 xxw() const;
        uint3 xyx() const;
        uint3 xyy() const;
        uint3 xyz() const;
        uint3 xyw() const;
        uint3 xzx() const;
        uint3 xzy() const;
        uint3 xzz() const;
        uint3 xzw() const;
        uint3 xwx() const;
        uint3 xwy() const;
        uint3 xwz() const;
        uint3 xww() const;
        uint3 yxx() const;
        uint3 yxy() const;
        uint3 yxz() const;
        uint3 yxw() const;
        uint3 yyx() const;
        uint3 yyy() const;
        uint3 yyz() const;
        uint3 yyw() const;
        uint3 yzx() const;
        uint3 yzy() const;
        uint3 yzz() const;
        uint3 yzw() const;
        uint3 ywx() const;
        uint3 ywy() const;
        uint3 ywz() const;
        uint3 yww() const;
        uint3 zxx() const;
        uint3 zxy() const;
        uint3 zxz() const;
        uint3 zxw() const;
        uint3 zyx() const;
        uint3 zyy() const;
        uint3 zyz() const;
        uint3 zyw() const;
        uint3 zzx() const;
        uint3 zzy() const;
        uint3 zzz() const;
        uint3 zzw() const;
        uint3 zwx() const;
        uint3 zwy() const;
        uint3 zwz() const;
        uint3 zww() const;
        uint3 wxx() const;
        uint3 wxy() const;
        uint3 wxz() const;
        uint3 wxw() const;
        uint3 wyx() const;
        uint3 wyy() const;
        uint3 wyz() const;
        uint3 wyw() const;
        uint3 wzx() const;
        uint3 wzy() const;
        uint3 wzz() const;
        uint3 wzw() const;
        uint3 wwx() const;
        uint3 wwy() const;
        uint3 wwz() const;
        uint3 www() const;
        uint4 xxxx() const;
        uint4 xxxy() const;
        uint4 xxxz() const;
        uint4 xxxw() const;
        uint4 xxyx() const;
        uint4 xxyy() const;
        uint4 xxyz() const;
        uint4 xxyw() const;
        uint4 xxzx() const;
        uint4 xxzy() const;
        uint4 xxzz() const;
        uint4 xxzw() const;
        uint4 xxwx() const;
        uint4 xxwy() const;
        uint4 xxwz() const;
        uint4 xxww() const;
        uint4 xyxx() const;
        uint4 xyxy() const;
        uint4 xyxz() const;
        uint4 xyxw() const;
        uint4 xyyx() const;
        uint4 xyyy() const;
        uint4 xyyz() const;
        uint4 xyyw() const;
        uint4 xyzx() const;
        uint4 xyzy() const;
        uint4 xyzz() const;
        uint4 xyzw() const;
        uint4 xywx() const;
        uint4 xywy() const;
        uint4 xywz() const;
        uint4 xyww() const;
        uint4 xzxx() const;
        uint4 xzxy() const;
        uint4 xzxz() const;
        uint4 xzxw() const;
        uint4 xzyx() const;
        uint4 xzyy() const;
        uint4 xzyz() const;
        uint4 xzyw() const;
        uint4 xzzx() const;
        uint4 xzzy() const;
        uint4 xzzz() const;
        uint4 xzzw() const;
        uint4 xzwx() const;
        uint4 xzwy() const;
        uint4 xzwz() const;
        uint4 xzww() const;
        uint4 xwxx() const;
        uint4 xwxy() const;
        uint4 xwxz() const;
        uint4 xwxw() const;
        uint4 xwyx() const;
        uint4 xwyy() const;
        uint4 xwyz() const;
        uint4 xwyw() const;
        uint4 xwzx() const;
        uint4 xwzy() const;
        uint4 xwzz() const;
        uint4 xwzw() const;
        uint4 xwwx() const;
        uint4 xwwy() const;
        uint4 xwwz() const;
        uint4 xwww() const;
        uint4 yxxx() const;
        uint4 yxxy() const;
        uint4 yxxz() const;
        uint4 yxxw() const;
        uint4 yxyx() const;
        uint4 yxyy() const;
        uint4 yxyz() const;
        uint4 yxyw() const;
        uint4 yxzx() const;
        uint4 yxzy() const;
        uint4 yxzz() const;
        uint4 yxzw() const;
        uint4 yxwx() const;
        uint4 yxwy() const;
        uint4 yxwz() const;
        uint4 yxww() const;
        uint4 yyxx() const;
        uint4 yyxy() const;
        uint4 yyxz() const;
        uint4 yyxw() const;
        uint4 yyyx() const;
        uint4 yyyy() const;
        uint4 yyyz() const;
        uint4 yyyw() const;
        uint4 yyzx() const;
        uint4 yyzy() const;
        uint4 yyzz() const;
        uint4 yyzw() const;
        uint4 yywx() const;
        uint4 yywy() const;
        uint4 yywz() const;
        uint4 yyww() const;
        uint4 yzxx() const;
        uint4 yzxy() const;
        uint4 yzxz() const;
        uint4 yzxw() const;
        uint4 yzyx() const;
        uint4 yzyy() const;
        uint4 yzyz() const;
        uint4 yzyw() const;
        uint4 yzzx() const;
        uint4 yzzy() const;
        uint4 yzzz() const;
        uint4 yzzw() const;
        uint4 yzwx() const;
        uint4 yzwy() const;
        uint4 yzwz() const;
        uint4 yzww() const;
        uint4 ywxx() const;
        uint4 ywxy() const;
        uint4 ywxz() const;
        uint4 ywxw() const;
        uint4 ywyx() const;
        uint4 ywyy() const;
        uint4 ywyz() const;
        uint4 ywyw() const;
        uint4 ywzx() const;
        uint4 ywzy() const;
        uint4 ywzz() const;
        uint4 ywzw() const;
        uint4 ywwx() const;
        uint4 ywwy() const;
        uint4 ywwz() const;
        uint4 ywww() const;
        uint4 zxxx() const;
        uint4 zxxy() const;
        uint4 zxxz() const;
        uint4 zxxw() const;
        uint4 zxyx() const;
        uint4 zxyy() const;
        uint4 zxyz() const;
        uint4 zxyw() const;
        uint4 zxzx() const;
        uint4 zxzy() const;
        uint4 zxzz() const;
        uint4 zxzw() const;
        uint4 zxwx() const;
        uint4 zxwy() const;
        uint4 zxwz() const;
        uint4 zxww() const;
        uint4 zyxx() const;
        uint4 zyxy() const;
        uint4 zyxz() const;
        uint4 zyxw() const;
        uint4 zyyx() const;
        uint4 zyyy() const;
        uint4 zyyz() const;
        uint4 zyyw() const;
        uint4 zyzx() const;
        uint4 zyzy() const;
        uint4 zyzz() const;
        uint4 zyzw() const;
        uint4 zywx() const;
        uint4 zywy() const;
        uint4 zywz() const;
        uint4 zyww() const;
        uint4 zzxx() const;
        uint4 zzxy() const;
        uint4 zzxz() const;
        uint4 zzxw() const;
        uint4 zzyx() const;
        uint4 zzyy() const;
        uint4 zzyz() const;
        uint4 zzyw() const;
        uint4 zzzx() const;
        uint4 zzzy() const;
        uint4 zzzz() const;
        uint4 zzzw() const;
        uint4 zzwx() const;
        uint4 zzwy() const;
        uint4 zzwz() const;
        uint4 zzww() const;
        uint4 zwxx() const;
        uint4 zwxy() const;
        uint4 zwxz() const;
        uint4 zwxw() const;
        uint4 zwyx() const;
        uint4 zwyy() const;
        uint4 zwyz() const;
        uint4 zwyw() const;
        uint4 zwzx() const;
        uint4 zwzy() const;
        uint4 zwzz() const;
        uint4 zwzw() const;
        uint4 zwwx() const;
        uint4 zwwy() const;
        uint4 zwwz() const;
        uint4 zwww() const;
        uint4 wxxx() const;
        uint4 wxxy() const;
        uint4 wxxz() const;
        uint4 wxxw() const;
        uint4 wxyx() const;
        uint4 wxyy() const;
        uint4 wxyz() const;
        uint4 wxyw() const;
        uint4 wxzx() const;
        uint4 wxzy() const;
        uint4 wxzz() const;
        uint4 wxzw() const;
        uint4 wxwx() const;
        uint4 wxwy() const;
        uint4 wxwz() const;
        uint4 wxww() const;
        uint4 wyxx() const;
        uint4 wyxy() const;
        uint4 wyxz() const;
        uint4 wyxw() const;
        uint4 wyyx() const;
        uint4 wyyy() const;
        uint4 wyyz() const;
        uint4 wyyw() const;
        uint4 wyzx() const;
        uint4 wyzy() const;
        uint4 wyzz() const;
        uint4 wyzw() const;
        uint4 wywx() const;
        uint4 wywy() const;
        uint4 wywz() const;
        uint4 wyww() const;
        uint4 wzxx() const;
        uint4 wzxy() const;
        uint4 wzxz() const;
        uint4 wzxw() const;
        uint4 wzyx() const;
        uint4 wzyy() const;
        uint4 wzyz() const;
        uint4 wzyw() const;
        uint4 wzzx() const;
        uint4 wzzy() const;
        uint4 wzzz() const;
        uint4 wzzw() const;
        uint4 wzwx() const;
        uint4 wzwy() const;
        uint4 wzwz() const;
        uint4 wzww() const;
        uint4 wwxx() const;
        uint4 wwxy() const;
        uint4 wwxz() const;
        uint4 wwxw() const;
        uint4 wwyx() const;
        uint4 wwyy() const;
        uint4 wwyz() const;
        uint4 wwyw() const;
        uint4 wwzx() const;
        uint4 wwzy() const;
        uint4 wwzz() const;
        uint4 wwzw() const;
        uint4 wwwx() const;
        uint4 wwwy() const;
        uint4 wwwz() const;
        uint4 wwww() const;

        uint32_t x;
        uint32_t y;
        uint32_t z;
        uint32_t w;
    };

    struct bool2
    {
        bool2() = default;

        explicit bool2(const bool* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit bool2(bool x, bool y) :
            x(x), y(y)
        {
        }

        bool& operator[](uint32_t i)
        {
            assert(i < 2);
            return (&x)[i];
        }

        bool operator[](uint32_t i) const
        {
            assert(i < 2);
            return (&x)[i];
        }

        bool2 xx() const;
        bool2 xy() const;
        bool2 yx() const;
        bool2 yy() const;
        bool3 xxx() const;
        bool3 xxy() const;
        bool3 xyx() const;
        bool3 xyy() const;
        bool3 yxx() const;
        bool3 yxy() const;
        bool3 yyx() const;
        bool3 yyy() const;
        bool4 xxxx() const;
        bool4 xxxy() const;
        bool4 xxyx() const;
        bool4 xxyy() const;
        bool4 xyxx() const;
        bool4 xyxy() const;
        bool4 xyyx() const;
        bool4 xyyy() const;
        bool4 yxxx() const;
        bool4 yxxy() const;
        bool4 yxyx() const;
        bool4 yxyy() const;
        bool4 yyxx() const;
        bool4 yyxy() const;
        bool4 yyyx() const;
        bool4 yyyy() const;

        bool x;
        bool y;
    };

    struct bool3
    {
        bool3() = default;

        explicit bool3(const bool* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit bool3(bool x, bool y, bool z) :
            x(x), y(y), z(z)
        {
        }

        constexpr explicit bool3(bool2 xy, bool z) :
            x(xy.x), y(xy.y), z(z)
        {
        }

        constexpr explicit bool3(bool x, bool2 yz) :
            x(x), y(yz.x), z(yz.y)
        {
        }

        bool& operator[](uint32_t i)
        {
            assert(i < 3);
            return (&x)[i];
        }

        bool operator[](uint32_t i) const
        {
            assert(i < 3);
            return (&x)[i];
        }

        bool2 xx() const;
        bool2 xy() const;
        bool2 xz() const;
        bool2 yx() const;
        bool2 yy() const;
        bool2 yz() const;
        bool2 zx() const;
        bool2 zy() const;
        bool2 zz() const;
        bool3 xxx() const;
        bool3 xxy() const;
        bool3 xxz() const;
        bool3 xyx() const;
        bool3 xyy() const;
        bool3 xyz() const;
        bool3 xzx() const;
        bool3 xzy() const;
        bool3 xzz() const;
        bool3 yxx() const;
        bool3 yxy() const;
        bool3 yxz() const;
        bool3 yyx() const;
        bool3 yyy() const;
        bool3 yyz() const;
        bool3 yzx() const;
        bool3 yzy() const;
        bool3 yzz() const;
        bool3 zxx() const;
        bool3 zxy() const;
        bool3 zxz() const;
        bool3 zyx() const;
        bool3 zyy() const;
        bool3 zyz() const;
        bool3 zzx() const;
        bool3 zzy() const;
        bool3 zzz() const;
        bool4 xxxx() const;
        bool4 xxxy() const;
        bool4 xxxz() const;
        bool4 xxyx() const;
        bool4 xxyy() const;
        bool4 xxyz() const;
        bool4 xxzx() const;
        bool4 xxzy() const;
        bool4 xxzz() const;
        bool4 xyxx() const;
        bool4 xyxy() const;
        bool4 xyxz() const;
        bool4 xyyx() const;
        bool4 xyyy() const;
        bool4 xyyz() const;
        bool4 xyzx() const;
        bool4 xyzy() const;
        bool4 xyzz() const;
        bool4 xzxx() const;
        bool4 xzxy() const;
        bool4 xzxz() const;
        bool4 xzyx() const;
        bool4 xzyy() const;
        bool4 xzyz() const;
        bool4 xzzx() const;
        bool4 xzzy() const;
        bool4 xzzz() const;
        bool4 yxxx() const;
        bool4 yxxy() const;
        bool4 yxxz() const;
        bool4 yxyx() const;
        bool4 yxyy() const;
        bool4 yxyz() const;
        bool4 yxzx() const;
        bool4 yxzy() const;
        bool4 yxzz() const;
        bool4 yyxx() const;
        bool4 yyxy() const;
        bool4 yyxz() const;
        bool4 yyyx() const;
        bool4 yyyy() const;
        bool4 yyyz() const;
        bool4 yyzx() const;
        bool4 yyzy() const;
        bool4 yyzz() const;
        bool4 yzxx() const;
        bool4 yzxy() const;
        bool4 yzxz() const;
        bool4 yzyx() const;
        bool4 yzyy() const;
        bool4 yzyz() const;
        bool4 yzzx() const;
        bool4 yzzy() const;
        bool4 yzzz() const;
        bool4 zxxx() const;
        bool4 zxxy() const;
        bool4 zxxz() const;
        bool4 zxyx() const;
        bool4 zxyy() const;
        bool4 zxyz() const;
        bool4 zxzx() const;
        bool4 zxzy() const;
        bool4 zxzz() const;
        bool4 zyxx() const;
        bool4 zyxy() const;
        bool4 zyxz() const;
        bool4 zyyx() const;
        bool4 zyyy() const;
        bool4 zyyz() const;
        bool4 zyzx() const;
        bool4 zyzy() const;
        bool4 zyzz() const;
        bool4 zzxx() const;
        bool4 zzxy() const;
        bool4 zzxz() const;
        bool4 zzyx() const;
        bool4 zzyy() const;
        bool4 zzyz() const;
        bool4 zzzx() const;
        bool4 zzzy() const;
        bool4 zzzz() const;

        bool x;
        bool y;
        bool z;
    };

    struct bool4
    {
        bool4() = default;

        explicit bool4(const bool* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit bool4(bool x, bool y, bool z, bool w) :
            x(x), y(y), z(z), w(w)
        {
        }

        constexpr explicit bool4(bool2 xy, bool z, bool w) :
            x(xy.x), y(xy.y), z(z), w(w)
        {
        }

        constexpr explicit bool4(bool x, bool2 yz, bool w) :
            x(x), y(yz.x), z(yz.y), w(w)
        {
        }

        constexpr explicit bool4(bool x, bool y, bool2 zw) :
            x(x), y(y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit bool4(bool2 xy, bool2 zw) :
            x(xy.x), y(xy.y), z(zw.x), w(zw.y)
        {
        }

        constexpr explicit bool4(bool3 xyz, bool w) :
            x(xyz.x), y(xyz.y), z(xyz.z), w(w)
        {
        }

        constexpr explicit bool4(bool x, bool3 yzw) :
            x(x), y(yzw.x), z(yzw.y), w(yzw.z)
        {
        }

        bool& operator[](uint32_t i)
        {
            assert(i < 4);
            return (&x)[i];
        }

        bool operator[](uint32_t i) const
        {
            assert(i < 4);
            return (&x)[i];
        }

        bool2 xx() const;
        bool2 xy() const;
        bool2 xz() const;
        bool2 xw() const;
        bool2 yx() const;
        bool2 yy() const;
        bool2 yz() const;
        bool2 yw() const;
        bool2 zx() const;
        bool2 zy() const;
        bool2 zz() const;
        bool2 zw() const;
        bool2 wx() const;
        bool2 wy() const;
        bool2 wz() const;
        bool2 ww() const;
        bool3 xxx() const;
        bool3 xxy() const;
        bool3 xxz() const;
        bool3 xxw() const;
        bool3 xyx() const;
        bool3 xyy() const;
        bool3 xyz() const;
        bool3 xyw() const;
        bool3 xzx() const;
        bool3 xzy() const;
        bool3 xzz() const;
        bool3 xzw() const;
        bool3 xwx() const;
        bool3 xwy() const;
        bool3 xwz() const;
        bool3 xww() const;
        bool3 yxx() const;
        bool3 yxy() const;
        bool3 yxz() const;
        bool3 yxw() const;
        bool3 yyx() const;
        bool3 yyy() const;
        bool3 yyz() const;
        bool3 yyw() const;
        bool3 yzx() const;
        bool3 yzy() const;
        bool3 yzz() const;
        bool3 yzw() const;
        bool3 ywx() const;
        bool3 ywy() const;
        bool3 ywz() const;
        bool3 yww() const;
        bool3 zxx() const;
        bool3 zxy() const;
        bool3 zxz() const;
        bool3 zxw() const;
        bool3 zyx() const;
        bool3 zyy() const;
        bool3 zyz() const;
        bool3 zyw() const;
        bool3 zzx() const;
        bool3 zzy() const;
        bool3 zzz() const;
        bool3 zzw() const;
        bool3 zwx() const;
        bool3 zwy() const;
        bool3 zwz() const;
        bool3 zww() const;
        bool3 wxx() const;
        bool3 wxy() const;
        bool3 wxz() const;
        bool3 wxw() const;
        bool3 wyx() const;
        bool3 wyy() const;
        bool3 wyz() const;
        bool3 wyw() const;
        bool3 wzx() const;
        bool3 wzy() const;
        bool3 wzz() const;
        bool3 wzw() const;
        bool3 wwx() const;
        bool3 wwy() const;
        bool3 wwz() const;
        bool3 www() const;
        bool4 xxxx() const;
        bool4 xxxy() const;
        bool4 xxxz() const;
        bool4 xxxw() const;
        bool4 xxyx() const;
        bool4 xxyy() const;
        bool4 xxyz() const;
        bool4 xxyw() const;
        bool4 xxzx() const;
        bool4 xxzy() const;
        bool4 xxzz() const;
        bool4 xxzw() const;
        bool4 xxwx() const;
        bool4 xxwy() const;
        bool4 xxwz() const;
        bool4 xxww() const;
        bool4 xyxx() const;
        bool4 xyxy() const;
        bool4 xyxz() const;
        bool4 xyxw() const;
        bool4 xyyx() const;
        bool4 xyyy() const;
        bool4 xyyz() const;
        bool4 xyyw() const;
        bool4 xyzx() const;
        bool4 xyzy() const;
        bool4 xyzz() const;
        bool4 xyzw() const;
        bool4 xywx() const;
        bool4 xywy() const;
        bool4 xywz() const;
        bool4 xyww() const;
        bool4 xzxx() const;
        bool4 xzxy() const;
        bool4 xzxz() const;
        bool4 xzxw() const;
        bool4 xzyx() const;
        bool4 xzyy() const;
        bool4 xzyz() const;
        bool4 xzyw() const;
        bool4 xzzx() const;
        bool4 xzzy() const;
        bool4 xzzz() const;
        bool4 xzzw() const;
        bool4 xzwx() const;
        bool4 xzwy() const;
        bool4 xzwz() const;
        bool4 xzww() const;
        bool4 xwxx() const;
        bool4 xwxy() const;
        bool4 xwxz() const;
        bool4 xwxw() const;
        bool4 xwyx() const;
        bool4 xwyy() const;
        bool4 xwyz() const;
        bool4 xwyw() const;
        bool4 xwzx() const;
        bool4 xwzy() const;
        bool4 xwzz() const;
        bool4 xwzw() const;
        bool4 xwwx() const;
        bool4 xwwy() const;
        bool4 xwwz() const;
        bool4 xwww() const;
        bool4 yxxx() const;
        bool4 yxxy() const;
        bool4 yxxz() const;
        bool4 yxxw() const;
        bool4 yxyx() const;
        bool4 yxyy() const;
        bool4 yxyz() const;
        bool4 yxyw() const;
        bool4 yxzx() const;
        bool4 yxzy() const;
        bool4 yxzz() const;
        bool4 yxzw() const;
        bool4 yxwx() const;
        bool4 yxwy() const;
        bool4 yxwz() const;
        bool4 yxww() const;
        bool4 yyxx() const;
        bool4 yyxy() const;
        bool4 yyxz() const;
        bool4 yyxw() const;
        bool4 yyyx() const;
        bool4 yyyy() const;
        bool4 yyyz() const;
        bool4 yyyw() const;
        bool4 yyzx() const;
        bool4 yyzy() const;
        bool4 yyzz() const;
        bool4 yyzw() const;
        bool4 yywx() const;
        bool4 yywy() const;
        bool4 yywz() const;
        bool4 yyww() const;
        bool4 yzxx() const;
        bool4 yzxy() const;
        bool4 yzxz() const;
        bool4 yzxw() const;
        bool4 yzyx() const;
        bool4 yzyy() const;
        bool4 yzyz() const;
        bool4 yzyw() const;
        bool4 yzzx() const;
        bool4 yzzy() const;
        bool4 yzzz() const;
        bool4 yzzw() const;
        bool4 yzwx() const;
        bool4 yzwy() const;
        bool4 yzwz() const;
        bool4 yzww() const;
        bool4 ywxx() const;
        bool4 ywxy() const;
        bool4 ywxz() const;
        bool4 ywxw() const;
        bool4 ywyx() const;
        bool4 ywyy() const;
        bool4 ywyz() const;
        bool4 ywyw() const;
        bool4 ywzx() const;
        bool4 ywzy() const;
        bool4 ywzz() const;
        bool4 ywzw() const;
        bool4 ywwx() const;
        bool4 ywwy() const;
        bool4 ywwz() const;
        bool4 ywww() const;
        bool4 zxxx() const;
        bool4 zxxy() const;
        bool4 zxxz() const;
        bool4 zxxw() const;
        bool4 zxyx() const;
        bool4 zxyy() const;
        bool4 zxyz() const;
        bool4 zxyw() const;
        bool4 zxzx() const;
        bool4 zxzy() const;
        bool4 zxzz() const;
        bool4 zxzw() const;
        bool4 zxwx() const;
        bool4 zxwy() const;
        bool4 zxwz() const;
        bool4 zxww() const;
        bool4 zyxx() const;
        bool4 zyxy() const;
        bool4 zyxz() const;
        bool4 zyxw() const;
        bool4 zyyx() const;
        bool4 zyyy() const;
        bool4 zyyz() const;
        bool4 zyyw() const;
        bool4 zyzx() const;
        bool4 zyzy() const;
        bool4 zyzz() const;
        bool4 zyzw() const;
        bool4 zywx() const;
        bool4 zywy() const;
        bool4 zywz() const;
        bool4 zyww() const;
        bool4 zzxx() const;
        bool4 zzxy() const;
        bool4 zzxz() const;
        bool4 zzxw() const;
        bool4 zzyx() const;
        bool4 zzyy() const;
        bool4 zzyz() const;
        bool4 zzyw() const;
        bool4 zzzx() const;
        bool4 zzzy() const;
        bool4 zzzz() const;
        bool4 zzzw() const;
        bool4 zzwx() const;
        bool4 zzwy() const;
        bool4 zzwz() const;
        bool4 zzww() const;
        bool4 zwxx() const;
        bool4 zwxy() const;
        bool4 zwxz() const;
        bool4 zwxw() const;
        bool4 zwyx() const;
        bool4 zwyy() const;
        bool4 zwyz() const;
        bool4 zwyw() const;
        bool4 zwzx() const;
        bool4 zwzy() const;
        bool4 zwzz() const;
        bool4 zwzw() const;
        bool4 zwwx() const;
        bool4 zwwy() const;
        bool4 zwwz() const;
        bool4 zwww() const;
        bool4 wxxx() const;
        bool4 wxxy() const;
        bool4 wxxz() const;
        bool4 wxxw() const;
        bool4 wxyx() const;
        bool4 wxyy() const;
        bool4 wxyz() const;
        bool4 wxyw() const;
        bool4 wxzx() const;
        bool4 wxzy() const;
        bool4 wxzz() const;
        bool4 wxzw() const;
        bool4 wxwx() const;
        bool4 wxwy() const;
        bool4 wxwz() const;
        bool4 wxww() const;
        bool4 wyxx() const;
        bool4 wyxy() const;
        bool4 wyxz() const;
        bool4 wyxw() const;
        bool4 wyyx() const;
        bool4 wyyy() const;
        bool4 wyyz() const;
        bool4 wyyw() const;
        bool4 wyzx() const;
        bool4 wyzy() const;
        bool4 wyzz() const;
        bool4 wyzw() const;
        bool4 wywx() const;
        bool4 wywy() const;
        bool4 wywz() const;
        bool4 wyww() const;
        bool4 wzxx() const;
        bool4 wzxy() const;
        bool4 wzxz() const;
        bool4 wzxw() const;
        bool4 wzyx() const;
        bool4 wzyy() const;
        bool4 wzyz() const;
        bool4 wzyw() const;
        bool4 wzzx() const;
        bool4 wzzy() const;
        bool4 wzzz() const;
        bool4 wzzw() const;
        bool4 wzwx() const;
        bool4 wzwy() const;
        bool4 wzwz() const;
        bool4 wzww() const;
        bool4 wwxx() const;
        bool4 wwxy() const;
        bool4 wwxz() const;
        bool4 wwxw() const;
        bool4 wwyx() const;
        bool4 wwyy() const;
        bool4 wwyz() const;
        bool4 wwyw() const;
        bool4 wwzx() const;
        bool4 wwzy() const;
        bool4 wwzz() const;
        bool4 wwzw() const;
        bool4 wwwx() const;
        bool4 wwwy() const;
        bool4 wwwz() const;
        bool4 wwww() const;

        bool x;
        bool y;
        bool z;
        bool w;
    };

    struct Matrix4x4
    {
        Matrix4x4() = default;

        explicit Matrix4x4(const float* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit Matrix4x4(float4 a, float4 b, float4 c, float4 d) :
            a(a), b(b), c(c), d(d)
        {
        }

        constexpr explicit Matrix4x4
        (
            float ax, float ay, float az, float aw,
            float bx, float by, float bz, float bw,
            float cx, float cy, float cz, float cw,
            float dx, float dy, float dz, float dw
        )
            : a(ax, ay, az, aw), b(bx, by, bz, bw), c(cx, cy, cz, cw), d(dx, dy, dz, dw)
        {
        }

        float& operator[](uint32_t index)
        {
            assert(index < 16);
            return (&a.x)[index];
        }

        float operator[](uint32_t index) const
        {
            assert(index < 16);
            return (&a.x)[index];
        }

        float& operator()(uint32_t i, uint32_t j)
        {
            assert(i < 4 && j < 4);
            return (&a.x)[i * 4 + j];
        }

        float operator()(uint32_t i, uint32_t j) const
        {
            assert(i < 4 && j < 4);
            return (&a.x)[i * 4 + j];
        }


        float4 a;
        float4 b;
        float4 c;
        float4 d;
    };

    struct Plane
    {
        Plane() = default;

        explicit Plane(const float* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit Plane(float normalX, float normalY, float normalZ, float distance) :
            normal(normalX, normalY, normalZ), distance(distance)
        {
        }

        constexpr explicit Plane(float3 normal, float distance) :
            normal(normal), distance(distance)
        {
        }

        float3 normal;
        float distance;
    };

    struct Quaternion
    {
        Quaternion() = default;

        explicit Quaternion(const float* ptr)
        {
            memcpy(this, ptr, sizeof(*this));
        }

        constexpr explicit Quaternion(float x, float y, float z, float w) :
            x(x), y(y), z(z), w(w)
        {
        }

        float x;
        float y;
        float z;
        float w;
    };

    // Constants
    // ======================================================================

    constexpr float Sqrt2 = 1.414213562f;
    constexpr float Sqrt3 = 1.732050808f;
    constexpr float DegToRadFactor = Pi / 180.0f;
    constexpr float RadToDegFactor = 180.0f / Pi;


    constexpr float2 Vec2Zero = float2(0, 0);
    constexpr float2 Vec2One = float2(1, 1);
    constexpr float2 Vec2NegOne = float2(-1, -1);
    constexpr float2 Vec2FloatMax = float2(FLT_MAX, FLT_MAX);
    constexpr float2 Vec2FloatMin = float2(FLT_MIN, FLT_MIN);

    constexpr float3 Vec3Zero = float3(0, 0, 0);
    constexpr float3 Vec3One = float3(1, 1, 1);
    constexpr float3 Vec3NegOne = float3(-1, -1, -1);
    constexpr float3 Vec3FloatMax = float3(FLT_MAX, FLT_MAX, FLT_MAX);
    constexpr float3 Vec3FloatMin = float3(FLT_MIN, FLT_MIN, FLT_MIN);

    constexpr float4 Vec4Zero = float4(0, 0, 0, 0);
    constexpr float4 Vec4One = float4(1, 1, 1, 1);
    constexpr float4 Vec4NegOne = float4(-1, -1, -1, -1);
    constexpr float4 Vec4FloatMax = float4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
    constexpr float4 Vec4FloatMin = float4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN);

    constexpr Quaternion QuatIdentity = Quaternion(0, 0, 0, 1);

    constexpr Matrix4x4 Mat4Zero = Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    constexpr Matrix4x4 Mat4Identity = Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    // Loads
    // =============================================================================================================
    inline vfloat4 VMATH_CALLCONV vLoad(const float2* x)
    {
        return vLoadFloat2(reinterpret_cast<const float*>(x));
    }

    inline vfloat4 VMATH_CALLCONV vLoad(const float3* x)
    {
        return vLoadFloat3(reinterpret_cast<const float*>(x));
    }

    inline vfloat4 VMATH_CALLCONV vLoad(const float4* x)
    {
        return vLoadFloat4(reinterpret_cast<const float*>(x));
    }

    inline vfloat4 VMATH_CALLCONV vLoad(const Quaternion* x)
    {
        return vLoadFloat4(reinterpret_cast<const float*>(x));
    }

    inline vfloat4 VMATH_CALLCONV vLoad(const Plane* x)
    {
        return vLoadFloat4(reinterpret_cast<const float*>(x));
    }

    inline vfloat4x4 VMATH_CALLCONV vLoad(const Matrix4x4* x)
    {
        return vLoadFloat4x4(reinterpret_cast<const float*>(x));
    }

    inline vbool4 vLoad(const bool2* x)
    {
        return vLoadBool2(reinterpret_cast<const bool*>(x));
    }

    inline vbool4 vLoad(const bool3* x)
    {
        return vLoadBool3(reinterpret_cast<const bool*>(x));
    }

    inline vbool4 vLoad(const bool4* x)
    {
        return vLoadBool4(reinterpret_cast<const bool*>(x));
    }

    // Stores
    // =============================================================================================================
    inline void VMATH_CALLCONV vStore(float* dst, vfloat4 x)
    {
        vStoreFloat(reinterpret_cast<float*>(dst), x);
    }

    inline void VMATH_CALLCONV vStore(float2* dst, vfloat4 x)
    {
        vStoreFloat2(reinterpret_cast<float*>(dst), x);
    }

    inline void VMATH_CALLCONV vStore(float3* dst, vfloat4 x)
    {
        vStoreFloat3(reinterpret_cast<float*>(dst), x);
    }

    inline void VMATH_CALLCONV vStore(float4* dst, vfloat4 x)
    {
        vStoreFloat3(reinterpret_cast<float*>(dst), x);
    }

    inline void VMATH_CALLCONV vStore(Quaternion* dst, vfloat4 x)
    {
        vStoreFloat4(reinterpret_cast<float*>(dst), x);
    }

    inline void VMATH_CALLCONV vStore(Plane* dst, vfloat4 x)
    {
        vStoreFloat4(reinterpret_cast<float*>(dst), x);
    }

    inline void VMATH_CALLCONV vStore(Matrix4x4* dst, vfloat4x4 x)
    {
        vStoreFloat4x4(reinterpret_cast<float*>(dst), x);
    }

    inline void vStore(bool2* dst, vbool4 x)
    {
        vStoreBool2(reinterpret_cast<bool*>(dst), x);
    }

    inline void vStore(bool3* dst, vbool4 x)
    {
        vStoreBool3(reinterpret_cast<bool*>(dst), x);
    }

    inline void vStore(bool4* dst, vbool4 x)
    {
        vStoreBool4(reinterpret_cast<bool*>(dst), x);
    }

    // Spreads
    // =============================================================================================================

    constexpr float4 Vec4fSpread(float x)
    {
        return float4(x, x, x, x);
    }

    constexpr float3 Vec3fSpread(float x)
    {
        return float3(x, x, x);
    }

    constexpr float2 Vec2fSpread(float x)
    {
        return float2(x, x);
    }

    constexpr uint4 Vec4uSpread(uint32_t x)
    {
        return uint4(x, x, x, x);
    }

    constexpr uint3 Vec3uSpread(uint32_t x)
    {
        return uint3(x, x, x);
    }

    constexpr uint2 Vec2uSpread(uint32_t x)
    {
        return uint2(x, x);
    }

    constexpr int4 Vec4iSpread(int32_t x)
    {
        return int4(x, x, x, x);
    }

    constexpr int3 Vec3iSpread(int32_t x)
    {
        return int3(x, x, x);
    }

    constexpr int2 Vec2iSpread(int32_t x)
    {
        return int2(x, x);
    }

    constexpr bool4 Vec4bSpread(bool x)
    {
        return bool4(x, x, x, x);
    }

    constexpr bool3 Vec3bSpread(bool x)
    {
        return bool3(x, x, x);
    }

    constexpr bool2 Vec2bSpread(bool x)
    {
        return bool2(x, x);
    }

    // Conversions
    // =============================================================================================================

    inline float2 Vec2To2f(int2 v)
    {
        return float2(float(v.x), float(v.y));
    }

    inline float3 Vec3To3f(int3 v)
    {
        return float3(float(v.x), float(v.y), float(v.z));
    }

    inline float4 Vec4To4f(int4 v)
    {
        return float4(float(v.x), float(v.y), float(v.z), float(v.w));
    }

    inline uint2 Vec2To2u(int2 v)
    {
        return uint2(uint32_t(v.x), uint32_t(v.y));
    }

    inline uint3 Vec3To3u(int3 v)
    {
        return uint3(uint32_t(v.x), uint32_t(v.y), uint32_t(v.z));
    }

    inline uint4 Vec4To4u(int4 v)
    {
        return uint4(uint32_t(v.x), uint32_t(v.y), uint32_t(v.z), uint32_t(v.w));
    }

    inline float2 Vec2To2f(uint2 v)
    {
        return float2(float(v.x), float(v.y));
    }

    inline float3 Vec3To3f(uint3 v)
    {
        return float3(float(v.x), float(v.y), float(v.z));
    }

    inline float4 Vec4To4f(uint4 v)
    {
        return float4(float(v.x), float(v.y), float(v.z), float(v.w));
    }

    inline int2 Vec2To2i(uint2 v)
    {
        return int2(int32_t(v.x), int32_t(v.y));
    }

    inline int3 Vec3To3i(uint3 v)
    {
        return int3(int32_t(v.x), int32_t(v.y), int32_t(v.z));
    }

    inline int4 Vec4To4i(uint4 v)
    {
        return int4(int32_t(v.x), int32_t(v.y), int32_t(v.z), int32_t(v.w));
    }

    inline int2 Vec2To2i(float2 v)
    {
        return int2(int32_t(v.x), int32_t(v.y));
    }

    inline int3 Vec3To3i(float3 v)
    {
        return int3(int32_t(v.x), int32_t(v.y), int32_t(v.z));
    }

    inline int4 Vec4To4i(float4 v)
    {
        return int4(int32_t(v.x), int32_t(v.y), int32_t(v.z), int32_t(v.w));
    }

    inline uint2 Vec2To2u(float2 v)
    {
        return uint2(uint32_t(v.x), uint32_t(v.y));
    }

    inline uint3 Vec3To3u(float3 v)
    {
        return uint3(uint32_t(v.x), uint32_t(v.y), uint32_t(v.z));
    }

    inline uint4 Vec4To4u(float4 v)
    {
        return uint4(uint32_t(v.x), uint32_t(v.y), uint32_t(v.z), uint32_t(v.w));
    }

    // =============================================================================================================

    inline bool4 Vec4NearEqual(float4 a, float4 b, float4 epsilon)
    {
        vbool4 vRet = v4CompressMask128(v4NearEqual(vLoad(&a), vLoad(&b), vLoad(&epsilon)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3NearEqual(float3 a, float3 b, float3 epsilon)
    {
        vbool4 vRet = v4CompressMask128(v4NearEqual(vLoad(&a), vLoad(&b), vLoad(&epsilon)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2NearEqual(float2 a, float2 b, float2 epsilon)
    {
        vbool4 vRet = v4CompressMask128(v4NearEqual(vLoad(&a), vLoad(&b), vLoad(&epsilon)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Rotate(float3 v, Quaternion rotationQuaternion)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&rotationQuaternion);
        vfloat4 vRet = v3Rotate(lv0, lv1);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3InverseRotate(float3 v, Quaternion rotationQuaternion)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&rotationQuaternion);
        vfloat4 vRet = v3InverseRotate(lv0, lv1);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool Mat4IsAnyNaN(const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        return vMat4IsAnyNaN(lm0);
    }

    inline bool Mat4IsAnyInfinite(const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        return vMat4IsAnyInfinite(lm0);
    }

    inline bool Mat4IsIdentity(const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        return vMat4IsIdentity(lm0);
    }

    inline Matrix4x4 Mat4Mul(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        vfloat4x4 lm0 = vLoad(&m1);
        vfloat4x4 lm1 = vLoad(&m2);
        vfloat4x4 vRet = vMat4Mul(lm0, lm1);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4MulTranspose(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        vfloat4x4 lm0 = vLoad(&m1);
        vfloat4x4 lm1 = vLoad(&m2);
        vfloat4x4 vRet = vMat4MulTranspose(lm0, lm1);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Transpose(const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4x4 vRet = vMat4Transpose(lm0);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4VectorTensorProduct(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4x4 vRet = vMat4VectorTensorProduct(lv0, lv1);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Mat4Determinant(const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = vMat4Determinant(lm0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool Mat4Decompose(float3* outScale, Quaternion* outRotQuat, float3* outTrans, const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 outV0;
        vfloat4 outV1;
        vfloat4 outV2;
        bool ret = vMat4Decompose(&outV0, &outV1, &outV2, lm0);
        vStore(outScale, outV0);
        vStore(outRotQuat, outV1);
        vStore(outTrans, outV2);
        return ret;
    }

    inline Matrix4x4 Mat4Translation(float offsetX, float offsetY, float offsetZ)
    {
        vfloat4x4 vRet = vMat4Translation(offsetX, offsetY, offsetZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Translation(float3 offset)
    {
        vfloat4 lv0 = vLoad(&offset);
        vfloat4x4 vRet = vMat4Translation(lv0);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Scaling(float scaleX, float scaleY, float scaleZ)
    {
        vfloat4x4 vRet = vMat4Scaling(scaleX, scaleY, scaleZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Scaling(float3 scale)
    {
        vfloat4 lv0 = vLoad(&scale);
        vfloat4x4 vRet = vMat4Scaling(lv0);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationX(float angle)
    {
        vfloat4x4 vRet = vMat4RotationX(angle);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationY(float angle)
    {
        vfloat4x4 vRet = vMat4RotationY(angle);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationZ(float angle)
    {
        vfloat4x4 vRet = vMat4RotationZ(angle);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationRollPitchYaw(float pitch, float yaw, float roll)
    {
        vfloat4x4 vRet = vMat4RotationRollPitchYaw(pitch, yaw, roll);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationRollPitchYawFromVector(float3 angles)
    {
        vfloat4 lv0 = vLoad(&angles);
        vfloat4x4 vRet = vMat4RotationRollPitchYawFromVector(lv0);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationNormal(float3 normalAxis, float angle)
    {
        vfloat4 lv0 = vLoad(&normalAxis);
        vfloat4x4 vRet = vMat4RotationNormal(lv0, angle);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationAxis(float3 axis, float angle)
    {
        vfloat4 lv0 = vLoad(&axis);
        vfloat4x4 vRet = vMat4RotationAxis(lv0, angle);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4RotationQuaternion(Quaternion quaternion)
    {
        vfloat4 lv0 = vLoad(&quaternion);
        vfloat4x4 vRet = vMat4RotationQuaternion(lv0);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Transformation2D(float2 scalingOrigin, float scalingOrientation, float2 scaling, float2 rotationOrigin, float rotation, float2 translation)
    {
        vfloat4 lv0 = vLoad(&scalingOrigin);
        vfloat4 lv1 = vLoad(&scaling);
        vfloat4 lv2 = vLoad(&rotationOrigin);
        vfloat4 lv3 = vLoad(&translation);
        vfloat4x4 vRet = vMat4Transformation2D(lv0, scalingOrientation, lv1, lv2, rotation, lv3);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Transformation(float3 scalingOrigin, Quaternion scalingOrientationQuaternion, float3 scaling, float3 rotationOrigin, Quaternion rotationQuaternion, float3 translation)
    {
        vfloat4 lv0 = vLoad(&scalingOrigin);
        vfloat4 lv1 = vLoad(&scalingOrientationQuaternion);
        vfloat4 lv2 = vLoad(&scaling);
        vfloat4 lv3 = vLoad(&rotationOrigin);
        vfloat4 lv4 = vLoad(&rotationQuaternion);
        vfloat4 lv5 = vLoad(&translation);
        vfloat4x4 vRet = vMat4Transformation(lv0, lv1, lv2, lv3, lv4, lv5);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4AffineTransformation2D(float3 scaling, float3 rotationOrigin, float rotation, float3 translation)
    {
        vfloat4 lv0 = vLoad(&scaling);
        vfloat4 lv1 = vLoad(&rotationOrigin);
        vfloat4 lv2 = vLoad(&translation);
        vfloat4x4 vRet = vMat4AffineTransformation2D(lv0, lv1, rotation, lv2);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4AffineTransformation(float3 scaling, float3 rotationOrigin, Quaternion rotationQuaternion, float3 translation)
    {
        vfloat4 lv0 = vLoad(&scaling);
        vfloat4 lv1 = vLoad(&rotationOrigin);
        vfloat4 lv2 = vLoad(&rotationQuaternion);
        vfloat4 lv3 = vLoad(&translation);
        vfloat4x4 vRet = vMat4AffineTransformation(lv0, lv1, lv2, lv3);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4LookAt(float3 eyePosition, float3 focusPosition, float3 upDirection)
    {
        vfloat4 lv0 = vLoad(&eyePosition);
        vfloat4 lv1 = vLoad(&focusPosition);
        vfloat4 lv2 = vLoad(&upDirection);
        vfloat4x4 vRet = vMat4LookAt(lv0, lv1, lv2);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4LookTo(float3 eyePosition, float3 eyeDirection, float3 upDirection)
    {
        vfloat4 lv0 = vLoad(&eyePosition);
        vfloat4 lv1 = vLoad(&eyeDirection);
        vfloat4 lv2 = vLoad(&upDirection);
        vfloat4x4 vRet = vMat4LookTo(lv0, lv1, lv2);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Perspective(float viewWidth, float viewHeight, float nearZ, float farZ)
    {
        vfloat4x4 vRet = vMat4Perspective(viewWidth, viewHeight, nearZ, farZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4PerspectiveFOV(float fovAngleY, float aspectRatio, float nearZ, float farZ)
    {
        vfloat4x4 vRet = vMat4PerspectiveFOV(fovAngleY, aspectRatio, nearZ, farZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4PerspectiveOffCenter(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
    {
        vfloat4x4 vRet = vMat4PerspectiveOffCenter(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Orthographic(float viewWidth, float viewHeight, float nearZ, float farZ)
    {
        vfloat4x4 vRet = vMat4Orthographic(viewWidth, viewHeight, nearZ, farZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4OrthographicOffCenter(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
    {
        vfloat4x4 vRet = vMat4OrthographicOffCenter(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Inverse(const Matrix4x4& m, float* pDeterminant = nullptr)
    {
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vdet;
        vfloat4x4 vRet = vMat4Inverse(&vdet, lm0);

        if (pDeterminant)
            vStore(pDeterminant, vdet);

        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Matrix4x4 Mat4Reflect(Plane reflectionPlane)
    {
        vfloat4 lv0 = vLoad(&reflectionPlane);
        vfloat4x4 vRet = vMat4Reflect(lv0);
        Matrix4x4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatSlerp(Quaternion q0, Quaternion q1, float4 t)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 lv2 = vLoad(&t);
        vfloat4 vRet = vQuatSlerp(lv0, lv1, lv2);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatSquad(Quaternion q0, Quaternion q1, Quaternion q2, Quaternion q3, float4 t)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 lv2 = vLoad(&q2);
        vfloat4 lv3 = vLoad(&q3);
        vfloat4 lv4 = vLoad(&t);
        vfloat4 vRet = vQuatSquad(lv0, lv1, lv2, lv3, lv4);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatBaryCentric(Quaternion q0, Quaternion q1, Quaternion q2, float4 f, float4 g)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 lv2 = vLoad(&q2);
        vfloat4 lv3 = vLoad(&f);
        vfloat4 lv4 = vLoad(&g);
        vfloat4 vRet = vQuatBaryCentric(lv0, lv1, lv2, lv3, lv4);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatRotationRollPitchYawFromVector(float3 angles)
    {
        vfloat4 lv0 = vLoad(&angles);
        vfloat4 vRet = vQuatRotationRollPitchYawFromVector(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatNormalAngle(float3 normalAxis, float angle)
    {
        vfloat4 lv0 = vLoad(&normalAxis);
        vfloat4 vRet = vQuatNormalAngle(lv0, angle);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatAxisAngle(float3 axis, float angle)
    {
        vfloat4 lv0 = vLoad(&axis);
        vfloat4 vRet = vQuatAxisAngle(lv0, angle);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline void QuatToAxisAngle(float3* pAxis, float* pAngle, Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 outV0;
        vQuatToAxisAngle(&outV0, pAngle, lv0);
        vStore(pAxis, outV0);
    }

    inline Plane PlaneNormalizeEst(Plane p)
    {
        vfloat4 lv0 = vLoad(&p);
        vfloat4 vRet = vPlaneNormalizeEst(lv0);
        Plane ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Plane PlaneNormalize(Plane p)
    {
        vfloat4 lv0 = vLoad(&p);
        vfloat4 vRet = vPlaneNormalize(lv0);
        Plane ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Plane PlaneIntersectLine(Plane p, float3 linePoint1, float3 linePoint2)
    {
        vfloat4 lv0 = vLoad(&p);
        vfloat4 lv1 = vLoad(&linePoint1);
        vfloat4 lv2 = vLoad(&linePoint2);
        vfloat4 vRet = vPlaneIntersectLine(lv0, lv1, lv2);
        Plane ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline void PlaneIntersectPlane(float3* pLinePoint1, float3* pLinePoint2, Plane p1, Plane p2)
    {
        vfloat4 lv0 = vLoad(&p1);
        vfloat4 lv1 = vLoad(&p2);
        vfloat4 outV0;
        vfloat4 outV1;
        vPlaneIntersectPlane(&outV0, &outV1, lv0, lv1);
        vStore(pLinePoint1, outV0);
        vStore(pLinePoint2, outV1);
    }

    inline Plane PlaneTransform(Plane p, const Matrix4x4& m)
    {
        vfloat4 lv0 = vLoad(&p);
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = vPlaneTransform(lv0, lm0);
        Plane ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Plane PlaneFromPointNormal(float3 point, float3 normal)
    {
        vfloat4 lv0 = vLoad(&point);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = vPlaneFromPointNormal(lv0, lv1);
        Plane ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Plane PlaneFromPoints(float3 point1, float3 point2, float3 point3)
    {
        vfloat4 lv0 = vLoad(&point1);
        vfloat4 lv1 = vLoad(&point2);
        vfloat4 lv2 = vLoad(&point3);
        vfloat4 vRet = vPlaneFromPoints(lv0, lv1, lv2);
        Plane ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SetBinary(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
    {
        vfloat4 vRet = v4SetBinary(x, y, z, w);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SpreadBinary(uint32_t value)
    {
        vfloat4 vRet = v4SpreadBinary(value);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Swizzle(float4 v, uint32_t e0, uint32_t e1, uint32_t e2, uint32_t e3)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Swizzle(lv0, e0, e1, e2, e3);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Shuffle(float4 v1, float4 v2, uint32_t permuteX, uint32_t permuteY, uint32_t permuteZ, uint32_t permuteW)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Shuffle(lv0, lv1, permuteX, permuteY, permuteZ, permuteW);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SelectControl(uint32_t vectorIndex0, uint32_t vectorIndex1, uint32_t vectorIndex2, uint32_t vectorIndex3)
    {
        vfloat4 vRet = v4SelectControl(vectorIndex0, vectorIndex1, vectorIndex2, vectorIndex3);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Select(float4 v1, float4 v2, float4 control)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&control);
        vfloat4 vRet = v4Select(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4PushLeft(float4 v1, float4 v2, uint32_t elements)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4PushLeft(lv0, lv1, elements);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4RotateLeft(float4 v, uint32_t elements)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RotateLeft(lv0, elements);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4RotateRight(float4 v, uint32_t elements)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RotateRight(lv0, elements);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Insert(float4 vD, float4 vS, uint32_t vSLeftRotateElements, uint32_t select0, uint32_t select1, uint32_t select2, uint32_t select3)
    {
        vfloat4 lv0 = vLoad(&vD);
        vfloat4 lv1 = vLoad(&vS);
        vfloat4 vRet = v4Insert(lv0, lv1, vSLeftRotateElements, select0, select1, select2, select3);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4InBoundsAbs(float4 v, float4 bounds)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&bounds);
        vfloat4 vRet = v4InBoundsAbs(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4IsNaN(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4IsNaN(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4IsInfinite(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4IsInfinite(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Min(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Min(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Max(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Max(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Round(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Round(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Truncate(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Truncate(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Floor(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Floor(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Ceiling(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Ceiling(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Clamp(float4 v, float4 min, float4 max)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&min);
        vfloat4 lv2 = vLoad(&max);
        vfloat4 vRet = v4Clamp(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Clamp01(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Clamp01(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4And(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4And(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4AndNot(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4AndNot(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Or(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Or(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Nor(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Nor(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Xor(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Xor(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Negate(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Negate(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Add(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Add(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4AddAngles(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4AddAngles(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Sub(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Sub(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SubAngles(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4SubAngles(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Mul(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Mul(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4MulAdd(float4 v1, float4 v2, float4 v3)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&v3);
        vfloat4 vRet = v4MulAdd(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Div(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Div(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4NegateMulAdd(float4 v1, float4 v2, float4 v3)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&v3);
        vfloat4 vRet = v4NegateMulAdd(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4RcpEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RcpEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Rcp(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Rcp(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SqrtEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4SqrtEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Sqrt(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Sqrt(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4RSqrtEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RSqrtEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4RSqrt(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RSqrt(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Exp2(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Exp2(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Exp10(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Exp10(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ExpE(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ExpE(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Log2(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Log2(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Log10(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Log10(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4LogE(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4LogE(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Pow(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Pow(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Abs(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Abs(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Mod(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Mod(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ModAngles(float4 angles)
    {
        vfloat4 lv0 = vLoad(&angles);
        vfloat4 vRet = v4ModAngles(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Sin(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Sin(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Cos(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Cos(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Tan(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Tan(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SinH(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4SinH(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4CosH(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4CosH(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4TanH(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4TanH(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ASin(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ASin(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ACos(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ACos(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ATan(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ATan(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ATan2(float4 y, float4 x)
    {
        vfloat4 lv0 = vLoad(&y);
        vfloat4 lv1 = vLoad(&x);
        vfloat4 vRet = v4ATan2(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4SinEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4SinEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4CosEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4CosEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4TanEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4TanEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ASinEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ASinEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ACosEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ACosEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ATanEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ATanEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ATan2Est(float4 y, float4 x)
    {
        vfloat4 lv0 = vLoad(&y);
        vfloat4 lv1 = vLoad(&x);
        vfloat4 vRet = v4ATan2Est(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Lerp(float4 v0, float4 v1, float t)
    {
        vfloat4 lv0 = vLoad(&v0);
        vfloat4 lv1 = vLoad(&v1);
        vfloat4 vRet = v4Lerp(lv0, lv1, t);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Lerp(float4 v0, float4 v1, float4 t)
    {
        vfloat4 lv0 = vLoad(&v0);
        vfloat4 lv1 = vLoad(&v1);
        vfloat4 lv2 = vLoad(&t);
        vfloat4 vRet = v4Lerp(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Hermite(float4 position0, float4 tangent0, float4 position1, float4 tangent1, float t)
    {
        vfloat4 lv0 = vLoad(&position0);
        vfloat4 lv1 = vLoad(&tangent0);
        vfloat4 lv2 = vLoad(&position1);
        vfloat4 lv3 = vLoad(&tangent1);
        vfloat4 vRet = v4Hermite(lv0, lv1, lv2, lv3, t);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Hermite(float4 position0, float4 tangent0, float4 position1, float4 tangent1, float4 t)
    {
        vfloat4 lv0 = vLoad(&position0);
        vfloat4 lv1 = vLoad(&tangent0);
        vfloat4 lv2 = vLoad(&position1);
        vfloat4 lv3 = vLoad(&tangent1);
        vfloat4 lv4 = vLoad(&t);
        vfloat4 vRet = v4Hermite(lv0, lv1, lv2, lv3, lv4);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4CatmullRom(float4 position0, float4 position1, float4 position2, float4 position3, float t)
    {
        vfloat4 lv0 = vLoad(&position0);
        vfloat4 lv1 = vLoad(&position1);
        vfloat4 lv2 = vLoad(&position2);
        vfloat4 lv3 = vLoad(&position3);
        vfloat4 vRet = v4CatmullRom(lv0, lv1, lv2, lv3, t);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4CatmullRom(float4 position0, float4 position1, float4 position2, float4 position3, float4 t)
    {
        vfloat4 lv0 = vLoad(&position0);
        vfloat4 lv1 = vLoad(&position1);
        vfloat4 lv2 = vLoad(&position2);
        vfloat4 lv3 = vLoad(&position3);
        vfloat4 lv4 = vLoad(&t);
        vfloat4 vRet = v4CatmullRom(lv0, lv1, lv2, lv3, lv4);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool Vec2AllEqual(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllEqual(lv0, lv1);
    }

    inline bool Vec2AllBinaryEqual(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllBinaryEqual(lv0, lv1);
    }

    inline bool Vec2AllNearEqual(float2 v1, float2 v2, float2 epsilon)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&epsilon);
        return v2AllNearEqual(lv0, lv1, lv2);
    }

    inline bool Vec2AnyNotEqual(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AnyNotEqual(lv0, lv1);
    }

    inline bool Vec2AnyBinaryNotEqual(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AnyBinaryNotEqual(lv0, lv1);
    }

    inline bool Vec2AllGreater(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllGreater(lv0, lv1);
    }

    inline bool Vec2AllGreaterEqual(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllGreaterEqual(lv0, lv1);
    }

    inline bool Vec2AllLess(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllLess(lv0, lv1);
    }

    inline bool Vec2AllLessEqual(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllLessEqual(lv0, lv1);
    }

    inline bool Vec2AllInBoundsAbs(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v2AllInBoundsAbs(lv0, lv1);
    }

    inline bool Vec2AnyIsNaN(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        return v2AnyIsNaN(lv0);
    }

    inline bool Vec2AnyIsInfinite(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        return v2AnyIsInfinite(lv0);
    }

    inline float Vec2Dot(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v2Dot(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Cross(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v2Cross(lv0, lv1);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2LengthSq(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2LengthSq(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2RLengthEst(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2RLengthEst(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2RLength(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2RLength(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2LengthEst(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2LengthEst(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2Length(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2Length(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2NormalizeEst(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2NormalizeEst(lv0);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Normalize(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2Normalize(lv0);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ClampLength(float2 v, float lengthMin, float lengthMax)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2ClampLength(lv0, lengthMin, lengthMax);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ClampLength(float2 v, float2 lengthMin, float2 lengthMax)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&lengthMin);
        vfloat4 lv2 = vLoad(&lengthMax);
        vfloat4 vRet = v2ClampLength(lv0, lv1, lv2);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Reflect(float2 incident, float2 normal)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = v2Reflect(lv0, lv1);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Refract(float2 incident, float2 normal, float refractionIndex)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = v2Refract(lv0, lv1, refractionIndex);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Refract(float2 incident, float2 normal, float2 refractionIndex)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 lv2 = vLoad(&refractionIndex);
        vfloat4 vRet = v2Refract(lv0, lv1, lv2);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Orthogonal(float2 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v2Orthogonal(lv0);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2AngleBetweenNormalsEst(float2 n1, float2 n2)
    {
        vfloat4 lv0 = vLoad(&n1);
        vfloat4 lv1 = vLoad(&n2);
        vfloat4 vRet = v2AngleBetweenNormalsEst(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2AngleBetweenNormals(float2 n1, float2 n2)
    {
        vfloat4 lv0 = vLoad(&n1);
        vfloat4 lv1 = vLoad(&n2);
        vfloat4 vRet = v2AngleBetweenNormals(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2AngleBetweenVectors(float2 v1, float2 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v2AngleBetweenVectors(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec2LinePointDistance(float2 linePoint1, float2 linePoint2, float2 point)
    {
        vfloat4 lv0 = vLoad(&linePoint1);
        vfloat4 lv1 = vLoad(&linePoint2);
        vfloat4 lv2 = vLoad(&point);
        vfloat4 vRet = v2LinePointDistance(lv0, lv1, lv2);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2IntersectLine(float2 line1Point1, float2 line1Point2, float2 line2Point1, float2 line2Point2)
    {
        vfloat4 lv0 = vLoad(&line1Point1);
        vfloat4 lv1 = vLoad(&line1Point2);
        vfloat4 lv2 = vLoad(&line2Point1);
        vfloat4 lv3 = vLoad(&line2Point2);
        vfloat4 vRet = v2IntersectLine(lv0, lv1, lv2, lv3);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool Vec3AllEqual(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllEqual(lv0, lv1);
    }

    inline bool Vec3AllBinaryEqual(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllBinaryEqual(lv0, lv1);
    }

    inline bool Vec3AllNearEqual(float3 v1, float3 v2, float3 epsilon)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&epsilon);
        return v3AllNearEqual(lv0, lv1, lv2);
    }

    inline bool Vec3AnyNotEqual(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AnyNotEqual(lv0, lv1);
    }

    inline bool Vec3AnyBinaryNotEqual(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AnyBinaryNotEqual(lv0, lv1);
    }

    inline bool Vec3AllGreater(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllGreater(lv0, lv1);
    }

    inline bool Vec3AllGreaterEqual(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllGreaterEqual(lv0, lv1);
    }

    inline bool Vec3AllLess(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllLess(lv0, lv1);
    }

    inline bool Vec3AllLessEqual(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllLessEqual(lv0, lv1);
    }

    inline bool Vec3AllInBoundsAbs(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v3AllInBoundsAbs(lv0, lv1);
    }

    inline bool Vec3AnyIsNaN(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        return v3AnyIsNaN(lv0);
    }

    inline bool Vec3AnyIsInfinite(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        return v3AnyIsInfinite(lv0);
    }

    inline float Vec3Dot(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v3Dot(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Cross(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v3Cross(lv0, lv1);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3LengthSq(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3LengthSq(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3RLengthEst(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3RLengthEst(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3RLength(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3RLength(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3LengthEst(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3LengthEst(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3Length(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3Length(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3NormalizeEst(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3NormalizeEst(lv0);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Normalize(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3Normalize(lv0);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ClampLength(float3 v, float lengthMin, float lengthMax)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3ClampLength(lv0, lengthMin, lengthMax);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ClampLength(float3 v, float3 lengthMin, float3 lengthMax)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&lengthMin);
        vfloat4 lv2 = vLoad(&lengthMax);
        vfloat4 vRet = v3ClampLength(lv0, lv1, lv2);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Reflect(float3 incident, float3 normal)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = v3Reflect(lv0, lv1);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Refract(float3 incident, float3 normal, float refractionIndex)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = v3Refract(lv0, lv1, refractionIndex);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Refract(float3 incident, float3 normal, float3 refractionIndex)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 lv2 = vLoad(&refractionIndex);
        vfloat4 vRet = v3Refract(lv0, lv1, lv2);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Orthogonal(float3 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v3Orthogonal(lv0);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3AngleBetweenNormalsEst(float3 n1, float3 n2)
    {
        vfloat4 lv0 = vLoad(&n1);
        vfloat4 lv1 = vLoad(&n2);
        vfloat4 vRet = v3AngleBetweenNormalsEst(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3AngleBetweenNormals(float3 n1, float3 n2)
    {
        vfloat4 lv0 = vLoad(&n1);
        vfloat4 lv1 = vLoad(&n2);
        vfloat4 vRet = v3AngleBetweenNormals(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3AngleBetweenVectors(float3 v1, float3 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v3AngleBetweenVectors(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec3LinePointDistance(float3 linePoint1, float3 linePoint2, float3 point)
    {
        vfloat4 lv0 = vLoad(&linePoint1);
        vfloat4 lv1 = vLoad(&linePoint2);
        vfloat4 lv2 = vLoad(&point);
        vfloat4 vRet = v3LinePointDistance(lv0, lv1, lv2);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline void Vec3ComponentsFromNormal(float3* pParallel, float3* pPerpendicular, float3 v, float3 normal)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 outV0;
        vfloat4 outV1;
        v3ComponentsFromNormal(&outV0, &outV1, lv0, lv1);
        vStore(pParallel, outV0);
        vStore(pPerpendicular, outV1);
    }

    inline float3 Vec3TransformPoint(float3 v, const Matrix4x4& m)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = v3TransformPoint(lv0, lm0);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3TransformPointDivideW(float3 v, const Matrix4x4& m)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = v3TransformPointDivideW(lv0, lm0);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3TransformNormal(float3 v, const Matrix4x4& m)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = v3TransformNormal(lv0, lm0);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Project(float3 v, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ, const Matrix4x4& projection, const Matrix4x4& view, const Matrix4x4& world)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4x4 lm0 = vLoad(&projection);
        vfloat4x4 lm1 = vLoad(&view);
        vfloat4x4 lm2 = vLoad(&world);
        vfloat4 vRet = v3Project(lv0, viewportX, viewportY, viewportWidth, viewportHeight, viewportMinZ, viewportMaxZ, lm0, lm1, lm2);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Unproject(float3 v, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ, const Matrix4x4& projection, const Matrix4x4& view, const Matrix4x4& world)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4x4 lm0 = vLoad(&projection);
        vfloat4x4 lm1 = vLoad(&view);
        vfloat4x4 lm2 = vLoad(&world);
        vfloat4 vRet = v3Unproject(lv0, viewportX, viewportY, viewportWidth, viewportHeight, viewportMinZ, viewportMaxZ, lm0, lm1, lm2);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool Vec4AllEqual(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllEqual(lv0, lv1);
    }

    inline bool Vec4AllBinaryEqual(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllBinaryEqual(lv0, lv1);
    }

    inline bool Vec4AllNearEqual(float4 v1, float4 v2, float4 epsilon)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&epsilon);
        return v4AllNearEqual(lv0, lv1, lv2);
    }

    inline bool Vec4AnyNotEqual(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AnyNotEqual(lv0, lv1);
    }

    inline bool Vec4AnyBinaryNotEqual(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AnyBinaryNotEqual(lv0, lv1);
    }

    inline bool Vec4AllGreater(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllGreater(lv0, lv1);
    }

    inline bool Vec4AllGreaterEqual(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllGreaterEqual(lv0, lv1);
    }

    inline bool Vec4AllLess(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllLess(lv0, lv1);
    }

    inline bool Vec4AllLessEqual(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllLessEqual(lv0, lv1);
    }

    inline bool Vec4AllInBoundsAbs(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        return v4AllInBoundsAbs(lv0, lv1);
    }

    inline bool Vec4AnyIsNaN(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        return v4AnyIsNaN(lv0);
    }

    inline bool Vec4AnyIsInfinite(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        return v4AnyIsInfinite(lv0);
    }

    inline float Vec4Dot(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4Dot(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Cross(float4 v1, float4 v2, float4 v3)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 lv2 = vLoad(&v3);
        vfloat4 vRet = v4Cross(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4LengthSq(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4LengthSq(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4RLengthEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RLengthEst(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4RLength(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4RLength(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4LengthEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4LengthEst(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4Length(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Length(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4NormalizeEst(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4NormalizeEst(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Normalize(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Normalize(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ClampLength(float4 v, float lengthMin, float lengthMax)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4ClampLength(lv0, lengthMin, lengthMax);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4ClampLength(float4 v, float4 lengthMin, float4 lengthMax)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 lv1 = vLoad(&lengthMin);
        vfloat4 lv2 = vLoad(&lengthMax);
        vfloat4 vRet = v4ClampLength(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Reflect(float4 incident, float4 normal)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = v4Reflect(lv0, lv1);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Refract(float4 incident, float4 normal, float refractionIndex)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 vRet = v4Refract(lv0, lv1, refractionIndex);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Refract(float4 incident, float4 normal, float4 refractionIndex)
    {
        vfloat4 lv0 = vLoad(&incident);
        vfloat4 lv1 = vLoad(&normal);
        vfloat4 lv2 = vLoad(&refractionIndex);
        vfloat4 vRet = v4Refract(lv0, lv1, lv2);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Orthogonal(float4 v)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4 vRet = v4Orthogonal(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4AngleBetweenNormalsEst(float4 n1, float4 n2)
    {
        vfloat4 lv0 = vLoad(&n1);
        vfloat4 lv1 = vLoad(&n2);
        vfloat4 vRet = v4AngleBetweenNormalsEst(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4AngleBetweenNormals(float4 n1, float4 n2)
    {
        vfloat4 lv0 = vLoad(&n1);
        vfloat4 lv1 = vLoad(&n2);
        vfloat4 vRet = v4AngleBetweenNormals(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float Vec4AngleBetweenVectors(float4 v1, float4 v2)
    {
        vfloat4 lv0 = vLoad(&v1);
        vfloat4 lv1 = vLoad(&v2);
        vfloat4 vRet = v4AngleBetweenVectors(lv0, lv1);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 Vec4Transform(float4 v, const Matrix4x4& m)
    {
        vfloat4 lv0 = vLoad(&v);
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = v4Transform(lv0, lm0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatMul(Quaternion q1, Quaternion q2)
    {
        vfloat4 lv0 = vLoad(&q1);
        vfloat4 lv1 = vLoad(&q2);
        vfloat4 vRet = vQuatMul(lv0, lv1);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float QuatLengthSq(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatLengthSq(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float QuatRLength(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatRLength(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float QuatLength(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatLength(lv0);
        float ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatNormalizeEst(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatNormalizeEst(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatNormalize(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatNormalize(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatConjugate(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatConjugate(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatInverse(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatInverse(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatLn(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatLn(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatExp(Quaternion q)
    {
        vfloat4 lv0 = vLoad(&q);
        vfloat4 vRet = vQuatExp(lv0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatSlerp(Quaternion q0, Quaternion q1, float t)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 vRet = vQuatSlerp(lv0, lv1, t);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatSquad(Quaternion q0, Quaternion q1, Quaternion q2, Quaternion q3, float t)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 lv2 = vLoad(&q2);
        vfloat4 lv3 = vLoad(&q3);
        vfloat4 vRet = vQuatSquad(lv0, lv1, lv2, lv3, t);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline void QuatSquadSetup(Quaternion* pA, Quaternion* pB, Quaternion* pC, Quaternion q0, Quaternion q1, Quaternion q2, Quaternion q3)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 lv2 = vLoad(&q2);
        vfloat4 lv3 = vLoad(&q3);
        vfloat4 outV0;
        vfloat4 outV1;
        vfloat4 outV2;
        vQuatSquadSetup(&outV0, &outV1, &outV2, lv0, lv1, lv2, lv3);
        vStore(pA, outV0);
        vStore(pB, outV1);
        vStore(pC, outV2);
    }

    inline Quaternion QuatBaryCentric(Quaternion q0, Quaternion q1, Quaternion q2, float f, float g)
    {
        vfloat4 lv0 = vLoad(&q0);
        vfloat4 lv1 = vLoad(&q1);
        vfloat4 lv2 = vLoad(&q2);
        vfloat4 vRet = vQuatBaryCentric(lv0, lv1, lv2, f, g);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatRotationRollPitchYaw(float pitch, float yaw, float roll)
    {
        vfloat4 vRet = vQuatRotationRollPitchYaw(pitch, yaw, roll);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline Quaternion QuatRotationMatrix(const Matrix4x4& m)
    {
        vfloat4x4 lm0 = vLoad(&m);
        vfloat4 vRet = vQuatRotationMatrix(lm0);
        Quaternion ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorNegative(float4 c)
    {
        vfloat4 lv0 = vLoad(&c);
        vfloat4 vRet = vColorNegative(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorAdjustSaturation(float4 c, float saturation)
    {
        vfloat4 lv0 = vLoad(&c);
        vfloat4 vRet = vColorAdjustSaturation(lv0, saturation);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorAdjustContrast(float4 c, float contrast)
    {
        vfloat4 lv0 = vLoad(&c);
        vfloat4 vRet = vColorAdjustContrast(lv0, contrast);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToHSL(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToHSL(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorHSLToRGB(float4 hsl)
    {
        vfloat4 lv0 = vLoad(&hsl);
        vfloat4 vRet = vColorHSLToRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToHSV(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToHSV(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorHSVToRGB(float4 hsv)
    {
        vfloat4 lv0 = vLoad(&hsv);
        vfloat4 vRet = vColorHSVToRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToYUV(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToYUV(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorYUVToRGB(float4 yuv)
    {
        vfloat4 lv0 = vLoad(&yuv);
        vfloat4 vRet = vColorYUVToRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToYUV_HD(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToYUV_HD(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorYUVToRGB_HD(float4 yuv)
    {
        vfloat4 lv0 = vLoad(&yuv);
        vfloat4 vRet = vColorYUVToRGB_HD(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToYUV_UHD(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToYUV_UHD(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorYUVToRGB_UHD(float4 yuv)
    {
        vfloat4 lv0 = vLoad(&yuv);
        vfloat4 vRet = vColorYUVToRGB_UHD(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToXYZ(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToXYZ(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorXYZToRGB(float4 xyz)
    {
        vfloat4 lv0 = vLoad(&xyz);
        vfloat4 vRet = vColorXYZToRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorXYZToSRGB(float4 xyz)
    {
        vfloat4 lv0 = vLoad(&xyz);
        vfloat4 vRet = vColorXYZToSRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorSRGBToXYZ(float4 srgb)
    {
        vfloat4 lv0 = vLoad(&srgb);
        vfloat4 vRet = vColorSRGBToXYZ(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorRGBToSRGB(float4 rgb)
    {
        vfloat4 lv0 = vLoad(&rgb);
        vfloat4 vRet = vColorRGBToSRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float4 ColorSRGBToRGB(float4 srgb)
    {
        vfloat4 lv0 = vLoad(&srgb);
        vfloat4 vRet = vColorSRGBToRGB(lv0);
        float4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Round(float3 v)
    {
        vfloat4 vRet = v4Round(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Truncate(float3 v)
    {
        vfloat4 vRet = v4Truncate(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Floor(float3 v)
    {
        vfloat4 vRet = v4Floor(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Ceiling(float3 v)
    {
        vfloat4 vRet = v4Ceiling(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Clamp01(float3 v)
    {
        vfloat4 vRet = v4Clamp01(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Negate(float3 v)
    {
        vfloat4 vRet = v4Negate(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3RcpEst(float3 v)
    {
        vfloat4 vRet = v4RcpEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Rcp(float3 v)
    {
        vfloat4 vRet = v4Rcp(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Sqrt(float3 v)
    {
        vfloat4 vRet = v4Sqrt(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3SqrtEst(float3 v)
    {
        vfloat4 vRet = v4SqrtEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3RSqrtEst(float3 v)
    {
        vfloat4 vRet = v4RSqrtEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3RSqrt(float3 v)
    {
        vfloat4 vRet = v4RSqrt(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Exp2(float3 v)
    {
        vfloat4 vRet = v4Exp2(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Exp10(float3 v)
    {
        vfloat4 vRet = v4Exp10(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ExpE(float3 v)
    {
        vfloat4 vRet = v4ExpE(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Log2(float3 v)
    {
        vfloat4 vRet = v4Log2(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Log10(float3 v)
    {
        vfloat4 vRet = v4Log10(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3LogE(float3 v)
    {
        vfloat4 vRet = v4LogE(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Abs(float3 v)
    {
        vfloat4 vRet = v4Abs(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ModAngles(float3 v)
    {
        vfloat4 vRet = v4ModAngles(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Sin(float3 v)
    {
        vfloat4 vRet = v4Sin(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3SinEst(float3 v)
    {
        vfloat4 vRet = v4SinEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Cos(float3 v)
    {
        vfloat4 vRet = v4Cos(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3CosEst(float3 v)
    {
        vfloat4 vRet = v4CosEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Tan(float3 v)
    {
        vfloat4 vRet = v4Tan(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3TanEst(float3 v)
    {
        vfloat4 vRet = v4TanEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3SinH(float3 v)
    {
        vfloat4 vRet = v4SinH(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3CosH(float3 v)
    {
        vfloat4 vRet = v4CosH(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3TanH(float3 v)
    {
        vfloat4 vRet = v4TanH(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ASin(float3 v)
    {
        vfloat4 vRet = v4ASin(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ASinEst(float3 v)
    {
        vfloat4 vRet = v4ASinEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ACos(float3 v)
    {
        vfloat4 vRet = v4ACos(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ACosEst(float3 v)
    {
        vfloat4 vRet = v4ACosEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ATan(float3 v)
    {
        vfloat4 vRet = v4ATan(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ATanEst(float3 v)
    {
        vfloat4 vRet = v4ATanEst(vLoad(&v));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Min(float3 a, float3 b)
    {
        vfloat4 vRet = v4Min(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Max(float3 a, float3 b)
    {
        vfloat4 vRet = v4Max(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3And(float3 a, float3 b)
    {
        vfloat4 vRet = v4And(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3AndNot(float3 a, float3 b)
    {
        vfloat4 vRet = v4AndNot(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Or(float3 a, float3 b)
    {
        vfloat4 vRet = v4Or(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Nor(float3 a, float3 b)
    {
        vfloat4 vRet = v4Nor(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Xor(float3 a, float3 b)
    {
        vfloat4 vRet = v4Xor(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Add(float3 a, float3 b)
    {
        vfloat4 vRet = v4Add(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3AddAngles(float3 a, float3 b)
    {
        vfloat4 vRet = v4AddAngles(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Sub(float3 a, float3 b)
    {
        vfloat4 vRet = v4Sub(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Mul(float3 a, float3 b)
    {
        vfloat4 vRet = v4Mul(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Div(float3 a, float3 b)
    {
        vfloat4 vRet = v4Div(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Pow(float3 a, float3 b)
    {
        vfloat4 vRet = v4Pow(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Mod(float3 a, float3 b)
    {
        vfloat4 vRet = v4Mod(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ATan2(float3 a, float3 b)
    {
        vfloat4 vRet = v4ATan2(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3ATan2Est(float3 a, float3 b)
    {
        vfloat4 vRet = v4ATan2Est(vLoad(&a), vLoad(&b));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Clamp(float3 a, float3 b, float3 c)
    {
        vfloat4 vRet = v4Clamp(vLoad(&a), vLoad(&b), vLoad(&c));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3MulAdd(float3 a, float3 b, float3 c)
    {
        vfloat4 vRet = v4MulAdd(vLoad(&a), vLoad(&b), vLoad(&c));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3NegateMulAdd(float3 a, float3 b, float3 c)
    {
        vfloat4 vRet = v4NegateMulAdd(vLoad(&a), vLoad(&b), vLoad(&c));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Lerp(float3 a, float3 b, float3 c)
    {
        vfloat4 vRet = v4Lerp(vLoad(&a), vLoad(&b), vLoad(&c));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Lerp(float3 a, float3 b, float c)
    {
        vfloat4 vRet = v4Lerp(vLoad(&a), vLoad(&b), c);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Hermite(float3 a, float3 b, float3 c, float3 d, float3 e)
    {
        vfloat4 vRet = v4Hermite(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), vLoad(&e));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3CatmullRom(float3 a, float3 b, float3 c, float3 d, float3 e)
    {
        vfloat4 vRet = v4CatmullRom(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), vLoad(&e));
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3Hermite(float3 a, float3 b, float3 c, float3 d, float e)
    {
        vfloat4 vRet = v4Hermite(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), e);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float3 Vec3CatmullRom(float3 a, float3 b, float3 c, float3 d, float e)
    {
        vfloat4 vRet = v4CatmullRom(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), e);
        float3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Round(float2 v)
    {
        vfloat4 vRet = v4Round(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Truncate(float2 v)
    {
        vfloat4 vRet = v4Truncate(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Floor(float2 v)
    {
        vfloat4 vRet = v4Floor(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Ceiling(float2 v)
    {
        vfloat4 vRet = v4Ceiling(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Clamp01(float2 v)
    {
        vfloat4 vRet = v4Clamp01(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Negate(float2 v)
    {
        vfloat4 vRet = v4Negate(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2RcpEst(float2 v)
    {
        vfloat4 vRet = v4RcpEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Rcp(float2 v)
    {
        vfloat4 vRet = v4Rcp(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Sqrt(float2 v)
    {
        vfloat4 vRet = v4Sqrt(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2SqrtEst(float2 v)
    {
        vfloat4 vRet = v4SqrtEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2RSqrtEst(float2 v)
    {
        vfloat4 vRet = v4RSqrtEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2RSqrt(float2 v)
    {
        vfloat4 vRet = v4RSqrt(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Exp2(float2 v)
    {
        vfloat4 vRet = v4Exp2(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Exp10(float2 v)
    {
        vfloat4 vRet = v4Exp10(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ExpE(float2 v)
    {
        vfloat4 vRet = v4ExpE(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Log2(float2 v)
    {
        vfloat4 vRet = v4Log2(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Log10(float2 v)
    {
        vfloat4 vRet = v4Log10(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2LogE(float2 v)
    {
        vfloat4 vRet = v4LogE(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Abs(float2 v)
    {
        vfloat4 vRet = v4Abs(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ModAngles(float2 v)
    {
        vfloat4 vRet = v4ModAngles(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Sin(float2 v)
    {
        vfloat4 vRet = v4Sin(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2SinEst(float2 v)
    {
        vfloat4 vRet = v4SinEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Cos(float2 v)
    {
        vfloat4 vRet = v4Cos(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2CosEst(float2 v)
    {
        vfloat4 vRet = v4CosEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Tan(float2 v)
    {
        vfloat4 vRet = v4Tan(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2TanEst(float2 v)
    {
        vfloat4 vRet = v4TanEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2SinH(float2 v)
    {
        vfloat4 vRet = v4SinH(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2CosH(float2 v)
    {
        vfloat4 vRet = v4CosH(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2TanH(float2 v)
    {
        vfloat4 vRet = v4TanH(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ASin(float2 v)
    {
        vfloat4 vRet = v4ASin(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ASinEst(float2 v)
    {
        vfloat4 vRet = v4ASinEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ACos(float2 v)
    {
        vfloat4 vRet = v4ACos(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ACosEst(float2 v)
    {
        vfloat4 vRet = v4ACosEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ATan(float2 v)
    {
        vfloat4 vRet = v4ATan(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ATanEst(float2 v)
    {
        vfloat4 vRet = v4ATanEst(vLoad(&v));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Min(float2 a, float2 b)
    {
        vfloat4 vRet = v4Min(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Max(float2 a, float2 b)
    {
        vfloat4 vRet = v4Max(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2And(float2 a, float2 b)
    {
        vfloat4 vRet = v4And(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2AndNot(float2 a, float2 b)
    {
        vfloat4 vRet = v4AndNot(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Or(float2 a, float2 b)
    {
        vfloat4 vRet = v4Or(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Nor(float2 a, float2 b)
    {
        vfloat4 vRet = v4Nor(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Xor(float2 a, float2 b)
    {
        vfloat4 vRet = v4Xor(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Add(float2 a, float2 b)
    {
        vfloat4 vRet = v4Add(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2AddAngles(float2 a, float2 b)
    {
        vfloat4 vRet = v4AddAngles(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Sub(float2 a, float2 b)
    {
        vfloat4 vRet = v4Sub(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Mul(float2 a, float2 b)
    {
        vfloat4 vRet = v4Mul(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Div(float2 a, float2 b)
    {
        vfloat4 vRet = v4Div(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Pow(float2 a, float2 b)
    {
        vfloat4 vRet = v4Pow(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Mod(float2 a, float2 b)
    {
        vfloat4 vRet = v4Mod(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ATan2(float2 a, float2 b)
    {
        vfloat4 vRet = v4ATan2(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2ATan2Est(float2 a, float2 b)
    {
        vfloat4 vRet = v4ATan2Est(vLoad(&a), vLoad(&b));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Clamp(float2 a, float2 b, float2 c)
    {
        vfloat4 vRet = v4Clamp(vLoad(&a), vLoad(&b), vLoad(&c));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2MulAdd(float2 a, float2 b, float2 c)
    {
        vfloat4 vRet = v4MulAdd(vLoad(&a), vLoad(&b), vLoad(&c));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2NegateMulAdd(float2 a, float2 b, float2 c)
    {
        vfloat4 vRet = v4NegateMulAdd(vLoad(&a), vLoad(&b), vLoad(&c));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Lerp(float2 a, float2 b, float2 c)
    {
        vfloat4 vRet = v4Lerp(vLoad(&a), vLoad(&b), vLoad(&c));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Lerp(float2 a, float2 b, float c)
    {
        vfloat4 vRet = v4Lerp(vLoad(&a), vLoad(&b), c);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Hermite(float2 a, float2 b, float2 c, float2 d, float2 e)
    {
        vfloat4 vRet = v4Hermite(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), vLoad(&e));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2CatmullRom(float2 a, float2 b, float2 c, float2 d, float2 e)
    {
        vfloat4 vRet = v4CatmullRom(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), vLoad(&e));
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2Hermite(float2 a, float2 b, float2 c, float2 d, float e)
    {
        vfloat4 vRet = v4Hermite(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), e);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline float2 Vec2CatmullRom(float2 a, float2 b, float2 c, float2 d, float e)
    {
        vfloat4 vRet = v4CatmullRom(vLoad(&a), vLoad(&b), vLoad(&c), vLoad(&d), e);
        float2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool4 Vec4Equal(float4 a, float4 b)
    {
        vbool4 vRet = v4CompressMask128(v4Equal(vLoad(&a), vLoad(&b)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool4 Vec4NotEqual(float4 a, float4 b)
    {
        vbool4 vRet = v4CompressMask128(v4NotEqual(vLoad(&a), vLoad(&b)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool4 Vec4Less(float4 a, float4 b)
    {
        vbool4 vRet = v4CompressMask128(v4Less(vLoad(&a), vLoad(&b)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool4 Vec4LessEqual(float4 a, float4 b)
    {
        vbool4 vRet = v4CompressMask128(v4LessEqual(vLoad(&a), vLoad(&b)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool4 Vec4Greater(float4 a, float4 b)
    {
        vbool4 vRet = v4CompressMask128(v4Greater(vLoad(&a), vLoad(&b)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool4 Vec4GreaterEqual(float4 a, float4 b)
    {
        vbool4 vRet = v4CompressMask128(v4GreaterEqual(vLoad(&a), vLoad(&b)));
        bool4 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3Equal(float3 a, float3 b)
    {
        vbool4 vRet = v4CompressMask128(v4Equal(vLoad(&a), vLoad(&b)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3NotEqual(float3 a, float3 b)
    {
        vbool4 vRet = v4CompressMask128(v4NotEqual(vLoad(&a), vLoad(&b)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3Less(float3 a, float3 b)
    {
        vbool4 vRet = v4CompressMask128(v4Less(vLoad(&a), vLoad(&b)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3LessEqual(float3 a, float3 b)
    {
        vbool4 vRet = v4CompressMask128(v4LessEqual(vLoad(&a), vLoad(&b)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3Greater(float3 a, float3 b)
    {
        vbool4 vRet = v4CompressMask128(v4Greater(vLoad(&a), vLoad(&b)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool3 Vec3GreaterEqual(float3 a, float3 b)
    {
        vbool4 vRet = v4CompressMask128(v4GreaterEqual(vLoad(&a), vLoad(&b)));
        bool3 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2Equal(float2 a, float2 b)
    {
        vbool4 vRet = v4CompressMask128(v4Equal(vLoad(&a), vLoad(&b)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2NotEqual(float2 a, float2 b)
    {
        vbool4 vRet = v4CompressMask128(v4NotEqual(vLoad(&a), vLoad(&b)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2Less(float2 a, float2 b)
    {
        vbool4 vRet = v4CompressMask128(v4Less(vLoad(&a), vLoad(&b)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2LessEqual(float2 a, float2 b)
    {
        vbool4 vRet = v4CompressMask128(v4LessEqual(vLoad(&a), vLoad(&b)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2Greater(float2 a, float2 b)
    {
        vbool4 vRet = v4CompressMask128(v4Greater(vLoad(&a), vLoad(&b)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline bool2 Vec2GreaterEqual(float2 a, float2 b)
    {
        vbool4 vRet = v4CompressMask128(v4GreaterEqual(vLoad(&a), vLoad(&b)));
        bool2 ret;
        vStore(&ret, vRet);
        return ret;
    }

    inline int2 Vec2Add(int2 l, int2 r)
    {
        l.x = l.x + r.x;
        l.y = l.y + r.y;
        return l;
    }

    inline int2 Vec2Sub(int2 l, int2 r)
    {
        l.x = l.x - r.x;
        l.y = l.y - r.y;
        return l;
    }

    inline int2 Vec2Mul(int2 l, int2 r)
    {
        l.x = l.x * r.x;
        l.y = l.y * r.y;
        return l;
    }

    inline int2 Vec2Div(int2 l, int2 r)
    {
        l.x = l.x / r.x;
        l.y = l.y / r.y;
        return l;
    }

    inline int2 Vec2ShiftLeft(int2 l, int2 r)
    {
        l.x = l.x << r.x;
        l.y = l.y << r.y;
        return l;
    }

    inline int2 Vec2ShiftRight(int2 l, int2 r)
    {
        l.x = l.x >> r.x;
        l.y = l.y >> r.y;
        return l;
    }

    inline int2 Vec2And(int2 l, int2 r)
    {
        l.x = l.x & r.x;
        l.y = l.y & r.y;
        return l;
    }

    inline int2 Vec2Or(int2 l, int2 r)
    {
        l.x = l.x | r.x;
        l.y = l.y | r.y;
        return l;
    }

    inline int2 Vec2Xor(int2 l, int2 r)
    {
        l.x = l.x ^ r.x;
        l.y = l.y ^ r.y;
        return l;
    }

    inline int2 Vec2Mod(int2 l, int2 r)
    {
        l.x = l.x % r.x;
        l.y = l.y % r.y;
        return l;
    }

    inline int2 Vec2Not(int2 r)
    {
        r.x = ~r.x;
        r.y = ~r.y;
        return r;
    }

    inline int2 Vec2Negate(int2 r)
    {
        r.x = -r.x;
        r.y = -r.y;
        return r;
    }

    inline int2 Vec2Decrement(int2 r)
    {
        --r.x;
        --r.y;
        return r;
    }

    inline int2 Vec2Increment(int2 r)
    {
        ++r.x;
        ++r.y;
        return r;
    }

    inline bool2 Vec2Equal(int2 l, int2 r)
    {
        bool2 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        return ret;
    }

    inline bool2 Vec2NotEqual(int2 l, int2 r)
    {
        bool2 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        return ret;
    }

    inline bool2 Vec2Less(int2 l, int2 r)
    {
        bool2 ret;
        ret.x = l.x < r.x;
        ret.y = l.y < r.y;
        return ret;
    }

    inline bool2 Vec2LessEqual(int2 l, int2 r)
    {
        bool2 ret;
        ret.x = l.x <= r.x;
        ret.y = l.y <= r.y;
        return ret;
    }

    inline bool2 Vec2Greater(int2 l, int2 r)
    {
        bool2 ret;
        ret.x = l.x > r.x;
        ret.y = l.y > r.y;
        return ret;
    }

    inline bool2 Vec2GreaterEqual(int2 l, int2 r)
    {
        bool2 ret;
        ret.x = l.x >= r.x;
        ret.y = l.y >= r.y;
        return ret;
    }

    inline int3 Vec3Add(int3 l, int3 r)
    {
        l.x = l.x + r.x;
        l.y = l.y + r.y;
        l.z = l.z + r.z;
        return l;
    }

    inline int3 Vec3Sub(int3 l, int3 r)
    {
        l.x = l.x - r.x;
        l.y = l.y - r.y;
        l.z = l.z - r.z;
        return l;
    }

    inline int3 Vec3Mul(int3 l, int3 r)
    {
        l.x = l.x * r.x;
        l.y = l.y * r.y;
        l.z = l.z * r.z;
        return l;
    }

    inline int3 Vec3Div(int3 l, int3 r)
    {
        l.x = l.x / r.x;
        l.y = l.y / r.y;
        l.z = l.z / r.z;
        return l;
    }

    inline int3 Vec3ShiftLeft(int3 l, int3 r)
    {
        l.x = l.x << r.x;
        l.y = l.y << r.y;
        l.z = l.z << r.z;
        return l;
    }

    inline int3 Vec3ShiftRight(int3 l, int3 r)
    {
        l.x = l.x >> r.x;
        l.y = l.y >> r.y;
        l.z = l.z >> r.z;
        return l;
    }

    inline int3 Vec3And(int3 l, int3 r)
    {
        l.x = l.x & r.x;
        l.y = l.y & r.y;
        l.z = l.z & r.z;
        return l;
    }

    inline int3 Vec3Or(int3 l, int3 r)
    {
        l.x = l.x | r.x;
        l.y = l.y | r.y;
        l.z = l.z | r.z;
        return l;
    }

    inline int3 Vec3Xor(int3 l, int3 r)
    {
        l.x = l.x ^ r.x;
        l.y = l.y ^ r.y;
        l.z = l.z ^ r.z;
        return l;
    }

    inline int3 Vec3Mod(int3 l, int3 r)
    {
        l.x = l.x % r.x;
        l.y = l.y % r.y;
        l.z = l.z % r.z;
        return l;
    }

    inline int3 Vec3Not(int3 r)
    {
        r.x = ~r.x;
        r.y = ~r.y;
        r.z = ~r.z;
        return r;
    }

    inline int3 Vec3Negate(int3 r)
    {
        r.x = -r.x;
        r.y = -r.y;
        r.z = -r.z;
        return r;
    }

    inline int3 Vec3Decrement(int3 r)
    {
        --r.x;
        --r.y;
        --r.z;
        return r;
    }

    inline int3 Vec3Increment(int3 r)
    {
        ++r.x;
        ++r.y;
        ++r.z;
        return r;
    }

    inline bool3 Vec3Equal(int3 l, int3 r)
    {
        bool3 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        ret.z = l.z == r.z;
        return ret;
    }

    inline bool3 Vec3NotEqual(int3 l, int3 r)
    {
        bool3 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        ret.z = l.z != r.z;
        return ret;
    }

    inline bool3 Vec3Less(int3 l, int3 r)
    {
        bool3 ret;
        ret.x = l.x < r.x;
        ret.y = l.y < r.y;
        ret.z = l.z < r.z;
        return ret;
    }

    inline bool3 Vec3LessEqual(int3 l, int3 r)
    {
        bool3 ret;
        ret.x = l.x <= r.x;
        ret.y = l.y <= r.y;
        ret.z = l.z <= r.z;
        return ret;
    }

    inline bool3 Vec3Greater(int3 l, int3 r)
    {
        bool3 ret;
        ret.x = l.x > r.x;
        ret.y = l.y > r.y;
        ret.z = l.z > r.z;
        return ret;
    }

    inline bool3 Vec3GreaterEqual(int3 l, int3 r)
    {
        bool3 ret;
        ret.x = l.x >= r.x;
        ret.y = l.y >= r.y;
        ret.z = l.z >= r.z;
        return ret;
    }

    inline int4 Vec4Add(int4 l, int4 r)
    {
        l.x = l.x + r.x;
        l.y = l.y + r.y;
        l.z = l.z + r.z;
        l.w = l.w + r.w;
        return l;
    }

    inline int4 Vec4Sub(int4 l, int4 r)
    {
        l.x = l.x - r.x;
        l.y = l.y - r.y;
        l.z = l.z - r.z;
        l.w = l.w - r.w;
        return l;
    }

    inline int4 Vec4Mul(int4 l, int4 r)
    {
        l.x = l.x * r.x;
        l.y = l.y * r.y;
        l.z = l.z * r.z;
        l.w = l.w * r.w;
        return l;
    }

    inline int4 Vec4Div(int4 l, int4 r)
    {
        l.x = l.x / r.x;
        l.y = l.y / r.y;
        l.z = l.z / r.z;
        l.w = l.w / r.w;
        return l;
    }

    inline int4 Vec4ShiftLeft(int4 l, int4 r)
    {
        l.x = l.x << r.x;
        l.y = l.y << r.y;
        l.z = l.z << r.z;
        l.w = l.w << r.w;
        return l;
    }

    inline int4 Vec4ShiftRight(int4 l, int4 r)
    {
        l.x = l.x >> r.x;
        l.y = l.y >> r.y;
        l.z = l.z >> r.z;
        l.w = l.w >> r.w;
        return l;
    }

    inline int4 Vec4And(int4 l, int4 r)
    {
        l.x = l.x & r.x;
        l.y = l.y & r.y;
        l.z = l.z & r.z;
        l.w = l.w & r.w;
        return l;
    }

    inline int4 Vec4Or(int4 l, int4 r)
    {
        l.x = l.x | r.x;
        l.y = l.y | r.y;
        l.z = l.z | r.z;
        l.w = l.w | r.w;
        return l;
    }

    inline int4 Vec4Xor(int4 l, int4 r)
    {
        l.x = l.x ^ r.x;
        l.y = l.y ^ r.y;
        l.z = l.z ^ r.z;
        l.w = l.w ^ r.w;
        return l;
    }

    inline int4 Vec4Mod(int4 l, int4 r)
    {
        l.x = l.x % r.x;
        l.y = l.y % r.y;
        l.z = l.z % r.z;
        l.w = l.w % r.w;
        return l;
    }

    inline int4 Vec4Not(int4 r)
    {
        r.x = ~r.x;
        r.y = ~r.y;
        r.z = ~r.z;
        r.w = ~r.w;
        return r;
    }

    inline int4 Vec4Negate(int4 r)
    {
        r.x = -r.x;
        r.y = -r.y;
        r.z = -r.z;
        r.w = -r.w;
        return r;
    }

    inline int4 Vec4Decrement(int4 r)
    {
        --r.x;
        --r.y;
        --r.z;
        --r.w;
        return r;
    }

    inline int4 Vec4Increment(int4 r)
    {
        ++r.x;
        ++r.y;
        ++r.z;
        ++r.w;
        return r;
    }

    inline bool4 Vec4Equal(int4 l, int4 r)
    {
        bool4 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        ret.z = l.z == r.z;
        ret.w = l.w == r.w;
        return ret;
    }

    inline bool4 Vec4NotEqual(int4 l, int4 r)
    {
        bool4 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        ret.z = l.z != r.z;
        ret.w = l.w != r.w;
        return ret;
    }

    inline bool4 Vec4Less(int4 l, int4 r)
    {
        bool4 ret;
        ret.x = l.x < r.x;
        ret.y = l.y < r.y;
        ret.z = l.z < r.z;
        ret.w = l.w < r.w;
        return ret;
    }

    inline bool4 Vec4LessEqual(int4 l, int4 r)
    {
        bool4 ret;
        ret.x = l.x <= r.x;
        ret.y = l.y <= r.y;
        ret.z = l.z <= r.z;
        ret.w = l.w <= r.w;
        return ret;
    }

    inline bool4 Vec4Greater(int4 l, int4 r)
    {
        bool4 ret;
        ret.x = l.x > r.x;
        ret.y = l.y > r.y;
        ret.z = l.z > r.z;
        ret.w = l.w > r.w;
        return ret;
    }

    inline bool4 Vec4GreaterEqual(int4 l, int4 r)
    {
        bool4 ret;
        ret.x = l.x >= r.x;
        ret.y = l.y >= r.y;
        ret.z = l.z >= r.z;
        ret.w = l.w >= r.w;
        return ret;
    }

    inline uint2 Vec2Add(uint2 l, uint2 r)
    {
        l.x = l.x + r.x;
        l.y = l.y + r.y;
        return l;
    }

    inline uint2 Vec2Sub(uint2 l, uint2 r)
    {
        l.x = l.x - r.x;
        l.y = l.y - r.y;
        return l;
    }

    inline uint2 Vec2Mul(uint2 l, uint2 r)
    {
        l.x = l.x * r.x;
        l.y = l.y * r.y;
        return l;
    }

    inline uint2 Vec2Div(uint2 l, uint2 r)
    {
        l.x = l.x / r.x;
        l.y = l.y / r.y;
        return l;
    }

    inline uint2 Vec2ShiftLeft(uint2 l, uint2 r)
    {
        l.x = l.x << r.x;
        l.y = l.y << r.y;
        return l;
    }

    inline uint2 Vec2ShiftRight(uint2 l, uint2 r)
    {
        l.x = l.x >> r.x;
        l.y = l.y >> r.y;
        return l;
    }

    inline uint2 Vec2And(uint2 l, uint2 r)
    {
        l.x = l.x & r.x;
        l.y = l.y & r.y;
        return l;
    }

    inline uint2 Vec2Or(uint2 l, uint2 r)
    {
        l.x = l.x | r.x;
        l.y = l.y | r.y;
        return l;
    }

    inline uint2 Vec2Xor(uint2 l, uint2 r)
    {
        l.x = l.x ^ r.x;
        l.y = l.y ^ r.y;
        return l;
    }

    inline uint2 Vec2Mod(uint2 l, uint2 r)
    {
        l.x = l.x % r.x;
        l.y = l.y % r.y;
        return l;
    }

    inline uint2 Vec2Not(uint2 r)
    {
        r.x = ~r.x;
        r.y = ~r.y;
        return r;
    }

    inline uint2 Vec2Decrement(uint2 r)
    {
        --r.x;
        --r.y;
        return r;
    }

    inline uint2 Vec2Increment(uint2 r)
    {
        ++r.x;
        ++r.y;
        return r;
    }

    inline bool2 Vec2Equal(uint2 l, uint2 r)
    {
        bool2 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        return ret;
    }

    inline bool2 Vec2NotEqual(uint2 l, uint2 r)
    {
        bool2 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        return ret;
    }

    inline bool2 Vec2Less(uint2 l, uint2 r)
    {
        bool2 ret;
        ret.x = l.x < r.x;
        ret.y = l.y < r.y;
        return ret;
    }

    inline bool2 Vec2LessEqual(uint2 l, uint2 r)
    {
        bool2 ret;
        ret.x = l.x <= r.x;
        ret.y = l.y <= r.y;
        return ret;
    }

    inline bool2 Vec2Greater(uint2 l, uint2 r)
    {
        bool2 ret;
        ret.x = l.x > r.x;
        ret.y = l.y > r.y;
        return ret;
    }

    inline bool2 Vec2GreaterEqual(uint2 l, uint2 r)
    {
        bool2 ret;
        ret.x = l.x >= r.x;
        ret.y = l.y >= r.y;
        return ret;
    }

    inline uint3 Vec3Add(uint3 l, uint3 r)
    {
        l.x = l.x + r.x;
        l.y = l.y + r.y;
        l.z = l.z + r.z;
        return l;
    }

    inline uint3 Vec3Sub(uint3 l, uint3 r)
    {
        l.x = l.x - r.x;
        l.y = l.y - r.y;
        l.z = l.z - r.z;
        return l;
    }

    inline uint3 Vec3Mul(uint3 l, uint3 r)
    {
        l.x = l.x * r.x;
        l.y = l.y * r.y;
        l.z = l.z * r.z;
        return l;
    }

    inline uint3 Vec3Div(uint3 l, uint3 r)
    {
        l.x = l.x / r.x;
        l.y = l.y / r.y;
        l.z = l.z / r.z;
        return l;
    }

    inline uint3 Vec3ShiftLeft(uint3 l, uint3 r)
    {
        l.x = l.x << r.x;
        l.y = l.y << r.y;
        l.z = l.z << r.z;
        return l;
    }

    inline uint3 Vec3ShiftRight(uint3 l, uint3 r)
    {
        l.x = l.x >> r.x;
        l.y = l.y >> r.y;
        l.z = l.z >> r.z;
        return l;
    }

    inline uint3 Vec3And(uint3 l, uint3 r)
    {
        l.x = l.x & r.x;
        l.y = l.y & r.y;
        l.z = l.z & r.z;
        return l;
    }

    inline uint3 Vec3Or(uint3 l, uint3 r)
    {
        l.x = l.x | r.x;
        l.y = l.y | r.y;
        l.z = l.z | r.z;
        return l;
    }

    inline uint3 Vec3Xor(uint3 l, uint3 r)
    {
        l.x = l.x ^ r.x;
        l.y = l.y ^ r.y;
        l.z = l.z ^ r.z;
        return l;
    }

    inline uint3 Vec3Mod(uint3 l, uint3 r)
    {
        l.x = l.x % r.x;
        l.y = l.y % r.y;
        l.z = l.z % r.z;
        return l;
    }

    inline uint3 Vec3Not(uint3 r)
    {
        r.x = ~r.x;
        r.y = ~r.y;
        r.z = ~r.z;
        return r;
    }

    inline uint3 Vec3Decrement(uint3 r)
    {
        --r.x;
        --r.y;
        --r.z;
        return r;
    }

    inline uint3 Vec3Increment(uint3 r)
    {
        ++r.x;
        ++r.y;
        ++r.z;
        return r;
    }

    inline bool3 Vec3Equal(uint3 l, uint3 r)
    {
        bool3 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        ret.z = l.z == r.z;
        return ret;
    }

    inline bool3 Vec3NotEqual(uint3 l, uint3 r)
    {
        bool3 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        ret.z = l.z != r.z;
        return ret;
    }

    inline bool3 Vec3Less(uint3 l, uint3 r)
    {
        bool3 ret;
        ret.x = l.x < r.x;
        ret.y = l.y < r.y;
        ret.z = l.z < r.z;
        return ret;
    }

    inline bool3 Vec3LessEqual(uint3 l, uint3 r)
    {
        bool3 ret;
        ret.x = l.x <= r.x;
        ret.y = l.y <= r.y;
        ret.z = l.z <= r.z;
        return ret;
    }

    inline bool3 Vec3Greater(uint3 l, uint3 r)
    {
        bool3 ret;
        ret.x = l.x > r.x;
        ret.y = l.y > r.y;
        ret.z = l.z > r.z;
        return ret;
    }

    inline bool3 Vec3GreaterEqual(uint3 l, uint3 r)
    {
        bool3 ret;
        ret.x = l.x >= r.x;
        ret.y = l.y >= r.y;
        ret.z = l.z >= r.z;
        return ret;
    }

    inline uint4 Vec4Add(uint4 l, uint4 r)
    {
        l.x = l.x + r.x;
        l.y = l.y + r.y;
        l.z = l.z + r.z;
        l.w = l.w + r.w;
        return l;
    }

    inline uint4 Vec4Sub(uint4 l, uint4 r)
    {
        l.x = l.x - r.x;
        l.y = l.y - r.y;
        l.z = l.z - r.z;
        l.w = l.w - r.w;
        return l;
    }

    inline uint4 Vec4Mul(uint4 l, uint4 r)
    {
        l.x = l.x * r.x;
        l.y = l.y * r.y;
        l.z = l.z * r.z;
        l.w = l.w * r.w;
        return l;
    }

    inline uint4 Vec4Div(uint4 l, uint4 r)
    {
        l.x = l.x / r.x;
        l.y = l.y / r.y;
        l.z = l.z / r.z;
        l.w = l.w / r.w;
        return l;
    }

    inline uint4 Vec4ShiftLeft(uint4 l, uint4 r)
    {
        l.x = l.x << r.x;
        l.y = l.y << r.y;
        l.z = l.z << r.z;
        l.w = l.w << r.w;
        return l;
    }

    inline uint4 Vec4ShiftRight(uint4 l, uint4 r)
    {
        l.x = l.x >> r.x;
        l.y = l.y >> r.y;
        l.z = l.z >> r.z;
        l.w = l.w >> r.w;
        return l;
    }

    inline uint4 Vec4And(uint4 l, uint4 r)
    {
        l.x = l.x & r.x;
        l.y = l.y & r.y;
        l.z = l.z & r.z;
        l.w = l.w & r.w;
        return l;
    }

    inline uint4 Vec4Or(uint4 l, uint4 r)
    {
        l.x = l.x | r.x;
        l.y = l.y | r.y;
        l.z = l.z | r.z;
        l.w = l.w | r.w;
        return l;
    }

    inline uint4 Vec4Xor(uint4 l, uint4 r)
    {
        l.x = l.x ^ r.x;
        l.y = l.y ^ r.y;
        l.z = l.z ^ r.z;
        l.w = l.w ^ r.w;
        return l;
    }

    inline uint4 Vec4Mod(uint4 l, uint4 r)
    {
        l.x = l.x % r.x;
        l.y = l.y % r.y;
        l.z = l.z % r.z;
        l.w = l.w % r.w;
        return l;
    }

    inline uint4 Vec4Not(uint4 r)
    {
        r.x = ~r.x;
        r.y = ~r.y;
        r.z = ~r.z;
        r.w = ~r.w;
        return r;
    }

    inline uint4 Vec4Decrement(uint4 r)
    {
        --r.x;
        --r.y;
        --r.z;
        --r.w;
        return r;
    }

    inline uint4 Vec4Increment(uint4 r)
    {
        ++r.x;
        ++r.y;
        ++r.z;
        ++r.w;
        return r;
    }

    inline bool4 Vec4Equal(uint4 l, uint4 r)
    {
        bool4 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        ret.z = l.z == r.z;
        ret.w = l.w == r.w;
        return ret;
    }

    inline bool4 Vec4NotEqual(uint4 l, uint4 r)
    {
        bool4 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        ret.z = l.z != r.z;
        ret.w = l.w != r.w;
        return ret;
    }

    inline bool4 Vec4Less(uint4 l, uint4 r)
    {
        bool4 ret;
        ret.x = l.x < r.x;
        ret.y = l.y < r.y;
        ret.z = l.z < r.z;
        ret.w = l.w < r.w;
        return ret;
    }

    inline bool4 Vec4LessEqual(uint4 l, uint4 r)
    {
        bool4 ret;
        ret.x = l.x <= r.x;
        ret.y = l.y <= r.y;
        ret.z = l.z <= r.z;
        ret.w = l.w <= r.w;
        return ret;
    }

    inline bool4 Vec4Greater(uint4 l, uint4 r)
    {
        bool4 ret;
        ret.x = l.x > r.x;
        ret.y = l.y > r.y;
        ret.z = l.z > r.z;
        ret.w = l.w > r.w;
        return ret;
    }

    inline bool4 Vec4GreaterEqual(uint4 l, uint4 r)
    {
        bool4 ret;
        ret.x = l.x >= r.x;
        ret.y = l.y >= r.y;
        ret.z = l.z >= r.z;
        ret.w = l.w >= r.w;
        return ret;
    }

    inline bool2 Vec2LogicalAnd(bool2 l, bool2 r)
    {
        l.x = l.x && r.x;
        l.y = l.y && r.y;
        return l;
    }

    inline bool2 Vec2LogicalOr(bool2 l, bool2 r)
    {
        l.x = l.x || r.x;
        l.y = l.y || r.y;
        return l;
    }

    inline bool2 Vec2LogicalNot(bool2 r)
    {
        r.x = !r.x;
        r.y = !r.y;
        return r;
    }

    inline bool2 Vec2Equal(bool2 l, bool2 r)
    {
        bool2 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        return ret;
    }

    inline bool2 Vec2NotEqual(bool2 l, bool2 r)
    {
        bool2 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        return ret;
    }

    inline bool3 Vec3LogicalAnd(bool3 l, bool3 r)
    {
        l.x = l.x && r.x;
        l.y = l.y && r.y;
        l.z = l.z && r.z;
        return l;
    }

    inline bool3 Vec3LogicalOr(bool3 l, bool3 r)
    {
        l.x = l.x || r.x;
        l.y = l.y || r.y;
        l.z = l.z || r.z;
        return l;
    }

    inline bool3 Vec3LogicalNot(bool3 r)
    {
        r.x = !r.x;
        r.y = !r.y;
        r.z = !r.z;
        return r;
    }

    inline bool3 Vec3Equal(bool3 l, bool3 r)
    {
        bool3 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        ret.z = l.z == r.z;
        return ret;
    }

    inline bool3 Vec3NotEqual(bool3 l, bool3 r)
    {
        bool3 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        ret.z = l.z != r.z;
        return ret;
    }

    inline bool4 Vec4LogicalAnd(bool4 l, bool4 r)
    {
        l.x = l.x && r.x;
        l.y = l.y && r.y;
        l.z = l.z && r.z;
        l.w = l.w && r.w;
        return l;
    }

    inline bool4 Vec4LogicalOr(bool4 l, bool4 r)
    {
        l.x = l.x || r.x;
        l.y = l.y || r.y;
        l.z = l.z || r.z;
        l.w = l.w || r.w;
        return l;
    }

    inline bool4 Vec4LogicalNot(bool4 r)
    {
        r.x = !r.x;
        r.y = !r.y;
        r.z = !r.z;
        r.w = !r.w;
        return r;
    }

    inline bool4 Vec4Equal(bool4 l, bool4 r)
    {
        bool4 ret;
        ret.x = l.x == r.x;
        ret.y = l.y == r.y;
        ret.z = l.z == r.z;
        ret.w = l.w == r.w;
        return ret;
    }

    inline bool4 Vec4NotEqual(bool4 l, bool4 r)
    {
        bool4 ret;
        ret.x = l.x != r.x;
        ret.y = l.y != r.y;
        ret.z = l.z != r.z;
        ret.w = l.w != r.w;
        return ret;
    }

    inline float2 operator+(float2 l, float2 r)
    {
        return Vec2Add(l, r);
    }

    inline float2 operator-(float2 l, float2 r)
    {
        return Vec2Sub(l, r);
    }

    inline float2 operator*(float2 l, float2 r)
    {
        return Vec2Mul(l, r);
    }

    inline float2 operator/(float2 l, float2 r)
    {
        return Vec2Div(l, r);
    }

    inline float2 operator%(float2 l, float2 r)
    {
        return Vec2Mod(l, r);
    }

    inline float2& operator+=(float2& l, float2 r)
    {
        return l = l + r;
    }

    inline float2& operator-=(float2& l, float2 r)
    {
        return l = l - r;
    }

    inline float2& operator*=(float2& l, float2 r)
    {
        return l = l * r;
    }

    inline float2& operator/=(float2& l, float2 r)
    {
        return l = l / r;
    }

    inline float2& operator%=(float2& l, float2 r)
    {
        return l = l % r;
    }

    inline float2 operator*(float l, float2 r)
    {
        return Vec2fSpread(l) * r;
    }

    inline float2 operator*(float2 l, float r)
    {
        return l * Vec2fSpread(r);
    }

    inline float2 operator/(float2 l, float r)
    {
        return l / Vec2fSpread(r);
    }

    inline float2& operator*=(float2& l, float r)
    {
        return l *= Vec2fSpread(r);
    }

    inline float2& operator/=(float2& l, float r)
    {
        return l *= Vec2fSpread(r);
    }

    inline float2 operator-(float2 r)
    {
        return Vec2Negate(r);
    }

    inline bool2 operator==(float2 l, float2 r)
    {
        return Vec2Equal(l, r);
    }

    inline bool2 operator!=(float2 l, float2 r)
    {
        return Vec2NotEqual(l, r);
    }

    inline bool2 operator<(float2 l, float2 r)
    {
        return Vec2Less(l, r);
    }

    inline bool2 operator<=(float2 l, float2 r)
    {
        return Vec2LessEqual(l, r);
    }

    inline bool2 operator>(float2 l, float2 r)
    {
        return Vec2Greater(l, r);
    }

    inline bool2 operator>=(float2 l, float2 r)
    {
        return Vec2GreaterEqual(l, r);
    }

    inline float3 operator+(float3 l, float3 r)
    {
        return Vec3Add(l, r);
    }

    inline float3 operator-(float3 l, float3 r)
    {
        return Vec3Sub(l, r);
    }

    inline float3 operator*(float3 l, float3 r)
    {
        return Vec3Mul(l, r);
    }

    inline float3 operator/(float3 l, float3 r)
    {
        return Vec3Div(l, r);
    }

    inline float3 operator%(float3 l, float3 r)
    {
        return Vec3Mod(l, r);
    }

    inline float3& operator+=(float3& l, float3 r)
    {
        return l = l + r;
    }

    inline float3& operator-=(float3& l, float3 r)
    {
        return l = l - r;
    }

    inline float3& operator*=(float3& l, float3 r)
    {
        return l = l * r;
    }

    inline float3& operator/=(float3& l, float3 r)
    {
        return l = l / r;
    }

    inline float3& operator%=(float3& l, float3 r)
    {
        return l = l % r;
    }

    inline float3 operator*(float l, float3 r)
    {
        return Vec3fSpread(l) * r;
    }

    inline float3 operator*(float3 l, float r)
    {
        return l * Vec3fSpread(r);
    }

    inline float3 operator/(float3 l, float r)
    {
        return l / Vec3fSpread(r);
    }

    inline float3& operator*=(float3& l, float r)
    {
        return l *= Vec3fSpread(r);
    }

    inline float3& operator/=(float3& l, float r)
    {
        return l *= Vec3fSpread(r);
    }

    inline float3 operator-(float3 r)
    {
        return Vec3Negate(r);
    }

    inline bool3 operator==(float3 l, float3 r)
    {
        return Vec3Equal(l, r);
    }

    inline bool3 operator!=(float3 l, float3 r)
    {
        return Vec3NotEqual(l, r);
    }

    inline bool3 operator<(float3 l, float3 r)
    {
        return Vec3Less(l, r);
    }

    inline bool3 operator<=(float3 l, float3 r)
    {
        return Vec3LessEqual(l, r);
    }

    inline bool3 operator>(float3 l, float3 r)
    {
        return Vec3Greater(l, r);
    }

    inline bool3 operator>=(float3 l, float3 r)
    {
        return Vec3GreaterEqual(l, r);
    }

    inline float4 operator+(float4 l, float4 r)
    {
        return Vec4Add(l, r);
    }

    inline float4 operator-(float4 l, float4 r)
    {
        return Vec4Sub(l, r);
    }

    inline float4 operator*(float4 l, float4 r)
    {
        return Vec4Mul(l, r);
    }

    inline float4 operator/(float4 l, float4 r)
    {
        return Vec4Div(l, r);
    }

    inline float4 operator%(float4 l, float4 r)
    {
        return Vec4Mod(l, r);
    }

    inline float4& operator+=(float4& l, float4 r)
    {
        return l = l + r;
    }

    inline float4& operator-=(float4& l, float4 r)
    {
        return l = l - r;
    }

    inline float4& operator*=(float4& l, float4 r)
    {
        return l = l * r;
    }

    inline float4& operator/=(float4& l, float4 r)
    {
        return l = l / r;
    }

    inline float4& operator%=(float4& l, float4 r)
    {
        return l = l % r;
    }

    inline float4 operator*(float l, float4 r)
    {
        return Vec4fSpread(l) * r;
    }

    inline float4 operator*(float4 l, float r)
    {
        return l * Vec4fSpread(r);
    }

    inline float4 operator/(float4 l, float r)
    {
        return l / Vec4fSpread(r);
    }

    inline float4& operator*=(float4& l, float r)
    {
        return l *= Vec4fSpread(r);
    }

    inline float4& operator/=(float4& l, float r)
    {
        return l *= Vec4fSpread(r);
    }

    inline float4 operator-(float4 r)
    {
        return Vec4Negate(r);
    }

    inline bool4 operator==(float4 l, float4 r)
    {
        return Vec4Equal(l, r);
    }

    inline bool4 operator!=(float4 l, float4 r)
    {
        return Vec4NotEqual(l, r);
    }

    inline bool4 operator<(float4 l, float4 r)
    {
        return Vec4Less(l, r);
    }

    inline bool4 operator<=(float4 l, float4 r)
    {
        return Vec4LessEqual(l, r);
    }

    inline bool4 operator>(float4 l, float4 r)
    {
        return Vec4Greater(l, r);
    }

    inline bool4 operator>=(float4 l, float4 r)
    {
        return Vec4GreaterEqual(l, r);
    }

    inline int2 operator+(int2 l, int2 r)
    {
        return Vec2Add(l, r);
    }

    inline int2 operator-(int2 l, int2 r)
    {
        return Vec2Sub(l, r);
    }

    inline int2 operator*(int2 l, int2 r)
    {
        return Vec2Mul(l, r);
    }

    inline int2 operator/(int2 l, int2 r)
    {
        return Vec2Div(l, r);
    }

    inline int2& operator+=(int2& l, int2 r)
    {
        return l = l + r;
    }

    inline int2& operator-=(int2& l, int2 r)
    {
        return l = l - r;
    }

    inline int2& operator*=(int2& l, int2 r)
    {
        return l = l * r;
    }

    inline int2& operator/=(int2& l, int2 r)
    {
        return l = l / r;
    }

    inline int2 operator<<(int2 l, int2 r)
    {
        return Vec2ShiftLeft(l, r);
    }

    inline int2 operator>>(int2 l, int2 r)
    {
        return Vec2ShiftRight(l, r);
    }

    inline int2 operator&(int2 l, int2 r)
    {
        return Vec2And(l, r);
    }

    inline int2 operator|(int2 l, int2 r)
    {
        return Vec2Or(l, r);
    }

    inline int2 operator^(int2 l, int2 r)
    {
        return Vec2Xor(l, r);
    }

    inline int2 operator%(int2 l, int2 r)
    {
        return Vec2Mod(l, r);
    }

    inline int2& operator<<=(int2& l, int2 r)
    {
        return l = l << r;
    }

    inline int2& operator>>=(int2& l, int2 r)
    {
        return l = l >> r;
    }

    inline int2& operator&=(int2& l, int2 r)
    {
        return l = l & r;
    }

    inline int2& operator|=(int2& l, int2 r)
    {
        return l = l | r;
    }

    inline int2& operator^=(int2& l, int2 r)
    {
        return l = l ^ r;
    }

    inline int2& operator%=(int2& l, int2 r)
    {
        return l = l % r;
    }

    inline int2 operator*(int32_t l, int2 r)
    {
        return Vec2iSpread(l) * r;
    }

    inline int2 operator*(int2 l, int32_t r)
    {
        return l * Vec2iSpread(r);
    }

    inline int2 operator/(int2 l, int32_t r)
    {
        return l / Vec2iSpread(r);
    }

    inline int2& operator*=(int2& l, int32_t r)
    {
        return l *= Vec2iSpread(r);
    }

    inline int2& operator/=(int2& l, int32_t r)
    {
        return l *= Vec2iSpread(r);
    }

    inline int2& operator&=(int2& l, int32_t r)
    {
        return l *= Vec2iSpread(r);
    }

    inline int2& operator|=(int2& l, int32_t r)
    {
        return l *= Vec2iSpread(r);
    }

    inline int2& operator^=(int2& l, int32_t r)
    {
        return l *= Vec2iSpread(r);
    }

    inline int2 operator~(int2 r)
    {
        return Vec2Not(r);
    }

    inline int2 operator-(int2 r)
    {
        return Vec2Negate(r);
    }

    inline bool2 operator==(int2 l, int2 r)
    {
        return Vec2Equal(l, r);
    }

    inline bool2 operator!=(int2 l, int2 r)
    {
        return Vec2NotEqual(l, r);
    }

    inline bool2 operator<(int2 l, int2 r)
    {
        return Vec2Less(l, r);
    }

    inline bool2 operator<=(int2 l, int2 r)
    {
        return Vec2LessEqual(l, r);
    }

    inline bool2 operator>(int2 l, int2 r)
    {
        return Vec2Greater(l, r);
    }

    inline bool2 operator>=(int2 l, int2 r)
    {
        return Vec2GreaterEqual(l, r);
    }

    inline int3 operator+(int3 l, int3 r)
    {
        return Vec3Add(l, r);
    }

    inline int3 operator-(int3 l, int3 r)
    {
        return Vec3Sub(l, r);
    }

    inline int3 operator*(int3 l, int3 r)
    {
        return Vec3Mul(l, r);
    }

    inline int3 operator/(int3 l, int3 r)
    {
        return Vec3Div(l, r);
    }

    inline int3& operator+=(int3& l, int3 r)
    {
        return l = l + r;
    }

    inline int3& operator-=(int3& l, int3 r)
    {
        return l = l - r;
    }

    inline int3& operator*=(int3& l, int3 r)
    {
        return l = l * r;
    }

    inline int3& operator/=(int3& l, int3 r)
    {
        return l = l / r;
    }

    inline int3 operator<<(int3 l, int3 r)
    {
        return Vec3ShiftLeft(l, r);
    }

    inline int3 operator>>(int3 l, int3 r)
    {
        return Vec3ShiftRight(l, r);
    }

    inline int3 operator&(int3 l, int3 r)
    {
        return Vec3And(l, r);
    }

    inline int3 operator|(int3 l, int3 r)
    {
        return Vec3Or(l, r);
    }

    inline int3 operator^(int3 l, int3 r)
    {
        return Vec3Xor(l, r);
    }

    inline int3 operator%(int3 l, int3 r)
    {
        return Vec3Mod(l, r);
    }

    inline int3& operator<<=(int3& l, int3 r)
    {
        return l = l << r;
    }

    inline int3& operator>>=(int3& l, int3 r)
    {
        return l = l >> r;
    }

    inline int3& operator&=(int3& l, int3 r)
    {
        return l = l & r;
    }

    inline int3& operator|=(int3& l, int3 r)
    {
        return l = l | r;
    }

    inline int3& operator^=(int3& l, int3 r)
    {
        return l = l ^ r;
    }

    inline int3& operator%=(int3& l, int3 r)
    {
        return l = l % r;
    }

    inline int3 operator*(int32_t l, int3 r)
    {
        return Vec3iSpread(l) * r;
    }

    inline int3 operator*(int3 l, int32_t r)
    {
        return l * Vec3iSpread(r);
    }

    inline int3 operator/(int3 l, int32_t r)
    {
        return l / Vec3iSpread(r);
    }

    inline int3& operator*=(int3& l, int32_t r)
    {
        return l *= Vec3iSpread(r);
    }

    inline int3& operator/=(int3& l, int32_t r)
    {
        return l *= Vec3iSpread(r);
    }

    inline int3& operator&=(int3& l, int32_t r)
    {
        return l *= Vec3iSpread(r);
    }

    inline int3& operator|=(int3& l, int32_t r)
    {
        return l *= Vec3iSpread(r);
    }

    inline int3& operator^=(int3& l, int32_t r)
    {
        return l *= Vec3iSpread(r);
    }

    inline int3 operator~(int3 r)
    {
        return Vec3Not(r);
    }

    inline int3 operator-(int3 r)
    {
        return Vec3Negate(r);
    }

    inline bool3 operator==(int3 l, int3 r)
    {
        return Vec3Equal(l, r);
    }

    inline bool3 operator!=(int3 l, int3 r)
    {
        return Vec3NotEqual(l, r);
    }

    inline bool3 operator<(int3 l, int3 r)
    {
        return Vec3Less(l, r);
    }

    inline bool3 operator<=(int3 l, int3 r)
    {
        return Vec3LessEqual(l, r);
    }

    inline bool3 operator>(int3 l, int3 r)
    {
        return Vec3Greater(l, r);
    }

    inline bool3 operator>=(int3 l, int3 r)
    {
        return Vec3GreaterEqual(l, r);
    }

    inline int4 operator+(int4 l, int4 r)
    {
        return Vec4Add(l, r);
    }

    inline int4 operator-(int4 l, int4 r)
    {
        return Vec4Sub(l, r);
    }

    inline int4 operator*(int4 l, int4 r)
    {
        return Vec4Mul(l, r);
    }

    inline int4 operator/(int4 l, int4 r)
    {
        return Vec4Div(l, r);
    }

    inline int4& operator+=(int4& l, int4 r)
    {
        return l = l + r;
    }

    inline int4& operator-=(int4& l, int4 r)
    {
        return l = l - r;
    }

    inline int4& operator*=(int4& l, int4 r)
    {
        return l = l * r;
    }

    inline int4& operator/=(int4& l, int4 r)
    {
        return l = l / r;
    }

    inline int4 operator<<(int4 l, int4 r)
    {
        return Vec4ShiftLeft(l, r);
    }

    inline int4 operator>>(int4 l, int4 r)
    {
        return Vec4ShiftRight(l, r);
    }

    inline int4 operator&(int4 l, int4 r)
    {
        return Vec4And(l, r);
    }

    inline int4 operator|(int4 l, int4 r)
    {
        return Vec4Or(l, r);
    }

    inline int4 operator^(int4 l, int4 r)
    {
        return Vec4Xor(l, r);
    }

    inline int4 operator%(int4 l, int4 r)
    {
        return Vec4Mod(l, r);
    }

    inline int4& operator<<=(int4& l, int4 r)
    {
        return l = l << r;
    }

    inline int4& operator>>=(int4& l, int4 r)
    {
        return l = l >> r;
    }

    inline int4& operator&=(int4& l, int4 r)
    {
        return l = l & r;
    }

    inline int4& operator|=(int4& l, int4 r)
    {
        return l = l | r;
    }

    inline int4& operator^=(int4& l, int4 r)
    {
        return l = l ^ r;
    }

    inline int4& operator%=(int4& l, int4 r)
    {
        return l = l % r;
    }

    inline int4 operator*(int32_t l, int4 r)
    {
        return Vec4iSpread(l) * r;
    }

    inline int4 operator*(int4 l, int32_t r)
    {
        return l * Vec4iSpread(r);
    }

    inline int4 operator/(int4 l, int32_t r)
    {
        return l / Vec4iSpread(r);
    }

    inline int4& operator*=(int4& l, int32_t r)
    {
        return l *= Vec4iSpread(r);
    }

    inline int4& operator/=(int4& l, int32_t r)
    {
        return l *= Vec4iSpread(r);
    }

    inline int4& operator&=(int4& l, int32_t r)
    {
        return l *= Vec4iSpread(r);
    }

    inline int4& operator|=(int4& l, int32_t r)
    {
        return l *= Vec4iSpread(r);
    }

    inline int4& operator^=(int4& l, int32_t r)
    {
        return l *= Vec4iSpread(r);
    }

    inline int4 operator~(int4 r)
    {
        return Vec4Not(r);
    }

    inline int4 operator-(int4 r)
    {
        return Vec4Negate(r);
    }

    inline bool4 operator==(int4 l, int4 r)
    {
        return Vec4Equal(l, r);
    }

    inline bool4 operator!=(int4 l, int4 r)
    {
        return Vec4NotEqual(l, r);
    }

    inline bool4 operator<(int4 l, int4 r)
    {
        return Vec4Less(l, r);
    }

    inline bool4 operator<=(int4 l, int4 r)
    {
        return Vec4LessEqual(l, r);
    }

    inline bool4 operator>(int4 l, int4 r)
    {
        return Vec4Greater(l, r);
    }

    inline bool4 operator>=(int4 l, int4 r)
    {
        return Vec4GreaterEqual(l, r);
    }

    inline uint2 operator+(uint2 l, uint2 r)
    {
        return Vec2Add(l, r);
    }

    inline uint2 operator-(uint2 l, uint2 r)
    {
        return Vec2Sub(l, r);
    }

    inline uint2 operator*(uint2 l, uint2 r)
    {
        return Vec2Mul(l, r);
    }

    inline uint2 operator/(uint2 l, uint2 r)
    {
        return Vec2Div(l, r);
    }

    inline uint2& operator+=(uint2& l, uint2 r)
    {
        return l = l + r;
    }

    inline uint2& operator-=(uint2& l, uint2 r)
    {
        return l = l - r;
    }

    inline uint2& operator*=(uint2& l, uint2 r)
    {
        return l = l * r;
    }

    inline uint2& operator/=(uint2& l, uint2 r)
    {
        return l = l / r;
    }

    inline uint2 operator<<(uint2 l, uint2 r)
    {
        return Vec2ShiftLeft(l, r);
    }

    inline uint2 operator>>(uint2 l, uint2 r)
    {
        return Vec2ShiftRight(l, r);
    }

    inline uint2 operator&(uint2 l, uint2 r)
    {
        return Vec2And(l, r);
    }

    inline uint2 operator|(uint2 l, uint2 r)
    {
        return Vec2Or(l, r);
    }

    inline uint2 operator^(uint2 l, uint2 r)
    {
        return Vec2Xor(l, r);
    }

    inline uint2 operator%(uint2 l, uint2 r)
    {
        return Vec2Mod(l, r);
    }

    inline uint2& operator<<=(uint2& l, uint2 r)
    {
        return l = l << r;
    }

    inline uint2& operator>>=(uint2& l, uint2 r)
    {
        return l = l >> r;
    }

    inline uint2& operator&=(uint2& l, uint2 r)
    {
        return l = l & r;
    }

    inline uint2& operator|=(uint2& l, uint2 r)
    {
        return l = l | r;
    }

    inline uint2& operator^=(uint2& l, uint2 r)
    {
        return l = l ^ r;
    }

    inline uint2& operator%=(uint2& l, uint2 r)
    {
        return l = l % r;
    }

    inline uint2 operator*(uint32_t l, uint2 r)
    {
        return Vec2uSpread(l) * r;
    }

    inline uint2 operator*(uint2 l, uint32_t r)
    {
        return l * Vec2uSpread(r);
    }

    inline uint2 operator/(uint2 l, uint32_t r)
    {
        return l / Vec2uSpread(r);
    }

    inline uint2& operator*=(uint2& l, uint32_t r)
    {
        return l *= Vec2uSpread(r);
    }

    inline uint2& operator/=(uint2& l, uint32_t r)
    {
        return l *= Vec2uSpread(r);
    }

    inline uint2& operator&=(uint2& l, uint32_t r)
    {
        return l *= Vec2uSpread(r);
    }

    inline uint2& operator|=(uint2& l, uint32_t r)
    {
        return l *= Vec2uSpread(r);
    }

    inline uint2& operator^=(uint2& l, uint32_t r)
    {
        return l *= Vec2uSpread(r);
    }

    inline uint2 operator~(uint2 r)
    {
        return Vec2Not(r);
    }

    inline bool2 operator==(uint2 l, uint2 r)
    {
        return Vec2Equal(l, r);
    }

    inline bool2 operator!=(uint2 l, uint2 r)
    {
        return Vec2NotEqual(l, r);
    }

    inline bool2 operator<(uint2 l, uint2 r)
    {
        return Vec2Less(l, r);
    }

    inline bool2 operator<=(uint2 l, uint2 r)
    {
        return Vec2LessEqual(l, r);
    }

    inline bool2 operator>(uint2 l, uint2 r)
    {
        return Vec2Greater(l, r);
    }

    inline bool2 operator>=(uint2 l, uint2 r)
    {
        return Vec2GreaterEqual(l, r);
    }

    inline uint3 operator+(uint3 l, uint3 r)
    {
        return Vec3Add(l, r);
    }

    inline uint3 operator-(uint3 l, uint3 r)
    {
        return Vec3Sub(l, r);
    }

    inline uint3 operator*(uint3 l, uint3 r)
    {
        return Vec3Mul(l, r);
    }

    inline uint3 operator/(uint3 l, uint3 r)
    {
        return Vec3Div(l, r);
    }

    inline uint3& operator+=(uint3& l, uint3 r)
    {
        return l = l + r;
    }

    inline uint3& operator-=(uint3& l, uint3 r)
    {
        return l = l - r;
    }

    inline uint3& operator*=(uint3& l, uint3 r)
    {
        return l = l * r;
    }

    inline uint3& operator/=(uint3& l, uint3 r)
    {
        return l = l / r;
    }

    inline uint3 operator<<(uint3 l, uint3 r)
    {
        return Vec3ShiftLeft(l, r);
    }

    inline uint3 operator>>(uint3 l, uint3 r)
    {
        return Vec3ShiftRight(l, r);
    }

    inline uint3 operator&(uint3 l, uint3 r)
    {
        return Vec3And(l, r);
    }

    inline uint3 operator|(uint3 l, uint3 r)
    {
        return Vec3Or(l, r);
    }

    inline uint3 operator^(uint3 l, uint3 r)
    {
        return Vec3Xor(l, r);
    }

    inline uint3 operator%(uint3 l, uint3 r)
    {
        return Vec3Mod(l, r);
    }

    inline uint3& operator<<=(uint3& l, uint3 r)
    {
        return l = l << r;
    }

    inline uint3& operator>>=(uint3& l, uint3 r)
    {
        return l = l >> r;
    }

    inline uint3& operator&=(uint3& l, uint3 r)
    {
        return l = l & r;
    }

    inline uint3& operator|=(uint3& l, uint3 r)
    {
        return l = l | r;
    }

    inline uint3& operator^=(uint3& l, uint3 r)
    {
        return l = l ^ r;
    }

    inline uint3& operator%=(uint3& l, uint3 r)
    {
        return l = l % r;
    }

    inline uint3 operator*(uint32_t l, uint3 r)
    {
        return Vec3uSpread(l) * r;
    }

    inline uint3 operator*(uint3 l, uint32_t r)
    {
        return l * Vec3uSpread(r);
    }

    inline uint3 operator/(uint3 l, uint32_t r)
    {
        return l / Vec3uSpread(r);
    }

    inline uint3& operator*=(uint3& l, uint32_t r)
    {
        return l *= Vec3uSpread(r);
    }

    inline uint3& operator/=(uint3& l, uint32_t r)
    {
        return l *= Vec3uSpread(r);
    }

    inline uint3& operator&=(uint3& l, uint32_t r)
    {
        return l *= Vec3uSpread(r);
    }

    inline uint3& operator|=(uint3& l, uint32_t r)
    {
        return l *= Vec3uSpread(r);
    }

    inline uint3& operator^=(uint3& l, uint32_t r)
    {
        return l *= Vec3uSpread(r);
    }

    inline uint3 operator~(uint3 r)
    {
        return Vec3Not(r);
    }

    inline bool3 operator==(uint3 l, uint3 r)
    {
        return Vec3Equal(l, r);
    }

    inline bool3 operator!=(uint3 l, uint3 r)
    {
        return Vec3NotEqual(l, r);
    }

    inline bool3 operator<(uint3 l, uint3 r)
    {
        return Vec3Less(l, r);
    }

    inline bool3 operator<=(uint3 l, uint3 r)
    {
        return Vec3LessEqual(l, r);
    }

    inline bool3 operator>(uint3 l, uint3 r)
    {
        return Vec3Greater(l, r);
    }

    inline bool3 operator>=(uint3 l, uint3 r)
    {
        return Vec3GreaterEqual(l, r);
    }

    inline uint4 operator+(uint4 l, uint4 r)
    {
        return Vec4Add(l, r);
    }

    inline uint4 operator-(uint4 l, uint4 r)
    {
        return Vec4Sub(l, r);
    }

    inline uint4 operator*(uint4 l, uint4 r)
    {
        return Vec4Mul(l, r);
    }

    inline uint4 operator/(uint4 l, uint4 r)
    {
        return Vec4Div(l, r);
    }

    inline uint4& operator+=(uint4& l, uint4 r)
    {
        return l = l + r;
    }

    inline uint4& operator-=(uint4& l, uint4 r)
    {
        return l = l - r;
    }

    inline uint4& operator*=(uint4& l, uint4 r)
    {
        return l = l * r;
    }

    inline uint4& operator/=(uint4& l, uint4 r)
    {
        return l = l / r;
    }

    inline uint4 operator<<(uint4 l, uint4 r)
    {
        return Vec4ShiftLeft(l, r);
    }

    inline uint4 operator>>(uint4 l, uint4 r)
    {
        return Vec4ShiftRight(l, r);
    }

    inline uint4 operator&(uint4 l, uint4 r)
    {
        return Vec4And(l, r);
    }

    inline uint4 operator|(uint4 l, uint4 r)
    {
        return Vec4Or(l, r);
    }

    inline uint4 operator^(uint4 l, uint4 r)
    {
        return Vec4Xor(l, r);
    }

    inline uint4 operator%(uint4 l, uint4 r)
    {
        return Vec4Mod(l, r);
    }

    inline uint4& operator<<=(uint4& l, uint4 r)
    {
        return l = l << r;
    }

    inline uint4& operator>>=(uint4& l, uint4 r)
    {
        return l = l >> r;
    }

    inline uint4& operator&=(uint4& l, uint4 r)
    {
        return l = l & r;
    }

    inline uint4& operator|=(uint4& l, uint4 r)
    {
        return l = l | r;
    }

    inline uint4& operator^=(uint4& l, uint4 r)
    {
        return l = l ^ r;
    }

    inline uint4& operator%=(uint4& l, uint4 r)
    {
        return l = l % r;
    }

    inline uint4 operator*(uint32_t l, uint4 r)
    {
        return Vec4uSpread(l) * r;
    }

    inline uint4 operator*(uint4 l, uint32_t r)
    {
        return l * Vec4uSpread(r);
    }

    inline uint4 operator/(uint4 l, uint32_t r)
    {
        return l / Vec4uSpread(r);
    }

    inline uint4& operator*=(uint4& l, uint32_t r)
    {
        return l *= Vec4uSpread(r);
    }

    inline uint4& operator/=(uint4& l, uint32_t r)
    {
        return l *= Vec4uSpread(r);
    }

    inline uint4& operator&=(uint4& l, uint32_t r)
    {
        return l *= Vec4uSpread(r);
    }

    inline uint4& operator|=(uint4& l, uint32_t r)
    {
        return l *= Vec4uSpread(r);
    }

    inline uint4& operator^=(uint4& l, uint32_t r)
    {
        return l *= Vec4uSpread(r);
    }

    inline uint4 operator~(uint4 r)
    {
        return Vec4Not(r);
    }

    inline bool4 operator==(uint4 l, uint4 r)
    {
        return Vec4Equal(l, r);
    }

    inline bool4 operator!=(uint4 l, uint4 r)
    {
        return Vec4NotEqual(l, r);
    }

    inline bool4 operator<(uint4 l, uint4 r)
    {
        return Vec4Less(l, r);
    }

    inline bool4 operator<=(uint4 l, uint4 r)
    {
        return Vec4LessEqual(l, r);
    }

    inline bool4 operator>(uint4 l, uint4 r)
    {
        return Vec4Greater(l, r);
    }

    inline bool4 operator>=(uint4 l, uint4 r)
    {
        return Vec4GreaterEqual(l, r);
    }

    inline bool4 operator&&(bool4 l, bool4 r)
    {
        return Vec4LogicalAnd(l, r);
    }

    inline bool4 operator||(bool4 l, bool4 r)
    {
        return Vec4LogicalOr(l, r);
    }

    inline bool4 operator!(bool4 r)
    {
        return Vec4LogicalNot(r);
    }

    inline bool4 operator==(bool4 l, bool4 r)
    {
        return Vec4Equal(l, r);
    }

    inline bool4 operator!=(bool4 l, bool4 r)
    {
        return Vec4NotEqual(l, r);
    }

    inline bool3 operator&&(bool3 l, bool3 r)
    {
        return Vec3LogicalAnd(l, r);
    }

    inline bool3 operator||(bool3 l, bool3 r)
    {
        return Vec3LogicalOr(l, r);
    }

    inline bool3 operator!(bool3 r)
    {
        return Vec3LogicalNot(r);
    }

    inline bool3 operator==(bool3 l, bool3 r)
    {
        return Vec3Equal(l, r);
    }

    inline bool3 operator!=(bool3 l, bool3 r)
    {
        return Vec3NotEqual(l, r);
    }

    inline bool2 operator&&(bool2 l, bool2 r)
    {
        return Vec2LogicalAnd(l, r);
    }

    inline bool2 operator||(bool2 l, bool2 r)
    {
        return Vec2LogicalOr(l, r);
    }

    inline bool2 operator!(bool2 r)
    {
        return Vec2LogicalNot(r);
    }

    inline bool2 operator==(bool2 l, bool2 r)
    {
        return Vec2Equal(l, r);
    }

    inline bool2 operator!=(bool2 l, bool2 r)
    {
        return Vec2NotEqual(l, r);
    }

    inline Matrix4x4 operator*(const Matrix4x4& l, const Matrix4x4& r)
    {
        return Mat4Mul(l, r);
    }

    inline float4 operator*(float4 l, const Matrix4x4& r)
    {
        return Vec4Transform(l, r);
    }

    inline Quaternion operator*(Quaternion l, Quaternion r)
    {
        return QuatMul(l, r);
    }

    // float2 swizzle
    inline float2 float2::xx() const { return float2(x, x); }
    inline float2 float2::xy() const { return float2(x, y); }
    inline float2 float2::yx() const { return float2(y, x); }
    inline float2 float2::yy() const { return float2(y, y); }
    inline float3 float2::xxx() const { return float3(x, x, x); }
    inline float3 float2::xxy() const { return float3(x, x, y); }
    inline float3 float2::xyx() const { return float3(x, y, x); }
    inline float3 float2::xyy() const { return float3(x, y, y); }
    inline float3 float2::yxx() const { return float3(y, x, x); }
    inline float3 float2::yxy() const { return float3(y, x, y); }
    inline float3 float2::yyx() const { return float3(y, y, x); }
    inline float3 float2::yyy() const { return float3(y, y, y); }
    inline float4 float2::xxxx() const { return float4(x, x, x, x); }
    inline float4 float2::xxxy() const { return float4(x, x, x, y); }
    inline float4 float2::xxyx() const { return float4(x, x, y, x); }
    inline float4 float2::xxyy() const { return float4(x, x, y, y); }
    inline float4 float2::xyxx() const { return float4(x, y, x, x); }
    inline float4 float2::xyxy() const { return float4(x, y, x, y); }
    inline float4 float2::xyyx() const { return float4(x, y, y, x); }
    inline float4 float2::xyyy() const { return float4(x, y, y, y); }
    inline float4 float2::yxxx() const { return float4(y, x, x, x); }
    inline float4 float2::yxxy() const { return float4(y, x, x, y); }
    inline float4 float2::yxyx() const { return float4(y, x, y, x); }
    inline float4 float2::yxyy() const { return float4(y, x, y, y); }
    inline float4 float2::yyxx() const { return float4(y, y, x, x); }
    inline float4 float2::yyxy() const { return float4(y, y, x, y); }
    inline float4 float2::yyyx() const { return float4(y, y, y, x); }
    inline float4 float2::yyyy() const { return float4(y, y, y, y); }
    // float3 swizzle
    inline float2 float3::xx() const { return float2(x, x); }
    inline float2 float3::xy() const { return float2(x, y); }
    inline float2 float3::xz() const { return float2(x, z); }
    inline float2 float3::yx() const { return float2(y, x); }
    inline float2 float3::yy() const { return float2(y, y); }
    inline float2 float3::yz() const { return float2(y, z); }
    inline float2 float3::zx() const { return float2(z, x); }
    inline float2 float3::zy() const { return float2(z, y); }
    inline float2 float3::zz() const { return float2(z, z); }
    inline float3 float3::xxx() const { return float3(x, x, x); }
    inline float3 float3::xxy() const { return float3(x, x, y); }
    inline float3 float3::xxz() const { return float3(x, x, z); }
    inline float3 float3::xyx() const { return float3(x, y, x); }
    inline float3 float3::xyy() const { return float3(x, y, y); }
    inline float3 float3::xyz() const { return float3(x, y, z); }
    inline float3 float3::xzx() const { return float3(x, z, x); }
    inline float3 float3::xzy() const { return float3(x, z, y); }
    inline float3 float3::xzz() const { return float3(x, z, z); }
    inline float3 float3::yxx() const { return float3(y, x, x); }
    inline float3 float3::yxy() const { return float3(y, x, y); }
    inline float3 float3::yxz() const { return float3(y, x, z); }
    inline float3 float3::yyx() const { return float3(y, y, x); }
    inline float3 float3::yyy() const { return float3(y, y, y); }
    inline float3 float3::yyz() const { return float3(y, y, z); }
    inline float3 float3::yzx() const { return float3(y, z, x); }
    inline float3 float3::yzy() const { return float3(y, z, y); }
    inline float3 float3::yzz() const { return float3(y, z, z); }
    inline float3 float3::zxx() const { return float3(z, x, x); }
    inline float3 float3::zxy() const { return float3(z, x, y); }
    inline float3 float3::zxz() const { return float3(z, x, z); }
    inline float3 float3::zyx() const { return float3(z, y, x); }
    inline float3 float3::zyy() const { return float3(z, y, y); }
    inline float3 float3::zyz() const { return float3(z, y, z); }
    inline float3 float3::zzx() const { return float3(z, z, x); }
    inline float3 float3::zzy() const { return float3(z, z, y); }
    inline float3 float3::zzz() const { return float3(z, z, z); }
    inline float4 float3::xxxx() const { return float4(x, x, x, x); }
    inline float4 float3::xxxy() const { return float4(x, x, x, y); }
    inline float4 float3::xxxz() const { return float4(x, x, x, z); }
    inline float4 float3::xxyx() const { return float4(x, x, y, x); }
    inline float4 float3::xxyy() const { return float4(x, x, y, y); }
    inline float4 float3::xxyz() const { return float4(x, x, y, z); }
    inline float4 float3::xxzx() const { return float4(x, x, z, x); }
    inline float4 float3::xxzy() const { return float4(x, x, z, y); }
    inline float4 float3::xxzz() const { return float4(x, x, z, z); }
    inline float4 float3::xyxx() const { return float4(x, y, x, x); }
    inline float4 float3::xyxy() const { return float4(x, y, x, y); }
    inline float4 float3::xyxz() const { return float4(x, y, x, z); }
    inline float4 float3::xyyx() const { return float4(x, y, y, x); }
    inline float4 float3::xyyy() const { return float4(x, y, y, y); }
    inline float4 float3::xyyz() const { return float4(x, y, y, z); }
    inline float4 float3::xyzx() const { return float4(x, y, z, x); }
    inline float4 float3::xyzy() const { return float4(x, y, z, y); }
    inline float4 float3::xyzz() const { return float4(x, y, z, z); }
    inline float4 float3::xzxx() const { return float4(x, z, x, x); }
    inline float4 float3::xzxy() const { return float4(x, z, x, y); }
    inline float4 float3::xzxz() const { return float4(x, z, x, z); }
    inline float4 float3::xzyx() const { return float4(x, z, y, x); }
    inline float4 float3::xzyy() const { return float4(x, z, y, y); }
    inline float4 float3::xzyz() const { return float4(x, z, y, z); }
    inline float4 float3::xzzx() const { return float4(x, z, z, x); }
    inline float4 float3::xzzy() const { return float4(x, z, z, y); }
    inline float4 float3::xzzz() const { return float4(x, z, z, z); }
    inline float4 float3::yxxx() const { return float4(y, x, x, x); }
    inline float4 float3::yxxy() const { return float4(y, x, x, y); }
    inline float4 float3::yxxz() const { return float4(y, x, x, z); }
    inline float4 float3::yxyx() const { return float4(y, x, y, x); }
    inline float4 float3::yxyy() const { return float4(y, x, y, y); }
    inline float4 float3::yxyz() const { return float4(y, x, y, z); }
    inline float4 float3::yxzx() const { return float4(y, x, z, x); }
    inline float4 float3::yxzy() const { return float4(y, x, z, y); }
    inline float4 float3::yxzz() const { return float4(y, x, z, z); }
    inline float4 float3::yyxx() const { return float4(y, y, x, x); }
    inline float4 float3::yyxy() const { return float4(y, y, x, y); }
    inline float4 float3::yyxz() const { return float4(y, y, x, z); }
    inline float4 float3::yyyx() const { return float4(y, y, y, x); }
    inline float4 float3::yyyy() const { return float4(y, y, y, y); }
    inline float4 float3::yyyz() const { return float4(y, y, y, z); }
    inline float4 float3::yyzx() const { return float4(y, y, z, x); }
    inline float4 float3::yyzy() const { return float4(y, y, z, y); }
    inline float4 float3::yyzz() const { return float4(y, y, z, z); }
    inline float4 float3::yzxx() const { return float4(y, z, x, x); }
    inline float4 float3::yzxy() const { return float4(y, z, x, y); }
    inline float4 float3::yzxz() const { return float4(y, z, x, z); }
    inline float4 float3::yzyx() const { return float4(y, z, y, x); }
    inline float4 float3::yzyy() const { return float4(y, z, y, y); }
    inline float4 float3::yzyz() const { return float4(y, z, y, z); }
    inline float4 float3::yzzx() const { return float4(y, z, z, x); }
    inline float4 float3::yzzy() const { return float4(y, z, z, y); }
    inline float4 float3::yzzz() const { return float4(y, z, z, z); }
    inline float4 float3::zxxx() const { return float4(z, x, x, x); }
    inline float4 float3::zxxy() const { return float4(z, x, x, y); }
    inline float4 float3::zxxz() const { return float4(z, x, x, z); }
    inline float4 float3::zxyx() const { return float4(z, x, y, x); }
    inline float4 float3::zxyy() const { return float4(z, x, y, y); }
    inline float4 float3::zxyz() const { return float4(z, x, y, z); }
    inline float4 float3::zxzx() const { return float4(z, x, z, x); }
    inline float4 float3::zxzy() const { return float4(z, x, z, y); }
    inline float4 float3::zxzz() const { return float4(z, x, z, z); }
    inline float4 float3::zyxx() const { return float4(z, y, x, x); }
    inline float4 float3::zyxy() const { return float4(z, y, x, y); }
    inline float4 float3::zyxz() const { return float4(z, y, x, z); }
    inline float4 float3::zyyx() const { return float4(z, y, y, x); }
    inline float4 float3::zyyy() const { return float4(z, y, y, y); }
    inline float4 float3::zyyz() const { return float4(z, y, y, z); }
    inline float4 float3::zyzx() const { return float4(z, y, z, x); }
    inline float4 float3::zyzy() const { return float4(z, y, z, y); }
    inline float4 float3::zyzz() const { return float4(z, y, z, z); }
    inline float4 float3::zzxx() const { return float4(z, z, x, x); }
    inline float4 float3::zzxy() const { return float4(z, z, x, y); }
    inline float4 float3::zzxz() const { return float4(z, z, x, z); }
    inline float4 float3::zzyx() const { return float4(z, z, y, x); }
    inline float4 float3::zzyy() const { return float4(z, z, y, y); }
    inline float4 float3::zzyz() const { return float4(z, z, y, z); }
    inline float4 float3::zzzx() const { return float4(z, z, z, x); }
    inline float4 float3::zzzy() const { return float4(z, z, z, y); }
    inline float4 float3::zzzz() const { return float4(z, z, z, z); }
    // float4 swizzle
    inline float2 float4::xx() const { return float2(x, x); }
    inline float2 float4::xy() const { return float2(x, y); }
    inline float2 float4::xz() const { return float2(x, z); }
    inline float2 float4::xw() const { return float2(x, w); }
    inline float2 float4::yx() const { return float2(y, x); }
    inline float2 float4::yy() const { return float2(y, y); }
    inline float2 float4::yz() const { return float2(y, z); }
    inline float2 float4::yw() const { return float2(y, w); }
    inline float2 float4::zx() const { return float2(z, x); }
    inline float2 float4::zy() const { return float2(z, y); }
    inline float2 float4::zz() const { return float2(z, z); }
    inline float2 float4::zw() const { return float2(z, w); }
    inline float2 float4::wx() const { return float2(w, x); }
    inline float2 float4::wy() const { return float2(w, y); }
    inline float2 float4::wz() const { return float2(w, z); }
    inline float2 float4::ww() const { return float2(w, w); }
    inline float3 float4::xxx() const { return float3(x, x, x); }
    inline float3 float4::xxy() const { return float3(x, x, y); }
    inline float3 float4::xxz() const { return float3(x, x, z); }
    inline float3 float4::xxw() const { return float3(x, x, w); }
    inline float3 float4::xyx() const { return float3(x, y, x); }
    inline float3 float4::xyy() const { return float3(x, y, y); }
    inline float3 float4::xyz() const { return float3(x, y, z); }
    inline float3 float4::xyw() const { return float3(x, y, w); }
    inline float3 float4::xzx() const { return float3(x, z, x); }
    inline float3 float4::xzy() const { return float3(x, z, y); }
    inline float3 float4::xzz() const { return float3(x, z, z); }
    inline float3 float4::xzw() const { return float3(x, z, w); }
    inline float3 float4::xwx() const { return float3(x, w, x); }
    inline float3 float4::xwy() const { return float3(x, w, y); }
    inline float3 float4::xwz() const { return float3(x, w, z); }
    inline float3 float4::xww() const { return float3(x, w, w); }
    inline float3 float4::yxx() const { return float3(y, x, x); }
    inline float3 float4::yxy() const { return float3(y, x, y); }
    inline float3 float4::yxz() const { return float3(y, x, z); }
    inline float3 float4::yxw() const { return float3(y, x, w); }
    inline float3 float4::yyx() const { return float3(y, y, x); }
    inline float3 float4::yyy() const { return float3(y, y, y); }
    inline float3 float4::yyz() const { return float3(y, y, z); }
    inline float3 float4::yyw() const { return float3(y, y, w); }
    inline float3 float4::yzx() const { return float3(y, z, x); }
    inline float3 float4::yzy() const { return float3(y, z, y); }
    inline float3 float4::yzz() const { return float3(y, z, z); }
    inline float3 float4::yzw() const { return float3(y, z, w); }
    inline float3 float4::ywx() const { return float3(y, w, x); }
    inline float3 float4::ywy() const { return float3(y, w, y); }
    inline float3 float4::ywz() const { return float3(y, w, z); }
    inline float3 float4::yww() const { return float3(y, w, w); }
    inline float3 float4::zxx() const { return float3(z, x, x); }
    inline float3 float4::zxy() const { return float3(z, x, y); }
    inline float3 float4::zxz() const { return float3(z, x, z); }
    inline float3 float4::zxw() const { return float3(z, x, w); }
    inline float3 float4::zyx() const { return float3(z, y, x); }
    inline float3 float4::zyy() const { return float3(z, y, y); }
    inline float3 float4::zyz() const { return float3(z, y, z); }
    inline float3 float4::zyw() const { return float3(z, y, w); }
    inline float3 float4::zzx() const { return float3(z, z, x); }
    inline float3 float4::zzy() const { return float3(z, z, y); }
    inline float3 float4::zzz() const { return float3(z, z, z); }
    inline float3 float4::zzw() const { return float3(z, z, w); }
    inline float3 float4::zwx() const { return float3(z, w, x); }
    inline float3 float4::zwy() const { return float3(z, w, y); }
    inline float3 float4::zwz() const { return float3(z, w, z); }
    inline float3 float4::zww() const { return float3(z, w, w); }
    inline float3 float4::wxx() const { return float3(w, x, x); }
    inline float3 float4::wxy() const { return float3(w, x, y); }
    inline float3 float4::wxz() const { return float3(w, x, z); }
    inline float3 float4::wxw() const { return float3(w, x, w); }
    inline float3 float4::wyx() const { return float3(w, y, x); }
    inline float3 float4::wyy() const { return float3(w, y, y); }
    inline float3 float4::wyz() const { return float3(w, y, z); }
    inline float3 float4::wyw() const { return float3(w, y, w); }
    inline float3 float4::wzx() const { return float3(w, z, x); }
    inline float3 float4::wzy() const { return float3(w, z, y); }
    inline float3 float4::wzz() const { return float3(w, z, z); }
    inline float3 float4::wzw() const { return float3(w, z, w); }
    inline float3 float4::wwx() const { return float3(w, w, x); }
    inline float3 float4::wwy() const { return float3(w, w, y); }
    inline float3 float4::wwz() const { return float3(w, w, z); }
    inline float3 float4::www() const { return float3(w, w, w); }
    inline float4 float4::xxxx() const { return float4(x, x, x, x); }
    inline float4 float4::xxxy() const { return float4(x, x, x, y); }
    inline float4 float4::xxxz() const { return float4(x, x, x, z); }
    inline float4 float4::xxxw() const { return float4(x, x, x, w); }
    inline float4 float4::xxyx() const { return float4(x, x, y, x); }
    inline float4 float4::xxyy() const { return float4(x, x, y, y); }
    inline float4 float4::xxyz() const { return float4(x, x, y, z); }
    inline float4 float4::xxyw() const { return float4(x, x, y, w); }
    inline float4 float4::xxzx() const { return float4(x, x, z, x); }
    inline float4 float4::xxzy() const { return float4(x, x, z, y); }
    inline float4 float4::xxzz() const { return float4(x, x, z, z); }
    inline float4 float4::xxzw() const { return float4(x, x, z, w); }
    inline float4 float4::xxwx() const { return float4(x, x, w, x); }
    inline float4 float4::xxwy() const { return float4(x, x, w, y); }
    inline float4 float4::xxwz() const { return float4(x, x, w, z); }
    inline float4 float4::xxww() const { return float4(x, x, w, w); }
    inline float4 float4::xyxx() const { return float4(x, y, x, x); }
    inline float4 float4::xyxy() const { return float4(x, y, x, y); }
    inline float4 float4::xyxz() const { return float4(x, y, x, z); }
    inline float4 float4::xyxw() const { return float4(x, y, x, w); }
    inline float4 float4::xyyx() const { return float4(x, y, y, x); }
    inline float4 float4::xyyy() const { return float4(x, y, y, y); }
    inline float4 float4::xyyz() const { return float4(x, y, y, z); }
    inline float4 float4::xyyw() const { return float4(x, y, y, w); }
    inline float4 float4::xyzx() const { return float4(x, y, z, x); }
    inline float4 float4::xyzy() const { return float4(x, y, z, y); }
    inline float4 float4::xyzz() const { return float4(x, y, z, z); }
    inline float4 float4::xyzw() const { return float4(x, y, z, w); }
    inline float4 float4::xywx() const { return float4(x, y, w, x); }
    inline float4 float4::xywy() const { return float4(x, y, w, y); }
    inline float4 float4::xywz() const { return float4(x, y, w, z); }
    inline float4 float4::xyww() const { return float4(x, y, w, w); }
    inline float4 float4::xzxx() const { return float4(x, z, x, x); }
    inline float4 float4::xzxy() const { return float4(x, z, x, y); }
    inline float4 float4::xzxz() const { return float4(x, z, x, z); }
    inline float4 float4::xzxw() const { return float4(x, z, x, w); }
    inline float4 float4::xzyx() const { return float4(x, z, y, x); }
    inline float4 float4::xzyy() const { return float4(x, z, y, y); }
    inline float4 float4::xzyz() const { return float4(x, z, y, z); }
    inline float4 float4::xzyw() const { return float4(x, z, y, w); }
    inline float4 float4::xzzx() const { return float4(x, z, z, x); }
    inline float4 float4::xzzy() const { return float4(x, z, z, y); }
    inline float4 float4::xzzz() const { return float4(x, z, z, z); }
    inline float4 float4::xzzw() const { return float4(x, z, z, w); }
    inline float4 float4::xzwx() const { return float4(x, z, w, x); }
    inline float4 float4::xzwy() const { return float4(x, z, w, y); }
    inline float4 float4::xzwz() const { return float4(x, z, w, z); }
    inline float4 float4::xzww() const { return float4(x, z, w, w); }
    inline float4 float4::xwxx() const { return float4(x, w, x, x); }
    inline float4 float4::xwxy() const { return float4(x, w, x, y); }
    inline float4 float4::xwxz() const { return float4(x, w, x, z); }
    inline float4 float4::xwxw() const { return float4(x, w, x, w); }
    inline float4 float4::xwyx() const { return float4(x, w, y, x); }
    inline float4 float4::xwyy() const { return float4(x, w, y, y); }
    inline float4 float4::xwyz() const { return float4(x, w, y, z); }
    inline float4 float4::xwyw() const { return float4(x, w, y, w); }
    inline float4 float4::xwzx() const { return float4(x, w, z, x); }
    inline float4 float4::xwzy() const { return float4(x, w, z, y); }
    inline float4 float4::xwzz() const { return float4(x, w, z, z); }
    inline float4 float4::xwzw() const { return float4(x, w, z, w); }
    inline float4 float4::xwwx() const { return float4(x, w, w, x); }
    inline float4 float4::xwwy() const { return float4(x, w, w, y); }
    inline float4 float4::xwwz() const { return float4(x, w, w, z); }
    inline float4 float4::xwww() const { return float4(x, w, w, w); }
    inline float4 float4::yxxx() const { return float4(y, x, x, x); }
    inline float4 float4::yxxy() const { return float4(y, x, x, y); }
    inline float4 float4::yxxz() const { return float4(y, x, x, z); }
    inline float4 float4::yxxw() const { return float4(y, x, x, w); }
    inline float4 float4::yxyx() const { return float4(y, x, y, x); }
    inline float4 float4::yxyy() const { return float4(y, x, y, y); }
    inline float4 float4::yxyz() const { return float4(y, x, y, z); }
    inline float4 float4::yxyw() const { return float4(y, x, y, w); }
    inline float4 float4::yxzx() const { return float4(y, x, z, x); }
    inline float4 float4::yxzy() const { return float4(y, x, z, y); }
    inline float4 float4::yxzz() const { return float4(y, x, z, z); }
    inline float4 float4::yxzw() const { return float4(y, x, z, w); }
    inline float4 float4::yxwx() const { return float4(y, x, w, x); }
    inline float4 float4::yxwy() const { return float4(y, x, w, y); }
    inline float4 float4::yxwz() const { return float4(y, x, w, z); }
    inline float4 float4::yxww() const { return float4(y, x, w, w); }
    inline float4 float4::yyxx() const { return float4(y, y, x, x); }
    inline float4 float4::yyxy() const { return float4(y, y, x, y); }
    inline float4 float4::yyxz() const { return float4(y, y, x, z); }
    inline float4 float4::yyxw() const { return float4(y, y, x, w); }
    inline float4 float4::yyyx() const { return float4(y, y, y, x); }
    inline float4 float4::yyyy() const { return float4(y, y, y, y); }
    inline float4 float4::yyyz() const { return float4(y, y, y, z); }
    inline float4 float4::yyyw() const { return float4(y, y, y, w); }
    inline float4 float4::yyzx() const { return float4(y, y, z, x); }
    inline float4 float4::yyzy() const { return float4(y, y, z, y); }
    inline float4 float4::yyzz() const { return float4(y, y, z, z); }
    inline float4 float4::yyzw() const { return float4(y, y, z, w); }
    inline float4 float4::yywx() const { return float4(y, y, w, x); }
    inline float4 float4::yywy() const { return float4(y, y, w, y); }
    inline float4 float4::yywz() const { return float4(y, y, w, z); }
    inline float4 float4::yyww() const { return float4(y, y, w, w); }
    inline float4 float4::yzxx() const { return float4(y, z, x, x); }
    inline float4 float4::yzxy() const { return float4(y, z, x, y); }
    inline float4 float4::yzxz() const { return float4(y, z, x, z); }
    inline float4 float4::yzxw() const { return float4(y, z, x, w); }
    inline float4 float4::yzyx() const { return float4(y, z, y, x); }
    inline float4 float4::yzyy() const { return float4(y, z, y, y); }
    inline float4 float4::yzyz() const { return float4(y, z, y, z); }
    inline float4 float4::yzyw() const { return float4(y, z, y, w); }
    inline float4 float4::yzzx() const { return float4(y, z, z, x); }
    inline float4 float4::yzzy() const { return float4(y, z, z, y); }
    inline float4 float4::yzzz() const { return float4(y, z, z, z); }
    inline float4 float4::yzzw() const { return float4(y, z, z, w); }
    inline float4 float4::yzwx() const { return float4(y, z, w, x); }
    inline float4 float4::yzwy() const { return float4(y, z, w, y); }
    inline float4 float4::yzwz() const { return float4(y, z, w, z); }
    inline float4 float4::yzww() const { return float4(y, z, w, w); }
    inline float4 float4::ywxx() const { return float4(y, w, x, x); }
    inline float4 float4::ywxy() const { return float4(y, w, x, y); }
    inline float4 float4::ywxz() const { return float4(y, w, x, z); }
    inline float4 float4::ywxw() const { return float4(y, w, x, w); }
    inline float4 float4::ywyx() const { return float4(y, w, y, x); }
    inline float4 float4::ywyy() const { return float4(y, w, y, y); }
    inline float4 float4::ywyz() const { return float4(y, w, y, z); }
    inline float4 float4::ywyw() const { return float4(y, w, y, w); }
    inline float4 float4::ywzx() const { return float4(y, w, z, x); }
    inline float4 float4::ywzy() const { return float4(y, w, z, y); }
    inline float4 float4::ywzz() const { return float4(y, w, z, z); }
    inline float4 float4::ywzw() const { return float4(y, w, z, w); }
    inline float4 float4::ywwx() const { return float4(y, w, w, x); }
    inline float4 float4::ywwy() const { return float4(y, w, w, y); }
    inline float4 float4::ywwz() const { return float4(y, w, w, z); }
    inline float4 float4::ywww() const { return float4(y, w, w, w); }
    inline float4 float4::zxxx() const { return float4(z, x, x, x); }
    inline float4 float4::zxxy() const { return float4(z, x, x, y); }
    inline float4 float4::zxxz() const { return float4(z, x, x, z); }
    inline float4 float4::zxxw() const { return float4(z, x, x, w); }
    inline float4 float4::zxyx() const { return float4(z, x, y, x); }
    inline float4 float4::zxyy() const { return float4(z, x, y, y); }
    inline float4 float4::zxyz() const { return float4(z, x, y, z); }
    inline float4 float4::zxyw() const { return float4(z, x, y, w); }
    inline float4 float4::zxzx() const { return float4(z, x, z, x); }
    inline float4 float4::zxzy() const { return float4(z, x, z, y); }
    inline float4 float4::zxzz() const { return float4(z, x, z, z); }
    inline float4 float4::zxzw() const { return float4(z, x, z, w); }
    inline float4 float4::zxwx() const { return float4(z, x, w, x); }
    inline float4 float4::zxwy() const { return float4(z, x, w, y); }
    inline float4 float4::zxwz() const { return float4(z, x, w, z); }
    inline float4 float4::zxww() const { return float4(z, x, w, w); }
    inline float4 float4::zyxx() const { return float4(z, y, x, x); }
    inline float4 float4::zyxy() const { return float4(z, y, x, y); }
    inline float4 float4::zyxz() const { return float4(z, y, x, z); }
    inline float4 float4::zyxw() const { return float4(z, y, x, w); }
    inline float4 float4::zyyx() const { return float4(z, y, y, x); }
    inline float4 float4::zyyy() const { return float4(z, y, y, y); }
    inline float4 float4::zyyz() const { return float4(z, y, y, z); }
    inline float4 float4::zyyw() const { return float4(z, y, y, w); }
    inline float4 float4::zyzx() const { return float4(z, y, z, x); }
    inline float4 float4::zyzy() const { return float4(z, y, z, y); }
    inline float4 float4::zyzz() const { return float4(z, y, z, z); }
    inline float4 float4::zyzw() const { return float4(z, y, z, w); }
    inline float4 float4::zywx() const { return float4(z, y, w, x); }
    inline float4 float4::zywy() const { return float4(z, y, w, y); }
    inline float4 float4::zywz() const { return float4(z, y, w, z); }
    inline float4 float4::zyww() const { return float4(z, y, w, w); }
    inline float4 float4::zzxx() const { return float4(z, z, x, x); }
    inline float4 float4::zzxy() const { return float4(z, z, x, y); }
    inline float4 float4::zzxz() const { return float4(z, z, x, z); }
    inline float4 float4::zzxw() const { return float4(z, z, x, w); }
    inline float4 float4::zzyx() const { return float4(z, z, y, x); }
    inline float4 float4::zzyy() const { return float4(z, z, y, y); }
    inline float4 float4::zzyz() const { return float4(z, z, y, z); }
    inline float4 float4::zzyw() const { return float4(z, z, y, w); }
    inline float4 float4::zzzx() const { return float4(z, z, z, x); }
    inline float4 float4::zzzy() const { return float4(z, z, z, y); }
    inline float4 float4::zzzz() const { return float4(z, z, z, z); }
    inline float4 float4::zzzw() const { return float4(z, z, z, w); }
    inline float4 float4::zzwx() const { return float4(z, z, w, x); }
    inline float4 float4::zzwy() const { return float4(z, z, w, y); }
    inline float4 float4::zzwz() const { return float4(z, z, w, z); }
    inline float4 float4::zzww() const { return float4(z, z, w, w); }
    inline float4 float4::zwxx() const { return float4(z, w, x, x); }
    inline float4 float4::zwxy() const { return float4(z, w, x, y); }
    inline float4 float4::zwxz() const { return float4(z, w, x, z); }
    inline float4 float4::zwxw() const { return float4(z, w, x, w); }
    inline float4 float4::zwyx() const { return float4(z, w, y, x); }
    inline float4 float4::zwyy() const { return float4(z, w, y, y); }
    inline float4 float4::zwyz() const { return float4(z, w, y, z); }
    inline float4 float4::zwyw() const { return float4(z, w, y, w); }
    inline float4 float4::zwzx() const { return float4(z, w, z, x); }
    inline float4 float4::zwzy() const { return float4(z, w, z, y); }
    inline float4 float4::zwzz() const { return float4(z, w, z, z); }
    inline float4 float4::zwzw() const { return float4(z, w, z, w); }
    inline float4 float4::zwwx() const { return float4(z, w, w, x); }
    inline float4 float4::zwwy() const { return float4(z, w, w, y); }
    inline float4 float4::zwwz() const { return float4(z, w, w, z); }
    inline float4 float4::zwww() const { return float4(z, w, w, w); }
    inline float4 float4::wxxx() const { return float4(w, x, x, x); }
    inline float4 float4::wxxy() const { return float4(w, x, x, y); }
    inline float4 float4::wxxz() const { return float4(w, x, x, z); }
    inline float4 float4::wxxw() const { return float4(w, x, x, w); }
    inline float4 float4::wxyx() const { return float4(w, x, y, x); }
    inline float4 float4::wxyy() const { return float4(w, x, y, y); }
    inline float4 float4::wxyz() const { return float4(w, x, y, z); }
    inline float4 float4::wxyw() const { return float4(w, x, y, w); }
    inline float4 float4::wxzx() const { return float4(w, x, z, x); }
    inline float4 float4::wxzy() const { return float4(w, x, z, y); }
    inline float4 float4::wxzz() const { return float4(w, x, z, z); }
    inline float4 float4::wxzw() const { return float4(w, x, z, w); }
    inline float4 float4::wxwx() const { return float4(w, x, w, x); }
    inline float4 float4::wxwy() const { return float4(w, x, w, y); }
    inline float4 float4::wxwz() const { return float4(w, x, w, z); }
    inline float4 float4::wxww() const { return float4(w, x, w, w); }
    inline float4 float4::wyxx() const { return float4(w, y, x, x); }
    inline float4 float4::wyxy() const { return float4(w, y, x, y); }
    inline float4 float4::wyxz() const { return float4(w, y, x, z); }
    inline float4 float4::wyxw() const { return float4(w, y, x, w); }
    inline float4 float4::wyyx() const { return float4(w, y, y, x); }
    inline float4 float4::wyyy() const { return float4(w, y, y, y); }
    inline float4 float4::wyyz() const { return float4(w, y, y, z); }
    inline float4 float4::wyyw() const { return float4(w, y, y, w); }
    inline float4 float4::wyzx() const { return float4(w, y, z, x); }
    inline float4 float4::wyzy() const { return float4(w, y, z, y); }
    inline float4 float4::wyzz() const { return float4(w, y, z, z); }
    inline float4 float4::wyzw() const { return float4(w, y, z, w); }
    inline float4 float4::wywx() const { return float4(w, y, w, x); }
    inline float4 float4::wywy() const { return float4(w, y, w, y); }
    inline float4 float4::wywz() const { return float4(w, y, w, z); }
    inline float4 float4::wyww() const { return float4(w, y, w, w); }
    inline float4 float4::wzxx() const { return float4(w, z, x, x); }
    inline float4 float4::wzxy() const { return float4(w, z, x, y); }
    inline float4 float4::wzxz() const { return float4(w, z, x, z); }
    inline float4 float4::wzxw() const { return float4(w, z, x, w); }
    inline float4 float4::wzyx() const { return float4(w, z, y, x); }
    inline float4 float4::wzyy() const { return float4(w, z, y, y); }
    inline float4 float4::wzyz() const { return float4(w, z, y, z); }
    inline float4 float4::wzyw() const { return float4(w, z, y, w); }
    inline float4 float4::wzzx() const { return float4(w, z, z, x); }
    inline float4 float4::wzzy() const { return float4(w, z, z, y); }
    inline float4 float4::wzzz() const { return float4(w, z, z, z); }
    inline float4 float4::wzzw() const { return float4(w, z, z, w); }
    inline float4 float4::wzwx() const { return float4(w, z, w, x); }
    inline float4 float4::wzwy() const { return float4(w, z, w, y); }
    inline float4 float4::wzwz() const { return float4(w, z, w, z); }
    inline float4 float4::wzww() const { return float4(w, z, w, w); }
    inline float4 float4::wwxx() const { return float4(w, w, x, x); }
    inline float4 float4::wwxy() const { return float4(w, w, x, y); }
    inline float4 float4::wwxz() const { return float4(w, w, x, z); }
    inline float4 float4::wwxw() const { return float4(w, w, x, w); }
    inline float4 float4::wwyx() const { return float4(w, w, y, x); }
    inline float4 float4::wwyy() const { return float4(w, w, y, y); }
    inline float4 float4::wwyz() const { return float4(w, w, y, z); }
    inline float4 float4::wwyw() const { return float4(w, w, y, w); }
    inline float4 float4::wwzx() const { return float4(w, w, z, x); }
    inline float4 float4::wwzy() const { return float4(w, w, z, y); }
    inline float4 float4::wwzz() const { return float4(w, w, z, z); }
    inline float4 float4::wwzw() const { return float4(w, w, z, w); }
    inline float4 float4::wwwx() const { return float4(w, w, w, x); }
    inline float4 float4::wwwy() const { return float4(w, w, w, y); }
    inline float4 float4::wwwz() const { return float4(w, w, w, z); }
    inline float4 float4::wwww() const { return float4(w, w, w, w); }
    // int2 swizzle
    inline int2 int2::xx() const { return int2(x, x); }
    inline int2 int2::xy() const { return int2(x, y); }
    inline int2 int2::yx() const { return int2(y, x); }
    inline int2 int2::yy() const { return int2(y, y); }
    inline int3 int2::xxx() const { return int3(x, x, x); }
    inline int3 int2::xxy() const { return int3(x, x, y); }
    inline int3 int2::xyx() const { return int3(x, y, x); }
    inline int3 int2::xyy() const { return int3(x, y, y); }
    inline int3 int2::yxx() const { return int3(y, x, x); }
    inline int3 int2::yxy() const { return int3(y, x, y); }
    inline int3 int2::yyx() const { return int3(y, y, x); }
    inline int3 int2::yyy() const { return int3(y, y, y); }
    inline int4 int2::xxxx() const { return int4(x, x, x, x); }
    inline int4 int2::xxxy() const { return int4(x, x, x, y); }
    inline int4 int2::xxyx() const { return int4(x, x, y, x); }
    inline int4 int2::xxyy() const { return int4(x, x, y, y); }
    inline int4 int2::xyxx() const { return int4(x, y, x, x); }
    inline int4 int2::xyxy() const { return int4(x, y, x, y); }
    inline int4 int2::xyyx() const { return int4(x, y, y, x); }
    inline int4 int2::xyyy() const { return int4(x, y, y, y); }
    inline int4 int2::yxxx() const { return int4(y, x, x, x); }
    inline int4 int2::yxxy() const { return int4(y, x, x, y); }
    inline int4 int2::yxyx() const { return int4(y, x, y, x); }
    inline int4 int2::yxyy() const { return int4(y, x, y, y); }
    inline int4 int2::yyxx() const { return int4(y, y, x, x); }
    inline int4 int2::yyxy() const { return int4(y, y, x, y); }
    inline int4 int2::yyyx() const { return int4(y, y, y, x); }
    inline int4 int2::yyyy() const { return int4(y, y, y, y); }
    // int3 swizzle
    inline int2 int3::xx() const { return int2(x, x); }
    inline int2 int3::xy() const { return int2(x, y); }
    inline int2 int3::xz() const { return int2(x, z); }
    inline int2 int3::yx() const { return int2(y, x); }
    inline int2 int3::yy() const { return int2(y, y); }
    inline int2 int3::yz() const { return int2(y, z); }
    inline int2 int3::zx() const { return int2(z, x); }
    inline int2 int3::zy() const { return int2(z, y); }
    inline int2 int3::zz() const { return int2(z, z); }
    inline int3 int3::xxx() const { return int3(x, x, x); }
    inline int3 int3::xxy() const { return int3(x, x, y); }
    inline int3 int3::xxz() const { return int3(x, x, z); }
    inline int3 int3::xyx() const { return int3(x, y, x); }
    inline int3 int3::xyy() const { return int3(x, y, y); }
    inline int3 int3::xyz() const { return int3(x, y, z); }
    inline int3 int3::xzx() const { return int3(x, z, x); }
    inline int3 int3::xzy() const { return int3(x, z, y); }
    inline int3 int3::xzz() const { return int3(x, z, z); }
    inline int3 int3::yxx() const { return int3(y, x, x); }
    inline int3 int3::yxy() const { return int3(y, x, y); }
    inline int3 int3::yxz() const { return int3(y, x, z); }
    inline int3 int3::yyx() const { return int3(y, y, x); }
    inline int3 int3::yyy() const { return int3(y, y, y); }
    inline int3 int3::yyz() const { return int3(y, y, z); }
    inline int3 int3::yzx() const { return int3(y, z, x); }
    inline int3 int3::yzy() const { return int3(y, z, y); }
    inline int3 int3::yzz() const { return int3(y, z, z); }
    inline int3 int3::zxx() const { return int3(z, x, x); }
    inline int3 int3::zxy() const { return int3(z, x, y); }
    inline int3 int3::zxz() const { return int3(z, x, z); }
    inline int3 int3::zyx() const { return int3(z, y, x); }
    inline int3 int3::zyy() const { return int3(z, y, y); }
    inline int3 int3::zyz() const { return int3(z, y, z); }
    inline int3 int3::zzx() const { return int3(z, z, x); }
    inline int3 int3::zzy() const { return int3(z, z, y); }
    inline int3 int3::zzz() const { return int3(z, z, z); }
    inline int4 int3::xxxx() const { return int4(x, x, x, x); }
    inline int4 int3::xxxy() const { return int4(x, x, x, y); }
    inline int4 int3::xxxz() const { return int4(x, x, x, z); }
    inline int4 int3::xxyx() const { return int4(x, x, y, x); }
    inline int4 int3::xxyy() const { return int4(x, x, y, y); }
    inline int4 int3::xxyz() const { return int4(x, x, y, z); }
    inline int4 int3::xxzx() const { return int4(x, x, z, x); }
    inline int4 int3::xxzy() const { return int4(x, x, z, y); }
    inline int4 int3::xxzz() const { return int4(x, x, z, z); }
    inline int4 int3::xyxx() const { return int4(x, y, x, x); }
    inline int4 int3::xyxy() const { return int4(x, y, x, y); }
    inline int4 int3::xyxz() const { return int4(x, y, x, z); }
    inline int4 int3::xyyx() const { return int4(x, y, y, x); }
    inline int4 int3::xyyy() const { return int4(x, y, y, y); }
    inline int4 int3::xyyz() const { return int4(x, y, y, z); }
    inline int4 int3::xyzx() const { return int4(x, y, z, x); }
    inline int4 int3::xyzy() const { return int4(x, y, z, y); }
    inline int4 int3::xyzz() const { return int4(x, y, z, z); }
    inline int4 int3::xzxx() const { return int4(x, z, x, x); }
    inline int4 int3::xzxy() const { return int4(x, z, x, y); }
    inline int4 int3::xzxz() const { return int4(x, z, x, z); }
    inline int4 int3::xzyx() const { return int4(x, z, y, x); }
    inline int4 int3::xzyy() const { return int4(x, z, y, y); }
    inline int4 int3::xzyz() const { return int4(x, z, y, z); }
    inline int4 int3::xzzx() const { return int4(x, z, z, x); }
    inline int4 int3::xzzy() const { return int4(x, z, z, y); }
    inline int4 int3::xzzz() const { return int4(x, z, z, z); }
    inline int4 int3::yxxx() const { return int4(y, x, x, x); }
    inline int4 int3::yxxy() const { return int4(y, x, x, y); }
    inline int4 int3::yxxz() const { return int4(y, x, x, z); }
    inline int4 int3::yxyx() const { return int4(y, x, y, x); }
    inline int4 int3::yxyy() const { return int4(y, x, y, y); }
    inline int4 int3::yxyz() const { return int4(y, x, y, z); }
    inline int4 int3::yxzx() const { return int4(y, x, z, x); }
    inline int4 int3::yxzy() const { return int4(y, x, z, y); }
    inline int4 int3::yxzz() const { return int4(y, x, z, z); }
    inline int4 int3::yyxx() const { return int4(y, y, x, x); }
    inline int4 int3::yyxy() const { return int4(y, y, x, y); }
    inline int4 int3::yyxz() const { return int4(y, y, x, z); }
    inline int4 int3::yyyx() const { return int4(y, y, y, x); }
    inline int4 int3::yyyy() const { return int4(y, y, y, y); }
    inline int4 int3::yyyz() const { return int4(y, y, y, z); }
    inline int4 int3::yyzx() const { return int4(y, y, z, x); }
    inline int4 int3::yyzy() const { return int4(y, y, z, y); }
    inline int4 int3::yyzz() const { return int4(y, y, z, z); }
    inline int4 int3::yzxx() const { return int4(y, z, x, x); }
    inline int4 int3::yzxy() const { return int4(y, z, x, y); }
    inline int4 int3::yzxz() const { return int4(y, z, x, z); }
    inline int4 int3::yzyx() const { return int4(y, z, y, x); }
    inline int4 int3::yzyy() const { return int4(y, z, y, y); }
    inline int4 int3::yzyz() const { return int4(y, z, y, z); }
    inline int4 int3::yzzx() const { return int4(y, z, z, x); }
    inline int4 int3::yzzy() const { return int4(y, z, z, y); }
    inline int4 int3::yzzz() const { return int4(y, z, z, z); }
    inline int4 int3::zxxx() const { return int4(z, x, x, x); }
    inline int4 int3::zxxy() const { return int4(z, x, x, y); }
    inline int4 int3::zxxz() const { return int4(z, x, x, z); }
    inline int4 int3::zxyx() const { return int4(z, x, y, x); }
    inline int4 int3::zxyy() const { return int4(z, x, y, y); }
    inline int4 int3::zxyz() const { return int4(z, x, y, z); }
    inline int4 int3::zxzx() const { return int4(z, x, z, x); }
    inline int4 int3::zxzy() const { return int4(z, x, z, y); }
    inline int4 int3::zxzz() const { return int4(z, x, z, z); }
    inline int4 int3::zyxx() const { return int4(z, y, x, x); }
    inline int4 int3::zyxy() const { return int4(z, y, x, y); }
    inline int4 int3::zyxz() const { return int4(z, y, x, z); }
    inline int4 int3::zyyx() const { return int4(z, y, y, x); }
    inline int4 int3::zyyy() const { return int4(z, y, y, y); }
    inline int4 int3::zyyz() const { return int4(z, y, y, z); }
    inline int4 int3::zyzx() const { return int4(z, y, z, x); }
    inline int4 int3::zyzy() const { return int4(z, y, z, y); }
    inline int4 int3::zyzz() const { return int4(z, y, z, z); }
    inline int4 int3::zzxx() const { return int4(z, z, x, x); }
    inline int4 int3::zzxy() const { return int4(z, z, x, y); }
    inline int4 int3::zzxz() const { return int4(z, z, x, z); }
    inline int4 int3::zzyx() const { return int4(z, z, y, x); }
    inline int4 int3::zzyy() const { return int4(z, z, y, y); }
    inline int4 int3::zzyz() const { return int4(z, z, y, z); }
    inline int4 int3::zzzx() const { return int4(z, z, z, x); }
    inline int4 int3::zzzy() const { return int4(z, z, z, y); }
    inline int4 int3::zzzz() const { return int4(z, z, z, z); }
    // int4 swizzle
    inline int2 int4::xx() const { return int2(x, x); }
    inline int2 int4::xy() const { return int2(x, y); }
    inline int2 int4::xz() const { return int2(x, z); }
    inline int2 int4::xw() const { return int2(x, w); }
    inline int2 int4::yx() const { return int2(y, x); }
    inline int2 int4::yy() const { return int2(y, y); }
    inline int2 int4::yz() const { return int2(y, z); }
    inline int2 int4::yw() const { return int2(y, w); }
    inline int2 int4::zx() const { return int2(z, x); }
    inline int2 int4::zy() const { return int2(z, y); }
    inline int2 int4::zz() const { return int2(z, z); }
    inline int2 int4::zw() const { return int2(z, w); }
    inline int2 int4::wx() const { return int2(w, x); }
    inline int2 int4::wy() const { return int2(w, y); }
    inline int2 int4::wz() const { return int2(w, z); }
    inline int2 int4::ww() const { return int2(w, w); }
    inline int3 int4::xxx() const { return int3(x, x, x); }
    inline int3 int4::xxy() const { return int3(x, x, y); }
    inline int3 int4::xxz() const { return int3(x, x, z); }
    inline int3 int4::xxw() const { return int3(x, x, w); }
    inline int3 int4::xyx() const { return int3(x, y, x); }
    inline int3 int4::xyy() const { return int3(x, y, y); }
    inline int3 int4::xyz() const { return int3(x, y, z); }
    inline int3 int4::xyw() const { return int3(x, y, w); }
    inline int3 int4::xzx() const { return int3(x, z, x); }
    inline int3 int4::xzy() const { return int3(x, z, y); }
    inline int3 int4::xzz() const { return int3(x, z, z); }
    inline int3 int4::xzw() const { return int3(x, z, w); }
    inline int3 int4::xwx() const { return int3(x, w, x); }
    inline int3 int4::xwy() const { return int3(x, w, y); }
    inline int3 int4::xwz() const { return int3(x, w, z); }
    inline int3 int4::xww() const { return int3(x, w, w); }
    inline int3 int4::yxx() const { return int3(y, x, x); }
    inline int3 int4::yxy() const { return int3(y, x, y); }
    inline int3 int4::yxz() const { return int3(y, x, z); }
    inline int3 int4::yxw() const { return int3(y, x, w); }
    inline int3 int4::yyx() const { return int3(y, y, x); }
    inline int3 int4::yyy() const { return int3(y, y, y); }
    inline int3 int4::yyz() const { return int3(y, y, z); }
    inline int3 int4::yyw() const { return int3(y, y, w); }
    inline int3 int4::yzx() const { return int3(y, z, x); }
    inline int3 int4::yzy() const { return int3(y, z, y); }
    inline int3 int4::yzz() const { return int3(y, z, z); }
    inline int3 int4::yzw() const { return int3(y, z, w); }
    inline int3 int4::ywx() const { return int3(y, w, x); }
    inline int3 int4::ywy() const { return int3(y, w, y); }
    inline int3 int4::ywz() const { return int3(y, w, z); }
    inline int3 int4::yww() const { return int3(y, w, w); }
    inline int3 int4::zxx() const { return int3(z, x, x); }
    inline int3 int4::zxy() const { return int3(z, x, y); }
    inline int3 int4::zxz() const { return int3(z, x, z); }
    inline int3 int4::zxw() const { return int3(z, x, w); }
    inline int3 int4::zyx() const { return int3(z, y, x); }
    inline int3 int4::zyy() const { return int3(z, y, y); }
    inline int3 int4::zyz() const { return int3(z, y, z); }
    inline int3 int4::zyw() const { return int3(z, y, w); }
    inline int3 int4::zzx() const { return int3(z, z, x); }
    inline int3 int4::zzy() const { return int3(z, z, y); }
    inline int3 int4::zzz() const { return int3(z, z, z); }
    inline int3 int4::zzw() const { return int3(z, z, w); }
    inline int3 int4::zwx() const { return int3(z, w, x); }
    inline int3 int4::zwy() const { return int3(z, w, y); }
    inline int3 int4::zwz() const { return int3(z, w, z); }
    inline int3 int4::zww() const { return int3(z, w, w); }
    inline int3 int4::wxx() const { return int3(w, x, x); }
    inline int3 int4::wxy() const { return int3(w, x, y); }
    inline int3 int4::wxz() const { return int3(w, x, z); }
    inline int3 int4::wxw() const { return int3(w, x, w); }
    inline int3 int4::wyx() const { return int3(w, y, x); }
    inline int3 int4::wyy() const { return int3(w, y, y); }
    inline int3 int4::wyz() const { return int3(w, y, z); }
    inline int3 int4::wyw() const { return int3(w, y, w); }
    inline int3 int4::wzx() const { return int3(w, z, x); }
    inline int3 int4::wzy() const { return int3(w, z, y); }
    inline int3 int4::wzz() const { return int3(w, z, z); }
    inline int3 int4::wzw() const { return int3(w, z, w); }
    inline int3 int4::wwx() const { return int3(w, w, x); }
    inline int3 int4::wwy() const { return int3(w, w, y); }
    inline int3 int4::wwz() const { return int3(w, w, z); }
    inline int3 int4::www() const { return int3(w, w, w); }
    inline int4 int4::xxxx() const { return int4(x, x, x, x); }
    inline int4 int4::xxxy() const { return int4(x, x, x, y); }
    inline int4 int4::xxxz() const { return int4(x, x, x, z); }
    inline int4 int4::xxxw() const { return int4(x, x, x, w); }
    inline int4 int4::xxyx() const { return int4(x, x, y, x); }
    inline int4 int4::xxyy() const { return int4(x, x, y, y); }
    inline int4 int4::xxyz() const { return int4(x, x, y, z); }
    inline int4 int4::xxyw() const { return int4(x, x, y, w); }
    inline int4 int4::xxzx() const { return int4(x, x, z, x); }
    inline int4 int4::xxzy() const { return int4(x, x, z, y); }
    inline int4 int4::xxzz() const { return int4(x, x, z, z); }
    inline int4 int4::xxzw() const { return int4(x, x, z, w); }
    inline int4 int4::xxwx() const { return int4(x, x, w, x); }
    inline int4 int4::xxwy() const { return int4(x, x, w, y); }
    inline int4 int4::xxwz() const { return int4(x, x, w, z); }
    inline int4 int4::xxww() const { return int4(x, x, w, w); }
    inline int4 int4::xyxx() const { return int4(x, y, x, x); }
    inline int4 int4::xyxy() const { return int4(x, y, x, y); }
    inline int4 int4::xyxz() const { return int4(x, y, x, z); }
    inline int4 int4::xyxw() const { return int4(x, y, x, w); }
    inline int4 int4::xyyx() const { return int4(x, y, y, x); }
    inline int4 int4::xyyy() const { return int4(x, y, y, y); }
    inline int4 int4::xyyz() const { return int4(x, y, y, z); }
    inline int4 int4::xyyw() const { return int4(x, y, y, w); }
    inline int4 int4::xyzx() const { return int4(x, y, z, x); }
    inline int4 int4::xyzy() const { return int4(x, y, z, y); }
    inline int4 int4::xyzz() const { return int4(x, y, z, z); }
    inline int4 int4::xyzw() const { return int4(x, y, z, w); }
    inline int4 int4::xywx() const { return int4(x, y, w, x); }
    inline int4 int4::xywy() const { return int4(x, y, w, y); }
    inline int4 int4::xywz() const { return int4(x, y, w, z); }
    inline int4 int4::xyww() const { return int4(x, y, w, w); }
    inline int4 int4::xzxx() const { return int4(x, z, x, x); }
    inline int4 int4::xzxy() const { return int4(x, z, x, y); }
    inline int4 int4::xzxz() const { return int4(x, z, x, z); }
    inline int4 int4::xzxw() const { return int4(x, z, x, w); }
    inline int4 int4::xzyx() const { return int4(x, z, y, x); }
    inline int4 int4::xzyy() const { return int4(x, z, y, y); }
    inline int4 int4::xzyz() const { return int4(x, z, y, z); }
    inline int4 int4::xzyw() const { return int4(x, z, y, w); }
    inline int4 int4::xzzx() const { return int4(x, z, z, x); }
    inline int4 int4::xzzy() const { return int4(x, z, z, y); }
    inline int4 int4::xzzz() const { return int4(x, z, z, z); }
    inline int4 int4::xzzw() const { return int4(x, z, z, w); }
    inline int4 int4::xzwx() const { return int4(x, z, w, x); }
    inline int4 int4::xzwy() const { return int4(x, z, w, y); }
    inline int4 int4::xzwz() const { return int4(x, z, w, z); }
    inline int4 int4::xzww() const { return int4(x, z, w, w); }
    inline int4 int4::xwxx() const { return int4(x, w, x, x); }
    inline int4 int4::xwxy() const { return int4(x, w, x, y); }
    inline int4 int4::xwxz() const { return int4(x, w, x, z); }
    inline int4 int4::xwxw() const { return int4(x, w, x, w); }
    inline int4 int4::xwyx() const { return int4(x, w, y, x); }
    inline int4 int4::xwyy() const { return int4(x, w, y, y); }
    inline int4 int4::xwyz() const { return int4(x, w, y, z); }
    inline int4 int4::xwyw() const { return int4(x, w, y, w); }
    inline int4 int4::xwzx() const { return int4(x, w, z, x); }
    inline int4 int4::xwzy() const { return int4(x, w, z, y); }
    inline int4 int4::xwzz() const { return int4(x, w, z, z); }
    inline int4 int4::xwzw() const { return int4(x, w, z, w); }
    inline int4 int4::xwwx() const { return int4(x, w, w, x); }
    inline int4 int4::xwwy() const { return int4(x, w, w, y); }
    inline int4 int4::xwwz() const { return int4(x, w, w, z); }
    inline int4 int4::xwww() const { return int4(x, w, w, w); }
    inline int4 int4::yxxx() const { return int4(y, x, x, x); }
    inline int4 int4::yxxy() const { return int4(y, x, x, y); }
    inline int4 int4::yxxz() const { return int4(y, x, x, z); }
    inline int4 int4::yxxw() const { return int4(y, x, x, w); }
    inline int4 int4::yxyx() const { return int4(y, x, y, x); }
    inline int4 int4::yxyy() const { return int4(y, x, y, y); }
    inline int4 int4::yxyz() const { return int4(y, x, y, z); }
    inline int4 int4::yxyw() const { return int4(y, x, y, w); }
    inline int4 int4::yxzx() const { return int4(y, x, z, x); }
    inline int4 int4::yxzy() const { return int4(y, x, z, y); }
    inline int4 int4::yxzz() const { return int4(y, x, z, z); }
    inline int4 int4::yxzw() const { return int4(y, x, z, w); }
    inline int4 int4::yxwx() const { return int4(y, x, w, x); }
    inline int4 int4::yxwy() const { return int4(y, x, w, y); }
    inline int4 int4::yxwz() const { return int4(y, x, w, z); }
    inline int4 int4::yxww() const { return int4(y, x, w, w); }
    inline int4 int4::yyxx() const { return int4(y, y, x, x); }
    inline int4 int4::yyxy() const { return int4(y, y, x, y); }
    inline int4 int4::yyxz() const { return int4(y, y, x, z); }
    inline int4 int4::yyxw() const { return int4(y, y, x, w); }
    inline int4 int4::yyyx() const { return int4(y, y, y, x); }
    inline int4 int4::yyyy() const { return int4(y, y, y, y); }
    inline int4 int4::yyyz() const { return int4(y, y, y, z); }
    inline int4 int4::yyyw() const { return int4(y, y, y, w); }
    inline int4 int4::yyzx() const { return int4(y, y, z, x); }
    inline int4 int4::yyzy() const { return int4(y, y, z, y); }
    inline int4 int4::yyzz() const { return int4(y, y, z, z); }
    inline int4 int4::yyzw() const { return int4(y, y, z, w); }
    inline int4 int4::yywx() const { return int4(y, y, w, x); }
    inline int4 int4::yywy() const { return int4(y, y, w, y); }
    inline int4 int4::yywz() const { return int4(y, y, w, z); }
    inline int4 int4::yyww() const { return int4(y, y, w, w); }
    inline int4 int4::yzxx() const { return int4(y, z, x, x); }
    inline int4 int4::yzxy() const { return int4(y, z, x, y); }
    inline int4 int4::yzxz() const { return int4(y, z, x, z); }
    inline int4 int4::yzxw() const { return int4(y, z, x, w); }
    inline int4 int4::yzyx() const { return int4(y, z, y, x); }
    inline int4 int4::yzyy() const { return int4(y, z, y, y); }
    inline int4 int4::yzyz() const { return int4(y, z, y, z); }
    inline int4 int4::yzyw() const { return int4(y, z, y, w); }
    inline int4 int4::yzzx() const { return int4(y, z, z, x); }
    inline int4 int4::yzzy() const { return int4(y, z, z, y); }
    inline int4 int4::yzzz() const { return int4(y, z, z, z); }
    inline int4 int4::yzzw() const { return int4(y, z, z, w); }
    inline int4 int4::yzwx() const { return int4(y, z, w, x); }
    inline int4 int4::yzwy() const { return int4(y, z, w, y); }
    inline int4 int4::yzwz() const { return int4(y, z, w, z); }
    inline int4 int4::yzww() const { return int4(y, z, w, w); }
    inline int4 int4::ywxx() const { return int4(y, w, x, x); }
    inline int4 int4::ywxy() const { return int4(y, w, x, y); }
    inline int4 int4::ywxz() const { return int4(y, w, x, z); }
    inline int4 int4::ywxw() const { return int4(y, w, x, w); }
    inline int4 int4::ywyx() const { return int4(y, w, y, x); }
    inline int4 int4::ywyy() const { return int4(y, w, y, y); }
    inline int4 int4::ywyz() const { return int4(y, w, y, z); }
    inline int4 int4::ywyw() const { return int4(y, w, y, w); }
    inline int4 int4::ywzx() const { return int4(y, w, z, x); }
    inline int4 int4::ywzy() const { return int4(y, w, z, y); }
    inline int4 int4::ywzz() const { return int4(y, w, z, z); }
    inline int4 int4::ywzw() const { return int4(y, w, z, w); }
    inline int4 int4::ywwx() const { return int4(y, w, w, x); }
    inline int4 int4::ywwy() const { return int4(y, w, w, y); }
    inline int4 int4::ywwz() const { return int4(y, w, w, z); }
    inline int4 int4::ywww() const { return int4(y, w, w, w); }
    inline int4 int4::zxxx() const { return int4(z, x, x, x); }
    inline int4 int4::zxxy() const { return int4(z, x, x, y); }
    inline int4 int4::zxxz() const { return int4(z, x, x, z); }
    inline int4 int4::zxxw() const { return int4(z, x, x, w); }
    inline int4 int4::zxyx() const { return int4(z, x, y, x); }
    inline int4 int4::zxyy() const { return int4(z, x, y, y); }
    inline int4 int4::zxyz() const { return int4(z, x, y, z); }
    inline int4 int4::zxyw() const { return int4(z, x, y, w); }
    inline int4 int4::zxzx() const { return int4(z, x, z, x); }
    inline int4 int4::zxzy() const { return int4(z, x, z, y); }
    inline int4 int4::zxzz() const { return int4(z, x, z, z); }
    inline int4 int4::zxzw() const { return int4(z, x, z, w); }
    inline int4 int4::zxwx() const { return int4(z, x, w, x); }
    inline int4 int4::zxwy() const { return int4(z, x, w, y); }
    inline int4 int4::zxwz() const { return int4(z, x, w, z); }
    inline int4 int4::zxww() const { return int4(z, x, w, w); }
    inline int4 int4::zyxx() const { return int4(z, y, x, x); }
    inline int4 int4::zyxy() const { return int4(z, y, x, y); }
    inline int4 int4::zyxz() const { return int4(z, y, x, z); }
    inline int4 int4::zyxw() const { return int4(z, y, x, w); }
    inline int4 int4::zyyx() const { return int4(z, y, y, x); }
    inline int4 int4::zyyy() const { return int4(z, y, y, y); }
    inline int4 int4::zyyz() const { return int4(z, y, y, z); }
    inline int4 int4::zyyw() const { return int4(z, y, y, w); }
    inline int4 int4::zyzx() const { return int4(z, y, z, x); }
    inline int4 int4::zyzy() const { return int4(z, y, z, y); }
    inline int4 int4::zyzz() const { return int4(z, y, z, z); }
    inline int4 int4::zyzw() const { return int4(z, y, z, w); }
    inline int4 int4::zywx() const { return int4(z, y, w, x); }
    inline int4 int4::zywy() const { return int4(z, y, w, y); }
    inline int4 int4::zywz() const { return int4(z, y, w, z); }
    inline int4 int4::zyww() const { return int4(z, y, w, w); }
    inline int4 int4::zzxx() const { return int4(z, z, x, x); }
    inline int4 int4::zzxy() const { return int4(z, z, x, y); }
    inline int4 int4::zzxz() const { return int4(z, z, x, z); }
    inline int4 int4::zzxw() const { return int4(z, z, x, w); }
    inline int4 int4::zzyx() const { return int4(z, z, y, x); }
    inline int4 int4::zzyy() const { return int4(z, z, y, y); }
    inline int4 int4::zzyz() const { return int4(z, z, y, z); }
    inline int4 int4::zzyw() const { return int4(z, z, y, w); }
    inline int4 int4::zzzx() const { return int4(z, z, z, x); }
    inline int4 int4::zzzy() const { return int4(z, z, z, y); }
    inline int4 int4::zzzz() const { return int4(z, z, z, z); }
    inline int4 int4::zzzw() const { return int4(z, z, z, w); }
    inline int4 int4::zzwx() const { return int4(z, z, w, x); }
    inline int4 int4::zzwy() const { return int4(z, z, w, y); }
    inline int4 int4::zzwz() const { return int4(z, z, w, z); }
    inline int4 int4::zzww() const { return int4(z, z, w, w); }
    inline int4 int4::zwxx() const { return int4(z, w, x, x); }
    inline int4 int4::zwxy() const { return int4(z, w, x, y); }
    inline int4 int4::zwxz() const { return int4(z, w, x, z); }
    inline int4 int4::zwxw() const { return int4(z, w, x, w); }
    inline int4 int4::zwyx() const { return int4(z, w, y, x); }
    inline int4 int4::zwyy() const { return int4(z, w, y, y); }
    inline int4 int4::zwyz() const { return int4(z, w, y, z); }
    inline int4 int4::zwyw() const { return int4(z, w, y, w); }
    inline int4 int4::zwzx() const { return int4(z, w, z, x); }
    inline int4 int4::zwzy() const { return int4(z, w, z, y); }
    inline int4 int4::zwzz() const { return int4(z, w, z, z); }
    inline int4 int4::zwzw() const { return int4(z, w, z, w); }
    inline int4 int4::zwwx() const { return int4(z, w, w, x); }
    inline int4 int4::zwwy() const { return int4(z, w, w, y); }
    inline int4 int4::zwwz() const { return int4(z, w, w, z); }
    inline int4 int4::zwww() const { return int4(z, w, w, w); }
    inline int4 int4::wxxx() const { return int4(w, x, x, x); }
    inline int4 int4::wxxy() const { return int4(w, x, x, y); }
    inline int4 int4::wxxz() const { return int4(w, x, x, z); }
    inline int4 int4::wxxw() const { return int4(w, x, x, w); }
    inline int4 int4::wxyx() const { return int4(w, x, y, x); }
    inline int4 int4::wxyy() const { return int4(w, x, y, y); }
    inline int4 int4::wxyz() const { return int4(w, x, y, z); }
    inline int4 int4::wxyw() const { return int4(w, x, y, w); }
    inline int4 int4::wxzx() const { return int4(w, x, z, x); }
    inline int4 int4::wxzy() const { return int4(w, x, z, y); }
    inline int4 int4::wxzz() const { return int4(w, x, z, z); }
    inline int4 int4::wxzw() const { return int4(w, x, z, w); }
    inline int4 int4::wxwx() const { return int4(w, x, w, x); }
    inline int4 int4::wxwy() const { return int4(w, x, w, y); }
    inline int4 int4::wxwz() const { return int4(w, x, w, z); }
    inline int4 int4::wxww() const { return int4(w, x, w, w); }
    inline int4 int4::wyxx() const { return int4(w, y, x, x); }
    inline int4 int4::wyxy() const { return int4(w, y, x, y); }
    inline int4 int4::wyxz() const { return int4(w, y, x, z); }
    inline int4 int4::wyxw() const { return int4(w, y, x, w); }
    inline int4 int4::wyyx() const { return int4(w, y, y, x); }
    inline int4 int4::wyyy() const { return int4(w, y, y, y); }
    inline int4 int4::wyyz() const { return int4(w, y, y, z); }
    inline int4 int4::wyyw() const { return int4(w, y, y, w); }
    inline int4 int4::wyzx() const { return int4(w, y, z, x); }
    inline int4 int4::wyzy() const { return int4(w, y, z, y); }
    inline int4 int4::wyzz() const { return int4(w, y, z, z); }
    inline int4 int4::wyzw() const { return int4(w, y, z, w); }
    inline int4 int4::wywx() const { return int4(w, y, w, x); }
    inline int4 int4::wywy() const { return int4(w, y, w, y); }
    inline int4 int4::wywz() const { return int4(w, y, w, z); }
    inline int4 int4::wyww() const { return int4(w, y, w, w); }
    inline int4 int4::wzxx() const { return int4(w, z, x, x); }
    inline int4 int4::wzxy() const { return int4(w, z, x, y); }
    inline int4 int4::wzxz() const { return int4(w, z, x, z); }
    inline int4 int4::wzxw() const { return int4(w, z, x, w); }
    inline int4 int4::wzyx() const { return int4(w, z, y, x); }
    inline int4 int4::wzyy() const { return int4(w, z, y, y); }
    inline int4 int4::wzyz() const { return int4(w, z, y, z); }
    inline int4 int4::wzyw() const { return int4(w, z, y, w); }
    inline int4 int4::wzzx() const { return int4(w, z, z, x); }
    inline int4 int4::wzzy() const { return int4(w, z, z, y); }
    inline int4 int4::wzzz() const { return int4(w, z, z, z); }
    inline int4 int4::wzzw() const { return int4(w, z, z, w); }
    inline int4 int4::wzwx() const { return int4(w, z, w, x); }
    inline int4 int4::wzwy() const { return int4(w, z, w, y); }
    inline int4 int4::wzwz() const { return int4(w, z, w, z); }
    inline int4 int4::wzww() const { return int4(w, z, w, w); }
    inline int4 int4::wwxx() const { return int4(w, w, x, x); }
    inline int4 int4::wwxy() const { return int4(w, w, x, y); }
    inline int4 int4::wwxz() const { return int4(w, w, x, z); }
    inline int4 int4::wwxw() const { return int4(w, w, x, w); }
    inline int4 int4::wwyx() const { return int4(w, w, y, x); }
    inline int4 int4::wwyy() const { return int4(w, w, y, y); }
    inline int4 int4::wwyz() const { return int4(w, w, y, z); }
    inline int4 int4::wwyw() const { return int4(w, w, y, w); }
    inline int4 int4::wwzx() const { return int4(w, w, z, x); }
    inline int4 int4::wwzy() const { return int4(w, w, z, y); }
    inline int4 int4::wwzz() const { return int4(w, w, z, z); }
    inline int4 int4::wwzw() const { return int4(w, w, z, w); }
    inline int4 int4::wwwx() const { return int4(w, w, w, x); }
    inline int4 int4::wwwy() const { return int4(w, w, w, y); }
    inline int4 int4::wwwz() const { return int4(w, w, w, z); }
    inline int4 int4::wwww() const { return int4(w, w, w, w); }
    // uint2 swizzle
    inline uint2 uint2::xx() const { return uint2(x, x); }
    inline uint2 uint2::xy() const { return uint2(x, y); }
    inline uint2 uint2::yx() const { return uint2(y, x); }
    inline uint2 uint2::yy() const { return uint2(y, y); }
    inline uint3 uint2::xxx() const { return uint3(x, x, x); }
    inline uint3 uint2::xxy() const { return uint3(x, x, y); }
    inline uint3 uint2::xyx() const { return uint3(x, y, x); }
    inline uint3 uint2::xyy() const { return uint3(x, y, y); }
    inline uint3 uint2::yxx() const { return uint3(y, x, x); }
    inline uint3 uint2::yxy() const { return uint3(y, x, y); }
    inline uint3 uint2::yyx() const { return uint3(y, y, x); }
    inline uint3 uint2::yyy() const { return uint3(y, y, y); }
    inline uint4 uint2::xxxx() const { return uint4(x, x, x, x); }
    inline uint4 uint2::xxxy() const { return uint4(x, x, x, y); }
    inline uint4 uint2::xxyx() const { return uint4(x, x, y, x); }
    inline uint4 uint2::xxyy() const { return uint4(x, x, y, y); }
    inline uint4 uint2::xyxx() const { return uint4(x, y, x, x); }
    inline uint4 uint2::xyxy() const { return uint4(x, y, x, y); }
    inline uint4 uint2::xyyx() const { return uint4(x, y, y, x); }
    inline uint4 uint2::xyyy() const { return uint4(x, y, y, y); }
    inline uint4 uint2::yxxx() const { return uint4(y, x, x, x); }
    inline uint4 uint2::yxxy() const { return uint4(y, x, x, y); }
    inline uint4 uint2::yxyx() const { return uint4(y, x, y, x); }
    inline uint4 uint2::yxyy() const { return uint4(y, x, y, y); }
    inline uint4 uint2::yyxx() const { return uint4(y, y, x, x); }
    inline uint4 uint2::yyxy() const { return uint4(y, y, x, y); }
    inline uint4 uint2::yyyx() const { return uint4(y, y, y, x); }
    inline uint4 uint2::yyyy() const { return uint4(y, y, y, y); }
    // uint3 swizzle
    inline uint2 uint3::xx() const { return uint2(x, x); }
    inline uint2 uint3::xy() const { return uint2(x, y); }
    inline uint2 uint3::xz() const { return uint2(x, z); }
    inline uint2 uint3::yx() const { return uint2(y, x); }
    inline uint2 uint3::yy() const { return uint2(y, y); }
    inline uint2 uint3::yz() const { return uint2(y, z); }
    inline uint2 uint3::zx() const { return uint2(z, x); }
    inline uint2 uint3::zy() const { return uint2(z, y); }
    inline uint2 uint3::zz() const { return uint2(z, z); }
    inline uint3 uint3::xxx() const { return uint3(x, x, x); }
    inline uint3 uint3::xxy() const { return uint3(x, x, y); }
    inline uint3 uint3::xxz() const { return uint3(x, x, z); }
    inline uint3 uint3::xyx() const { return uint3(x, y, x); }
    inline uint3 uint3::xyy() const { return uint3(x, y, y); }
    inline uint3 uint3::xyz() const { return uint3(x, y, z); }
    inline uint3 uint3::xzx() const { return uint3(x, z, x); }
    inline uint3 uint3::xzy() const { return uint3(x, z, y); }
    inline uint3 uint3::xzz() const { return uint3(x, z, z); }
    inline uint3 uint3::yxx() const { return uint3(y, x, x); }
    inline uint3 uint3::yxy() const { return uint3(y, x, y); }
    inline uint3 uint3::yxz() const { return uint3(y, x, z); }
    inline uint3 uint3::yyx() const { return uint3(y, y, x); }
    inline uint3 uint3::yyy() const { return uint3(y, y, y); }
    inline uint3 uint3::yyz() const { return uint3(y, y, z); }
    inline uint3 uint3::yzx() const { return uint3(y, z, x); }
    inline uint3 uint3::yzy() const { return uint3(y, z, y); }
    inline uint3 uint3::yzz() const { return uint3(y, z, z); }
    inline uint3 uint3::zxx() const { return uint3(z, x, x); }
    inline uint3 uint3::zxy() const { return uint3(z, x, y); }
    inline uint3 uint3::zxz() const { return uint3(z, x, z); }
    inline uint3 uint3::zyx() const { return uint3(z, y, x); }
    inline uint3 uint3::zyy() const { return uint3(z, y, y); }
    inline uint3 uint3::zyz() const { return uint3(z, y, z); }
    inline uint3 uint3::zzx() const { return uint3(z, z, x); }
    inline uint3 uint3::zzy() const { return uint3(z, z, y); }
    inline uint3 uint3::zzz() const { return uint3(z, z, z); }
    inline uint4 uint3::xxxx() const { return uint4(x, x, x, x); }
    inline uint4 uint3::xxxy() const { return uint4(x, x, x, y); }
    inline uint4 uint3::xxxz() const { return uint4(x, x, x, z); }
    inline uint4 uint3::xxyx() const { return uint4(x, x, y, x); }
    inline uint4 uint3::xxyy() const { return uint4(x, x, y, y); }
    inline uint4 uint3::xxyz() const { return uint4(x, x, y, z); }
    inline uint4 uint3::xxzx() const { return uint4(x, x, z, x); }
    inline uint4 uint3::xxzy() const { return uint4(x, x, z, y); }
    inline uint4 uint3::xxzz() const { return uint4(x, x, z, z); }
    inline uint4 uint3::xyxx() const { return uint4(x, y, x, x); }
    inline uint4 uint3::xyxy() const { return uint4(x, y, x, y); }
    inline uint4 uint3::xyxz() const { return uint4(x, y, x, z); }
    inline uint4 uint3::xyyx() const { return uint4(x, y, y, x); }
    inline uint4 uint3::xyyy() const { return uint4(x, y, y, y); }
    inline uint4 uint3::xyyz() const { return uint4(x, y, y, z); }
    inline uint4 uint3::xyzx() const { return uint4(x, y, z, x); }
    inline uint4 uint3::xyzy() const { return uint4(x, y, z, y); }
    inline uint4 uint3::xyzz() const { return uint4(x, y, z, z); }
    inline uint4 uint3::xzxx() const { return uint4(x, z, x, x); }
    inline uint4 uint3::xzxy() const { return uint4(x, z, x, y); }
    inline uint4 uint3::xzxz() const { return uint4(x, z, x, z); }
    inline uint4 uint3::xzyx() const { return uint4(x, z, y, x); }
    inline uint4 uint3::xzyy() const { return uint4(x, z, y, y); }
    inline uint4 uint3::xzyz() const { return uint4(x, z, y, z); }
    inline uint4 uint3::xzzx() const { return uint4(x, z, z, x); }
    inline uint4 uint3::xzzy() const { return uint4(x, z, z, y); }
    inline uint4 uint3::xzzz() const { return uint4(x, z, z, z); }
    inline uint4 uint3::yxxx() const { return uint4(y, x, x, x); }
    inline uint4 uint3::yxxy() const { return uint4(y, x, x, y); }
    inline uint4 uint3::yxxz() const { return uint4(y, x, x, z); }
    inline uint4 uint3::yxyx() const { return uint4(y, x, y, x); }
    inline uint4 uint3::yxyy() const { return uint4(y, x, y, y); }
    inline uint4 uint3::yxyz() const { return uint4(y, x, y, z); }
    inline uint4 uint3::yxzx() const { return uint4(y, x, z, x); }
    inline uint4 uint3::yxzy() const { return uint4(y, x, z, y); }
    inline uint4 uint3::yxzz() const { return uint4(y, x, z, z); }
    inline uint4 uint3::yyxx() const { return uint4(y, y, x, x); }
    inline uint4 uint3::yyxy() const { return uint4(y, y, x, y); }
    inline uint4 uint3::yyxz() const { return uint4(y, y, x, z); }
    inline uint4 uint3::yyyx() const { return uint4(y, y, y, x); }
    inline uint4 uint3::yyyy() const { return uint4(y, y, y, y); }
    inline uint4 uint3::yyyz() const { return uint4(y, y, y, z); }
    inline uint4 uint3::yyzx() const { return uint4(y, y, z, x); }
    inline uint4 uint3::yyzy() const { return uint4(y, y, z, y); }
    inline uint4 uint3::yyzz() const { return uint4(y, y, z, z); }
    inline uint4 uint3::yzxx() const { return uint4(y, z, x, x); }
    inline uint4 uint3::yzxy() const { return uint4(y, z, x, y); }
    inline uint4 uint3::yzxz() const { return uint4(y, z, x, z); }
    inline uint4 uint3::yzyx() const { return uint4(y, z, y, x); }
    inline uint4 uint3::yzyy() const { return uint4(y, z, y, y); }
    inline uint4 uint3::yzyz() const { return uint4(y, z, y, z); }
    inline uint4 uint3::yzzx() const { return uint4(y, z, z, x); }
    inline uint4 uint3::yzzy() const { return uint4(y, z, z, y); }
    inline uint4 uint3::yzzz() const { return uint4(y, z, z, z); }
    inline uint4 uint3::zxxx() const { return uint4(z, x, x, x); }
    inline uint4 uint3::zxxy() const { return uint4(z, x, x, y); }
    inline uint4 uint3::zxxz() const { return uint4(z, x, x, z); }
    inline uint4 uint3::zxyx() const { return uint4(z, x, y, x); }
    inline uint4 uint3::zxyy() const { return uint4(z, x, y, y); }
    inline uint4 uint3::zxyz() const { return uint4(z, x, y, z); }
    inline uint4 uint3::zxzx() const { return uint4(z, x, z, x); }
    inline uint4 uint3::zxzy() const { return uint4(z, x, z, y); }
    inline uint4 uint3::zxzz() const { return uint4(z, x, z, z); }
    inline uint4 uint3::zyxx() const { return uint4(z, y, x, x); }
    inline uint4 uint3::zyxy() const { return uint4(z, y, x, y); }
    inline uint4 uint3::zyxz() const { return uint4(z, y, x, z); }
    inline uint4 uint3::zyyx() const { return uint4(z, y, y, x); }
    inline uint4 uint3::zyyy() const { return uint4(z, y, y, y); }
    inline uint4 uint3::zyyz() const { return uint4(z, y, y, z); }
    inline uint4 uint3::zyzx() const { return uint4(z, y, z, x); }
    inline uint4 uint3::zyzy() const { return uint4(z, y, z, y); }
    inline uint4 uint3::zyzz() const { return uint4(z, y, z, z); }
    inline uint4 uint3::zzxx() const { return uint4(z, z, x, x); }
    inline uint4 uint3::zzxy() const { return uint4(z, z, x, y); }
    inline uint4 uint3::zzxz() const { return uint4(z, z, x, z); }
    inline uint4 uint3::zzyx() const { return uint4(z, z, y, x); }
    inline uint4 uint3::zzyy() const { return uint4(z, z, y, y); }
    inline uint4 uint3::zzyz() const { return uint4(z, z, y, z); }
    inline uint4 uint3::zzzx() const { return uint4(z, z, z, x); }
    inline uint4 uint3::zzzy() const { return uint4(z, z, z, y); }
    inline uint4 uint3::zzzz() const { return uint4(z, z, z, z); }
    // uint4 swizzle
    inline uint2 uint4::xx() const { return uint2(x, x); }
    inline uint2 uint4::xy() const { return uint2(x, y); }
    inline uint2 uint4::xz() const { return uint2(x, z); }
    inline uint2 uint4::xw() const { return uint2(x, w); }
    inline uint2 uint4::yx() const { return uint2(y, x); }
    inline uint2 uint4::yy() const { return uint2(y, y); }
    inline uint2 uint4::yz() const { return uint2(y, z); }
    inline uint2 uint4::yw() const { return uint2(y, w); }
    inline uint2 uint4::zx() const { return uint2(z, x); }
    inline uint2 uint4::zy() const { return uint2(z, y); }
    inline uint2 uint4::zz() const { return uint2(z, z); }
    inline uint2 uint4::zw() const { return uint2(z, w); }
    inline uint2 uint4::wx() const { return uint2(w, x); }
    inline uint2 uint4::wy() const { return uint2(w, y); }
    inline uint2 uint4::wz() const { return uint2(w, z); }
    inline uint2 uint4::ww() const { return uint2(w, w); }
    inline uint3 uint4::xxx() const { return uint3(x, x, x); }
    inline uint3 uint4::xxy() const { return uint3(x, x, y); }
    inline uint3 uint4::xxz() const { return uint3(x, x, z); }
    inline uint3 uint4::xxw() const { return uint3(x, x, w); }
    inline uint3 uint4::xyx() const { return uint3(x, y, x); }
    inline uint3 uint4::xyy() const { return uint3(x, y, y); }
    inline uint3 uint4::xyz() const { return uint3(x, y, z); }
    inline uint3 uint4::xyw() const { return uint3(x, y, w); }
    inline uint3 uint4::xzx() const { return uint3(x, z, x); }
    inline uint3 uint4::xzy() const { return uint3(x, z, y); }
    inline uint3 uint4::xzz() const { return uint3(x, z, z); }
    inline uint3 uint4::xzw() const { return uint3(x, z, w); }
    inline uint3 uint4::xwx() const { return uint3(x, w, x); }
    inline uint3 uint4::xwy() const { return uint3(x, w, y); }
    inline uint3 uint4::xwz() const { return uint3(x, w, z); }
    inline uint3 uint4::xww() const { return uint3(x, w, w); }
    inline uint3 uint4::yxx() const { return uint3(y, x, x); }
    inline uint3 uint4::yxy() const { return uint3(y, x, y); }
    inline uint3 uint4::yxz() const { return uint3(y, x, z); }
    inline uint3 uint4::yxw() const { return uint3(y, x, w); }
    inline uint3 uint4::yyx() const { return uint3(y, y, x); }
    inline uint3 uint4::yyy() const { return uint3(y, y, y); }
    inline uint3 uint4::yyz() const { return uint3(y, y, z); }
    inline uint3 uint4::yyw() const { return uint3(y, y, w); }
    inline uint3 uint4::yzx() const { return uint3(y, z, x); }
    inline uint3 uint4::yzy() const { return uint3(y, z, y); }
    inline uint3 uint4::yzz() const { return uint3(y, z, z); }
    inline uint3 uint4::yzw() const { return uint3(y, z, w); }
    inline uint3 uint4::ywx() const { return uint3(y, w, x); }
    inline uint3 uint4::ywy() const { return uint3(y, w, y); }
    inline uint3 uint4::ywz() const { return uint3(y, w, z); }
    inline uint3 uint4::yww() const { return uint3(y, w, w); }
    inline uint3 uint4::zxx() const { return uint3(z, x, x); }
    inline uint3 uint4::zxy() const { return uint3(z, x, y); }
    inline uint3 uint4::zxz() const { return uint3(z, x, z); }
    inline uint3 uint4::zxw() const { return uint3(z, x, w); }
    inline uint3 uint4::zyx() const { return uint3(z, y, x); }
    inline uint3 uint4::zyy() const { return uint3(z, y, y); }
    inline uint3 uint4::zyz() const { return uint3(z, y, z); }
    inline uint3 uint4::zyw() const { return uint3(z, y, w); }
    inline uint3 uint4::zzx() const { return uint3(z, z, x); }
    inline uint3 uint4::zzy() const { return uint3(z, z, y); }
    inline uint3 uint4::zzz() const { return uint3(z, z, z); }
    inline uint3 uint4::zzw() const { return uint3(z, z, w); }
    inline uint3 uint4::zwx() const { return uint3(z, w, x); }
    inline uint3 uint4::zwy() const { return uint3(z, w, y); }
    inline uint3 uint4::zwz() const { return uint3(z, w, z); }
    inline uint3 uint4::zww() const { return uint3(z, w, w); }
    inline uint3 uint4::wxx() const { return uint3(w, x, x); }
    inline uint3 uint4::wxy() const { return uint3(w, x, y); }
    inline uint3 uint4::wxz() const { return uint3(w, x, z); }
    inline uint3 uint4::wxw() const { return uint3(w, x, w); }
    inline uint3 uint4::wyx() const { return uint3(w, y, x); }
    inline uint3 uint4::wyy() const { return uint3(w, y, y); }
    inline uint3 uint4::wyz() const { return uint3(w, y, z); }
    inline uint3 uint4::wyw() const { return uint3(w, y, w); }
    inline uint3 uint4::wzx() const { return uint3(w, z, x); }
    inline uint3 uint4::wzy() const { return uint3(w, z, y); }
    inline uint3 uint4::wzz() const { return uint3(w, z, z); }
    inline uint3 uint4::wzw() const { return uint3(w, z, w); }
    inline uint3 uint4::wwx() const { return uint3(w, w, x); }
    inline uint3 uint4::wwy() const { return uint3(w, w, y); }
    inline uint3 uint4::wwz() const { return uint3(w, w, z); }
    inline uint3 uint4::www() const { return uint3(w, w, w); }
    inline uint4 uint4::xxxx() const { return uint4(x, x, x, x); }
    inline uint4 uint4::xxxy() const { return uint4(x, x, x, y); }
    inline uint4 uint4::xxxz() const { return uint4(x, x, x, z); }
    inline uint4 uint4::xxxw() const { return uint4(x, x, x, w); }
    inline uint4 uint4::xxyx() const { return uint4(x, x, y, x); }
    inline uint4 uint4::xxyy() const { return uint4(x, x, y, y); }
    inline uint4 uint4::xxyz() const { return uint4(x, x, y, z); }
    inline uint4 uint4::xxyw() const { return uint4(x, x, y, w); }
    inline uint4 uint4::xxzx() const { return uint4(x, x, z, x); }
    inline uint4 uint4::xxzy() const { return uint4(x, x, z, y); }
    inline uint4 uint4::xxzz() const { return uint4(x, x, z, z); }
    inline uint4 uint4::xxzw() const { return uint4(x, x, z, w); }
    inline uint4 uint4::xxwx() const { return uint4(x, x, w, x); }
    inline uint4 uint4::xxwy() const { return uint4(x, x, w, y); }
    inline uint4 uint4::xxwz() const { return uint4(x, x, w, z); }
    inline uint4 uint4::xxww() const { return uint4(x, x, w, w); }
    inline uint4 uint4::xyxx() const { return uint4(x, y, x, x); }
    inline uint4 uint4::xyxy() const { return uint4(x, y, x, y); }
    inline uint4 uint4::xyxz() const { return uint4(x, y, x, z); }
    inline uint4 uint4::xyxw() const { return uint4(x, y, x, w); }
    inline uint4 uint4::xyyx() const { return uint4(x, y, y, x); }
    inline uint4 uint4::xyyy() const { return uint4(x, y, y, y); }
    inline uint4 uint4::xyyz() const { return uint4(x, y, y, z); }
    inline uint4 uint4::xyyw() const { return uint4(x, y, y, w); }
    inline uint4 uint4::xyzx() const { return uint4(x, y, z, x); }
    inline uint4 uint4::xyzy() const { return uint4(x, y, z, y); }
    inline uint4 uint4::xyzz() const { return uint4(x, y, z, z); }
    inline uint4 uint4::xyzw() const { return uint4(x, y, z, w); }
    inline uint4 uint4::xywx() const { return uint4(x, y, w, x); }
    inline uint4 uint4::xywy() const { return uint4(x, y, w, y); }
    inline uint4 uint4::xywz() const { return uint4(x, y, w, z); }
    inline uint4 uint4::xyww() const { return uint4(x, y, w, w); }
    inline uint4 uint4::xzxx() const { return uint4(x, z, x, x); }
    inline uint4 uint4::xzxy() const { return uint4(x, z, x, y); }
    inline uint4 uint4::xzxz() const { return uint4(x, z, x, z); }
    inline uint4 uint4::xzxw() const { return uint4(x, z, x, w); }
    inline uint4 uint4::xzyx() const { return uint4(x, z, y, x); }
    inline uint4 uint4::xzyy() const { return uint4(x, z, y, y); }
    inline uint4 uint4::xzyz() const { return uint4(x, z, y, z); }
    inline uint4 uint4::xzyw() const { return uint4(x, z, y, w); }
    inline uint4 uint4::xzzx() const { return uint4(x, z, z, x); }
    inline uint4 uint4::xzzy() const { return uint4(x, z, z, y); }
    inline uint4 uint4::xzzz() const { return uint4(x, z, z, z); }
    inline uint4 uint4::xzzw() const { return uint4(x, z, z, w); }
    inline uint4 uint4::xzwx() const { return uint4(x, z, w, x); }
    inline uint4 uint4::xzwy() const { return uint4(x, z, w, y); }
    inline uint4 uint4::xzwz() const { return uint4(x, z, w, z); }
    inline uint4 uint4::xzww() const { return uint4(x, z, w, w); }
    inline uint4 uint4::xwxx() const { return uint4(x, w, x, x); }
    inline uint4 uint4::xwxy() const { return uint4(x, w, x, y); }
    inline uint4 uint4::xwxz() const { return uint4(x, w, x, z); }
    inline uint4 uint4::xwxw() const { return uint4(x, w, x, w); }
    inline uint4 uint4::xwyx() const { return uint4(x, w, y, x); }
    inline uint4 uint4::xwyy() const { return uint4(x, w, y, y); }
    inline uint4 uint4::xwyz() const { return uint4(x, w, y, z); }
    inline uint4 uint4::xwyw() const { return uint4(x, w, y, w); }
    inline uint4 uint4::xwzx() const { return uint4(x, w, z, x); }
    inline uint4 uint4::xwzy() const { return uint4(x, w, z, y); }
    inline uint4 uint4::xwzz() const { return uint4(x, w, z, z); }
    inline uint4 uint4::xwzw() const { return uint4(x, w, z, w); }
    inline uint4 uint4::xwwx() const { return uint4(x, w, w, x); }
    inline uint4 uint4::xwwy() const { return uint4(x, w, w, y); }
    inline uint4 uint4::xwwz() const { return uint4(x, w, w, z); }
    inline uint4 uint4::xwww() const { return uint4(x, w, w, w); }
    inline uint4 uint4::yxxx() const { return uint4(y, x, x, x); }
    inline uint4 uint4::yxxy() const { return uint4(y, x, x, y); }
    inline uint4 uint4::yxxz() const { return uint4(y, x, x, z); }
    inline uint4 uint4::yxxw() const { return uint4(y, x, x, w); }
    inline uint4 uint4::yxyx() const { return uint4(y, x, y, x); }
    inline uint4 uint4::yxyy() const { return uint4(y, x, y, y); }
    inline uint4 uint4::yxyz() const { return uint4(y, x, y, z); }
    inline uint4 uint4::yxyw() const { return uint4(y, x, y, w); }
    inline uint4 uint4::yxzx() const { return uint4(y, x, z, x); }
    inline uint4 uint4::yxzy() const { return uint4(y, x, z, y); }
    inline uint4 uint4::yxzz() const { return uint4(y, x, z, z); }
    inline uint4 uint4::yxzw() const { return uint4(y, x, z, w); }
    inline uint4 uint4::yxwx() const { return uint4(y, x, w, x); }
    inline uint4 uint4::yxwy() const { return uint4(y, x, w, y); }
    inline uint4 uint4::yxwz() const { return uint4(y, x, w, z); }
    inline uint4 uint4::yxww() const { return uint4(y, x, w, w); }
    inline uint4 uint4::yyxx() const { return uint4(y, y, x, x); }
    inline uint4 uint4::yyxy() const { return uint4(y, y, x, y); }
    inline uint4 uint4::yyxz() const { return uint4(y, y, x, z); }
    inline uint4 uint4::yyxw() const { return uint4(y, y, x, w); }
    inline uint4 uint4::yyyx() const { return uint4(y, y, y, x); }
    inline uint4 uint4::yyyy() const { return uint4(y, y, y, y); }
    inline uint4 uint4::yyyz() const { return uint4(y, y, y, z); }
    inline uint4 uint4::yyyw() const { return uint4(y, y, y, w); }
    inline uint4 uint4::yyzx() const { return uint4(y, y, z, x); }
    inline uint4 uint4::yyzy() const { return uint4(y, y, z, y); }
    inline uint4 uint4::yyzz() const { return uint4(y, y, z, z); }
    inline uint4 uint4::yyzw() const { return uint4(y, y, z, w); }
    inline uint4 uint4::yywx() const { return uint4(y, y, w, x); }
    inline uint4 uint4::yywy() const { return uint4(y, y, w, y); }
    inline uint4 uint4::yywz() const { return uint4(y, y, w, z); }
    inline uint4 uint4::yyww() const { return uint4(y, y, w, w); }
    inline uint4 uint4::yzxx() const { return uint4(y, z, x, x); }
    inline uint4 uint4::yzxy() const { return uint4(y, z, x, y); }
    inline uint4 uint4::yzxz() const { return uint4(y, z, x, z); }
    inline uint4 uint4::yzxw() const { return uint4(y, z, x, w); }
    inline uint4 uint4::yzyx() const { return uint4(y, z, y, x); }
    inline uint4 uint4::yzyy() const { return uint4(y, z, y, y); }
    inline uint4 uint4::yzyz() const { return uint4(y, z, y, z); }
    inline uint4 uint4::yzyw() const { return uint4(y, z, y, w); }
    inline uint4 uint4::yzzx() const { return uint4(y, z, z, x); }
    inline uint4 uint4::yzzy() const { return uint4(y, z, z, y); }
    inline uint4 uint4::yzzz() const { return uint4(y, z, z, z); }
    inline uint4 uint4::yzzw() const { return uint4(y, z, z, w); }
    inline uint4 uint4::yzwx() const { return uint4(y, z, w, x); }
    inline uint4 uint4::yzwy() const { return uint4(y, z, w, y); }
    inline uint4 uint4::yzwz() const { return uint4(y, z, w, z); }
    inline uint4 uint4::yzww() const { return uint4(y, z, w, w); }
    inline uint4 uint4::ywxx() const { return uint4(y, w, x, x); }
    inline uint4 uint4::ywxy() const { return uint4(y, w, x, y); }
    inline uint4 uint4::ywxz() const { return uint4(y, w, x, z); }
    inline uint4 uint4::ywxw() const { return uint4(y, w, x, w); }
    inline uint4 uint4::ywyx() const { return uint4(y, w, y, x); }
    inline uint4 uint4::ywyy() const { return uint4(y, w, y, y); }
    inline uint4 uint4::ywyz() const { return uint4(y, w, y, z); }
    inline uint4 uint4::ywyw() const { return uint4(y, w, y, w); }
    inline uint4 uint4::ywzx() const { return uint4(y, w, z, x); }
    inline uint4 uint4::ywzy() const { return uint4(y, w, z, y); }
    inline uint4 uint4::ywzz() const { return uint4(y, w, z, z); }
    inline uint4 uint4::ywzw() const { return uint4(y, w, z, w); }
    inline uint4 uint4::ywwx() const { return uint4(y, w, w, x); }
    inline uint4 uint4::ywwy() const { return uint4(y, w, w, y); }
    inline uint4 uint4::ywwz() const { return uint4(y, w, w, z); }
    inline uint4 uint4::ywww() const { return uint4(y, w, w, w); }
    inline uint4 uint4::zxxx() const { return uint4(z, x, x, x); }
    inline uint4 uint4::zxxy() const { return uint4(z, x, x, y); }
    inline uint4 uint4::zxxz() const { return uint4(z, x, x, z); }
    inline uint4 uint4::zxxw() const { return uint4(z, x, x, w); }
    inline uint4 uint4::zxyx() const { return uint4(z, x, y, x); }
    inline uint4 uint4::zxyy() const { return uint4(z, x, y, y); }
    inline uint4 uint4::zxyz() const { return uint4(z, x, y, z); }
    inline uint4 uint4::zxyw() const { return uint4(z, x, y, w); }
    inline uint4 uint4::zxzx() const { return uint4(z, x, z, x); }
    inline uint4 uint4::zxzy() const { return uint4(z, x, z, y); }
    inline uint4 uint4::zxzz() const { return uint4(z, x, z, z); }
    inline uint4 uint4::zxzw() const { return uint4(z, x, z, w); }
    inline uint4 uint4::zxwx() const { return uint4(z, x, w, x); }
    inline uint4 uint4::zxwy() const { return uint4(z, x, w, y); }
    inline uint4 uint4::zxwz() const { return uint4(z, x, w, z); }
    inline uint4 uint4::zxww() const { return uint4(z, x, w, w); }
    inline uint4 uint4::zyxx() const { return uint4(z, y, x, x); }
    inline uint4 uint4::zyxy() const { return uint4(z, y, x, y); }
    inline uint4 uint4::zyxz() const { return uint4(z, y, x, z); }
    inline uint4 uint4::zyxw() const { return uint4(z, y, x, w); }
    inline uint4 uint4::zyyx() const { return uint4(z, y, y, x); }
    inline uint4 uint4::zyyy() const { return uint4(z, y, y, y); }
    inline uint4 uint4::zyyz() const { return uint4(z, y, y, z); }
    inline uint4 uint4::zyyw() const { return uint4(z, y, y, w); }
    inline uint4 uint4::zyzx() const { return uint4(z, y, z, x); }
    inline uint4 uint4::zyzy() const { return uint4(z, y, z, y); }
    inline uint4 uint4::zyzz() const { return uint4(z, y, z, z); }
    inline uint4 uint4::zyzw() const { return uint4(z, y, z, w); }
    inline uint4 uint4::zywx() const { return uint4(z, y, w, x); }
    inline uint4 uint4::zywy() const { return uint4(z, y, w, y); }
    inline uint4 uint4::zywz() const { return uint4(z, y, w, z); }
    inline uint4 uint4::zyww() const { return uint4(z, y, w, w); }
    inline uint4 uint4::zzxx() const { return uint4(z, z, x, x); }
    inline uint4 uint4::zzxy() const { return uint4(z, z, x, y); }
    inline uint4 uint4::zzxz() const { return uint4(z, z, x, z); }
    inline uint4 uint4::zzxw() const { return uint4(z, z, x, w); }
    inline uint4 uint4::zzyx() const { return uint4(z, z, y, x); }
    inline uint4 uint4::zzyy() const { return uint4(z, z, y, y); }
    inline uint4 uint4::zzyz() const { return uint4(z, z, y, z); }
    inline uint4 uint4::zzyw() const { return uint4(z, z, y, w); }
    inline uint4 uint4::zzzx() const { return uint4(z, z, z, x); }
    inline uint4 uint4::zzzy() const { return uint4(z, z, z, y); }
    inline uint4 uint4::zzzz() const { return uint4(z, z, z, z); }
    inline uint4 uint4::zzzw() const { return uint4(z, z, z, w); }
    inline uint4 uint4::zzwx() const { return uint4(z, z, w, x); }
    inline uint4 uint4::zzwy() const { return uint4(z, z, w, y); }
    inline uint4 uint4::zzwz() const { return uint4(z, z, w, z); }
    inline uint4 uint4::zzww() const { return uint4(z, z, w, w); }
    inline uint4 uint4::zwxx() const { return uint4(z, w, x, x); }
    inline uint4 uint4::zwxy() const { return uint4(z, w, x, y); }
    inline uint4 uint4::zwxz() const { return uint4(z, w, x, z); }
    inline uint4 uint4::zwxw() const { return uint4(z, w, x, w); }
    inline uint4 uint4::zwyx() const { return uint4(z, w, y, x); }
    inline uint4 uint4::zwyy() const { return uint4(z, w, y, y); }
    inline uint4 uint4::zwyz() const { return uint4(z, w, y, z); }
    inline uint4 uint4::zwyw() const { return uint4(z, w, y, w); }
    inline uint4 uint4::zwzx() const { return uint4(z, w, z, x); }
    inline uint4 uint4::zwzy() const { return uint4(z, w, z, y); }
    inline uint4 uint4::zwzz() const { return uint4(z, w, z, z); }
    inline uint4 uint4::zwzw() const { return uint4(z, w, z, w); }
    inline uint4 uint4::zwwx() const { return uint4(z, w, w, x); }
    inline uint4 uint4::zwwy() const { return uint4(z, w, w, y); }
    inline uint4 uint4::zwwz() const { return uint4(z, w, w, z); }
    inline uint4 uint4::zwww() const { return uint4(z, w, w, w); }
    inline uint4 uint4::wxxx() const { return uint4(w, x, x, x); }
    inline uint4 uint4::wxxy() const { return uint4(w, x, x, y); }
    inline uint4 uint4::wxxz() const { return uint4(w, x, x, z); }
    inline uint4 uint4::wxxw() const { return uint4(w, x, x, w); }
    inline uint4 uint4::wxyx() const { return uint4(w, x, y, x); }
    inline uint4 uint4::wxyy() const { return uint4(w, x, y, y); }
    inline uint4 uint4::wxyz() const { return uint4(w, x, y, z); }
    inline uint4 uint4::wxyw() const { return uint4(w, x, y, w); }
    inline uint4 uint4::wxzx() const { return uint4(w, x, z, x); }
    inline uint4 uint4::wxzy() const { return uint4(w, x, z, y); }
    inline uint4 uint4::wxzz() const { return uint4(w, x, z, z); }
    inline uint4 uint4::wxzw() const { return uint4(w, x, z, w); }
    inline uint4 uint4::wxwx() const { return uint4(w, x, w, x); }
    inline uint4 uint4::wxwy() const { return uint4(w, x, w, y); }
    inline uint4 uint4::wxwz() const { return uint4(w, x, w, z); }
    inline uint4 uint4::wxww() const { return uint4(w, x, w, w); }
    inline uint4 uint4::wyxx() const { return uint4(w, y, x, x); }
    inline uint4 uint4::wyxy() const { return uint4(w, y, x, y); }
    inline uint4 uint4::wyxz() const { return uint4(w, y, x, z); }
    inline uint4 uint4::wyxw() const { return uint4(w, y, x, w); }
    inline uint4 uint4::wyyx() const { return uint4(w, y, y, x); }
    inline uint4 uint4::wyyy() const { return uint4(w, y, y, y); }
    inline uint4 uint4::wyyz() const { return uint4(w, y, y, z); }
    inline uint4 uint4::wyyw() const { return uint4(w, y, y, w); }
    inline uint4 uint4::wyzx() const { return uint4(w, y, z, x); }
    inline uint4 uint4::wyzy() const { return uint4(w, y, z, y); }
    inline uint4 uint4::wyzz() const { return uint4(w, y, z, z); }
    inline uint4 uint4::wyzw() const { return uint4(w, y, z, w); }
    inline uint4 uint4::wywx() const { return uint4(w, y, w, x); }
    inline uint4 uint4::wywy() const { return uint4(w, y, w, y); }
    inline uint4 uint4::wywz() const { return uint4(w, y, w, z); }
    inline uint4 uint4::wyww() const { return uint4(w, y, w, w); }
    inline uint4 uint4::wzxx() const { return uint4(w, z, x, x); }
    inline uint4 uint4::wzxy() const { return uint4(w, z, x, y); }
    inline uint4 uint4::wzxz() const { return uint4(w, z, x, z); }
    inline uint4 uint4::wzxw() const { return uint4(w, z, x, w); }
    inline uint4 uint4::wzyx() const { return uint4(w, z, y, x); }
    inline uint4 uint4::wzyy() const { return uint4(w, z, y, y); }
    inline uint4 uint4::wzyz() const { return uint4(w, z, y, z); }
    inline uint4 uint4::wzyw() const { return uint4(w, z, y, w); }
    inline uint4 uint4::wzzx() const { return uint4(w, z, z, x); }
    inline uint4 uint4::wzzy() const { return uint4(w, z, z, y); }
    inline uint4 uint4::wzzz() const { return uint4(w, z, z, z); }
    inline uint4 uint4::wzzw() const { return uint4(w, z, z, w); }
    inline uint4 uint4::wzwx() const { return uint4(w, z, w, x); }
    inline uint4 uint4::wzwy() const { return uint4(w, z, w, y); }
    inline uint4 uint4::wzwz() const { return uint4(w, z, w, z); }
    inline uint4 uint4::wzww() const { return uint4(w, z, w, w); }
    inline uint4 uint4::wwxx() const { return uint4(w, w, x, x); }
    inline uint4 uint4::wwxy() const { return uint4(w, w, x, y); }
    inline uint4 uint4::wwxz() const { return uint4(w, w, x, z); }
    inline uint4 uint4::wwxw() const { return uint4(w, w, x, w); }
    inline uint4 uint4::wwyx() const { return uint4(w, w, y, x); }
    inline uint4 uint4::wwyy() const { return uint4(w, w, y, y); }
    inline uint4 uint4::wwyz() const { return uint4(w, w, y, z); }
    inline uint4 uint4::wwyw() const { return uint4(w, w, y, w); }
    inline uint4 uint4::wwzx() const { return uint4(w, w, z, x); }
    inline uint4 uint4::wwzy() const { return uint4(w, w, z, y); }
    inline uint4 uint4::wwzz() const { return uint4(w, w, z, z); }
    inline uint4 uint4::wwzw() const { return uint4(w, w, z, w); }
    inline uint4 uint4::wwwx() const { return uint4(w, w, w, x); }
    inline uint4 uint4::wwwy() const { return uint4(w, w, w, y); }
    inline uint4 uint4::wwwz() const { return uint4(w, w, w, z); }
    inline uint4 uint4::wwww() const { return uint4(w, w, w, w); }
    // bool2 swizzle
    inline bool2 bool2::xx() const { return bool2(x, x); }
    inline bool2 bool2::xy() const { return bool2(x, y); }
    inline bool2 bool2::yx() const { return bool2(y, x); }
    inline bool2 bool2::yy() const { return bool2(y, y); }
    inline bool3 bool2::xxx() const { return bool3(x, x, x); }
    inline bool3 bool2::xxy() const { return bool3(x, x, y); }
    inline bool3 bool2::xyx() const { return bool3(x, y, x); }
    inline bool3 bool2::xyy() const { return bool3(x, y, y); }
    inline bool3 bool2::yxx() const { return bool3(y, x, x); }
    inline bool3 bool2::yxy() const { return bool3(y, x, y); }
    inline bool3 bool2::yyx() const { return bool3(y, y, x); }
    inline bool3 bool2::yyy() const { return bool3(y, y, y); }
    inline bool4 bool2::xxxx() const { return bool4(x, x, x, x); }
    inline bool4 bool2::xxxy() const { return bool4(x, x, x, y); }
    inline bool4 bool2::xxyx() const { return bool4(x, x, y, x); }
    inline bool4 bool2::xxyy() const { return bool4(x, x, y, y); }
    inline bool4 bool2::xyxx() const { return bool4(x, y, x, x); }
    inline bool4 bool2::xyxy() const { return bool4(x, y, x, y); }
    inline bool4 bool2::xyyx() const { return bool4(x, y, y, x); }
    inline bool4 bool2::xyyy() const { return bool4(x, y, y, y); }
    inline bool4 bool2::yxxx() const { return bool4(y, x, x, x); }
    inline bool4 bool2::yxxy() const { return bool4(y, x, x, y); }
    inline bool4 bool2::yxyx() const { return bool4(y, x, y, x); }
    inline bool4 bool2::yxyy() const { return bool4(y, x, y, y); }
    inline bool4 bool2::yyxx() const { return bool4(y, y, x, x); }
    inline bool4 bool2::yyxy() const { return bool4(y, y, x, y); }
    inline bool4 bool2::yyyx() const { return bool4(y, y, y, x); }
    inline bool4 bool2::yyyy() const { return bool4(y, y, y, y); }
    // bool3 swizzle
    inline bool2 bool3::xx() const { return bool2(x, x); }
    inline bool2 bool3::xy() const { return bool2(x, y); }
    inline bool2 bool3::xz() const { return bool2(x, z); }
    inline bool2 bool3::yx() const { return bool2(y, x); }
    inline bool2 bool3::yy() const { return bool2(y, y); }
    inline bool2 bool3::yz() const { return bool2(y, z); }
    inline bool2 bool3::zx() const { return bool2(z, x); }
    inline bool2 bool3::zy() const { return bool2(z, y); }
    inline bool2 bool3::zz() const { return bool2(z, z); }
    inline bool3 bool3::xxx() const { return bool3(x, x, x); }
    inline bool3 bool3::xxy() const { return bool3(x, x, y); }
    inline bool3 bool3::xxz() const { return bool3(x, x, z); }
    inline bool3 bool3::xyx() const { return bool3(x, y, x); }
    inline bool3 bool3::xyy() const { return bool3(x, y, y); }
    inline bool3 bool3::xyz() const { return bool3(x, y, z); }
    inline bool3 bool3::xzx() const { return bool3(x, z, x); }
    inline bool3 bool3::xzy() const { return bool3(x, z, y); }
    inline bool3 bool3::xzz() const { return bool3(x, z, z); }
    inline bool3 bool3::yxx() const { return bool3(y, x, x); }
    inline bool3 bool3::yxy() const { return bool3(y, x, y); }
    inline bool3 bool3::yxz() const { return bool3(y, x, z); }
    inline bool3 bool3::yyx() const { return bool3(y, y, x); }
    inline bool3 bool3::yyy() const { return bool3(y, y, y); }
    inline bool3 bool3::yyz() const { return bool3(y, y, z); }
    inline bool3 bool3::yzx() const { return bool3(y, z, x); }
    inline bool3 bool3::yzy() const { return bool3(y, z, y); }
    inline bool3 bool3::yzz() const { return bool3(y, z, z); }
    inline bool3 bool3::zxx() const { return bool3(z, x, x); }
    inline bool3 bool3::zxy() const { return bool3(z, x, y); }
    inline bool3 bool3::zxz() const { return bool3(z, x, z); }
    inline bool3 bool3::zyx() const { return bool3(z, y, x); }
    inline bool3 bool3::zyy() const { return bool3(z, y, y); }
    inline bool3 bool3::zyz() const { return bool3(z, y, z); }
    inline bool3 bool3::zzx() const { return bool3(z, z, x); }
    inline bool3 bool3::zzy() const { return bool3(z, z, y); }
    inline bool3 bool3::zzz() const { return bool3(z, z, z); }
    inline bool4 bool3::xxxx() const { return bool4(x, x, x, x); }
    inline bool4 bool3::xxxy() const { return bool4(x, x, x, y); }
    inline bool4 bool3::xxxz() const { return bool4(x, x, x, z); }
    inline bool4 bool3::xxyx() const { return bool4(x, x, y, x); }
    inline bool4 bool3::xxyy() const { return bool4(x, x, y, y); }
    inline bool4 bool3::xxyz() const { return bool4(x, x, y, z); }
    inline bool4 bool3::xxzx() const { return bool4(x, x, z, x); }
    inline bool4 bool3::xxzy() const { return bool4(x, x, z, y); }
    inline bool4 bool3::xxzz() const { return bool4(x, x, z, z); }
    inline bool4 bool3::xyxx() const { return bool4(x, y, x, x); }
    inline bool4 bool3::xyxy() const { return bool4(x, y, x, y); }
    inline bool4 bool3::xyxz() const { return bool4(x, y, x, z); }
    inline bool4 bool3::xyyx() const { return bool4(x, y, y, x); }
    inline bool4 bool3::xyyy() const { return bool4(x, y, y, y); }
    inline bool4 bool3::xyyz() const { return bool4(x, y, y, z); }
    inline bool4 bool3::xyzx() const { return bool4(x, y, z, x); }
    inline bool4 bool3::xyzy() const { return bool4(x, y, z, y); }
    inline bool4 bool3::xyzz() const { return bool4(x, y, z, z); }
    inline bool4 bool3::xzxx() const { return bool4(x, z, x, x); }
    inline bool4 bool3::xzxy() const { return bool4(x, z, x, y); }
    inline bool4 bool3::xzxz() const { return bool4(x, z, x, z); }
    inline bool4 bool3::xzyx() const { return bool4(x, z, y, x); }
    inline bool4 bool3::xzyy() const { return bool4(x, z, y, y); }
    inline bool4 bool3::xzyz() const { return bool4(x, z, y, z); }
    inline bool4 bool3::xzzx() const { return bool4(x, z, z, x); }
    inline bool4 bool3::xzzy() const { return bool4(x, z, z, y); }
    inline bool4 bool3::xzzz() const { return bool4(x, z, z, z); }
    inline bool4 bool3::yxxx() const { return bool4(y, x, x, x); }
    inline bool4 bool3::yxxy() const { return bool4(y, x, x, y); }
    inline bool4 bool3::yxxz() const { return bool4(y, x, x, z); }
    inline bool4 bool3::yxyx() const { return bool4(y, x, y, x); }
    inline bool4 bool3::yxyy() const { return bool4(y, x, y, y); }
    inline bool4 bool3::yxyz() const { return bool4(y, x, y, z); }
    inline bool4 bool3::yxzx() const { return bool4(y, x, z, x); }
    inline bool4 bool3::yxzy() const { return bool4(y, x, z, y); }
    inline bool4 bool3::yxzz() const { return bool4(y, x, z, z); }
    inline bool4 bool3::yyxx() const { return bool4(y, y, x, x); }
    inline bool4 bool3::yyxy() const { return bool4(y, y, x, y); }
    inline bool4 bool3::yyxz() const { return bool4(y, y, x, z); }
    inline bool4 bool3::yyyx() const { return bool4(y, y, y, x); }
    inline bool4 bool3::yyyy() const { return bool4(y, y, y, y); }
    inline bool4 bool3::yyyz() const { return bool4(y, y, y, z); }
    inline bool4 bool3::yyzx() const { return bool4(y, y, z, x); }
    inline bool4 bool3::yyzy() const { return bool4(y, y, z, y); }
    inline bool4 bool3::yyzz() const { return bool4(y, y, z, z); }
    inline bool4 bool3::yzxx() const { return bool4(y, z, x, x); }
    inline bool4 bool3::yzxy() const { return bool4(y, z, x, y); }
    inline bool4 bool3::yzxz() const { return bool4(y, z, x, z); }
    inline bool4 bool3::yzyx() const { return bool4(y, z, y, x); }
    inline bool4 bool3::yzyy() const { return bool4(y, z, y, y); }
    inline bool4 bool3::yzyz() const { return bool4(y, z, y, z); }
    inline bool4 bool3::yzzx() const { return bool4(y, z, z, x); }
    inline bool4 bool3::yzzy() const { return bool4(y, z, z, y); }
    inline bool4 bool3::yzzz() const { return bool4(y, z, z, z); }
    inline bool4 bool3::zxxx() const { return bool4(z, x, x, x); }
    inline bool4 bool3::zxxy() const { return bool4(z, x, x, y); }
    inline bool4 bool3::zxxz() const { return bool4(z, x, x, z); }
    inline bool4 bool3::zxyx() const { return bool4(z, x, y, x); }
    inline bool4 bool3::zxyy() const { return bool4(z, x, y, y); }
    inline bool4 bool3::zxyz() const { return bool4(z, x, y, z); }
    inline bool4 bool3::zxzx() const { return bool4(z, x, z, x); }
    inline bool4 bool3::zxzy() const { return bool4(z, x, z, y); }
    inline bool4 bool3::zxzz() const { return bool4(z, x, z, z); }
    inline bool4 bool3::zyxx() const { return bool4(z, y, x, x); }
    inline bool4 bool3::zyxy() const { return bool4(z, y, x, y); }
    inline bool4 bool3::zyxz() const { return bool4(z, y, x, z); }
    inline bool4 bool3::zyyx() const { return bool4(z, y, y, x); }
    inline bool4 bool3::zyyy() const { return bool4(z, y, y, y); }
    inline bool4 bool3::zyyz() const { return bool4(z, y, y, z); }
    inline bool4 bool3::zyzx() const { return bool4(z, y, z, x); }
    inline bool4 bool3::zyzy() const { return bool4(z, y, z, y); }
    inline bool4 bool3::zyzz() const { return bool4(z, y, z, z); }
    inline bool4 bool3::zzxx() const { return bool4(z, z, x, x); }
    inline bool4 bool3::zzxy() const { return bool4(z, z, x, y); }
    inline bool4 bool3::zzxz() const { return bool4(z, z, x, z); }
    inline bool4 bool3::zzyx() const { return bool4(z, z, y, x); }
    inline bool4 bool3::zzyy() const { return bool4(z, z, y, y); }
    inline bool4 bool3::zzyz() const { return bool4(z, z, y, z); }
    inline bool4 bool3::zzzx() const { return bool4(z, z, z, x); }
    inline bool4 bool3::zzzy() const { return bool4(z, z, z, y); }
    inline bool4 bool3::zzzz() const { return bool4(z, z, z, z); }
    // bool4 swizzle
    inline bool2 bool4::xx() const { return bool2(x, x); }
    inline bool2 bool4::xy() const { return bool2(x, y); }
    inline bool2 bool4::xz() const { return bool2(x, z); }
    inline bool2 bool4::xw() const { return bool2(x, w); }
    inline bool2 bool4::yx() const { return bool2(y, x); }
    inline bool2 bool4::yy() const { return bool2(y, y); }
    inline bool2 bool4::yz() const { return bool2(y, z); }
    inline bool2 bool4::yw() const { return bool2(y, w); }
    inline bool2 bool4::zx() const { return bool2(z, x); }
    inline bool2 bool4::zy() const { return bool2(z, y); }
    inline bool2 bool4::zz() const { return bool2(z, z); }
    inline bool2 bool4::zw() const { return bool2(z, w); }
    inline bool2 bool4::wx() const { return bool2(w, x); }
    inline bool2 bool4::wy() const { return bool2(w, y); }
    inline bool2 bool4::wz() const { return bool2(w, z); }
    inline bool2 bool4::ww() const { return bool2(w, w); }
    inline bool3 bool4::xxx() const { return bool3(x, x, x); }
    inline bool3 bool4::xxy() const { return bool3(x, x, y); }
    inline bool3 bool4::xxz() const { return bool3(x, x, z); }
    inline bool3 bool4::xxw() const { return bool3(x, x, w); }
    inline bool3 bool4::xyx() const { return bool3(x, y, x); }
    inline bool3 bool4::xyy() const { return bool3(x, y, y); }
    inline bool3 bool4::xyz() const { return bool3(x, y, z); }
    inline bool3 bool4::xyw() const { return bool3(x, y, w); }
    inline bool3 bool4::xzx() const { return bool3(x, z, x); }
    inline bool3 bool4::xzy() const { return bool3(x, z, y); }
    inline bool3 bool4::xzz() const { return bool3(x, z, z); }
    inline bool3 bool4::xzw() const { return bool3(x, z, w); }
    inline bool3 bool4::xwx() const { return bool3(x, w, x); }
    inline bool3 bool4::xwy() const { return bool3(x, w, y); }
    inline bool3 bool4::xwz() const { return bool3(x, w, z); }
    inline bool3 bool4::xww() const { return bool3(x, w, w); }
    inline bool3 bool4::yxx() const { return bool3(y, x, x); }
    inline bool3 bool4::yxy() const { return bool3(y, x, y); }
    inline bool3 bool4::yxz() const { return bool3(y, x, z); }
    inline bool3 bool4::yxw() const { return bool3(y, x, w); }
    inline bool3 bool4::yyx() const { return bool3(y, y, x); }
    inline bool3 bool4::yyy() const { return bool3(y, y, y); }
    inline bool3 bool4::yyz() const { return bool3(y, y, z); }
    inline bool3 bool4::yyw() const { return bool3(y, y, w); }
    inline bool3 bool4::yzx() const { return bool3(y, z, x); }
    inline bool3 bool4::yzy() const { return bool3(y, z, y); }
    inline bool3 bool4::yzz() const { return bool3(y, z, z); }
    inline bool3 bool4::yzw() const { return bool3(y, z, w); }
    inline bool3 bool4::ywx() const { return bool3(y, w, x); }
    inline bool3 bool4::ywy() const { return bool3(y, w, y); }
    inline bool3 bool4::ywz() const { return bool3(y, w, z); }
    inline bool3 bool4::yww() const { return bool3(y, w, w); }
    inline bool3 bool4::zxx() const { return bool3(z, x, x); }
    inline bool3 bool4::zxy() const { return bool3(z, x, y); }
    inline bool3 bool4::zxz() const { return bool3(z, x, z); }
    inline bool3 bool4::zxw() const { return bool3(z, x, w); }
    inline bool3 bool4::zyx() const { return bool3(z, y, x); }
    inline bool3 bool4::zyy() const { return bool3(z, y, y); }
    inline bool3 bool4::zyz() const { return bool3(z, y, z); }
    inline bool3 bool4::zyw() const { return bool3(z, y, w); }
    inline bool3 bool4::zzx() const { return bool3(z, z, x); }
    inline bool3 bool4::zzy() const { return bool3(z, z, y); }
    inline bool3 bool4::zzz() const { return bool3(z, z, z); }
    inline bool3 bool4::zzw() const { return bool3(z, z, w); }
    inline bool3 bool4::zwx() const { return bool3(z, w, x); }
    inline bool3 bool4::zwy() const { return bool3(z, w, y); }
    inline bool3 bool4::zwz() const { return bool3(z, w, z); }
    inline bool3 bool4::zww() const { return bool3(z, w, w); }
    inline bool3 bool4::wxx() const { return bool3(w, x, x); }
    inline bool3 bool4::wxy() const { return bool3(w, x, y); }
    inline bool3 bool4::wxz() const { return bool3(w, x, z); }
    inline bool3 bool4::wxw() const { return bool3(w, x, w); }
    inline bool3 bool4::wyx() const { return bool3(w, y, x); }
    inline bool3 bool4::wyy() const { return bool3(w, y, y); }
    inline bool3 bool4::wyz() const { return bool3(w, y, z); }
    inline bool3 bool4::wyw() const { return bool3(w, y, w); }
    inline bool3 bool4::wzx() const { return bool3(w, z, x); }
    inline bool3 bool4::wzy() const { return bool3(w, z, y); }
    inline bool3 bool4::wzz() const { return bool3(w, z, z); }
    inline bool3 bool4::wzw() const { return bool3(w, z, w); }
    inline bool3 bool4::wwx() const { return bool3(w, w, x); }
    inline bool3 bool4::wwy() const { return bool3(w, w, y); }
    inline bool3 bool4::wwz() const { return bool3(w, w, z); }
    inline bool3 bool4::www() const { return bool3(w, w, w); }
    inline bool4 bool4::xxxx() const { return bool4(x, x, x, x); }
    inline bool4 bool4::xxxy() const { return bool4(x, x, x, y); }
    inline bool4 bool4::xxxz() const { return bool4(x, x, x, z); }
    inline bool4 bool4::xxxw() const { return bool4(x, x, x, w); }
    inline bool4 bool4::xxyx() const { return bool4(x, x, y, x); }
    inline bool4 bool4::xxyy() const { return bool4(x, x, y, y); }
    inline bool4 bool4::xxyz() const { return bool4(x, x, y, z); }
    inline bool4 bool4::xxyw() const { return bool4(x, x, y, w); }
    inline bool4 bool4::xxzx() const { return bool4(x, x, z, x); }
    inline bool4 bool4::xxzy() const { return bool4(x, x, z, y); }
    inline bool4 bool4::xxzz() const { return bool4(x, x, z, z); }
    inline bool4 bool4::xxzw() const { return bool4(x, x, z, w); }
    inline bool4 bool4::xxwx() const { return bool4(x, x, w, x); }
    inline bool4 bool4::xxwy() const { return bool4(x, x, w, y); }
    inline bool4 bool4::xxwz() const { return bool4(x, x, w, z); }
    inline bool4 bool4::xxww() const { return bool4(x, x, w, w); }
    inline bool4 bool4::xyxx() const { return bool4(x, y, x, x); }
    inline bool4 bool4::xyxy() const { return bool4(x, y, x, y); }
    inline bool4 bool4::xyxz() const { return bool4(x, y, x, z); }
    inline bool4 bool4::xyxw() const { return bool4(x, y, x, w); }
    inline bool4 bool4::xyyx() const { return bool4(x, y, y, x); }
    inline bool4 bool4::xyyy() const { return bool4(x, y, y, y); }
    inline bool4 bool4::xyyz() const { return bool4(x, y, y, z); }
    inline bool4 bool4::xyyw() const { return bool4(x, y, y, w); }
    inline bool4 bool4::xyzx() const { return bool4(x, y, z, x); }
    inline bool4 bool4::xyzy() const { return bool4(x, y, z, y); }
    inline bool4 bool4::xyzz() const { return bool4(x, y, z, z); }
    inline bool4 bool4::xyzw() const { return bool4(x, y, z, w); }
    inline bool4 bool4::xywx() const { return bool4(x, y, w, x); }
    inline bool4 bool4::xywy() const { return bool4(x, y, w, y); }
    inline bool4 bool4::xywz() const { return bool4(x, y, w, z); }
    inline bool4 bool4::xyww() const { return bool4(x, y, w, w); }
    inline bool4 bool4::xzxx() const { return bool4(x, z, x, x); }
    inline bool4 bool4::xzxy() const { return bool4(x, z, x, y); }
    inline bool4 bool4::xzxz() const { return bool4(x, z, x, z); }
    inline bool4 bool4::xzxw() const { return bool4(x, z, x, w); }
    inline bool4 bool4::xzyx() const { return bool4(x, z, y, x); }
    inline bool4 bool4::xzyy() const { return bool4(x, z, y, y); }
    inline bool4 bool4::xzyz() const { return bool4(x, z, y, z); }
    inline bool4 bool4::xzyw() const { return bool4(x, z, y, w); }
    inline bool4 bool4::xzzx() const { return bool4(x, z, z, x); }
    inline bool4 bool4::xzzy() const { return bool4(x, z, z, y); }
    inline bool4 bool4::xzzz() const { return bool4(x, z, z, z); }
    inline bool4 bool4::xzzw() const { return bool4(x, z, z, w); }
    inline bool4 bool4::xzwx() const { return bool4(x, z, w, x); }
    inline bool4 bool4::xzwy() const { return bool4(x, z, w, y); }
    inline bool4 bool4::xzwz() const { return bool4(x, z, w, z); }
    inline bool4 bool4::xzww() const { return bool4(x, z, w, w); }
    inline bool4 bool4::xwxx() const { return bool4(x, w, x, x); }
    inline bool4 bool4::xwxy() const { return bool4(x, w, x, y); }
    inline bool4 bool4::xwxz() const { return bool4(x, w, x, z); }
    inline bool4 bool4::xwxw() const { return bool4(x, w, x, w); }
    inline bool4 bool4::xwyx() const { return bool4(x, w, y, x); }
    inline bool4 bool4::xwyy() const { return bool4(x, w, y, y); }
    inline bool4 bool4::xwyz() const { return bool4(x, w, y, z); }
    inline bool4 bool4::xwyw() const { return bool4(x, w, y, w); }
    inline bool4 bool4::xwzx() const { return bool4(x, w, z, x); }
    inline bool4 bool4::xwzy() const { return bool4(x, w, z, y); }
    inline bool4 bool4::xwzz() const { return bool4(x, w, z, z); }
    inline bool4 bool4::xwzw() const { return bool4(x, w, z, w); }
    inline bool4 bool4::xwwx() const { return bool4(x, w, w, x); }
    inline bool4 bool4::xwwy() const { return bool4(x, w, w, y); }
    inline bool4 bool4::xwwz() const { return bool4(x, w, w, z); }
    inline bool4 bool4::xwww() const { return bool4(x, w, w, w); }
    inline bool4 bool4::yxxx() const { return bool4(y, x, x, x); }
    inline bool4 bool4::yxxy() const { return bool4(y, x, x, y); }
    inline bool4 bool4::yxxz() const { return bool4(y, x, x, z); }
    inline bool4 bool4::yxxw() const { return bool4(y, x, x, w); }
    inline bool4 bool4::yxyx() const { return bool4(y, x, y, x); }
    inline bool4 bool4::yxyy() const { return bool4(y, x, y, y); }
    inline bool4 bool4::yxyz() const { return bool4(y, x, y, z); }
    inline bool4 bool4::yxyw() const { return bool4(y, x, y, w); }
    inline bool4 bool4::yxzx() const { return bool4(y, x, z, x); }
    inline bool4 bool4::yxzy() const { return bool4(y, x, z, y); }
    inline bool4 bool4::yxzz() const { return bool4(y, x, z, z); }
    inline bool4 bool4::yxzw() const { return bool4(y, x, z, w); }
    inline bool4 bool4::yxwx() const { return bool4(y, x, w, x); }
    inline bool4 bool4::yxwy() const { return bool4(y, x, w, y); }
    inline bool4 bool4::yxwz() const { return bool4(y, x, w, z); }
    inline bool4 bool4::yxww() const { return bool4(y, x, w, w); }
    inline bool4 bool4::yyxx() const { return bool4(y, y, x, x); }
    inline bool4 bool4::yyxy() const { return bool4(y, y, x, y); }
    inline bool4 bool4::yyxz() const { return bool4(y, y, x, z); }
    inline bool4 bool4::yyxw() const { return bool4(y, y, x, w); }
    inline bool4 bool4::yyyx() const { return bool4(y, y, y, x); }
    inline bool4 bool4::yyyy() const { return bool4(y, y, y, y); }
    inline bool4 bool4::yyyz() const { return bool4(y, y, y, z); }
    inline bool4 bool4::yyyw() const { return bool4(y, y, y, w); }
    inline bool4 bool4::yyzx() const { return bool4(y, y, z, x); }
    inline bool4 bool4::yyzy() const { return bool4(y, y, z, y); }
    inline bool4 bool4::yyzz() const { return bool4(y, y, z, z); }
    inline bool4 bool4::yyzw() const { return bool4(y, y, z, w); }
    inline bool4 bool4::yywx() const { return bool4(y, y, w, x); }
    inline bool4 bool4::yywy() const { return bool4(y, y, w, y); }
    inline bool4 bool4::yywz() const { return bool4(y, y, w, z); }
    inline bool4 bool4::yyww() const { return bool4(y, y, w, w); }
    inline bool4 bool4::yzxx() const { return bool4(y, z, x, x); }
    inline bool4 bool4::yzxy() const { return bool4(y, z, x, y); }
    inline bool4 bool4::yzxz() const { return bool4(y, z, x, z); }
    inline bool4 bool4::yzxw() const { return bool4(y, z, x, w); }
    inline bool4 bool4::yzyx() const { return bool4(y, z, y, x); }
    inline bool4 bool4::yzyy() const { return bool4(y, z, y, y); }
    inline bool4 bool4::yzyz() const { return bool4(y, z, y, z); }
    inline bool4 bool4::yzyw() const { return bool4(y, z, y, w); }
    inline bool4 bool4::yzzx() const { return bool4(y, z, z, x); }
    inline bool4 bool4::yzzy() const { return bool4(y, z, z, y); }
    inline bool4 bool4::yzzz() const { return bool4(y, z, z, z); }
    inline bool4 bool4::yzzw() const { return bool4(y, z, z, w); }
    inline bool4 bool4::yzwx() const { return bool4(y, z, w, x); }
    inline bool4 bool4::yzwy() const { return bool4(y, z, w, y); }
    inline bool4 bool4::yzwz() const { return bool4(y, z, w, z); }
    inline bool4 bool4::yzww() const { return bool4(y, z, w, w); }
    inline bool4 bool4::ywxx() const { return bool4(y, w, x, x); }
    inline bool4 bool4::ywxy() const { return bool4(y, w, x, y); }
    inline bool4 bool4::ywxz() const { return bool4(y, w, x, z); }
    inline bool4 bool4::ywxw() const { return bool4(y, w, x, w); }
    inline bool4 bool4::ywyx() const { return bool4(y, w, y, x); }
    inline bool4 bool4::ywyy() const { return bool4(y, w, y, y); }
    inline bool4 bool4::ywyz() const { return bool4(y, w, y, z); }
    inline bool4 bool4::ywyw() const { return bool4(y, w, y, w); }
    inline bool4 bool4::ywzx() const { return bool4(y, w, z, x); }
    inline bool4 bool4::ywzy() const { return bool4(y, w, z, y); }
    inline bool4 bool4::ywzz() const { return bool4(y, w, z, z); }
    inline bool4 bool4::ywzw() const { return bool4(y, w, z, w); }
    inline bool4 bool4::ywwx() const { return bool4(y, w, w, x); }
    inline bool4 bool4::ywwy() const { return bool4(y, w, w, y); }
    inline bool4 bool4::ywwz() const { return bool4(y, w, w, z); }
    inline bool4 bool4::ywww() const { return bool4(y, w, w, w); }
    inline bool4 bool4::zxxx() const { return bool4(z, x, x, x); }
    inline bool4 bool4::zxxy() const { return bool4(z, x, x, y); }
    inline bool4 bool4::zxxz() const { return bool4(z, x, x, z); }
    inline bool4 bool4::zxxw() const { return bool4(z, x, x, w); }
    inline bool4 bool4::zxyx() const { return bool4(z, x, y, x); }
    inline bool4 bool4::zxyy() const { return bool4(z, x, y, y); }
    inline bool4 bool4::zxyz() const { return bool4(z, x, y, z); }
    inline bool4 bool4::zxyw() const { return bool4(z, x, y, w); }
    inline bool4 bool4::zxzx() const { return bool4(z, x, z, x); }
    inline bool4 bool4::zxzy() const { return bool4(z, x, z, y); }
    inline bool4 bool4::zxzz() const { return bool4(z, x, z, z); }
    inline bool4 bool4::zxzw() const { return bool4(z, x, z, w); }
    inline bool4 bool4::zxwx() const { return bool4(z, x, w, x); }
    inline bool4 bool4::zxwy() const { return bool4(z, x, w, y); }
    inline bool4 bool4::zxwz() const { return bool4(z, x, w, z); }
    inline bool4 bool4::zxww() const { return bool4(z, x, w, w); }
    inline bool4 bool4::zyxx() const { return bool4(z, y, x, x); }
    inline bool4 bool4::zyxy() const { return bool4(z, y, x, y); }
    inline bool4 bool4::zyxz() const { return bool4(z, y, x, z); }
    inline bool4 bool4::zyxw() const { return bool4(z, y, x, w); }
    inline bool4 bool4::zyyx() const { return bool4(z, y, y, x); }
    inline bool4 bool4::zyyy() const { return bool4(z, y, y, y); }
    inline bool4 bool4::zyyz() const { return bool4(z, y, y, z); }
    inline bool4 bool4::zyyw() const { return bool4(z, y, y, w); }
    inline bool4 bool4::zyzx() const { return bool4(z, y, z, x); }
    inline bool4 bool4::zyzy() const { return bool4(z, y, z, y); }
    inline bool4 bool4::zyzz() const { return bool4(z, y, z, z); }
    inline bool4 bool4::zyzw() const { return bool4(z, y, z, w); }
    inline bool4 bool4::zywx() const { return bool4(z, y, w, x); }
    inline bool4 bool4::zywy() const { return bool4(z, y, w, y); }
    inline bool4 bool4::zywz() const { return bool4(z, y, w, z); }
    inline bool4 bool4::zyww() const { return bool4(z, y, w, w); }
    inline bool4 bool4::zzxx() const { return bool4(z, z, x, x); }
    inline bool4 bool4::zzxy() const { return bool4(z, z, x, y); }
    inline bool4 bool4::zzxz() const { return bool4(z, z, x, z); }
    inline bool4 bool4::zzxw() const { return bool4(z, z, x, w); }
    inline bool4 bool4::zzyx() const { return bool4(z, z, y, x); }
    inline bool4 bool4::zzyy() const { return bool4(z, z, y, y); }
    inline bool4 bool4::zzyz() const { return bool4(z, z, y, z); }
    inline bool4 bool4::zzyw() const { return bool4(z, z, y, w); }
    inline bool4 bool4::zzzx() const { return bool4(z, z, z, x); }
    inline bool4 bool4::zzzy() const { return bool4(z, z, z, y); }
    inline bool4 bool4::zzzz() const { return bool4(z, z, z, z); }
    inline bool4 bool4::zzzw() const { return bool4(z, z, z, w); }
    inline bool4 bool4::zzwx() const { return bool4(z, z, w, x); }
    inline bool4 bool4::zzwy() const { return bool4(z, z, w, y); }
    inline bool4 bool4::zzwz() const { return bool4(z, z, w, z); }
    inline bool4 bool4::zzww() const { return bool4(z, z, w, w); }
    inline bool4 bool4::zwxx() const { return bool4(z, w, x, x); }
    inline bool4 bool4::zwxy() const { return bool4(z, w, x, y); }
    inline bool4 bool4::zwxz() const { return bool4(z, w, x, z); }
    inline bool4 bool4::zwxw() const { return bool4(z, w, x, w); }
    inline bool4 bool4::zwyx() const { return bool4(z, w, y, x); }
    inline bool4 bool4::zwyy() const { return bool4(z, w, y, y); }
    inline bool4 bool4::zwyz() const { return bool4(z, w, y, z); }
    inline bool4 bool4::zwyw() const { return bool4(z, w, y, w); }
    inline bool4 bool4::zwzx() const { return bool4(z, w, z, x); }
    inline bool4 bool4::zwzy() const { return bool4(z, w, z, y); }
    inline bool4 bool4::zwzz() const { return bool4(z, w, z, z); }
    inline bool4 bool4::zwzw() const { return bool4(z, w, z, w); }
    inline bool4 bool4::zwwx() const { return bool4(z, w, w, x); }
    inline bool4 bool4::zwwy() const { return bool4(z, w, w, y); }
    inline bool4 bool4::zwwz() const { return bool4(z, w, w, z); }
    inline bool4 bool4::zwww() const { return bool4(z, w, w, w); }
    inline bool4 bool4::wxxx() const { return bool4(w, x, x, x); }
    inline bool4 bool4::wxxy() const { return bool4(w, x, x, y); }
    inline bool4 bool4::wxxz() const { return bool4(w, x, x, z); }
    inline bool4 bool4::wxxw() const { return bool4(w, x, x, w); }
    inline bool4 bool4::wxyx() const { return bool4(w, x, y, x); }
    inline bool4 bool4::wxyy() const { return bool4(w, x, y, y); }
    inline bool4 bool4::wxyz() const { return bool4(w, x, y, z); }
    inline bool4 bool4::wxyw() const { return bool4(w, x, y, w); }
    inline bool4 bool4::wxzx() const { return bool4(w, x, z, x); }
    inline bool4 bool4::wxzy() const { return bool4(w, x, z, y); }
    inline bool4 bool4::wxzz() const { return bool4(w, x, z, z); }
    inline bool4 bool4::wxzw() const { return bool4(w, x, z, w); }
    inline bool4 bool4::wxwx() const { return bool4(w, x, w, x); }
    inline bool4 bool4::wxwy() const { return bool4(w, x, w, y); }
    inline bool4 bool4::wxwz() const { return bool4(w, x, w, z); }
    inline bool4 bool4::wxww() const { return bool4(w, x, w, w); }
    inline bool4 bool4::wyxx() const { return bool4(w, y, x, x); }
    inline bool4 bool4::wyxy() const { return bool4(w, y, x, y); }
    inline bool4 bool4::wyxz() const { return bool4(w, y, x, z); }
    inline bool4 bool4::wyxw() const { return bool4(w, y, x, w); }
    inline bool4 bool4::wyyx() const { return bool4(w, y, y, x); }
    inline bool4 bool4::wyyy() const { return bool4(w, y, y, y); }
    inline bool4 bool4::wyyz() const { return bool4(w, y, y, z); }
    inline bool4 bool4::wyyw() const { return bool4(w, y, y, w); }
    inline bool4 bool4::wyzx() const { return bool4(w, y, z, x); }
    inline bool4 bool4::wyzy() const { return bool4(w, y, z, y); }
    inline bool4 bool4::wyzz() const { return bool4(w, y, z, z); }
    inline bool4 bool4::wyzw() const { return bool4(w, y, z, w); }
    inline bool4 bool4::wywx() const { return bool4(w, y, w, x); }
    inline bool4 bool4::wywy() const { return bool4(w, y, w, y); }
    inline bool4 bool4::wywz() const { return bool4(w, y, w, z); }
    inline bool4 bool4::wyww() const { return bool4(w, y, w, w); }
    inline bool4 bool4::wzxx() const { return bool4(w, z, x, x); }
    inline bool4 bool4::wzxy() const { return bool4(w, z, x, y); }
    inline bool4 bool4::wzxz() const { return bool4(w, z, x, z); }
    inline bool4 bool4::wzxw() const { return bool4(w, z, x, w); }
    inline bool4 bool4::wzyx() const { return bool4(w, z, y, x); }
    inline bool4 bool4::wzyy() const { return bool4(w, z, y, y); }
    inline bool4 bool4::wzyz() const { return bool4(w, z, y, z); }
    inline bool4 bool4::wzyw() const { return bool4(w, z, y, w); }
    inline bool4 bool4::wzzx() const { return bool4(w, z, z, x); }
    inline bool4 bool4::wzzy() const { return bool4(w, z, z, y); }
    inline bool4 bool4::wzzz() const { return bool4(w, z, z, z); }
    inline bool4 bool4::wzzw() const { return bool4(w, z, z, w); }
    inline bool4 bool4::wzwx() const { return bool4(w, z, w, x); }
    inline bool4 bool4::wzwy() const { return bool4(w, z, w, y); }
    inline bool4 bool4::wzwz() const { return bool4(w, z, w, z); }
    inline bool4 bool4::wzww() const { return bool4(w, z, w, w); }
    inline bool4 bool4::wwxx() const { return bool4(w, w, x, x); }
    inline bool4 bool4::wwxy() const { return bool4(w, w, x, y); }
    inline bool4 bool4::wwxz() const { return bool4(w, w, x, z); }
    inline bool4 bool4::wwxw() const { return bool4(w, w, x, w); }
    inline bool4 bool4::wwyx() const { return bool4(w, w, y, x); }
    inline bool4 bool4::wwyy() const { return bool4(w, w, y, y); }
    inline bool4 bool4::wwyz() const { return bool4(w, w, y, z); }
    inline bool4 bool4::wwyw() const { return bool4(w, w, y, w); }
    inline bool4 bool4::wwzx() const { return bool4(w, w, z, x); }
    inline bool4 bool4::wwzy() const { return bool4(w, w, z, y); }
    inline bool4 bool4::wwzz() const { return bool4(w, w, z, z); }
    inline bool4 bool4::wwzw() const { return bool4(w, w, z, w); }
    inline bool4 bool4::wwwx() const { return bool4(w, w, w, x); }
    inline bool4 bool4::wwwy() const { return bool4(w, w, w, y); }
    inline bool4 bool4::wwwz() const { return bool4(w, w, w, z); }
    inline bool4 bool4::wwww() const { return bool4(w, w, w, w); }

    inline float Vec4Sum(float4 v)
    {
        return v.x + v.y + v.z + v.w;
    }

    inline float Vec3Sum(float3 v)
    {
        return v.x + v.y + v.z;
    }

    inline float Vec2Sum(float2 v)
    {
        return v.x + v.y;
    }

    inline float4 Vec4Pow(float4 v, float p)
    {
        return Vec4Pow(v, Vec4fSpread(p));
    }

    inline float3 Vec3Pow(float3 v, float p)
    {
        return Vec3Pow(v, Vec3fSpread(p));
    }

    inline float2 Vec2Pow(float2 v, float p)
    {
        return Vec2Pow(v, Vec2fSpread(p));
    }

    inline bool All(bool4 b4)
    {
        return v4All(vLoad(&b4));
    }

    inline bool AllFalse(bool4 b4)
    {
        return v4AllFalse(vLoad(&b4));
    }

    inline bool Any(bool4 b4)
    {
        return v4Any(vLoad(&b4));
    }

    inline bool AnyFalse(bool4 b4)
    {
        return v4AnyFalse(vLoad(&b4));
    }

    inline bool All(bool3 b3)
    {
        return v3All(vLoad(&b3));
    }

    inline bool AllFalse(bool3 b3)
    {
        return v3AllFalse(vLoad(&b3));
    }

    inline bool Any(bool3 b3)
    {
        return v3Any(vLoad(&b3));
    }

    inline bool AnyFalse(bool3 b3)
    {
        return v3AnyFalse(vLoad(&b3));
    }

    inline bool All(bool2 b2)
    {
        return v2All(vLoad(&b2));
    }

    inline bool AllFalse(bool2 b2)
    {
        return v2AllFalse(vLoad(&b2));
    }

    inline bool Any(bool2 b2)
    {
        return v2Any(vLoad(&b2));
    }

    inline bool AnyFalse(bool2 b2)
    {
        return v2AnyFalse(vLoad(&b2));
    }

    inline float DegToRad(float deg)
    {
        return DegToRadFactor * deg;
    }

    inline float RadToDeg(float rad)
    {
        return RadToDegFactor * rad;
    }

    inline float Lerp(float a, float b, float t)
    {
        return (1.f - t) * a + t * b;
    }

    inline float Clamp(float x, float min, float max)
    {
        assert(min <= max);
        return (x < min ? min : x) > max ? max : x;
    }

    inline int32_t Clamp(int32_t x, int32_t min, int32_t max)
    {
        assert(min <= max);
        return (x < min ? min : x) > max ? max : x;
    }

    inline uint32_t Clamp(uint32_t x, uint32_t min, uint32_t max)
    {
        assert(min <= max);
        return (x < min ? min : x) > max ? max : x;
    }

    inline float Min(float a, float b)
    {
        return a < b ? a : b;
    }

    inline int32_t Min(int32_t a, int32_t b)
    {
        return a < b ? a : b;
    }

    inline uint32_t Min(uint32_t a, uint32_t b)
    {
        return a < b ? a : b;
    }

    inline float Max(float a, float b)
    {
        return a > b ? a : b;
    }

    inline int32_t Max(int32_t a, int32_t b)
    {
        return a > b ? a : b;
    }

    inline uint32_t Max(uint32_t a, uint32_t b)
    {
        return a > b ? a : b;
    }
}
