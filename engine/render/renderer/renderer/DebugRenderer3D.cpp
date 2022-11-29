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

    lineMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/line.vert.spv")
            .WithVertexBinding(
                Vulkan::Shader::VertexBinding().AddFloatVec3Attribute().AddFloatVec4Attribute())
            .WithGlobalData3DUniform()
            .WithVertexTopology(Vulkan::Utils::TOPOLOGY_LINE_LIST)
            .Build(),
        Vulkan::Shader::Builder().FromFragmentShader("assets/shaders/line.frag.spv").Build());

    // Set empty mesh
    lineModel.SetMesh({sizeof(LineVertex), nullptr, 0, nullptr, 0});

    lineModel.SetMaterial(&lineMaterial);
}

void DebugRenderer3D::Destroy()
{
    lineMaterial.~Material();
    lineModel.DestroyModel();
}

void DebugRenderer3D::RecreateMaterials()
{
    lineMaterial.Recreate();
}

// Wire primitives
void DebugRenderer3D::DrawLine(const Vec3& origin, const Vec3& destination, const Vec4& colour)
{
    lines.Append({origin, colour});
    lines.Append({destination, colour});
}

void DebugRenderer3D::Flush()
{
    lines.Clear();
}

void DebugRenderer3D::RenderLines(Vulkan::CommandBuffer& buffer,
                                  const uint64_t& globalDataSize,
                                  const void* globalData)
{
    if (lines.Count() == 0) return;

    lineMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lineMaterial.Bind(buffer);

    lineModel.UpdateMesh(
        {sizeof(LineVertex), lines.Data(), static_cast<uint32_t>(lines.Count()), nullptr, 0});

    lineModel.Bind(buffer);
    lineModel.Draw(buffer, 0);
}

void DebugRenderer3D::Render(Vulkan::CommandBuffer& commandBuffer,
                             const uint64_t& globalDataSize,
                             const void* globalData)
{
    RenderLines(commandBuffer, globalDataSize, globalData);
}
} // namespace Siege
