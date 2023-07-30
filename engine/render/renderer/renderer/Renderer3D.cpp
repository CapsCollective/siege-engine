//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer3D.h"

#include <utils/math/vec/Vec2.h>

#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
// static initialisation
Hash::StringId Renderer3D::globalDataId;

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
}

void Renderer3D::DrawBillboard(const Vec3& position,
                               const Vec2& scale,
                               const IColour& colour,
                               Vulkan::Texture2D* texture)
{
    billboardRenderer.DrawBillboard(position, scale, colour, texture);
}

void Renderer3D::DrawMesh(Vulkan::StaticMesh* mesh,
                          const Vec3& position,
                          const Vec3& scale,
                          const Vec3& rotation)
{
    modelRenderer.DrawMesh(mesh, position, scale, rotation);
}

void Renderer3D::DrawPointLight(const Vec3& position,
                                float radius,
                                const IColour& colour,
                                const IColour& ambientColor)
{
    global3DData.lightData = {ToFColour(colour), ToFColour(ambientColor), position};

    lightRenderer.DrawPointLight(position, colour);
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
                        const Camera& cameraData)
{
    global3DData.cameraData = cameraData;
    uint64_t globalDataSize = sizeof(global3DData);

    modelRenderer.Render(commandBuffer, globalDataSize, &global3DData);

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

void Renderer3D::SetGridEnabled(bool enabled)
{
    debugRenderer.SetGridEnabled(enabled);
}

void Renderer3D::RecreateMaterials()
{
    modelRenderer.RecreateMaterials();
    debugRenderer.RecreateMaterials();
    billboardRenderer.RecreateMaterials();
    lightRenderer.RecreateMaterials();
    quadRenderer3D.RecreateMaterials();
    textRenderer.RecreateMaterials();
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
    billboardRenderer.Free();
    lightRenderer.Free();
    quadRenderer3D.Free();
    textRenderer.Free();
}

void Renderer3D::Update()
{
    quadRenderer3D.Update();
    textRenderer.Update();
    billboardRenderer.Update();
}
} // namespace Siege
