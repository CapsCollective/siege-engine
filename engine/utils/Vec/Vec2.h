//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC2_H
#define SIEGE_ENGINE_VEC2_H

#include <cmath>

#include "../String.h"
#include "Macros.h"
#include "Vec.h"

#define VEC2_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep

#define VEC2_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep

namespace Siege::Vec
{

struct Vec2
{
    // Public constants

    static const Vec2 Zero;
    static const Vec2 One;
    static const Vec2 Up;

    // 'Structors

    Vec2(const float& x = 0.f, const float& y = 0.f) : x(x), y(y) {}
    Vec2(const Vec3& other);
    Vec2(const Vec4& other);

    // Operator overloads

    Vec2& operator=(const Vec2& rhs) = default;

    DEFINE_OPERATOR(+=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(-=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(*=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(/=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    DEFINE_OPERATOR_NO_IMP(+=, Vec2, Vec3)
    DEFINE_OPERATOR_NO_IMP(-=, Vec2, Vec3)
    DEFINE_OPERATOR_NO_IMP(*=, Vec2, Vec3)
    DEFINE_OPERATOR_NO_IMP(/=, Vec2, Vec3)

    DEFINE_OPERATOR_NO_IMP(+=, Vec2, Vec4)
    DEFINE_OPERATOR_NO_IMP(-=, Vec2, Vec4)
    DEFINE_OPERATOR_NO_IMP(*=, Vec2, Vec4)
    DEFINE_OPERATOR_NO_IMP(/=, Vec2, Vec4)

    bool operator==(const Vec2& other);
    bool operator!=(const Vec2& other);

    DEFINE_OPERATOR(+=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(-=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(*=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(/=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    DEFINE_CONST_OPERATOR(+, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(-, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(*, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(/, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec2, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec2, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec2, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec2, Vec3)

    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec2, Vec4)
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec2, Vec4)
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec2, Vec4)
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec2, Vec4)

    DEFINE_CONST_OPERATOR(+, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(-, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(*, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(/, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    // Static methods

    static bool FromString(OUT Vec2& vec, const String& string);

    // Public methods

    String ToString() const;

    Vec2 Normalise() const;

    float Length() const
    {
        return sqrtf(x * x + y * y);
    }

    Vec2 XComp() const
    {
        return {
            x,
            0.f,
        };
    }

    Vec2 YComp() const
    {
        return {0.f, y};
    }

    float x, y;
};
} // namespace Siege::Vec

#endif // SIEGE_ENGINE_VEC2_H
