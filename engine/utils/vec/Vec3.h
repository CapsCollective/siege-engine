//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC3_H
#define SIEGE_ENGINE_VEC3_H

#include "Vec2.h"

#define VEC_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep

#define VEC_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep z op rhs sep

namespace Siege
{
struct Vec3
{
    // Public constants

    static const Vec3 Zero;

    static const Vec3 One;

    static const Vec3 Up;

    // 'Structors

    Vec3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

    Vec3& operator=(const Vec3& rhs) = default;
    Vec3& operator=(const Vec2& rhs);

    // Operator overloads

    DEFINE_OPERATOR(+=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(-=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(*=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(/=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    bool operator==(const Vec3& other);

    DEFINE_OPERATOR(+=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(-=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(*=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(/=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    DEFINE_CONST_OPERATOR(+, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(-, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(*, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(/, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    DEFINE_CONST_OPERATOR(+, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(-, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(*, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(/, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    // Static methods

    static bool FromString(OUT Vec3& vec, const String& string);

    // Public methods

    String ToString() const;

    Vec3 Normalise() const;

    float Dot(const Vec3& other) const;

    Vec3 Cross(const Vec3& other) const;

    float Length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    Vec3 XComp() const
    {
        return {x, 0.f, 0.f};
    }

    Vec3 YComp() const
    {
        return {0.f, y, 0.f};
    }

    Vec3 ZComp() const
    {
        return {0.f, 0.f, z};
    }

    // Public members

    float x;
    float y;
    float z;
};

inline Vec3 operator/(const float& scalar, const Vec3& vec)
{
    return {scalar / vec.x, scalar / vec.y, scalar / vec.z};
}

inline Vec3 operator*(const float& scalar, const Vec3& vec)
{
    return {scalar * vec.x, scalar * vec.y, scalar * vec.z};
}

inline bool operator==(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

} // namespace Siege

#endif // SIEGE_ENGINE_VEC3_H
