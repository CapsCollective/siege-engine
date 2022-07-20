//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FLOAT_H
#define SIEGE_ENGINE_FLOAT_H

#include <cmath>

namespace Siege
{
/**
 * A library for computing common float computations.
 */
class Float
{
public:

    /**
     * A const value representing Pi.
     */
    static inline constexpr float Pi {M_PI};

    /**
     * A const value representing Pi * 2.
     */
    static inline constexpr float TwoPi {Pi * 2};

    /**
     * Returns the Epsilon of a C++ float value. Useful for float comparisons.
     * @return a float Epsilon.
     */
    static inline constexpr float Epsilon()
    {
        return std::numeric_limits<float>::epsilon();
    }

    /**
     * Computes the Sin of a give value. Value is assumed to be in radians.
     * @param val the radian value to compute.
     * @return the Sin of the given angle.
     */
    static inline float Sin(float val) noexcept
    {
        return sinf(val);
    }

    /**
     * Computes the Cos of a give value. Value is assumed to be in radians.
     * @param val the radian value to compute.
     * @return the Cos of the given angle.
     */
    static inline float Cos(float val) noexcept
    {
        return cosf(val);
    }

    /**
     * Computes the Tan of a give value. Value is assumed to be in radians.
     * @param val the radian value to compute.
     * @return the Tan of the given angle.
     */
    static inline float Tan(float val) noexcept
    {
        return tanf(val);
    }

    /**
     * Converts a degree value to radians.
     * @param degrees the angle in degrees.
     * @return the radian value of the angle.
     */
    static inline float Radians(float degrees)
    {
        return degrees * (Pi / 180.f);
    }

    /**
     * Converts a radian value to degrees.
     * @param degrees the angle in radians.
     * @return the degree value of the angle.
     */
    static inline float Degrees(float radians)
    {
        return radians * (180 / Pi);
    }

    /**
     * Returns the Sign of the value. Results are returned as follows:
     * If val is less than 0 then return -1.
     * If val equals 0 then return 0.
     * If val is greater than 0 then return 1.
     * @param val tthe value who's sign needs to be calculated.
     * @return the sign of the value.
     */
    static inline int8_t Sign(float val)
    {
        return (0 < val) - (val < 0);
    }

    /**
     * Normalise a -1 to 1 range to 0 - 1.
     * @param value the value to be normalised.
     * @return the normalised value between 0 and 1.
     */
    static inline float Normalise(const float& value)
    {
        return Normalise(value, 0, 1);
    }

    /**
     * Normalise a value between a given minimum and maximum.
     * @param value the value to be normalised.
     * @param min the minimum range.
     * @param max the maximum range.
     * @return the value normalised between -1 and 1.
     */
    static inline float Normalise(float value, float min, float max)
    {
        return 2 * ((value - min) / (max - min)) - 1;
    }

    /**
     * Interpolates between two values over a given delta.
     * @param origin the original value.
     * @param destination the destination (end value by the end of the interpolation)
     * @param time the delta to interpolate to.
     * @return an interpolated value between origin and destination.
     */
    static inline float Lerp(float origin, float destination, float time)
    {
        return origin * (1.0f - time) + destination * time;
    }

    /**
     * Returns the absolute value of a given float.
     * @param value tthe value to be computed.
     * @return the absolute value.
     */
    static inline float Abs(const float& value)
    {
        return (static_cast<float>(-(value) < 0) * value) +
               (static_cast<float>(-(value) > 0) * -value);
    }

    /**
     * Returns the greater of two values.
     * @param lhs the first value.
     * @param rhs the second value.
     * @return the greater of lhs and rhs.
     */
    static inline float Max(const float& lhs, const float& rhs)
    {
        return ((lhs + rhs + Abs(lhs - rhs)) / 2);
    }

    /**
     * Returns the lesser of two values.
     * @param lhs the first value.
     * @param rhs the second value.
     * @return the lesser of lhs and rhs.
     */
    static inline float Min(const float& lhs, const float& rhs)
    {
        return ((lhs + rhs - Abs(lhs - rhs)) / 2);
    }

    /**
     * Clamps a value between a given range.
     * @param value the value to be clamped.
     * @param min the miniumum possible result.
     * @param max the maxiumu possible result.
     * @return the clamped value between min and max.
     */
    static inline float Clamp(const float& value, const float& min, const float& max)
    {
        return Min(Max(value, min), max);
    }

    /**
     * Computes the modulus of two values.
     * @param lhs the left hand side.
     * @param rhs the right hand side value.
     * @return the modulus of lhs and rhs.
     */
    static inline float Mod(const float& lhs, const float& rhs)
    {
        return lhs - rhs * floor(lhs / rhs);
    }
};
} // namespace Siege

#endif // SIEGE_ENGINE_FLOAT_H
