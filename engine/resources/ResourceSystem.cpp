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
    for(const auto& entry : std::filesystem::directory_iterator(basePath))
    {
        if(entry.path().extension() == ".pck")
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
}

PackFile* ResourceSystem::GetPackFile()
{
    return packFile;
}

ResourceSystem& ResourceSystem::GetInstance()
{
    static ResourceSystem resourceSystem;
    return resourceSystem;
}

} // namespace std