//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Vec2.h"

namespace Siege
{

Vec2::Vec2(const Vec3& other) : x {other.x}, y {other.y} {}

Vec2::Vec2(const Vec4& other) : x {other.x}, y {other.y} {}

// Define static members
const Vec2 Vec2::Zero = {0.f, 0.f};
const Vec2 Vec2::One = {1.f, 1.f};
const Vec2 Vec2::Up = {0.f, 1.f};

// Unary operator implementations.

Vec2& Vec2::operator=(const Vec3& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vec2& Vec2::operator=(const Vec4& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vec2::operator Vec3() const
{
    return {x, y, 0.f};
}

Vec2::operator Vec4() const
{
    return {x, y, 0.f, 0.f};
}

// bool Vec2::operator==(const Vec2& other)
//{
//     return x == other.x && y == other.y;
// }
//
// bool Vec2::operator!=(const Vec2& other)
//{
//     return x != other.x || y != other.y;
// }

// DEFINE_VEC_OP_IMP(+=, Vec2, Vec3, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(-=, Vec2, Vec3, VEC2_OPERATOR_BODY_VEC)
//
// DEFINE_VEC_OP_IMP(+=, Vec2, Vec4, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(-=, Vec2, Vec4, VEC2_OPERATOR_BODY_VEC)

// DEFINE_VEC_CONST_OP_IMP(+, Vec2, Vec3, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_CONST_OP_IMP(-, Vec2, Vec3, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_CONST_OP_IMP(*, Vec2, Vec3, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_CONST_OP_IMP(/, Vec2, Vec3, VEC2_OPERATOR_BODY_VEC)

// DEFINE_VEC_CONST_OP_IMP(+, Vec2, Vec4, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_CONST_OP_IMP(-, Vec2, Vec4, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_CONST_OP_IMP(*, Vec2, Vec4, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_CONST_OP_IMP(/, Vec2, Vec4, VEC2_OPERATOR_BODY_VEC)

bool Vec2::FromString(OUT Vec2& vec, const String& string)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = string.Split(',');
    if (components.size() != 2) return false;

    // Try to convert the components to float values and return them as a Vector2
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y));
}

String Vec2::ToString() const
{
    return String("%.2f,%.2f").Formatted(x, y);
}
Vec2 Vec2::Normalise() const
{
    float length = Length();
    if (length == 0.f) length = 1.f;
    return *this * 1.f / length;
}

float Vec2::Dot(const Vec2& other) const
{
    return (x * other.x) + (y * other.y);
}
} // namespace Siege
