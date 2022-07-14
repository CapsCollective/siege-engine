//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC2_H
#define SIEGE_ENGINE_VEC2_H

#include <cmath>

#include "../String.h"
#include "Macros.h"
#include "Vec.h"

#define VEC2_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep

#define VEC2_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep

namespace Siege
{
/**
 * Struct representing a 2 dimensional vector.
 */
struct Vec2
{
    // Public constants

    /**
     * A vector in which all elements are 0.
     */
    static const Vec2 Zero;

    /**
     * A vector in which all elements are instantiated to 1.
     */
    static const Vec2 One;

    /**
     * A vector in which all elements except for the y value are 0.
     */
    static const Vec2 Up;

    // 'Structors

    /**
     * Base Vector constructor.
     * @param x the value to be given to the x dimension (default is zero)
     * @param y the value to be given to the y dimension (default is zero)
     */
    Vec2(const float& x = 0.f, const float& y = 0.f) : x(x), y(y) {}

    /**
     * A 3D vector constructor. Takes the x and y components from the other vector and copies them
     * into our vector.
     * @param other a 3 dimensional vector to copy.
     */
    Vec2(const Vec3& other);

    /**
     * A 4D vector constructor. Takes the x and y components from the other vector and copies them
     * into our vector.
     * @param other a 4 dimensional vector to copy.
     */
    Vec2(const Vec4& other);

    // Operator overloads

    /**
     * Default assignment operator.
     * @param rhs variable to assign values from
     * @return a reference to the current object.
     */
    Vec2& operator=(const Vec2& rhs) = default;

    /**
     * A two dimensional Vector Addition operator.
     * @param rhs the 2D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(+=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A two dimensional Vector subtraction operator.
     * @param rhs the 2D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(-=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A two dimensional Vector multiplication operator.
     * @param rhs the 2D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(*=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A two dimensional Vector division operator.
     * @param rhs the 2D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(/=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A 2D - 3D Vector addition operator.
     * @param rhs the 3D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(+=, Vec2, Vec3)

    /**
     * A 2D - 3D Vector subtraction operator.
     * @param rhs the 3D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(-=, Vec2, Vec3)

    /**
     * A 2D - 3D Vector multiplication operator.
     * @param rhs the 3D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(*=, Vec2, Vec3)

    /**
     * A 2D - 3D Vector division operator.
     * @param rhs the 3D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(/=, Vec2, Vec3)

    /**
     * A 2D - 4D Vector addition operator.
     * @param rhs the 4D vector to be added by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(+=, Vec2, Vec4)

    /**
     * A 2D - 4D Vector subtraction operator.
     * @param rhs the 4D vector to be subtracted by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(-=, Vec2, Vec4)

    /**
     * A 2D - 4D Vector multiplication operator.
     * @param rhs the 4D vector to be multiplied by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(*=, Vec2, Vec4)

    /**
     * A 2D - 4D Vector division operator.
     * @param rhs the 4D vector to be divided by.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR_NO_IMP(/=, Vec2, Vec4)

    /**
     * An equality operator.
     * @param other the vector to be compared.
     * @return a boolean specifying if the two vectors are equal.
     */
    bool operator==(const Vec2& other);

    /**
     * An inequality operator.
     * @param other the vector to be compared.
     * @return a boolean specifying if the two vectors are not equal.
     */
    bool operator!=(const Vec2& other);

    /**
     * A scalar addition operator.
     * @param rhs the scalar to add.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(+=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A scalar subtraction operator.
     * @param rhs the scalar to subtract.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(-=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A scalar multiplication operator.
     * @param rhs the scalar to multiply.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(*=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A scalar divide operator.
     * @param rhs the scalar to divide.
     * @return a reference to the current vector.
     */
    DEFINE_OPERATOR(/=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A 2D vector const addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR(+, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A 2D vector const subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR(-, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A 2D vector const multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR(*, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A 2D vector const division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR(/, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    /**
     * A 2D - 3D vector const addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec2, Vec3)

    /**
     * A 2D - 3D vector const subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec2, Vec3)

    /**
     * A 2D - 3D vector const multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec2, Vec3)

    /**
     * A 2D - 3D vector const division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec2, Vec3)

    /**
     * A 2D - 4D vector const addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec2, Vec4)

    /**
     * A 2D - 4D vector const subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec2, Vec4)

    /**
     * A 2D - 4D vector const multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec2, Vec4)

    /**
     * A 2D - 4D vector const division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec2, Vec4)

    /**
     * A const scalar addition operator.
     * @param rhs the vector to add by.
     * @return a new vector representing the result of the vector addition.
     */
    DEFINE_CONST_OPERATOR(+, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar subtraction operator.
     * @param rhs the vector to subtract by.
     * @return a new vector representing the result of the vector subtraction.
     */
    DEFINE_CONST_OPERATOR(-, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar multiplication operator.
     * @param rhs the vector to multiply by.
     * @return a new vector representing the result of the vector multiplication.
     */
    DEFINE_CONST_OPERATOR(*, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    /**
     * A const scalar division operator.
     * @param rhs the vector to divide by.
     * @return a new vector representing the result of the vector division.
     */
    DEFINE_CONST_OPERATOR(/, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    // Static methods

    /**
     * Creates a Vec2 from a String.
     * @param vec a reference to a vector to populate
     * @param string the string to extract values from.
     * @return a boolean specifying if the operation was successful.
     */
    static bool FromString(OUT Vec2& vec, const String& string);

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
    Vec2 Normalise() const;

    /**
     * Computes the length of the vector.
     * @return the length of the vector.
     */
    float Length() const
    {
        return sqrtf(x * x + y * y);
    }

    /**
     * Returns a vector with the x dimension filled in and the rest set to 0.
     * @return a vector with the vector's x dimension.
     */
    Vec2 XComp() const
    {
        return {
            x,
            0.f,
        };
    }

    /**
     * Returns a vector with the y dimension filled in and the rest set to 0.
     * @return a vector with the vector's y dimension.
     */
    Vec2 YComp() const
    {
        return {0.f, y};
    }

    float x, y;
};

// Binary Operators

inline Vec2 operator/(const float& scalar, const Vec2& vec)
{
    return {scalar / vec.x, scalar / vec.y};
}

inline Vec2 operator*(const float& scalar, const Vec2& vec)
{
    return {scalar * vec.x, scalar * vec.y};
}

inline bool operator==(const Vec2& lhs, const Vec2& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
} // namespace Siege

#endif // SIEGE_ENGINE_VEC2_H
