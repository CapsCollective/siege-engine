#include "ResourceManagerFacade.h"

#include "ResourceManager.h"

void ResourceManagerFacade::FreeResources()
{
    ResourceManager::FreeResources();
}

void ResourceManagerFacade::SetBaseDirectory(const String& dir)
{
    ResourceManager::SetBaseDirectory(dir);
}

const String& ResourceManagerFacade::GetBaseDirectory()
{
    return ResourceManager::GetBaseDirectory();
}
