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
 * A fuzzy equals comparison function. Checks if two 4D vectors are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the vector
 * @param lhs the vector on the left hand side of the operation
 * @param rhs the vector on the right hand side of the operation
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool FEquals(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
{
    T epsilon = std::numeric_limits<T>::epsilon();

    return (lhs.x - rhs.x <= epsilon) && (lhs.y - rhs.y <= epsilon) && (lhs.z - rhs.z <= epsilon) &&
           (lhs.w - rhs.w <= epsilon);
}

/**
 * A fuzzy equals comparison function. Checks if two 3D vectors are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the vector
 * @param lhs the vector on the left hand side of the operation
 * @param rhs the vector on the right hand side of the operation
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool FEquals(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
{
    T epsilon = std::numeric_limits<T>::epsilon();

    return (lhs.x - rhs.x <= epsilon) && (lhs.y - rhs.y <= epsilon) && (lhs.z - rhs.z <= epsilon);
}

/**
 * A fuzzy equals comparison function. Checks if two 2D vectors are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the vector
 * @param lhs the vector on the left hand side of the operation
 * @param rhs the vector on the right hand side of the operation
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool FEquals(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
{
    T epsilon = std::numeric_limits<T>::epsilon();
    return (lhs.x - rhs.x <= epsilon) && (lhs.y - rhs.y <= epsilon);
}
} // namespace Siege

#endif // SIEGE_ENGINE_EQUALITY_H
