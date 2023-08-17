//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_MAT_EQUALITY_H
#define SIEGE_ENGINE_UTILS_MATH_MAT_EQUALITY_H

#include <cmath>

#include "Mat.h"

namespace Siege
{
/**
 * A fuzzy equals comparison function. Checks if two 4x4 matrices are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right hand side of the operation
 * @param epsilon the tolerance level for the comparison
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool Equals(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs, T epsilon = 0)
{
    return (std::fabs(lhs[0][0] - rhs[0][0]) <= epsilon) &&
           (std::fabs(lhs[0][1] - rhs[0][1]) <= epsilon) &&
           (std::fabs(lhs[0][2] - rhs[0][2]) <= epsilon) &&
           (std::fabs(lhs[0][3] - rhs[0][3]) <= epsilon) &&
           (std::fabs(lhs[1][0] - rhs[1][0]) <= epsilon) &&
           (std::fabs(lhs[1][1] - rhs[1][1]) <= epsilon) &&
           (std::fabs(lhs[1][2] - rhs[1][2]) <= epsilon) &&
           (std::fabs(lhs[1][3] - rhs[1][3]) <= epsilon) &&
           (std::fabs(lhs[2][0] - rhs[2][0]) <= epsilon) &&
           (std::fabs(lhs[2][1] - rhs[2][1]) <= epsilon) &&
           (std::fabs(lhs[2][2] - rhs[2][2]) <= epsilon) &&
           (std::fabs(lhs[2][3] - rhs[2][3]) <= epsilon) &&
           (std::fabs(lhs[3][0] - rhs[3][0]) <= epsilon) &&
           (std::fabs(lhs[3][1] - rhs[3][1]) <= epsilon) &&
           (std::fabs(lhs[3][2] - rhs[3][2]) <= epsilon) &&
           (std::fabs(lhs[3][3] - rhs[3][3]) <= epsilon);
}

/**
 * A fuzzy equals comparison function. Checks if two 3x3 matrices are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right hand side of the operation
 * @param epsilon the tolerance level for the comparison
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool Equals(const Mat<T, 3, 3>& lhs, const Mat<T, 3, 3>& rhs, T epsilon = 0)
{
    return (std::fabs(lhs[0][0] - rhs[0][0]) <= epsilon) &&
           (std::fabs(lhs[0][1] - rhs[0][1]) <= epsilon) &&
           (std::fabs(lhs[0][2] - rhs[0][2]) <= epsilon) &&
           (std::fabs(lhs[1][0] - rhs[1][0]) <= epsilon) &&
           (std::fabs(lhs[1][1] - rhs[1][1]) <= epsilon) &&
           (std::fabs(lhs[1][2] - rhs[1][2]) <= epsilon) &&
           (std::fabs(lhs[2][0] - rhs[2][0]) <= epsilon) &&
           (std::fabs(lhs[2][1] - rhs[2][1]) <= epsilon) &&
           (std::fabs(lhs[2][2] - rhs[2][2]) <= epsilon);
}

/**
 * A fuzzy equals comparison function. Checks if two 2x2 matrices are equal within a given tolerance
 * level
 * @tparam T the type of numerical value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right hand side of the operation
 * @param epsilon the tolerance level for the comparison
 * @return a boolean specifying if they are equal within the given tolerance
 */
template<typename T>
inline constexpr bool Equals(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs, T epsilon = 0)
{
    return (std::fabs(lhs[0][0] - rhs[0][0]) <= epsilon) &&
           (std::fabs(lhs[0][1] - rhs[0][1]) <= epsilon) &&
           (std::fabs(lhs[1][0] - rhs[1][0]) <= epsilon) &&
           (std::fabs(lhs[1][1] - rhs[1][1]) <= epsilon);
}
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MATH_MAT_EQUALITY_H
