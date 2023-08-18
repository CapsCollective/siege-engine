//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_BASEVERTEX_H
#define SIEGE_ENGINE_RENDER_BASEVERTEX_H

#include <utils/Colour.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>

namespace Siege
{
struct BaseVertex
{
    Vec3 position;
    FColour color;
    Vec3 normal;
    Vec2 uv;
};
} // namespace Siege

#endif // SIEGE_ENGINE_BASEVERTEX_H
