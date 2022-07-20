//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC2_H
#define SIEGE_ENGINE_VEC2_H

#include "../../String.h"
#include "Operators.h"

namespace Siege
{
/**
 * Struct representing a 2 dimensional vector
 */
struct Vec2
{
    // Public constants

    /**
     * A vector in which all elements are 0
     */
    static const Vec2 Zero;

    /**
     * A vector in which all elements are instantiated to 1
     */
    static const Vec2 One;

    /**
     * A vector in which all elements except for the y value are 0
     */
    static const Vec2 Up;

    // 'Structors

    /**
     * A default Vec2 constructor. Initialises all values to Zero
     */
    Vec2() = default;

    /**
     * Base Vector constructor.
     * @param x the value to be given to the x dimension (default is zero)
     * @param y the value to be given to the y dimension (default is zero)
     */
    Vec2(float x, float y) : x(x), y(y) {}

    /**
     * A 3D vector constructor. Takes the x and y components from the other vector and copies them
     * into our vector.
     * @param other a 3 dimensional vector to copy.
     */
    explicit Vec2(const struct Vec3& other);

    /**
     * A 4D vector constructor. Takes the x and y components from the other vector and copies them
     * into our vector.
     * @param other a 4 dimensional vector to copy.
     */
    explicit Vec2(const struct Vec4& other);

    // Static functions

    /**
     * Lerps between two vectors
     * @param origin the start vector.
     * @param destination the end result
     * @param time the interpolation point.
     * @return an interpolated vector between origin and destination.
     */
    static Vec2 Lerp(Vec2 origin, Vec2 destination, float time);

    // Operator overloads

    /**
     * A Vecw assignment operator
     * @param rhs a Vec2 to assign data from
     * @return a reference to the current Vec2 object
     */
    Vec2& operator=(const Vec2& rhs) = default;

    /**
     * A Vec3 assignment operator
     * @param rhs a Vec3 to assign data from
     * @return a reference to the current Vec2 object
     */
    Vec2& operator=(const Vec3& rhs);

    /**
     * A Vec4 assignment operator
     * @param rhs a Vec4 to assign data from
     * @return a reference to the current Vec2 object
     */
    Vec2& operator=(const Vec4& rhs);

    /**
     * A Vec3 conversion operator. Converts a Vec2 to a Vec3
     * @return a new Vec3 with the Vec2's x and y values
     */
    operator Vec3() const;

    /**
     * A Vec4 conversion operator. Converts a Vec2 to a Vec3
     * @return a new Vec3 with the Vec2's x and y values
     */
    operator Vec4() const;

    // Static methods

    /**
     * Creates a Vec2 from a String.
     * @param vec a reference to a vector to populate
     * @param string the string to extract values from.
     * @return a boolean specifying if the operation was successful.
     */
    static bool FromString(OUT Vec2& vec, const String& string);

    /**
     * Normalises a target vector between 0 and 1 for each element
     * @param vec the vector to be normalised
     * @return a new vector with the normalised vector coordinates
     */
    static Vec2 Normalise(const Vec2& vec);

    /**
     * Calculates the length of the vector
     * @param vec the vector who's length is to be calculated
     * @return the length of the vector in the form of a float
     */
    static float Length(const Vec2& vec);

    /**
     * Calculates the dot product of two vectors
     * @param lhs the left hand vector
     * @param rhs the right hand vector
     * @return the dot product of both vectors
     */
    static float Dot(const Vec2& lhs, const Vec2& rhs);

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
    float Length() const;

    /**
     * Computes the dot product of the vector with another vector.
     * @param other the vector to get the dot product with.
     * @return the dot product of the two vectors.
     */
    float Dot(const Vec2& other) const;

    /**
     * Returns a vector with the x dimension filled in and the rest set to 0.
     * @return a vector with the vector's x dimension.
     */
    Vec2 XComp() const
    {
        return {x, 0.f};
    }

    /**
     * Returns a vector with the y dimension filled in and the rest set to 0.
     * @return a vector with the vector's y dimension.
     */
    Vec2 YComp() const
    {
        return {0.f, y};
    }

    // Member variables (the x and y components)

    float x {0.f}, y {0.f};
};
} // namespace Siege

#endif // SIEGE_ENGINE_VEC2_H
