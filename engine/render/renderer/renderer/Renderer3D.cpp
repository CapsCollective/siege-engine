//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer3D.h"

#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
// static initialisation
Hash::StringId Renderer3D::globalDataId;

Vulkan::Material Renderer3D::gridMaterial;

ModelRenderer Renderer3D::modelRenderer;
DebugRenderer3D Renderer3D::debugRenderer;
BillboardRenderer Renderer3D::billboardRenderer;
LightRenderer Renderer3D::lightRenderer;
TextRenderer Renderer3D::textRenderer;
QuadRenderer3D Renderer3D::quadRenderer3D;

Renderer3D::GlobalData Renderer3D::global3DData;

void Renderer3D::Initialise()
{
    globalDataId = INTERN_STR("globalData");

    modelRenderer.Initialise("globalData");
    debugRenderer.Initialise("globalData");
    billboardRenderer.Initialise("globalData");
    lightRenderer.Initialise("globalData");
    textRenderer.Initialise("globalData");
    quadRenderer3D.Initialise("globalData");

    gridMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/grid.vert.spv")
            .WithGlobalData3DUniform()
            .Build(),
        Vulkan::Shader::Builder().FromFragmentShader("assets/shaders/grid.frag.spv").Build(),
        false);
}

void Renderer3D::DrawBillboard(const Vec3& position, const Vec2& scale, const IColour& colour)
{
    billboardRenderer.DrawBillboard(position, scale, colour);
}

void Renderer3D::DrawModel(Model* model,
                           const Vec3& position,
                           const Vec3& scale,
                           const Vec3& rotation)
{
    modelRenderer.DrawModel(model, position, scale, rotation);
}

void Renderer3D::DrawModel(Model* model, const Vec3& position, const Vec3& scale)
{
    DrawModel(model, position, scale, Vec3::Zero);
}

void Renderer3D::DrawModel(Model* model, const Vec3& position)
{
    DrawModel(model, position, Vec3::One, Vec3::Zero);
}

void Renderer3D::DrawPointLight(const Vec3& position,
                                float radius,
                                const IColour& colour,
                                const IColour& ambientColor)
{
    global3DData.lightData = {ToFColour(colour), ToFColour(ambientColor), position};

    lightRenderer.DrawPointLight(position, 0.05f, colour, ambientColor);
}

void Renderer3D::DrawText3D(const char* text,
                            const Vec3 position,
                            const Vec3 rotation,
                            const Vec2 scale,
                            const IColour& colour,
                            Vulkan::Font* font)
{
    textRenderer.DrawFont(text, position, rotation, scale, colour, font);
}

void Renderer3D::DrawQuad(const Vec3 position,
                          const Vec2 scale,
                          const Vec3 rotation,
                          IColour colour,
                          Vulkan::Texture2D* texture)
{
    quadRenderer3D.DrawQuad(position, scale, rotation, colour, texture);
}

void Renderer3D::Render(uint32_t currentFrame,
                        Vulkan::CommandBuffer& commandBuffer,
                        const CameraData& cameraData)
{
    global3DData.cameraData = cameraData;
    uint64_t globalDataSize = sizeof(global3DData);

    modelRenderer.Render(commandBuffer, globalDataSize, &global3DData, currentFrame);

#ifdef ENABLE_GRID
    RenderGrid(commandBuffer, global3DData);
#endif

    lightRenderer.Render(commandBuffer, globalDataSize, &global3DData, currentFrame);
    debugRenderer.Render(commandBuffer, globalDataSize, &global3DData, currentFrame);
    billboardRenderer.Render(commandBuffer, globalDataSize, &global3DData, currentFrame);
    quadRenderer3D.Render(commandBuffer, globalDataSize, &global3DData, currentFrame);
    textRenderer.Render(commandBuffer, globalDataSize, &global3DData, currentFrame);
}

void Renderer3D::DrawLine(const Vec3& origin, const Vec3& destination, const IColour& colour)
{
    debugRenderer.DrawLine(origin, destination, colour);
}

void Renderer3D::RenderGrid(Vulkan::CommandBuffer& commandBuffer, const GlobalData& globalData)
{
    gridMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
    gridMaterial.Bind(commandBuffer);

    Vulkan::Utils::Draw(commandBuffer.GetActiveCommandBuffer(), 6, 1, 0, 0);
}

void Renderer3D::RecreateMaterials()
{
    modelRenderer.RecreateMaterials();
    debugRenderer.RecreateMaterials();
    billboardRenderer.RecreateMaterials();
    lightRenderer.RecreateMaterials();
    quadRenderer3D.RecreateMaterials();
    textRenderer.RecreateMaterials();

    gridMaterial.Recreate();
}

void Renderer3D::Flush()
{
    modelRenderer.Flush();
    debugRenderer.Flush();
    billboardRenderer.Flush();
    lightRenderer.Flush();
    quadRenderer3D.Flush();
    textRenderer.Flush();
}

void Renderer3D::DestroyRenderer3D()
{
    debugRenderer.Destroy();
    billboardRenderer.Destroy();
    lightRenderer.Destroy();
    quadRenderer3D.Free();
    textRenderer.Free();

    gridMaterial.~Material();
}

void Renderer3D::Update()
{
    quadRenderer3D.Update();
    textRenderer.Update();
}
} // namespace Siege
