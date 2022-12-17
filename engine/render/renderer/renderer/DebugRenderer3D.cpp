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
DebugRenderer3D::DebugRenderer3D() {}

DebugRenderer3D::~DebugRenderer3D() {}

void DebugRenderer3D::Initialise(const String& globalDataAttributeName,
                                 const uint64_t& globalDataSize)
{
    globalDataId = INTERN_STR(globalDataAttributeName);

    // vertex Shaders
    auto vertexShader =
        Shader::BuildShader()
            .FromShader("assets/shaders/line.vert.spv")
            .WithStage(PipelineConfig::VERTEX)
            .WithVertexType(sizeof(LineVertex))
            .WithVertexAttribute(offsetof(LineVertex, position), VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(LineVertex, colour), VertexDescription::VEC3)
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
void DebugRenderer3D::DrawLine(const Vec3& origin, const Vec3& destination, const Vec4& colour)
{
    lines.Append({origin, colour});
    lines.Append({destination, colour});
}

void DebugRenderer3D::DrawCube(const Vec3& position, const Vec3& rotation, const Vec3& scale) {}

void DebugRenderer3D::Flush()
{
    lines.Clear();
}

void DebugRenderer3D::RenderLines(VkCommandBuffer& commandBuffer,
                                  const uint64_t& globalDataSize,
                                  const void* globalData)
{
    if (lines.Count() == 0) return;

    lineMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lineMaterial.Bind(commandBuffer);

    lineModel.UpdateMesh(
        {sizeof(LineVertex), lines.Data(), static_cast<uint32_t>(lines.Count()), nullptr, 0});

    lineModel.Bind(commandBuffer);
    lineModel.Draw(commandBuffer, 0);
}

void DebugRenderer3D::Render(VkCommandBuffer& commandBuffer,
                             const uint64_t& globalDataSize,
                             const void* globalData)
{
    RenderLines(commandBuffer, globalDataSize, globalData);
}
} // namespace Siege