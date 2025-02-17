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
#include <utils/math/mat/Mat4.h>

#include "StaticMeshData.h"
#include "PackFile.h"

namespace Siege
{
struct Bone
{
    uint32_t id;
    Mat4 bindMatrix;
};

struct SkinnedVertex : public BaseVertex
{
    Vec4 bones;
    Vec4 weights;
};

inline bool operator==(const SkinnedVertex& left, const SkinnedVertex& right)
{
    return static_cast<BaseVertex>(left) == static_cast<BaseVertex>(right) && left.bones == right.bones && left.weights == right.weights;
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
    uint32_t bonesCount = 0;
    uint32_t boneNamesLength = 0;
    char data[];

    uint32_t* GetIndices() const
    {
        return (uint32_t*) data;
    }

    SkinnedVertex* GetVertices() const
    {
        return (SkinnedVertex*) (data + sizeof(uint32_t) * indicesCount);
    }

    Bone* GetBones() const
    {
        return (Bone*) (data + (sizeof(uint32_t) * indicesCount + sizeof(SkinnedVertex) * verticesCount));
    }

    char* GetBoneNames() const
    {
        return (char*) (data + (sizeof(uint32_t) * indicesCount + sizeof(SkinnedVertex) * verticesCount) + sizeof(Bone) * bonesCount);
    }

    static SkeletalMeshData* Create(const std::vector<uint32_t>& objIndices,
                                    const std::vector<SkinnedVertex>& objVertices,
                                    const std::map<Siege::String, Siege::Bone>& objBones)
    {
        std::vector<Siege::Bone> bones;
        std::vector<Siege::String> boneNames;
        size_t boneNamesLength = 0;
        for (const auto& it : objBones)
        {
            bones.push_back(it.second);
            boneNames.push_back(it.first);
            boneNamesLength += it.first.Size() + 1;
        }

        char boneNamesArr[boneNamesLength];
        size_t currentBoneNamesOffset = 0;
        for (const Siege::String& name : boneNames)
        {
            memcpy(&boneNamesArr[0] + currentBoneNamesOffset, name.Str(), name.Size() + 1);
            currentBoneNamesOffset += name.Size() + 1;
        }

        uint32_t indicesDataSize = sizeof(uint32_t) * objIndices.size();
        uint32_t verticesDataSize = sizeof(SkinnedVertex) * objVertices.size();
        uint32_t bonesDataSize = sizeof(Bone) * objBones.size();
        uint32_t totalDataSize = sizeof(SkeletalMeshData) + indicesDataSize + verticesDataSize + bonesDataSize + boneNamesLength;

        void* mem = malloc(totalDataSize);
        SkeletalMeshData* skeletalMeshData = new (mem) SkeletalMeshData();

        skeletalMeshData->indicesCount = objIndices.size();
        skeletalMeshData->verticesCount = objVertices.size();
        skeletalMeshData->bonesCount = objBones.size();
        skeletalMeshData->boneNamesLength = boneNamesLength;
        memcpy(&skeletalMeshData->data[0], objIndices.data(), indicesDataSize);
        memcpy(&skeletalMeshData->data[0] + indicesDataSize, objVertices.data(), verticesDataSize);

        memcpy(&skeletalMeshData->data[0] + indicesDataSize + verticesDataSize, bones.data(), bonesDataSize);
        memcpy(&skeletalMeshData->data[0] + indicesDataSize + verticesDataSize + bonesDataSize, boneNamesArr, boneNamesLength);

        return skeletalMeshData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        if (!objectData) return 0;
        SkeletalMeshData* skeletalMeshData = reinterpret_cast<SkeletalMeshData*>(objectData);
        return sizeof(SkeletalMeshData) + sizeof(uint32_t) * skeletalMeshData->indicesCount +
               sizeof(SkinnedVertex) * skeletalMeshData->verticesCount +
               sizeof(Bone) * skeletalMeshData->bonesCount + skeletalMeshData->boneNamesLength;
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
        size_t seed;
        Siege::Hash::HashCombine(seed, vertex, vertex.bones, vertex.weights);
        return seed;
    };
};

} // namespace std

#endif // SIEGE_ENGINE_SKELETALMESHDATA_H
