//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VEC2_H
#define SIEGE_ENGINE_VEC2_H

#include "Vec.h"

namespace Siege
{
template<typename T>
struct Vec<T, 2>
{
    // Static functions

    /**
     * @returns a new 2D vector with all elements set to zero
     */
    static inline constexpr Vec<T, 2> Zero()
    {
        return Vec<T, 2> {};
    }

    /**
     * @returns a new 2D vector with all elements set to one
     */
    static inline constexpr Vec<T, 2> One()
    {
        return {1.f, 1.f};
    }

    /**
     * @returns a new 2D vector which denotes the world right direction
     */
    static inline constexpr Vec<T, 2> Right()
    {
        return Vec<T, 2> {1, 0};
    }

    /**
     * @returns a new 2D vector which denotes the world up direction
     */
    static inline constexpr Vec<T, 2> Up()
    {
        return Vec<T, 2> {0, -1};
    }

    /**
     * Adds two 2D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector addition
     */
    static inline constexpr Vec<T, 2> Add(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    /**
     * Adds a scalar value to a 2D vector and returns the result
     * @param vector the 2D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar addition
     */
    static inline constexpr Vec<T, 2> Add(const Vec<T, 2>& vector, T scalar)
    {
        return {vector.x + scalar, vector.y + scalar};
    }

    /**
     * Subtracts two 2D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector subtraction
     */
    static inline constexpr Vec<T, 2> Subtract(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    /**
     * Subtracts a scalar value to a 2D vector and returns the result
     * @param vector the 2D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar subtraction
     */
    static inline constexpr Vec<T, 2> Subtract(const Vec<T, 2>& vector, T scalar)
    {
        return {vector.x - scalar, vector.y - scalar};
    }

    /**
     * Multiplies a 2D vector by a scalar value and returns the result
     * @param vector the 2D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar multiplication
     */
    static inline constexpr Vec<T, 2> Multiply(const Vec<T, 2>& vector, T scalar)
    {
        return {vector.x * scalar, vector.y * scalar};
    }

    /**
     * Multiplies two 2D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector multiplication
     */
    static inline constexpr Vec<T, 2> Multiply(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
    {
        return {lhs.x * rhs.x, lhs.y * rhs.y};
    }

    /**
     * Divides a 2D vector by a scalar value and returns the result
     * @param vector the 2D vector to scale
     * @param scalar the scalar value to scale by
     * @returns the result of the scalar division
     */
    static inline constexpr Vec<T, 2> Divide(const Vec<T, 2>& vector, T scalar)
    {
        return {vector.x / scalar, vector.y / scalar};
    }

    /**
     * Divides two 2D vectors and returns the result
     * @param lhs the vector on the left hand side of the operation
     * @param rhs the vector on the right hand side of the operation
     * @returns the result of the vector division
     */
    static inline constexpr Vec<T, 2> Divide(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
    {
        return {lhs.x / rhs.x, lhs.y / rhs.y};
    }

    /**
     * Computes the Dot product of two 2D vectors
     * @param lhs the vector on the left hand of the operation
     * @param rhs the vector on the right hand of the operation
     * @return the dot product of the two inputted vectors
     */
    static inline constexpr T Dot(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
    {
        return (lhs.x * rhs.x) + (lhs.y * rhs.y);
    }

    /**
     * Computes the length of a 2D vector
     * @param vector the vector to be calculated
     * @return the length of the inputted vector
     */
    static inline constexpr float Length(const Vec<T, 2>& vector)
    {
        return Float::FastSqrt((vector.x * vector.x) + (vector.y * vector.y));
    }

    /**
     * Normalises a 2D vector so that all values range between 0 and 1
     * @param vector the vector to be normalised
     * @return a new 2D vector with the normalised values
     */
    static inline constexpr Vec<T, 2> Normalise(const Vec<T, 2>& vector)
    {
        return Vec<T, 2>::Multiply(vector, 1.f / Length(vector));
    }

    /**
     * Computes the cos of every element of a provided 2D vector
     * @param vec the vector to be computed
     * @return a new vector with the cos of each value
     */
    static inline constexpr Vec<T, 2> Cos(const Vec<T, 2>& vec)
    {
        return {Float::Cos(vec.x), Float::Cos(vec.y)};
    }

    /**
     * Computes the sin of every element of a provided 2D vector
     * @param vec the vector to be computed
     * @return a new vector with the sin of each value
     */
    static inline constexpr Vec<T, 2> Sin(const Vec<T, 2>& vec)
    {
        return {Float::Sin(vec.x), Float::Sin(vec.y)};
    }

    /**
     * Computes the tan of every element of a provided 2D vector
     * @param vec the vector to be computed
     * @return a new vector with the tan of each value
     */
    static inline constexpr Vec<T, 2> Tan(const Vec<T, 2>& vec)
    {
        return {Float::Tan(vec.x), Float::Tan(vec.y)};
    }

    /**
     * Linearly interpolates between two 2D vectors
     * @param origin the vector to begin from
     * @param destination the vector end-result
     * @param time the proportion of movement to be done from origin to destination
     * @return a new vector with the results of the linear interpolation
     */
    static inline constexpr Vec<T, 2> Lerp(const Vec<T, 2>& origin,
                                           const Vec<T, 2>& destination,
                                           T time)
    {
        return origin * (1.0f - time) + destination * time;
    }

    // 'Structors

    /**
     * The Vec<T, 2> default constructor
     */
    inline constexpr Vec<T, 2>() = default;

    /**
     * The base Vec<T, 2> constructor
     * @param x the x axis value
     * @param y the y axis value
     */
    inline constexpr Vec<T, 2>(T x, T y) : x {x}, y {y} {}

    /**
     * The partial Vec<T, 2> constructor. Sets the x value and defaults the y value to 0
     * @param x the value of the x axis
     */
    inline constexpr Vec<T, 2>(T x) : Vec<T, 2>(x, 0) {}

    /**
     * The 3D vector constructor. Takes the x and y axes from the provided vector and returns a new
     * Vec<T, 2>
     * @param other the 3D vector to convert from
     */
    inline constexpr Vec<T, 2>(const Vec<T, 3>& other) : Vec<T, 2>(other.x, other.y) {}

    /**
     * The 4D vector constructor. Takes the x and y axes from the provided vector and returns a new
     * Vec<T, 2>
     * @param other the 4D vector to convert from
     */
    inline constexpr Vec<T, 2>(const Vec<T, 4>& other) : Vec<T, 2>(other.x, other.y) {}

    // Operator Overloads

    /**
     * The 2D vector subscript operator
     * @param index the index of the requested dimension
     * @return a reference to the requested value
     */
    inline constexpr T& operator[](unsigned int index)
    {
        switch (index)
        {
            default:
            case (0):
                return x;
            case (1):
                return y;
        }
    }

    /**
     * The const 2D vector subscript operator
     * @param index the index of the requested dimension
     * @return a copy of the requested value
     */
    inline constexpr T operator[](unsigned int index) const
    {
        return ((index == 0) * x) + ((index == 1) * y);
    }

    /**
     * The equality operator. Checks if two 2D vectors are equal on all axes
     * @param rhs the 2D vector to compare
     * @return a boolean specifying if the vectors are equal
     */
    inline constexpr bool operator==(const Vec<T, 2>& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    /**
     * The inequality operator. Checks if two 2D vectors are not equal on all axes
     * @param rhs the 2D vector to compare
     * @return a boolean specifying if the vectors are not equal
     */
    inline constexpr bool operator!=(const Vec<T, 2>& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    /**
     * The scalar addition operator. Adds the provided scalar value to the 2D vector
     * @param scalar the scalar value to add
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator+=(T scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }

    /**
     * The addition operator. Adds the values of a 2D vector to the current vector
     * @param rhs the vector to add values from
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator+=(const Vec<T, 2>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    /**
     * The scalar subtraction operator. Subtracts the provided scalar value from the 2D vector
     * @param scalar the scalar value to subtract
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator-=(T scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    /**
     * The subtraction operator. Subtracts the values of a 2D vector from the current vector
     * @param rhs the vector to subtract values from
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator-=(const Vec<T, 2>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    /**
     * The scalar multiplication operator. Multiplies the provided scalar value from the 2D vector
     * @param scalar the scalar value to multiply by
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    /**
     * The multiplication operator. Multiplies the values of a 2D vector by the current vector
     * @param rhs the vector to multiply values from
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator*=(const Vec<T, 2>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    /**
     * The scalar division operator. Divides the 2D vector by the provided scalar value
     * @param scalar the scalar value to divide by
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    /**
     * The division operator. Divides the values of the 2D vector by the provided vector
     * @param rhs the vector to divide values by
     * @return a reference to the current Vec<T, 2>
     */
    inline constexpr Vec<T, 2>& operator/=(const Vec<T, 2>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    /**
     * The Negation operator. Negates the values in the vector
     * @return a new vector with the negated vector values
     */
    inline constexpr Vec<T, 2>& operator-()
    {
        x = -x;
        y = -y;
        return *this;
    }

    // Swizzle function + getters

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
     * Computes the vector's x component and returns a zero vector with the x component filled in
     * @return a new zero vector with the x component of the vector
     */
    inline constexpr Vec<T, 2> XComp()
    {
        return {x, 0};
    }

    /**
     * Computes the vector's y component and returns a zero vector with the y component filled in
     * @return a new zero vector with the y component of the vector
     */
    inline constexpr Vec<T, 2> YComp()
    {
        return {0, y};
    }

    T x {0.f}, y {0.f};
};

/**
 * The addition operator between two 2D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector addition
 */
template<typename T>
inline constexpr Vec<T, 2> operator+(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
{
    return Vec<T, 2>::Add(lhs, rhs);
}

/**
 * The addition operator between a 2D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to add to the vector
 * @return a new vector with the result of the vector addition
 */
template<typename T>
inline constexpr Vec<T, 2> operator+(const Vec<T, 2>& lhs, T scalar)
{
    return Vec<T, 2>::Add(lhs, scalar);
}

/**
 * The subtraction operator between two 2D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector subtraction
 */
template<typename T>
inline constexpr Vec<T, 2> operator-(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
{
    return Vec<T, 2>::Subtract(lhs, rhs);
}

/**
 * The subtraction operator between a 2D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to subtract from the vector
 * @return a new vector with the result of the vector subtraction
 */
template<typename T>
inline constexpr Vec<T, 2> operator-(const Vec<T, 2>& lhs, T scalar)
{
    return Vec<T, 2>::Subtract(lhs, scalar);
}

/**
 * The multiplication operator between two 2D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 2> operator*(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
{
    return Vec<T, 2>::Multiply(lhs, rhs);
}

/**
 * The multiplication operator between a 2D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to multiply the vector by
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 2> operator*(const Vec<T, 2>& lhs, T scalar)
{
    return Vec<T, 2>::Multiply(lhs, scalar);
}

/**
 * The multiplication operator between a 2D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param scalar the scalar value to multiply the vector by
 * @param lhs the vector on the left hand side of the expression
 * @return a new vector with the result of the vector multiplication
 */
template<typename T>
inline constexpr Vec<T, 2> operator*(T scalar, const Vec<T, 2>& rhs)
{
    return Vec<T, 2>::Multiply(rhs, scalar);
}

/**
 * The division operator between two 2D vectors
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param rhs the vector on the right hand side of the expression
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 2> operator/(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs)
{
    return Vec<T, 2>::Divide(lhs, rhs);
}

/**
 * The division operator between a 2D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param lhs the vector on the left hand side of the expression
 * @param scalar the scalar value to divide the vector by
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 2> operator/(const Vec<T, 2>& lhs, T scalar)
{
    return Vec<T, 2>::Divide(lhs, scalar);
}

/**
 * The division operator between a 2D vector and a scalar value
 * @tparam T the type of numerical value held by the vectors
 * @param scalar the scalar value to divide the vector by
 * @param lhs the vector on the left hand side of the expression
 * @return a new vector with the result of the vector division
 */
template<typename T>
inline constexpr Vec<T, 2> operator/(T scalar, const Vec<T, 2>& rhs)
{
    return Vec<T, 2>::Divide(rhs, scalar);
}

/**
 * The negation operator
 * @tparam T the type of numerical value held by the vectors
 * @param vector the vector to negate
 * @return a new vector with all values negated
 */
template<typename T>
inline constexpr Vec<T, 2> operator-(const Vec<T, 2>& vector)
{
    return {-vector.x, -vector.y};
}
} // namespace Siege

#endif // SIEGE_ENGINE_VEC2_H
