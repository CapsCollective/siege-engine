//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#ifndef SIEGE_ENGINE_VEC4_H
#define SIEGE_ENGINE_VEC4_H

#include "Vec3.h"

#define VEC4_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep w op rhs.w sep

#define VEC4_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep z op rhs sep w op rhs sep

#define VEC4_OPERATOR_BODY_VEC3(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep w sep

#define VEC4_OPERATOR_BODY_VEC2(op, sep) x op rhs.x sep y op rhs.y sep z sep w sep

namespace Siege
{

/**
 * Struct representing a 3 dimensional vector.
 */
struct Vec4
{
    // Public constants

    /**
     * A vector in which all elements are 0.
     */
    static const Vec4 Zero;

    /**
     * A vector in which all elements are instantiated to 1.
     */
    static const Vec4 One;

    /**
     * A vector in which all elements except for the y value are 0.
     */
    static const Vec4 Up;

    // 'Structors

    /**
     * Base Vector constructor.
     * @param x the value to be given to the x dimension (default is zero)
     * @param y the value to be given to the y dimension (default is zero)
     * @param z the value to be given to the z dimension (default is zero)
     * @param 2 the value to be given to the 2 dimension (default is zero)
     */
    Vec4(const float& x = 0.f, const float& y = 0.f, const float& z = 0.f, const float& w = 0.f) :
        x {x},
        y {y},
        z {z},
        w {w}
    {}

    /**
     * A constructor from a 2D vector.
     * @param other the 2D vector to copy elements from.
     */
    Vec4(const Vec2& other);

    /**
     * A constructor from a 3D vector.
     * @param other the 3D vector to copy elements from.
     */
    Vec4(const Vec3& other);

    // Static functions

    /**
     * Lerps between two vectors
     * @param origin the start vector.
     * @param destination the end result
     * @param time the interpolation point.
     * @return an interpolated vector between origin and destination.
     */
    static Vec4 Lerp(Vec4 origin, Vec4 destination, float time);

    // Operator overloads

    /**
     * Default assignment operator.
     * @param rhs variable to assign values from.
     * @return a reference to the current object.
     */
    Vec4& operator=(const Vec4& rhs) = default;

    /**
     * A four dimensional Vector addition operator.
     * @param rhs the 4D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(+=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A four dimensional Vector subtraction operator.
     * @param rhs the 4D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(-=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A four dimensional Vector multiplication operator.
     * @param rhs the 4D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(*=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A four dimensional Vector division operator.
     * @param rhs the 4D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(/=, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A 4D - 2D Vector addition operator.
     * @param rhs the 2D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(+=, Vec4, Vec2)

    /**
     * A 4D - 2D Vector subtraction operator.
     * @param rhs the 2D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(-=, Vec4, Vec2)

    /**
     * A 4D - 2D Vector multiplication operator.
     * @param rhs the 2D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(*=, Vec4, Vec2)

    /**
     * A 4D - 2D Vector division operator.
     * @param rhs the 3D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(/=, Vec4, Vec2)

    /**
     * A 4D - 3D Vector addition operator.
     * @param rhs the 3D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(+=, Vec4, Vec3)

    /**
     * A 4D - 3D Vector subtraction operator.
     * @param rhs the 3D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(-=, Vec4, Vec3)

    /**
     * A 4D - 3D Vector multiplication operator.
     * @param rhs the 3D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(*=, Vec4, Vec3)

    /**
     * A 4D - 3D Vector division operator.
     * @param rhs the 3D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(/=, Vec4, Vec3)

    /**
     * An inequality operator.
     * @param other the vector to be compared.
     * @return a boolean specifying if the two vectors are not equal.
     */
    bool operator!=(const Vec4& other);

    /**
     * An equality operator.
     * @param other the vector to be compared.
     * @return a boolean specifying if the two vectors are equal.
     */
    bool operator==(const Vec4& other);

    /**
     * A scalar addition operator.
     * @param rhs the scalar to add.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(+=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A scalar subtraction operator.
     * @param rhs the scalar to subtract.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(-=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A scalar multiplication operator.
     * @param rhs the scalar to multiply.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(*=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A scalar division operator.
     * @param rhs the scalar to divide.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(/=, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A four dimensional Vector Addition operator.
     * @param rhs the 4D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_CONST_OPERATOR(+, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A four dimensional Vector subtraction operator.
     * @param rhs the 4D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_CONST_OPERATOR(-, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A four dimensional Vector multiplication operator.
     * @param rhs the 4D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_CONST_OPERATOR(*, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A four dimensional Vector division operator.
     * @param rhs the 4D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_CONST_OPERATOR(/, Vec4, Vec4, VEC4_OPERATOR_BODY_VEC)

    /**
     * A 4D - 3D vector const addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec4, Vec3)

    /**
     * A 4D - 3D vector const subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec4, Vec3)

    /**
     * A 4D - 3D vector const multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec4, Vec3)

    /**
     * A 4D - 3D vector const division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec4, Vec3)

    /**
     * A 4D - 2D vector const addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec4, Vec2)

    /**
     * A 4D - 2D vector const subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec4, Vec2)

    /**
     * A 4D - 2D vector const multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec4, Vec2)

    /**
     * A 4D - 2D vector const division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec4, Vec2)

    /**
     * A const scalar addition operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR(+, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR(-, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR(*, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR(/, Vec4, float, VEC4_OPERATOR_BODY_FLOAT)

    // Static methods

    /**
     * Creates a Vec4 from a String.
     * @param vec a reference to a vector to populate
     * @param string the string to extract values from.
     * @return a boolean specifying if the operation was successful.
     */
    static bool FromString(OUT Vec4& vec, const String& string);

    // Public methods

    /**
     * Returns a string representation of the vector.
     * @return the string result of the operation.
     */
    String ToString() const;

    /**
     * Returns a vector with the x dimension filled in and the rest set to 0.
     * @return a vector with the vector's x dimension.
     */
    Vec4 XComp() const
    {
        return {x};
    }

    /**
     * Returns a vector with the y dimension filled in and the rest set to 0.
     * @return a vector with the vector's y dimension.
     */
    Vec4 YComp() const
    {
        return {0.f, y};
    }

    /**
     * Returns a vector with the z dimension filled in and the rest set to 0.
     * @return a vector with the vector's z dimension.
     */
    Vec4 ZComp() const
    {
        return {0.f, 0.f, z};
    }

    /**
     * Returns a vector with the 2 dimension filled in and the rest set to 0.
     * @return a vector with the vector's 2 dimension.
     */
    Vec4 WComp() const
    {
        return {0.f, 0.f, 0.f, w};
    }

    /**
     * Computes the length of the vector.
     * @return the length of the vector.
     */
    float Length() const
    {
        return sqrtf(x * x + y * y + z * z + w * w);
    }

    /**
     * Normalises the vector elements to range between 0 and 1.
     * @return
     */
    Vec4 Normalise() const;

    float x, y, z, w;
};

// Binary Operators

inline Vec4 operator/(const float& scalar, const Vec4& vec)
{
    return {scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w};
}

inline Vec4 operator*(const float& scalar, const Vec4& vec)
{
    return {scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w};
}

inline bool operator==(const Vec4& lhs, const Vec4& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}
} // namespace Siege

template<>
struct std::hash<Siege::Vec4>
{
    size_t operator()(const Siege::Vec4& vec) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vec.x, vec.y, vec.z, vec.w);
        return seed;
    };
};

#endif // SIEGE_ENGINE_VEC4_H
