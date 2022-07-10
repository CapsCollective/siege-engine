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
#define W(idx, size) (idx * size) + 3

#define LHS(dimension, idx, size) values[dimension(idx, size)]
#define RHS(dimension, idx, size) other[dimension(idx, size)]

#define DEFINE_OP(op, sep, idx, element, rowSize) \
    LHS(element, idx, rowSize) op RHS(element, idx, rowSize)

#define DEFINE_SCALAR_OP(op, sep, idx, element, rowSize) \
    LHS(element, idx, rowSize) op scalar

// Matrix3x3 macros

#define MAT3_MATH_OP(op, body) \
    MAT2_MATH_OP(op, body) _SEMICOLON body(op, _SEMICOLON, 2)

#define MAT3_LOGIC_OP(op, logic, bLogic, body) \
    MAT2_LOGIC_OP(op, logic, bLogic, body) bLogic body(op, logic, 2)

#define MAT3_BODY_FUNC(op, sep, idx) \
    DEFINE_OP(op, sep, idx, X, 3) sep DEFINE_OP(op, sep, idx, Y, 3) sep \
    DEFINE_OP(op, sep, idx, Z, 3)

#define MAT3_BODY_SCALAR_FUNC(op, sep, idx) \
    DEFINE_SCALAR_OP(op, sep, idx, X, 3) sep DEFINE_SCALAR_OP(op, sep, idx, Y, 3) sep \
    DEFINE_SCALAR_OP(op, sep, idx, Z, 3)

#define GET_ROW3(varX, varY, varZ, idx) \
    varX = LHS(X, idx, 3), varY = LHS(Y, idx, 3), varZ = LHS(Z, idx, 3);

#define GET_COLUMN3(varX, varY, varZ, vals, idx) \
    varX = vals[idx], varY = vals[idx + 3], varZ = vals[idx + 6];

#define DOT3(m0x, m0y, m0z, m1x, m1y, m1z) DOT(m0x, m0y, m1x, m1y) + (m0z * m1z)

// Matrix2x2 macros

#define MAT2_MATH_OP(op, body) body(op, _SEMICOLON, 0) _SEMICOLON body(op, _SEMICOLON, 1)

#define MAT2_LOGIC_OP(op, logic, bLogic, body) body(op, logic, 0) bLogic body(op, logic, 1)

#define MAT2_BODY_FUNC(op, sep, idx)                       \
    DEFINE_OP(op, sep, idx, X, 2) sep DEFINE_OP(op, sep, idx, Y, 2) \

#define MAT2_BODY_SCALAR_FUNC(op, sep, idx) \
    DEFINE_SCALAR_OP(op, sep, idx, X, 2) sep DEFINE_SCALAR_OP(op, sep, idx, Y, 2)

#define GET_ROW2(varX, varY, idx) varX = LHS(X, idx, 2), varY = LHS(Y, idx, 2);

#define GET_COLUMN2(varX, varY, vals, idx) varX = vals[idx], varY = vals[idx + 2];

#define DOT(m0x, m0y, m1x, m1y) (m0x * m1x) + (m0y * m1y)

// Matrix4x4 macros

#define MAT4_MATH_OP(op, body) MAT3_MATH_OP(op, body) _SEMICOLON body(op, _SEMICOLON, 3)

#define MAT4_LOGIC_OP(op, logic, bLogic, body) \
    MAT3_LOGIC_OP(op, logic, bLogic, body) bLogic body(op, logic, 3)

#define MAT4_BODY_FUNC(op, sep, idx) \
    DEFINE_OP(op, sep, idx, X, 4) sep DEFINE_OP(op, sep, idx, Y, 4) sep \
    DEFINE_OP(op, sep, idx, Z, 4) sep DEFINE_OP(op, sep, idx, W, 4)

#define MAT4_BODY_SCALAR_FUNC(op, sep, idx) \
    DEFINE_SCALAR_OP(op, sep, idx, X, 4) sep DEFINE_SCALAR_OP(op, sep, idx, Y, 4) sep \
    DEFINE_SCALAR_OP(op, sep, idx, Z, 4) sep DEFINE_SCALAR_OP(op, sep, idx, W, 4)

#define GET_ROW4(varX, varY, varZ, varW, idx) \
    varX = LHS(X, idx, 4), varY = LHS(Y, idx, 4), varZ = LHS(Z, idx, 4), varW = LHS(W, idx, 4);

#define GET_COLUMN4(varX, varY, varZ, varW, vals, idx) \
    varX = vals[idx], varY = vals[idx + 4], varZ = vals[idx + 8], varW = vals[idx + 12];

#define DOT4(m0x, m0y, m0z, m0w, m1x, m1y, m1z, m1w) \
    DOT3(m0x, m0y, m0z, m1x, m1y, m1z) + (m0w * m1w)

namespace Siege::Utils
{

// Possible Matrix/Vector operations:

// 2x2 * 2x2 = 2x2 -> DONE
// 2x3 * 3x2 = 2x3 -> TBD
// 3x3 * 3x3 = 3x3 -> DONE
// 3x3 * 3x2 = 3x2 -> TBD
// 3x3 * 3x4 = 3x4 -> TBD
// 3x3 * 3x1 = 3x1 -> a 3 dimensional vector (DONE)

// -------------------------------------- Matrix2x2 -----------------------------------------------

struct Matrix2x2
{
    Matrix2x2(const float& x0 = 0, const float& y0 = 0, const float& x1 = 0, const float& y1 = 0) :
        values {x0, y0, x1, y1} {}

    Matrix2x2(const Vec2& row0, const Vec2& row1)
        : Matrix2x2(row0.x, row0.y, row1.x, row1.y) {}

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
    Matrix2x2 Transpose() const;

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
    Matrix3x3(const float& vx0 = 0.f,
              const float& vy0 = 0.f,
              const float& vz0 = 0.f,
              const float& vx1 = 0.f,
              const float& vy1 = 0.f,
              const float& vz1 = 0.f,
              const float& vx2 = 0.f,
              const float& vy2 = 0.f,
              const float& vz2 = 0.f)
        : values {vx0, vy0, vz0, vx1, vy1, vz1, vx2, vy2, vz2}
    {}

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
    Matrix3x3 Transpose() const;

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

 struct Matrix4x4
{
 public:
     Matrix4x4(const float& x0 = 0.f, const float& y0 = 0.f, const float& z0 = 0.f, const float& w0 = 0.f,
               const float& x1 = 0.f, const float& y1 = 0.f, const float& z1 = 0.f, const float& w1 = 0.f,
               const float& x2 = 0.f, const float& y2 = 0.f, const float& z2 = 0.f, const float& w2 = 0.f,
               const float& x3 = 0.f, const float& y3 = 0.f, const float& z3 = 0.f, const float& w3 = 0.f)
         : values {x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3} {}

     Matrix4x4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3)
         : Matrix4x4(row0.x, row0.y, row0.z, row0.w,
                   row1.x, row1.y, row1.z, row1.w,
                   row2.x, row2.y, row2.z, row2.w,
                   row3.x, row3.y, row3.z, row3.w) {}

     // public const variables

     static const Matrix4x4 Identity;
     static const Matrix4x4 Zero;

     // static functions

     static Matrix4x4 Add(const Matrix4x4& lhs, const Matrix4x4& rhs);
     static Matrix4x4 Subtract(const Matrix4x4& lhs, const Matrix4x4& rhs);
     static Matrix4x4 MultiplyScalar(const Matrix4x4& lhs, const float& scalar);
     static Matrix4x4 Multiply(const Matrix4x4& lhs, const Matrix4x4& rhs);
     static Vec4 Multiply(const Matrix4x4& lhs, const Vec4& rhs);
     static Matrix4x4 Divide(const Matrix4x4& lhs, const Matrix4x4& rhs);

     static float Determinant(const Matrix4x4& matrix);
     static Matrix4x4 Inverse(const Matrix4x4& matrix);
     static Matrix4x4 Transpose(const Matrix4x4& matrix);

     // operators

     const float& operator[](const size_t& index) const;
     float& operator[](const size_t& index);

     bool operator==(const Matrix4x4& other);
     bool operator!=(const Matrix4x4& other);

     Matrix4x4& operator+=(const Matrix4x4& other);
     Matrix4x4& operator-=(const Matrix4x4& other);
     Matrix4x4& operator*=(const float& scalar);
     Matrix4x4& operator*=(const Matrix4x4& other);
     Matrix4x4& operator/=(const Matrix4x4& other);

     Matrix4x4 operator-();

     // functions

     const float& Get(const size_t& rowIndex, const size_t& colIndex);
     void Add(const Matrix4x4& other);
     void Subtract(const Matrix4x4& other);
     void MultiplyScalar(const float& scalar);
     void Multiply(const Matrix4x4& rhs);
     Vec4 Multiply(const Vec4& vector) const;
     void Divide(const Matrix4x4& rhs);

     float Determinant() const;
     Matrix4x4 Inverse() const;
     Matrix4x4 Transpose() const;

 private:

     float values[16];
 };

 inline Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs)
 {
     return Matrix4x4::Add(lhs, rhs);
 }

 inline Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs)
 {
     return Matrix4x4::Subtract(lhs, rhs);
 }

 inline Matrix4x4 operator*(const Matrix4x4& lhs, const float& scalar)
 {
     return Matrix4x4::MultiplyScalar(lhs, scalar);
 }

 inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
 {
     return Matrix4x4::Multiply(lhs, rhs);
 }

 inline Vec4 operator*(const Matrix4x4& lhs, const Vec4& rhs)
 {
     return Matrix4x4::Multiply(lhs, rhs);
 }

 inline Matrix4x4 operator/(const Matrix4x4& lhs, const Matrix4x4& rhs)
 {
     return Matrix4x4::Divide(lhs, rhs);
 }


} // namespace Siege::Utils

#endif // SIEGE_ENGINE_MATRIX_H
