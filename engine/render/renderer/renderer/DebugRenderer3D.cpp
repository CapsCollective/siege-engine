//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "DebugRenderer3D.h"

namespace Siege
{
DebugRenderer3D::DebugRenderer3D() = default;

DebugRenderer3D::~DebugRenderer3D() = default;

void DebugRenderer3D::Initialise(const String& globalDataAttributeName, const u64& globalDataSize)
{
    globalDataId = INTERN_STR(globalDataAttributeName);

    // vertex Shaders
    auto vertexShader =
        Shader::BuildShader()
            .FromShader("assets/shaders/line.vert.spv")
            .WithStage(PipelineConfig::VERTEX)
            .WithVertexType(sizeof(LineVertex))
            .WithVertexAttribute(offsetof(LineVertex, position), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(LineVertex, colour), Siege::VertexDescription::VEC3)
            .WithUniform(0, globalDataAttributeName, globalDataSize, 1);

    // fragmentShaders
    auto fragmentShader = Shader::BuildShader()
                              .FromShader("assets/shaders/line.frag.spv")
                              .WithStage(PipelineConfig::FRAGMENT);

    lineMaterial.SetVertexShader(&vertexShader);
    lineMaterial.SetFragmentShader(&fragmentShader);
    lineMaterial.SetTopology(Material::Topology::LINE_LIST);
    lineMaterial.BuildMaterial();

    // Set empty mesh
    lineModel.SetMesh({sizeof(LineVertex), nullptr, 0, nullptr, 0});

    lineModel.SetMaterial(&lineMaterial);
}

void DebugRenderer3D::Destroy()
{
    lineMaterial.DestroyMaterial();
    lineModel.DestroyModel();
    rectMaterial.DestroyMaterial();
    rectModel.DestroyModel();
}

void DebugRenderer3D::RecreateMaterials()
{
    lineMaterial.RecreatePipeline();
    rectMaterial.RecreatePipeline();
}

// Wire primitives
void DebugRenderer3D::DrawLine(const glm::vec3& origin,
                               const glm::vec3& destination,
                               const glm::vec3& colour)
{
    lines.Append({origin, colour});
    lines.Append({destination, colour});
}

void DebugRenderer3D::Flush()
{
    lines.Clear();
}

void DebugRenderer3D::RenderLines(VkCommandBuffer& commandBuffer,
                                  const u64& globalDataSize,
                                  const void* globalData)
{
    if (lines.Count() == 0) return;

    lineMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lineMaterial.Bind(commandBuffer);

    lineModel.UpdateMesh(
        {sizeof(LineVertex), lines.Data(), static_cast<u32>(lines.Count()), nullptr, 0});

    lineModel.Bind(commandBuffer);
    lineModel.Draw(commandBuffer, 0);
}

void DebugRenderer3D::Render(VkCommandBuffer& commandBuffer,
                             const u64& globalDataSize,
                             const void* globalData)
{
    RenderLines(commandBuffer, globalDataSize, globalData);
}
} // namespace Siege