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
struct AnimationChannelData
{
    uint64_t positionKeysCount = 0;
    uint64_t rotationKeysCount = 0;
    uint64_t scaleKeysCount = 0;
};

struct AnimationData
{
    double length;
    double speed;
    uint32_t channelCount = 0;
    char data[];

    bool GetAnim(Anim& anim) const
    {
        anim.length = length;
        anim.speed = speed;

        uint32_t keysStartOffset = sizeof(AnimationChannelData) * channelCount;

        std::vector<AnimChannel> channels;
        for (uint32_t i = 0; i < channelCount; ++i)
        {
            AnimationChannelData* channelData = reinterpret_cast<AnimationChannelData*>(data[0] + sizeof(AnimationChannelData) * i);
//            AnimKeyPosition* positions = reinterpret_cast<AnimKeyPosition*>(data[0] + keysStartOffset + sizeof(AnimKeyPosition) * j);
//            channels.push_back(channelData);
//            channelData.
        }

        return true;
    }

    static AnimationData* Create(const Anim& animation)
    {
        std::vector<Siege::AnimChannel> channels;
        std::vector<Siege::String> channelNames;
        size_t channelNamesLength = 0;
        for (const auto& it : animation.channels)
        {
            channels.push_back(it.second);

            channelNames.push_back(it.first);
            channelNamesLength += it.first.Size() + 1;
        }

        char channelNamesArr[channelNamesLength];
        size_t currentChannelNamesOffset = 0;
        for (const Siege::String& name : channelNames)
        {
            memcpy(&channelNamesArr[0] + currentChannelNamesOffset, name.Str(), name.Size() + 1);
            currentChannelNamesOffset += name.Size() + 1;
        }

        uint32_t keysDataSize = 0;
        std::vector<AnimationChannelData> channelDataEntries;
        for (const AnimChannel& channel : channels)
        {
            AnimationChannelData channelData;
            channelData.positionKeysCount = channel.positionKeys.size();
            keysDataSize += sizeof(AnimKeyPosition) * channel.positionKeys.size();
            channelData.rotationKeysCount = channel.rotationKeys.size();
            keysDataSize += sizeof(AnimKeyRotation) * channel.rotationKeys.size();
            channelData.scaleKeysCount = channel.scaleKeys.size();
            keysDataSize += sizeof(AnimKeyScale) * channel.scaleKeys.size();
            channelDataEntries.push_back(channelData);
        }
        uint32_t channelsDataSize = sizeof(AnimationChannelData) * channelDataEntries.size();
        uint32_t totalDataSize = sizeof(AnimationData) + channelsDataSize + keysDataSize + channelNamesLength;

        void* mem = malloc(totalDataSize);
        AnimationData* animationData = new (mem) AnimationData();

        animationData->length = animation.length;
        animationData->speed = animation.speed;
        animationData->channelCount = animation.channels.size();
        memcpy(&animationData->data[0], channelDataEntries.data(), channelsDataSize);

        uint32_t keysWriteOffset = channelsDataSize;
        for (const AnimChannel& channel : channels)
        {
            uint32_t positionKeysDataSize = sizeof(AnimKeyPosition) * channel.positionKeys.size();
            uint32_t rotationKeysDataSize = sizeof(AnimKeyRotation) * channel.rotationKeys.size();
            uint32_t scaleKeysDataSize = sizeof(AnimKeyScale) * channel.scaleKeys.size();
            uint32_t channelDataSize = sizeof(AnimationChannelData) + positionKeysDataSize + rotationKeysDataSize + scaleKeysDataSize;

            memcpy(&animationData->data[0] + keysWriteOffset, channel.positionKeys.data(), positionKeysDataSize);
            memcpy(&animationData->data[0] + keysWriteOffset + positionKeysDataSize, channel.rotationKeys.data(), rotationKeysDataSize);
            memcpy(&animationData->data[0] + keysWriteOffset + positionKeysDataSize + rotationKeysDataSize, channel.scaleKeys.data(), scaleKeysDataSize);
            keysWriteOffset += channelDataSize + 1;
        }
        memcpy(&animationData->data[0] + keysWriteOffset, channelNamesArr, channelNamesLength);

        return animationData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        if (!objectData) return 0;
        AnimationData* animationData = reinterpret_cast<AnimationData*>(objectData);
        return sizeof(AnimationData);
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_ANIMATIONDATA_H
