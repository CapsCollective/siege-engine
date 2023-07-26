//
// Copyright (c) 2022-2023 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ResourceSystem.h"
#include <vector>

namespace Siege
{
static std::map<String, Model> models = std::map<String, Model>();

static std::map<String, Texture> textures = std::map<String, Texture>();

void ResourceSystem::RegisterModel(const String& path)
{
    String fullPath = baseDir + path;
//    if (models.find(fullPath) != models.end()) return;
//    models[fullPath] = LoadModel(fullPath);
}

void ResourceSystem::RegisterTexture(const String& path)
{
    String fullPath = baseDir + path;
//    if (textures.find(fullPath) != textures.end()) return;
//    textures[fullPath] = LoadTexture(fullPath);
}

void* ResourceSystem::GetModel(const String& path)
{
    String fullPath = baseDir + path;
    return &models.at(fullPath);
}

void* ResourceSystem::GetTexture(const String& path)
{
    String fullPath = baseDir + path;
    return &textures.at(fullPath);
}

void ResourceSystem::FreeResources()
{
    if (freedModels.empty() && freedTextures.empty()) return;

    // Iterate over vectors backwards to avoid memory issues
    for (auto i = freedModels.rbegin(); i != freedModels.rend(); i++)
    {
//        UnloadModel(**i);
    }
    freedModels.clear();
    models.clear();

    for (auto i = freedTextures.rbegin(); i != freedTextures.rend(); i++)
    {
//        UnloadTexture(**i);
    }
    freedTextures.clear();
    textures.clear();
}

void ResourceSystem::ClearResources()
{
//    for (auto& model : models) freedModels.push_back(&model.second);
//    for (auto& texture : textures) freedTextures.push_back(&texture.second);
}

void ResourceSystem::SetBaseDirectory(const String& dir)
{
    baseDir = dir;
}

const String& ResourceSystem::GetBaseDirectory()
{
    return baseDir;
}
} // namespace Siege