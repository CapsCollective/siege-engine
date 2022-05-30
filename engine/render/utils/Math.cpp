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

    glm::vec3 Lerp(glm::vec3 origin, glm::vec3 destination, float time)
    {
        return { 
            Lerp(origin.x, destination.x, time),
            Lerp(origin.y, destination.y, time),
            Lerp(origin.z, destination.z, time)
        };
    }
}