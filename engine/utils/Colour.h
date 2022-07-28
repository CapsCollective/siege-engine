//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COLOUR_H
#define SIEGE_ENGINE_COLOUR_H

#include "Hash.h"

namespace Siege
{
struct Colour
{
    // Public constants

    static const Colour Black;

    static const Colour White;

    static const Colour Red;

    static const Colour Green;

    static const Colour Blue;

    static const Colour Pink;

    static const Colour Blackf;

    static const Colour Whitef;

    static const Colour Redf;

    static const Colour Greenf;

    static const Colour Bluef;

    static const Colour Pinkf;

    // 'Structors

    Colour() : Colour(0, 0, 0, 0) {}

    Colour(int r, int g, int b) : r8(r), g8(g), b8(b), a8(255) {}

    Colour(int r, int g, int b, int a) : r8(r), g8(g), b8(b), a8(a) {}

    Colour(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {}

    Colour(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    bool operator==(const Colour& rhs) const;

    // Public members

    // Unions are in place for now to translate between raylib and render. This should be removed
    // once raylib is removed from the project.
    union
    {
        float r;
        unsigned char r8;
    };
    union
    {
        float g;
        unsigned char g8;
    };
    union
    {
        float b;
        unsigned char b8;
    };
    union
    {
        float a;
        unsigned char a8;
    };
};
} // namespace Siege

template<>
struct std::hash<Siege::Colour>
{
    size_t operator()(const Siege::Colour& colour) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed,
                                 colour.r,
                                 colour.g,
                                 colour.b,
                                 colour.a,
                                 colour.r8,
                                 colour.g8,
                                 colour.b8,
                                 colour.a8);
        return seed;
    };
};

#endif // SIEGE_ENGINE_COLOUR_H
