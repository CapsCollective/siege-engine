//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT3_H
#define SIEGE_ENGINE_MAT3_H

#include <cassert>

#include "../vec/Vec3.h"
#include "Macros.h"

namespace Siege
{
/**
 * A class representing a 3x3 matrix of float values.
 */
struct Mat3
{
    // 'Structors

    /**
     * A base constructor for the Mat3x3 class.
     * @param vx0 the x value of the first row matrix.
     * @param vy0 the y value of the first row matrix.
     * @param vz0 the z value of the first row matrix.
     * @param vx1 the x value of the second row matrix.
     * @param vy1 the y value of the second row matrix.
     * @param vz1 the z value of the second row matrix.
     * @param vx2 the x value of the third row matrix.
     * @param vy2 the y value of the third row matrix.
     * @param vz2 the z value of the third row matrix.
     */
    Mat3(const float& vx0 = 0.f,
         const float& vy0 = 0.f,
         const float& vz0 = 0.f,
         const float& vx1 = 0.f,
         const float& vy1 = 0.f,
         const float& vz1 = 0.f,
         const float& vx2 = 0.f,
         const float& vy2 = 0.f,
         const float& vz2 = 0.f) :
        values {vx0, vy0, vz0, vx1, vy1, vz1, vx2, vy2, vz2}
    {}

    /**
     * The Mat3x3 vector constructor.
     * @param vec0 a Vec3 containing the first row of the matrix.
     * @param vec1 a Vec3 containing the second row of the matrix.
     * @param vec2 a Vec3 containing the third row of the matrix.
     */
    Mat3(const Vec3& vec0, const Vec3& vec1, const Vec3& vec2) :
        Mat3(vec0.x, vec0.y, vec0.z, vec1.x, vec1.y, vec1.z, vec2.x, vec2.y, vec2.z)
    {}

    // Statics

    /**
     * A pre-configured matrix set to contain identity values (a 1 for every diagonal).
     */
    static const Mat3 Identity;

    /**
     * A pre-configured empty matrix.
     */
    static const Mat3 Zero;

    /**
     * Returns a value stored within a logical index.
     * @param rowIndex the row index (0 - 2).
     * @param colIndex the column index (0 - 2).
     * @return the float value stored in the given indices.
     */
    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    /**
     * Divides two 3x3 matrices.
     * @param lhs the left hand side matrix to be divided.
     * @param rhs the right hand side matrix to be divided by.
     * @return a matrix containing the division result.
     */
    static Mat3 Divide(const Mat3& lhs, const Mat3& rhs);

    /**
     * Adds two 3x3 matrices.
     * @param lhs the left hand matrix to be added to.
     * @param rhs the right hand matrix to be added by.
     * @return a new matrix containing the addition result.
     */
    static Mat3 Add(const Mat3& lhs, const Mat3& rhs);

    /**
     * Multiplies a 3x3 matrix by another 3x3 matrix.
     * @param lhs the left hand side matrix to be multiplied.
     * @param rhs the right hand side matrix to be multiplied by.
     * @return the product of the two matrices.
     */
    static Mat3 Multiply(const Mat3& lhs, const Mat3& rhs);

    /**
     * Subtracts two 3x3 matrices.
     * @param lhs the left hand matrix to be subtracted to.
     * @param rhs the right hand matrix to be subtracted by.
     * @return a new matrix containing the subtraction result.
     */
    static Mat3 Subtract(const Mat3& lhs, const Mat3& rhs);

    /**
     * Multiples a 3x3 matrix by a scalar value.
     * @param lhs the matrix to be scaled.
     * @param scalar the scalar to be used on the matrix.
     * @return a new matrix containing the scalar result.
     */
    static Mat3 Multiply(const Mat3& lhs, const float& rhs);

    /**
     * Multiplies a 3x3 matrix by a three dimensional vector.
     * @param lhs the matrix to be multiplied by.
     * @param rhs the vector to be multiplied.
     * @return a new vector containing the product of the vector and the matrix.
     */
    static Vec3 Multiply(const Mat3& lhs, const Vec3& rhs);

    /**
     * Computes the determinant of the matrix.
     * @param mat the matrix to to be evaluated.
     * @return the determinant of the matrix.
     */
    static float Determinant(const Mat3& mat);

    /**
     * Computes the inverse of the matrix.
     * @param mat the matrix to be evaluated.
     * @return a new matrix with the result of the matrix inversion.
     */
    static Mat3 Inverse(const Mat3& mat);

    // Operators

    /**
     * A const subscript operator for indexing into the matrix.
     * @param index the index location of the float value being searched.
     * @return the float value of the given index.
     */
    constexpr float const& operator[](const size_t& index) const
    {
        assert(index < 9 &&
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
     * Adds another matrix to the current matrix.
     * @param other the matrix to add.
     * @return A reference to the new evaluated matrix.
     */
    Mat3& operator+=(const Mat3& other);

    /**
     * Subtracts another matrix to the current matrix.
     * @param other the matrix to subtract.
     * @return A reference to the new evaluated matrix.
     */
    Mat3& operator-=(const Mat3& other);

    /**
     * Multiplies the matrix with a scalar.
     * @param scalar the scalar to multiply by.
     * @return A reference to the new evaluated matrix.
     */
    Mat3& operator*=(const float& scalar);

    /**
     * Multiplies another matrix to the current matrix.
     * @param other the matrix to multiply.
     * @return A reference to the new evaluated matrix.
     */
    Mat3& operator*=(const Mat3& other);

    /**
     * Divides another matrix to the current matrix.
     * @param other the matrix to divide.
     * @return A reference to the new evaluated matrix.
     */
    Mat3& operator/=(const Mat3& other);

    /**
     * Negates all values in the matrix.
     * @return a new matrix with the negated values.
     */
    Mat3 operator-();

    /**
     * An equality operator.
     * @param other the second matrix to be evaluated.
     * @return a boolean value representing whether the matrices are equal.
     */
    bool operator==(const Mat3& other);

    /**
     * A negative equality operator.
     * @param other the matrix to check for equality.
     * @return a boolean value representing if the matrices are unequal.
     */
    bool operator!=(const Mat3& other);

    // Functions

    /**
     * Adds a matrix to the current matrix.
     * @param other the matrix to be added.
     */
    void Add(const Mat3& other);

    /**
     * Subtracts a matrix to the current matrix.
     * @param other the matrix to be subtracted.
     */
    void Subtract(const Mat3& other);

    /**
     * Multiplies the matrix by a scalar.
     * @param scalar the scalar to be multiplied.
     */
    void Multiply(const float& scalar);

    /**
     * Multiplies the matrix by another matrix.
     * @param other the matrix to be multiplied.
     */
    void Multiply(const Mat3& matrix);

    /**
     * Multiplies a vector by the matrix.
     * @param vector the vector to be multiplied.
     * @return A new vector representing the product of the matrix and the vector.
     */
    Vec3 Multiply(const Vec3& vector) const;

    /**
     * Divides the matrix by another matrix.
     * @param other the matrix to be divided.
     */
    void Divide(const Mat3& other);

    /**
     * Returns the determinant of the matrix.
     * @return the matrix's determinant.
     */
    float Determinant() const;

    /**
     * Evaluates the matrix's inverse and returns a new inverse value.
     * @return a new matrix representing the matrix's inverse.
     */
    Mat3 Inverse() const;

    /**
     * Returns the matrix's transpose.
     * @return a new matrix representing the matrix's transpose.
     */
    Mat3 Transpose() const;

    float values[9];
};

// Binary operators
/**
 * Adds two 3x3 matrices.
 * @param lhs the left hand matrix to be added to.
 * @param rhs the right hand matrix to be added by.
 * @return a new matrix containing the addition result.
 */
Mat3 operator+(const Mat3& lhs, const Mat3& rhs);

/**
 * Subtracts two 3x3 matrices.
 * @param lhs the left hand matrix to be subtracted to.
 * @param rhs the right hand matrix to be subtracted by.
 * @return a new matrix containing the subtraction result.
 */
Mat3 operator-(const Mat3& lhs, const Mat3& rhs);

/**
 * Multiples a 3x3 matrix by a scalar value.
 * @param lhs the matrix to be scaled.
 * @param scalar the scalar to be used on the matrix.
 * @return a new matrix containing the scalar result.
 */
Mat3 operator*(const Mat3& lhs, const float& scalar);

/**
 * Multiplies a 3x3 matrix by another 3x3 matrix.
 * @param lhs the left hand side matrix to be multiplied.
 * @param rhs the right hand side matrix to be multiplied by.
 * @return the product of the two matrices.
 */
Mat3 operator*(const Mat3& lhs, const Mat3& rhs);

/**
 * Multiplies a matrix by a three dimensional vector.
 * @param lhs the matrix to be multiplied by.
 * @param rhs the vector to be multiplied.
 * @return a new matrix containing the product of the vector and the matrix.
 */
const Vec3 operator*(const Mat3& lhs, const Vec3& rhs);

/**
 * Divides two 3x3 matrices.
 * @param lhs the left hand side matrix to be divided.
 * @param rhs the right hand side matrix to be divided by.
 * @return a matrix containing the division result.
 */
Mat3 operator/(const Mat3& lhs, const Mat3& rhs);
} // namespace Siege

#endif // SIEGE_ENGINE_MAT3_H
