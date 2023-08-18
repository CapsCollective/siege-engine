//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MAT2X2_H
#define SIEGE_ENGINE_UTILS_MAT2X2_H

#include "../vec/Vec2.h"
#include "Mat.h"

namespace Siege
{

template<typename T>
struct Mat<T, 2, 2>
{
    // Static functions & members

    /**
     * Creates a 2x2 matrix with all values set to 0
     * @return a new 2x2 matrix with all values set to 0
     */
    static inline constexpr Mat<T, 2, 2> Zero()
    {
        return Mat<T, 2, 2>(0);
    }

    /**
     * Creates a 2x2 identity matrix
     * @return a new 2x2 identity matrix
     */
    static inline constexpr Mat<T, 2, 2> Identity()
    {
        return Mat<T, 2, 2>(1);
    }

    /**
     * Adds two 2x2 matrices and returns a new matrix with the result of the addition
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix addition
     */
    static inline constexpr Mat<T, 2, 2> Add(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
    {
        return {lhs[0] + rhs[0], lhs[1] + rhs[1]};
    }

    /**
     * Subtracts two 2x2 matrices and returns a new matrix with the result of the subtraction
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix subtraction
     */
    static inline constexpr Mat<T, 2, 2> Subtract(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
    {
        return {lhs[0] - rhs[0], lhs[1] - rhs[1]};
    }

    /**
     * Multiplies two 2x2 matrices and returns a new matrix with the result of the multiplication
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix multiplication
     */
    static inline constexpr Mat<T, 2, 2> Multiply(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
    {
        return {
            {(lhs[0][0] * rhs[0][0]) + (lhs[1][0] * rhs[0][1]),
             (lhs[0][1] * rhs[0][0]) + (lhs[1][1] * rhs[0][1])},
            {(lhs[0][0] * rhs[1][0]) + (lhs[1][0] * rhs[1][1]),
             (lhs[0][1] * rhs[1][0]) + (lhs[1][1] * rhs[1][1])},
        };
    }

    /**
     * Multiplies a 2x2 matrix by a scalar and returns the result in a new matrix
     * @param lhs the matrix to be multiplied
     * @param scalar the scalar to multiply the matrix by
     * @return a new matrix with the result of the multiplication
     */
    static inline constexpr Mat<T, 2, 2> Multiply(const Mat<T, 2, 2>& lhs, T scalar)
    {
        return {{lhs[0] * scalar}, {lhs[1] * scalar}};
    }

    /**
     * Multiplies a 2x2 matrix by a 2D vector and returns a 2D vector with the results
     * @param lhs the matrix to be multiplied
     * @param vector the vector to be multiplied by
     * @return a 2D vector with the results of the multiplication
     */
    static inline constexpr Vec<T, 2> Multiply(const Mat<T, 2, 2>& lhs, const Vec<T, 2> vector)
    {
        return {(lhs[0] * vector.x) + (lhs[1] * vector.y)};
    }

    /**
     * Divides two 2x2 matrices and returns a new matrix with the result of the division
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix division
     */
    static inline constexpr Mat<T, 2, 2> Divide(const Mat<T, 2, 2> lhs, const Mat<T, 2, 2> rhs)
    {
        return Multiply(lhs, Inverse(rhs));
    }

    /**
     * Computes the determinant of a 2x2 matrix
     * @param matrix the matrix who's determinant needs to be computed
     * @return the matrix's determinant
     */
    static inline constexpr T Determinant(const Mat<T, 2, 2>& mat)
    {
        return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
    }

    /**
     * Computes the inverse of a 2x2 matrix
     * @param matrix the matrix who's inverse needs to be computed
     * @return a new matrix with the result of the inverse operation
     */
    static inline constexpr Mat<T, 2, 2> Inverse(const Mat<T, 2, 2>& mat)
    {
        float oneOverDeterminant = 1.f / Determinant(mat);

        return {{mat[1][1] * oneOverDeterminant, -mat[0][1] * oneOverDeterminant},
                {-mat[1][0] * oneOverDeterminant, mat[0][0] * oneOverDeterminant}};
    }

    /**
     * Transposes a 2x2 matrix and returns a new matrix with the results
     * @param matrix the matrix to be computed
     * @return a new matrix with the results of the transpose operation
     */
    static inline constexpr Mat<T, 2, 2> Transpose(const Mat<T, 2, 2>& matrix)
    {
        return {{matrix[0][0], matrix[1][0]}, {matrix[0][1], matrix[1][1]}};
    }

    // 'Structors

    /**
     * The default 2x2 matrix constructor
     */
    inline constexpr Mat<T, 2, 2>() = default;

    /**
     * The base 2x2 matrix constructor. Accepts two 2D vectors for each row in the matrix and
     * returns the final result
     * @param row0 the first row of the matrix
     * @param row1 the second row of the matrix
     */
    inline constexpr Mat<T, 2, 2>(const Vec<T, 2>& row0, const Vec<T, 2>& row1) :
        values {row0, row1}
    {}

    /**
     * A one element partial 2x2 matrix constructor. Accepts one 2D vector for the first row and
     * defaults the rest of the values to 0
     * @param row0 the first row of the matrix
     */
    inline constexpr Mat<T, 2, 2>(const Vec<T, 2>& row0) : Mat<T, 2, 2>(row0, Zero()) {}

    /**
     * A scalar 2x2 matrix constructor. Sets all diagonal elements to the provided scalar value
     * @param value the scalar value to set the identity to
     */
    inline constexpr Mat<T, 2, 2>(T scalar) : Mat<T, 2, 2>({scalar, 0}, {0, scalar}) {}

    /**
     * Creates a 2x2 matrix from a 3x3 matrix
     * @param mat the 3x3 matrix to convert from
     */
    inline constexpr Mat<T, 2, 2>(const Mat<T, 3, 3>& mat) : Mat<T, 2, 2>(mat[0].XY(), mat[1].XY())
    {}

    /**
     * Creates a 2x2 matrix from a 4x4 matrix
     * @param mat the 4x4 matrix to construct the matrix from
     */
    inline constexpr Mat<T, 2, 2>(const Mat<T, 4, 4>& mat) :
        Mat<T, 2, 2>({mat[0].XY()}, {mat[1].XY()})
    {}

    // Operator overloads

    /**
     * The 2x2 matrix subscript operator
     * @param index the index of the row to be accessed
     * @return a const reference to the vector representing the requested row
     */
    inline constexpr Vec<T, 2> operator[](unsigned int index) const
    {
        return values[index];
    }

    /**
     * The 2x2 matrix subscript operator
     * @param index the index of the row to be accessed
     * @return a reference to the vector representing the requested row
     */
    inline constexpr Vec<T, 2>& operator[](unsigned int index)
    {
        return values[index];
    }

    /**
     * The addition operator. Adds the values of the provided matrix to those of the current matrix
     * @param rhs the matrix to add values from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2>& operator+=(const Mat<T, 2, 2>& rhs)
    {
        values[0] += rhs[0];
        values[1] += rhs[1];
        return *this;
    }

    /**
     * The subtraction operator. Subtracts the values of the provided matrix to those of the current
     * matrix
     * @param rhs the matrix to subtract values from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2>& operator-=(const Mat<T, 2, 2>& rhs)
    {
        values[0] -= rhs[0];
        values[1] -= rhs[1];
        return *this;
    }

    /**
     * The multiplication operator. Multiplies the values of the provided matrix to those of the
     * current matrix
     * @param rhs the matrix to multiply values from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2>& operator*=(const Mat<T, 2, 2>& rhs)
    {
        *this = Multiply(*this, rhs);
        return *this;
    }

    /**
     * The scalar multiplication operator. Multiplies the matrix by a scalar value
     * @param scalar the scalar value to multiply by
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2>& operator*=(T scalar)
    {
        *this = Multiply(*this, scalar);
        return *this;
    }

    /**
     * The division operator. Divides the matrix by another matrix
     * @param rhs the matrix to divide by
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2>& operator/=(const Mat<T, 2, 2>& rhs)
    {
        *this = Divide(*this, rhs);
        return *this;
    }

    /**
     * The quality operator. Checks two 2x2 matrices for equality
     * @param rhs the matrix to compare with
     * @return a boolean representing whether the matrices are equal
     */
    inline constexpr bool operator==(const Mat<T, 2, 2>& rhs) const
    {
        return values[0] == rhs[0] && values[1] == rhs[1];
    }

    /**
     * The inequality operator. Checks two 2x2 matrices for non-equality
     * @param rhs the matrix to compare with
     * @return a boolean representing whether the matrices are not equal
     */
    inline constexpr bool operator!=(const Mat<T, 2, 2>& rhs) const
    {
        return values[0] != rhs[0] || values[1] != rhs[1];
    }

    /**
     * The 3x3 matrix assignment operator. Sets the values of the matrix to those of the provided
     * 3x3 matrix
     * @param other the 3x3 matrix to convert from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2> operator=(const Mat<T, 3, 3>& other)
    {
        values[0] = other[0].XY();
        values[1] = other[1].XY();
        return *this;
    }

    /**
     * The 4x4 matrix assignment operator. Sets the values of the matrix to those of the provided
     * 4x4 matrix
     * @param other the 3x3 matrix to convert from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 2, 2> operator=(const Mat<T, 4, 4>& other)
    {
        values[0] = other[0].XY();
        values[1] = other[1].XY();
        return *this;
    }

    Vec<T, 2> values[2] = {Vec<T, 2>::Zero(), Vec<T, 2>::Zero()};
};

/**
 * The binary 2x2 matrix addition operator. Adds two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix addition
 */
template<typename T>
inline constexpr Mat<T, 2, 2> operator+(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
{
    return Mat<T, 2, 2>::Add(lhs, rhs);
}

/**
 * The binary 2x2 matrix subtraction operator. Subtracts two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix subtraction
 */
template<typename T>
inline constexpr Mat<T, 2, 2> operator-(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
{
    return Mat<T, 2, 2>::Subtract(lhs, rhs);
}

/**
 * The binary 2x2 matrix multiplication operator. Multiplies two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix multiplication
 */
template<typename T>
inline constexpr Mat<T, 2, 2> operator*(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
{
    return Mat<T, 2, 2>::Multiply(lhs, rhs);
}

/**
 * A binary 2x2 matrix to scalar multiplication operator. Multiples a matrix by a scalar and returns
 * the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix being multiplied
 * @param scalar the scalar being used to multiply the matrix by
 * @return a new matrix with the result of the scalar operation
 */
template<typename T>
inline constexpr Mat<T, 2, 2> operator*(const Mat<T, 2, 2>& lhs, T scalar)
{
    return Mat<T, 2, 2>::Multiply(lhs, scalar);
}

/**
 * A binary 2x2 matrix to vector multiplication operator. Multiplies a matrix by a vector and
 * returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix being multiplied
 * @param vector the vector the matrix is being multiplied by
 * @return a 2D vector with the results of the multiplication
 */
template<typename T>
inline constexpr Vec<T, 2> operator*(const Mat<T, 2, 2>& lhs, const Vec<T, 2>& vector)
{
    return Mat<T, 2, 2>::Multiply(lhs, vector);
}

/**
 * The binary 2x2 matrix division operator. Divides two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix division
 */
template<typename T>
inline constexpr Mat<T, 2, 2> operator/(const Mat<T, 2, 2>& lhs, const Mat<T, 2, 2>& rhs)
{
    return Mat<T, 2, 2>::Divide(lhs, rhs);
}

/**
 * The 2x2 matrix negation operator. Negates the values of a 2x2 matrix
 * @tparam T the type of numeric value stored by the matrix
 * @param matrix the matrix to be negated
 * @return a new matrix with the negated values
 */
template<typename T>
inline constexpr Mat<T, 2, 2> operator-(const Mat<T, 2, 2>& matrix)
{
    return {-matrix[0], -matrix[1]};
}
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MAT2X2_H
