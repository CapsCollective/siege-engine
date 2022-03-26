#include "Maths.h"

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
    // If the position is inside the box, the distance is negative, reversing the
    // direction will in this case give us the correct result.
    bool insideBox = (ray.position.x > min.x) && (ray.position.x < max.x) &&
                     (ray.position.y > min.y) && (ray.position.y < max.y) &&
                     (ray.position.z > min.z) && (ray.position.z < max.z);
    Vec3 direction = insideBox ? ray.direction * -1 : ray.direction;

    float t[11] = {};
    t[8] = 1.0f / direction.x;
    t[9] = 1.0f / direction.y;
    t[10] = 1.0f / direction.z;

    t[0] = (min.x - ray.position.x) * t[8];
    t[1] = (max.x - ray.position.x) * t[8];
    t[2] = (min.y - ray.position.y) * t[9];
    t[3] = (max.y - ray.position.y) * t[9];
    t[4] = (min.z - ray.position.z) * t[10];
    t[5] = (max.z - ray.position.z) * t[10];
    t[6] = (float) fmax(fmax(fmin(t[0], t[1]), fmin(t[2], t[3])), fmin(t[4], t[5]));
    t[7] = (float) fmin(fmin(fmax(t[0], t[1]), fmax(t[2], t[3])), fmax(t[4], t[5]));

    return !((t[7] < 0) || (t[6] > t[7]));
}