//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Math.h"

namespace Siege::Utils::Math
{
float Normalise(float value, float min, float max)
{
    return 2 * ((value - min) / (max - min)) - 1;
}

float Lerp(float origin, float destination, float time)
{
    return origin * (1.0f - time) + destination * time;
}

Vec3 Lerp(Vec3 origin, Vec3 destination, float time)
{
    return {Lerp(origin.x, destination.x, time),
            Lerp(origin.y, destination.y, time),
            Lerp(origin.z, destination.z, time)};
}
} // namespace Siege::Utils::Math