//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Texture2DDataPacker.h"

#define STB_IMAGE_IMPLEMENTATION
#include <resources/PackFileData.h>
#include <resources/Texture2DData.h>
#include <stb_image.h>
#include <utils/BinarySerialisation.h>
#include <utils/Defer.h>
#include <utils/Logging.h>

Siege::PackFileData* PackTexture2DFile(const Siege::String& filePath)
{
    int32_t texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    defer([pixels] { stbi_image_free(pixels); });
    CC_ASSERT(pixels, "Failed to load image file!")

    CC_LOG_INFO("Reading 2D texture with dimensions {} by {} over {} channels",
                texWidth,
                texHeight,
                texChannels)

    Siege::Texture2DData texture2dData;
    texture2dData.texWidth = texWidth;
    texture2dData.texHeight = texHeight;
    texture2dData.texChannels = TEXTURE2D_TEXTURE_CHANNELS;
    std::copy_n(pixels, texture2dData.GetImageSize(), std::back_inserter(texture2dData.pixels));

    Siege::BinarySerialisation::Buffer dataBuffer;
    Siege::BinarySerialisation::serialise(dataBuffer,
                                          texture2dData,
                                          Siege::BinarySerialisation::SERIALISE);

    char* data = reinterpret_cast<char*>(dataBuffer.data.data());
    Siege::PackFileData* fileData = Siege::PackFileData::Create(data, dataBuffer.data.size());
    return fileData;
}
