//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Maths.h"

namespace Siege
{
// -------------------------------------- BoundedBox -----------------------------------------------

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
}