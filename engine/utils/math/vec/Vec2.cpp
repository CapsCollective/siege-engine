//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Vec2.h"

#include "../Float.h"
#include "Common.h"

namespace Siege
{

// 'Structors

Vec2::Vec2(const Vec3& other) : x {other.x}, y {other.y} {}

Vec2::Vec2(const Vec4& other) : x {other.x}, y {other.y} {}

// Public Constants

const Vec2 Vec2::Zero = {0.f, 0.f};
const Vec2 Vec2::One = {1.f, 1.f};
const Vec2 Vec2::Up = {0.f, 1.f};

Vec2 Vec2::Lerp(Vec2 origin, Vec2 destination, float time)
{
    return {Float::Lerp(origin.x, destination.x, time), Float::Lerp(origin.y, destination.y, time)};
}

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

// Static Methods

bool Vec2::FromString(OUT Vec2& vec, const String& string)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = string.Split(',');
    if (components.size() != 2) return false;

    // Try to convert the components to float values and return them as a Vector2
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y));
}

Vec2 Vec2::Normalise(const Vec2& vec)
{
    float length = vec.Length();
    length = ((length == 0) * 1.f) + ((length > 0 || length < 0) * length);
    return {Float::Normalise(vec.x, length), Float::Normalise(vec.y, length)};
}

float Vec2::Length(const Vec2& vec)
{
    return ::Siege::Length(vec.x, vec.y);
}

float Vec2::Dot(const Vec2& lhs, const Vec2& rhs)
{
    return ::Siege::Dot(lhs.x, rhs.x, lhs.y, rhs.y);
}

// Public Methods

String Vec2::ToString() const
{
    return String("%.2f,%.2f").Formatted(x, y);
}
Vec2 Vec2::Normalise() const
{
    return Normalise(*this);
}

float Vec2::Length() const
{
    return Length(*this);
}

float Vec2::Dot(const Vec2& other) const
{
    return Dot(*this, other);
}
} // namespace Siege
