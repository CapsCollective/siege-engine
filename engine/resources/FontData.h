//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FONTDATA_H
#define SIEGE_ENGINE_FONTDATA_H

#include "PackFile.h"

namespace Siege
{

#pragma pack(push, 1)
struct FontData
{
    uint32_t dataSize = 0;
    char data[];

    const char* GetData() const
    {
        return &data[0];
    }

    static FontData* Create(char* data, uint32_t dataSize)
    {
        uint32_t totalDataSize = sizeof(FontData) + dataSize;

        void* mem = malloc(totalDataSize);
        Siege::FontData* fontData = new (mem) Siege::FontData();

        fontData->dataSize = dataSize;
        memcpy(&fontData->data[0], data, dataSize);

        return fontData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        FontData* fontData = reinterpret_cast<FontData*>(objectData);
        return sizeof(FontData) + fontData->dataSize;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_FONTDATA_H
