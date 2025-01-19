//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "StaticMeshDataPacker.h"
#include "WavefrontDataPacker.h"

#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <algorithm>
#include <fstream>

static constexpr const char LINE_SEP = ';';
static constexpr const char NAME_SEP = ':';
static constexpr const char* SOURCE_PATH_ATTR = "SOURCE_PATH";

void* PackStaticMeshFile(const Siege::String& filePath)
{
    Siege::String contents = Siege::FileSystem::Read(filePath);

    std::string str(contents);
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.cend());
    contents = str.c_str();

    std::map<Siege::String, Siege::String> attributes;
    for (const Siege::String& line : contents.Split(LINE_SEP))
    {
        std::vector<Siege::String> attrValue = line.Split(NAME_SEP);
        attributes[attrValue[0]] = attrValue[1];
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
