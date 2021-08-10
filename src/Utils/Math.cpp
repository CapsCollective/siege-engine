#include "Math.h"

namespace Utils::Math
{
    float Normalise(float value, float min, float max)
    {
        return 2 * ((value - min) / (max - min)) - 1;
    }
}