//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GENERICFILEDATA_H
#define SIEGE_ENGINE_GENERICFILEDATA_H

#include "PackFile.h"

namespace Siege
{

#pragma pack(push, 1)
struct GenericFileData
{
    uint32_t dataSize = 0;
    char data[];

    const char* GetData() const
    {
        return &data[0];
    }

    static GenericFileData* Create(char* data, uint32_t dataSize)
    {
        uint32_t totalDataSize = sizeof(GenericFileData) + dataSize;

        void* mem = malloc(totalDataSize);
        Siege::GenericFileData* fileData = new (mem) Siege::GenericFileData();

        fileData->dataSize = dataSize;
        memcpy(&fileData->data[0], data, dataSize);

        return fileData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        GenericFileData* fileData = reinterpret_cast<GenericFileData*>(objectData);
        return sizeof(GenericFileData) + fileData->dataSize;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_GENERICFILEDATA_H
