//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT_H
#define SIEGE_ENGINE_MAT_H

namespace Siege
{
/**
 * The base matrix template specialisation
 * @tparam T the type of numerical value held by the matrix
 * @tparam R the number of rows stored by the matrix
 * @tparam C the number of columns stored by the matrix
 */
template<typename T, unsigned int R, unsigned int C>
struct Mat;

/**
 * A 4x4 matrix which stores all values as floating point numbers
 */
typedef Mat<float, 4, 4> Mat4;

/**
 * A 3x3 matrix which stores all values as floating point numbers
 */
typedef Mat<float, 3, 3> Mat3;

/**
 * A 2x2 matrix which stores all values as floating point numbers
 */
typedef Mat<float, 2, 2> Mat2;

} // namespace Siege

#endif // SIEGE_ENGINE_MAT_H
