//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FILESYSTEM_H
#define SIEGE_ENGINE_FILESYSTEM_H

#include <filesystem>

#include "String.h"

namespace Siege::FileSystem
{

String Read(const String& filename);

bool Exists(const String& filename);

bool Save(const String& filename, const String& content);

bool Remove(const String& filepath);

bool CreateDirectoryRecursive(const String& dirpath);

bool ForEachFileInDir(const String& path,
                      const std::function<void(const std::filesystem::path&)>& func);

} // namespace Siege::FileSystem

#endif // SIEGE_ENGINE_FILESYSTEM_H
