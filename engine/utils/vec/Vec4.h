//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#ifndef SIEGE_ENGINE_VEC4_H
#define SIEGE_ENGINE_VEC4_H

#include "Vec3.h"

#define VEC4_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep w op rhs.w sep

#define VEC4_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep z op rhs sep w op rhs sep

#define VEC4_OPERATOR_BODY_VEC3(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep w sep

#define VEC4_OPERATOR_BODY_VEC2(op, sep) x op rhs.x sep y op rhs.y sep z sep w sep

namespace Siege
{
struct Vec4
{
    static const Vec4 Zero;

    static const Vec4 One;

    static const Vec4 Up;

    // 'Structors

    Vec4(const float& x = 0.f, const float& y = 0.f, const float& z = 0.f, const float& w = 0.f) :
        x {x},
        y {y},
        z {z},
        w {w}
    {}

    Vec4(const Vec2& other);
    Vec4(const Vec3& other);

    // Operator overloads

    Vec4& operator=(const Vec4& rhs) = default;

    DEFINE_OPERATOR(+=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(-=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(*=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(/=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    DEFINE_OPERATOR_NO_IMP(+=, Vec4, Vec2)
    DEFINE_OPERATOR_NO_IMP(-=, Vec4, Vec2)
    DEFINE_OPERATOR_NO_IMP(*=, Vec4, Vec2)
    DEFINE_OPERATOR_NO_IMP(/=, Vec4, Vec2)

    DEFINE_OPERATOR_NO_IMP(+=, Vec4, Vec3)
    DEFINE_OPERATOR_NO_IMP(-=, Vec4, Vec3)
    DEFINE_OPERATOR_NO_IMP(*=, Vec4, Vec3)
    DEFINE_OPERATOR_NO_IMP(/=, Vec4, Vec3)

    bool operator!=(const Vec4& other);
    bool operator==(const Vec4& other);

    DEFINE_OPERATOR(+=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(-=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(*=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(/=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    DEFINE_CONST_OPERATOR(+, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(-, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(*, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(/, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec4, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec4, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec4, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec4, Vec3)

    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec4, Vec2)
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec4, Vec2)
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec4, Vec2)
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec4, Vec2)

    DEFINE_CONST_OPERATOR(+, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(-, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(*, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(/, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    // Static methods

    static bool FromString(OUT Vec4& vec, const String& string);

    Vec4 XComp() const
    {
        return {x};
    }

    Vec4 YComp() const
    {
        return {0.f, y};
    }

    Vec4 ZComp() const
    {
        return {0.f, 0.f, z};
    }

    Vec4 WComp() const
    {
        return {0.f, 0.f, 0.f, w};
    }

    String ToString() const;

    float Length() const
    {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    Vec4 Normalise() const;

    float x, y, z, w;
};

inline Vec4 operator/(const float& scalar, const Vec4& vec)
{
    return {scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w};
}

inline Vec4 operator*(const float& scalar, const Vec4& vec)
{
    return {scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w};
}

inline bool operator==(const Vec4& lhs, const Vec4& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}
} // namespace Siege

#endif // SIEGE_ENGINE_VEC4_H
