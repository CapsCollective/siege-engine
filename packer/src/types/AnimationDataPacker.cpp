//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "AnimationDataPacker.h"

#include <resources/AnimationData.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <algorithm>
#include <fstream>

#include "StaticMeshDataPacker.h"

REGISTER_TOKEN(ANIM_NAME);

static void GetAnimationData(aiAnimation* animation, OUT Siege::AnimationData& anim)
{
    anim.length = animation->mDuration;
    anim.speed = animation->mTicksPerSecond;

    Siege::String animName = animation->mName.C_Str();
    CC_LOG_INFO("Reading animation \"{}\" with duration of {} seconds", animName, anim.length / anim.speed)

    for (uint32_t i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim* channel = animation->mChannels[i];
        Siege::String channelName = channel->mNodeName.C_Str();

        CC_LOG_INFO("Reading channel with name \"{}\"", channelName)

        Siege::AnimationChannel& animChannel = anim.channels[channelName];

        for (uint32_t j = 0; j < channel->mNumPositionKeys; j++)
        {
            aiVectorKey key = channel->mPositionKeys[j];
            Siege::AnimKeyPosition& positionKey = animChannel.positionKeys.emplace_back();
            positionKey.timestamp = key.mTime;
            positionKey.position = {key.mValue.x, key.mValue.y, key.mValue.z};
        }

        for (uint32_t j = 0; j < channel->mNumRotationKeys; j++)
        {
            aiQuatKey key = channel->mRotationKeys[j];
            Siege::AnimKeyRotation& rotationKey = animChannel.rotationKeys.emplace_back();
            rotationKey.timestamp = key.mTime;
            rotationKey.rotation = {key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w};
        }

        for (uint32_t j = 0; j < channel->mNumScalingKeys; j++)
        {
            aiVectorKey key = channel->mScalingKeys[j];
            Siege::AnimKeyScale& scaleKey = animChannel.scaleKeys.emplace_back();
            scaleKey.timestamp = key.mTime;
            scaleKey.scale = {key.mValue.x, key.mValue.y, key.mValue.z};
        }
    }
}

Siege::PackFileData* PackAnimationFile(const Siege::String& filePath, const Siege::String& assetsPath)
{
    Siege::String contents = Siege::FileSystem::Read(filePath);
    std::map<Siege::Token, Siege::String> attributes = Siege::FileSystem::ParseAttributeFileData(contents);

    if (attributes.empty())
    {
        CC_LOG_WARNING("Received empty animation file at path \"{}\"", filePath)
        return nullptr;
    }

    auto it = attributes.find(TOKEN_SOURCE_PATH);
    if (it == attributes.end())
    {
        CC_LOG_WARNING("Failed to find SOURCE_PATH attribute in .ska file at path \"{}\"", filePath)
        return nullptr;
    }

    auto nameIt = attributes.find(TOKEN_ANIM_NAME);
    if (nameIt == attributes.end())
    {
        CC_LOG_WARNING("Failed to find NAME attribute in .ska file at path \"{}\"", filePath)
    }

    Siege::String modelPath = assetsPath + '/' + it->second;
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(modelPath,
                          aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                              aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
    if (!scene)
    {
        CC_LOG_WARNING("Failed to read file at path \"{}\"", modelPath)
        return nullptr;
    }

    if (!scene->HasAnimations())
    {
        CC_LOG_WARNING("File at path \"{}\" contains no animations", modelPath)
        return nullptr;
    }

    aiAnimation* animation = nullptr;
    for (uint32_t i = 0; i < scene->mNumAnimations; i++)
    {
        if (nameIt->second == scene->mAnimations[i]->mName.C_Str())
        {
            animation = scene->mAnimations[i];
            break;
        }
    }

    if (!animation)
    {
        CC_LOG_WARNING("Failed to find animation with name \"{}\"", nameIt->second)
        return nullptr;
    }

    Siege::AnimationData animData;
    GetAnimationData(animation, animData);

    Siege::BinarySerialisation::Buffer dataBuffer;
    Siege::BinarySerialisation::serialise(dataBuffer,
                                          animData,
                                          Siege::BinarySerialisation::SERIALISE);

    char* data = reinterpret_cast<char*>(dataBuffer.data.data());
    Siege::PackFileData* fileData = Siege::PackFileData::Create(data, dataBuffer.data.size());
    return fileData;
}
