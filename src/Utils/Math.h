#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Utils::Math
{
    float Normalise(float value, float min, float max);

    float Lerp(float origin, float destination, float time);

    glm::vec3 Lerp(glm::vec3 origin, glm::vec3 destination, float time);

    template <typename T> 
    int Sign(T val) {
        return (T(0) < val) - (val < T(0));
    }
}
