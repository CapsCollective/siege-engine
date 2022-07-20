//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Float.h"

namespace Siege
{
int8_t Float::Sign(float val)
{
    return (0 < val) - (val < 0);
}

float Float::Normalise(const float& value)
{
    return Normalise(value, 0, 1);
}

float Float::Normalise(float value, float min, float max)
{
    return 2 * ((value - min) / (max - min)) - 1;
}

float Float::Lerp(float origin, float destination, float time)
{
    return origin * (1.0f - time) + destination * time;
}

float Float::Abs(const float& value)
{
    return (static_cast<float>(-(value) < 0) * value) + (static_cast<float>(-(value) > 0) * -value);
}

float Float::Max(const float& lhs, const float& rhs)
{
    return ((lhs + rhs + Abs(lhs - rhs)) / 2);
}

float Float::Min(const float& lhs, const float& rhs)
{
    return ((lhs + rhs - Abs(lhs - rhs)) / 2);
}

float Float::Clamp(const float& value, const float& min, const float& max)
{
    return Min(Max(value, min), max);
}
}
