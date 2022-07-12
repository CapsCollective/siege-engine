//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT2X2_H
#define SIEGE_ENGINE_MAT2X2_H

#include <cassert>

#include "../vec/Vec2.h"
#include "Macros.h"

namespace Siege
{
struct Mat2x2
{
    Mat2x2(const float& x0 = 0, const float& y0 = 0, const float& x1 = 0, const float& y1 = 0) :
        values {x0, y0, x1, y1}
    {}

    Mat2x2(const Vec2& row0, const Vec2& row1) : Mat2x2(row0.x, row0.y, row1.x, row1.y) {}

    static const Mat2x2 Identity;
    static const Mat2x2 Zero;

    static Mat2x2 Divide(const Mat2x2& lhs, const Mat2x2& rhs);
    static Mat2x2 Add(const Mat2x2& lhs, const Mat2x2& rhs);
    static Mat2x2 Subtract(const Mat2x2& lhs, const Mat2x2& rhs);
    static Mat2x2 MultiplyScalar(const Mat2x2& lhs, const float& scalar);
    static Mat2x2 Multiply(const Mat2x2& lhs, const Mat2x2& rhs);
    static Vec2 Multiply(const Mat2x2& lhs, const Vec2& rhs);

    static float Determinant(const Mat2x2& mat);
    static Mat2x2 Inverse(const Mat2x2& mat);

    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    constexpr float const& operator[](const size_t& index) const
    {
        assert(index < 4 &&
               "Error: trying to index into matrix with a size that's greater than matrix!");

        return values[index];
    }

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
    Vec2 Multiply(const Vec2& vector) const;
    void Divide(const Mat2x2& other);

    float Determinant() const;
    Mat2x2 Inverse() const;
    Mat2x2 Transpose() const;

private:

    float values[4];
};

Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs);
Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs);
Mat2x2 operator*(const Mat2x2& lhs, const float& scalar);
Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs);
Mat2x2 operator/(const Mat2x2& lhs, const Mat2x2& rhs);
Vec2 operator*(const Mat2x2& lhs, const Vec2& rhs);
} // namespace Siege

#endif // SIEGE_ENGINE_MAT2X2_H
