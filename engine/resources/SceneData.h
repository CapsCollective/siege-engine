//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SCENEDATA_H
#define SIEGE_ENGINE_SCENEDATA_H

#include "PackFile.h"

namespace Siege
{

#pragma pack(push, 1)
struct SceneData
{
    uint32_t dataSize = 0;
    char data[];

    const char* GetData() const
    {
        return &data[0];
    }

    static SceneData* Create(char* data, uint32_t dataSize)
    {
        uint32_t totalDataSize = sizeof(SceneData) + dataSize;

        void* mem = malloc(totalDataSize);
        Siege::SceneData* sceneData = new (mem) Siege::SceneData();

        sceneData->dataSize = dataSize;
        memcpy(&sceneData->data[0], data, dataSize);

        return sceneData;
    }

    static uint32_t GetDataSize(void* objectData)
    {
        SceneData* sceneData = reinterpret_cast<SceneData*>(objectData);
        return sizeof(SceneData) + sceneData->dataSize;
    }
};
#pragma pack(pop)

} // namespace Siege

#endif // SIEGE_ENGINE_SCENEDATA_H
