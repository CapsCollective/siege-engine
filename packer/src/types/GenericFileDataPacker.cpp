//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "GenericFileDataPacker.h"

#include <resources/PackFileData.h>
#include <utils/Logging.h>

#include <fstream>

void* PackGenericFile(const Siege::String& filePath, uint32_t& fileSize)
{
    CC_LOG_INFO("Reading file data as generic binary asset")

    // Read the file as binary and consume the entire file.
    std::ifstream file {filePath.Str(), std::ios::ate | std::ios::binary};

    CC_ASSERT(file.is_open(), Siege::String("Could not find file: ") + filePath)

    // Since we consumed the entire file, we can tell the size by checking where
    // the file stream is reading from (which presumably is at the end of the file).
    fileSize = static_cast<uint32_t>(file.tellg());
    char* data = static_cast<char*>(malloc(fileSize));
    defer([&data] { free(data); });

    file.seekg(0); // Move to the beginning of the file.
    file.read(data, fileSize);
    file.close();

    Siege::PackFileData* fileData = Siege::PackFileData::Create(&data[0], fileSize);
    fileSize = Siege::PackFileData::GetDataSize(fileData);
    return fileData;
}
