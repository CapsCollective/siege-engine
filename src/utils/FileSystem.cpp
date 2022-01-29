#include "FileSystem.h"

#include <libc.h>

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

bool FileSystem::Exists(const String& filename)
{
    return access(filename, W_OK) == 0;
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