#include <vector>
#include "ResourceManager.h"

RESOURCE_MAP ResourceManager::resources = RESOURCE_MAP();
std::vector<std::variant<Model, Texture2D>*> ResourceManager::freedResources = std::vector<std::variant<Model, Texture2D>*>();

void ResourceManager::ClearResources()
{
    for (auto& resource : resources)
    {
        freedResources.push_back(&resource.second);
    }
}

void ResourceManager::FreeAllResources()
{
    if (!freedResources.empty()) {
        for (auto i = freedResources.rbegin(); i != freedResources.rend(); ++i)
        {
            if (std::holds_alternative<Model>(**i))
            {
                UnloadModel(std::get<Model>(**i));
            }
            else if (std::holds_alternative<Texture2D>(**i))
            {
                UnloadTexture(std::get<Texture2D>(**i));
            }

        }
        freedResources.clear();
        resources.clear();
    }
}
