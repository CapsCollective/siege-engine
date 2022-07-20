//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#include "Vec4.h"
#include "../math/Float.h"

namespace Siege
{
// Define static members
const Vec4 Vec4::Zero = {0.f, 0.f, 0.f, 0.f};
const Vec4 Vec4::One = {1.f, 1.f, 1.f, 1.f};
const Vec4 Vec4::Up = {0.f, 1.f, 0.f, 0.f};

Vec4::Vec4(const Vec2& other) : x {other.x}, y {other.y}, z {0.f}, w {0.f} {}
Vec4::Vec4(const Vec3& other) : x {other.x}, y {other.y}, z {other.z}, w {0.f} {}

Vec4 Vec4::Lerp(Vec4 origin, Vec4 destination, float time)
{
    return {Float::Lerp(origin.x, destination.x, time),
            Float::Lerp(origin.y, destination.y, time),
            Float::Lerp(origin.z, destination.z, time),
            Float::Lerp(origin.w, destination.w, time)};
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

DEFINE_OPERATOR_IMP(+=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
DEFINE_OPERATOR_IMP(-=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
DEFINE_OPERATOR_IMP(*=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)
DEFINE_OPERATOR_IMP(/=, Vec4, Vec3, VEC_OPERATOR_BODY_VEC)

DEFINE_OPERATOR_IMP(+=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
DEFINE_OPERATOR_IMP(-=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
DEFINE_OPERATOR_IMP(*=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)
DEFINE_OPERATOR_IMP(/=, Vec4, Vec2, VEC2_OPERATOR_BODY_VEC)

DEFINE_CONST_OPERATOR_IMP(+, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
DEFINE_CONST_OPERATOR_IMP(-, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
DEFINE_CONST_OPERATOR_IMP(*, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)
DEFINE_CONST_OPERATOR_IMP(/, Vec4, Vec3, VEC4_OPERATOR_BODY_VEC3)

DEFINE_CONST_OPERATOR_IMP(+, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)
DEFINE_CONST_OPERATOR_IMP(-, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)
DEFINE_CONST_OPERATOR_IMP(*, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)
DEFINE_CONST_OPERATOR_IMP(/, Vec4, Vec2, VEC4_OPERATOR_BODY_VEC2)

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
} // namespace Siege