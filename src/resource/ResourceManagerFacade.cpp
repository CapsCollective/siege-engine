#include "ResourceManagerFacade.h"

#include "ResourceManager.h"

void ResourceManagerFacade::FreeResources()
{
    ResourceManager::FreeResources();
}

void ResourceManagerFacade::SetBaseDirectory(const std::string& dir)
{
    ResourceManager::SetBaseDirectory(dir);
}

const std::string& ResourceManagerFacade::GetBaseDirectory()
{
    return ResourceManager::GetBaseDirectory();
}
