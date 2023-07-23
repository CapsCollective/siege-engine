//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_VEC_EQUALITY_H
#define SIEGE_ENGINE_UTILS_MATH_VEC_EQUALITY_H

#include <cmath>

#include "Vec.h"

namespace Siege
{

/**
 * Calculates the epsilon scaled to the larger of the values to compare. Creates a more generous
 * tolerance level than a standard epsilon
 * @tparam T The type of the values to compare
 * @param lhs the value on the left hand side
 * @param rhs the value on the right hand side
 * @return the scaled epsilon of the two values
 */
template<typename T>
inline constexpr T ScaledEpsilon(T lhs, T rhs)
{
    return std::numeric_limits<T>::epsilon() * std::fmax(std::fabs(lhs), std::fabs(rhs));
}

/**
 * A fuzzy equals comparison function. Checks if two 4D vectors are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the vector
 * @param lhs the vector on the left hand side of the operation
 * @param rhs the vector on the right hand side of the operation
 * @param epsilon the epsilon value used for the acceptable difference
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool Equals(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs, T epsilon = 0)
{
    return (std::fabs(lhs.x - rhs.x) <= epsilon) && (std::fabs(lhs.y - rhs.y) <= epsilon) &&
           (std::fabs(lhs.z - rhs.z) <= epsilon) && (std::fabs(lhs.w - rhs.w) <= epsilon);
}

/**
 * A equals comparison function. Checks if two 3D vectors are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the vector
 * @param lhs the vector on the left hand side of the operation
 * @param rhs the vector on the right hand side of the operation
 * @param epsilon the epsilon value used for the acceptable difference
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool Equals(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs, T epsilon = 0)
{
    return (std::fabs(lhs.x - rhs.x) <= epsilon) && (std::fabs(lhs.y - rhs.y) <= epsilon) &&
           (std::fabs(lhs.z - rhs.z) <= epsilon);
}

/**
 * An comparison function. Checks if two 2D vectors are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the vector
 * @param lhs the vector on the left hand side of the operation
 * @param rhs the vector on the right hand side of the operation
 * @param epsilon the epsilon value used for the acceptable difference
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool Equals(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs, T epsilon = 0)
{
    return (std::fabs(lhs.x - rhs.x) <= epsilon) && (std::fabs(lhs.y - rhs.y) <= epsilon);
}
} // namespace Siege

#endif // SIEGE_ENGINE_EQUALITY_H
