//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MATRIX_H
#define SIEGE_ENGINE_MATRIX_H

#include "Maths.h"

#define X(idx, size) idx* size
#define Y(idx, size) (idx * size) + 1
#define Z(idx, size) (idx * size) + 2

#define LHS(dimension, idx, size) values[dimension(idx, size)]
#define RHS(dimension, idx, size) other[dimension(idx, size)]

#define MAT3_MATH_OP(op, body) \
    body(op, _SEMICOLON, 0) body(op, _SEMICOLON, 1) body(op, _SEMICOLON, 2)

#define MAT2_MATH_OP(op, body) body(op, _SEMICOLON, 0) body(op, _SEMICOLON, 1)

#define MAT3_LOGIC_OP(op, logic, bLogic, body) \
    body(op, logic, 0) bLogic body(op, logic, 1) bLogic body(op, logic, 2);

#define MAT2_LOGIC_OP(op, logic, bLogic, body) body(op, logic, 0) bLogic body(op, logic, 1)

#define MAT3_MATH_OPERATOR_BODY_VEC3(op, sep, idx) \
    LHS(X, idx, 3)                                 \
    op RHS(X, idx, 3) sep LHS(Y, idx, 3) op RHS(Y, idx, 3) sep LHS(Z, idx, 3) op RHS(Z, idx, 3) sep

#define MAT2_MATH_OPERATOR_BODY(op, sep, idx) \
    LHS(X, idx, 2) op RHS(X, idx, 2) sep LHS(Y, idx, 2) op RHS(Y, idx, 2) sep

#define MAT3_MATH_SCALAR_OPERATOR_BODY_VEC3(op, sep, idx) \
    LHS(X, idx, 3) op scalar sep LHS(Y, idx, 3) op scalar sep LHS(Z, idx, 3) op scalar sep

#define MAT2_MATH_SCALAR_OPERATOR_BODY(op, sep, idx) \
    LHS(X, idx, 2) op scalar sep LHS(Y, idx, 2) op scalar sep

#define MAT2_LOGIC_OPERATOR_BODY(op, sep, idx) \
    LHS(X, idx, 2) op RHS(X, idx, 2) sep LHS(Y, idx, 2) op RHS(Y, idx, 2)

#define MAT3_LOGIC_OPERATOR_BODY_VEC3(op, sep, idx) \
    LHS(X, idx, 3)                                  \
    op RHS(X, idx, 3) sep LHS(Y, idx, 3) op RHS(Y, idx, 3) sep LHS(Z, idx, 3) op RHS(Z, idx, 3)

#define GET_ROW3(varX, varY, varZ, idx) \
    varX = LHS(X, idx, 3), varY = LHS(Y, idx, 3), varZ = LHS(Z, idx, 3);

#define GET_ROW2(varX, varY, idx) varX = LHS(X, idx, 2), varY = LHS(Y, idx, 2);

#define GET_COLUMN3(varX, varY, varZ, vals, idx) \
    varX = vals[idx], varY = vals[idx + 3], varZ = vals[idx + 6];

#define GET_COLUMN2(varX, varY, vals, idx) varX = vals[idx], varY = vals[idx + 2];

#define DOT(m0x, m0y, m1x, m1y) (m0x * m1x) + (m0y * m1y)

#define DOT3(m0x, m0y, m0z, m1x, m1y, m1z) DOT(m0x, m0y, m1x, m1y) + (m0z * m1z)

namespace Siege::Utils
{

// Possible Matrix/Vector operations:

// 2x2 * 2x2 = 2x2 -> TBD
// 2x3 * 3x2 = 2x3 -> TBD
// 3x3 * 3x3 = 3x3 -> (DONE)
// 3x3 * 3x2 = 3x2 -> TBD
// 3x3 * 3x4 = 3x4 -> TBD
// 3x3 * 3x1 = 3x1 -> a 3 dimensional vector (DONE)

// -------------------------------------- Matrix2x2 -----------------------------------------------

struct Matrix2x2
{
    Matrix2x2() : Matrix2x2(0, 0, 0, 0) {}

    Matrix2x2(const float& x0, const float& y0, const float& x1, const float& y1) :
        values {x0, y0, x1, y1}
    {}

    static const Matrix2x2 Identity;
    static const Matrix2x2 Zero;

    static Matrix2x2 Divide(const Matrix2x2& lhs, const Matrix2x2& rhs);
    static Matrix2x2 Add(const Matrix2x2& lhs, const Matrix2x2& rhs);
    static Matrix2x2 Subtract(const Matrix2x2& lhs, const Matrix2x2& rhs);
    static Matrix2x2 MultiplyScalar(const Matrix2x2& lhs, const float& scalar);
    static Matrix2x2 Multiply(const Matrix2x2& lhs, const Matrix2x2& rhs);
    static Vec2 Multiply(const Matrix2x2& lhs, const Vec2& rhs);

    static float Determinant(const Matrix2x2& mat);
    static Matrix2x2 Inverse(const Matrix2x2& mat);

    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    const float& operator[](const size_t& index) const;

    bool operator==(const Matrix2x2& other);
    bool operator!=(const Matrix2x2& other);

    Matrix2x2& operator+=(const Matrix2x2& other);
    Matrix2x2& operator-=(const Matrix2x2& other);
    Matrix2x2& operator*=(const Matrix2x2& other);
    Matrix2x2& operator/=(const Matrix2x2& other);
    Matrix2x2& operator*=(const float& scalar);

    Matrix2x2 operator-();

    void Add(const Matrix2x2& other);
    void Subtract(const Matrix2x2& other);
    void MultiplyScalar(const float& scalar);
    void Multiply(const Matrix2x2& other);
    Vec2 Multiply(const Vec2& vector) const;
    void Divide(const Matrix2x2& other);

    float Determinant() const;
    Matrix2x2 Inverse() const;

private:

    float values[4];
};

inline Matrix2x2 operator+(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2::Add(lhs, rhs);
}

inline Matrix2x2 operator-(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2::Subtract(lhs, rhs);
}

inline Matrix2x2 operator*(const Matrix2x2& lhs, const float& scalar)
{
    return Matrix2x2::MultiplyScalar(lhs, scalar);
}

inline Matrix2x2 operator*(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2::Multiply(lhs, rhs);
}

inline Matrix2x2 operator/(const Matrix2x2& lhs, const Matrix2x2& rhs)
{
    return Matrix2x2::Divide(lhs, rhs);
}

inline Vec2 operator*(const Matrix2x2& lhs, const Vec2& rhs)
{
    return Matrix2x2::Multiply(lhs, rhs);
}

// -------------------------------------- Matrix3x3 -----------------------------------------------

struct Matrix3x3
{
    Matrix3x3(const float& vx0,
              const float& vy0,
              const float& vz0,
              const float& vx1,
              const float& vy1,
              const float& vz1,
              const float& vx2,
              const float& vy2,
              const float& vz2) :
        values {vx0, vy0, vz0, vx1, vy1, vz1, vx2, vy2, vz2}
    {}

    Matrix3x3() : Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0) {}

    Matrix3x3(const Vec3& vec0, const Vec3& vec1, const Vec3& vec2) :
        Matrix3x3(vec0.x, vec0.y, vec0.z, vec1.x, vec1.y, vec1.z, vec2.x, vec2.y, vec2.z)
    {}

    static const Matrix3x3 Identity;
    static const Matrix3x3 Zero;

    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    static Matrix3x3 Divide(const Matrix3x3& lhs, const Matrix3x3& rhs);
    static Matrix3x3 Add(const Matrix3x3& lhs, const Matrix3x3& rhs);
    static Matrix3x3 Multiply(const Matrix3x3& lhs, const Matrix3x3& rhs);
    static Matrix3x3 Subtract(const Matrix3x3& lhs, const Matrix3x3& rhs);
    static Matrix3x3 MultiplyScalar(const Matrix3x3& lhs, const float& rhs);
    static Vec3 Multiply(const Matrix3x3& lhs, const Vec3& rhs);

    static float Determinant(const Matrix3x3& mat);
    static Matrix3x3 Inverse(const Matrix3x3& mat);

    const float& operator[](const size_t& index) const;
    float& operator[](const size_t& index);

    Matrix3x3& operator+=(const Matrix3x3& other);
    Matrix3x3& operator-=(const Matrix3x3& other);
    Matrix3x3& operator*=(const float& scalar);
    Matrix3x3& operator*=(const Matrix3x3& other);
    Matrix3x3& operator/=(const Matrix3x3& other);

    Matrix3x3 operator-();

    bool operator==(const Matrix3x3& other);
    bool operator!=(const Matrix3x3& other);

    void Add(const Matrix3x3& other);

    void Subtract(const Matrix3x3& other);

    void MultiplyScalar(const float& scalar);

    void Multiply(const Matrix3x3& matrix);
    Vec3 Multiply(const Vec3& vector) const;

    void Divide(const Matrix3x3& other);

    float Determinant() const;
    Matrix3x3 Inverse() const;

private:

    float values[9];
};

inline Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    return Matrix3x3::Add(lhs, rhs);
}

inline Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    return Matrix3x3::Subtract(lhs, rhs);
}

inline Matrix3x3 operator*(const Matrix3x3& lhs, const float& scalar)
{
    return Matrix3x3::MultiplyScalar(lhs, scalar);
}

inline Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    return Matrix3x3::Multiply(lhs, rhs);
}

inline Vec3 operator*(const Matrix3x3& lhs, const Vec3& rhs)
{
    return Matrix3x3::Multiply(lhs, rhs);
}

inline Matrix3x3 operator/(const Matrix3x3& lhs, const Matrix3x3& rhs)
{
    return Matrix3x3::Divide(lhs, rhs);
}

// struct Matrix4
//{
// public:
//
// private:
//
//     float values[16];
// };
} // namespace Siege::Utils

#endif // SIEGE_ENGINE_MATRIX_H
