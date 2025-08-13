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
            count = &it->second.first;
            data = static_cast<T*>(it->second.second);
        }
        else
        {
            data = GetPackFile()->FindData<T>(path);
            std::pair<uint32_t, void*> pair(0, data);
            const auto& newIt = loadedResources.emplace(path, pair);
            count = &newIt.first->second.first;
        }
        return {count, data};
    }

private:

    // Private fields

    PackFile* packFile = nullptr;

    std::map<String, std::pair<uint32_t, void*>> loadedResources;
};

} // namespace Siege

#endif // SIEGE_ENGINE_RESOURCESYSTEM_H
