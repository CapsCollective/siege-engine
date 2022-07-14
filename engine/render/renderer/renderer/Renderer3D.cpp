//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer3D.h"

#include <iostream>

namespace Siege
{
// static initialisation
StringId Renderer3D::globalDataId;

Material Renderer3D::gridMaterial;

ModelRenderer Renderer3D::modelRenderer;
DebugRenderer3D Renderer3D::debugRenderer;
BillboardRenderer Renderer3D::billboardRenderer;
LightRenderer Renderer3D::lightRenderer;

Renderer3D::GlobalData Renderer3D::global3DData;

void Renderer3D::Initialise()
{
    globalDataId = INTERN_STR("globalData");

    modelRenderer.Initialise("globalData", sizeof(GlobalData));
    debugRenderer.Initialise("globalData", sizeof(GlobalData));
    billboardRenderer.Initialise("globalData", sizeof(GlobalData));
    lightRenderer.Initialise("globalData", sizeof(GlobalData));

    auto gridShader = Siege::Shader::BuildShader()
                          .FromShader("assets/shaders/grid.vert.spv")
                          .WithStage(Siege::PipelineConfig::VERTEX)
                          .WithUniform(0, "globalData", sizeof(Siege::Renderer3D::GlobalData));

    auto gridFragShader = Siege::Shader::BuildShader()
                              .FromShader("assets/shaders/grid.frag.spv")
                              .WithStage(Siege::PipelineConfig::FRAGMENT);

    gridMaterial.SetVertexShader(&gridShader);
    gridMaterial.SetFragmentShader(&gridFragShader);
    gridMaterial.BuildMaterial();
}

void Renderer3D::DrawBillboard(const Siege::Vec3& position,
                               const Siege::Vec2& scale,
                               const Siege::Vec4& colour)
{
    billboardRenderer.DrawBillboard(position, scale, colour);
}

void Renderer3D::DrawModel(Model* model,
                           const Siege::Vec3& position,
                           const Siege::Vec3& scale,
                           const Siege::Vec3& rotation)
{
    modelRenderer.DrawModel(model, position, scale, rotation);
}

void Renderer3D::DrawModel(Model* model, const Siege::Vec3& position, const Siege::Vec3& scale)
{
    DrawModel(model, position, scale, Siege::Vec3::Zero);
}

void Renderer3D::DrawModel(Model* model, const Siege::Vec3& position)
{
    DrawModel(model, position, Siege::Vec3::One, Siege::Vec3::Zero);
}

void Renderer3D::DrawPointLight(const Siege::Vec3& position,
                                const float& radius,
                                const Siege::Vec4& colour,
                                const Siege::Vec4& ambientColor)
{
    global3DData.lightData = {colour, ambientColor, position};

    lightRenderer.DrawPointLight(position, 0.05f, colour, ambientColor);
}

void Renderer3D::Render(VkCommandBuffer& commandBuffer, const CameraData& cameraData)
{
    global3DData.cameraData = cameraData;
    u64 globalDataSize = sizeof(global3DData);

    modelRenderer.Render(commandBuffer, globalDataSize, &global3DData);
    lightRenderer.Render(commandBuffer, globalDataSize, &global3DData);
    debugRenderer.Render(commandBuffer, globalDataSize, &global3DData);
    billboardRenderer.Render(commandBuffer, globalDataSize, &global3DData);

#ifdef ENABLE_GRID
    RenderGrid(commandBuffer, global3DData);
#endif
}

void Renderer3D::DrawLine(const Siege::Vec3& origin,
                          const Siege::Vec3& destination,
                          const Siege::Vec4& colour)
{
    debugRenderer.DrawLine(origin, destination, colour);
}

void Renderer3D::RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
{
    gridMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
    gridMaterial.Bind(commandBuffer);

    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

void Renderer3D::RecreateMaterials()
{
    modelRenderer.RecreateMaterials();
    debugRenderer.RecreateMaterials();
    billboardRenderer.RecreateMaterials();
    lightRenderer.RecreateMaterials();

    gridMaterial.RecreatePipeline();
}

void Renderer3D::Flush()
{
    modelRenderer.Flush();
    debugRenderer.Flush();
    billboardRenderer.Flush();
    lightRenderer.Flush();
}

void Renderer3D::DestroyRenderer3D()
{
    modelRenderer.Destroy();
    debugRenderer.Destroy();
    billboardRenderer.Destroy();
    lightRenderer.Destroy();

    gridMaterial.DestroyMaterial();
}
} // namespace Siege
