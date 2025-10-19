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

#include <utils/String.h>

#include <map>

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
            data = GetPackFile()->FindData<T>(path, size);
            ResourceEntry e{0, size, data};
            const auto& newIt = loadedResources.emplace(path, e);
            count = &newIt.first->second.refs;
            runningTotal += newIt.first->second.size;
            CC_LOG_INFO("[RESOURCES] Running Total: {}, allocating \"{}\"", runningTotal, path)
        }
        return {count, data};
    }

private:

    uint32_t runningTotal = 0;

    // Private fields

    PackFile* packFile = nullptr;

    std::map<String, ResourceEntry> loadedResources;
};

} // namespace Siege

#endif // SIEGE_ENGINE_RESOURCESYSTEM_H
