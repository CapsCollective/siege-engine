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

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

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

    uint32_t GetDataSize() const
    {
        return sizeof(PackFileData) + dataSize;
    }

    static PackFileData* Create(char* data, uint32_t dataSize)
    {
        void* mem = malloc(sizeof(PackFileData) + dataSize);
        PackFileData* fileData = new (mem) PackFileData();
        fileData->dataSize = dataSize;
        memcpy(&fileData->data[0], data, dataSize);
        return fileData;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_PACKFILEDATA_H
