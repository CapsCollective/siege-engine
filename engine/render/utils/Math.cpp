//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Math.h"

namespace Utils::Math
{
float Normalise(float value, float min, float max)
{
    return 2 * ((value - min) / (max - min)) - 1;
}

float Lerp(float origin, float destination, float time)
{
    return origin * (1.0f - time) + destination * time;
}

Siege::Vec3 Lerp(Siege::Vec3 origin, Siege::Vec3 destination, float time)
{
    return {Lerp(origin.x, destination.x, time),
            Lerp(origin.y, destination.y, time),
            Lerp(origin.z, destination.z, time)};
}
} // namespace Utils::Math