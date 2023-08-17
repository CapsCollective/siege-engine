//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERRESOURCES_H
#define SIEGE_ENGINE_RENDERRESOURCES_H

#include <render/renderer/platform/vulkan/Font.h>
#include <render/renderer/platform/vulkan/Material.h>
#include <render/renderer/platform/vulkan/Shader.h>
#include <render/renderer/platform/vulkan/StaticMesh.h>

class RenderResources
{
public:

    RenderResources();

    Siege::Vulkan::Material* GetCubeMaterial();
    Siege::Vulkan::StaticMesh* GetCubeMesh();
    Siege::Vulkan::Font& GetFont();

private:

    Siege::Vulkan::Material cubeMaterial;
    Siege::Vulkan::StaticMesh cubeMesh;
    Siege::Vulkan::Font defaultFont;
};

#endif // SIEGE_ENGINE_RENDERRESOURCES_H
