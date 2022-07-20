//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FLOAT_H
#define SIEGE_ENGINE_FLOAT_H

#include <cmath>

namespace Siege {

class Float
{
public:
    static constexpr float Epsilon()
    {
        return std::numeric_limits<float>::epsilon();
    }

    static int8_t Sign(float val);

    static float Normalise(const float& value);

    static float Normalise(float value, float min, float max);

    static float Lerp(float origin, float destination, float time);

    static float Abs(const float& value);

    static float Max(const float& lhs, const float& rhs);

    static float Min(const float& lhs, const float& rhs);

    static float Clamp(const float& value, const float& min, const float& max);
};

}

#endif // SIEGE_ENGINE_FLOAT_H
