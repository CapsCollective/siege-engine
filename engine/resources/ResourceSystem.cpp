//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ResourceSystem.h"

#include "PackFile.h"

namespace Siege
{

bool ResourceSystem::MountPackFile(const String& searchPath)
{
    if (packFile) return false;

    const char* basePath = "./";
    if (!searchPath.IsEmpty())
    {
        basePath = searchPath.Str();
    }

    std::filesystem::path packFilePath;
    for (const auto& entry : std::filesystem::directory_iterator(basePath))
    {
        if (entry.path().extension() == ".pck")
        {
            packFile = new PackFile(entry.path().c_str());
            return true;
        }
    }
    return false;
}

void ResourceSystem::UnmountPackFile()
{
    delete packFile;
    packFile = nullptr;
}

PackFile* ResourceSystem::GetPackFile()
{
    return packFile;
}

bool ResourceSystem::FreeResources()
{
    bool freedResources = false;
    for (auto it = loadedResources.begin(); it != loadedResources.end();)
    {
        if (it->second.refs == 0)
        {
            packFile->resourceAllocator.Deallocate(&it->second.ptr);
            runningTotal -= it->second.size;
            CC_LOG_INFO("[RESOURCES] Running Total: {}, deallocating \"{}\"", runningTotal, it->first)
            loadedResources.erase(it++);
            freedResources = true;
        }
        else
        {
            ++it;
        }
    }
    return freedResources;
}

ResourceSystem& ResourceSystem::GetInstance()
{
    static ResourceSystem resourceSystem;
    return resourceSystem;
}

} // namespace Siege