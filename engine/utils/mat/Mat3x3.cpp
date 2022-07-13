//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Mat3x3.h"

namespace Siege
{

// Static Variables

const Mat3x3 Mat3x3::Identity = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};

const Mat3x3 Mat3x3::Zero = {};

// Static functions

Mat3x3 Mat3x3::Divide(const Mat3x3& lhs, const Mat3x3& rhs)
{
    return (lhs.Inverse() * rhs);
}

Mat3x3 Mat3x3::Add(const Mat3x3& lhs, const Mat3x3& rhs)
{
    Mat3x3 newMat {lhs};
    newMat.Add(rhs);
    return newMat;
}

Mat3x3 Mat3x3::Multiply(const Mat3x3& lhs, const Mat3x3& rhs)
{
    Mat3x3 newMat {lhs};
    newMat.Multiply(rhs);
    return newMat;
}

Mat3x3 Mat3x3::Subtract(const Mat3x3& lhs, const Mat3x3& rhs)
{
    Mat3x3 newMat {lhs};
    newMat.Subtract(rhs);
    return newMat;
}

Mat3x3 Mat3x3::Multiply(const Mat3x3& lhs, const float& rhs)
{
    Mat3x3 newMat {lhs};
    newMat.Multiply(rhs);
    return newMat;
}

Vec3 Mat3x3::Multiply(const Mat3x3& lhs, const Vec3& rhs)
{
    return lhs.Multiply(rhs);
}

float Mat3x3::Determinant(const Mat3x3& mat)
{
    return mat.Determinant();
}

Mat3x3 Mat3x3::Inverse(const Mat3x3& mat)
{
    return mat.Inverse();
}

// Unary Operators

float& Mat3x3::operator[](const size_t& index)
{
    assert(index < 9 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

Mat3x3& Mat3x3::operator+=(const Mat3x3& other)
{
    Add(other);
    return *this;
}

Mat3x3& Mat3x3::operator-=(const Mat3x3& other)
{
    Subtract(other);
    return *this;
}

Mat3x3& Mat3x3::operator*=(const float& scalar)
{
    Multiply(scalar);
    return *this;
}

Mat3x3& Mat3x3::operator*=(const Mat3x3& other)
{
    Multiply(other);
    return *this;
}

Mat3x3& Mat3x3::operator/=(const Mat3x3& other)
{
    Divide(other);
    return *this;
}

Mat3x3 Mat3x3::operator-()
{
    return {{-values[0], -values[1], -values[2]},
            {-values[3], -values[4], -values[5]},
            {-values[6], -values[7], -values[8]}};
}

bool Mat3x3::operator==(const Mat3x3& other)
{
    return MAT3_LOGIC_OP(==, &&, &&, MAT3_BODY_FUNC);
}

bool Mat3x3::operator!=(const Mat3x3& other)
{
    return MAT3_LOGIC_OP(!=, ||, ||, MAT3_BODY_FUNC);
}

// Functions

void Mat3x3::Add(const Mat3x3& other)
{
    // To add two matrices, we simply add each element from matrix A to the corresponding value in
    // matrix B. i.e: matA[0] += matB[0] ...

    MAT3_MATH_OP(+=, MAT3_BODY_FUNC);
}

void Mat3x3::Subtract(const Mat3x3& other)
{
    // To subtract two matrices, we simply subtract each element from matrix A to the corresponding
    // value in matrix B. i.e: matA[0] -= matB[0] ...

    MAT3_MATH_OP(-=, MAT3_BODY_FUNC);
}

void Mat3x3::Multiply(const float& scalar)
{
    // Multiplying a matrix by a scalar involves multiplying every element in the matrix with the
    // given scalar. i.e: mat[0] *= scalar

    MAT3_MATH_OP(*=, MAT3_BODY_SCALAR_FUNC);
}

void Mat3x3::Multiply(const Mat3x3& matrix)
{
    // Rows of lhs matrix

    float m0x0, m0y0, m0z0, // Row 0
        m0x1, m0y1, m0z1, // Row 1
        m0x2, m0y2, m0z2; // Row 2

    GET_ROW3(m0x0, m0y0, m0z0, 0)
    GET_ROW3(m0x1, m0y1, m0z1, 1)
    GET_ROW3(m0x2, m0y2, m0z2, 2)

    // Rows of rhs matrix

    float m1cx0, m1cy0, m1cz0, // Column 0
        m1cx1, m1cy1, m1cz1, // Column 1
        m1cx2, m1cy2, m1cz2; // Column 2

    GET_COLUMN3(m1cx0, m1cy0, m1cz0, matrix, 0)
    GET_COLUMN3(m1cx1, m1cy1, m1cz1, matrix, 1)
    GET_COLUMN3(m1cx2, m1cy2, m1cz2, matrix, 2)

    // Matrix multiplication is different to other matrix operations. Instead of multiplying each
    // row by its corresponding parts, we evaluate each element of each row and get the dot product
    // of the corresponding column and the row.
    //
    // For example:
    //
    // │1, 2, 3│   │2, 3, 4│   │(1*2) + (2*6) + (3*6), (1*3) + (2*4) + (3*3), (1*4) + (2*7) + (3*5)│
    // │4, 5, 6│ * │6, 4, 7│ = │(4*2) + (5*6) + (6*6), (4*3) + (5*4) + (6*3), (4*4) + (5*7) + (6*5)│
    // │7, 8, 9│   │6, 3, 5│   │(7*2) + (8*6) + (9*6), (7*3) + (8*4) + (9*3), (7*4) + (8*7) + (9*5)│
    //
    //   │32,  20, 33 │
    // = │74,  50, 81 │
    //   │116, 80, 129│

    values[0] = DOT3(m0x0, m0y0, m0z0, m1cx0, m1cy0, m1cz0);
    values[1] = DOT3(m0x0, m0y0, m0z0, m1cx1, m1cy1, m1cz1);
    values[2] = DOT3(m0x0, m0y0, m0z0, m1cx2, m1cy2, m1cz2);

    values[3] = DOT3(m0x1, m0y1, m0z1, m1cx0, m1cy0, m1cz0);
    values[4] = DOT3(m0x1, m0y1, m0z1, m1cx1, m1cy1, m1cz1);
    values[5] = DOT3(m0x1, m0y1, m0z1, m1cx2, m1cy2, m1cz2);

    values[6] = DOT3(m0x2, m0y2, m0z2, m1cx0, m1cy0, m1cz0);
    values[7] = DOT3(m0x2, m0y2, m0z2, m1cx1, m1cy1, m1cz1);
    values[8] = DOT3(m0x2, m0y2, m0z2, m1cx2, m1cy2, m1cz2);
}

Vec3 Mat3x3::Multiply(const Vec3& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y) + (values[2] * vector.z),
            (values[3] * vector.x) + (values[4] * vector.y) + (values[5] * vector.z),
            (values[6] * vector.x) + (values[7] * vector.y) + (values[8] * vector.z)};
}

void Mat3x3::Divide(const Mat3x3& other)
{
    *this = Mat3x3::Divide(*this, other);
}

float Mat3x3::Determinant() const
{
    // Matrix determinants are calculated by getting the dot product of the diagonals of 2x2 a
    // 2x2 matrix. Since a 3x3 matrix is larger than 2x2, we need to get the determinants of each
    // combination of 2x2 matrices in the matrix.
    //
    // To find these, we start at the top row and for each value, eliminate each value on their
    // x and y axes. There should be four elements left. We use these as the 2x2 matrix from which
    // the determinant should be calculated.
    //
    // We then multiply the determinant by the element in the top row which we used to find the 2x2
    // matrix.
    //
    // Finally, we evaluate each determinant by a chequer-board pattern of addition and subtraction,
    // starting from positive. I.e: + det(a) - det(b) + det(c)
    //
    //                 │5, 2, 3│
    //                 │2, 8, 4│         │8, 4│         │2, 4│         │2, 8│
    // For example: det│3, 5, 4│ = (5)det│5, 4│ - (2)det│3, 4│ + (3)det│3, 5│

    return values[0] * (values[4] * values[8] - values[5] * values[7]) -
           values[1] * (values[3] * values[8] - values[6] * values[5]) +
           values[2] * (values[3] * values[7] - values[6] * values[4]);
}

Mat3x3 Mat3x3::Inverse() const
{
    // The inverse of a 3x3 matrix is 1/det(matrix) multiplied by the determinants of all possible
    // 2x2 combinations in the matrix.
    //
    //                 │a, b, c│
    //                 │d, e, f│
    // Given a matrix: │g, h, i│
    //                             │     │e, i│      │b, c│      │b, c│  │
    //                             │  det│h, f│, -det│h, i│,  det│e, f│  │
    // Inverse(mat) = 1/det(mat) * │                                     │
    //                             │     │d, f│      │a, c│      │a, c│  │
    //                             │ -det│g, i│,  det│g, i│, -det│d, f│  │
    //                             │                                     │
    //                             │     │d, e│      │a, b│     │a, b│   │
    //                             │  det│g, h│, -det│g, h│, det│d, e│   │
    //

    float oneOverDeterminant = 1.f / Determinant();

    return {+(values[4] * values[8] - values[5] * values[7]) * oneOverDeterminant,
            -(values[1] * values[8] - values[2] * values[7]) * oneOverDeterminant,
            +(values[1] * values[5] - values[2] * values[4]) * oneOverDeterminant,
            -(values[3] * values[8] - values[5] * values[6]) * oneOverDeterminant,
            +(values[0] * values[8] - values[2] * values[6]) * oneOverDeterminant,
            -(values[0] * values[5] - values[2] * values[3]) * oneOverDeterminant,
            +(values[3] * values[7] - values[4] * values[6]) * oneOverDeterminant,
            -(values[0] * values[7] - values[1] * values[6]) * oneOverDeterminant,
            +(values[0] * values[4] - values[1] * values[3]) * oneOverDeterminant};
}

Mat3x3 Mat3x3::Transpose() const
{
    // A transpose is a matrix where all rows have been converted to columns and all columns have
    // been moved to rows.
    //                               │a, b, c│    │a, d, g│
    //                               │d, e, f│    │b, e, h│
    // For example, the transpose of │g, h, i│ is │c, f, i│

    return {values[0],
            values[3],
            values[6],
            values[1],
            values[4],
            values[7],
            values[2],
            values[5],
            values[8]};
}

const float& Mat3x3::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(3 * rowIndex) + colIndex];
}

Mat3x3 operator+(const Mat3x3& lhs, const Mat3x3& rhs)
{
    return Mat3x3::Add(lhs, rhs);
}

Mat3x3 operator-(const Mat3x3& lhs, const Mat3x3& rhs)
{
    return Mat3x3::Subtract(lhs, rhs);
}

Mat3x3 operator*(const Mat3x3& lhs, const float& scalar)
{
    return Mat3x3::Multiply(lhs, scalar);
}

Mat3x3 operator*(const Mat3x3& lhs, const Mat3x3& rhs)
{
    return Mat3x3::Multiply(lhs, rhs);
}

const Vec3 operator*(const Mat3x3& lhs, const Siege::Vec3& rhs)
{
    return Mat3x3::Multiply(lhs, rhs);
}

Mat3x3 operator/(const Mat3x3& lhs, const Mat3x3& rhs)
{
    return Mat3x3::Divide(lhs, rhs);
}
} // namespace Siege
