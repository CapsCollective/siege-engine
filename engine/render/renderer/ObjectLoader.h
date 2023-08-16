//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_OBJECTLOADER_H
#define SIEGE_ENGINE_RENDER_OBJECTLOADER_H

#include <utils/Colour.h>
#include <utils/collections/HeapArray.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>

#include "vertex/BaseVertex.h"

namespace Siege
{
struct ObjectVertexData
{
    static inline constexpr unsigned int INDEX_SIZE = sizeof(unsigned int);

    unsigned int vertexSize {0};
    MHArray<BaseVertex> vertices;
    MHArray<unsigned int> indices;
};

ObjectVertexData LoadObjectFromFile(const char* filePath);

inline bool operator==(const BaseVertex& left, const BaseVertex& right)
{
    return left.position == right.position && left.color == right.color &&
           left.normal == right.normal && left.uv == right.uv;
}
} // namespace Siege

#endif // SIEGE_ENGINE_OBJECTLOADER_H
