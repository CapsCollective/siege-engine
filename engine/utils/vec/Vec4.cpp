//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#include "Vec4.h"

namespace Siege
{
// Define static members
const Vec4 Vec4::Zero = {0.f, 0.f, 0.f, 0.f};
const Vec4 Vec4::One = {1.f, 1.f, 1.f, 1.f};

Vec4& Vec4::operator=(const Vec2& rhs)
{
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vec4& Vec4::operator=(const Vec3& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

Vec4& Vec4::operator=(const Vec4& rhs)
{
    x = rhs.x, y = rhs.y, z = rhs.z, w = rhs.w;
    return *this;
}

Vec4::operator Vec2() const
{
    return {x, y};
}

Vec4::operator Vec3() const
{
    return {x, y, z};
}

bool Vec4::FromString(OUT Vec4& vec, const String& string)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = string.Split(',');
    if (components.size() != 4) return false;

    // Try to convert the components to float values and return them as a Vector4
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y) &&
            components[2].GetFloat(vec.z) && components[3].GetFloat(vec.w));
}

bool Vec4::operator==(const Vec4& other)
{
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Vec4::operator!=(const Vec4& other)
{
    return x != other.x || y != other.y || z != other.z || w != other.w;
}

// DEFINE_VEC_OP_IMP(+=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(-=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(*=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(/=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
//
// DEFINE_VEC_OP_IMP(+=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(-=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(*=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
// DEFINE_VEC_OP_IMP(/=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
//
// DEFINE_VEC_CONST_OP_IMP(+, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
// DEFINE_VEC_CONST_OP_IMP(-, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
// DEFINE_VEC_CONST_OP_IMP(*, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
// DEFINE_VEC_CONST_OP_IMP(/, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
//
// DEFINE_VEC_CONST_OP_IMP(+, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)
// DEFINE_VEC_CONST_OP_IMP(-, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)
// DEFINE_VEC_CONST_OP_IMP(*, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)
// DEFINE_VEC_CONST_OP_IMP(/, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)

String Vec4::ToString() const
{
    return String("%.2f,%.2f,%.2f,%.2f").Formatted(x, y, z, w);
}

Vec4 Vec4::Normalise() const
{
    float length = Length();
    if (length == 0.f) length = 1.f;
    return *this * 1.f / length;
}

float Vec4::Dot(const Vec4& other) const
{
    return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

Vec4::Vec4(const Vec2& other) : x {other.x}, y {other.y} {}

Vec4::Vec4(const Vec3& other) : x {other.x}, y {other.y}, z {other.z} {}

// Addition

// static Vec4 operator+(const Vec4& lhs, float rhs);

DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec4&, const Vec2&, lhs.x += rhs.x; lhs.y += rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec4&, const Vec3&, lhs.x += rhs.x; lhs.y += rhs.y;
                         lhs.z += rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec4&, const Vec4&, lhs.x += rhs.x; lhs.y += rhs.y;
                         lhs.z += rhs.z;
                         lhs.w += rhs.w;)

DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec4&, float, lhs.x += rhs; lhs.y += rhs; lhs.z += rhs;
                         lhs.w += rhs;)

DEFINE_VEC_BINARY_OP_IMP(
    +, Vec4, const Vec4&, const Vec2&, return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(
    +, Vec4, const Vec4&, const Vec3&, return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(+,
                         Vec4,
                         const Vec4&,
                         const Vec4&,
                         return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};)

DEFINE_VEC_BINARY_OP_IMP(
    +, Vec4, const Vec4&, float, return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs};)

// Subtraction

DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec4&, const Vec2&, lhs.x -= rhs.x; lhs.y -= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec4&, const Vec3&, lhs.x -= rhs.x; lhs.y -= rhs.y;
                         lhs.z -= rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec4&, const Vec4&, lhs.x -= rhs.x; lhs.y -= rhs.y;
                         lhs.z -= rhs.z;
                         lhs.w -= rhs.w;)

DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec4&, float, lhs.x -= rhs; lhs.y -= rhs; lhs.z -= rhs;
                         lhs.w -= rhs;)

DEFINE_VEC_BINARY_OP_IMP(
    -, Vec4, const Vec4&, const Vec2&, return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(
    -, Vec4, const Vec4&, const Vec3&, return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(-,
                         Vec4,
                         const Vec4&,
                         const Vec4&,
                         return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};)

DEFINE_VEC_BINARY_OP_IMP(
    -, Vec4, const Vec4&, float, return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs};)

// Multiplication

DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec4&, const Vec2&, lhs.x *= rhs.x; lhs.y *= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec4&, const Vec3&, lhs.x *= rhs.x; lhs.y *= rhs.y;
                         lhs.z *= rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec4&, const Vec4&, lhs.x *= rhs.x; lhs.y *= rhs.y;
                         lhs.z *= rhs.z;
                         lhs.w *= rhs.w;)

DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec4&, float, lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs;
                         lhs.w *= rhs;)

DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec4,
                         const Vec4&,
                         const Vec2&,
                         return {lhs.x * rhs.x, lhs.y* rhs.y, lhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec4,
                         const Vec4&,
                         const Vec3&,
                         return {lhs.x * rhs.x, lhs.y* rhs.y, lhs.z* rhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec4,
                         const Vec4&,
                         const Vec4&,
                         return {lhs.x * rhs.x, lhs.y* rhs.y, lhs.z* rhs.z, lhs.w* rhs.w};)

DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec4,
                         const Vec4&,
                         float,
                         return {lhs.x * rhs, lhs.y* rhs, lhs.z* rhs, lhs.w* rhs};)

// Division

DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec4&, const Vec2&, lhs.x /= rhs.x; lhs.y /= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec4&, const Vec3&, lhs.x /= rhs.x; lhs.y /= rhs.y;
                         lhs.z /= rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec4&, const Vec4&, lhs.x /= rhs.x; lhs.y /= rhs.y;
                         lhs.z /= rhs.z;
                         lhs.w /= rhs.w;)

DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec4&, float, lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs;
                         lhs.w /= rhs;)

DEFINE_VEC_BINARY_OP_IMP(
    /, Vec4, const Vec4&, const Vec2&, return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(
    /, Vec4, const Vec4&, const Vec3&, return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w};)
DEFINE_VEC_BINARY_OP_IMP(/,
                         Vec4,
                         const Vec4&,
                         const Vec4&,
                         return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w};)

DEFINE_VEC_BINARY_OP_IMP(
    /, Vec4, const Vec4&, float, return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs};)
} // namespace Siege