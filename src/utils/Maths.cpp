#include "Maths.h"

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