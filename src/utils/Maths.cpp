#include "Maths.h"

#include <iomanip>
#include <sstream>

#include "TransitionAdapter.h"

// Define static members
const Vec3 Vec3::Zero = {0.f, 0.f, 0.f};
const Vec3 Vec3::One = {1.f, 1.f, 1.f};
const Vec3 Vec3::Up = {0.f, 1.f, 0.f};

Vec3 Vec3::Normalise() const
{
    float length = Length();
    if (length == 0.f) length = 1.f;
    return *this * 1.f / length;
}

std::string Vec3::ToString() const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << x << "," << y << "," << z;
    return ss.str();
}

bool BoundedBox::Intersects(const BoundedBox& other) const
{
    if ((max.x >= other.min.x) && (min.x <= other.max.x))
    {
        if ((max.y < other.min.y) || (min.y > other.max.y)) return false;
        if ((max.z < other.min.z) || (min.z > other.max.z)) return false;
    }
    else return false;
    return true;
}

bool BoundedBox::Intersects(const RayCast& ray) const
{
    return GetRayCollisionBox(FromRayCast(ray), FromBoundedBox(*this)).hit;
}