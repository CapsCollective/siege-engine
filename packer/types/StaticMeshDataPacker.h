//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STATICMESHDATAPACKER_H
#define SIEGE_ENGINE_STATICMESHDATAPACKER_H

#include <utils/Token.h>
#include <resources/PackFile.h>

REGISTER_TOKEN(SOURCE_PATH);
REGISTER_TOKEN(NODE_PATH);
REGISTER_TOKEN(FLIP_AXES);

void* PackStaticMeshFile(const Siege::String& filePath, const Siege::String& assetsPath);

#endif // SIEGE_ENGINE_STATICMESHDATAPACKER_H
