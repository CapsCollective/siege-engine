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

#include "WavefrontDataPacker.h"

static constexpr const char* SOURCE_PATH_ATTR = "SOURCE_PATH";

void* PackStaticMeshFile(const Siege::String& filePath)
{
    Siege::String contents = Siege::FileSystem::Read(filePath);
    std::map<Siege::String, Siege::String> attributes =
        Siege::FileSystem::ParseAttributeFileData(contents);

    if (attributes.empty())
    {
        CC_LOG_WARNING("Received empty static mesh file at path \"{}\"", filePath);
        return nullptr;
    }

    auto it = attributes.find(SOURCE_PATH_ATTR);
    if (it == attributes.end())
    {
        CC_LOG_ERROR("Failed to find SOURCE_PATH attribute in .sm file at path \"{}\"", filePath)
        return nullptr;
    }

    void* staticMeshData = PackWavefrontFile(it->second);
    return staticMeshData;
}
