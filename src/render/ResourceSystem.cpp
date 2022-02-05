#include "ResourceSystem.h"

#include <vector>

void ResourceSystem::FreeResources()
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

void ResourceSystem::ClearResources()
{
    for (auto& model : models) freedModels.push_back(&model.second);
    for (auto& texture : textures) freedTextures.push_back(&texture.second);
}

void ResourceSystem::SetBaseDirectory(const String& dir)
{
    baseDir = dir;
}

const String& ResourceSystem::GetBaseDirectory()
{
    return baseDir;
}
