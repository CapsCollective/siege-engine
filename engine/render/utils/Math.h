//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_UTILS_MATH_H
#define SIEGE_ENGINE_RENDERER_UTILS_MATH_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/vec/Vec3.h>

#include <glm/glm.hpp>

namespace Utils::Math
{

class Float
{
public:

    static float Epsilon()
    {
        return std::numeric_limits<float>::epsilon();
    }
};
float Normalise(float value, float min, float max);

float Lerp(float origin, float destination, float time);

Siege::Vec3 Lerp(Siege::Vec3 origin, Siege::Vec3 destination, float time);

template<typename T>
int Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}
} // namespace Utils::Math

#endif