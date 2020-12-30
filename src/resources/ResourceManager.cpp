#include <vector>
#include "ResourceManager.h"

RESOURCE_MAP ResourceManager::resources = RESOURCE_MAP();

void ResourceManager::ClearResources()
{
    std::vector<std::variant<Model, Texture2D>> unloadedResources = std::vector<std::variant<Model, Texture2D>>();

    for (auto& resource : resources)
    {
        unloadedResources.push_back(resource.second);
    }

    for (auto i = unloadedResources.rbegin(); i != unloadedResources.rend(); ++i)
    {
        if (std::holds_alternative<Model>(*i))
        {
            UnloadModel(std::get<Model>(*i));
        }
        else if (std::holds_alternative<Texture2D>(*i))
        {
            UnloadTexture(std::get<Texture2D>(*i));
        }
    }

    resources.clear();
}
