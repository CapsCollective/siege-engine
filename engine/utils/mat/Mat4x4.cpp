//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Mat4x4.h"

namespace Siege
{
const Mat4x4 Mat4x4::Identity = {
    1.f,
    0.f,
    0.f,
    0.f,
    0.f,
    1.f,
    0.f,
    0.f,
    0.f,
    0.f,
    1.f,
    0.f,
    0.f,
    0.f,
    0.f,
    1.f,
};

const Mat4x4 Mat4x4::Zero {};

// static functions

Mat4x4 Mat4x4::Add(const Mat4x4& lhs, const Mat4x4& rhs)
{
    Mat4x4 newMat = lhs;
    newMat.Add(rhs);
    return newMat;
}

Mat4x4 Mat4x4::Subtract(const Mat4x4& lhs, const Mat4x4& rhs)
{
    Mat4x4 newMat = lhs;
    newMat.Subtract(rhs);
    return newMat;
}

Mat4x4 Mat4x4::MultiplyScalar(const Mat4x4& lhs, const float& scalar)
{
    Mat4x4 newMat = lhs;
    newMat.MultiplyScalar(scalar);
    return newMat;
}

Mat4x4 Mat4x4::Multiply(const Mat4x4& lhs, const Mat4x4& rhs)
{
    Mat4x4 newMat = lhs;
    newMat.Multiply(rhs);
    return newMat;
}

Vec4 Mat4x4::Multiply(const Mat4x4& lhs, const Vec4& rhs)
{
    return lhs.Multiply(rhs);
}

Mat4x4 Mat4x4::Divide(const Mat4x4& lhs, const Mat4x4& rhs)
{
    return lhs * rhs.Inverse();
}

float Mat4x4::Determinant(const Mat4x4& matrix)
{
    return matrix.Determinant();
}

Mat4x4 Mat4x4::Inverse(const Mat4x4& matrix)
{
    return matrix.Inverse();
}

// operators

float& Mat4x4::operator[](const size_t& index)
{
    assert(index < 16 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

bool Mat4x4::operator==(const Mat4x4& other)
{
    return MAT4_LOGIC_OP(==, &&, &&, MAT4_BODY_FUNC);
}

bool Mat4x4::operator!=(const Mat4x4& other)
{
    return MAT4_LOGIC_OP(!=, ||, ||, MAT4_BODY_FUNC);
}

Mat4x4& Mat4x4::operator+=(const Mat4x4& other)
{
    Add(other);
    return *this;
}

Mat4x4& Mat4x4::operator-=(const Mat4x4& other)
{
    Subtract(other);
    return *this;
}

Mat4x4& Mat4x4::operator*=(const float& scalar)
{
    MultiplyScalar(scalar);
    return *this;
}

Mat4x4& Mat4x4::operator*=(const Mat4x4& other)
{
    Multiply(other);
    return *this;
}

Mat4x4& Mat4x4::operator/=(const Mat4x4& other)
{
    Divide(other);
    return *this;
}

Mat4x4 Mat4x4::operator-()
{
    return {{-values[0], -values[1], -values[2], -values[3]},
            {-values[4], -values[5], -values[6], -values[7]},
            {-values[8], -values[9], -values[10], -values[11]},
            {-values[12], -values[13], -values[14], -values[15]}};
}

Mat4x4 Mat4x4::Transpose(const Mat4x4& matrix)
{
    return matrix.Transpose();
}

// functions

const float& Mat4x4::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(4 * rowIndex) + colIndex];
}

void Mat4x4::Add(const Mat4x4& other)
{
    MAT4_MATH_OP(+=, MAT4_BODY_FUNC);
}

void Mat4x4::Subtract(const Mat4x4& other)
{
    MAT4_MATH_OP(-=, MAT4_BODY_FUNC);
}

void Mat4x4::MultiplyScalar(const float& scalar)
{
    MAT4_MATH_OP(*=, MAT4_BODY_SCALAR_FUNC);
}

void Mat4x4::Multiply(const Mat4x4& rhs)
{
    // Rows of lhs matrix

    float m0x0, m0y0, m0z0, m0w0, // Row 0
        m0x1, m0y1, m0z1, m0w1, // Row 1
        m0x2, m0y2, m0z2, m0w2, m0x3, m0y3, m0z3, m0w3; // Row 2

    GET_ROW4(m0x0, m0y0, m0z0, m0w0, 0)
    GET_ROW4(m0x1, m0y1, m0z1, m0w1, 1)
    GET_ROW4(m0x2, m0y2, m0z2, m0w2, 2)
    GET_ROW4(m0x3, m0y3, m0z3, m0w3, 3)

    // Rows of rhs matrix

    float m1cx0, m1cy0, m1cz0, m1cw0, // Column 0
        m1cx1, m1cy1, m1cz1, m1cw1, // Column 1
        m1cx2, m1cy2, m1cz2, m1cw2, // Column 2
        m1cx3, m1cy3, m1cz3, m1cw3; // Column 3

    GET_COLUMN4(m1cx0, m1cy0, m1cz0, m1cw0, rhs, 0)
    GET_COLUMN4(m1cx1, m1cy1, m1cz1, m1cw1, rhs, 1)
    GET_COLUMN4(m1cx2, m1cy2, m1cz2, m1cw2, rhs, 2)
    GET_COLUMN4(m1cx3, m1cy3, m1cz3, m1cw3, rhs, 3)

    values[0] = DOT4(m0x0, m0y0, m0z0, m0w0, m1cx0, m1cy0, m1cz0, m1cw0);
    values[1] = DOT4(m0x0, m0y0, m0z0, m0w0, m1cx1, m1cy1, m1cz1, m1cw1);
    values[2] = DOT4(m0x0, m0y0, m0z0, m0w0, m1cx2, m1cy2, m1cz2, m1cw2);
    values[3] = DOT4(m0x0, m0y0, m0z0, m0w0, m1cx3, m1cy3, m1cz3, m1cw3);

    values[4] = DOT4(m0x1, m0y1, m0z1, m0w1, m1cx0, m1cy0, m1cz0, m1cw0);
    values[5] = DOT4(m0x1, m0y1, m0z1, m0w1, m1cx1, m1cy1, m1cz1, m1cw1);
    values[6] = DOT4(m0x1, m0y1, m0z1, m0w1, m1cx2, m1cy2, m1cz2, m1cw2);
    values[7] = DOT4(m0x1, m0y1, m0z1, m0w1, m1cx3, m1cy3, m1cz3, m1cw3);

    values[8] = DOT4(m0x2, m0y2, m0z2, m0w2, m1cx0, m1cy0, m1cz0, m1cw0);
    values[9] = DOT4(m0x2, m0y2, m0z2, m0w2, m1cx1, m1cy1, m1cz1, m1cw1);
    values[10] = DOT4(m0x2, m0y2, m0z2, m0w2, m1cx2, m1cy2, m1cz2, m1cw2);
    values[11] = DOT4(m0x2, m0y2, m0z2, m0w2, m1cx3, m1cy3, m1cz3, m1cw3);

    values[12] = DOT4(m0x3, m0y3, m0z3, m0w3, m1cx0, m1cy0, m1cz0, m1cw0);
    values[13] = DOT4(m0x3, m0y3, m0z3, m0w3, m1cx1, m1cy1, m1cz1, m1cw1);
    values[14] = DOT4(m0x3, m0y3, m0z3, m0w3, m1cx2, m1cy2, m1cz2, m1cw2);
    values[15] = DOT4(m0x3, m0y3, m0z3, m0w3, m1cx3, m1cy3, m1cz3, m1cw3);
}

Vec4 Mat4x4::Multiply(const Vec4& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y) + (values[2] * vector.z) +
                (values[3] * vector.w),
            (values[4] * vector.x) + (values[5] * vector.y) + (values[6] * vector.z) +
                (values[7] * vector.w),
            (values[8] * vector.x) + (values[9] * vector.y) + (values[10] * vector.z) +
                (values[11] * vector.w),
            (values[12] * vector.x) + (values[13] * vector.y) + (values[14] * vector.z) +
                (values[15] * vector.w)};
}

void Mat4x4::Divide(const Mat4x4& rhs)
{
    *this = Mat4x4::Divide(*this, rhs);
}

float Mat4x4::Determinant() const
{
    float SubFactor00 = values[10] * values[15] - values[11] * values[14]; // 1 2 3 4
    float SubFactor01 = values[6] * values[15] - values[7] * values[14]; // 5 6 7 8
    float SubFactor02 = values[6] * values[11] - values[7] * values[10]; // 9 0 1 2
    float SubFactor03 = values[2] * values[15] - values[3] * values[14]; // 3 4 5 6
    float SubFactor04 = values[2] * values[11] - values[3] * values[10];
    float SubFactor05 = values[2] * values[7] - values[3] * values[6];

    Siege::Vec4 coefficient = {
        +(values[5] * SubFactor00 - values[9] * SubFactor01 + values[13] * SubFactor02),
        -(values[1] * SubFactor00 - values[9] * SubFactor03 + values[13] * SubFactor04),
        +(values[1] * SubFactor01 - values[5] * SubFactor03 + values[13] * SubFactor05),
        -(values[1] * SubFactor02 - values[5] * SubFactor04 + values[9] * SubFactor05),
    };

    return values[0] * coefficient.x + values[4] * coefficient.y + values[8] * coefficient.z +
           values[12] * coefficient.w;
}

Mat4x4 Mat4x4::Inverse() const
{
    Vec4 fac0 = {(values[10] * values[15]) - (values[14] * values[11]),
                 (values[10] * values[15]) - (values[14] * values[11]),
                 (values[6] * values[15]) - (values[14] * values[7]),
                 (values[6] * values[11]) - (values[10] * values[7])};

    Vec4 fac1 = {values[9] * values[15] - values[13] * values[11],
                 values[9] * values[15] - values[13] * values[11],
                 values[5] * values[15] - values[13] * values[7],
                 values[5] * values[11] - values[9] * values[7]};

    Vec4 fac2 = {values[9] * values[14] - values[13] * values[10],
                 values[9] * values[14] - values[13] * values[10],
                 values[5] * values[14] - values[13] * values[6],
                 values[5] * values[10] - values[9] * values[6]};

    Vec4 fac3 = {values[8] * values[15] - values[12] * values[11],
                 values[8] * values[15] - values[12] * values[11],
                 values[4] * values[15] - values[12] * values[7],
                 values[4] * values[11] - values[8] * values[7]};

    Vec4 fac4 = {values[8] * values[14] - values[12] * values[10],
                 values[8] * values[14] - values[12] * values[10],
                 values[4] * values[14] - values[12] * values[6],
                 values[4] * values[10] - values[8] * values[6]};

    Vec4 fac5 = {values[8] * values[13] - values[12] * values[9],
                 values[8] * values[13] - values[12] * values[9],
                 values[4] * values[13] - values[12] * values[5],
                 values[4] * values[9] - values[8] * values[5]};

    Vec4 vec0 = {values[4], values[0], values[0], values[0]};
    Vec4 vec1 = {values[5], values[1], values[1], values[1]};
    Vec4 vec2 = {values[6], values[2], values[2], values[2]};
    Vec4 vec3 = {values[7], values[3], values[3], values[3]};

    Vec4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
    Vec4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
    Vec4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
    Vec4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;

    Vec4 signA = {+1, -1, +1, -1};
    Vec4 signB = {-1, +1, -1, +1};

    Mat4x4 inverse = {inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

    Vec4 dot0 = Vec4(inverse[0], inverse[4], inverse[8], inverse[12]) *
                Vec4(values[0], values[1], values[2], values[3]);
    float dot = (dot0.x + dot0.y) + (dot0.z + dot0.w);

    float OneOverDeterminant = 1.f / dot;

    return inverse * OneOverDeterminant;
}

Mat4x4 Mat4x4::Transpose() const
{
    return {values[0],
            values[4],
            values[8],
            values[12],
            values[1],
            values[5],
            values[9],
            values[13],
            values[2],
            values[6],
            values[10],
            values[14],
            values[3],
            values[7],
            values[11],
            values[15]};
}

Mat4x4 operator+(const Mat4x4& lhs, const Mat4x4& rhs)
{
    return Mat4x4::Add(lhs, rhs);
}

Mat4x4 operator-(const Mat4x4& lhs, const Mat4x4& rhs)
{
    return Mat4x4::Subtract(lhs, rhs);
}

Mat4x4 operator*(const Mat4x4& lhs, const float& scalar)
{
    return Mat4x4::MultiplyScalar(lhs, scalar);
}

Mat4x4 operator*(const Mat4x4& lhs, const Mat4x4& rhs)
{
    return Mat4x4::Multiply(lhs, rhs);
}

Vec4 operator*(const Mat4x4& lhs, const Vec4& rhs)
{
    return Mat4x4::Multiply(lhs, rhs);
}

Mat4x4 operator/(const Mat4x4& lhs, const Mat4x4& rhs)
{
    return Mat4x4::Divide(lhs, rhs);
}
} // namespace Siege
