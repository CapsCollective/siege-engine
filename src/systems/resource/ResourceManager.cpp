#include <vector>
#include "ResourceManager.h"

// Static member initialisation
ResourceMap ResourceManager::resources;
ResourceList ResourceManager::freedResources;

void ResourceManager::ClearResources()
{
    for (auto& resource : resources)
    {
        freedResources.push_back(&resource.second);
    }
}

void ResourceManager::FreeResources()
{
    if (freedResources.empty()) return;

    // Iterate over the vector backwards to avoid memory issues
    for (auto i = freedResources.rbegin(); i != freedResources.rend(); ++i)
    {
        // Check what type the variant holds - requires two levels
        // of de - referencing (once for the iterator once for the vector element)
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
