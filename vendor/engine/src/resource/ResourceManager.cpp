#include <vector>
#include "ResourceManager.h"

// Static member initialisation
std::map<std::string, Model> ResourceManager::models;
std::map<std::string, Texture> ResourceManager::textures;

std::vector<Model*> ResourceManager::freedModels;
std::vector<Texture*> ResourceManager::freedTextures;

void ResourceManager::RegisterModel(const std::string& path)
{
    if (models.find(path) != models.end()) return;
    models[path] = LoadModel(path.c_str());
}

void ResourceManager::RegisterTexture(const std::string& path)
{
    if (textures.find(path) != textures.end()) return;
    textures[path] = LoadTexture(path.c_str());
}

Model& ResourceManager::GetModel(const std::string& path)
{
    return models.at(path);
}

Texture& ResourceManager::GetTexture(const std::string& path)
{
    return textures.at(path);
}

void ResourceManager::FreeResources()
{
    if (freedModels.empty() && freedTextures.empty()) return;

    // Iterate over the vector backwards to avoid memory issues
    for (auto i = freedModels.rbegin(); i != freedModels.rend(); i++)
    {
        UnloadModel(**i);
    }
    freedModels.clear();
    models.clear();

    // Iterate over the vector backwards to avoid memory issues
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