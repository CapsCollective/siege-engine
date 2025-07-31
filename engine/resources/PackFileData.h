//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PACKFILEDATA_H
#define SIEGE_ENGINE_PACKFILEDATA_H

#include "PackFile.h"

namespace Siege
{

#pragma pack(push, 1)
struct PackFileData
{
    uint32_t dataSize = 0;
    char data[];

    const char* GetData() const
    {
        return &data[0];
    }

    static PackFileData* Create(char* data, uint32_t dataSize)
    {
        uint32_t totalDataSize = sizeof(PackFileData) + dataSize;

        void* mem = malloc(totalDataSize);
        PackFileData* fileData = new (mem) PackFileData();

        fileData->dataSize = dataSize;
        memcpy(&fileData->data[0], data, dataSize);

        return fileData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        if (!objectData) return 0;
        PackFileData* fileData = reinterpret_cast<PackFileData*>(objectData);
        return sizeof(PackFileData) + fileData->dataSize;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_PACKFILEDATA_H
