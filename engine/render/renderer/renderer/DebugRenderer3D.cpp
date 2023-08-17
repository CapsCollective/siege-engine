//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "DebugRenderer3D.h"

#include <cstdint>

#include "render/renderer/platform/vulkan/Swapchain.h"
#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
DebugRenderer3D::DebugRenderer3D() {}

DebugRenderer3D::~DebugRenderer3D() {}

void DebugRenderer3D::Initialise(const String& globalDataAttributeName)
{
    globalDataId = INTERN_STR(globalDataAttributeName);

    lineMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/DebugLine.vert.spv")
            .WithVertexBinding(
                Vulkan::Shader::VertexBinding().AddFloatVec3Attribute().AddFloatVec4Attribute())
            .WithGlobalData3DUniform()
            .WithVertexTopology(Vulkan::Utils::TOPOLOGY_LINE_LIST)
            .Build(),
        Vulkan::Shader::Builder().FromFragmentShader("assets/shaders/DebugLine.frag.spv").Build());

    perFrameVertexBuffers = MHArray<Vulkan::VertexBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        perFrameVertexBuffers[i] = Vulkan::VertexBuffer(sizeof(LineVertex) * MAX_LINES);
    }

    gridMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/Grid3D.vert.spv")
            .WithGlobalData3DUniform()
            .Build(),
        Vulkan::Shader::Builder().FromFragmentShader("assets/shaders/Grid3D.frag.spv").Build(),
        false);
}

void DebugRenderer3D::Destroy()
{
    lineMaterial.Free();
    for (auto it = perFrameVertexBuffers.CreateFIterator(); it; ++it) it->Free();
    gridMaterial.~Material();
}

void DebugRenderer3D::RecreateMaterials()
{
    lineMaterial.Recreate();
    gridMaterial.Recreate();
}

// Wire primitives
void DebugRenderer3D::DrawLine(const Vec3& origin, const Vec3& destination, const IColour& colour)
{
    auto fColour = ToFColour(colour);
    lines.Append({origin, fColour});
    lines.Append({destination, fColour});
}

void DebugRenderer3D::Flush()
{
    lines.Clear();
}

void DebugRenderer3D::RenderLines(Vulkan::CommandBuffer& buffer,
                                  const uint64_t& globalDataSize,
                                  const void* globalData,
                                  uint32_t currentFrame)
{
    auto& vBuffer = perFrameVertexBuffers[currentFrame];

    if (lines.Count() == 0) return;

    lineMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lineMaterial.Bind(buffer);

    vBuffer.Copy(lines.Data(), sizeof(LineVertex) * lines.Count(), 0);

    uint64_t bindOffset = 0;

    vBuffer.Bind(buffer, &bindOffset);

    Vulkan::Utils::Draw(buffer.Get(), lines.Count(), 1, 0, 0);
}

void DebugRenderer3D::RenderGrid(Vulkan::CommandBuffer& commandBuffer,
                                 const uint64_t& globalDataSize,
                                 const void* globalData)
{
    if (!drawGrid) return;

    gridMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    gridMaterial.Bind(commandBuffer);

    Vulkan::Utils::Draw(commandBuffer.Get(), 6, 1, 0, 0);
}

void DebugRenderer3D::Render(Vulkan::CommandBuffer& commandBuffer,
                             const uint64_t& globalDataSize,
                             const void* globalData,
                             uint32_t currentFrame)
{
    RenderLines(commandBuffer, globalDataSize, globalData, currentFrame);
    RenderGrid(commandBuffer, globalDataSize, globalData);
}

void DebugRenderer3D::SetGridEnabled(bool enabled)
{
    drawGrid = enabled;
}
} // namespace Siege
