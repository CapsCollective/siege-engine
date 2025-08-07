//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COLOUR_H
#define SIEGE_ENGINE_COLOUR_H

#include <cstdint>
#include <type_traits>

#include "Hash.h"
#include "String.h"

namespace Siege
{
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
struct Colour;

typedef struct Colour<float> FColour;
typedef struct Colour<uint8_t> IColour;

/**
 * An 8-bit unsigned integer based Colour struct. Every colour channel in this struct ranges between
 * 0 and 255, with 255 being the brightest colour channel
 */
template<>
struct Colour<uint8_t>
{
    // Static constants

    static const IColour White;
    static const IColour Black;
    static const IColour Red;
    static const IColour Green;
    static const IColour Blue;
    static const IColour Pink;

    // 'Structors

    /**
     * The base constructor for the colour
     * @param red the red channel value
     * @param green the green channel value
     * @param blue the blue channel value
     * @param alpha the alpha channel value
     */
    inline constexpr Colour(uint8_t red = 0,
                            uint8_t green = 0,
                            uint8_t blue = 0,
                            uint8_t alpha = 0) :
        r {red},
        g {green},
        b {blue},
        a {alpha}
    {}

    /**
     * A conversion from a float-based Colour struct.
     * @warn this does not convert the normalised values of an FColour into an IColour. Please be
     * careful with how you convert between these two. If you want a function which converts the
     * normalised value of the FColour to a 0 - 255 range, please use the ToIColour() method
     * @param other
     */
    Colour(const FColour& other);

    /**
     * A copy constructor for the IColour struct
     * @param other the IColour to copy over
     */
    inline constexpr Colour(const IColour& other) :
        r {other.r},
        g {other.g},
        b {other.b},
        a {other.a}
    {}

    /**
     * An equality operator
     * @param other the Colour to compare to
     * @return true of they are the same, false if they aren't
     */
    inline constexpr bool operator==(const IColour& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    /**
     * An in-equality operator
     * @param other the Colour to compare to
     * @return false of they are the same, true if they aren't
     */
    inline constexpr bool operator!=(const IColour& other) const
    {
        return r == other.r || g == other.g || b == other.b || a == other.a;
    }

    // Public members

    uint8_t r {0};
    uint8_t g {0};
    uint8_t b {0};
    uint8_t a {0};
};

inline constexpr IColour IColour::White {255, 255, 255, 255};
inline constexpr IColour IColour::Black {0, 0, 0, 255};
inline constexpr IColour IColour::Red {255, 0, 0, 255};
inline constexpr IColour IColour::Green {0, 255, 0, 255};
inline constexpr IColour IColour::Blue {0, 0, 255, 255};
inline constexpr IColour IColour::Pink {255, 109, 194, 255};

template<>
struct Colour<float>
{
    // Static constants

    static const FColour White;
    static const FColour Black;
    static const FColour Red;
    static const FColour Green;
    static const FColour Blue;
    static const FColour Pink;

    /**
     * The base constructor for the colour
     * @param red the red channel value
     * @param green the green channel value
     * @param blue the blue channel value
     * @param alpha the alpha channel value
     */
    inline constexpr Colour(float red = 0.f,
                            float green = 0.f,
                            float blue = 0.f,
                            float alpha = 0.f) :
        r {red},
        g {green},
        b {blue},
        a {alpha}
    {}

    /**
     * A copy constructor for the FColour struct
     * @param other the FColour to copy over
     */
    inline constexpr Colour(const FColour& other) :
        r {other.r},
        g {other.g},
        b {other.b},
        a {other.a}
    {}

    /**
     * A conversion from an 8-bit unsigned integer based Colour struct.
     * @warn this does not convert the normalised values of an FColour into an IColour. Please be
     * careful with how you convert between these two. If you want a function which converts the
     * normalised value of the FColour to a 0 - 255 range, please use the ToFColour() method
     * @param other
     */
    inline constexpr Colour(const IColour& other) :
        r {static_cast<float>(other.r)},
        g {static_cast<float>(other.g)},
        b {static_cast<float>(other.b)},
        a {static_cast<float>(other.a)}
    {}

    /**
     * An equality operator
     * @param other the Colour to compare to
     * @return true of they are the same, false if they aren't
     */
    inline constexpr bool operator==(const FColour& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    /**
     * An in-equality operator
     * @param other the Colour to compare to
     * @return false of they are the same, true if they aren't
     */
    inline constexpr bool operator!=(const FColour& other) const
    {
        return r == other.r || g == other.g || b == other.b || a == other.a;
    }

    float r {0.f};
    float g {0.f};
    float b {0.f};
    float a {0.f};
};

// FColour constants

inline constexpr FColour FColour::White = {1.f, 1.f, 1.f, 1.f};
inline constexpr FColour FColour::Black = {0.f, 0.f, 0.f, 1.f};
inline constexpr FColour FColour::Red = {1.f, 0.f, 0.f, 1.f};
inline constexpr FColour FColour::Green = {0.f, 1.f, 0.f, 1.f};
inline constexpr FColour FColour::Blue = {0.f, 0.f, 1.f, 1.f};
inline constexpr FColour FColour::Pink = {1.f, 0.427450980392, 0.760784313725, 1.f};

// Conversion functions

/**
 * Converts an IColour to an FColour. This function is different to the standard constructors in
 * that it takes the 0 to 255 range of the IColour and normalises it between 0 and 1 (as a float).
 * This Method is useful for converting 8 bit Colours to float colours in an accurate manner. This
 * is the recommended approach for converting the two colours
 * @param other the IColour to compare
 * @return an FColour with the normalised values
 */
inline constexpr FColour ToFColour(const IColour& other)
{
    return {(static_cast<float>(other.r) - 0.f) / (255.f - 0.f),
            (static_cast<float>(other.g) - 0.f) / (255.f - 0.f),
            (static_cast<float>(other.b) - 0.f) / (255.f - 0.f),
            (static_cast<float>(other.a) - 0.f) / (255.f - 0.f)};
}

/**
 * Converts an FColour to an IColour. This function is different to the standard constructors in
 * that it takes the 0 to 1 range of the FColour and expands it between 0 and 255 (as a uint8_t).
 * This Method is useful for converting 8 bit Colours to float colours in an accurate manner. This
 * is the recommended approach for converting the two colours
 * @param other the FColour to convert
 * @return an IColour with the expanded values
 */
inline constexpr FColour ToIColour(const FColour& other)
{
    return {static_cast<float>(other.r * 255),
            static_cast<float>(other.g * 255),
            static_cast<float>(other.b * 255),
            static_cast<float>(other.a * 255)};
}

/**
 * Converts an integer colour to a string
 * @param colour the colour to convert to a string
 * @return a string representation of the colour
 */
inline String ToString(const IColour& colour)
{
    return String("%d,%d,%d,%d").Formatted(colour.r, colour.g, colour.b, colour.a);
}


/**
 * Converts a float colour to a string
 * @param colour the colour to convert to a string
 * @return a string representation of the colour
 */
inline String ToString(const FColour& colour)
{
    return String("%.2f,%.2f,%.2f,%.2f").Formatted(colour.r, colour.g, colour.b, colour.a);
}


} // namespace Siege

namespace std
{
template<>
struct hash<Siege::FColour>
{
    size_t operator()(const Siege::FColour& colour) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, colour.r, colour.g, colour.b, colour.a);
        return seed;
    };
};

template<>
struct hash<Siege::IColour>
{
    size_t operator()(const Siege::IColour& colour) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, colour.r, colour.g, colour.b, colour.a);
        return seed;
    };
};
} // namespace std

#endif // SIEGE_ENGINE_COLOUR_H
