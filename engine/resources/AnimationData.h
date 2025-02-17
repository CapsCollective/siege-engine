//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_ANIMATIONDATA_H
#define SIEGE_ENGINE_ANIMATIONDATA_H

#include <utils/math/vec/Vec3.h>
#include <utils/math/mat/Mat4.h>

#include "PackFile.h"

namespace Siege
{

struct AnimKeyPosition
{
    double timestamp = 0.f;
    Vec3 position;
};

struct AnimKeyRotation
{
    double timestamp = 0.f;
    Vec4 rotation;
};

struct AnimKeyScale
{
    double timestamp = 0.f;
    Vec3 scale;
};

struct AnimChannel
{
    std::vector<AnimKeyPosition> positionKeys;
    std::vector<AnimKeyRotation> rotationKeys;
    std::vector<AnimKeyScale> scaleKeys;
};

struct Anim
{
    double length;
    double speed;
    std::map<Siege::String, AnimChannel> channels;
};

#pragma pack(push, 1)
struct AnimationData
{
    double length;
    double speed;
    uint64_t channelCount = 0;
    char data[];

    static AnimationData* Create(const Anim& animation)
    {
        uint32_t totalDataSize = sizeof(AnimationData);
        void* mem = malloc(totalDataSize);
        AnimationData* animationData = new (mem) AnimationData();

        animationData->length = animation.length;
        animationData->speed = animation.speed;
        animationData->channelCount = animation.channels.size();
        return animationData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        if (!objectData) return 0;
        AnimationData* animationData = reinterpret_cast<AnimationData*>(objectData);
        return sizeof(AnimationData) + animationData->channelCount;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_ANIMATIONDATA_H
