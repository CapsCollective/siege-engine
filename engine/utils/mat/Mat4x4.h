//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT4X4_H
#define SIEGE_ENGINE_MAT4X4_H

#include <cassert>

#include "../vec/Vec4.h"
#include "Macros.h"
#include "Mat3x3.h"

namespace Siege
{
struct Mat4x4
{
public:

    Mat4x4(const float& x0 = 0.f,
           const float& y0 = 0.f,
           const float& z0 = 0.f,
           const float& w0 = 0.f,
           const float& x1 = 0.f,
           const float& y1 = 0.f,
           const float& z1 = 0.f,
           const float& w1 = 0.f,
           const float& x2 = 0.f,
           const float& y2 = 0.f,
           const float& z2 = 0.f,
           const float& w2 = 0.f,
           const float& x3 = 0.f,
           const float& y3 = 0.f,
           const float& z3 = 0.f,
           const float& w3 = 0.f) :
        values {x0, y0, z0, w0, x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3}
    {}

    Mat4x4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3) :
        Mat4x4(row0.x,
               row0.y,
               row0.z,
               row0.w,
               row1.x,
               row1.y,
               row1.z,
               row1.w,
               row2.x,
               row2.y,
               row2.z,
               row2.w,
               row3.x,
               row3.y,
               row3.z,
               row3.w)
    {}

    Mat4x4(const Mat3x3& other) :
        Mat4x4({other[0], other[1], other[2], 0.f},
               {other[3], other[4], other[5], 0.f},
               {other[6], other[7], other[8], 0.f},
               {0.f, 0.f, 0.f, 1.f})
    {}

    // public const variables

    static const Mat4x4 Identity;
    static const Mat4x4 Zero;

    // static functions

    static Mat4x4 Add(const Mat4x4& lhs, const Mat4x4& rhs);
    static Mat4x4 Subtract(const Mat4x4& lhs, const Mat4x4& rhs);
    static Mat4x4 MultiplyScalar(const Mat4x4& lhs, const float& scalar);
    static Mat4x4 Multiply(const Mat4x4& lhs, const Mat4x4& rhs);
    static Vec4 Multiply(const Mat4x4& lhs, const Vec4& rhs);
    static Mat4x4 Divide(const Mat4x4& lhs, const Mat4x4& rhs);

    static float Determinant(const Mat4x4& matrix);
    static Mat4x4 Inverse(const Mat4x4& matrix);
    static Mat4x4 Transpose(const Mat4x4& matrix);

    // operators

    constexpr float const& operator[](const size_t& index) const
    {
        assert(index < 16 &&
               "Error: trying to index into matrix with a size that's greater than matrix!");

        return values[index];
    }

    float& operator[](const size_t& index);

    bool operator==(const Mat4x4& other);
    bool operator!=(const Mat4x4& other);

    Mat4x4& operator+=(const Mat4x4& other);
    Mat4x4& operator-=(const Mat4x4& other);
    Mat4x4& operator*=(const float& scalar);
    Mat4x4& operator*=(const Mat4x4& other);
    Mat4x4& operator/=(const Mat4x4& other);

    Mat4x4 operator-();

    // functions

    const float& Get(const size_t& rowIndex, const size_t& colIndex);
    void Add(const Mat4x4& other);
    void Subtract(const Mat4x4& other);
    void MultiplyScalar(const float& scalar);
    void Multiply(const Mat4x4& rhs);
    Vec4 Multiply(const Vec4& vector) const;
    void Divide(const Mat4x4& rhs);

    float Determinant() const;
    Mat4x4 Inverse() const;
    Mat4x4 Transpose() const;

    float values[16];
};

Mat4x4 operator+(const Mat4x4& lhs, const Mat4x4& rhs);
Mat4x4 operator-(const Mat4x4& lhs, const Mat4x4& rhs);
Mat4x4 operator*(const Mat4x4& lhs, const float& scalar);
Mat4x4 operator*(const Mat4x4& lhs, const Mat4x4& rhs);
Vec4 operator*(const Mat4x4& lhs, const Vec4& rhs);
Mat4x4 operator/(const Mat4x4& lhs, const Mat4x4& rhs);
} // namespace Siege

#endif // SIEGE_ENGINE_MAT4X4_H
