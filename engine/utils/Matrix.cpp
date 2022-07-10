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
    return MAT2_LOGIC_OP(==, &&, &&, MAT2_BODY_FUNC);
}

bool Matrix2x2::operator!=(const Matrix2x2& other)
{
    return MAT2_LOGIC_OP(!=, ||, ||, MAT2_BODY_FUNC);
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
    MAT2_MATH_OP(+=, MAT2_BODY_FUNC);
}
void Matrix2x2::Subtract(const Matrix2x2& other)
{
    MAT2_MATH_OP(-=, MAT2_BODY_FUNC);
}
void Matrix2x2::MultiplyScalar(const float& scalar)
{
    MAT2_MATH_OP(*=, MAT2_BODY_SCALAR_FUNC);
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
    return MAT3_LOGIC_OP(==, &&, &&, MAT3_BODY_FUNC);
}

bool Matrix3x3::operator!=(const Matrix3x3& other)
{
    return MAT3_LOGIC_OP(!=, ||, ||, MAT3_BODY_FUNC);
}

void Matrix3x3::Add(const Matrix3x3& other)
{
    MAT3_MATH_OP(+=, MAT3_BODY_FUNC);
}

void Matrix3x3::Subtract(const Matrix3x3& other)
{
    MAT3_MATH_OP(-=, MAT3_BODY_FUNC);
}

void Matrix3x3::MultiplyScalar(const float& scalar)
{
    MAT3_MATH_OP(*=, MAT3_BODY_SCALAR_FUNC);
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

// -------------------------------------- Matrix4x4 -----------------------------------------------

const Matrix4x4 Matrix4x4::Identity = {1.f, 0.f, 0.f, 0.f,
                                       0.f, 1.f, 0.f, 0.f,
                                       0.f, 0.f, 1.f, 0.f,
                                       0.f, 0.f, 0.f, 1.f,};
const Matrix4x4 Matrix4x4::Zero {};

// static functions

Matrix4x4 Matrix4x4::Add(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 newMat = lhs;
    newMat.Add(rhs);
    return newMat;
}

Matrix4x4 Matrix4x4::Subtract(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 newMat = lhs;
    newMat.Subtract(rhs);
    return newMat;
}

Matrix4x4 Matrix4x4::MultiplyScalar(const Matrix4x4& lhs, const float& scalar)
{
    Matrix4x4 newMat = lhs;
    newMat.MultiplyScalar(scalar);
    return newMat;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    Matrix4x4 newMat = lhs;
    newMat.Multiply(rhs);
    return newMat;
}

Vec4 Matrix4x4::Multiply(const Matrix4x4& lhs, const Vec4& rhs)
{
    return lhs.Multiply(rhs);
}

Matrix4x4 Matrix4x4::Divide(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
    return lhs * rhs.Inverse();
}

float Matrix4x4::Determinant(const Matrix4x4& matrix)
{
    return matrix.Determinant();
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& matrix)
{
    return matrix.Inverse();
}

// operators

const float& Matrix4x4::operator[](const size_t& index) const
{
    assert(index < 16 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

float& Matrix4x4::operator[](const size_t& index)
{
    assert(index < 16 &&
           "Error: trying to index into matrix with a size that's greater than matrix!");

    return values[index];
}

bool Matrix4x4::operator==(const Matrix4x4& other)
{
    return MAT4_LOGIC_OP(==, &&, &&, MAT4_BODY_FUNC);
}

bool Matrix4x4::operator!=(const Matrix4x4& other)
{
    return MAT4_LOGIC_OP(!=, ||, ||, MAT4_BODY_FUNC);
}

Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& other)
{
    Add(other);
    return *this;
}

Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& other)
{
    Subtract(other);
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(const float& scalar)
{
    MultiplyScalar(scalar);
    return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& other)
{
    Multiply(other);
    return *this;
}

Matrix4x4& Matrix4x4::operator/=(const Matrix4x4& other)
{
    Divide(other);
    return *this;
}

Matrix4x4 Matrix4x4::operator-()
{
    return {
        {-values[0], -values[1], -values[2], -values[3]},
        {-values[4], -values[5], -values[6], -values[7]},
        {-values[8], -values[9], -values[10], -values[11]},
        {-values[12], -values[13], -values[14], -values[15]}};
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& matrix)
{
    return matrix.Transpose();
}

// functions

const float& Matrix4x4::Get(const size_t& rowIndex, const size_t& colIndex)
{
    return values[(4 * rowIndex) + colIndex];
}

void Matrix4x4::Add(const Matrix4x4& other)
{
    MAT4_MATH_OP(+=, MAT4_BODY_FUNC);
}

void Matrix4x4::Subtract(const Matrix4x4& other)
{
    MAT4_MATH_OP(-=, MAT4_BODY_FUNC);
}

void Matrix4x4::MultiplyScalar(const float& scalar)
{
    MAT4_MATH_OP(*=, MAT4_BODY_SCALAR_FUNC);
}

void Matrix4x4::Multiply(const Matrix4x4& rhs)
{
    // Rows of lhs matrix

    float m0x0, m0y0, m0z0, m0w0, // Row 0
        m0x1, m0y1, m0z1, m0w1,   // Row 1
        m0x2, m0y2, m0z2, m0w2,
        m0x3, m0y3, m0z3, m0w3;   // Row 2

    GET_ROW4(m0x0, m0y0, m0z0, m0w0, 0)
    GET_ROW4(m0x1, m0y1, m0z1, m0w1, 1)
    GET_ROW4(m0x2, m0y2, m0z2, m0w2, 2)
    GET_ROW4(m0x3, m0y3, m0z3, m0w3, 3)

    // Rows of rhs matrix

    float m1cx0, m1cy0, m1cz0, m1cw0,// Column 0
        m1cx1, m1cy1, m1cz1, m1cw1,  // Column 1
        m1cx2, m1cy2, m1cz2, m1cw2,  // Column 2
        m1cx3, m1cy3, m1cz3, m1cw3;  // Column 3

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

Vec4 Matrix4x4::Multiply(const Vec4& vector) const
{
    return {(values[0] * vector.x) + (values[1] * vector.y) + (values[2] * vector.z) + (values[3] * vector.w),
            (values[4] * vector.x) + (values[5] * vector.y) + (values[6] * vector.z) + (values[7] * vector.w),
            (values[8] * vector.x) + (values[9] * vector.y) + (values[10] * vector.z) + (values[11] * vector.w),
            (values[12] * vector.x) + (values[13] * vector.y) + (values[14] * vector.z) + (values[15] * vector.w)};
}

void Matrix4x4::Divide(const Matrix4x4& rhs)
{
    *this = Matrix4x4::Divide(*this, rhs);
}

float Matrix4x4::Determinant() const
{
    float SubFactor00 = values[10] * values[15] - values[11] * values[14];  // 1 2 3 4
    float SubFactor01 = values[6] * values[15] - values[7] * values[14];    // 5 6 7 8
    float SubFactor02 = values[6] * values[11] - values[7] * values[10];    // 9 0 1 2
    float SubFactor03 = values[2] * values[15] - values[3] * values[14];    // 3 4 5 6
    float SubFactor04 = values[2] * values[11] - values[3] * values[10];
    float SubFactor05 = values[2] * values[7] - values[3] * values[6];

    Vec4 coefficient = {
        + (values[5] * SubFactor00 - values[9] * SubFactor01 + values[13] * SubFactor02),
        - (values[1] * SubFactor00 - values[9] * SubFactor03 + values[13] * SubFactor04),
        + (values[1] * SubFactor01 - values[5] * SubFactor03 + values[13] * SubFactor05),
        - (values[1] * SubFactor02 - values[5] * SubFactor04 + values[9] * SubFactor05),
    };

    return values[0] * coefficient.x +
           values[4] * coefficient.y +
           values[8] * coefficient.z +
           values[12] * coefficient.w;
}

Matrix4x4 Matrix4x4::Inverse() const
{
    Vec4 fac0 = {(values[10] * values[15]) - (values[14] * values[11]), // 0  1  2  3
                 (values[10] * values[15]) - (values[14] * values[11]), // 4  5  6  7
                 (values[6] * values[15]) -  (values[14] * values[7]),  // 8  9  10 11
                 (values[6] * values[11]) - (values[10] * values[7])};  // 12 13 14 15

    Vec4 fac1 = {values[9] * values[15] - values[13] * values[11],
                 values[9] * values[15] - values[13] * values[11],
                 values[5] * values[15] -  values[13] * values[7],
                 values[5] * values[11] - values[9] * values[7]};

    Vec4 fac2 = {values[9] * values[14] - values[13] * values[10],
                 values[9] * values[14] - values[13] * values[10],
                 values[5] * values[14] -  values[13] * values[6],
                 values[5] * values[10] - values[9] * values[6]};

    Vec4 fac3 = {values[8] * values[15] - values[12] * values[11],
                 values[8] * values[15] - values[12] * values[11],
                 values[4] * values[15] -  values[12] * values[7],
                 values[4] * values[11] - values[8] * values[7]};

    Vec4 fac4 = {values[8] * values[14] - values[12] * values[10],  // 0  1  2  3
                 values[8] * values[14] - values[12] * values[10],  // 4  5  6  7
                 values[4] * values[14] -  values[12] * values[6],  // 8  9  10 11
                 values[4] * values[10] - values[8] * values[6]};   // 12 13 14 15

    Vec4 fac5 = {values[8] * values[13] - values[12] * values[9],
                 values[8] * values[13] - values[12] * values[9],
                 values[4] * values[13] -  values[12] * values[5],
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

    Matrix4x4 inverse = {inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

    Vec4 dot0 = Vec4(inverse[0], inverse[4], inverse[8], inverse[12]) * Vec4(values[0], values[1], values[2], values[3]);
    float dot = (dot0.x + dot0.y) + (dot0.z + dot0.w);

    float OneOverDeterminant = 1.f / dot;

    return inverse * OneOverDeterminant;
}

Matrix4x4 Matrix4x4::Transpose() const
{
    return {
        values[0], values[4], values[8], values[12],
        values[1], values[5], values[9], values[13],
        values[2], values[6], values[10], values[14],
        values[3], values[7], values[11], values[15]};
}

} // namespace Siege::Utils
