//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Texture2DDataPacker.h"

#define STB_IMAGE_IMPLEMENTATION
#include <resources/Texture2DData.h>
#include <stb_image.h>
#include <utils/Defer.h>
#include <utils/Logging.h>

void* PackTexture2DFile(const Siege::String& filePath)
{
    int32_t texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    defer([pixels] { stbi_image_free(pixels); });
    CC_ASSERT(pixels, "Failed to load image file!")

    CC_LOG_INFO("Reading 2D texture with dimensions {} by {} over {} channels", texWidth, texHeight, texChannels)

    Siege::Texture2DData* texture2dData =
        Siege::Texture2DData::Create(pixels, texWidth, texHeight, texChannels);
    return texture2dData;
}
