//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MATH_VEC_VEC_H
#define SIEGE_ENGINE_MATH_VEC_VEC_H

#include "../Float.h"

namespace Siege
{
/**
 * The base template type for vectors. Utilised partial template specialisation to allow
 * interoperability between vector types
 * @tparam T the type of the numerical value held by the vector
 * @tparam D the number of dimensions held by the vector
 */
template<typename T, unsigned int D>
struct Vec;

/**
 * A two dimensional vector which stores an x and a y axis value.
 */
typedef Vec<float, 2> Vec2;

/**
 * A three dimensional vector which stores an x, a y, and a z axis value.
 */
typedef Vec<float, 3> Vec3;

/**
 * A four dimensional vector which stores an x, a y, a z, and a w axis value.
 */
typedef Vec<float, 4> Vec4;
} // namespace Siege

#endif // SIEGE_ENGINE_MATH_VEC_VEC_H
