//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SHADERDATA_H
#define SIEGE_ENGINE_SHADERDATA_H

#include "PackFile.h"

namespace Siege
{

#pragma pack(push, 1)
struct ShaderData
{
    uint32_t dataSize = 0;
    char data[];

    const char* GetData() const
    {
        return &data[0];
    }

    static ShaderData* Create(char* data, uint32_t dataSize)
    {
        uint32_t totalDataSize = sizeof(ShaderData) + dataSize;

        void* mem = malloc(totalDataSize);
        Siege::ShaderData* shaderData = new (mem) Siege::ShaderData();

        shaderData->dataSize = dataSize;
        memcpy(&shaderData->data[0], data, dataSize);

        return shaderData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        ShaderData* shaderData = reinterpret_cast<ShaderData*>(objectData);
        return sizeof(ShaderData) + shaderData->dataSize;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_SHADERDATA_H
