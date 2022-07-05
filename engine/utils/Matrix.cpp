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
const Matrix3 Matrix3::Identity = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};

const Matrix3 Matrix3::Zero = {};

Matrix3 Matrix3::Divide(const Matrix3& lhs, const Matrix3& rhs)
{
    return (lhs.Inverse() * rhs);
}

Matrix3 Matrix3::Add(const Matrix3& lhs, const Matrix3& rhs)
{
    Matrix3 newMat {lhs};
    newMat.Add(rhs);
    return newMat;
}

Matrix3 Matrix3::Multiply(const Matrix3& lhs, const Matrix3& rhs)
{
    Matrix3 newMat {lhs};
    newMat.Multiply(rhs);
    return newMat;
}

Matrix3 Matrix3::Subtract(const Matrix3& lhs, const Matrix3& rhs)
{
    Matrix3 newMat {lhs};
    newMat.Subtract(rhs);
    return newMat;
}

Matrix3 Matrix3::MultiplyScalar(const Matrix3& lhs, const float& rhs)
{
    Matrix3 newMat {lhs};
    newMat.MultiplyScalar(rhs);
    return newMat;
}

Vec3 Matrix3::Multiply(const Matrix3& lhs, const Vec3& rhs)
{
    return lhs.Multiply(rhs);
}

float Matrix3::Determinant(const Matrix3& mat)
{
    return mat.Determinant();
}

Matrix3 Matrix3::Inverse(const Matrix3& mat)
{
    return mat.Inverse();
}

const float& Matrix3::operator[](const size_t& index) const
{
    assert(index < 9 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

float& Matrix3::operator[](const size_t& index)
{
    assert(index < 9 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

Matrix3& Matrix3::operator+=(const Matrix3& other)
{
    Add(other);
    return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& other)
{
    Subtract(other);
    return *this;
}

Matrix3& Matrix3::operator*=(const float& scalar)
{
    MultiplyScalar(scalar);
    return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& other)
{
    Multiply(other);
    return *this;
}

Matrix3& Matrix3::operator/=(const Matrix3& other)
{
    Divide(other);
    return *this;
}

Matrix3 Matrix3::operator-()
{
    return {{-values[0], -values[1], -values[2]},
            {-values[3], -values[4], -values[5]},
            {-values[6], -values[7], -values[8]}};
}

bool Matrix3::operator==(const Matrix3& other)
{
    return MAT3_LOGIC_OP(==, &&, &&, MAT3_LOGIC_OPERATOR_BODY_VEC3)
}

bool Matrix3::operator!=(const Matrix3& other)
{
    return MAT3_LOGIC_OP(!=, ||, ||, MAT3_LOGIC_OPERATOR_BODY_VEC3)
}

void Matrix3::Add(const Matrix3& other)
{
    MAT3_MATH_OP(+=, MAT3_MATH_OPERATOR_BODY_VEC3)
}

void Matrix3::Subtract(const Matrix3& other)
{
    MAT3_MATH_OP(-=, MAT3_MATH_OPERATOR_BODY_VEC3)
}

void Matrix3::MultiplyScalar(const float& scalar)
{
    MAT3_MATH_OP(*=, MAT3_MATH_SCALAR_OPERATOR_BODY_VEC3)
}

void Matrix3::Multiply(const Matrix3& matrix)
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

Vec3 Matrix3::Multiply(const Vec3& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y) + (values[2] * vector.z),
            (values[3] * vector.x) + (values[4] * vector.y) + (values[5] * vector.z),
            (values[6] * vector.x) + (values[7] * vector.y) + (values[8] * vector.z)};
}

void Matrix3::Divide(const Matrix3& other)
{
    *this = Matrix3::Divide(*this, other);
}

float Matrix3::Determinant() const
{
    return values[0] * (values[4] * values[8] - values[5] * values[7]) -
           values[1] * (values[3] * values[8] - values[6] * values[5]) +
           values[2] * (values[3] * values[7] - values[6] * values[4]);
}

Matrix3 Matrix3::Inverse() const
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

const float& Matrix3::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(3 * rowIndex) + colIndex];
}

} // namespace Siege::Utils
