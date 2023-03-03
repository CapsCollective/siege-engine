//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "BillboardRenderer.h"

namespace Siege
{
BillboardRenderer::BillboardRenderer() {}
BillboardRenderer::~BillboardRenderer() {}

void BillboardRenderer::Initialise(const String& globalDataAttributeName,
                                   const uint64_t& globalDataSize)
{
    globalDataId = INTERN_STR(globalDataAttributeName);
    positionsId = INTERN_STR("positions");

    billboardMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/billboard.vert.spv")
            .WithVertexBinding(
                Vulkan::Shader::VertexBinding().AddFloatVec3Attribute().AddFloatVec4Attribute())
            .WithGlobalData3DUniform()
            .WithStorage<BillboardUBO>("positions", 0, 1000)
            .Build(),
        Vulkan::Shader::Builder().FromFragmentShader("assets/shaders/billboard.frag.spv").Build());

    billboardModel.SetMesh({sizeof(BillboardVertex), nullptr, 0, nullptr, 0});

    billboardModel.SetMaterial(&billboardMaterial);
}

void BillboardRenderer::RecreateMaterials()
{
    billboardMaterial.Recreate();
}

void BillboardRenderer::Destroy()
{
    billboardMaterial.~Material();
    billboardModel.DestroyModel();
}

void BillboardRenderer::DrawBillboard(const Vec3& position,
                                      const Vec2& scale,
                                      const IColour& colour)
{
    auto fColour = ToFColour(colour);
    vertices.Append({{1.f, 1.f, 1.f}, fColour});
    vertices.Append({{1.f, -1.f, 1.f}, fColour});
    vertices.Append({{-1.f, -1.f, 1.f}, fColour});
    vertices.Append({{-1.f, 1.f, 1.f}, fColour});

    // Pattern: 0, 1, 3, 1, 2, 3
    indices.Append(vertices.Count() - 4);
    indices.Append(vertices.Count() - 3);
    indices.Append(vertices.Count() - 1);

    indices.Append(vertices.Count() - 3);
    indices.Append(vertices.Count() - 2);
    indices.Append(vertices.Count() - 1);

    positions.Append({position, {scale.x, scale.y, 0.f}});
}

void BillboardRenderer::Render(Vulkan::CommandBuffer& buffer,
                               const uint64_t& globalDataSize,
                               const void* globalData)
{
    if (vertices.Count() == 0) return;

    billboardMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    billboardMaterial.SetUniformData(positionsId,
                                     sizeof(positions[0]) * positions.Count(),
                                     positions.Data());
    billboardMaterial.Bind(buffer);

    billboardModel.UpdateMesh({sizeof(BillboardVertex),
                               vertices.Data(),
                               static_cast<uint32_t>(vertices.Count()),
                               indices.Data(),
                               static_cast<uint32_t>(indices.Count())});

    billboardModel.Bind(buffer);
    billboardModel.Draw(buffer);
}

void BillboardRenderer::Flush()
{
    vertices.Clear();
    indices.Clear();
    positions.Clear();
}
} // namespace Siege
