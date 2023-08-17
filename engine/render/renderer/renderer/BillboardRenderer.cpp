//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "BillboardRenderer.h"

#include <utils/math/vec/Vec2.h>

#include <cstdint>

#include "render/renderer/platform/vulkan/Swapchain.h"
#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
BillboardRenderer::BillboardRenderer() {}
BillboardRenderer::~BillboardRenderer() {}

void BillboardRenderer::Initialise(const String& globalDataAttributeName)
{
    using namespace Vulkan;
    globalDataId = INTERN_STR(globalDataAttributeName);

    defaultTexture = Siege::Vulkan::Texture2D("default");

    billboardMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/BillboardInstanced.vert.spv")
            .WithVertexBinding(Shader::VertexBinding()
                                   .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                   .AddFloatVec3Attribute()
                                   .AddFloatVec3Attribute()
                                   .AddFloatVec4Attribute()
                                   .AddFloatVec4Attribute())
            .WithGlobalData3DUniform()
            .WithPushConstant(sizeof(uint32_t))
            .Build(),
        Vulkan::Shader::Builder()
            .FromFragmentShader("assets/shaders/BillboardInstanced.frag.spv")
            .WithTexture("texture", 0, 16)
            .WithDefaultTexture(&defaultTexture)
            .Build());

    textureId = INTERN_STR("texture");

    uint32_t fontIndices[] = {0, 1, 3, 1, 2, 3};

    perFrameVertexBuffers = MHArray<Vulkan::VertexBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        perFrameVertexBuffers[i] =
            Vulkan::VertexBuffer(sizeof(BillboardVertex) * VERTEX_BUFFER_SIZE);
    }

    vertices = MHArray<MHArray<BillboardVertex>>(MAX_TEXTURES);

    indexBuffer = Vulkan::IndexBuffer(sizeof(uint32_t) * 6);
    indexBuffer.Copy(fontIndices, sizeof(uint32_t) * 6);
}

void BillboardRenderer::RecreateMaterials()
{
    billboardMaterial.Recreate();
}

void BillboardRenderer::Free()
{
    defaultTexture.Free();
    billboardMaterial.Free();
    for (auto it = perFrameVertexBuffers.CreateFIterator(); it; ++it) it->Free();
    indexBuffer.Free();
}

void BillboardRenderer::DrawBillboard(const Vec3& position,
                                      const Vec2& scale,
                                      const IColour& colour,
                                      Vulkan::Texture2D* texture)
{
    auto targetTexture = texture == nullptr ? &defaultTexture : texture;

    auto texIndex = billboardMaterial.SetTexture(textureId, targetTexture);

    if (texIndex >= vertices.Count())
        vertices.Append(MHArray<BillboardVertex>(MAX_QUADS_PER_TEXTURE));

    auto& textureQuads = vertices[texIndex];

    textureQuads.Append({position, scale, ToFColour(colour), {0.f, 0.f, 1.f, 1.f}});
}

void BillboardRenderer::Render(Vulkan::CommandBuffer& buffer,
                               const uint64_t& globalDataSize,
                               const void* globalData,
                               uint32_t currentFrame)
{
    billboardMaterial.SetUniformData(globalDataId, globalDataSize, globalData);

    auto& vBuffer = perFrameVertexBuffers[currentFrame];

    for (uint32_t i = 0; i < vertices.Count(); i++)
    {
        billboardMaterial.BindPushConstant(buffer, &i);

        billboardMaterial.Bind(buffer);

        uint64_t quadOffset = (i * MAX_QUADS_PER_TEXTURE);

        vBuffer.Copy(vertices[i].Data(),
                     sizeof(BillboardVertex) * vertices[i].Count(),
                     sizeof(BillboardVertex) * quadOffset);

        uint64_t bindOffset = sizeof(BillboardVertex) * quadOffset;

        vBuffer.Bind(buffer, &bindOffset);
        indexBuffer.Bind(buffer);

        Vulkan::Utils::DrawIndexed(buffer.Get(), 6, vertices[i].Count(), 0, 0, 0);
    }
}

void BillboardRenderer::Flush()
{
    for (size_t i = 0; i < vertices.Count(); i++) vertices[i].Clear();
}

void BillboardRenderer::Update()
{
    billboardMaterial.Update();
}
} // namespace Siege
