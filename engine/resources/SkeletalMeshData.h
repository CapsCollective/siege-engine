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

#include <utils/Colour.h>
#include <utils/math/vec/Hashing.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>

#include "StaticMeshData.h"
#include "PackFile.h"

namespace Siege
{

struct SkinnedVertex
{
    BaseVertex vertex;
    Vec3 position;
    FColour color;
    Vec3 normal;
    Vec2 uv;
};

inline bool operator==(const SkinnedVertex& left, const SkinnedVertex& right)
{
    return left.position == right.position && left.color == right.color &&
           left.normal == right.normal && left.uv == right.uv;
}

inline bool operator!=(const SkinnedVertex& left, const SkinnedVertex& right)
{
    return !(left == right);
}

#pragma pack(push, 1)
struct SkeletalMeshData
{
    uint64_t indicesCount = 0;
    uint64_t verticesCount = 0;
    char data[];

    uint32_t* GetIndices() const
    {
        return (uint32_t*) data;
    }

    SkinnedVertex* GetVertices() const
    {
        return (SkinnedVertex*) (data + sizeof(uint32_t) * indicesCount);
    }

    static SkeletalMeshData* Create(const std::vector<uint32_t>& objIndices,
                                  const std::vector<SkinnedVertex>& objVertices)
    {
        uint32_t indicesDataSize = sizeof(uint32_t) * objIndices.size();
        uint32_t verticesDataSize = sizeof(SkinnedVertex) * objVertices.size();
        uint32_t totalDataSize = sizeof(SkeletalMeshData) + indicesDataSize + verticesDataSize;

        void* mem = malloc(totalDataSize);
        SkeletalMeshData* staticMeshData = new (mem) SkeletalMeshData();

        staticMeshData->indicesCount = objIndices.size();
        staticMeshData->verticesCount = objVertices.size();
        memcpy(&staticMeshData->data[0], objIndices.data(), indicesDataSize);
        memcpy(&staticMeshData->data[0] + indicesDataSize, objVertices.data(), verticesDataSize);

        return staticMeshData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        if (!objectData) return 0;
        SkeletalMeshData* staticMeshData = reinterpret_cast<SkeletalMeshData*>(objectData);
        return sizeof(SkeletalMeshData) + sizeof(uint32_t) * staticMeshData->indicesCount +
               sizeof(SkinnedVertex) * staticMeshData->verticesCount;
    }
};
#pragma pack(pop)

} // namespace Siege

namespace std
{

template<>
struct hash<Siege::SkinnedVertex>
{
    size_t operator()(const Siege::SkinnedVertex& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    };
};

} // namespace std

#endif // SIEGE_ENGINE_SKELETALMESHDATA_H
