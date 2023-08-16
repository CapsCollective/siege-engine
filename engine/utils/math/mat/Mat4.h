//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MAT4X4_H
#define SIEGE_ENGINE_UTILS_MAT4X4_H

#include "../vec/Vec4.h"
#include "Mat.h"

namespace Siege
{
template<typename T>
struct Mat<T, 4, 4>
{
    // Static functions

    /**
     * Creates a 4x4 matrix with all values set to 0
     * @return a new 4x4 matrix with all values set to 0
     */
    static inline constexpr Mat<T, 4, 4> Zero()
    {
        return Mat<T, 4, 4>(0);
    }

    /**
     * Creates a 4x4 identity matrix
     * @return a new 4x4 identity matrix
     */
    static inline constexpr Mat<T, 4, 4> Identity()
    {
        return Mat<T, 4, 4>(1);
    }

    /**
     * Adds two 4x4 matrices and returns a new matrix with the result of the addition
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix addition
     */
    static inline constexpr Mat<T, 4, 4> Add(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
    {
        return {lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3]};
    }

    /**
     * Subtracts two 4x4 matrices and returns a new matrix with the result of the subtraction
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix subtraction
     */
    static inline constexpr Mat<T, 4, 4> Subtract(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
    {
        return {lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3]};
    }

    /**
     * Multiplies two 4x4 matrices and returns a new matrix with the result of the multiplication
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix multiplication
     */
    static inline constexpr Mat<T, 4, 4> Multiply(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
    {
        return {
            {lhs[0] * rhs[0][0] + lhs[1] * rhs[0][1] + lhs[2] * rhs[0][2] + lhs[3] * rhs[0][3]},
            {lhs[0] * rhs[1][0] + lhs[1] * rhs[1][1] + lhs[2] * rhs[1][2] + lhs[3] * rhs[1][3]},
            {lhs[0] * rhs[2][0] + lhs[1] * rhs[2][1] + lhs[2] * rhs[2][2] + lhs[3] * rhs[2][3]},
            {lhs[0] * rhs[3][0] + lhs[1] * rhs[3][1] + lhs[2] * rhs[3][2] + lhs[3] * rhs[3][3]}};
    }

    /**
     * Multiplies a 4x4 matrix by a scalar and returns the result in a new matrix
     * @param lhs the matrix to be multiplied
     * @param scalar the scalar to multiply the matrix by
     * @return a new matrix with the result of the multiplication
     */
    static inline constexpr Mat<T, 4, 4> Multiply(const Mat<T, 4, 4>& lhs, T scalar)
    {
        return {{lhs[0] * scalar}, {lhs[1] * scalar}, {lhs[2] * scalar}, {lhs[3] * scalar}};
    }

    /**
     * Multiplies a 4x4 matrix by a 4D vector and returns a 4D vector with the results
     * @param lhs the matrix to be multiplied
     * @param vector the vector to be multiplied by
     * @return a 4D vector with the results of the multiplication
     */
    static inline constexpr Vec<T, 4> Multiply(const Mat<T, 4, 4>& lhs, const Vec<T, 4> vector)
    {
        return {(lhs[0] * vector.x) + (lhs[1] * vector.y) + (lhs[2] * vector.z) +
                (lhs[3] * vector.w)};
    }

    /**
     * Divides two 4x4 matrices and returns a new matrix with the result of the division
     * @param lhs the matrix on the left hand side
     * @param rhs the matrix on the right hand side
     * @return a new matrix with the results of the matrix division
     */
    static inline constexpr Mat<T, 4, 4> Divide(const Mat<T, 4, 4> lhs, const Mat<T, 4, 4> rhs)
    {
        return Multiply(lhs, Inverse(rhs));
    }

    /**
     * Computes the determinant of a 4x4 matrix
     * @param matrix the matrix who's determinant needs to be computed
     * @return the matrix's determinant
     */
    static inline constexpr T Determinant(const Mat<T, 4, 4>& matrix)
    {
        float subFactor00 = matrix[2][2] * matrix[3][3] - matrix[2][3] * matrix[3][2];
        float subFactor01 = matrix[1][2] * matrix[3][3] - matrix[1][3] * matrix[3][2];
        float subFactor02 = matrix[1][2] * matrix[2][3] - matrix[1][3] * matrix[2][2];
        float subFactor03 = matrix[0][2] * matrix[3][3] - matrix[0][3] * matrix[3][2];
        float subFactor04 = matrix[0][2] * matrix[2][3] - matrix[0][3] * matrix[2][2];
        float subFactor05 = matrix[0][2] * matrix[1][3] - matrix[0][3] * matrix[1][2];

        Vec<T, 4> coefficient = {
            +(matrix[1][1] * subFactor00 - matrix[2][1] * subFactor01 + matrix[3][1] * subFactor02),
            -(matrix[0][1] * subFactor00 - matrix[2][1] * subFactor03 + matrix[3][1] * subFactor04),
            +(matrix[0][1] * subFactor01 - matrix[1][1] * subFactor03 + matrix[3][1] * subFactor05),
            -(matrix[0][1] * subFactor02 - matrix[1][1] * subFactor04 + matrix[2][1] * subFactor05),
        };

        return matrix[0][0] * coefficient.x + matrix[1][0] * coefficient.y +
               matrix[2][0] * coefficient.z + matrix[3][0] * coefficient.w;
    }

    /**
     * Computes the inverse of a 4x4 matrix
     * @param matrix the matrix who's inverse needs to be computed
     * @return a new matrix with the result of the inverse operation
     */
    static inline constexpr Mat<T, 4, 4> Inverse(const Mat<T, 4, 4>& matrix)
    {
        T coef0 = matrix[2][2] * matrix[3][3] - matrix[3][2] * matrix[2][3];
        T coef2 = matrix[2][1] * matrix[3][3] - matrix[3][1] * matrix[2][3];
        T coef3 = matrix[2][1] * matrix[3][2] - matrix[3][1] * matrix[2][2];
        T coef4 = matrix[2][0] * matrix[3][3] - matrix[3][0] * matrix[2][3];
        T coef5 = matrix[2][0] * matrix[3][2] - matrix[3][0] * matrix[2][2];
        T coef6 = matrix[2][0] * matrix[3][1] - matrix[3][0] * matrix[2][1];

        Vec<T, 4> fac0(coef0,
                       coef0,
                       matrix[1][2] * matrix[3][3] - matrix[3][2] * matrix[1][3],
                       matrix[1][2] * matrix[2][3] - matrix[2][2] * matrix[1][3]);

        Vec<T, 4> fac1(coef2,
                       coef2,
                       matrix[1][1] * matrix[3][3] - matrix[3][1] * matrix[1][3],
                       matrix[1][1] * matrix[2][3] - matrix[2][1] * matrix[1][3]);

        Vec<T, 4> fac2(coef3,
                       coef3,
                       matrix[1][1] * matrix[3][2] - matrix[3][1] * matrix[1][2],
                       matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2]);

        Vec<T, 4> fac3(coef4,
                       coef4,
                       matrix[1][0] * matrix[3][3] - matrix[3][0] * matrix[1][3],
                       matrix[1][0] * matrix[2][3] - matrix[2][0] * matrix[1][3]);

        Vec<T, 4> fac4(coef5,
                       coef5,
                       matrix[1][0] * matrix[3][2] - matrix[3][0] * matrix[1][2],
                       matrix[1][0] * matrix[2][2] - matrix[2][0] * matrix[1][2]);

        Vec<T, 4> fac5(coef6,
                       coef6,
                       matrix[1][0] * matrix[3][1] - matrix[3][0] * matrix[1][1],
                       matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1]);

        Vec<T, 4> vec0(matrix[1][0], matrix[0][0], matrix[0][0], matrix[0][0]);
        Vec<T, 4> vec1(matrix[1][1], matrix[0][1], matrix[0][1], matrix[0][1]);
        Vec<T, 4> vec2(matrix[1][2], matrix[0][2], matrix[0][2], matrix[0][2]);
        Vec<T, 4> vec3(matrix[1][3], matrix[0][3], matrix[0][3], matrix[0][3]);

        Vec<T, 4> inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
        Vec<T, 4> inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
        Vec<T, 4> inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
        Vec<T, 4> inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

        Vec<T, 4> signA(+1, -1, +1, -1);
        Vec<T, 4> signB(-1, +1, -1, +1);
        Mat<T, 4, 4> inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

        Vec<T, 4> column0(inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]);

        Vec<T, 4> dot0(matrix[0] * column0);

        T oneOverDeterminant = static_cast<T>(1) / ((dot0.x + dot0.y) + (dot0.z + dot0.w));

        return inverse * oneOverDeterminant;
    }

    /**
     * Transposes a 4x4 matrix and returns a new matrix with the results
     * @param matrix the matrix to be computed
     * @return a new matrix with the results of the transpose operation
     */
    static inline constexpr Mat<T, 4, 4> Transpose(const Mat<T, 4, 4>& matrix)
    {
        return {{matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]},
                {matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]},
                {matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]},
                {matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]}};
    }

    // 'Structors

    /**
     * The default 4x4 matrix constructor
     */
    inline constexpr Mat<T, 4, 4>() = default;

    /**
     * The base 4x4 matrix constructor. Accepts four 4D vectors for each row in the matrix and
     * returns the final result
     * @param row0 the first row of the matrix
     * @param row1 the second row of the matrix
     * @param row2 the third row of the matrix
     * @param row3 the fourth row of the matrix
     */
    inline constexpr Mat<T, 4, 4>(const Vec<T, 4>& row0,
                                  const Vec<T, 4>& row1,
                                  const Vec<T, 4>& row2,
                                  const Vec<T, 4>& row3) :
        values {row0, row1, row2, row3}
    {}

    /**
     * A one element partial 4x4 matrix constructor. Accepts one 4D vector for the first row and
     * defaults the rest of the values to 0
     * @param row0 the first row of the matrix
     */
    inline constexpr Mat<T, 4, 4>(const Vec<T, 4>& row0) :
        Mat<T, 4, 4>(row0, Mat<T, 4, 4>::Zero(), Mat<T, 4, 4>::Zero(), Mat<T, 4, 4>::Zero())
    {}

    /**
     * A two element partial 4x4 matrix constructor. Accepts two 4D vectors for the first and second
     * rows and defaults the rest to 0
     * @param row0 the first row of the matrix
     * @param row1 the second row of the matrix
     */
    inline constexpr Mat<T, 4, 4>(const Vec<T, 4>& row0, const Vec<T, 4>& row1) :
        Mat<T, 4, 4>(row0, row1, Mat<T, 4, 4>::Zero(), Mat<T, 4, 4>::Zero())
    {}

    /**
     * A three element partial 4x4 matrix constructor. Accepts three 4D vectors for the first,
     * second, and third rows and defaults the rest to 0
     * @param row0 the first row of the matrix
     * @param row1 the second row of the matrix
     * @param row2 the third row of the matrix
     */
    inline constexpr Mat<T, 4, 4>(const Vec<T, 4>& row0,
                                  const Vec<T, 4>& row1,
                                  const Vec<T, 4>& row2) :
        Mat<T, 4, 4>(row0, row1, row2, Vec<T, 4>::Zero())
    {}

    /**
     * A scalar 4x4 matrix constructor. Sets all diagonal elements to the provided scalar value
     * @param value the scalar value to set the identity to
     */
    inline constexpr Mat<T, 4, 4>(T value) :
        Mat<T, 4, 4>({value, 0, 0, 0}, {0, value, 0, 0}, {0, 0, value, 0}, {0, 0, 0, value})
    {}

    /**
     * Creates a 4x4 matrix from a 3x3 matrix. Sets the last row and last column of the matrix to
     * their identity
     * @param mat the 3x3 matrix to convert from
     */
    inline constexpr Mat<T, 4, 4>(const Mat<T, 3, 3>& mat) :
        Mat<T, 4, 4>({mat[0], 0.f}, {mat[1], 0.f}, {mat[2], 0.f}, {0.f, 0.f, 0.f, 1.f})
    {}

    /**
     * Creates a 4x4 matrix from a 2x2 matrix. Sets the last two rows to the identity
     * @param mat the 2x2 matrix to construct the matrix from
     */
    inline constexpr Mat<T, 4, 4>(const Mat<T, 2, 2>& mat) :
        Mat<T, 4, 4>({mat[0], 0.f, 0.f},
                     {mat[1], 0.f, 0.f},
                     {0.f, 0.f, 1.f, 0.f},
                     {0.f, 0.f, 0.f, 1.f})
    {}

    // Operator overloads

    /**
     * The 4x4 matrix subscript operator
     * @param index the index of the row to be accessed
     * @return a const reference to the vector representing the requested row
     */
    inline constexpr Vec<T, 4> operator[](unsigned int index) const
    {
        return values[index];
    }

    /**
     * The 4x4 matrix subscript operator
     * @param index the index of the row to be accessed
     * @return a reference to the vector representing the requested row
     */
    inline constexpr Vec<T, 4>& operator[](unsigned int index)
    {
        return values[index];
    }

    /**
     * The equality operator. Checks two 4x4 matrices for equality
     * @param rhs the matrix to compare with
     * @return a boolean representing whether the matrices are equal
     */
    inline constexpr bool operator==(const Mat<T, 4, 4>& rhs) const
    {
        return values[0] == rhs[0] && values[1] == rhs[1] && values[2] == rhs[2] &&
               values[3] == rhs[3];
    }

    /**
     * The inequality operator. Checks two 4x4 matrices for non-equality
     * @param rhs the matrix to compare with
     * @return a boolean representing whether the matrices are not equal
     */
    inline constexpr bool operator!=(const Mat<T, 4, 4>& rhs) const
    {
        return values[0] != rhs[0] || values[1] != rhs[1] || values[2] != rhs[2] ||
               values[3] != rhs[3];
    }

    /**
     * The 2x2 matrix assignment operator. Sets the values of the matrix to those of the provided
     * 2x2 matrix
     * @param other the 2x2 matrix to convert from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4> operator=(const Mat<T, 2, 2>& other)
    {
        values[0] = other[0];
        values[1] = other[1];
        values[2] = {0.f, 0.f, 1.f, 0.f};
        values[3] = {0.f, 0.f, 0.f, 1.f};
        return *this;
    }

    /**
     * The 3x3 matrix assignment operator. Sets the values of the matrix to those of the provided
     * 3x3 matrix
     * @param other the 3x3 matrix to convert from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4> operator=(const Mat<T, 3, 3>& other)
    {
        values[0] = other[0];
        values[1] = other[1];
        values[2] = other[2];
        values[3] = {0.f, 0.f, 0.f, 1.f};
        return *this;
    }

    /**
     * The addition operator. Adds the values of the provided matrix to those of the current matrix
     * @param rhs the matrix to add values from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4>& operator+=(const Mat<T, 4, 4>& rhs)
    {
        values[0] += rhs[0];
        values[1] += rhs[1];
        values[2] += rhs[2];
        values[3] += rhs[3];
        return *this;
    }

    /**
     * The subtraction operator. Subtracts the values of the provided matrix to those of the current
     * matrix
     * @param rhs the matrix to subtract values from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4>& operator-=(const Mat<T, 4, 4>& rhs)
    {
        values[0] -= rhs[0];
        values[1] -= rhs[1];
        values[2] -= rhs[2];
        values[3] -= rhs[3];
        return *this;
    }

    /**
     * The multiplication operator. Multiplies the values of the provided matrix to those of the
     * current matrix
     * @param rhs the matrix to multiply values from
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4>& operator*=(const Mat<T, 4, 4>& rhs)
    {
        *this = Multiply(*this, rhs);
        return *this;
    }

    /**
     * The scalar multiplication operator. Multiplies the matrix by a scalar value
     * @param scalar the scalar value to multiply by
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4>& operator*=(T scalar)
    {
        *this = Multiply(*this, scalar);
        return *this;
    }

    /**
     * The division operator. Divides the matrix by another matrix
     * @param rhs the matrix to divide by
     * @return a reference to the current matrix
     */
    inline constexpr Mat<T, 4, 4>& operator/=(const Mat<T, 4, 4>& rhs)
    {
        *this = Divide(*this, rhs);
        return *this;
    }

    /**
     * The negation operator. Negates every row of the matrix
     * @return a new matrix with the result of the negation
     */
    inline constexpr Mat<T, 4, 4>& operator-()
    {
        values[0] = -values[0];
        values[1] = -values[1];
        values[2] = -values[2];
        values[3] = -values[3];

        return *this;
    }

    Vec<T, 4> values[4] {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
};

/**
 * The binary 4x4 matrix addition operator. Adds two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix addition
 */
template<typename T>
inline constexpr Mat<T, 4, 4> operator+(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
{
    return Mat<T, 4, 4>::Add(lhs, rhs);
}

/**
 * The binary 4x4 matrix subtraction operator. Subtracts two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix subtraction
 */
template<typename T>
inline constexpr Mat<T, 4, 4> operator-(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
{
    return Mat<T, 4, 4>::Subtract(lhs, rhs);
}

/**
 * The binary 4x4 matrix multiplication operator. Multiplies two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix multiplication
 */
template<typename T>
inline constexpr Mat<T, 4, 4> operator*(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
{
    return Mat<T, 4, 4>::Multiply(lhs, rhs);
}

/**
 * A binary 4x4 matrix to scalar multiplication operator. Multiples a matrix by a scalar and returns
 * the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix being multiplied
 * @param scalar the scalar being used to multiply the matrix by
 * @return a new matrix with the result of the scalar operation
 */
template<typename T>
inline constexpr Mat<T, 4, 4> operator*(const Mat<T, 4, 4>& lhs, T scalar)
{
    return Mat<T, 4, 4>::Multiply(lhs, scalar);
}

/**
 * A binary 4x4 matrix to vector multiplication operator. Multiplies a matrix by a vector and
 * returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix being multiplied
 * @param vector the vector the matrix is being multiplied by
 * @return a 4D vector with the results of the multiplication
 */
template<typename T>
inline constexpr Vec<T, 4> operator*(const Mat<T, 4, 4>& lhs, const Vec<T, 4>& vector)
{
    return Mat<T, 4, 4>::Multiply(lhs, vector);
}

/**
 * The binary 4x4 matrix division operator. Divides two matrices and returns the result
 * @tparam T the type of numeric value stored by the matrix
 * @param lhs the matrix on the left hand side of the operation
 * @param rhs the matrix on the right side of the operation
 * @return a new matrix with the result of the matrix division
 */
template<typename T>
inline constexpr Mat<T, 4, 4> operator/(const Mat<T, 4, 4>& lhs, const Mat<T, 4, 4>& rhs)
{
    return Mat<T, 4, 4>::Divide(lhs, rhs);
}

/**
 * The 4x4 matrix negation operator. Negates the values of a 4x4 matrix
 * @tparam T the type of numeric value stored by the matrix
 * @param matrix the matrix to be negated
 * @return a new matrix with the negated values
 */
template<typename T>
inline constexpr Mat<T, 4, 4> operator-(const Mat<T, 4, 4>& matrix)
{
    return {-matrix[0], -matrix[1], -matrix[2], -matrix[3]};
}
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MAT4X4_H
