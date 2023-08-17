//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Colour.h"

namespace Siege
{
Colour<uint8_t>::Colour(const FColour& other) :
    r {static_cast<uint8_t>(other.r)},
    g {static_cast<uint8_t>(other.g)},
    b {static_cast<uint8_t>(other.b)},
    a {static_cast<uint8_t>(other.a)}
{}
} // namespace Siege