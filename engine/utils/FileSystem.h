//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FILESYSTEM_H
#define SIEGE_ENGINE_FILESYSTEM_H

#include <cstring>
#include "String.h"

namespace FileSystem
{

struct FileData
{
    FileData() : data {nullptr}, fileSize {0} {}

    FileData(const size_t& size) : fileSize {size}
    {
        data = static_cast<char*>(malloc(sizeof(char) * size));
    }

    FileData(const FileData& other)
    {
        if (data) free(data);

        fileSize = other.fileSize;
        data = static_cast<char*>(malloc(sizeof(char) * fileSize));
        memcpy(data, other.data, sizeof(char) * fileSize);
    }

    ~FileData()
    {
        free(data);
        data = nullptr;
        fileSize = 0;
    }

    char* data {nullptr};
    size_t fileSize {0};
};

String Read(const String& filename);

FileData ReadAsBinary(const String& filename);

bool Exists(const String& filename);

bool Save(const String& filename, const String& content);

} // namespace FileSystem

#endif // SIEGE_ENGINE_FILESYSTEM_H
