//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT3X3_H
#define SIEGE_ENGINE_MAT3X3_H

#include <cassert>

#include "../vec/Vec3.h"
#include "Macros.h"

namespace Siege
{
struct Mat3x3
{
    Mat3x3(const float& vx0 = 0.f,
           const float& vy0 = 0.f,
           const float& vz0 = 0.f,
           const float& vx1 = 0.f,
           const float& vy1 = 0.f,
           const float& vz1 = 0.f,
           const float& vx2 = 0.f,
           const float& vy2 = 0.f,
           const float& vz2 = 0.f) :
        values {vx0, vy0, vz0, vx1, vy1, vz1, vx2, vy2, vz2}
    {}

    Mat3x3(const Vec3& vec0, const Vec3& vec1, const Vec3& vec2) :
        Mat3x3(vec0.x, vec0.y, vec0.z, vec1.x, vec1.y, vec1.z, vec2.x, vec2.y, vec2.z)
    {}

    static const Mat3x3 Identity;
    static const Mat3x3 Zero;

    const float& Get(const size_t& rowIndex, const size_t& colIndex);

    static Mat3x3 Divide(const Mat3x3& lhs, const Mat3x3& rhs);
    static Mat3x3 Add(const Mat3x3& lhs, const Mat3x3& rhs);
    static Mat3x3 Multiply(const Mat3x3& lhs, const Mat3x3& rhs);
    static Mat3x3 Subtract(const Mat3x3& lhs, const Mat3x3& rhs);
    static Mat3x3 MultiplyScalar(const Mat3x3& lhs, const float& rhs);
    static Vec3 Multiply(const Mat3x3& lhs, const Vec3& rhs);

    static float Determinant(const Mat3x3& mat);
    static Mat3x3 Inverse(const Mat3x3& mat);

    constexpr float const& operator[](const size_t& index) const
    {
        assert(index < 9 &&
               "Error: trying to index into matrix with a size that's greater than matrix!");

        return values[index];
    }

    float& operator[](const size_t& index);

    Mat3x3& operator+=(const Mat3x3& other);
    Mat3x3& operator-=(const Mat3x3& other);
    Mat3x3& operator*=(const float& scalar);
    Mat3x3& operator*=(const Mat3x3& other);
    Mat3x3& operator/=(const Mat3x3& other);

    Mat3x3 operator-();

    bool operator==(const Mat3x3& other);
    bool operator!=(const Mat3x3& other);

    void Add(const Mat3x3& other);

    void Subtract(const Mat3x3& other);

    void MultiplyScalar(const float& scalar);

    void Multiply(const Mat3x3& matrix);
    Vec3 Multiply(const Vec3& vector) const;

    void Divide(const Mat3x3& other);

    float Determinant() const;
    Mat3x3 Inverse() const;
    Mat3x3 Transpose() const;

private:

    float values[9];
};

Mat3x3 operator+(const Mat3x3& lhs, const Mat3x3& rhs);
Mat3x3 operator-(const Mat3x3& lhs, const Mat3x3& rhs);
Mat3x3 operator*(const Mat3x3& lhs, const float& scalar);
Mat3x3 operator*(const Mat3x3& lhs, const Mat3x3& rhs);
const Vec3 operator*(const Mat3x3& lhs, const Siege::Vec3& rhs);
Mat3x3 operator/(const Mat3x3& lhs, const Mat3x3& rhs);
} // namespace Siege

#endif // SIEGE_ENGINE_MAT3X3_H
