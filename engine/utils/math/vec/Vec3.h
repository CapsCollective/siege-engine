//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC3_H
#define SIEGE_ENGINE_VEC3_H

#include "Vec2.h"

#define VEC_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep

#define VEC_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep z op rhs sep

namespace Siege
{

/**
 * Struct representing a 3 dimensional vector.
 */
struct Vec3
{
    // Public constants

    /**
     * A vector in which all elements are 0.
     */
    static const Vec3 Zero;

    /**
     * A vector in which all elements are instantiated to 1.
     */
    static const Vec3 One;

    /**
     * A vector in which all elements except for the y value are 0.
     */
    static const Vec3 Up;

    // 'Structors

    /**
     * Base Vector constructor.
     * @param x the value to be given to the x dimension (default is zero)
     * @param y the value to be given to the y dimension (default is zero)
     * @param z the value to be given to the z dimension (default is zero)
     */
    Vec3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

    // Static functions:

    /**
     * Lerps between two vectors
     * @param origin the start vector.
     * @param destination the end result
     * @param time the interpolation point.
     * @return an interpolated vector between origin and destination.
     */
    static Vec3 Lerp(Vec3 origin, Vec3 destination, float time);

    // Operator overloads

    /**
     * Default assignment operator.
     * @param rhs variable to assign values from.
     * @return a reference to the current object.
     */
    Vec3& operator=(const Vec3& rhs) = default;

    /**
     * Assignment operator from a 2D vector
     * @param rhs variable to assign values from.
     * @return a reference to the current object.
     */
    Vec3& operator=(const Vec2& rhs);

    // Operator overloads

    /**
     * A three dimensional Vector addition operator.
     * @param rhs the 3D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(+=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A three dimensional Vector subtraction operator.
     * @param rhs the 3D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(-=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A three dimensional Vector multiplication operator.
     * @param rhs the 3D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(*=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A three dimensional Vector division operator.
     * @param rhs the 3D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(/=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * An equality operator.
     * @param other the vector to be compared.
     * @return a boolean specifying if the two vectors are equal.
     */
    bool operator==(const Vec3& other);

    /**
     * A scalar addition operator.
     * @param rhs the scalar to add.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(+=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A scalar subtraction operator.
     * @param rhs the scalar to subtract.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(-=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A scalar multiplication operator.
     * @param rhs the scalar to multiply.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(*=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A scalar division operator.
     * @param rhs the scalar to divide.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(/=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A 3D vector const addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR(+, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A 3D vector const subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR(-, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A 3D vector const multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR(*, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A 3D vector const division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR(/, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    /**
     * A const scalar addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR(+, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR(-, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR(*, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR(/, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    // Static methods

    /**
     * Creates a Vec3 from a String.
     * @param vec a reference to a vector to populate
     * @param string the string to extract values from.
     * @return a boolean specifying if the operation was successful.
     */
    static bool FromString(OUT Vec3& vec, const String& string);

    // Public methods

    /**
     * Returns a string representation of the vector.
     * @return the string result of the operation.
     */
    String ToString() const;

    /**
     * Normalises the vector elements to range between 0 and 1.
     * @return
     */
    Vec3 Normalise() const;

    /**
     * Computes the dot product of the vector with another vector.
     * @param other the vector to get the dot product with.
     * @return the dot product of the two vectors.
     */
    float Dot(const Vec3& other) const;

    /**
     * Computes the cross product of the vector with another vector.
     * @param other the vector to get the cross product with.
     * @return the cross product of the two vectors.
     */
    Vec3 Cross(const Vec3& other) const;

    /**
     * Computes the length of the vector.
     * @return the length of the vector.
     */
    float Length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    /**
     * Returns a vector with the x dimension filled in and the rest set to 0.
     * @return a vector with the vector's x dimension.
     */
    Vec3 XComp() const
    {
        return {x, 0.f, 0.f};
    }

    /**
     * Returns a vector with the y dimension filled in and the rest set to 0.
     * @return a vector with the vector's y dimension.
     */
    Vec3 YComp() const
    {
        return {0.f, y, 0.f};
    }

    /**
     * Returns a vector with the z dimension filled in and the rest set to 0.
     * @return a vector with the vector's z dimension.
     */
    Vec3 ZComp() const
    {
        return {0.f, 0.f, z};
    }

    // Public members

    float x;
    float y;
    float z;
};

// Binary operators

inline Vec3 operator/(const float& scalar, const Vec3& vec)
{
    return {scalar / vec.x, scalar / vec.y, scalar / vec.z};
}

inline Vec3 operator*(const float& scalar, const Vec3& vec)
{
    return {scalar * vec.x, scalar * vec.y, scalar * vec.z};
}

inline bool operator==(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

} // namespace Siege

#endif // SIEGE_ENGINE_VEC3_H
