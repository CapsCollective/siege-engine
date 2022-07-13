//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT2X2_H
#define SIEGE_ENGINE_MAT2X2_H

#include <cassert>

#include "../vec/Vec2.h"
#include "Macros.h"

namespace Siege
{

/**
 * A class representing a 2x2 matrix of float values.
 */
struct Mat2x2
{
    // 'Structors

    /**
     * The base constructor of the Mat2x2 matrix class.
     * @param x0 the x value of the first row matrix.
     * @param y0 the y value of the first row matrix.
     * @param x1 the x value of the second row matrix.
     * @param y1 the y value of the second row matrix.
     */
    Mat2x2(const float& x0 = 0, const float& y0 = 0, const float& x1 = 0, const float& y1 = 0) :
        values {x0, y0, x1, y1}
    {}

    /**
     * The Mat2x2 vector constructor.
     * @param row0 a Vec2 containing the first row of the matrix.
     * @param row1 a Vec2 containing the second row of the matrix.
     */
    Mat2x2(const Vec2& row0, const Vec2& row1) : Mat2x2(row0.x, row0.y, row1.x, row1.y) {}

    // Statics

    /**
     * A pre-configured matrix set to contain identity values (a 1 for every diagonal).
     */
    static const Mat2x2 Identity;

    /**
     * A pre-configured empty matrix.
     */
    static const Mat2x2 Zero;

    /**
     * A matrix division function.
     * @param lhs the left hand side matrix to be divided.
     * @param rhs the right hand side matrix to be divided by.
     * @return a matrix containing the division result.
     */
    static Mat2x2 Divide(const Mat2x2& lhs, const Mat2x2& rhs);

    /**
     * Adds two 2x2 matrices.
     * @param lhs the left hand matrix to be added to.
     * @param rhs the right hand matrix to be added by.
     * @return a new matrix containing the addition result.
     */
    static Mat2x2 Add(const Mat2x2& lhs, const Mat2x2& rhs);

    /**
     * Subtracts two matrices.
     * @param lhs the left hand matrix to be subtracted to.
     * @param rhs the right hand matrix to be subtracted by.
     * @return a new matrix containing the subtraction result.
     */
    static Mat2x2 Subtract(const Mat2x2& lhs, const Mat2x2& rhs);

    /**
     * Multiples a matrix by a scalar value.
     * @param lhs the matrix to be scaled.
     * @param scalar the scalar to be used on the matrix.
     * @return a new matrix containing the scalar result.
     */
    static Mat2x2 Multiply(const Mat2x2& lhs, const float& scalar);

    /**
     * Multiplies a 2x2 matrix by another 2x2 matrix.
     * @param lhs the left hand side matrix to be multiplied.
     * @param rhs the right hand side matrix to be multiplied by.
     * @return the product of the two matrices.
     */
    static Mat2x2 Multiply(const Mat2x2& lhs, const Mat2x2& rhs);

    /**
     * Multiplies a matrix by a two dimensional vector.
     * @param lhs the matrix to be multiplied by.
     * @param rhs the vector to be multiplied.
     * @return a new matrix containing the product of the vector and the matrix.
     */
    static Vec2 Multiply(const Mat2x2& lhs, const Vec2& rhs);

    /**
     * Computes the determinant of the matrix.
     * @param mat the matrix to to be evaluated.
     * @return the determinant of the matrix.
     */
    static float Determinant(const Mat2x2& mat);

    /**
     * Computes the inverse of the matrix.
     * @param mat the matrix to be evaluated.
     * @return a new matrix with the result of the matrix inversion.
     */
    static Mat2x2 Inverse(const Mat2x2& mat);

    /**
     * Returns a value stored within a logical index.
     * @param rowIndex the row index (0 - 1).
     * @param colIndex the column index (0 -1).
     * @return the float value stored in the given indices.
     */
    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    // Operators

    /**
     * A subscript operator for indexing into the matrix.
     * @param index the index location of the float value being searched.
     * @return the float value of the given index.
     */
    constexpr float const& operator[](const size_t& index) const
    {
        assert(index < 4 &&
               "Error: trying to index into matrix with a size that's greater than matrix!");

        return values[index];
    }

    /**
     * An equality operator between two 2x2 matrices.
     * @param other the second matrix to be evaluated.
     * @return a boolean value representing whether the matrices are equal.
     */
    bool operator==(const Mat2x2& other);

    /**
     * A negative equality operator.
     * @param other the matrix to check for equality.
     * @return a boolean value representing if the matrices are unequal.
     */
    bool operator!=(const Mat2x2& other);

    /**
     * Adds another matrix to the current matrix.
     * @param other the matrix to add.
     * @return A reference to the new evaluated matrix.
     */
    Mat2x2& operator+=(const Mat2x2& other);

    /**
     * Subtracts another matrix to the current matrix.
     * @param other the matrix to subtract.
     * @return A reference to the new evaluated matrix.
     */
    Mat2x2& operator-=(const Mat2x2& other);

    /**
     * Multiplies another matrix to the current matrix.
     * @param other the matrix to multiply.
     * @return A reference to the new evaluated matrix.
     */
    Mat2x2& operator*=(const Mat2x2& other);

    /**
     * Divides another matrix to the current matrix.
     * @param other the matrix to divide.
     * @return A reference to the new evaluated matrix.
     */
    Mat2x2& operator/=(const Mat2x2& other);

    /**
     * Multiplies the matrix with a scalar.
     * @param scalar the scalar to multiply by.
     * @return A reference to the new evaluated matrix.
     */
    Mat2x2& operator*=(const float& scalar);

    /**
     * Negates all values in the matrix.
     * @return a new matrix with the negated values.
     */
    Mat2x2 operator-();

    // Functions

    /**
     * Adds a matrix to the current matrix.
     * @param other the matrix to be added.
     */
    void Add(const Mat2x2& other);

    /**
     * Subtracts a matrix to the current matrix.
     * @param other the matrix to be subtracted.
     */
    void Subtract(const Mat2x2& other);

    /**
     * Multiplies the matrix by a scalar.
     * @param scalar the scalar to be multiplied.
     */
    void Multiply(const float& scalar);

    /**
     * Multiplies the matrix by another matrix.
     * @param other the matrix to be multiplied.
     */
    void Multiply(const Mat2x2& other);

    /**
     * Multiplies a vector by the matrix.
     * @param vector the vector to be multiplied.
     * @return A new vector representing the product of the matrix and the vector.
     */
    Vec2 Multiply(const Vec2& vector) const;

    /**
     * Divides the matrix by another matrix.
     * @param other the matrix to be divided.
     */
    void Divide(const Mat2x2& other);

    /**
     * Returns the determinant of the matrix.
     * @return the matrix's determinant.
     */
    float Determinant() const;

    /**
     * Evaluates the matrix's inverse and returns a new inverse value.
     * @return a new matrix representing the matrix's inverse.
     */
    Mat2x2 Inverse() const;

    /**
     * Returns the matrix's transpose.
     * @return a new matrix representing the matrix's transpose.
     */
    Mat2x2 Transpose() const;

    float values[4];
};

// Binary operators

/**
 * Adds two 2x2 matrices.
 * @param lhs the left hand matrix to be added to.
 * @param rhs the right hand matrix to be added by.
 * @return a new matrix containing the addition result.
 */
Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs);

/**
 * Subtracts two matrices.
 * @param lhs the left hand matrix to be subtracted to.
 * @param rhs the right hand matrix to be subtracted by.
 * @return a new matrix containing the subtraction result.
 */
Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs);

/**
 * Multiples a matrix by a scalar value.
 * @param lhs the matrix to be scaled.
 * @param scalar the scalar to be used on the matrix.
 * @return a new matrix containing the scalar result.
 */
Mat2x2 operator*(const Mat2x2& lhs, const float& scalar);

/**
 * Multiplies a 2x2 matrix by another 2x2 matrix.
 * @param lhs the left hand side matrix to be multiplied.
 * @param rhs the right hand side matrix to be multiplied by.
 * @return the product of the two matrices.
 */
Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs);

/**
 * A matrix division function.
 * @param lhs the left hand side matrix to be divided.
 * @param rhs the right hand side matrix to be divided by.
 * @return a matrix containing the division result.
 */
Mat2x2 operator/(const Mat2x2& lhs, const Mat2x2& rhs);

/**
 * Multiplies a matrix by a two dimensional vector.
 * @param lhs the matrix to be multiplied by.
 * @param rhs the vector to be multiplied.
 * @return a new matrix containing the product of the vector and the matrix.
 */
Vec2 operator*(const Mat2x2& lhs, const Vec2& rhs);

} // namespace Siege

#endif // SIEGE_ENGINE_MAT2X2_H
