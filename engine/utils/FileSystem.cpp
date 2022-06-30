//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FileSystem.h"

#include <cstdio>
#include <fstream>

String FileSystem::Read(const String& filename)
{
    // Try open the file for reading
    FILE* file = fopen(filename, "r");
    if (!file) return "";

    // Determine file size
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    // Copy the content
    char content[size];
    fread(content, sizeof(char), size, file);
    content[size] = '\0';

    // Close the file stream and return
    fclose(file);
    return content;
}

FileSystem::FileData FileSystem::ReadAsBinary(const String& filename)
{
    // Try open the file for reading
    FILE* file = fopen(filename, "r");
    if (!file) return {};

    // Determine file size
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    FileSystem::FileData fileContent(size);

    // Copy the content
    fread(fileContent.data, sizeof(char), size, file);

    // Close the file stream and return
    fclose(file);
    return fileContent;
}

bool FileSystem::Exists(const String& filename)
{
    std::ifstream file(filename);
    return (bool) file;
}

bool FileSystem::Save(const String& filename, const String& content)
{
    // Open a new file stream, serialise the data to it and close it
    std::ofstream fileStream(filename.Str());
    if (!fileStream.is_open()) return false;
    fileStream << content;
    fileStream.close();
    return true;
}