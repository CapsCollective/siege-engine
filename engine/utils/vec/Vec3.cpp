//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Vec3.h"
#include "../math/Float.h"

#include "Common.h"

namespace Siege
{
// Public Constants

const Vec3 Vec3::Zero = {0.f, 0.f, 0.f};
const Vec3 Vec3::One = {1.f, 1.f, 1.f};
const Vec3 Vec3::Up = {0.f, 1.f, 0.f};

Vec3 Vec3::Lerp(Vec3 origin, Vec3 destination, float time)
{
    return {Float::Lerp(origin.x, destination.x, time),
            Float::Lerp(origin.y, destination.y, time),
            Float::Lerp(origin.z, destination.z, time)};
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

// Static Methods

bool Vec3::FromString(Vec3& vec, const String& string)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = string.Split(',');
    if (components.size() != 3) return false;

    // Try to convert the components to float values and return them as a Vector3
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y) &&
            components[2].GetFloat(vec.z));
}

Vec3 Vec3::Normalise(const Vec3& vec)
{
    float length = vec.Length();
    length = ((length == 0) * 1.f) + ((length > 0 || length < 0) * length);
    return {::Siege::Normalise(vec.x, length),
            ::Siege::Normalise(vec.y, length),
            ::Siege::Normalise(vec.z, length)};
}

float Vec3::Dot(const Vec3& lhs, const Vec3& rhs)
{
    return ::Siege::Dot(lhs.x, rhs.x, lhs.y, rhs.y, lhs.z, rhs.z);
}

Vec3 Vec3::Cross(const Vec3& lhs, const Vec3& rhs)
{
    return {(lhs.y * rhs.z) - (rhs.y * lhs.z),
            (lhs.z * rhs.x) - (rhs.z * lhs.x),
            (lhs.x * rhs.y) - (rhs.x * lhs.y)};
}

float Vec3::Length(const Vec3& vec)
{
    return ::Siege::Length(vec.x, vec.y, vec.z);
}

// Public Methods

String Vec3::ToString() const
{
    return String("%.2f,%.2f,%.2f").Formatted(x, y, z);
}

Vec3 Vec3::Normalise() const
{
    return Normalise(*this);
}

float Vec3::Dot(const Vec3& other) const
{
    return Dot(*this, other);
}

Vec3 Vec3::Cross(const Vec3& other) const
{
    return Cross(*this, other);
}

float Vec3::Length() const
{
    return Length(*this);
}
} // namespace Siege
