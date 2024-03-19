//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "GenericFileDataPacker.h"

#include <resources/GenericFileData.h>
#include <utils/Logging.h>

#include <fstream>

void* PackGenericFile(const Siege::String& filePath)
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

    Siege::GenericFileData* fileData = Siege::GenericFileData::Create(&data[0], fileSize);
    return fileData;
}
