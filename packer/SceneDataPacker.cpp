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

#include <algorithm>
#include <fstream>

void* PackSceneFile(const Siege::String& filePath)
{
    Siege::String bodyString;
    auto appendFile = [&bodyString](const std::filesystem::path& path) {
        if (path.extension() != ".entity") return;
        bodyString += Siege::FileSystem::Read(path.c_str());
        bodyString += '|';
    };

    bool result = Siege::FileSystem::ForEachFileInDir(filePath, appendFile);
    if (!result)
    {
        CC_LOG_ERROR("Failed to read scene file at path \"{}\"", filePath)
        return nullptr;
    }

    std::string str(bodyString);
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.cend());
    bodyString = str.c_str();

    uint32_t fileSize = bodyString.Size() + 1;
    char data[fileSize];
    memcpy(data, bodyString.Str(), fileSize);

    Siege::SceneData* sceneData = Siege::SceneData::Create(&data[0], fileSize);
    return sceneData;
}
