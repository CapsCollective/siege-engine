//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Common.h"

#define SQRT_MAGIC_F 0x5f3759df

namespace Siege
{
float FastSqrt(float n)
{
    const float xhalf = 0.5f * n;

    union // get bits for floating value
    {
        float x;
        int i;
    } u;
    u.x = n;
    u.i = SQRT_MAGIC_F - (u.i >> 1); // gives initial guess y0
    return n * u.x * (1.5f - xhalf * u.x * u.x); // Newton step, repeating increases accuracy
}
} // namespace Siege