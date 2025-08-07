//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STATICMESHDATA_H
#define SIEGE_ENGINE_STATICMESHDATA_H

#include <utils/BinarySerialisation.h>
#include <utils/Colour.h>
#include <utils/math/vec/Hashing.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>

#include "PackFile.h"

namespace Siege
{

struct BaseVertex
{
    Vec3 position;
    FColour color;
    Vec3 normal;
    Vec2 uv;
};

inline bool operator==(const BaseVertex& left, const BaseVertex& right)
{
    return left.position == right.position && left.color == right.color &&
           left.normal == right.normal && left.uv == right.uv;
}

inline bool operator!=(const BaseVertex& left, const BaseVertex& right)
{
    return !(left == right);
}

struct StaticMeshData
{
    std::vector<uint32_t> indices;
    std::vector<BaseVertex> vertices;
};

namespace BinarySerialisation
{

inline void serialise(BinarySerialisation::Buffer& buffer,
                      BaseVertex& value,
                      BinarySerialisation::SerialisationMode mode)
{
    serialise(buffer, value.position, mode);
    serialise(buffer, value.color, mode);
    serialise(buffer, value.normal, mode);
    serialise(buffer, value.uv, mode);
}

inline void serialise(Buffer& buffer, StaticMeshData& value, SerialisationMode mode)
{
    serialise(buffer, value.indices, mode);
    serialise(buffer, value.vertices, mode);
}

} // namespace BinarySerialisation

} // namespace Siege

namespace std
{

template<>
struct hash<Siege::BaseVertex>
{
    size_t operator()(const Siege::BaseVertex& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    };
};

} // namespace std

#endif // SIEGE_ENGINE_STATICMESHDATA_H
