//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Colour.h"

namespace Siege
{
// Define static members
const Colour Colour::Black = {0, 0, 0, 255};
const Colour Colour::White = {255, 255, 255, 255};
const Colour Colour::Red = {230, 41, 55, 255};
const Colour Colour::Green = {0, 228, 48, 255};
const Colour Colour::Blue = {0, 121, 241, 255};
const Colour Colour::Pink = {255, 109, 194, 255};

const Colour Colour::Blackf = {0.f, 0.f, 0.f, 1.f};
const Colour Colour::Whitef = {1.f, 1.f, 1.f, 1.f};
const Colour Colour::Redf = {0.90f, 0.16f, 0.21f, 1.f};
const Colour Colour::Greenf = {0.f, 0.89f, 0.18f, 1.f};
const Colour Colour::Bluef = {0.f, 0.47f, 0.94f, 1.f};
const Colour Colour::Pinkf = {1.f, 0.42f, 0.76f, 1.f};

bool Colour::operator==(const Colour& rhs) const
{
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a && r8 == rhs.r8 && g8 == rhs.g8 &&
           b8 == rhs.b8 && a8 == rhs.a8;
}
} // namespace Siege