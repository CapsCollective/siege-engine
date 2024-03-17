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

#pragma pack(push, 1)
struct StaticMeshData
{
    uint64_t indicesCount = 0;
    uint64_t verticesCount = 0;
    char data[];

    uint32_t* GetIndices() const
    {
        return (uint32_t*) data;
    }

    Siege::BaseVertex* GetVertices() const
    {
        return (Siege::BaseVertex*) (data + sizeof(uint32_t) * indicesCount);
    }

    static StaticMeshData* Create(const std::vector<uint32_t>& objIndices, const std::vector<BaseVertex>& objVertices)
    {
        uint32_t indicesDataSize = sizeof(uint32_t) * objIndices.size();
        uint32_t verticesDataSize = sizeof(Siege::BaseVertex) * objVertices.size();
        uint32_t totalDataSize = sizeof(StaticMeshData) + indicesDataSize + verticesDataSize;

        void* mem = malloc(totalDataSize);
        Siege::StaticMeshData* staticMeshData = new (mem) Siege::StaticMeshData();

        staticMeshData->indicesCount = objIndices.size();
        staticMeshData->verticesCount = objVertices.size();
        memcpy(&staticMeshData->data[0], objIndices.data(), indicesDataSize);
        memcpy(&staticMeshData->data[0] + indicesDataSize, objVertices.data(), verticesDataSize);

        return staticMeshData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        StaticMeshData* staticMeshData = reinterpret_cast<StaticMeshData*>(objectData);
        return sizeof(StaticMeshData) + sizeof(uint32_t) * staticMeshData->indicesCount + sizeof(Siege::BaseVertex) * staticMeshData->verticesCount;
    }
};
#pragma pack(pop)

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
