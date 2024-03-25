//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TEXTURE2DDATA_H
#define SIEGE_ENGINE_TEXTURE2DDATA_H

#include "PackFile.h"

namespace Siege
{

#pragma pack(push, 1)
struct Texture2DData
{
    int32_t texWidth = 0;
    int32_t texHeight = 0;
    int32_t texChannels = 0;
    uint8_t pixels[];

    const uint8_t* GetPixels() const
    {
        return &pixels[0];
    }

    static Texture2DData* Create(uint8_t* pixels,
                                 int32_t texWidth,
                                 int32_t texHeight,
                                 int32_t texChannels)
    {
        uint64_t imageSize = texWidth * texHeight * 4;
        uint32_t pixelDataSize = sizeof(uint8_t) * texWidth * texHeight * 4;
        uint32_t totalDataSize = sizeof(Texture2DData) + pixelDataSize;

        void* mem = malloc(totalDataSize);
        Texture2DData* texture2dData = new (mem) Texture2DData();

        texture2dData->texWidth = texWidth;
        texture2dData->texHeight = texHeight;
        texture2dData->texChannels = texChannels;
        // Ensure we get no garbage data in the pixel array
        memset(&texture2dData->pixels[0], 0, imageSize);
        memcpy(&texture2dData->pixels[0], pixels, sizeof(uint8_t) * imageSize);

        return texture2dData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        Texture2DData* textureData = reinterpret_cast<Texture2DData*>(objectData);
        return sizeof(Texture2DData) +
               sizeof(uint8_t) * textureData->texWidth * textureData->texHeight * 4;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_TEXTURE2DDATA_H
