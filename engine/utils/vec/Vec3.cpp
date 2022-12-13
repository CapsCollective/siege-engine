//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Vec3.h"

#include "Vec4.h"

namespace Siege
{
// Define static members
const Vec3 Vec3::Zero = {0.f, 0.f, 0.f};
const Vec3 Vec3::One = {1.f, 1.f, 1.f};
const Vec3 Vec3::Up = {0.f, 1.f, 0.f};

Vec3::Vec3(const Vec2& other) : x {other.x}, y {other.y} {}

Vec3::Vec3(const Vec4& other) : x {other.x}, y {other.y}, z {other.z} {}

Vec3 Vec3::Normalise() const
{
    float length = Length();
    if (length == 0.f) length = 1.f;
    return *this * 1.f / length;
}

float Vec3::Dot(const Vec3& other) const
{
    return (x * other.x) + (y * other.y) + (z * other.z);
}

Vec3 Vec3::Cross(const Vec3& other) const
{
    return {(y * other.z) - (other.y * z),
            (z * other.x) - (other.z * x),
            (x * other.y) - (other.x * y)};
}

String Vec3::ToString() const
{
    return String("%.2f,%.2f,%.2f").Formatted(x, y, z);
}

bool Vec3::FromString(Vec3& vec, const String& string)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = string.Split(',');
    if (components.size() != 3) return false;

    // Try to convert the components to float values and return them as a Vector3
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y) &&
            components[2].GetFloat(vec.z));
}

Vec3& Vec3::operator=(const Vec2& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = 0.f;
    return *this;
}

Vec3& Vec3::operator=(const Vec4& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    z = 0.f;
    return *this;
}

Vec3::operator Vec2() const
{
    return {x, y};
}

Vec3::operator Vec4() const
{
    return {x, y, z, 0.f};
}

// Addition

DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec3&, const Vec2&, lhs.x += rhs.x; lhs.y += rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(+=, Vec3&, Vec3&, const Vec3&, lhs.x += rhs.x; lhs.y += rhs.y;
                         lhs.z += rhs.z;
                         return lhs;)
DEFINE_VEC_BINARY_OP_IMP(+=, Vec3&, Vec3&, const Vec4&, lhs.x += rhs.x; lhs.y += rhs.y;
                         lhs.z += rhs.z;
                         return lhs;)

DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec3&, float, lhs.x += rhs; lhs.y += rhs; lhs.z += rhs;)

DEFINE_VEC_BINARY_OP_IMP(
    +, Vec3, const Vec3&, const Vec2&, return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z};)
DEFINE_VEC_BINARY_OP_IMP(
    +, Vec3, const Vec3&, const Vec3&, return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};)
DEFINE_VEC_BINARY_OP_IMP(
    +, Vec3, const Vec3&, const Vec4&, return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};)

DEFINE_VEC_BINARY_OP_IMP(
    +, Vec3, const Vec3&, float, return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};)

// Subtraction

DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec3&, const Vec2&, lhs.x -= rhs.x; lhs.y -= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec3&, const Vec3&, lhs.x -= rhs.x; lhs.y -= rhs.y;
                         lhs.z -= rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec3&, const Vec4&, lhs.x -= rhs.x; lhs.y -= rhs.y;
                         lhs.z -= rhs.z;)

DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec3&, float, lhs.x -= rhs; lhs.y -= rhs; lhs.z -= rhs;)

DEFINE_VEC_BINARY_OP_IMP(
    -, Vec3, const Vec3&, const Vec2&, return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z};)
DEFINE_VEC_BINARY_OP_IMP(
    -, Vec3, const Vec3&, const Vec3&, return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};)
DEFINE_VEC_BINARY_OP_IMP(
    -, Vec3, const Vec3&, const Vec4&, return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};)

DEFINE_VEC_BINARY_OP_IMP(
    -, Vec3, const Vec3&, float, return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};)

// Multiplication

DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec3&, const Vec2&, lhs.x *= rhs.x; lhs.y *= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec3&, const Vec3&, lhs.x *= rhs.x; lhs.y *= rhs.y;
                         lhs.z *= rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec3&, const Vec4&, lhs.x *= rhs.x; lhs.y *= rhs.y;
                         lhs.z *= rhs.z;)

DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec3&, float, lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs;)

DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec3,
                         const Vec3&,
                         const Vec2&,
                         return {lhs.x * rhs.x, lhs.y* rhs.y, lhs.z};)
DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec3,
                         const Vec3&,
                         const Vec3&,
                         return {lhs.x * rhs.x, lhs.y* rhs.y, lhs.z* rhs.z};)
DEFINE_VEC_BINARY_OP_IMP(*,
                         Vec3,
                         const Vec3&,
                         const Vec4&,
                         return {lhs.x * rhs.x, lhs.y* rhs.y, lhs.z* rhs.z};)

DEFINE_VEC_BINARY_OP_IMP(*, Vec3, const Vec3&, float, return {lhs.x * rhs, lhs.y* rhs, lhs.z* rhs};)

// Division

DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec3&, const Vec2&, lhs.x /= rhs.x; lhs.y /= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec3&, const Vec3&, lhs.x /= rhs.x; lhs.y /= rhs.y;
                         lhs.z /= rhs.z;)
DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec3&, const Vec4&, lhs.x /= rhs.x; lhs.y /= rhs.y;
                         lhs.z /= rhs.z;)

DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec3&, float, lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs;)

DEFINE_VEC_BINARY_OP_IMP(
    /, Vec3, const Vec3&, const Vec2&, return {lhs.x / rhs.x, lhs.y / rhs.y, 0.f};)
DEFINE_VEC_BINARY_OP_IMP(
    /, Vec3, const Vec3&, const Vec3&, return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};)
DEFINE_VEC_BINARY_OP_IMP(
    /, Vec3, const Vec3&, const Vec4&, return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};)

DEFINE_VEC_BINARY_OP_IMP(
    /, Vec3, const Vec3&, float, return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};)

bool Vec3::operator==(const Vec3& other)
{
    return x == other.x && y == other.y && z == other.z;
}

bool Vec3::operator!=(const Vec3& other)
{
    return x != other.x || y != other.y || z != other.z;
}

} // namespace Siege