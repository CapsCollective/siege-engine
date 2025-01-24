//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "StaticMeshDataPacker.h"

#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <algorithm>
#include <fstream>


REGISTER_TOKEN(SOURCE_PATH);

void* PackStaticMeshFile(const Siege::String& filePath)
{
    Siege::String contents = Siege::FileSystem::Read(filePath);
    std::map<Siege::Token, Siege::String> attributes =
        Siege::FileSystem::ParseAttributeFileData(contents);

    if (attributes.empty())
    {
        CC_LOG_WARNING("Received empty static mesh file at path \"{}\"", filePath);
        return nullptr;
    }

    auto it = attributes.find(TOKEN_SOURCE_PATH);
    if (it == attributes.end())
    {
        CC_LOG_ERROR("Failed to find SOURCE_PATH attribute in .sm file at path \"{}\"", filePath)
        return nullptr;
    }

}
