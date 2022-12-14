//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#ifndef SIEGE_ENGINE_VEC4_H
#define SIEGE_ENGINE_VEC4_H

#include "Operators.h"

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

    // 'Structors

    Vec4() = default;

    /**
     * Base Vector constructor.
     * @param x the value to be given to the x dimension (default is zero)
     * @param y the value to be given to the y dimension (default is zero)
     * @param z the value to be given to the z dimension (default is zero)
     * @param 2 the value to be given to the 2 dimension (default is zero)
     */
    Vec4(float x, float y, float z, float w) : x {x}, y {y}, z {z}, w {w} {}

    /**
     * A constructor from a 2D vector.
     * @param other the 2D vector to copy elements from.
     */
    explicit Vec4(const struct Vec2& other);

    /**
     * A constructor from a 3D vector.
     * @param other the 3D vector to copy elements from.
     */
    explicit Vec4(const struct Vec3& other);

    /**
     * An assignment operator from a 2D vector
     * @param rhs the Vec2 to assign values from
     * @return a reference to the current object
     */
    Vec4& operator=(const Vec2& rhs);

    /**
     * An assignment operator from a 3D vector
     * @param rhs the Vec3 to assign values from
     * @return a reference to the current object
     */
    Vec4& operator=(const Vec3& rhs);

    /**
     * An assignment operator from a 4D vector
     * @param rhs the Vec4 to assign values from
     * @return a reference to the current object
     */
    Vec4& operator=(const Vec4& rhs);

    /**
     * A conversion operator to a 2D vector
     * @return A 2D vector with the current vector's x and y values
     */
    operator Vec2() const;

    /**
     * A conversion operator to a 3D vector
     * @return A 3D vector with the current vector's x, y, and z values
     */
    operator Vec3() const;

    // Static Methods

    /**
     * Creates a Vec4 from a String.
     * @param vec a reference to a vector to populate
     * @param string the string to extract values from.
     * @return a boolean specifying if the operation was successful.
     */
    static bool FromString(OUT Vec4& vec, const String& string);

    /**
     * Normalises a target vector between 0 and 1 for each element
     * @param vec the vector to be normalised
     * @return a new vector with the normalised vector coordinates
     */
    static Vec4 Normalise(const Vec4& vec);

    /**
     * Calculates the length of the vector
     * @param vec the vector who's length is to be calculated
     * @return the length of the vector in the form of a float
     */
    static float Length(const Vec4 vec);

    /**
     * Calculates the dot product of two vectors
     * @param lhs the left hand vector
     * @param rhs the right hand vector
     * @return the dot product of both vectors
     */
    static float Dot(const Vec4& lhs, const Vec4& rhs);

    // Public Methods

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
        return {x, 0.f, 0.f, 0.f};
    }

    /**
     * Returns a vector with the y dimension filled in and the rest set to 0.
     * @return a vector with the vector's y dimension.
     */
    Vec4 YComp() const
    {
        return {0.f, y, 0.f, 0.f};
    }

    /**
     * Returns a vector with the z dimension filled in and the rest set to 0.
     * @return a vector with the vector's z dimension.
     */
    Vec4 ZComp() const
    {
        return {0.f, 0.f, z, 0.f};
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
    float Length() const;

    /**
     * Normalises the vector elements to range between 0 and 1.
     * @return
     */
    Vec4 Normalise() const;

    /**
     * Computes the dot product of the vector with another vector.
     * @param other the vector to get the dot product with.
     * @return the dot product of the two vectors.
     */
    float Dot(const Vec4& other) const;

    // Member variables

    float x {0.f}, y {0.f}, z {0.f}, w {0.f};
};
} // namespace Siege

#endif // SIEGE_ENGINE_VEC4_H
