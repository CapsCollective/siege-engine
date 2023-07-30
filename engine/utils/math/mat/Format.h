//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_MAT_FORMAT_H
#define SIEGE_ENGINE_UTILS_MATH_MAT_FORMAT_H

#include "../../String.h"
#include "Mat.h"

namespace Siege
{

/**
 * Converts a 4x4 matrix to a string
 * @tparam T the type of numeric value stored by the matrix
 * @param mat the matrix to be converted to a string
 * @return a string representation of the matrix
 */
template<typename T>
inline String ToString(const Mat<T, 4, 4>& mat)
{
    return String("{%.2f,%.2f,%.2f,%.2f}\n"
                  "{%.2f,%.2f,%.2f,%.2f}\n"
                  "{%.2f,%.2f,%.2f,%.2f}\n"
                  "{%.2f,%.2f,%.2f,%.2f}")
        .Formatted(mat[0][0],
                   mat[0][1],
                   mat[0][2],
                   mat[0][3],
                   mat[1][0],
                   mat[1][1],
                   mat[1][2],
                   mat[1][3],
                   mat[2][0],
                   mat[2][1],
                   mat[2][2],
                   mat[2][3],
                   mat[3][0],
                   mat[3][1],
                   mat[3][2],
                   mat[3][3]);
}

/**
 * Converts a 3x3 matrix to a string
 * @tparam T the type of numeric value stored by the matrix
 * @param mat the matrix to be converted to a string
 * @return a string representation of the matrix
 */
template<typename T>
inline String ToString(const Mat<T, 3, 3>& mat)
{
    return String("{%.2f,%.2f,%.2f}\n"
                  "{%.2f,%.2f,%.2f}\n"
                  "{%.2f,%.2f,%.2f}")
        .Formatted(mat[0][0],
                   mat[0][1],
                   mat[0][2],
                   mat[1][0],
                   mat[1][1],
                   mat[1][2],
                   mat[2][0],
                   mat[2][1],
                   mat[2][2]);
}

/**
 * Converts a 2x2 matrix to a string
 * @tparam T the type of numeric value stored by the matrix
 * @param mat the matrix to be converted to a string
 * @return a string representation of the matrix
 */
template<typename T>
inline String ToString(const Mat<T, 2, 2>& mat)
{
    return String("{%.2f,%.2f}\n"
                  "{%.2f,%.2f}")
        .Formatted(mat[0][0], mat[0][1], mat[1][0], mat[1][1]);
}

// TODO: Is there ever a scenario where we need to create a matrix from a string?
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MATH_MAT_FORMAT_H
