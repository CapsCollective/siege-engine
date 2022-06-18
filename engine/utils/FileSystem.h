//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FILESYSTEM_H
#define SIEGE_ENGINE_FILESYSTEM_H

#include "String.h"

namespace FileSystem
{

String Read(const String& filename);

bool Exists(const String& filename);

bool Save(const String& filename, const String& content);

} // namespace FileSystem

#endif // SIEGE_ENGINE_FILESYSTEM_H
