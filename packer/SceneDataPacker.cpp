//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SceneDataPacker.h"

#include <resources/SceneData.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <fstream>

void* PackSceneFile(const Siege::String& filePath, const Siege::String& assetsDir)
{
    Siege::String bodyString;
    auto appendFile = [&bodyString, &assetsDir](const std::filesystem::path& path) {
        bodyString += std::filesystem::relative(path, assetsDir.Str()).c_str();
        bodyString += ":";
        bodyString.Replace(assetsDir.Str(), "");
    };

    bool result = Siege::FileSystem::ForEachFileInDir(filePath, appendFile);
    if (!result)
    {
        CC_LOG_ERROR("Failed to read scene file at path \"{}\"", filePath)
        return nullptr;
    }

    uint32_t fileSize = bodyString.Size() + 1;
    char data[fileSize];
    memcpy(data, bodyString.Str(), fileSize);

    Siege::SceneData* sceneData = Siege::SceneData::Create(&data[0], fileSize);
    return sceneData;
}

void* PackEntityFile(const Siege::String& filePath)
{
    // Read the file as binary and consume the entire file.
    std::ifstream file {filePath.Str(), std::ios::ate | std::ios::binary};

    CC_ASSERT(file.is_open(), Siege::String("Could not find file: ") + filePath)

    // Since we consumed the entire file, we can tell the size by checking where
    // the file stream is reading from (which presumably is at the end of the file).
    uint32_t fileSize = static_cast<uint32_t>(file.tellg());

    char data[fileSize];
    file.seekg(0); // Move to the beginning of the file.
    file.read(data, fileSize);
    file.close();

    Siege::SceneEntityData* entityData = Siege::SceneEntityData::Create(&data[0], fileSize);
    return entityData;
}