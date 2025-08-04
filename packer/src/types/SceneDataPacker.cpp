//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
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

#include "render/renderer/buffer/Buffer.h"
#include "resources/PackFileData.h"

Siege::PackFileData* PackSceneFile(const Siege::String& filePath)
{
    Siege::SceneData sceneData;
    auto appendFile = [&sceneData](const std::filesystem::path& path) {
        if (path.extension() != ".entity") return;
        CC_LOG_INFO("Reading entity file {}", path.filename().c_str())

        Siege::String content = Siege::FileSystem::Read(path.c_str());
        content = Siege::FileSystem::StripNewLines(content);
        sceneData.entities.emplace_back(content);
    };

    bool result = Siege::FileSystem::ForEachFileInDir(filePath, appendFile);
    if (!result)
    {
        CC_LOG_ERROR("Failed to read scene file at path \"{}\"", filePath)
        return nullptr;
    }

    Siege::BinarySerialisation::Buffer dataBuffer;
    Siege::BinarySerialisation::serialise(dataBuffer,
                                          sceneData,
                                          Siege::BinarySerialisation::SERIALISE);

    char* data = reinterpret_cast<char*>(dataBuffer.data.data());
    Siege::PackFileData* fileData = Siege::PackFileData::Create(data, dataBuffer.data.size());
    return fileData;
}
