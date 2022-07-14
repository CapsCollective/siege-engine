//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COLOUR_H
#define SIEGE_ENGINE_COLOUR_H

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

    // 'Structors

    Colour() : Colour(0, 0, 0, 0) {}

    Colour(int r, int g, int b) : r(r), g(g), b(b), a(255) {}

    Colour(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}

    // Public members

    int r;
    int g;
    int b;
    int a;
};
}

#endif // SIEGE_ENGINE_COLOUR_H
