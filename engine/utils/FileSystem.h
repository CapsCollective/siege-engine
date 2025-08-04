//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FILESYSTEM_H
#define SIEGE_ENGINE_FILESYSTEM_H

#include <filesystem>
#include <map>

#include "String.h"
#include "Token.h"

// Define constants
static constexpr const char ATTR_FILE_LINE_SEP = ';';
static constexpr const char ATTR_FILE_VALUE_SEP = ':';

namespace Siege::FileSystem
{

String Read(const String& filename);

bool Exists(const String& filename);

bool Save(const String& filename, const String& content);

bool Remove(const String& filepath);

bool CreateDirectoryRecursive(const String& dirpath);

bool ForEachFileInDir(const String& path,
                      const std::function<void(const std::filesystem::path&)>& func);

String StripNewLines(const String& string);

std::map<Token, String> ParseAttributeFileData(const String& fileData);

} // namespace Siege::FileSystem

#endif // SIEGE_ENGINE_FILESYSTEM_H
