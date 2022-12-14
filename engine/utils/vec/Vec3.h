//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC3_H
#define SIEGE_ENGINE_VEC3_H

#include "Operators.h"

namespace Siege
{
/**
 * Struct representing a 3 dimensional vector.
 */
struct Vec3
{
    // Public Constants

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
     * A default Vec3 constructor. Initiates all values to 0
     */
    Vec3() = default;

    /**
     * Base Vector constructor.
     * @param x the value to be given to the x dimension (default is zero)
     * @param y the value to be given to the y dimension (default is zero)
     * @param z the value to be given to the z dimension (default is zero)
     */
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    /**
     * A 3D vector constructor from a 2D vector.
     * @param other a 2 dimensional vector to copy.
     */
    explicit Vec3(const struct Vec2& other);

    /**
     * A 3D vector constructor from a 4 dimensional vector
     * @param other a 4 dimensional vector to copy.
     */
    explicit Vec3(const struct Vec4& other);

    // Operator overloads

    /**
     * Default assignment operator
     * @param rhs variable to assign values from
     * @return a reference to the current object
     */
    Vec3& operator=(const Vec3& rhs) = default;

    /**
     * Assignment operator from a 2D vector
     * @param rhs variable to assign values from
     * @return a reference to the current object
     */
    Vec3& operator=(const Vec2& rhs);

    /**
     * Assignment operator from a 4D vector
     * @param rhs a 4D vector to assign values from
     * @return a reference to the current object
     */
    Vec3& operator=(const Vec4& rhs);

    /**
     * A Vec2 conversion operator. Converts a Vec3 to a Vec2
     * @return a new Vec3 with the Vec3's x and y values
     */
    operator Vec2() const;

    /**
     * A Vec4 conversion operator. Converts a Vec3 to a Vec4
     * @return a new Vec4 with the Vec3's x, y and z values
     */
    operator Vec4() const;

    // Static methods

    /**
     * Creates a Vec3 from a String.
     * @param vec a reference to a vector to populate
     * @param string the string to extract values from.
     * @return a boolean specifying if the operation was successful.
     */
    static bool FromString(OUT Vec3& vec, const String& string);

    /**
     * Normalises a target vector between 0 and 1 for each element
     * @param vec the vector to be normalised
     * @return a new vector with the normalised vector coordinates
     */
    static Vec3 Normalise(const Vec3& vec);

    /**
     * Calculates the length of the vector
     * @param vec the vector who's length is to be calculated
     * @return the length of the vector in the form of a float
     */
    static float Length(const Vec3& vec);

    /**
     * Calculates the dot product of two vectors
     * @param lhs the left hand vector
     * @param rhs the right hand vector
     * @return the dot product of both vectors
     */
    static float Dot(const Vec3& lhs, const Vec3& rhs);

    /**
     * Calculates the Cross product of two vectors
     * @param lhs the left hand vector
     * @param rhs the right hand vector
     * @return the cross product of the two vectors
     */
    static Vec3 Cross(const Vec3& lhs, const Vec3& rhs);

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
    float Length() const;

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

    float x {0.f}, y {0.f}, z {0.f};
};
} // namespace Siege

#endif // SIEGE_ENGINE_VEC3_H
