//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PACKER_PACKERUTILS_H
#define SIEGE_ENGINE_PACKER_PACKERUTILS_H

#include <assimp/types.h>
#include <utils/math/mat/Mat4.h>

static constexpr Siege::Mat4 AssimpMat4ToMat4(const aiMatrix4x4& matrix)
{
    return {{matrix.a1, matrix.b1, matrix.c1, matrix.d1},
            {matrix.a2, matrix.b2, matrix.c2, matrix.d2},
            {matrix.a3, matrix.b3, matrix.c3, matrix.d3},
            {matrix.a4, matrix.b4, matrix.c4, matrix.d4}};
}

#endif // SIEGE_ENGINE_PACKER_PACKERUTILS_H
