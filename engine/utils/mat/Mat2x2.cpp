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
const Mat2x2 Mat2x2::Identity = {1.f, 0.f, 0.f, 1.f};
const Mat2x2 Mat2x2::Zero = {};

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

Mat2x2 Mat2x2::MultiplyScalar(const Mat2x2& lhs, const float& scalar)
{
    Mat2x2 newMat {lhs};
    newMat.MultiplyScalar(scalar);
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
    MultiplyScalar(scalar);
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

void Mat2x2::Add(const Mat2x2& other)
{
    MAT2_MATH_OP(+=, MAT2_BODY_FUNC);
}
void Mat2x2::Subtract(const Mat2x2& other)
{
    MAT2_MATH_OP(-=, MAT2_BODY_FUNC);
}
void Mat2x2::MultiplyScalar(const float& scalar)
{
    MAT2_MATH_OP(*=, MAT2_BODY_SCALAR_FUNC);
}

const float& Mat2x2::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(2 * rowIndex) + colIndex];
}

float Mat2x2::Determinant() const
{
    return (values[0] * values[3]) - (values[1] * values[2]);
}

Mat2x2 Mat2x2::Inverse() const
{
    float oneOverDeterminant = 1.f / Determinant();

    return {values[3] * oneOverDeterminant,
            -values[1] * oneOverDeterminant,
            -values[2] * oneOverDeterminant,
            values[0] * oneOverDeterminant};
}

Mat2x2 Mat2x2::Transpose() const
{
    return {values[0], values[2], values[1], values[3]};
}

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
    return Mat2x2::MultiplyScalar(lhs, scalar);
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
