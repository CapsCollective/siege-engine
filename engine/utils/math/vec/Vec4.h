//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC4_H
#define SIEGE_ENGINE_VEC4_H

#include "Vec.h"

namespace Siege
{
template<typename T>
struct Vec<T, 4>
{
    // Static functions

    /**
     * @returns a new 4D vector with all elements set to zero
     */
    static inline constexpr const Vec<T, 4> Zero()
    {
        return Vec<T, 4> {};
    }

    /**
     * @returns a new 4D vector with all elements set to one
     */
    static inline constexpr Vec<T, 4> One()
    {
        return {1.f, 1.f, 1.f, 1};
    }

    /**
     * @returns a new 4D vector which denotes the world forward direction
     */
    static inline constexpr const Vec<T, 4> Forward()
    {
        return Vec<T, 4> {0, 0, 1, 0};
    }

    /**
     * @returns a new 4D vector which denotes the world right direction
     */
    static inline constexpr const Vec<T, 4> Right()
    {
        return Vec<T, 4> {1, 0, 0, 0};
    }

    /**
     * @returns a new 4D vector which denotes the world up direction
     */
    static inline constexpr const Vec<T, 4> Up()
    {
        return Vec<T, 4> {0, -1, 0, 0};
    }

    /**
     * Adds two 4D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector addition
     */
    static inline constexpr Vec<T, 4> Add(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
    }

    /**
     * Adds a scalar value to a 4D vector and returns the result
     * @param vector the 4D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar addition
     */
    static inline constexpr Vec<T, 4> Add(const Vec<T, 4>& vector, T scalar)
    {
        return {vector.x + scalar, vector.y + scalar, vector.z + scalar, vector.w + scalar};
    }

    /**
     * Subtracts two 4D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector subtraction
     */
    static inline constexpr Vec<T, 4> Subtract(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
    }

    /**
     * Subtracts a scalar value to a 4D vector and returns the result
     * @param vector the 4D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar subtraction
     */
    static inline constexpr Vec<T, 4> Subtract(const Vec<T, 4>& vector, T scalar)
    {
        return {vector.x - scalar, vector.y - scalar, vector.z - scalar, vector.w - scalar};
    }

    /**
     * Multiplies a 4D vector by a scalar value and returns the result
     * @param vector the 4D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar multiplication
     */
    static inline constexpr Vec<T, 4> Multiply(const Vec<T, 4>& vector, T scalar)
    {
        return {vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar};
    }

    /**
     * Multiplies two 4D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector multiplication
     */
    static inline constexpr Vec<T, 4> Multiply(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
    {
        return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w};
    }

    /**
     * Divides two 4D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector division
     */
    static inline constexpr Vec<T, 4> Divide(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
    {
        return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w};
    }

    /**
     * Divides a 4D vector by a scalar value and returns the result
     * @param vector the 4D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar division
     */
    static inline constexpr Vec<T, 4> Divide(const Vec<T, 4>& lhs, T scalar)
    {
        return {lhs.x / scalar, lhs.y / scalar, lhs.z / scalar, lhs.w / scalar};
    }

    /**
     * Computes the Dot product of two 4D vectors
     * @param lhs the vector on the left hand of the operation
     * @param rhs the vector on the right hand of the operation
     * @return the dot product of the two inputted vectors
     */
    static inline constexpr T Dot(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
    {
        return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
    }

    /**
     * Normalises a 4D vector so that all values range between 0 and 1
     * @param vector the vector to be normalised
     * @return a new 4D vector with the normalised values
     */
    static inline constexpr Vec<T, 4> Normalise(const Vec<T, 4>& vector)
    {
        return vector / Length(vector);
    }

    /**
     * Computes the length of a 4D vector
     * @param vector the vector to be calculated
     * @return the length of the inputted vector
     */
    static inline constexpr T Length(const Vec<T, 4>& vector)
    {
        return Float::FastSqrt(Vec<T, 4>::Dot(vector, vector));
    }

    /**
     * Computes the cos of every element of a provided 4D vector
     * @param vec the vector to be computed
     * @return a new vector with the cos of each value
     */
    static inline constexpr Vec<T, 4> Cos(const Vec<T, 4>& vec)
    {
        return {Float::Cos(vec.x), Float::Cos(vec.y), Float::Cos(vec.z), Float::Cos(vec.w)};
    }

    /**
     * Computes the sin of every element of a provided 4D vector
     * @param vec the vector to be computed
     * @return a new vector with the sin of each value
     */
    static inline constexpr Vec<T, 4> Sin(const Vec<T, 4>& vec)
    {
        return {Float::Sin(vec.x), Float::Sin(vec.y), Float::Sin(vec.z), Float::Sin(vec.w)};
    }

    /**
     * Computes the tan of every element of a provided 4D vector
     * @param vec the vector to be computed
     * @return a new vector with the tan of each value
     */
    static inline constexpr Vec<T, 4> Tan(const Vec<T, 4>& vec)
    {
        return {Float::Tan(vec.x), Float::Tan(vec.y), Float::Tan(vec.z), Float::Tan(vec.w)};
    }

    /**
     * Linearly interpolates between two 4D vectors
     * @param origin the vector to begin from
     * @param destination the vector end-result
     * @param time the proportion of movement to be done from origin to destination
     * @return a new vector with the results of the linear interpolation
     */
    static inline constexpr Vec<T, 4> Lerp(const Vec<T, 4>& origin,
                                           const Vec<T, 4>& destination,
                                           T time)
    {
        return origin * (1.0f - time) + destination * time;
    }

    // Structors

    /**
     * The Vec<T, 4> default constructor
     */
    inline constexpr Vec<T, 4>() = default;

    /**
     * The base Vec<T, 4> constructor. Accepts four values
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the z axis
     * @param w the value for the w axis
     */
    inline constexpr Vec<T, 4>(T x, T y, T z, T w) : x {x}, y {y}, z {z}, w {w} {}

    /**
     * The one-element partial constructor for the Vec<T, 4>
     * @param x the value for the x axis
     */
    inline constexpr Vec<T, 4>(T x) : Vec<T, 4>(x, 0, 0, 0) {}

    /**
     * The two-element partial constructor for the Vec<T, 4>
     * @param x the value for the x axis
     * @param y the value for the y axis
     */
    inline constexpr Vec<T, 4>(T x, T y) : Vec<T, 4>(x, y, 0, 0) {}

    /**
     * The three-element partial constructor for the Vec<T, 4>
     * @param x the value for the x axis
     * @param y the value for the y axis
     * @param z the value for the y axis
     */
    inline constexpr Vec<T, 4>(T x, T y, T z) : Vec<T, 4>(x, y, z, 0) {}

    /**
     * The one element 2D vector partial constructor for the Vec<T, 4> class
     * @param other the 2D vector containing the x and y elements
     * @param z the value for the z axis
     * @param w the value for the w axis
     */
    inline constexpr Vec<T, 4>(const Vec<T, 2> other, T z = 0.f, T w = 0.f) :
        Vec<T, 4>(other.x, other.y, z, w)
    {}

    /**
     * The two element 2D vector partial constructor for the Vec<T, 4> class
     * @param otherXY a 2D vector containing the x and y element values
     * @param otherZW a 2D vector containing the z and w element values
     */
    inline constexpr Vec<T, 4>(const Vec<T, 2> otherXY, const Vec<T, 2> otherZW) :
        Vec<T, 4>(otherXY.x, otherXY.y, otherZW.x, otherZW.y)
    {}

    /**
     * The 3D vector constructor. Creates a 4D vector from a 3D vector of the same type
     * @param other the 3D vector to convert from
     * @param w the w axis value
     */
    inline constexpr Vec<T, 4>(const Vec<T, 3> other, T w) : Vec<T, 4>(other.x, other.y, other.z, w)
    {}

    /**
     * The 3D vector constructor. Creates a 4D vector from a 3D vector of the same type. The w
     * element defaults to 0
     * @param other the 3D vector to convert from
     */
    inline constexpr Vec<T, 4>(const Vec<T, 3> other) : Vec<T, 4>(other, 0.f) {}

    // Operator Overloads

    /**
     * The 4D vector subscript operator
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
            case (3):
                return w;
            case (0):
            default:
                return x;
        }
    }

    /**
     * The const 4D vector subscript operator
     * @param index the index of the requested dimension
     * @return a copy of the requested value
     */
    inline constexpr T operator[](unsigned int index) const
    {
        return ((index == 0) * x) + ((index == 1) * y) + ((index == 2) * z) + ((index == 3) * w);
    }

    /**
     * The equality operator. Checks if two 4D vectors are equal on all axes
     * @param rhs the 4D vector to compare
     * @return a boolean specifying if the vectors are equal
     */
    inline constexpr bool operator==(const Vec<T, 4>& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }

    /**
     * The inequality operator. Checks if two 4D vectors are not equal on all axes
     * @param rhs the 4D vector to compare
     * @return a boolean specifying if the vectors are not equal
     */
    inline constexpr bool operator!=(const Vec<T, 4>& rhs) const
    {
        return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
    }

    /**
     * The 3D Vec assignment operator. Assigns the values of the x, y, and z axes of a 3D vector to
     * the 4D vector
     * @param vec the 3D vector to assign values from
     * @return a reference to the current 4D vector
     */
    inline constexpr Vec<T, 4> operator=(const Vec<T, 3>& vec) const
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    /**
     * The 2D Vec assignment operator. Assigns the values of the x, and y axes of a 2D vector to the
     * 4D vector
     * @param vec the 2D vector to assign values from
     * @return a reference to the current 4D vector
     */
    inline constexpr Vec<T, 4> operator=(const Vec<T, 2>& vec) const
    {
        x = vec.x;
        y = vec.y;
        return *this;
    }

    /**
     * The addition operator. Adds the values of a 4D vector to the current vector
     * @param rhs the vector to add values from
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator+=(const Vec<T, 4>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    /**
     * The scalar addition operator. Adds the provided scalar value to the 4D vector
     * @param scalar the scalar value to add
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator+=(T scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }

    /**
     * The subtraction operator. Subtracts the values of a 4D vector from the current vector
     * @param rhs the vector to subtract values from
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator-=(const Vec<T, 4>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    /**
     * The scalar subtraction operator. Subtracts the provided scalar value from the 4D vector
     * @param scalar the scalar value to subtract
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator-=(T scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }

    /**
     * The multiplication operator. Multiplies the values of a 4D vector by the current vector
     * @param rhs the vector to multiply values from
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator*=(const Vec<T, 4>& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }

    /**
     * The scalar multiplication operator. Multiplies the provided scalar value from the 4D vector
     * @param scalar the scalar value to multiply by
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    /**
     * The division operator. Divides the values of the 4D vector by the provided vector
     * @param rhs the vector to divide values by
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator/=(const Vec<T, 4>& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;
        return *this;
    }

    /**
     * The scalar division operator. Divides the 4D vector by the provided scalar value
     * @param scalar the scalar value to divide by
     * @return a reference to the current Vec<T, 4>
     */
    inline constexpr Vec<T, 4>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    /**
     * The Negation operator. Negates the values in the vector
     * @return a new vector with the negated vector values
     */
    inline constexpr Vec<T, 4>& operator-()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
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
     * A swizzle function for the x and w components of the vector
     * @return a new 2D vector with the corresponding x and w components of the original vector
     */
    inline constexpr Vec<T, 2> XW() const
    {
        return {x, w};
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
     * A swizzle function for the y and w components of the vector
     * @return a new 2D vector with the corresponding y and w components of the original vector
     */
    inline constexpr Vec<T, 2> YW() const
    {
        return {y, w};
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
     * A swizzle function for the z and w components of the vector
     * @return a new 2D vector with the corresponding z and w components of the original vector
     */
    inline constexpr Vec<T, 2> ZW() const
    {
        return {z, w};
    }

    /**
     * A swizzle function for the w and x components of the vector
     * @return a new 2D vector with the corresponding w and x components of the original vector
     */
    inline constexpr Vec<T, 2> WX() const
    {
        return {w, x};
    }

    /**
     * A swizzle function for the w and y components of the vector
     * @return a new 2D vector with the corresponding w and y components of the original vector
     */
    inline constexpr Vec<T, 2> WY() const
    {
        return {w, y};
    }

    /**
     * A swizzle function for the w and z components of the vector
     * @return a new 2D vector with the corresponding w and z components of the original vector
     */
    inline constexpr Vec<T, 2> WZ() const
    {
        return {w, z};
    }

    /**
     * A swizzle function for the w and w components of the vector
     * @return a new 2D vector with the corresponding w and w components of the original vector
     */
    inline constexpr Vec<T, 2> WW() const
    {
        return {w, w};
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
     * A swizzle function for the x, x, and w components of the vector
     * @return a new 3D vector with the corresponding x, x, and w components of the original vector
     */
    inline constexpr Vec<T, 3> XXW() const
    {
        return {x, x, w};
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
     * A swizzle function for the x, y, and w components of the vector
     * @return a new 3D vector with the corresponding x, y, and w components of the original vector
     */
    inline constexpr Vec<T, 3> XYW() const
    {
        return {x, y, w};
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
     * A swizzle function for the x, z, and w components of the vector
     * @return a new 3D vector with the corresponding x, z, and w components of the original vector
     */
    inline constexpr Vec<T, 3> XZW() const
    {
        return {x, z, w};
    }

    /**
     * A swizzle function for the x, w, and x components of the vector
     * @return a new 3D vector with the corresponding x, w, and x components of the original vector
     */
    inline constexpr Vec<T, 3> XWX() const
    {
        return {x, w, x};
    }

    /**
     * A swizzle function for the x, w, and y components of the vector
     * @return a new 3D vector with the corresponding x, w, and y components of the original vector
     */
    inline constexpr Vec<T, 3> XWY() const
    {
        return {x, w, y};
    }

    /**
     * A swizzle function for the x, w, and z components of the vector
     * @return a new 3D vector with the corresponding x, w, and z components of the original vector
     */
    inline constexpr Vec<T, 3> XWZ() const
    {
        return {x, w, z};
    }

    /**
     * A swizzle function for the x, w, and w components of the vector
     * @return a new 3D vector with the corresponding x, w, and w components of the original vector
     */
    inline constexpr Vec<T, 3> XWW() const
    {
        return {x, w, w};
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
     * A swizzle function for the y, x, and w components of the vector
     * @return a new 3D vector with the corresponding y, x, and w components of the original vector
     */
    inline constexpr Vec<T, 3> YXW() const
    {
        return {y, x, w};
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
     * A swizzle function for the y, y, and w components of the vector
     * @return a new 3D vector with the corresponding y, y, and w components of the original vector
     */
    inline constexpr Vec<T, 3> YYW() const
    {
        return {y, y, w};
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
     * A swizzle function for the y, z, and w components of the vector
     * @return a new 3D vector with the corresponding y, z, and w components of the original vector
     */
    inline constexpr Vec<T, 3> YZW() const
    {
        return {y, z, w};
    }

    /**
     * A swizzle function for the y, w, and x components of the vector
     * @return a new 3D vector with the corresponding y, w, and x components of the original vector
     */
    inline constexpr Vec<T, 3> YWX() const
    {
        return {y, w, x};
    }

    /**
     * A swizzle function for the y, w, and y components of the vector
     * @return a new 3D vector with the corresponding y, w, and y components of the original vector
     */
    inline constexpr Vec<T, 3> YWY() const
    {
        return {y, w, y};
    }

    /**
     * A swizzle function for the y, w, and z components of the vector
     * @return a new 3D vector with the corresponding y, w, and z components of the original vector
     */
    inline constexpr Vec<T, 3> YWZ() const
    {
        return {y, w, z};
    }

    /**
     * A swizzle function for the y, w, and w components of the vector
     * @return a new 3D vector with the corresponding y, w, and w components of the original vector
     */
    inline constexpr Vec<T, 3> YWW() const
    {
        return {y, w, w};
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
     * A swizzle function for the z, x, and w components of the vector
     * @return a new 3D vector with the corresponding z, x, and w components of the original vector
     */
    inline constexpr Vec<T, 3> ZXW() const
    {
        return {z, x, w};
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
     * A swizzle function for the z, y, and w components of the vector
     * @return a new 3D vector with the corresponding z, y, and w components of the original vector
     */
    inline constexpr Vec<T, 3> ZYW() const
    {
        return {z, y, w};
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
     * A swizzle function for the z, z, and w components of the vector
     * @return a new 3D vector with the corresponding z, z, and w components of the original vector
     */
    inline constexpr Vec<T, 3> ZZW() const
    {
        return {z, z, w};
    }

    /**
     * A swizzle function for the z, w, and x components of the vector
     * @return a new 3D vector with the corresponding z, w, and x components of the original vector
     */
    inline constexpr Vec<T, 3> ZWX() const
    {
        return {z, w, x};
    }

    /**
     * A swizzle function for the z, w, and y components of the vector
     * @return a new 3D vector with the corresponding z, w, and y components of the original vector
     */
    inline constexpr Vec<T, 3> ZWY() const
    {
        return {z, w, y};
    }

    /**
     * A swizzle function for the z, w, and z components of the vector
     * @return a new 3D vector with the corresponding z, w, and z components of the original vector
     */
    inline constexpr Vec<T, 3> ZWZ() const
    {
        return {z, w, z};
    }

    /**
     * A swizzle function for the z, w, and w components of the vector
     * @return a new 3D vector with the corresponding z, w, and w components of the original vector
     */
    inline constexpr Vec<T, 3> ZWW() const
    {
        return {z, w, w};
    }

    /**
     * A swizzle function for the w, x, and x components of the vector
     * @return a new 3D vector with the corresponding w, x, and x components of the original vector
     */
    inline constexpr Vec<T, 3> WXX() const
    {
        return {w, x, x};
    }

    /**
     * A swizzle function for the w, x, and y components of the vector
     * @return a new 3D vector with the corresponding w, x, and y components of the original vector
     */
    inline constexpr Vec<T, 3> WXY() const
    {
        return {w, x, y};
    }

    /**
     * A swizzle function for the w, x, and z components of the vector
     * @return a new 3D vector with the corresponding w, x, and z components of the original vector
     */
    inline constexpr Vec<T, 3> WXZ() const
    {
        return {w, x, z};
    }

    /**
     * A swizzle function for the w, x, and w components of the vector
     * @return a new 3D vector with the corresponding w, x, and w components of the original vector
     */
    inline constexpr Vec<T, 3> WXW() const
    {
        return {w, x, w};
    }

    /**
     * A swizzle function for the w, y, and x components of the vector
     * @return a new 3D vector with the corresponding w, y, and x components of the original vector
     */
    inline constexpr Vec<T, 3> WYX() const
    {
        return {w, y, x};
    }

    /**
     * A swizzle function for the w, y, and y components of the vector
     * @return a new 3D vector with the corresponding w, y, and y components of the original vector
     */
    inline constexpr Vec<T, 3> WYY() const
    {
        return {w, y, y};
    }

    /**
     * A swizzle function for the w, y, and z components of the vector
     * @return a new 3D vector with the corresponding w, y, and z components of the original vector
     */
    inline constexpr Vec<T, 3> WYZ() const
    {
        return {w, y, z};
    }

    /**
     * A swizzle function for the w, y, and w components of the vector
     * @return a new 3D vector with the corresponding w, y, and w components of the original vector
     */
    inline constexpr Vec<T, 3> WYW() const
    {
        return {w, y, w};
    }

    /**
     * A swizzle function for the w, z, and x components of the vector
     * @return a new 3D vector with the corresponding w, z, and x components of the original vector
     */
    inline constexpr Vec<T, 3> WZX() const
    {
        return {w, z, x};
    }

    /**
     * A swizzle function for the w, z, and y components of the vector
     * @return a new 3D vector with the corresponding w, z, and y components of the original vector
     */
    inline constexpr Vec<T, 3> WZY() const
    {
        return {w, z, y};
    }

    /**
     * A swizzle function for the w, z, and z components of the vector
     * @return a new 3D vector with the corresponding w, z, and z components of the original vector
     */
    inline constexpr Vec<T, 3> WZZ() const
    {
        return {w, z, z};
    }

    /**
     * A swizzle function for the w, z, and w components of the vector
     * @return a new 3D vector with the corresponding w, z, and w components of the original vector
     */
    inline constexpr Vec<T, 3> WZW() const
    {
        return {w, z, w};
    }

    /**
     * A swizzle function for the w, w, and x components of the vector
     * @return a new 3D vector with the corresponding w, w, and x components of the original vector
     */
    inline constexpr Vec<T, 3> WWX() const
    {
        return {w, w, x};
    }

    /**
     * A swizzle function for the w, w, and y components of the vector
     * @return a new 3D vector with the corresponding w, w, and y components of the original vector
     */
    inline constexpr Vec<T, 3> WWY() const
    {
        return {w, w, y};
    }

    /**
     * A swizzle function for the w, w, and z components of the vector
     * @return a new 3D vector with the corresponding w, w, and z components of the original vector
     */
    inline constexpr Vec<T, 3> WWZ() const
    {
        return {w, w, z};
    }

    /**
     * A swizzle function for the w, w, and w components of the vector
     * @return a new 3D vector with the corresponding w, w, and w components of the original vector
     */
    inline constexpr Vec<T, 3> WWW() const
    {
        return {w, w, w};
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
     * A swizzle function for the x, x, x, and w components of the vector
     * @return a new 4D vector with the corresponding x, x, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXXW() const
    {
        return {x, x, x, w};
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
     * A swizzle function for the x, x, y, and w components of the vector
     * @return a new 4D vector with the corresponding x, x, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXYW() const
    {
        return {x, x, y, w};
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
     * A swizzle function for the x, x, z, and w components of the vector
     * @return a new 4D vector with the corresponding x, x, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXZW() const
    {
        return {x, x, z, w};
    }

    /**
     * A swizzle function for the x, x, w, and x components of the vector
     * @return a new 4D vector with the corresponding x, x, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXWX() const
    {
        return {x, x, w, x};
    }

    /**
     * A swizzle function for the x, x, w, and y components of the vector
     * @return a new 4D vector with the corresponding x, x, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXWY() const
    {
        return {x, x, w, y};
    }

    /**
     * A swizzle function for the x, x, w, and z components of the vector
     * @return a new 4D vector with the corresponding x, x, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXWZ() const
    {
        return {x, x, w, z};
    }

    /**
     * A swizzle function for the x, x, w, and w components of the vector
     * @return a new 4D vector with the corresponding x, x, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XXWW() const
    {
        return {x, x, w, w};
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
     * A swizzle function for the x, y, x, and w components of the vector
     * @return a new 4D vector with the corresponding x, y, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYXW() const
    {
        return {x, y, x, w};
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
     * A swizzle function for the x, y, y, and w components of the vector
     * @return a new 4D vector with the corresponding x, y, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYYW() const
    {
        return {x, y, y, w};
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
     * A swizzle function for the x, y, z, and w components of the vector
     * @return a new 4D vector with the corresponding x, y, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYZW() const
    {
        return {x, y, z, w};
    }

    /**
     * A swizzle function for the x, y, w, and x components of the vector
     * @return a new 4D vector with the corresponding x, y, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYWX() const
    {
        return {x, y, w, x};
    }

    /**
     * A swizzle function for the x, y, w, and y components of the vector
     * @return a new 4D vector with the corresponding x, y, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYWY() const
    {
        return {x, y, w, y};
    }

    /**
     * A swizzle function for the x, y, w, and z components of the vector
     * @return a new 4D vector with the corresponding x, y, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYWZ() const
    {
        return {x, y, w, z};
    }

    /**
     * A swizzle function for the x, y, w, and w components of the vector
     * @return a new 4D vector with the corresponding x, y, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XYWW() const
    {
        return {x, y, w, w};
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
     * A swizzle function for the x, z, x, and w components of the vector
     * @return a new 4D vector with the corresponding x, z, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZXW() const
    {
        return {x, z, x, w};
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
     * A swizzle function for the x, z, y, and w components of the vector
     * @return a new 4D vector with the corresponding x, z, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZYW() const
    {
        return {x, z, y, w};
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
     * A swizzle function for the x, z, z, and w components of the vector
     * @return a new 4D vector with the corresponding x, z, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZZW() const
    {
        return {x, z, z, w};
    }

    /**
     * A swizzle function for the x, z, w, and x components of the vector
     * @return a new 4D vector with the corresponding x, z, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZWX() const
    {
        return {x, z, w, x};
    }

    /**
     * A swizzle function for the x, z, w, and y components of the vector
     * @return a new 4D vector with the corresponding x, z, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZWY() const
    {
        return {x, z, w, y};
    }

    /**
     * A swizzle function for the x, z, w, and z components of the vector
     * @return a new 4D vector with the corresponding x, z, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZWZ() const
    {
        return {x, z, w, z};
    }

    /**
     * A swizzle function for the x, z, w, and w components of the vector
     * @return a new 4D vector with the corresponding x, z, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XZWW() const
    {
        return {x, z, w, w};
    }

    /**
     * A swizzle function for the x, w, x, and x components of the vector
     * @return a new 4D vector with the corresponding x, w, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWXX() const
    {
        return {x, w, x, x};
    }

    /**
     * A swizzle function for the x, w, x, and y components of the vector
     * @return a new 4D vector with the corresponding x, w, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWXY() const
    {
        return {x, w, x, y};
    }

    /**
     * A swizzle function for the x, w, x, and z components of the vector
     * @return a new 4D vector with the corresponding x, w, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWXZ() const
    {
        return {x, w, x, z};
    }

    /**
     * A swizzle function for the x, w, x, and w components of the vector
     * @return a new 4D vector with the corresponding x, w, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWXW() const
    {
        return {x, w, x, w};
    }

    /**
     * A swizzle function for the x, w, y, and x components of the vector
     * @return a new 4D vector with the corresponding x, w, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWYX() const
    {
        return {x, w, y, x};
    }

    /**
     * A swizzle function for the x, w, y, and y components of the vector
     * @return a new 4D vector with the corresponding x, w, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWYY() const
    {
        return {x, w, y, y};
    }

    /**
     * A swizzle function for the x, w, y, and z components of the vector
     * @return a new 4D vector with the corresponding x, w, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWYZ() const
    {
        return {x, w, y, z};
    }

    /**
     * A swizzle function for the x, w, y, and w components of the vector
     * @return a new 4D vector with the corresponding x, w, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWYW() const
    {
        return {x, w, y, w};
    }

    /**
     * A swizzle function for the x, w, z, and x components of the vector
     * @return a new 4D vector with the corresponding x, w, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWZX() const
    {
        return {x, w, z, x};
    }

    /**
     * A swizzle function for the x, w, z, and y components of the vector
     * @return a new 4D vector with the corresponding x, w, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWZY() const
    {
        return {x, w, z, y};
    }

    /**
     * A swizzle function for the x, w, z, and z components of the vector
     * @return a new 4D vector with the corresponding x, w, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWZZ() const
    {
        return {x, w, z, z};
    }

    /**
     * A swizzle function for the x, w, z, and w components of the vector
     * @return a new 4D vector with the corresponding x, w, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWZW() const
    {
        return {x, w, z, w};
    }

    /**
     * A swizzle function for the x, w, w, and x components of the vector
     * @return a new 4D vector with the corresponding x, w, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWWX() const
    {
        return {x, w, w, x};
    }

    /**
     * A swizzle function for the x, w, w, and y components of the vector
     * @return a new 4D vector with the corresponding x, w, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWWY() const
    {
        return {x, w, w, y};
    }

    /**
     * A swizzle function for the x, w, w, and z components of the vector
     * @return a new 4D vector with the corresponding x, w, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWWZ() const
    {
        return {x, w, w, z};
    }

    /**
     * A swizzle function for the x, w, w, and w components of the vector
     * @return a new 4D vector with the corresponding x, w, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> XWWW() const
    {
        return {x, w, w, w};
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
     * A swizzle function for the y, x, x, and w components of the vector
     * @return a new 4D vector with the corresponding y, x, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXXW() const
    {
        return {y, x, x, w};
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
     * A swizzle function for the y, x, y, and w components of the vector
     * @return a new 4D vector with the corresponding y, x, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXYW() const
    {
        return {y, x, y, w};
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
     * A swizzle function for the y, x, z, and w components of the vector
     * @return a new 4D vector with the corresponding y, x, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXZW() const
    {
        return {y, x, z, w};
    }

    /**
     * A swizzle function for the y, x, w, and x components of the vector
     * @return a new 4D vector with the corresponding y, x, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXWX() const
    {
        return {y, x, w, x};
    }

    /**
     * A swizzle function for the y, x, w, and y components of the vector
     * @return a new 4D vector with the corresponding y, x, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXWY() const
    {
        return {y, x, w, y};
    }

    /**
     * A swizzle function for the y, x, w, and z components of the vector
     * @return a new 4D vector with the corresponding y, x, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXWZ() const
    {
        return {y, x, w, z};
    }

    /**
     * A swizzle function for the y, x, w, and w components of the vector
     * @return a new 4D vector with the corresponding y, x, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YXWW() const
    {
        return {y, x, w, w};
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
     * A swizzle function for the y, y, x, and w components of the vector
     * @return a new 4D vector with the corresponding y, y, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYXW() const
    {
        return {y, y, x, w};
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
     * A swizzle function for the y, y, y, and w components of the vector
     * @return a new 4D vector with the corresponding y, y, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYYW() const
    {
        return {y, y, y, w};
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
     * A swizzle function for the y, y, z, and w components of the vector
     * @return a new 4D vector with the corresponding y, y, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYZW() const
    {
        return {y, y, z, w};
    }

    /**
     * A swizzle function for the y, y, w, and x components of the vector
     * @return a new 4D vector with the corresponding y, y, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYWX() const
    {
        return {y, y, w, x};
    }

    /**
     * A swizzle function for the y, y, w, and y components of the vector
     * @return a new 4D vector with the corresponding y, y, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYWY() const
    {
        return {y, y, w, y};
    }

    /**
     * A swizzle function for the y, y, w, and z components of the vector
     * @return a new 4D vector with the corresponding y, y, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYWZ() const
    {
        return {y, y, w, z};
    }

    /**
     * A swizzle function for the y, y, w, and w components of the vector
     * @return a new 4D vector with the corresponding y, y, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YYWW() const
    {
        return {y, y, w, w};
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
     * A swizzle function for the y, z, x, and w components of the vector
     * @return a new 4D vector with the corresponding y, z, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZXW() const
    {
        return {y, z, x, w};
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
     * A swizzle function for the y, z, y, and w components of the vector
     * @return a new 4D vector with the corresponding y, z, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZYW() const
    {
        return {y, z, y, w};
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
     * A swizzle function for the y, z, z, and w components of the vector
     * @return a new 4D vector with the corresponding y, z, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZZW() const
    {
        return {y, z, z, w};
    }

    /**
     * A swizzle function for the y, z, w, and x components of the vector
     * @return a new 4D vector with the corresponding y, z, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZWX() const
    {
        return {y, z, w, x};
    }

    /**
     * A swizzle function for the y, z, w, and y components of the vector
     * @return a new 4D vector with the corresponding y, z, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZWY() const
    {
        return {y, z, w, y};
    }

    /**
     * A swizzle function for the y, z, w, and z components of the vector
     * @return a new 4D vector with the corresponding y, z, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZWZ() const
    {
        return {y, z, w, z};
    }

    /**
     * A swizzle function for the y, z, w, and w components of the vector
     * @return a new 4D vector with the corresponding y, z, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YZWW() const
    {
        return {y, z, w, w};
    }

    /**
     * A swizzle function for the y, w, x, and x components of the vector
     * @return a new 4D vector with the corresponding y, w, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWXX() const
    {
        return {y, w, x, x};
    }

    /**
     * A swizzle function for the y, w, x, and y components of the vector
     * @return a new 4D vector with the corresponding y, w, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWXY() const
    {
        return {y, w, x, y};
    }

    /**
     * A swizzle function for the y, w, x, and z components of the vector
     * @return a new 4D vector with the corresponding y, w, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWXZ() const
    {
        return {y, w, x, z};
    }

    /**
     * A swizzle function for the y, w, x, and w components of the vector
     * @return a new 4D vector with the corresponding y, w, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWXW() const
    {
        return {y, w, x, w};
    }

    /**
     * A swizzle function for the y, w, y, and x components of the vector
     * @return a new 4D vector with the corresponding y, w, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWYX() const
    {
        return {y, w, y, x};
    }

    /**
     * A swizzle function for the y, w, y, and y components of the vector
     * @return a new 4D vector with the corresponding y, w, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWYY() const
    {
        return {y, w, y, y};
    }

    /**
     * A swizzle function for the y, w, y, and z components of the vector
     * @return a new 4D vector with the corresponding y, w, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWYZ() const
    {
        return {y, w, y, z};
    }

    /**
     * A swizzle function for the y, w, y, and w components of the vector
     * @return a new 4D vector with the corresponding y, w, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWYW() const
    {
        return {y, w, y, w};
    }

    /**
     * A swizzle function for the y, w, z, and x components of the vector
     * @return a new 4D vector with the corresponding y, w, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWZX() const
    {
        return {y, w, z, x};
    }

    /**
     * A swizzle function for the y, w, z, and y components of the vector
     * @return a new 4D vector with the corresponding y, w, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWZY() const
    {
        return {y, w, z, y};
    }

    /**
     * A swizzle function for the y, w, z, and z components of the vector
     * @return a new 4D vector with the corresponding y, w, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWZZ() const
    {
        return {y, w, z, z};
    }

    /**
     * A swizzle function for the y, w, z, and w components of the vector
     * @return a new 4D vector with the corresponding y, w, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWZW() const
    {
        return {y, w, z, w};
    }

    /**
     * A swizzle function for the y, w, w, and x components of the vector
     * @return a new 4D vector with the corresponding y, w, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWWX() const
    {
        return {y, w, w, x};
    }

    /**
     * A swizzle function for the y, w, w, and y components of the vector
     * @return a new 4D vector with the corresponding y, w, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWWY() const
    {
        return {y, w, w, y};
    }

    /**
     * A swizzle function for the y, w, w, and z components of the vector
     * @return a new 4D vector with the corresponding y, w, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWWZ() const
    {
        return {y, w, w, z};
    }

    /**
     * A swizzle function for the y, w, w, and w components of the vector
     * @return a new 4D vector with the corresponding y, w, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> YWWW() const
    {
        return {y, w, w, w};
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
     * A swizzle function for the z, x, x, and w components of the vector
     * @return a new 4D vector with the corresponding z, x, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXXW() const
    {
        return {z, x, x, w};
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
     * A swizzle function for the z, x, y, and w components of the vector
     * @return a new 4D vector with the corresponding z, x, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXYW() const
    {
        return {z, x, y, w};
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
     * A swizzle function for the z, x, z, and w components of the vector
     * @return a new 4D vector with the corresponding z, x, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXZW() const
    {
        return {z, x, z, w};
    }

    /**
     * A swizzle function for the z, x, w, and x components of the vector
     * @return a new 4D vector with the corresponding z, x, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXWX() const
    {
        return {z, x, w, x};
    }

    /**
     * A swizzle function for the z, x, w, and y components of the vector
     * @return a new 4D vector with the corresponding z, x, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXWY() const
    {
        return {z, x, w, y};
    }

    /**
     * A swizzle function for the z, x, w, and z components of the vector
     * @return a new 4D vector with the corresponding z, x, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXWZ() const
    {
        return {z, x, w, z};
    }

    /**
     * A swizzle function for the z, x, w, and w components of the vector
     * @return a new 4D vector with the corresponding z, x, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZXWW() const
    {
        return {z, x, w, w};
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
     * A swizzle function for the z, y, x, and w components of the vector
     * @return a new 4D vector with the corresponding z, y, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYXW() const
    {
        return {z, y, x, w};
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
     * A swizzle function for the z, y, y, and w components of the vector
     * @return a new 4D vector with the corresponding z, y, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYYW() const
    {
        return {z, y, y, w};
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
     * A swizzle function for the z, y, z, and w components of the vector
     * @return a new 4D vector with the corresponding z, y, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYZW() const
    {
        return {z, y, z, w};
    }

    /**
     * A swizzle function for the z, y, w, and x components of the vector
     * @return a new 4D vector with the corresponding z, y, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYWX() const
    {
        return {z, y, w, x};
    }

    /**
     * A swizzle function for the z, y, w, and y components of the vector
     * @return a new 4D vector with the corresponding z, y, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYWY() const
    {
        return {z, y, w, y};
    }

    /**
     * A swizzle function for the z, y, w, and z components of the vector
     * @return a new 4D vector with the corresponding z, y, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYWZ() const
    {
        return {z, y, w, z};
    }

    /**
     * A swizzle function for the z, y, w, and w components of the vector
     * @return a new 4D vector with the corresponding z, y, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZYWW() const
    {
        return {z, y, w, w};
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
     * A swizzle function for the z, z, x, and w components of the vector
     * @return a new 4D vector with the corresponding z, z, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZXW() const
    {
        return {z, z, x, w};
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
     * A swizzle function for the z, z, y, and w components of the vector
     * @return a new 4D vector with the corresponding z, z, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZYW() const
    {
        return {z, z, y, w};
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
     * A swizzle function for the z, z, z, and w components of the vector
     * @return a new 4D vector with the corresponding z, z, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZZW() const
    {
        return {z, z, z, w};
    }

    /**
     * A swizzle function for the z, z, w, and x components of the vector
     * @return a new 4D vector with the corresponding z, z, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZWX() const
    {
        return {z, z, w, x};
    }

    /**
     * A swizzle function for the z, z, w, and y components of the vector
     * @return a new 4D vector with the corresponding z, z, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZWY() const
    {
        return {z, z, w, y};
    }

    /**
     * A swizzle function for the z, z, w, and z components of the vector
     * @return a new 4D vector with the corresponding z, z, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZWZ() const
    {
        return {z, z, w, z};
    }

    /**
     * A swizzle function for the z, z, w, and w components of the vector
     * @return a new 4D vector with the corresponding z, z, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZZWW() const
    {
        return {z, z, w, w};
    }

    /**
     * A swizzle function for the z, w, x, and x components of the vector
     * @return a new 4D vector with the corresponding z, w, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWXX() const
    {
        return {z, w, x, x};
    }

    /**
     * A swizzle function for the z, w, x, and y components of the vector
     * @return a new 4D vector with the corresponding z, w, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWXY() const
    {
        return {z, w, x, y};
    }

    /**
     * A swizzle function for the z, w, x, and z components of the vector
     * @return a new 4D vector with the corresponding z, w, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWXZ() const
    {
        return {z, w, x, z};
    }

    /**
     * A swizzle function for the z, w, x, and w components of the vector
     * @return a new 4D vector with the corresponding z, w, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWXW() const
    {
        return {z, w, x, w};
    }

    /**
     * A swizzle function for the z, w, y, and x components of the vector
     * @return a new 4D vector with the corresponding z, w, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWYX() const
    {
        return {z, w, y, x};
    }

    /**
     * A swizzle function for the z, w, y, and y components of the vector
     * @return a new 4D vector with the corresponding z, w, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWYY() const
    {
        return {z, w, y, y};
    }

    /**
     * A swizzle function for the z, w, y, and z components of the vector
     * @return a new 4D vector with the corresponding z, w, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWYZ() const
    {
        return {z, w, y, z};
    }

    /**
     * A swizzle function for the z, w, y, and w components of the vector
     * @return a new 4D vector with the corresponding z, w, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWYW() const
    {
        return {z, w, y, w};
    }

    /**
     * A swizzle function for the z, w, z, and x components of the vector
     * @return a new 4D vector with the corresponding z, w, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWZX() const
    {
        return {z, w, z, x};
    }

    /**
     * A swizzle function for the z, w, z, and y components of the vector
     * @return a new 4D vector with the corresponding z, w, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWZY() const
    {
        return {z, w, z, y};
    }

    /**
     * A swizzle function for the z, w, z, and z components of the vector
     * @return a new 4D vector with the corresponding z, w, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWZZ() const
    {
        return {z, w, z, z};
    }

    /**
     * A swizzle function for the z, w, z, and w components of the vector
     * @return a new 4D vector with the corresponding z, w, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWZW() const
    {
        return {z, w, z, w};
    }

    /**
     * A swizzle function for the z, w, w, and x components of the vector
     * @return a new 4D vector with the corresponding z, w, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWWX() const
    {
        return {z, w, w, x};
    }

    /**
     * A swizzle function for the z, w, w, and y components of the vector
     * @return a new 4D vector with the corresponding z, w, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWWY() const
    {
        return {z, w, w, y};
    }

    /**
     * A swizzle function for the z, w, w, and z components of the vector
     * @return a new 4D vector with the corresponding z, w, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWWZ() const
    {
        return {z, w, w, z};
    }

    /**
     * A swizzle function for the z, w, w, and w components of the vector
     * @return a new 4D vector with the corresponding z, w, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> ZWWW() const
    {
        return {z, w, w, w};
    }

    /**
     * A swizzle function for the w, x, x, and x components of the vector
     * @return a new 4D vector with the corresponding w, x, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXXX() const
    {
        return {w, x, x, x};
    }

    /**
     * A swizzle function for the w, x, x, and y components of the vector
     * @return a new 4D vector with the corresponding w, x, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXXY() const
    {
        return {w, x, x, y};
    }

    /**
     * A swizzle function for the w, x, x, and z components of the vector
     * @return a new 4D vector with the corresponding w, x, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXXZ() const
    {
        return {w, x, x, z};
    }

    /**
     * A swizzle function for the w, x, x, and w components of the vector
     * @return a new 4D vector with the corresponding w, x, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXXW() const
    {
        return {w, x, x, w};
    }

    /**
     * A swizzle function for the w, x, y, and x components of the vector
     * @return a new 4D vector with the corresponding w, x, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXYX() const
    {
        return {w, x, y, x};
    }

    /**
     * A swizzle function for the w, x, y, and y components of the vector
     * @return a new 4D vector with the corresponding w, x, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXYY() const
    {
        return {w, x, y, y};
    }

    /**
     * A swizzle function for the w, x, y, and z components of the vector
     * @return a new 4D vector with the corresponding w, x, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXYZ() const
    {
        return {w, x, y, z};
    }

    /**
     * A swizzle function for the w, x, y, and w components of the vector
     * @return a new 4D vector with the corresponding w, x, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXYW() const
    {
        return {w, x, y, w};
    }

    /**
     * A swizzle function for the w, x, z, and x components of the vector
     * @return a new 4D vector with the corresponding w, x, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXZX() const
    {
        return {w, x, z, x};
    }

    /**
     * A swizzle function for the w, x, z, and y components of the vector
     * @return a new 4D vector with the corresponding w, x, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXZY() const
    {
        return {w, x, z, y};
    }

    /**
     * A swizzle function for the w, x, z, and z components of the vector
     * @return a new 4D vector with the corresponding w, x, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXZZ() const
    {
        return {w, x, z, z};
    }

    /**
     * A swizzle function for the w, x, z, and w components of the vector
     * @return a new 4D vector with the corresponding w, x, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXZW() const
    {
        return {w, x, z, w};
    }

    /**
     * A swizzle function for the w, x, w, and x components of the vector
     * @return a new 4D vector with the corresponding w, x, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXWX() const
    {
        return {w, x, w, x};
    }

    /**
     * A swizzle function for the w, x, w, and y components of the vector
     * @return a new 4D vector with the corresponding w, x, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXWY() const
    {
        return {w, x, w, y};
    }

    /**
     * A swizzle function for the w, x, w, and z components of the vector
     * @return a new 4D vector with the corresponding w, x, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXWZ() const
    {
        return {w, x, w, z};
    }

    /**
     * A swizzle function for the w, x, w, and w components of the vector
     * @return a new 4D vector with the corresponding w, x, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WXWW() const
    {
        return {w, x, w, w};
    }

    /**
     * A swizzle function for the w, y, x, and x components of the vector
     * @return a new 4D vector with the corresponding w, y, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYXX() const
    {
        return {w, y, x, x};
    }

    /**
     * A swizzle function for the w, y, x, and y components of the vector
     * @return a new 4D vector with the corresponding w, y, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYXY() const
    {
        return {w, y, x, y};
    }

    /**
     * A swizzle function for the w, y, x, and z components of the vector
     * @return a new 4D vector with the corresponding w, y, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYXZ() const
    {
        return {w, y, x, z};
    }

    /**
     * A swizzle function for the w, y, x, and w components of the vector
     * @return a new 4D vector with the corresponding w, y, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYXW() const
    {
        return {w, y, x, w};
    }

    /**
     * A swizzle function for the w, y, y, and x components of the vector
     * @return a new 4D vector with the corresponding w, y, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYYX() const
    {
        return {w, y, y, x};
    }

    /**
     * A swizzle function for the w, y, y, and y components of the vector
     * @return a new 4D vector with the corresponding w, y, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYYY() const
    {
        return {w, y, y, y};
    }

    /**
     * A swizzle function for the w, y, y, and z components of the vector
     * @return a new 4D vector with the corresponding w, y, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYYZ() const
    {
        return {w, y, y, z};
    }

    /**
     * A swizzle function for the w, y, y, and w components of the vector
     * @return a new 4D vector with the corresponding w, y, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYYW() const
    {
        return {w, y, y, w};
    }

    /**
     * A swizzle function for the w, y, z, and x components of the vector
     * @return a new 4D vector with the corresponding w, y, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYZX() const
    {
        return {w, y, z, x};
    }

    /**
     * A swizzle function for the w, y, z, and y components of the vector
     * @return a new 4D vector with the corresponding w, y, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYZY() const
    {
        return {w, y, z, y};
    }

    /**
     * A swizzle function for the w, y, z, and z components of the vector
     * @return a new 4D vector with the corresponding w, y, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYZZ() const
    {
        return {w, y, z, z};
    }

    /**
     * A swizzle function for the w, y, z, and w components of the vector
     * @return a new 4D vector with the corresponding w, y, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYZW() const
    {
        return {w, y, z, w};
    }

    /**
     * A swizzle function for the w, y, w, and x components of the vector
     * @return a new 4D vector with the corresponding w, y, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYWX() const
    {
        return {w, y, w, x};
    }

    /**
     * A swizzle function for the w, y, w, and y components of the vector
     * @return a new 4D vector with the corresponding w, y, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYWY() const
    {
        return {w, y, w, y};
    }

    /**
     * A swizzle function for the w, y, w, and z components of the vector
     * @return a new 4D vector with the corresponding w, y, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYWZ() const
    {
        return {w, y, w, z};
    }

    /**
     * A swizzle function for the w, y, w, and w components of the vector
     * @return a new 4D vector with the corresponding w, y, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WYWW() const
    {
        return {w, y, w, w};
    }

    /**
     * A swizzle function for the w, z, x, and x components of the vector
     * @return a new 4D vector with the corresponding w, z, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZXX() const
    {
        return {w, z, x, x};
    }

    /**
     * A swizzle function for the w, z, x, and y components of the vector
     * @return a new 4D vector with the corresponding w, z, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZXY() const
    {
        return {w, z, x, y};
    }

    /**
     * A swizzle function for the w, z, x, and z components of the vector
     * @return a new 4D vector with the corresponding w, z, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZXZ() const
    {
        return {w, z, x, z};
    }

    /**
     * A swizzle function for the w, z, x, and w components of the vector
     * @return a new 4D vector with the corresponding w, z, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZXW() const
    {
        return {w, z, x, w};
    }

    /**
     * A swizzle function for the w, z, y, and x components of the vector
     * @return a new 4D vector with the corresponding w, z, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZYX() const
    {
        return {w, z, y, x};
    }

    /**
     * A swizzle function for the w, z, y, and y components of the vector
     * @return a new 4D vector with the corresponding w, z, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZYY() const
    {
        return {w, z, y, y};
    }

    /**
     * A swizzle function for the w, z, y, and z components of the vector
     * @return a new 4D vector with the corresponding w, z, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZYZ() const
    {
        return {w, z, y, z};
    }

    /**
     * A swizzle function for the w, z, y, and w components of the vector
     * @return a new 4D vector with the corresponding w, z, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZYW() const
    {
        return {w, z, y, w};
    }

    /**
     * A swizzle function for the w, z, z, and x components of the vector
     * @return a new 4D vector with the corresponding w, z, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZZX() const
    {
        return {w, z, z, x};
    }

    /**
     * A swizzle function for the w, z, z, and y components of the vector
     * @return a new 4D vector with the corresponding w, z, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZZY() const
    {
        return {w, z, z, y};
    }

    /**
     * A swizzle function for the w, z, z, and z components of the vector
     * @return a new 4D vector with the corresponding w, z, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZZZ() const
    {
        return {w, z, z, z};
    }

    /**
     * A swizzle function for the w, z, z, and w components of the vector
     * @return a new 4D vector with the corresponding w, z, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZZW() const
    {
        return {w, z, z, w};
    }

    /**
     * A swizzle function for the w, z, w, and x components of the vector
     * @return a new 4D vector with the corresponding w, z, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZWX() const
    {
        return {w, z, w, x};
    }

    /**
     * A swizzle function for the w, z, w, and y components of the vector
     * @return a new 4D vector with the corresponding w, z, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZWY() const
    {
        return {w, z, w, y};
    }

    /**
     * A swizzle function for the w, z, w, and z components of the vector
     * @return a new 4D vector with the corresponding w, z, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZWZ() const
    {
        return {w, z, w, z};
    }

    /**
     * A swizzle function for the w, z, w, and w components of the vector
     * @return a new 4D vector with the corresponding w, z, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WZWW() const
    {
        return {w, z, w, w};
    }

    /**
     * A swizzle function for the w, w, x, and x components of the vector
     * @return a new 4D vector with the corresponding w, w, x, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWXX() const
    {
        return {w, w, x, x};
    }

    /**
     * A swizzle function for the w, w, x, and y components of the vector
     * @return a new 4D vector with the corresponding w, w, x, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWXY() const
    {
        return {w, w, x, y};
    }

    /**
     * A swizzle function for the w, w, x, and z components of the vector
     * @return a new 4D vector with the corresponding w, w, x, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWXZ() const
    {
        return {w, w, x, z};
    }

    /**
     * A swizzle function for the w, w, x, and w components of the vector
     * @return a new 4D vector with the corresponding w, w, x, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWXW() const
    {
        return {w, w, x, w};
    }

    /**
     * A swizzle function for the w, w, y, and x components of the vector
     * @return a new 4D vector with the corresponding w, w, y, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWYX() const
    {
        return {w, w, y, x};
    }

    /**
     * A swizzle function for the w, w, y, and y components of the vector
     * @return a new 4D vector with the corresponding w, w, y, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWYY() const
    {
        return {w, w, y, y};
    }

    /**
     * A swizzle function for the w, w, y, and z components of the vector
     * @return a new 4D vector with the corresponding w, w, y, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWYZ() const
    {
        return {w, w, y, z};
    }

    /**
     * A swizzle function for the w, w, y, and w components of the vector
     * @return a new 4D vector with the corresponding w, w, y, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWYW() const
    {
        return {w, w, y, w};
    }

    /**
     * A swizzle function for the w, w, z, and x components of the vector
     * @return a new 4D vector with the corresponding w, w, z, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWZX() const
    {
        return {w, w, z, x};
    }

    /**
     * A swizzle function for the w, w, z, and y components of the vector
     * @return a new 4D vector with the corresponding w, w, z, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWZY() const
    {
        return {w, w, z, y};
    }

    /**
     * A swizzle function for the w, w, z, and z components of the vector
     * @return a new 4D vector with the corresponding w, w, z, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWZZ() const
    {
        return {w, w, z, z};
    }

    /**
     * A swizzle function for the w, w, z, and w components of the vector
     * @return a new 4D vector with the corresponding w, w, z, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWZW() const
    {
        return {w, w, z, w};
    }

    /**
     * A swizzle function for the w, w, w, and x components of the vector
     * @return a new 4D vector with the corresponding w, w, w, and x components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWWX() const
    {
        return {w, w, w, x};
    }

    /**
     * A swizzle function for the w, w, w, and y components of the vector
     * @return a new 4D vector with the corresponding w, w, w, and y components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWWY() const
    {
        return {w, w, w, y};
    }

    /**
     * A swizzle function for the w, w, w, and z components of the vector
     * @return a new 4D vector with the corresponding w, w, w, and z components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWWZ() const
    {
        return {w, w, w, z};
    }

    /**
     * A swizzle function for the w, w, w, and w components of the vector
     * @return a new 4D vector with the corresponding w, w, w, and w components of the original
     * vector
     */
    inline constexpr Vec<T, 4> WWWW() const
    {
        return {w, w, w, w};
    }

    /**
     * Computes the vector's x component and returns a zero vector with the x component filled in
     * @return a new zero vector with the x component of the vector
     */
    inline constexpr Vec<T, 4> XComp()
    {
        return {x, 0, 0, 0};
    };

    /**
     * Computes the vector's y component and returns a zero vector with the y component filled in
     * @return a new zero vector with the y component of the vector
     */
    inline constexpr Vec<T, 4> YComp()
    {
        return {0, y, 0, 0};
    };

    /**
     * Computes the vector's z component and returns a zero vector with the z component filled in
     * @return a new zero vector with the z component of the vector
     */
    inline constexpr Vec<T, 4> ZComp()
    {
        return {0, 0, z, 0};
    };

    /**
     * Computes the vector's w component and returns a zero vector with the w component filled in
     * @return a new zero vector with the w component of the vector
     */
    inline constexpr Vec<T, 4> WComp()
    {
        return {0, 0, 0, w};
    };

    T x {0}, y {0}, z {0}, w {0};
};

/**
 * The addition operator between two 4D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector addition
 */
template<typename T>
inline constexpr Vec<T, 4> operator+(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
{
    return Vec<T, 4>::Add(lhs, rhs);
}

/**
 * The addition operator between a 4D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to add to the vector
 * @return a new vector with the result of the vector addition
 */
template<typename T>
inline constexpr Vec<T, 4> operator+(const Vec<T, 4>& lhs, const T& scalar)
{
    return Vec<T, 4>::Add(lhs, scalar);
}

/**
 * The subtraction operator between two 4D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector subtraction
 */
template<typename T>
inline constexpr Vec<T, 4> operator-(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
{
    return Vec<T, 4>::Subtract(lhs, rhs);
}

/**
 * The subtraction operator between a 4D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to subtract from the vector
 * @return a new vector with the result of the vector subtraction
 */
template<typename T>
inline constexpr Vec<T, 4> operator-(const Vec<T, 4>& lhs, const T& scalar)
{
    return Vec<T, 4>::Subtract(lhs, scalar);
}

/**
 * The multiplication operator between two 4D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 4> operator*(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
{
    return Vec<T, 4>::Multiply(lhs, rhs);
}

/**
 * The multiplication operator between a 4D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to multiply the vector by
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 4> operator*(const Vec<T, 4>& lhs, T scalar)
{
    return Vec<T, 4>::Multiply(lhs, scalar);
}

/**
 * The multiplication operator between a 4D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param scalar the scalar value to multiply the vector by
 * @param lhs the vector on the left hand side of the expression
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 4> operator*(T scalar, const Vec<T, 4>& rhs)
{
    return Vec<T, 4>::Multiply(rhs, scalar);
}

/**
 * The negation operator
 * @tparam T the type of numerical value held by the vectors
 * @param vector the vector to negate
 * @return a new vector with all values negated
 */
template<typename T>
inline constexpr Vec<T, 4> operator-(const Vec<T, 4>& vector)
{
    return {-vector.x, -vector.y, -vector.z, -vector.w};
}

/**
 * The division operator between a 4D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to divide the vector by
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 4> operator/(const Vec<T, 4>& lhs, T scalar)
{
    return Vec<T, 4>::Divide(lhs, scalar);
}

/**
 * The division operator between a 4D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param scalar the scalar value to divide the vector by
 * @param lhs the vector on the left hand side of the expression
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 4> operator/(T scalar, const Vec<T, 4>& rhs)
{
    return Vec<T, 4>::Divide(rhs, scalar);
}

/**
 * The division operator between two 4D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 4> operator/(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs)
{
    return Vec<T, 4>::Divide(lhs, rhs);
}
} // namespace Siege

#endif // SIEGE_ENGINE_VEC4_H
