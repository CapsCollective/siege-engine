//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC3_H
#define SIEGE_ENGINE_VEC3_H

#include "Vec.h"

namespace Siege
{
template<typename T>
struct Vec<T, 3>
{
    /**
     * @returns a new 3D vector with all elements set to zero
     */
    static inline constexpr Vec<T, 3> Zero()
    {
        return Vec<T, 3> {};
    }

    /**
     * @returns a new 3D vector with all elements set to one
     */
    static inline constexpr Vec<T, 3> One()
    {
        return {1.f, 1.f, 1.f};
    }

    /**
     * @returns a new 3D vector which denotes the world forward direction
     */
    static inline constexpr Vec<T, 3> Forward()
    {
        return Vec<T, 3> {0, 0, 1};
    }

    /**
     * @returns a new 3D vector which denotes the world right direction
     */
    static inline constexpr Vec<T, 3> Right()
    {
        return Vec<T, 3> {1, 0, 0};
    }

    /**
     * @returns a new 3D vector which denotes the world up direction
     */
    static inline constexpr Vec<T, 3> Up()
    {
        return Vec<T, 3> {0, -1, 0};
    }

    /**
     * Adds two 3D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector addition
     */
    static inline constexpr Vec<T, 3> Add(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
    }

    /**
     * Adds a scalar value to a 3D vector and returns the result
     * @param vector the 3D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar addition
     */
    static inline constexpr Vec<T, 3> Add(const Vec<T, 3>& vector, T scalar)
    {
        return {vector.x + scalar, vector.y + scalar, vector.z + scalar};
    }

    /**
     * Subtracts two 3D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector subtraction
     */
    static inline constexpr Vec<T, 3> Subtract(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }

    /**
     * Subtracts a scalar value to a 3D vector and returns the result
     * @param vector the 3D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar subtraction
     */
    static inline constexpr Vec<T, 3> Subtract(const Vec<T, 3>& vector, T scalar)
    {
        return {vector.x - scalar, vector.y - scalar, vector.z - scalar};
    }

    /**
     * Multiplies a 3D vector by a scalar value and returns the result
     * @param vector the 3D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar multiplication
     */
    static inline constexpr Vec<T, 3> Multiply(const Vec<T, 3>& vector, T scalar)
    {
        return {vector.x * scalar, vector.y * scalar, vector.z * scalar};
    }

    /**
     * Multiplies two 3D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector multiplication
     */
    static inline constexpr Vec<T, 3> Multiply(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
    {
        return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
    }

    /**
     * Divides two 3D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector division
     */
    static inline constexpr Vec<T, 3> Divide(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
    {
        return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
    }

    /**
     * Divides a 3D vector by a scalar value and returns the result
     * @param vector the 3D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar division
     */
    static inline constexpr Vec<T, 3> Divide(const Vec<T, 3>& lhs, T scalar)
    {
        return {lhs.x / scalar, lhs.y / scalar, lhs.z / scalar};
    }

    /**
     * Computes the Dot product of two 3D vectors
     * @param lhs the vector on the left hand of the operation
     * @param rhs the vector on the right hand of the operation
     * @return the dot product of the two inputted vectors
     */
    static inline constexpr float Dot(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
    {
        return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
    }

    /**
     * Computes the length of a 3D vector
     * @param vector the vector to be calculated
     * @return the length of the inputted vector
     */
    static inline constexpr float Length(const Vec<T, 3>& vector)
    {
        return Float::FastSqrt((vector.x * vector.x) + (vector.y * vector.y) +
                               (vector.z * vector.z));
    }

    /**
     * Normalises a 3D vector so that all values range between 0 and 1
     * @param vector the vector to be normalised
     * @return a new 3D vector with the normalised values
     */
    static inline constexpr Vec<T, 3> Normalise(const Vec<T, 3>& vector)
    {
        T length = Length(vector);
        return length == 0.f ? Zero() : vector / length;
    }

    /**
     * Computes the cross product of two vectors
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @return a new vector with the results of the cross product computation
     */
    static inline constexpr Vec<T, 3> Cross(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
    {
        return {lhs.y * rhs.z - rhs.y * lhs.z,
                lhs.z * rhs.x - rhs.z * lhs.x,
                lhs.x * rhs.y - rhs.x * lhs.y};
    }

    /**
     * Computes the cos of every element of a provided 3D vector
     * @param vec the vector to be computed
     * @return a new vector with the cos of each value
     */
    static inline constexpr Vec<T, 3> Cos(const Vec<T, 3>& vec)
    {
        return {Float::Cos(vec.x), Float::Cos(vec.y), Float::Cos(vec.z)};
    }

    /**
     * Computes the sin of every element of a provided 3D vector
     * @param vec the vector to be computed
     * @return a new vector with the sin of each value
     */
    static inline constexpr Vec<T, 3> Sin(const Vec<T, 3>& vec)
    {
        return {Float::Sin(vec.x), Float::Sin(vec.y), Float::Sin(vec.z)};
    }

    /**
     * Computes the tan of every element of a provided 3D vector
     * @param vec the vector to be computed
     * @return a new vector with the tan of each value
     */
    static inline constexpr Vec<T, 3> Tan(const Vec<T, 3>& vec)
    {
        return {Float::Tan(vec.x), Float::Tan(vec.y), Float::Tan(vec.z)};
    }

    /**
     * Linearly interpolates between two 3D vectors
     * @param origin the vector to begin from
     * @param destination the vector end-result
     * @param time the proportion of movement to be done from origin to destination
     * @return a new vector with the results of the linear interpolation
     */
    static inline constexpr Vec<T, 3> Lerp(const Vec<T, 3>& origin,
                                           const Vec<T, 3>& destination,
                                           T time)
    {
        return origin * (1.0f - time) + destination * time;
    }

    // 'Structors

    /**
     * The Vec<T, 3> default constructor
     */
    inline constexpr Vec<T, 3>() = default;

    /**
     * The base Vec<T, 3> constructor. Accepts four values
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     */
    inline constexpr Vec<T, 3>(T x, T y, T z) : x {x}, y {y}, z {z} {}

    /**
     * The two-element partial constructor for the Vec<T, 3>
     * @param x the value for the x axis
     * @param y the value for the y axis
     */
    inline constexpr Vec<T, 3>(T x, T y) : Vec<T, 3>(x, y, 0) {}

    /**
     * The one-element partial constructor for the Vec<T, 3>
     * @param x the value for the x axis
     */
    inline constexpr Vec<T, 3>(T x) : Vec<T, 3>(x, 0, 0) {}

    /**
     * The 4D vector constructor. Constructs a 3D vector using the x, y, and z element of the
     * inputted 4D vector
     * @param other a 4D vector of the same type to convert from
     */
    inline constexpr Vec<T, 3>(const Vec<T, 4>& other) : Vec<T, 3>(other.x, other.y, other.z) {}

    /**
     * The 2D vector constructor. Constructs a 3D vector from a 2D vector. Sets the z element to 0
     * @param other the 2D vector to convert from
     */
    inline constexpr Vec<T, 3>(const Vec<T, 2>& other) : Vec<T, 3>(other.x, other.y, 0) {}

    /**
     * The partial 2D vector constructor. Constructs a 3D vector from a 2D vector
     * @param other the 2D vector to convert from
     * @param val the z axis value
     */
    inline constexpr Vec<T, 3>(const Vec<T, 2>& other, T val) : Vec<T, 3>(other.x, other.y, val) {}

    // Operator Overloads

    /**
     * The 3D vector subscript operator
     * @param index the index of the requested dimension
     * @return a reference to the requested value
     */
    inline constexpr T& operator[](unsigned int index)
    {
        switch (index)
        {
            case (1):
                return y;
            case (2):
                return z;
            case (0):
            default:
                return x;
        }
    }

    /**
     * The const 3D vector subscript operator
     * @param index the index of the requested dimension
     * @return a copy of the requested value
     */
    inline constexpr T operator[](unsigned int index) const
    {
        return ((index == 0) * x) + ((index == 1) * y) + ((index == 2) * z);
    }

    /**
     * The equality operator. Checks if two 3D vectors are equal on all axes
     * @param rhs the 3D vector to compare
     * @return a boolean specifying if the vectors are equal
     */
    inline constexpr bool operator==(const Vec<T, 3>& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    /**
     * The inequality operator. Checks if two 3D vectors are not equal on all axes
     * @param rhs the 3D vector to compare
     * @return a boolean specifying if the vectors are not equal
     */
    inline constexpr bool operator!=(const Vec<T, 3>& rhs) const
    {
        return x != rhs.x && y != rhs.y && z != rhs.z;
    }

    /**
     * The scalar addition operator. Adds the provided scalar value to the 3D vector
     * @param scalar the scalar value to add
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator+=(T scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    /**
     * The addition operator. Adds the values of a 3D vector to the current vector
     * @param rhs the vector to add values from
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator+=(const Vec<T, 3>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    /**
     * The scalar subtraction operator. Subtracts the provided scalar value from the 3D vector
     * @param scalar the scalar value to subtract
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator-=(T scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    /**
     * The subtraction operator. Subtracts the values of a 3D vector from the current vector
     * @param rhs the vector to subtract values from
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator-=(const Vec<T, 3>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    /**
     * The multiplication operator. Multiplies the values of a 3D vector by the current vector
     * @param rhs the vector to multiply values from
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator*=(const Vec<T, 3>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    /**
     * The scalar multiplication operator. Multiplies the provided scalar value from the 3D vector
     * @param scalar the scalar value to multiply by
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    /**
     * The division operator. Divides the values of the 3D vector by the provided vector
     * @param rhs the vector to divide values by
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator/=(const Vec<T, 3>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    /**
     * The scalar division operator. Divides the 3D vector by the provided scalar value
     * @param scalar the scalar value to divide by
     * @return a reference to the current Vec<T, 3>
     */
    inline constexpr Vec<T, 3>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    /**
     * The Negation operator. Negates the values in the vector
     * @return a new vector with the negated vector values
     */
    inline constexpr Vec<T, 3>& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    // Swizzle functions + getters

    /**
     * A swizzle function for the x and x components of the vector
     * @return a new 2D vector with the corresponding x and x components of the original vector
     */
    inline constexpr Vec<T, 2> XX() const
    {
        return {x, x};
    }

    /**
     * A swizzle function for the x and y components of the vector
     * @return a new 2D vector with the corresponding x and y components of the original vector
     */
    inline constexpr Vec<T, 2> XY() const
    {
        return {x, y};
    }

    /**
     * A swizzle function for the x and z components of the vector
     * @return a new 2D vector with the corresponding x and z components of the original vector
     */
    inline constexpr Vec<T, 2> XZ() const
    {
        return {x, z};
    }

    /**
     * A swizzle function for the y and x components of the vector
     * @return a new 2D vector with the corresponding y and x components of the original vector
     */
    inline constexpr Vec<T, 2> YX() const
    {
        return {y, x};
    }

    /**
     * A swizzle function for the y and y components of the vector
     * @return a new 2D vector with the corresponding y and y components of the original vector
     */
    inline constexpr Vec<T, 2> YY() const
    {
        return {y, y};
    }

    /**
     * A swizzle function for the y and z components of the vector
     * @return a new 2D vector with the corresponding y and z components of the original vector
     */
    inline constexpr Vec<T, 2> YZ() const
    {
        return {y, z};
    }

    /**
     * A swizzle function for the z and x components of the vector
     * @return a new 2D vector with the corresponding z and x components of the original vector
     */
    inline constexpr Vec<T, 2> ZX() const
    {
        return {z, x};
    }

    /**
     * A swizzle function for the z and y components of the vector
     * @return a new 2D vector with the corresponding z and y components of the original vector
     */
    inline constexpr Vec<T, 2> ZY() const
    {
        return {z, y};
    }

    /**
     * A swizzle function for the z and z components of the vector
     * @return a new 2D vector with the corresponding z and z components of the original vector
     */
    inline constexpr Vec<T, 2> ZZ() const
    {
        return {z, z};
    }

    /**
     * A swizzle function for the x, x, and x components of the vector
     * @return a new 3D vector with the corresponding x, x, and x components of the original vector
     */
    inline constexpr Vec<T, 3> XXX() const
    {
        return {x, x, x};
    }

    /**
     * A swizzle function for the x, x, and y components of the vector
     * @return a new 3D vector with the corresponding x, x, and y components of the original vector
     */
    inline constexpr Vec<T, 3> XXY() const
    {
        return {x, x, y};
    }

    /**
     * A swizzle function for the x, x, and z components of the vector
     * @return a new 3D vector with the corresponding x, x, and z components of the original vector
     */
    inline constexpr Vec<T, 3> XXZ() const
    {
        return {x, x, z};
    }

    /**
     * A swizzle function for the x, y, and x components of the vector
     * @return a new 3D vector with the corresponding x, y, and x components of the original vector
     */
    inline constexpr Vec<T, 3> XYX() const
    {
        return {x, y, x};
    }

    /**
     * A swizzle function for the x, y, and y components of the vector
     * @return a new 3D vector with the corresponding x, y, and y components of the original vector
     */
    inline constexpr Vec<T, 3> XYY() const
    {
        return {x, y, y};
    }

    /**
     * A swizzle function for the x, y, and z components of the vector
     * @return a new 3D vector with the corresponding x, y, and z components of the original vector
     */
    inline constexpr Vec<T, 3> XYZ() const
    {
        return {x, y, z};
    }

    /**
     * A swizzle function for the x, z, and x components of the vector
     * @return a new 3D vector with the corresponding x, z, and x components of the original vector
     */
    inline constexpr Vec<T, 3> XZX() const
    {
        return {x, z, x};
    }

    /**
     * A swizzle function for the x, z, and y components of the vector
     * @return a new 3D vector with the corresponding x, z, and y components of the original vector
     */
    inline constexpr Vec<T, 3> XZY() const
    {
        return {x, z, y};
    }

    /**
     * A swizzle function for the x, z, and z components of the vector
     * @return a new 3D vector with the corresponding x, z, and z components of the original vector
     */
    inline constexpr Vec<T, 3> XZZ() const
    {
        return {x, z, z};
    }

    /**
     * A swizzle function for the y, x, and x components of the vector
     * @return a new 3D vector with the corresponding y, x, and x components of the original vector
     */
    inline constexpr Vec<T, 3> YXX() const
    {
        return {y, x, x};
    }

    /**
     * A swizzle function for the y, x, and y components of the vector
     * @return a new 3D vector with the corresponding y, x, and y components of the original vector
     */
    inline constexpr Vec<T, 3> YXY() const
    {
        return {y, x, y};
    }

    /**
     * A swizzle function for the y, x, and z components of the vector
     * @return a new 3D vector with the corresponding y, x, and z components of the original vector
     */
    inline constexpr Vec<T, 3> YXZ() const
    {
        return {y, x, z};
    }

    /**
     * A swizzle function for the y, y, and x components of the vector
     * @return a new 3D vector with the corresponding y, y, and x components of the original vector
     */
    inline constexpr Vec<T, 3> YYX() const
    {
        return {y, y, x};
    }

    /**
     * A swizzle function for the y, y, and y components of the vector
     * @return a new 3D vector with the corresponding y, y, and y components of the original vector
     */
    inline constexpr Vec<T, 3> YYY() const
    {
        return {y, y, y};
    }

    /**
     * A swizzle function for the y, y, and z components of the vector
     * @return a new 3D vector with the corresponding y, y, and z components of the original vector
     */
    inline constexpr Vec<T, 3> YYZ() const
    {
        return {y, y, z};
    }

    /**
     * A swizzle function for the y, z, and x components of the vector
     * @return a new 3D vector with the corresponding y, z, and x components of the original vector
     */
    inline constexpr Vec<T, 3> YZX() const
    {
        return {y, z, x};
    }

    /**
     * A swizzle function for the y, z, and y components of the vector
     * @return a new 3D vector with the corresponding y, z, and y components of the original vector
     */
    inline constexpr Vec<T, 3> YZY() const
    {
        return {y, z, y};
    }

    /**
     * A swizzle function for the y, z, and z components of the vector
     * @return a new 3D vector with the corresponding y, z, and z components of the original vector
     */
    inline constexpr Vec<T, 3> YZZ() const
    {
        return {y, z, z};
    }

    /**
     * A swizzle function for the z, x, and x components of the vector
     * @return a new 3D vector with the corresponding z, x, and x components of the original vector
     */
    inline constexpr Vec<T, 3> ZXX() const
    {
        return {z, x, x};
    }

    /**
     * A swizzle function for the z, x, and y components of the vector
     * @return a new 3D vector with the corresponding z, x, and y components of the original vector
     */
    inline constexpr Vec<T, 3> ZXY() const
    {
        return {z, x, y};
    }

    /**
     * A swizzle function for the z, x, and z components of the vector
     * @return a new 3D vector with the corresponding z, x, and z components of the original vector
     */
    inline constexpr Vec<T, 3> ZXZ() const
    {
        return {z, x, z};
    }

    /**
     * A swizzle function for the z, y, and x components of the vector
     * @return a new 3D vector with the corresponding z, y, and x components of the original vector
     */
    inline constexpr Vec<T, 3> ZYX() const
    {
        return {z, y, x};
    }

    /**
     * A swizzle function for the z, y, and y components of the vector
     * @return a new 3D vector with the corresponding z, y, and y components of the original vector
     */
    inline constexpr Vec<T, 3> ZYY() const
    {
        return {z, y, y};
    }

    /**
     * A swizzle function for the z, y, and z components of the vector
     * @return a new 3D vector with the corresponding z, y, and z components of the original vector
     */
    inline constexpr Vec<T, 3> ZYZ() const
    {
        return {z, y, z};
    }

    /**
     * A swizzle function for the z, z, and x components of the vector
     * @return a new 3D vector with the corresponding z, z, and x components of the original vector
     */
    inline constexpr Vec<T, 3> ZZX() const
    {
        return {z, z, x};
    }

    /**
     * A swizzle function for the z, z, and y components of the vector
     * @return a new 3D vector with the corresponding z, z, and y components of the original vector
     */
    inline constexpr Vec<T, 3> ZZY() const
    {
        return {z, z, y};
    }

    /**
     * A swizzle function for the z, z, and z components of the vector
     * @return a new 3D vector with the corresponding z, z, and z components of the original vector
     */
    inline constexpr Vec<T, 3> ZZZ() const
    {
        return {z, z, z};
    }

    /**
     * A swizzle function for the x, x, x, and x components of the vector
     * @return a new 4D vector with the corresponding x, x, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXXX() const
    {
        return {x, x, x, x};
    }

    /**
     * A swizzle function for the x, x, x, and y components of the vector
     * @return a new 4D vector with the corresponding x, x, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXXY() const
    {
        return {x, x, x, y};
    }

    /**
     * A swizzle function for the x, x, x, and z components of the vector
     * @return a new 4D vector with the corresponding x, x, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXXZ() const
    {
        return {x, x, x, z};
    }

    /**
     * A swizzle function for the x, x, y, and x components of the vector
     * @return a new 4D vector with the corresponding x, x, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXYX() const
    {
        return {x, x, y, x};
    }

    /**
     * A swizzle function for the x, x, y, and y components of the vector
     * @return a new 4D vector with the corresponding x, x, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXYY() const
    {
        return {x, x, y, y};
    }

    /**
     * A swizzle function for the x, x, y, and z components of the vector
     * @return a new 4D vector with the corresponding x, x, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXYZ() const
    {
        return {x, x, y, z};
    }

    /**
     * A swizzle function for the x, x, z, and x components of the vector
     * @return a new 4D vector with the corresponding x, x, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXZX() const
    {
        return {x, x, z, x};
    }

    /**
     * A swizzle function for the x, x, z, and y components of the vector
     * @return a new 4D vector with the corresponding x, x, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXZY() const
    {
        return {x, x, z, y};
    }

    /**
     * A swizzle function for the x, x, z, and z components of the vector
     * @return a new 4D vector with the corresponding x, x, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXZZ() const
    {
        return {x, x, z, z};
    }

    /**
     * A swizzle function for the x, y, x, and x components of the vector
     * @return a new 4D vector with the corresponding x, y, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYXX() const
    {
        return {x, y, x, x};
    }

    /**
     * A swizzle function for the x, y, x, and y components of the vector
     * @return a new 4D vector with the corresponding x, y, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYXY() const
    {
        return {x, y, x, y};
    }

    /**
     * A swizzle function for the x, y, x, and z components of the vector
     * @return a new 4D vector with the corresponding x, y, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYXZ() const
    {
        return {x, y, x, z};
    }

    /**
     * A swizzle function for the x, y, y, and x components of the vector
     * @return a new 4D vector with the corresponding x, y, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYYX() const
    {
        return {x, y, y, x};
    }

    /**
     * A swizzle function for the x, y, y, and y components of the vector
     * @return a new 4D vector with the corresponding x, y, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYYY() const
    {
        return {x, y, y, y};
    }

    /**
     * A swizzle function for the x, y, y, and z components of the vector
     * @return a new 4D vector with the corresponding x, y, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYYZ() const
    {
        return {x, y, y, z};
    }

    /**
     * A swizzle function for the x, y, z, and x components of the vector
     * @return a new 4D vector with the corresponding x, y, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYZX() const
    {
        return {x, y, z, x};
    }

    /**
     * A swizzle function for the x, y, z, and y components of the vector
     * @return a new 4D vector with the corresponding x, y, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYZY() const
    {
        return {x, y, z, y};
    }

    /**
     * A swizzle function for the x, y, z, and z components of the vector
     * @return a new 4D vector with the corresponding x, y, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYZZ() const
    {
        return {x, y, z, z};
    }

    /**
     * A swizzle function for the x, z, x, and x components of the vector
     * @return a new 4D vector with the corresponding x, z, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZXX() const
    {
        return {x, z, x, x};
    }

    /**
     * A swizzle function for the x, z, x, and y components of the vector
     * @return a new 4D vector with the corresponding x, z, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZXY() const
    {
        return {x, z, x, y};
    }

    /**
     * A swizzle function for the x, z, x, and z components of the vector
     * @return a new 4D vector with the corresponding x, z, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZXZ() const
    {
        return {x, z, x, z};
    }

    /**
     * A swizzle function for the x, z, y, and x components of the vector
     * @return a new 4D vector with the corresponding x, z, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZYX() const
    {
        return {x, z, y, x};
    }

    /**
     * A swizzle function for the x, z, y, and y components of the vector
     * @return a new 4D vector with the corresponding x, z, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZYY() const
    {
        return {x, z, y, y};
    }

    /**
     * A swizzle function for the x, z, y, and z components of the vector
     * @return a new 4D vector with the corresponding x, z, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZYZ() const
    {
        return {x, z, y, z};
    }

    /**
     * A swizzle function for the x, z, z, and x components of the vector
     * @return a new 4D vector with the corresponding x, z, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZZX() const
    {
        return {x, z, z, x};
    }

    /**
     * A swizzle function for the x, z, z, and y components of the vector
     * @return a new 4D vector with the corresponding x, z, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZZY() const
    {
        return {x, z, z, y};
    }

    /**
     * A swizzle function for the x, z, z, and z components of the vector
     * @return a new 4D vector with the corresponding x, z, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZZZ() const
    {
        return {x, z, z, z};
    }

    /**
     * A swizzle function for the y, x, x, and x components of the vector
     * @return a new 4D vector with the corresponding y, x, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXXX() const
    {
        return {y, x, x, x};
    }

    /**
     * A swizzle function for the y, x, x, and y components of the vector
     * @return a new 4D vector with the corresponding y, x, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXXY() const
    {
        return {y, x, x, y};
    }

    /**
     * A swizzle function for the y, x, x, and z components of the vector
     * @return a new 4D vector with the corresponding y, x, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXXZ() const
    {
        return {y, x, x, z};
    }

    /**
     * A swizzle function for the y, x, y, and x components of the vector
     * @return a new 4D vector with the corresponding y, x, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXYX() const
    {
        return {y, x, y, x};
    }

    /**
     * A swizzle function for the y, x, y, and y components of the vector
     * @return a new 4D vector with the corresponding y, x, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXYY() const
    {
        return {y, x, y, y};
    }

    /**
     * A swizzle function for the y, x, y, and z components of the vector
     * @return a new 4D vector with the corresponding y, x, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXYZ() const
    {
        return {y, x, y, z};
    }

    /**
     * A swizzle function for the y, x, z, and x components of the vector
     * @return a new 4D vector with the corresponding y, x, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXZX() const
    {
        return {y, x, z, x};
    }

    /**
     * A swizzle function for the y, x, z, and y components of the vector
     * @return a new 4D vector with the corresponding y, x, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXZY() const
    {
        return {y, x, z, y};
    }

    /**
     * A swizzle function for the y, x, z, and z components of the vector
     * @return a new 4D vector with the corresponding y, x, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXZZ() const
    {
        return {y, x, z, z};
    }

    /**
     * A swizzle function for the y, y, x, and x components of the vector
     * @return a new 4D vector with the corresponding y, y, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYXX() const
    {
        return {y, y, x, x};
    }

    /**
     * A swizzle function for the y, y, x, and y components of the vector
     * @return a new 4D vector with the corresponding y, y, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYXY() const
    {
        return {y, y, x, y};
    }

    /**
     * A swizzle function for the y, y, x, and z components of the vector
     * @return a new 4D vector with the corresponding y, y, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYXZ() const
    {
        return {y, y, x, z};
    }

    /**
     * A swizzle function for the y, y, y, and x components of the vector
     * @return a new 4D vector with the corresponding y, y, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYYX() const
    {
        return {y, y, y, x};
    }

    /**
     * A swizzle function for the y, y, y, and y components of the vector
     * @return a new 4D vector with the corresponding y, y, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYYY() const
    {
        return {y, y, y, y};
    }

    /**
     * A swizzle function for the y, y, y, and z components of the vector
     * @return a new 4D vector with the corresponding y, y, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYYZ() const
    {
        return {y, y, y, z};
    }

    /**
     * A swizzle function for the y, y, z, and x components of the vector
     * @return a new 4D vector with the corresponding y, y, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYZX() const
    {
        return {y, y, z, x};
    }

    /**
     * A swizzle function for the y, y, z, and y components of the vector
     * @return a new 4D vector with the corresponding y, y, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYZY() const
    {
        return {y, y, z, y};
    }

    /**
     * A swizzle function for the y, y, z, and z components of the vector
     * @return a new 4D vector with the corresponding y, y, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYZZ() const
    {
        return {y, y, z, z};
    }

    /**
     * A swizzle function for the y, z, x, and x components of the vector
     * @return a new 4D vector with the corresponding y, z, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZXX() const
    {
        return {y, z, x, x};
    }

    /**
     * A swizzle function for the y, z, x, and y components of the vector
     * @return a new 4D vector with the corresponding y, z, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZXY() const
    {
        return {y, z, x, y};
    }

    /**
     * A swizzle function for the y, z, x, and z components of the vector
     * @return a new 4D vector with the corresponding y, z, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZXZ() const
    {
        return {y, z, x, z};
    }

    /**
     * A swizzle function for the y, z, y, and x components of the vector
     * @return a new 4D vector with the corresponding y, z, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZYX() const
    {
        return {y, z, y, x};
    }

    /**
     * A swizzle function for the y, z, y, and y components of the vector
     * @return a new 4D vector with the corresponding y, z, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZYY() const
    {
        return {y, z, y, y};
    }

    /**
     * A swizzle function for the y, z, y, and z components of the vector
     * @return a new 4D vector with the corresponding y, z, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZYZ() const
    {
        return {y, z, y, z};
    }

    /**
     * A swizzle function for the y, z, z, and x components of the vector
     * @return a new 4D vector with the corresponding y, z, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZZX() const
    {
        return {y, z, z, x};
    }

    /**
     * A swizzle function for the y, z, z, and y components of the vector
     * @return a new 4D vector with the corresponding y, z, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZZY() const
    {
        return {y, z, z, y};
    }

    /**
     * A swizzle function for the y, z, z, and z components of the vector
     * @return a new 4D vector with the corresponding y, z, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZZZ() const
    {
        return {y, z, z, z};
    }

    /**
     * A swizzle function for the z, x, x, and x components of the vector
     * @return a new 4D vector with the corresponding z, x, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXXX() const
    {
        return {z, x, x, x};
    }

    /**
     * A swizzle function for the z, x, x, and y components of the vector
     * @return a new 4D vector with the corresponding z, x, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXXY() const
    {
        return {z, x, x, y};
    }

    /**
     * A swizzle function for the z, x, x, and z components of the vector
     * @return a new 4D vector with the corresponding z, x, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXXZ() const
    {
        return {z, x, x, z};
    }

    /**
     * A swizzle function for the z, x, y, and x components of the vector
     * @return a new 4D vector with the corresponding z, x, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXYX() const
    {
        return {z, x, y, x};
    }

    /**
     * A swizzle function for the z, x, y, and y components of the vector
     * @return a new 4D vector with the corresponding z, x, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXYY() const
    {
        return {z, x, y, y};
    }

    /**
     * A swizzle function for the z, x, y, and z components of the vector
     * @return a new 4D vector with the corresponding z, x, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXYZ() const
    {
        return {z, x, y, z};
    }

    /**
     * A swizzle function for the z, x, z, and x components of the vector
     * @return a new 4D vector with the corresponding z, x, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXZX() const
    {
        return {z, x, z, x};
    }

    /**
     * A swizzle function for the z, x, z, and y components of the vector
     * @return a new 4D vector with the corresponding z, x, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXZY() const
    {
        return {z, x, z, y};
    }

    /**
     * A swizzle function for the z, x, z, and z components of the vector
     * @return a new 4D vector with the corresponding z, x, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXZZ() const
    {
        return {z, x, z, z};
    }

    /**
     * A swizzle function for the z, y, x, and x components of the vector
     * @return a new 4D vector with the corresponding z, y, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYXX() const
    {
        return {z, y, x, x};
    }

    /**
     * A swizzle function for the z, y, x, and y components of the vector
     * @return a new 4D vector with the corresponding z, y, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYXY() const
    {
        return {z, y, x, y};
    }

    /**
     * A swizzle function for the z, y, x, and z components of the vector
     * @return a new 4D vector with the corresponding z, y, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYXZ() const
    {
        return {z, y, x, z};
    }

    /**
     * A swizzle function for the z, y, y, and x components of the vector
     * @return a new 4D vector with the corresponding z, y, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYYX() const
    {
        return {z, y, y, x};
    }

    /**
     * A swizzle function for the z, y, y, and y components of the vector
     * @return a new 4D vector with the corresponding z, y, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYYY() const
    {
        return {z, y, y, y};
    }

    /**
     * A swizzle function for the z, y, y, and z components of the vector
     * @return a new 4D vector with the corresponding z, y, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYYZ() const
    {
        return {z, y, y, z};
    }

    /**
     * A swizzle function for the z, y, z, and x components of the vector
     * @return a new 4D vector with the corresponding z, y, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYZX() const
    {
        return {z, y, z, x};
    }

    /**
     * A swizzle function for the z, y, z, and y components of the vector
     * @return a new 4D vector with the corresponding z, y, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYZY() const
    {
        return {z, y, z, y};
    }

    /**
     * A swizzle function for the z, y, z, and z components of the vector
     * @return a new 4D vector with the corresponding z, y, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYZZ() const
    {
        return {z, y, z, z};
    }

    /**
     * A swizzle function for the z, z, x, and x components of the vector
     * @return a new 4D vector with the corresponding z, z, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZXX() const
    {
        return {z, z, x, x};
    }

    /**
     * A swizzle function for the z, z, x, and y components of the vector
     * @return a new 4D vector with the corresponding z, z, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZXY() const
    {
        return {z, z, x, y};
    }

    /**
     * A swizzle function for the z, z, x, and z components of the vector
     * @return a new 4D vector with the corresponding z, z, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZXZ() const
    {
        return {z, z, x, z};
    }

    /**
     * A swizzle function for the z, z, y, and x components of the vector
     * @return a new 4D vector with the corresponding z, z, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZYX() const
    {
        return {z, z, y, x};
    }

    /**
     * A swizzle function for the z, z, y, and y components of the vector
     * @return a new 4D vector with the corresponding z, z, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZYY() const
    {
        return {z, z, y, y};
    }

    /**
     * A swizzle function for the z, z, y, and z components of the vector
     * @return a new 4D vector with the corresponding z, z, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZYZ() const
    {
        return {z, z, y, z};
    }

    /**
     * A swizzle function for the z, z, z, and x components of the vector
     * @return a new 4D vector with the corresponding z, z, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZZX() const
    {
        return {z, z, z, x};
    }

    /**
     * A swizzle function for the z, z, z, and y components of the vector
     * @return a new 4D vector with the corresponding z, z, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZZY() const
    {
        return {z, z, z, y};
    }

    /**
     * A swizzle function for the z, z, z, and z components of the vector
     * @return a new 4D vector with the corresponding z, z, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZZZ() const
    {
        return {z, z, z, z};
    }

    /**
     * Computes the vector's x component and returns a zero vector with the x component filled in
     * @return a new zero vector with the x component of the vector
     */
    inline constexpr Vec<T, 3> XComp()
    {
        return {x, 0, 0};
    };

    /**
     * Computes the vector's y component and returns a zero vector with the y component filled in
     * @return a new zero vector with the y component of the vector
     */
    inline constexpr Vec<T, 3> YComp()
    {
        return {0, y, 0};
    };

    /**
     * Computes the vector's z component and returns a zero vector with the z component filled in
     * @return a new zero vector with the z component of the vector
     */
    inline constexpr Vec<T, 3> ZComp()
    {
        return {0, 0, z};
    };

    T x {0}, y {0}, z {0};
};

/**
 * The addition operator between two 3D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector addition
 */
template<typename T>
inline constexpr Vec<T, 3> operator+(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
{
    return Vec<T, 3>::Add(lhs, rhs);
}

/**
 * The addition operator between a 3D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to add to the vector
 * @return a new vector with the result of the vector addition
 */
template<typename T>
inline constexpr Vec<T, 3> operator+(const Vec<T, 3>& lhs, T scalar)
{
    return Vec<T, 3>::Add(lhs, scalar);
}

/**
 * The subtraction operator between two 3D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector subtraction
 */
template<typename T>
inline constexpr Vec<T, 3> operator-(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
{
    return Vec<T, 3>::Subtract(lhs, rhs);
}

/**
 * The subtraction operator between a 3D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to subtract from the vector
 * @return a new vector with the result of the vector subtraction
 */
template<typename T>
inline constexpr Vec<T, 3> operator-(const Vec<T, 3>& lhs, T scalar)
{
    return Vec<T, 3>::Subtract(lhs, scalar);
}

/**
 * The multiplication operator between two 3D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 3> operator*(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
{
    return Vec<T, 3>::Multiply(lhs, rhs);
}

/**
 * The multiplication operator between a 3D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to multiply the vector by
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 3> operator*(const Vec<T, 3>& lhs, T scalar)
{
    return Vec<T, 3>::Multiply(lhs, scalar);
}

/**
 * The multiplication operator between a 3D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param scalar the scalar value to multiply the vector by
 * @param lhs the vector on the left hand side of the expression
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 3> operator*(T scalar, const Vec<T, 3>& rhs)
{
    return Vec<T, 3>::Multiply(rhs, scalar);
}

/**
 * The division operator between two 3D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 3> operator/(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs)
{
    return Vec<T, 3>::Divide(lhs, rhs);
}

/**
 * The division operator between a 3D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to divide the vector by
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 3> operator/(const Vec<T, 3>& lhs, T scalar)
{
    return Vec<T, 3>::Divide(lhs, scalar);
}

/**
 * The division operator between a 3D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param scalar the scalar value to divide the vector by
 * @param lhs the vector on the left hand side of the expression
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 3> operator/(T scalar, const Vec<T, 3>& rhs)
{
    return Vec<T, 3>::Divide(rhs, scalar);
}

/**
 * The negation operator
 * @tparam T the type of numerical value held by the vectors
 * @param vector the vector to negate
 * @return a new vector with all values negated
 */
template<typename T>
inline constexpr Vec<T, 3> operator-(const Vec<T, 3>& vector)
{
    return {-vector.x, -vector.y, -vector.z};
}
} // namespace Siege

#endif // SIEGE_ENGINE_VEC3_H
