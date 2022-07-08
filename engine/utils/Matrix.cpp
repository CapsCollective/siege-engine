//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Matrix.h"

#include <cassert>

namespace Siege::Utils
{

// -------------------------------------- Matrix2x2 -----------------------------------------------

const Matrix2x2 Matrix2x2::Identity = {1.f, 0.f, 0.f, 1.f};
const Matrix2x2 Matrix2x2::Zero = {};

Matrix2x2 Matrix2x2::Add(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    Matrix2x2 newMat {lhs};
    newMat.Add(rhs);
    return newMat;
}

Matrix2x2 Matrix2x2::Subtract(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    Matrix2x2 newMat {lhs};
    newMat.Subtract(rhs);
    return newMat;
}

Matrix2x2 Matrix2x2::MultiplyScalar(const Matrix2x2& lhs, const float& scalar)
{
    Matrix2x2 newMat {lhs};
    newMat.MultiplyScalar(scalar);
    return newMat;
}

Matrix2x2 Matrix2x2::Multiply(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    Matrix2x2 newMat {lhs};
    newMat.Multiply(rhs);
    return newMat;
}

Matrix2x2 Matrix2x2::Divide(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return (lhs.Inverse() * rhs);
}

void Matrix2x2::Divide(const Matrix2x2& other)
{
    *this = Matrix2x2::Divide(*this, other);
}

float Matrix2x2::Determinant(const Matrix2x2& mat)
{
    return mat.Determinant();
}

Matrix2x2 Matrix2x2::Inverse(const Matrix2x2& mat)
{
    return mat.Inverse();
}

Vec2 Matrix2x2::Multiply(const Matrix2x2& lhs, const Vec2& rhs)
{
    return lhs.Multiply(rhs);
}

void Matrix2x2::Multiply(const Matrix2x2& other)
{
    float m0x0, m0y0, // Row 0
        m0x1, m0y1; // Row 1

    GET_ROW2(m0x0, m0y0, 0)
    GET_ROW2(m0x1, m0y1, 1)

    float m1x0, m1y0, // Row 0
        m1x1, m1y1; // Row 1

    GET_COLUMN2(m1x0, m1y0, other, 0)
    GET_COLUMN2(m1x1, m1y1, other, 1)

    values[0] = DOT(m0x0, m0y0, m1x0, m1y0);
    values[1] = DOT(m0x0, m0y0, m1x1, m1y1);
    values[2] = DOT(m0x1, m0y1, m1x0, m1y0);
    values[3] = DOT(m0x1, m0y1, m1x1, m1y1);
}

Vec2 Matrix2x2::Multiply(const Vec2& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y),
            (values[2] * vector.x) + (values[3] * vector.y)};
}

const float& Matrix2x2::operator[](const size_t& index) const
{
    assert(index < 4 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

bool Matrix2x2::operator==(const Matrix2x2& other)
{
    return MAT2_LOGIC_OP(==, &&, &&, MAT2_LOGIC_OPERATOR_BODY);
}

bool Matrix2x2::operator!=(const Matrix2x2& other)
{
    return MAT2_LOGIC_OP(!=, ||, ||, MAT2_LOGIC_OPERATOR_BODY);
}

Matrix2x2& Matrix2x2::operator+=(const Matrix2x2& other)
{
    Add(other);
    return *this;
}

Matrix2x2& Matrix2x2::operator-=(const Matrix2x2& other)
{
    Subtract(other);
    return *this;
}

Matrix2x2& Matrix2x2::operator/=(const Matrix2x2& other)
{
    Divide(other);
    return *this;
}

Matrix2x2& Matrix2x2::operator*=(const float& scalar)
{
    MultiplyScalar(scalar);
    return *this;
}

Matrix2x2& Matrix2x2::operator*=(const Matrix2x2& other)
{
    Multiply(other);
    return *this;
}

Matrix2x2 Matrix2x2::operator-()
{
    return {-values[0], -values[1], -values[2], -values[3]};
}

void Matrix2x2::Add(const Matrix2x2& other)
{
    MAT2_MATH_OP(+=, MAT2_MATH_OPERATOR_BODY)
}
void Matrix2x2::Subtract(const Matrix2x2& other)
{
    MAT2_MATH_OP(-=, MAT2_MATH_OPERATOR_BODY)
}
void Matrix2x2::MultiplyScalar(const float& scalar)
{
    MAT2_MATH_OP(*=, MAT2_MATH_SCALAR_OPERATOR_BODY)
}

const float& Matrix2x2::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(2 * rowIndex) + colIndex];
}

float Matrix2x2::Determinant() const
{
    return (values[0] * values[3]) - (values[1] * values[2]);
}

Matrix2x2 Matrix2x2::Inverse() const
{
    float oneOverDeterminant = 1.f / Determinant();

    return {values[3] * oneOverDeterminant,
            -values[1] * oneOverDeterminant,
            -values[2] * oneOverDeterminant,
            values[0] * oneOverDeterminant};
}

Matrix2x2 Matrix2x2::Transpose() const { return { values[0], values[2], values[1], values[3] }; }

// -------------------------------------- Matrix3x3 -----------------------------------------------

const Matrix3x3 Matrix3x3::Identity = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};

const Matrix3x3 Matrix3x3::Zero = {};

Matrix3x3 Matrix3x3::Divide(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    return (lhs.Inverse() * rhs);
}

Matrix3x3 Matrix3x3::Add(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    Matrix3x3 newMat {lhs};
    newMat.Add(rhs);
    return newMat;
}

Matrix3x3 Matrix3x3::Multiply(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    Matrix3x3 newMat {lhs};
    newMat.Multiply(rhs);
    return newMat;
}

Matrix3x3 Matrix3x3::Subtract(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    Matrix3x3 newMat {lhs};
    newMat.Subtract(rhs);
    return newMat;
}

Matrix3x3 Matrix3x3::MultiplyScalar(const Matrix3x3& lhs, const float& rhs)
{
    Matrix3x3 newMat {lhs};
    newMat.MultiplyScalar(rhs);
    return newMat;
}

Vec3 Matrix3x3::Multiply(const Matrix3x3& lhs, const Vec3& rhs)
{
    return lhs.Multiply(rhs);
}

float Matrix3x3::Determinant(const Matrix3x3& mat)
{
    return mat.Determinant();
}

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& mat)
{
    return mat.Inverse();
}

const float& Matrix3x3::operator[](const size_t& index) const
{
    assert(index < 9 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

float& Matrix3x3::operator[](const size_t& index)
{
    assert(index < 9 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& other)
{
    Add(other);
    return *this;
}

Matrix3x3& Matrix3x3::operator-=(const Matrix3x3& other)
{
    Subtract(other);
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(const float& scalar)
{
    MultiplyScalar(scalar);
    return *this;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& other)
{
    Multiply(other);
    return *this;
}

Matrix3x3& Matrix3x3::operator/=(const Matrix3x3& other)
{
    Divide(other);
    return *this;
}

Matrix3x3 Matrix3x3::operator-()
{
    return {{-values[0], -values[1], -values[2]},
            {-values[3], -values[4], -values[5]},
            {-values[6], -values[7], -values[8]}};
}

bool Matrix3x3::operator==(const Matrix3x3& other)
{
    return MAT3_LOGIC_OP(==, &&, &&, MAT3_LOGIC_OPERATOR_BODY_VEC3)
}

bool Matrix3x3::operator!=(const Matrix3x3& other)
{
    return MAT3_LOGIC_OP(!=, ||, ||, MAT3_LOGIC_OPERATOR_BODY_VEC3)
}

void Matrix3x3::Add(const Matrix3x3& other)
{
    MAT3_MATH_OP(+=, MAT3_MATH_OPERATOR_BODY_VEC3)
}

void Matrix3x3::Subtract(const Matrix3x3& other)
{
    MAT3_MATH_OP(-=, MAT3_MATH_OPERATOR_BODY_VEC3)
}

void Matrix3x3::MultiplyScalar(const float& scalar)
{
    MAT3_MATH_OP(*=, MAT3_MATH_SCALAR_OPERATOR_BODY_VEC3)
}

void Matrix3x3::Multiply(const Matrix3x3& matrix)
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

Vec3 Matrix3x3::Multiply(const Vec3& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y) + (values[2] * vector.z),
            (values[3] * vector.x) + (values[4] * vector.y) + (values[5] * vector.z),
            (values[6] * vector.x) + (values[7] * vector.y) + (values[8] * vector.z)};
}

void Matrix3x3::Divide(const Matrix3x3& other)
{
    *this = Matrix3x3::Divide(*this, other);
}

float Matrix3x3::Determinant() const
{
    return values[0] * (values[4] * values[8] - values[5] * values[7]) -
           values[1] * (values[3] * values[8] - values[6] * values[5]) +
           values[2] * (values[3] * values[7] - values[6] * values[4]);
}

Matrix3x3 Matrix3x3::Inverse() const
{
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

Matrix3x3 Matrix3x3::Transpose() const
{
    return {
        values[0], values[3], values[6],
        values[1], values[4], values[7],
        values[2], values[5], values[8]
    };
}

const float& Matrix3x3::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(3 * rowIndex) + colIndex];
}

} // namespace Siege::Utils
