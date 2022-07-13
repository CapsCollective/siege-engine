//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT4X4_H
#define SIEGE_ENGINE_MAT4X4_H

#include <cassert>

#include "../vec/Vec4.h"
#include "Macros.h"
#include "Mat3x3.h"

namespace Siege
{
/**
 * A class representing a 4x4 matrix of float values.
 */
struct Mat4x4
{
public:

    // 'Structors

    /**
     * A base constructor for the Mat4x4 class.
     * @param x0 the x value of the first row matrix.
     * @param y0 the y value of the first row matrix.
     * @param z0 the z value of the first row matrix.
     * @param w0 the w value of the first row matrix.
     * @param x1 the x value of the second row matrix.
     * @param y1 the y value of the second row matrix.
     * @param z1 the z value of the second row matrix.
     * @param w1 the w value of the second row matrix.
     * @param x2 the x value of the third row matrix.
     * @param y2 the y value of the third row matrix.
     * @param z2 the z value of the third row matrix.
     * @param w2 the w value of the third row matrix.
     * @param x3 the x value of the fourth row matrix.
     * @param y3 the y value of the fourth row matrix.
     * @param z3 the z value of the fourth row matrix.
     * @param w3 the w value of the fourth row matrix.
     */
    Mat4x4(const float& x0 = 0.f,
           const float& y0 = 0.f,
           const float& z0 = 0.f,
           const float& w0 = 0.f,
           const float& x1 = 0.f,
           const float& y1 = 0.f,
           const float& z1 = 0.f,
           const float& w1 = 0.f,
           const float& x2 = 0.f,
           const float& y2 = 0.f,
           const float& z2 = 0.f,
           const float& w2 = 0.f,
           const float& x3 = 0.f,
           const float& y3 = 0.f,
           const float& z3 = 0.f,
           const float& w3 = 0.f) :
        values {x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3}
    {}

    /**
     * The Mat4x4 vector constructor.
     * @param row0 a Vec4 containing the first row of the matrix.
     * @param row1 a Vec4 containing the second row of the matrix.
     * @param row2 a Vec4 containing the third row of the matrix.
     * @param row3 a Vec4 containing the fourth row of the matrix.
     */
    Mat4x4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3) :
        Mat4x4(row0.x,
               row0.y,
               row0.z,
               row0.w,
               row1.x,
               row1.y,
               row1.z,
               row1.w,
               row2.x,
               row2.y,
               row2.z,
               row2.w,
               row3.x,
               row3.y,
               row3.z,
               row3.w)
    {}

    /**
     * A 4x4 matrix constructor using a 3x3 matrix.
     * @param other a 3x3 matrix used to construct the matrix
     */
    Mat4x4(const Mat3x3& other) :
        Mat4x4({other[0], other[1], other[2], 0.f},
               {other[3], other[4], other[5], 0.f},
               {other[6], other[7], other[8], 0.f},
               {0.f, 0.f, 0.f, 1.f})
    {}

    // Statics

    /**
     * A pre-configured matrix set to contain identity values (a 1 for every diagonal).
     */
    static const Mat4x4 Identity;

    /**
     * A pre-configured empty matrix.
     */
    static const Mat4x4 Zero;

    /**
     * Adds two 4x4 matrices.
     * @param lhs the left hand matrix to be added to.
     * @param rhs the right hand matrix to be added by.
     * @return a new matrix containing the addition result.
     */
    static Mat4x4 Add(const Mat4x4& lhs, const Mat4x4& rhs);

    /**
     * Subtracts two 4x4 matrices.
     * @param lhs the left hand matrix to be subtracted to.
     * @param rhs the right hand matrix to be subtracted by.
     * @return a new matrix containing the subtraction result.
     */
    static Mat4x4 Subtract(const Mat4x4& lhs, const Mat4x4& rhs);

    /**
     * Multiples a 4x4 matrix by a scalar value.
     * @param lhs the matrix to be scaled.
     * @param scalar the scalar to be used on the matrix.
     * @return a new matrix containing the scalar result.
     */
    static Mat4x4 Multiply(const Mat4x4& lhs, const float& scalar);

    /**
     * Multiplies a 4x4 matrix by another 3x3 matrix.
     * @param lhs the left hand side matrix to be multiplied.
     * @param rhs the right hand side matrix to be multiplied by.
     * @return the product of the two matrices.
     */
    static Mat4x4 Multiply(const Mat4x4& lhs, const Mat4x4& rhs);

    /**
     * Multiplies a 4x4 matrix by a four dimensional vector.
     * @param lhs the matrix to be multiplied by.
     * @param rhs the vector to be multiplied.
     * @return a new vector containing the product of the vector and the matrix.
     */
    static Vec4 Multiply(const Mat4x4& lhs, const Vec4& rhs);

    /**
     * Divides two 4x4 matrices.
     * @param lhs the left hand side matrix to be divided.
     * @param rhs the right hand side matrix to be divided by.
     * @return a matrix containing the division result.
     */
    static Mat4x4 Divide(const Mat4x4& lhs, const Mat4x4& rhs);

    /**
     * Computes the determinant of the matrix.
     * @param mat the matrix to to be evaluated.
     * @return the determinant of the matrix.
     */
    static float Determinant(const Mat4x4& matrix);

    /**
     * Computes the inverse of the matrix.
     * @param mat the matrix to be evaluated.
     * @return a new matrix with the result of the matrix inversion.
     */
    static Mat4x4 Inverse(const Mat4x4& matrix);

    /**
     * Returns the transpose of the matrix.
     * @param matrix the matrix who's transpose needs to be found.
     * @return a new matrix representing the transpose of the original matrix.
     */
    static Mat4x4 Transpose(const Mat4x4& matrix);

    // Operators

    /**
     * A const subscript operator for indexing into the matrix.
     * @param index the index location of the float value being searched.
     * @return the float value of the given index.
     */
    constexpr float const& operator[](const size_t& index) const
    {
        assert(index < 16 &&
               "Error: trying to index into matrix with a size that's greater than matrix!");

        return values[index];
    }

    /**
     * A subscript operator for indexing into the matrix.
     * @param index the index location of the float value being searched.
     * @return the float value of the given index.
     */
    float& operator[](const size_t& index);

    /**
     * An equality operator.
     * @param other the second matrix to be evaluated.
     * @return a boolean value representing whether the matrices are equal.
     */
    bool operator==(const Mat4x4& other);

    /**
     * An inequality operator.
     * @param other the matrix to check for equality.
     * @return a boolean value representing if the matrices are unequal.
     */
    bool operator!=(const Mat4x4& other);

    /**
     * Adds another matrix to the current matrix.
     * @param other the matrix to add.
     * @return A reference to the new evaluated matrix.
     */
    Mat4x4& operator+=(const Mat4x4& other);

    /**
     * Subtracts another matrix to the current matrix.
     * @param other the matrix to subtract.
     * @return A reference to the new evaluated matrix.
     */
    Mat4x4& operator-=(const Mat4x4& other);

    /**
     * Multiplies the matrix with a scalar.
     * @param scalar the scalar to multiply by.
     * @return A reference to the new evaluated matrix.
     */
    Mat4x4& operator*=(const float& scalar);

    /**
     * Multiplies another matrix to the current matrix.
     * @param other the matrix to multiply.
     * @return A reference to the new evaluated matrix.
     */
    Mat4x4& operator*=(const Mat4x4& other);

    /**
     * Divides another matrix to the current matrix.
     * @param other the matrix to divide.
     * @return A reference to the new evaluated matrix.
     */
    Mat4x4& operator/=(const Mat4x4& other);

    /**
     * Negates all values in the matrix.
     * @return a new matrix with the negated values.
     */
    Mat4x4 operator-();

    // Functions

    /**
     * Returns a value stored within a logical index.
     * @param rowIndex the row index (0 - 3).
     * @param colIndex the column index (0 - 3).
     * @return the float value stored in the given indices.
     */
    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    /**
     * Adds a matrix to the current matrix.
     * @param other the matrix to be added.
     */
    void Add(const Mat4x4& other);

    /**
     * Subtracts a matrix to the current matrix.
     * @param other the matrix to be subtracted.
     */
    void Subtract(const Mat4x4& other);

    /**
     * Multiplies the matrix by a scalar.
     * @param scalar the scalar to be multiplied.
     */
    void Multiply(const float& scalar);

    /**
     * Multiplies the matrix by another matrix.
     * @param other the matrix to be multiplied.
     */
    void Multiply(const Mat4x4& rhs);

    /**
     * Multiplies a vector by the matrix.
     * @param vector the vector to be multiplied.
     * @return A new vector representing the product of the matrix and the vector.
     */
    Vec4 Multiply(const Vec4& vector) const;

    /**
     * Divides the matrix by another matrix.
     * @param other the matrix to be divided.
     */
    void Divide(const Mat4x4& rhs);

    /**
     * Returns the determinant of the matrix.
     * @return the matrix's determinant.
     */
    float Determinant() const;

    /**
     * Evaluates the matrix's inverse and returns a new inverse value.
     * @return a new matrix representing the matrix's inverse.
     */
    Mat4x4 Inverse() const;

    /**
     * Returns the matrix's transpose.
     * @return a new matrix representing the matrix's transpose.
     */
    Mat4x4 Transpose() const;

    float values[16];
};

// Binary operators

/**
 * Adds two 4x4 matrices.
 * @param lhs the left hand matrix to be added to.
 * @param rhs the right hand matrix to be added by.
 * @return a new matrix containing the addition result.
 */
Mat4x4 operator+(const Mat4x4& lhs, const Mat4x4& rhs);

/**
 * Subtracts two 4x4 matrices.
 * @param lhs the left hand matrix to be subtracted to.
 * @param rhs the right hand matrix to be subtracted by.
 * @return a new matrix containing the subtraction result.
 */
Mat4x4 operator-(const Mat4x4& lhs, const Mat4x4& rhs);

/**
 * Multiples a 4x4 matrix by a scalar value.
 * @param lhs the matrix to be scaled.
 * @param scalar the scalar to be used on the matrix.
 * @return a new matrix containing the scalar result.
 */
Mat4x4 operator*(const Mat4x4& lhs, const float& scalar);

/**
 * Multiplies a 4x4 matrix by another 4x4 matrix.
 * @param lhs the left hand side matrix to be multiplied.
 * @param rhs the right hand side matrix to be multiplied by.
 * @return the product of the two matrices.
 */
Mat4x4 operator*(const Mat4x4& lhs, const Mat4x4& rhs);

/**
 * Multiplies a matrix by a four dimensional vector.
 * @param lhs the matrix to be multiplied by.
 * @param rhs the vector to be multiplied.
 * @return a new matrix containing the product of the vector and the matrix.
 */
Vec4 operator*(const Mat4x4& lhs, const Vec4& rhs);

/**
 * Divides two 4x4 matrices.
 * @param lhs the left hand side matrix to be divided.
 * @param rhs the right hand side matrix to be divided by.
 * @return a matrix containing the division result.
 */
Mat4x4 operator/(const Mat4x4& lhs, const Mat4x4& rhs);
} // namespace Siege

#endif // SIEGE_ENGINE_MAT4X4_H
