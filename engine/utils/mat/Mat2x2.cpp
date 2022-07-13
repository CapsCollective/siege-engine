//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Mat2x2.h"

namespace Siege
{

// Static variables

const Mat2x2 Mat2x2::Identity = {1.f, 0.f, 0.f, 1.f};
const Mat2x2 Mat2x2::Zero = {};

// Static functions

Mat2x2 Mat2x2::Add(const Mat2x2& lhs, const Mat2x2& rhs)
{
    Mat2x2 newMat {lhs};
    newMat.Add(rhs);
    return newMat;
}

Mat2x2 Mat2x2::Subtract(const Mat2x2& lhs, const Mat2x2& rhs)
{
    Mat2x2 newMat {lhs};
    newMat.Subtract(rhs);
    return newMat;
}

Mat2x2 Mat2x2::Multiply(const Mat2x2& lhs, const float& scalar)
{
    Mat2x2 newMat {lhs};
    newMat.Multiply(scalar);
    return newMat;
}

Mat2x2 Mat2x2::Multiply(const Mat2x2& lhs, const Mat2x2& rhs)
{
    Mat2x2 newMat {lhs};
    newMat.Multiply(rhs);
    return newMat;
}

Mat2x2 Mat2x2::Divide(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return (lhs.Inverse() * rhs);
}

void Mat2x2::Divide(const Mat2x2& other)
{
    *this = Mat2x2::Divide(*this, other);
}

float Mat2x2::Determinant(const Mat2x2& mat)
{
    return mat.Determinant();
}

Mat2x2 Mat2x2::Inverse(const Mat2x2& mat)
{
    return mat.Inverse();
}

Vec2 Mat2x2::Multiply(const Mat2x2& lhs, const Vec2& rhs)
{
    return lhs.Multiply(rhs);
}

void Mat2x2::Multiply(const Mat2x2& other)
{
    float m0x0, m0y0, // Row 0
        m0x1, m0y1; // Row 1

    GET_ROW2(m0x0, m0y0, 0)
    GET_ROW2(m0x1, m0y1, 1)

    float m1x0, m1y0, // Row 0
        m1x1, m1y1; // Row 1

    GET_COLUMN2(m1x0, m1y0, other, 0)
    GET_COLUMN2(m1x1, m1y1, other, 1)

    // Matrix multiplication is different to other matrix operations. Instead of multiplying each
    // row by its corresponding parts, we evaluate each element of each row and get the dot product
    // of the corresponding column.
    //              │1, 2│   │2, 5│   │(1 * 2) + (2 * 6), (1 * 5) + (2 * 3)│   │14, 11│
    // For example, │3, 4│ * │6, 3│ = │(3 * 2) + (4 * 2), (3 * 5) + (4 * 3)│ = │14, 27│

    values[0] = DOT(m0x0, m0y0, m1x0, m1y0);
    values[1] = DOT(m0x0, m0y0, m1x1, m1y1);
    values[2] = DOT(m0x1, m0y1, m1x0, m1y0);
    values[3] = DOT(m0x1, m0y1, m1x1, m1y1);
}

Vec2 Mat2x2::Multiply(const Vec2& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y),
            (values[2] * vector.x) + (values[3] * vector.y)};
}

// Unary operators

bool Mat2x2::operator==(const Mat2x2& other)
{
    return MAT2_LOGIC_OP(==, &&, &&, MAT2_BODY_FUNC);
}

bool Mat2x2::operator!=(const Mat2x2& other)
{
    return MAT2_LOGIC_OP(!=, ||, ||, MAT2_BODY_FUNC);
}

Mat2x2& Mat2x2::operator+=(const Mat2x2& other)
{
    Add(other);
    return *this;
}

Mat2x2& Mat2x2::operator-=(const Mat2x2& other)
{
    Subtract(other);
    return *this;
}

Mat2x2& Mat2x2::operator/=(const Mat2x2& other)
{
    Divide(other);
    return *this;
}

Mat2x2& Mat2x2::operator*=(const float& scalar)
{
    Multiply(scalar);
    return *this;
}

Mat2x2& Mat2x2::operator*=(const Mat2x2& other)
{
    Multiply(other);
    return *this;
}

Mat2x2 Mat2x2::operator-()
{
    return {-values[0], -values[1], -values[2], -values[3]};
}

// Functions

void Mat2x2::Add(const Mat2x2& other)
{
    // To add two matrices, we simply add each element from matrix A to the corresponding value in
    // matrix B. i.e: matA[0] += matB[0] ...

    MAT2_MATH_OP(+=, MAT2_BODY_FUNC);
}
void Mat2x2::Subtract(const Mat2x2& other)
{
    // To subtract two matrices, we simply subtract each element from matrix A to the corresponding
    // value in matrix B. i.e: matA[0] -= matB[0] ...
    MAT2_MATH_OP(-=, MAT2_BODY_FUNC);
}
void Mat2x2::Multiply(const float& scalar)
{
    // Multiplying a matrix by a scalar involves multiplying every element in the matrix with the
    // given scalar. i.e: mat[0] *= scalar
    MAT2_MATH_OP(*=, MAT2_BODY_SCALAR_FUNC);
}

const float& Mat2x2::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(2 * rowIndex) + colIndex];
}

float Mat2x2::Determinant() const
{
    // Matrix determinants are calculated by getting the dot product of the diagonals of 2x2 a
    // 2x2 matrix.
    //                              │1, 2│
    // For example, given a matrix: │3, 4│ the determinant would be |(1*4) - (2*3)| = |4-6| = -2

    return (values[0] * values[3]) - (values[1] * values[2]);
}

Mat2x2 Mat2x2::Inverse() const
{
    //                                                         │d b│
    // The inverse of a matrix is defined as 1.0/det(matrix) * │c a│
    // An inverse is a tool for dividing two matrices. The idea is that a matrix * the inverse of
    // the same matrix equals an identity matrix. An identity matrix, for all intents and purposes
    // is the matrix equivalent of 1. A number divided the same number equals 1, whilst a number
    // divided by 1 equals the same number. Therefore, an inverse serves the same purpose in
    // matrices.

    float oneOverDeterminant = 1.f / Determinant();

    return {values[3] * oneOverDeterminant,
            -values[1] * oneOverDeterminant,
            -values[2] * oneOverDeterminant,
            values[0] * oneOverDeterminant};
}

Mat2x2 Mat2x2::Transpose() const
{
    // A transpose is a matrix where all rows have been converted to columns and all columns have
    // been moved to rows.
    //                               │a, b│    │a, c│
    // For example, the transpose of │c, d│ is │b, d│
    return {values[0], values[2], values[1], values[3]};
}

// Binary operators

Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Add(lhs, rhs);
}

Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Subtract(lhs, rhs);
}

Mat2x2 operator*(const Mat2x2& lhs, const float& scalar)
{
    return Mat2x2::Multiply(lhs, scalar);
}

Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Multiply(lhs, rhs);
}

Mat2x2 operator/(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Divide(lhs, rhs);
}

Vec2 operator*(const Mat2x2& lhs, const Vec2& rhs)
{
    return Mat2x2::Multiply(lhs, rhs);
}
} // namespace Siege
