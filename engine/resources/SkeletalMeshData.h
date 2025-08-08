//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SKELETALMESHDATA_H
#define SIEGE_ENGINE_SKELETALMESHDATA_H

#include <utils/BinarySerialisation.h>
#include <utils/Colour.h>
#include <utils/math/mat/Mat4.h>
#include <utils/math/vec/Hashing.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>

#include "StaticMeshData.h"

namespace Siege
{
struct Bone
{
    uint32_t id = 0;
    Mat4 bindMatrix;
};

struct SkinnedVertex : BaseVertex
{
    Vec4 bones;
    Vec4 weights;
};

inline bool operator==(const SkinnedVertex& left, const SkinnedVertex& right)
{
    return static_cast<BaseVertex>(left) == static_cast<BaseVertex>(right) &&
           left.bones == right.bones && left.weights == right.weights;
}

inline bool operator!=(const SkinnedVertex& left, const SkinnedVertex& right)
{
    return !(left == right);
}

struct SkeletalMeshData
{
    std::vector<uint32_t> indices;
    std::vector<SkinnedVertex> vertices;
    std::map<String, Bone> bones;
};

namespace BinarySerialisation
{

inline void serialise(Buffer& buffer, Bone& value, SerialisationMode mode)
{
    serialise(buffer, value.id, mode);
    serialise(buffer, value.bindMatrix, mode);
}

inline void serialise(Buffer& buffer, SkinnedVertex& value, SerialisationMode mode)
{
    serialise(buffer, value.color, mode);
    serialise(buffer, value.normal, mode);
    serialise(buffer, value.uv, mode);
    serialise(buffer, value.bones, mode);
    serialise(buffer, value.weights, mode);
}

inline void serialise(Buffer& buffer, SkeletalMeshData& value, SerialisationMode mode)
{
    serialise(buffer, value.indices, mode);
    serialise(buffer, value.vertices, mode);
    serialise(buffer, value.bones, mode);
}

} // namespace BinarySerialisation

} // namespace Siege

namespace std
{

template<>
struct hash<Siege::SkinnedVertex>
{
    size_t operator()(const Siege::SkinnedVertex& vertex) const
    {
        size_t seed;
        Siege::Hash::HashCombine(seed, vertex, vertex.bones, vertex.weights);
        return seed;
    };
};

} // namespace std

#endif // SIEGE_ENGINE_SKELETALMESHDATA_H
