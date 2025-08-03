//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FileSystem.h"

#include <algorithm>
#include <cstdio>
#include <fstream>

#include "Logging.h"

namespace Siege::FileSystem
{
String Read(const String& filename)
{
    // Try open the file for reading
    FILE* file = fopen(filename, "rb");
    if (!file) return "";

    // Determine file size
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    // Copy the content
    char* content = static_cast<char*>(alloca(size + 1));
    fread(content, sizeof(char), size, file);
    content[size] = '\0';

    // Close the file stream and return
    fclose(file);
    return content;
}

bool Exists(const String& filename)
{
    return std::filesystem::exists(filename.Str());
}

bool Save(const String& filename, const String& content)
{
    // Open a new file stream, serialise the data to it and close it
    std::ofstream fileStream(filename.Str());
    if (!fileStream.is_open()) return false;
    fileStream << content;
    fileStream.close();
    return true;
}

bool Remove(const String& filepath)
{
    return std::filesystem::remove_all(filepath.Str());
}

bool CreateDirectoryRecursive(const String& dirpath)
{
    return std::filesystem::create_directories(dirpath.Str());
}

bool ForEachFileInDir(const String& path,
                      const std::function<void(const std::filesystem::path&)>& func)
{
    if (!Exists(path)) return false;

    auto it = std::filesystem::recursive_directory_iterator(path.Str());
    for (const auto& entry : it)
    {
        if (!std::filesystem::is_directory(entry)) func(entry.path());
    }
    return true;
}

String StripNewLines(const String& string)
{
    std::string str(string);
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.cend());
    return str.c_str();
}

std::map<Token, String> ParseAttributeFileData(const String& fileData)
{
    // Expunge any newlines characters from the file data
    String sanitisedFileData = StripNewLines(fileData);

    // Split the file line contents into attributes
    std::map<Token, String> attributes;
    for (const String& line : sanitisedFileData.Split(ATTR_FILE_LINE_SEP))
    {
        std::vector<String> attribute = line.Split(ATTR_FILE_VALUE_SEP);
        if (attribute.size() != 2)
        {
            CC_LOG_WARNING("Received wrong number of attribute value seperations for line \"{}\", "
                           "will not register!",
                           line)
            continue;
        }

        Token attributeToken = Token::FindToken(attribute[0]);
        if (!attributeToken)
        {
            CC_LOG_WARNING("Failed to find token for attribute \"{}\", will not register!",
                           attribute[0])
            continue;
        }
        attributes[attributeToken] = attribute[1];
    }

    return attributes;
}
} // namespace Siege::FileSystem