//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "RenderResources.h"

#include <render/renderer/renderer/ModelRenderer.h>

RenderResources::RenderResources()
{
    using namespace Siege::Vulkan;

    cubeMaterial = Material(Shader::Builder()
                                .FromVertexShader("assets/shaders/simpleShader.vert.spv")
                                .WithVertexBinding(Shader::VertexBinding()
                                                       .AddFloatVec3Attribute()
                                                       .AddFloatVec4Attribute()
                                                       .AddFloatVec3Attribute()
                                                       .AddFloatVec2Attribute())
                                .WithStorage<Siege::ModelTransform>("transforms", 0, 1000)
                                .WithGlobalData3DUniform()
                                .Build(),
                            Shader::Builder()
                                .FromFragmentShader("assets/shaders/diffuseFragShader.frag.spv")
                                .WithGlobalData3DUniform()
                                .Build());

    cubeMesh = Siege::Vulkan::StaticMesh("assets/models/cube/cube.obj", &cubeMaterial);

    defaultFont = Siege::Vulkan::Font("assets/fonts/PublicPixel.ttf");
}

Siege::Vulkan::Material* RenderResources::GetCubeMaterial()
{
    return &cubeMaterial;
}

Siege::Vulkan::StaticMesh* RenderResources::GetCubeMesh()
{
    return &cubeMesh;
}

Siege::Vulkan::Font& RenderResources::GetFont()
{
    return defaultFont;
}