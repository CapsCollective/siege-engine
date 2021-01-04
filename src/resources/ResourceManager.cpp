#include <vector>
#include "ResourceManager.h"

// Static member initialisation
RESOURCE_MAP ResourceManager::resources = RESOURCE_MAP();
RESOURCE_LIST ResourceManager::freedResources = RESOURCE_LIST();

void ResourceManager::ClearResources()
{
    // Get every resource in the map
    for (auto& resource : resources)
    {
        // Push the resource to the freeResources vector for unloading at the end of the frame
        freedResources.push_back(&resource.second);
    }
}

void ResourceManager::FreeAllResources()
{
    // If we have resources to free
    if (freedResources.empty()) return;

    // Iterate over the vector backwards to avoid memory issues
    for (auto i = freedResources.rbegin(); i != freedResources.rend(); ++i)
    {
        // Check whether the resource in question is a Model
        if (std::holds_alternative<Model>(**i))
        {
            // Unload the resource as a model
            UnloadModel(std::get<Model>(**i));
        }
        // Check whether the resource in question is a Texture2D
        else if (std::holds_alternative<Texture2D>(**i))
        {
            // Unload the resource as a texture
            UnloadTexture(std::get<Texture2D>(**i));
        }
    }
    // Empty out the freed resources vector
    freedResources.clear();
    // Clear out the resources in the resources map
    resources.clear();
}
