//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT2X2_H
#define SIEGE_ENGINE_MAT2X2_H

#include "../Vec/Vec2.h"
#include "Macros.h"

namespace Siege::Mat
{
struct Mat2x2
{
    Mat2x2(const float& x0 = 0, const float& y0 = 0, const float& x1 = 0, const float& y1 = 0) :
        values {x0, y0, x1, y1}
    {}

    Mat2x2(const Vec::Vec2& row0, const Vec::Vec2& row1) : Mat2x2(row0.x, row0.y, row1.x, row1.y) {}

    static const Mat2x2 Identity;
    static const Mat2x2 Zero;

    static Mat2x2 Divide(const Mat2x2& lhs, const Mat2x2& rhs);
    static Mat2x2 Add(const Mat2x2& lhs, const Mat2x2& rhs);
    static Mat2x2 Subtract(const Mat2x2& lhs, const Mat2x2& rhs);
    static Mat2x2 MultiplyScalar(const Mat2x2& lhs, const float& scalar);
    static Mat2x2 Multiply(const Mat2x2& lhs, const Mat2x2& rhs);
    static Vec::Vec2 Multiply(const Mat2x2& lhs, const Vec::Vec2& rhs);

    static float Determinant(const Mat2x2& mat);
    static Mat2x2 Inverse(const Mat2x2& mat);

    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    const float& operator[](const size_t& index) const;

    bool operator==(const Mat2x2& other);
    bool operator!=(const Mat2x2& other);

    Mat2x2& operator+=(const Mat2x2& other);
    Mat2x2& operator-=(const Mat2x2& other);
    Mat2x2& operator*=(const Mat2x2& other);
    Mat2x2& operator/=(const Mat2x2& other);
    Mat2x2& operator*=(const float& scalar);

    Mat2x2 operator-();

    void Add(const Mat2x2& other);
    void Subtract(const Mat2x2& other);
    void MultiplyScalar(const float& scalar);
    void Multiply(const Mat2x2& other);
    Vec::Vec2 Multiply(const Vec::Vec2& vector) const;
    void Divide(const Mat2x2& other);

    float Determinant() const;
    Mat2x2 Inverse() const;
    Mat2x2 Transpose() const;

private:

    float values[4];
};

inline Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Add(lhs, rhs);
}

inline Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Subtract(lhs, rhs);
}

inline Mat2x2 operator*(const Mat2x2& lhs, const float& scalar)
{
    return Mat2x2::MultiplyScalar(lhs, scalar);
}

inline Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Multiply(lhs, rhs);
}

inline Mat2x2 operator/(const Mat2x2& lhs, const Mat2x2& rhs)
{
    return Mat2x2::Divide(lhs, rhs);
}

inline Vec::Vec2 operator*(const Mat2x2& lhs, const Vec::Vec2& rhs)
{
    return Mat2x2::Multiply(lhs, rhs);
}
} // namespace Siege::Mat

#endif // SIEGE_ENGINE_MAT2X2_H
