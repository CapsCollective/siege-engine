#include <vector>
#include "ResourceManager.h"

// Static member initialisation
std::map<std::string, Model> ResourceManager::models;
std::map<std::string, Texture> ResourceManager::textures;
std::vector<Model*> ResourceManager::freedModels;
std::vector<Texture*> ResourceManager::freedTextures;

void ResourceManager::FreeResources()
{
    if (freedModels.empty() && freedTextures.empty()) return;

    // Iterate over vectors backwards to avoid memory issues
    for (auto i = freedModels.rbegin(); i != freedModels.rend(); i++)
    {
        UnloadModel(**i);
    }
    freedModels.clear();
    models.clear();

    for (auto i = freedTextures.rbegin(); i != freedTextures.rend(); i++)
    {
        UnloadTexture(**i);
    }
    freedTextures.clear();
    textures.clear();
}

void ResourceManager::ClearResources()
{
    for (auto& model : models) freedModels.push_back(&model.second);
    for (auto& texture : textures) freedTextures.push_back(&texture.second);
}