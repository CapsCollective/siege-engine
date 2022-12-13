//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COMMON_H
#define SIEGE_ENGINE_COMMON_H

#include <cmath>

#include "../Macros.h"

namespace Siege
{

/**
 * Computes the square root of a float
 * @param n the number to evaluate
 * @return a float value representing the square root
 */
inline constexpr float FastSqrt(float n)
{
    return sqrtf(n);
}

/**
 * computes the dot product of a set of floats.
 * NOTE: all dimensions must be provided as pairs (i.e: x0 must have an x1 value provided)
 * @param x0 the x dimension on the first vector
 * @param x1 the x dimension on the second vector
 * @param y0 the y dimension on the first vector
 * @param y1 the y dimension on the second vector
 * @param z0 the z dimension on the first vector
 * @param z1 the z dimension on the second vector
 * @param w0 the w dimension on the first vector
 * @param w1 the w dimension on the second vector
 * @return a float value representing the dot product
 */
inline constexpr float Dot(float x0 = 0,
                           float x1 = 0,
                           float y0 = 0,
                           float y1 = 0,
                           float z0 = 0,
                           float z1 = 0,
                           float w0 = 0,
                           float w1 = 0)
{
    return (x0 * x1) + (y0 * y1) + (z0 * z1) + (w0 * w1);
}

/**
 * Computes the length of a vector of the given coordinates
 * @param x the x dimension of the vector
 * @param y the y dimension of the vector
 * @param z the z dimension of the vector
 * @param w the w dimension of the vector
 * @return the length of the vector represented as a float
 */
inline constexpr float Length(float x = 0, float y = 0, float z = 0, float w = 0)
{
    return FastSqrt((x * x) + (y * y) + (z * z) + (w * w));
}

/**
 * Normalises a value by a given maximum value
 * @param value the value to normalise
 * @param max the maximum it should be normalised from
 * @return a float representing a normalised value between 0 and 1
 */
inline constexpr float Normalise(float value, float max)
{
    return value * 1.f / max;
}
} // namespace Siege

#endif // SIEGE_ENGINE_COMMON_H
