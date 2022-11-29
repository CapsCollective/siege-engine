//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "LightRenderer.h"

namespace Siege
{
void LightRenderer::Initialise(const String& globalDataAttributeName,
                               const uint64_t& globalDataSize)
{
    globalDataId = INTERN_STR(globalDataAttributeName);

    lightMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/pointLight.vert.spv")
            .WithVertexBinding(Vulkan::Shader::VertexBinding().AddFloatVec2Attribute())
            .WithGlobalData3DUniform()
            .Build(),
        Vulkan::Shader::Builder()
            .FromFragmentShader("assets/shaders/pointLight.frag.spv")
            .WithGlobalData3DUniform()
            .Build());

    lightModel.SetMesh({sizeof(Vec2), nullptr, 0, nullptr, 0});

    lightModel.SetMaterial(&lightMaterial);
}

void LightRenderer::Destroy()
{
    lightMaterial.~Material();
    lightModel.DestroyModel();
}

void LightRenderer::DrawPointLight(const Vec3& position,
                                   float radius,
                                   const Vec4& colour,
                                   const Vec4& ambientColor)
{
    pointLightVertices.Append({1.f, 1.f});
    pointLightVertices.Append({1.f, -1.f});
    pointLightVertices.Append({-1.f, -1.f});
    pointLightVertices.Append({-1.f, 1.f});

    // Pattern: 0, 1, 3, 1, 2, 3
    pointLightIndices.Append(pointLightVertices.Count() - 4);
    pointLightIndices.Append(pointLightVertices.Count() - 3);
    pointLightIndices.Append(pointLightVertices.Count() - 1);

    pointLightIndices.Append(pointLightVertices.Count() - 3);
    pointLightIndices.Append(pointLightVertices.Count() - 2);
    pointLightIndices.Append(pointLightVertices.Count() - 1);
}

void LightRenderer::Render(Vulkan::CommandBuffer& buffer,
                           const uint64_t& globalDataSize,
                           const void* globalData)
{
    if (pointLightVertices.Count() == 0) return;

    lightMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lightMaterial.Bind(buffer);

    lightModel.UpdateMesh({sizeof(Vec2),
                           pointLightVertices.Data(),
                           static_cast<uint32_t>(pointLightVertices.Count()),
                           pointLightIndices.Data(),
                           static_cast<uint32_t>(pointLightIndices.Count())});

    lightModel.Bind(buffer);
    lightModel.Draw(buffer, 0);
}

void LightRenderer::Flush()
{
    pointLightVertices.Clear();
    pointLightIndices.Clear();
}

void LightRenderer::RecreateMaterials()
{
    lightMaterial.Recreate();
}
} // namespace Siege
