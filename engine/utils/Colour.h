//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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

namespace Siege
{
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
struct Colour;

typedef Colour<float> FColour;
typedef Colour<uint8_t> IColour;

template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type*>
struct Colour
{
    // 'Structors

    inline constexpr Colour(T red = 0, T green = 0, T blue = 0, T alpha = 0) :
        r {red},
        g {green},
        b {blue},
        a {alpha}
    {}

    inline constexpr Colour(const FColour& other) :
        r {static_cast<T>(other.r)},
        g {static_cast<T>(other.g)},
        b {static_cast<T>(other.b)},
        a {static_cast<T>(other.a)}
    {}

    inline constexpr Colour(const IColour& other) :
        r {static_cast<T>(other.r)},
        g {static_cast<T>(other.g)},
        b {static_cast<T>(other.b)},
        a {static_cast<T>(other.a)}
    {}

    inline constexpr bool operator==(const FColour& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    inline constexpr bool operator==(const IColour& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    inline constexpr bool operator!=(const FColour& other) const
    {
        return r == other.r || g == other.g || b == other.b || a == other.a;
    }

    inline constexpr bool operator!=(const IColour& other) const
    {
        return r == other.r || g == other.g || b == other.b || a == other.a;
    }

    // Public members

    T r {0};
    T g {0};
    T b {0};
    T a {0};
};

// FColour constants

inline constexpr FColour FWhite = {1.f, 1.f, 1.f, 1.f};
inline constexpr FColour FBlack = {0.f, 0.f, 0.f, 1.f};
inline constexpr FColour FRed = {1.f, 0.f, 0.f, 1.f};
inline constexpr FColour FGreen = {0.f, 1.f, 0.f, 1.f};
inline constexpr FColour FBlue = {0.f, 0.f, 1.f, 1.f};
inline constexpr FColour FPink = {1.f, 0.427450980392, 0.760784313725, 1.f};

// IColour constants

inline constexpr IColour IWhite = {255, 255, 255, 255};
inline constexpr IColour IBlack = {0, 0, 0, 255};
inline constexpr IColour IRed = {255, 0, 0, 255};
inline constexpr IColour IGreen = {0, 255, 0, 255};
inline constexpr IColour IBlue = {0, 0, 255, 255};
inline constexpr IColour IPink = {255, 109, 194, 255};

// Conversion functions

inline constexpr FColour ToFColour(const IColour& other)
{
    return {(static_cast<float>(other.r) - 0.f) / (255.f - 0.f),
            (static_cast<float>(other.g) - 0.f) / (255.f - 0.f),
            (static_cast<float>(other.b) - 0.f) / (255.f - 0.f),
            (static_cast<float>(other.a) - 0.f) / (255.f - 0.f)};
}

inline constexpr FColour ToIColour(const FColour& other)
{
    return {static_cast<float>(other.r * 255),
            static_cast<float>(other.g * 255),
            static_cast<float>(other.b * 255),
            static_cast<float>(other.a * 255)};
}

} // namespace Siege

namespace std
{
template<>
struct ::std::hash<Siege::FColour>
{
    size_t operator()(const Siege::FColour& colour) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, colour.r, colour.g, colour.b, colour.a);
        return seed;
    };
};

template<>
struct ::std::hash<Siege::IColour>
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
