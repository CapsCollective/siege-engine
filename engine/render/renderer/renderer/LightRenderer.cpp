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
LightRenderer::LightRenderer() {}
LightRenderer::~LightRenderer() {}

void LightRenderer::Initialise(const String& globalDataAttributeName,
                               const uint64_t& globalDataSize)
{
    globalDataId = INTERN_STR(globalDataAttributeName);
    lightDataId = INTERN_STR("lightUBO");

    auto pointLightVertShader = Shader::BuildShader()
                                    .FromShader("assets/shaders/pointLight.vert.spv")
                                    .WithStage(PipelineConfig::VERTEX)
                                    .WithVertexType(sizeof(Vec2))
                                    .WithVertexAttribute(0, VertexDescription::VEC2)
                                    .WithUniform(0, globalDataAttributeName, globalDataSize);

    auto pointLightFragShader = Shader::BuildShader()
                                    .FromShader("assets/shaders/pointLight.frag.spv")
                                    .WithStage(PipelineConfig::FRAGMENT)
                                    .WithUniform(0, globalDataAttributeName, globalDataSize);

    lightMaterial.SetVertexShader(&pointLightVertShader);
    lightMaterial.SetFragmentShader(&pointLightFragShader);
    lightMaterial.BuildMaterial();

    lightModel.SetMesh({sizeof(glm::vec2), nullptr, 0, nullptr, 0});

    lightModel.SetMaterial(&lightMaterial);
}

void LightRenderer::Destroy()
{
    lightMaterial.DestroyMaterial();
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

void LightRenderer::Render(VkCommandBuffer& commandBuffer,
                           const uint64_t& globalDataSize,
                           const void* globalData)
{
    if (pointLightVertices.Count() == 0) return;

    lightMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lightMaterial.Bind(commandBuffer);

    lightModel.UpdateMesh({sizeof(glm::vec2),
                           pointLightVertices.Data(),
                           static_cast<uint32_t>(pointLightVertices.Count()),
                           pointLightIndices.Data(),
                           static_cast<uint32_t>(pointLightIndices.Count())});

    lightModel.Bind(commandBuffer);
    lightModel.Draw(commandBuffer, 0);
}

void LightRenderer::Flush()
{
    pointLightVertices.Clear();
    pointLightIndices.Clear();
}

void LightRenderer::RecreateMaterials()
{
    lightMaterial.RecreatePipeline();
}
} // namespace Siege