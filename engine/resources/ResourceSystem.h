//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RESOURCESYSTEM_H
#define SIEGE_ENGINE_RESOURCESYSTEM_H

#include <utils/allocators/Tlsf.h>
#include <utils/BinarySerialisation.h>
#include <utils/Logging.h>
#include <utils/String.h>

#include <map>

#include "AnimationData.h"
#include "PackFileData.h"
#include "SceneData.h"
#include "SkeletalMeshData.h"
#include "StaticMeshData.h"
#include "Texture2DData.h"

#include "PackFile.h"
#include "ResourceHandle.h"

namespace Siege
{
struct ResourceEntry
{
    uint32_t refs;
    uint32_t size;
    void* ptr;
};

class ResourceSystem
{
public:

    // Public methods

    static ResourceSystem& GetInstance();

    bool MountPackFile(const String& searchPath = {});

    void UnmountPackFile();

    PackFile* GetPackFile();

    bool FreeResources();

    template<typename T>
    ResourceHandle<T> LoadResource(const String& path)
    {
        T* data;
        uint32_t* count;
        const auto& it = loadedResources.find(path);
        if (it != loadedResources.end())
        {
            count = &it->second.refs;
            data = static_cast<T*>(it->second.ptr);
        }
        else
        {
            uint32_t size;
            data = FindData<T>(path, size);
            ResourceEntry e{0, size, data};
            const auto& newIt = loadedResources.emplace(path, e);
            count = &newIt.first->second.refs;
            totalAllocation += newIt.first->second.size;
            CC_LOG_INFO("[RESOURCES] Running Total: {}, allocating \"{}\"", totalAllocation, path)
        }
        return {count, data};
    }

    template<typename T>
    T* FindData(const String& filepath, uint32_t& outSize)
    {
        uint32_t size;
        PackFileData* packFileData = FindDataRaw(filepath, size);

        BinarySerialisation::Buffer dataBuffer;
        dataBuffer.Fill(reinterpret_cast<uint8_t*>(packFileData->data), packFileData->dataSize);
        resourceAllocator.Deallocate(packFileData);

        outSize = sizeof(T);
        void* allocation = resourceAllocator.Allocate(outSize);
        T* typedData = new (allocation) T();
        BinarySerialisation::serialise(dataBuffer, *typedData, BinarySerialisation::DESERIALISE);

        return typedData;
    }

    template<>
    PackFileData* FindData(const String& filepath, uint32_t& outSize)
    {
        PackFileData* packFileData = FindDataRaw(filepath, outSize);
        return packFileData;
    }

    PackFileData* FindDataRaw(const String& filepath, uint32_t& outSize)
    {
        const std::map<String, PackFile::TocEntry*>& entries = packFile->GetEntries();
        const auto it = entries.find(filepath);
        if (it == entries.end())
        {
            return nullptr;
        }

        uLongf bodyDataSizeUncompressed = it->second->dataSize;
        uLongf bodyDataSizeCompressed = it->second->dataSizeCompressed;

        void* allocation = resourceAllocator.Allocate(bodyDataSizeUncompressed);
        PackFileData* packFileData = new (allocation) PackFileData();
        int result = uncompress(reinterpret_cast<Bytef*>(packFileData),
                                &bodyDataSizeUncompressed,
                                reinterpret_cast<Bytef*>(packFile->GetBody() + it->second->dataOffset),
                                bodyDataSizeCompressed);
        CC_ASSERT(result == Z_OK, "Decompression failed for filepath: " + filepath);

        outSize = bodyDataSizeUncompressed;
        return packFileData;
    }

private:

    // Private fields

    PackFile* packFile = nullptr;

    std::map<String, ResourceEntry> loadedResources;

    uint32_t totalAllocation = 0;

    MediumTlsfAllocator resourceAllocator = 1000000 * 4; // 4MB
};

} // namespace Siege

#endif // SIEGE_ENGINE_RESOURCESYSTEM_H
