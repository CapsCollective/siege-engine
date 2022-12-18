//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#include "Vec4.h"

#include "../Float.h"
#include "Common.h"

namespace Siege
{
// Public Constants

const Vec4 Vec4::Zero = {0.f, 0.f, 0.f, 0.f};
const Vec4 Vec4::One = {1.f, 1.f, 1.f, 1.f};

// 'Structors

Vec4::Vec4(const Vec2& other) : x {other.x}, y {other.y} {}

Vec4::Vec4(const Vec3& other) : x {other.x}, y {other.y}, z {other.z} {}

// Unary Operators

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

// Static Methods

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

float Vec4::Length(const Vec4 vec)
{
    return ::Siege::Length(vec.x, vec.y, vec.z, vec.w);
}

Vec4 Vec4::Normalise(const Vec4& vec)
{
    float length = vec.Length();
    length = ((length == 0) * 1.f) + ((length > 0 || length < 0) * length);
    return {Float::Normalise(vec.x, length),
            Float::Normalise(vec.y, length),
            Float::Normalise(vec.z, length),
            Float::Normalise(vec.w, length)};
}

float Vec4::Dot(const Vec4& lhs, const Vec4& rhs)
{
    return ::Siege::Dot(lhs.x, rhs.x, lhs.y, rhs.y, lhs.z, rhs.z, lhs.w, rhs.w);
}

// Public Methods

String Vec4::ToString() const
{
    return String("%.2f,%.2f,%.2f,%.2f").Formatted(x, y, z, w);
}

float Vec4::Length() const
{
    return Length(*this);
}

Vec4 Vec4::Normalise() const
{
    return Normalise(*this);
}

float Vec4::Dot(const Vec4& other) const
{
    return Dot(*this, other);
}
} // namespace Siege