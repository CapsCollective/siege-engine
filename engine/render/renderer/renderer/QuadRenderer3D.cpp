//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "QuadRenderer3D.h"

#include <utils/math/Graphics.h>

#include "render/renderer/platform/vulkan/Swapchain.h"

namespace Siege
{
void QuadRenderer3D::Initialise(const String& globalDataAttributeName)
{
    globalDataId = INTERN_STR(globalDataAttributeName);

    using Vulkan::Material;
    using Vulkan::Mesh;
    using Vulkan::Shader;

    defaultTexture = Siege::Vulkan::Texture2D("default");

    defaultMaterial =
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/Quad3D.vert.spv")
                     .WithVertexBinding(Shader::VertexBinding()
                                            .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                            .AddMat4Attribute()
                                            .AddFloatVec4Attribute()
                                            .AddFloatVec4Attribute())
                     .WithGlobalData3DUniform()
                     .WithPushConstant(sizeof(uint32_t))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/Quad3D.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(&defaultTexture)
                     .Build());

    textureId = INTERN_STR("texture");

    auto size = sizeof(QuadVertex) * VERTEX_BUFFER_SIZE;

    uint32_t fontIndices[] = {0, 1, 3, 1, 2, 3};

    perFrameVertexBuffers = MHArray<Vulkan::VertexBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        perFrameVertexBuffers[i] = Vulkan::VertexBuffer(size);
    }

    indexBuffer = Vulkan::IndexBuffer(6, fontIndices);
}

void QuadRenderer3D::DrawQuad(const Siege::Vec3& position,
                              const Siege::Vec2& scale,
                              const Siege::Vec3& rotation,
                              const IColour& colour,
                              Vulkan::Texture2D* texture)
{
    auto targetTexture = texture == nullptr ? &defaultTexture : texture;

    auto texIndex = defaultMaterial.SetTexture(textureId, targetTexture);

    if (texIndex >= quads.Count()) quads.Append(MHArray<QuadVertex>(MAX_QUADS_PER_TEXTURE));

    auto& textureQuads = quads[texIndex];

    textureQuads.Append({Graphics::CalculateTransform3D(position, rotation, scale),
                         ToFColour(colour),
                         {0.f, 0.f, 1.f, 1.f}});
}

void QuadRenderer3D::RecreateMaterials()
{
    defaultMaterial.Recreate();
}

void QuadRenderer3D::Render(Vulkan::CommandBuffer& buffer,
                            const uint64_t& globalDataSize,
                            const void* globalData,
                            uint32_t frameIndex)
{
    defaultMaterial.SetUniformData(globalDataId, globalDataSize, globalData);

    for (uint32_t i = 0; i < quads.Count(); i++)
    {
        uint32_t texIndex = i;

        defaultMaterial.BindPushConstant(buffer, &texIndex);

        defaultMaterial.Bind(buffer);

        uint64_t fontOffset = (i * MAX_QUADS_PER_TEXTURE);

        auto& vertexBuffer = perFrameVertexBuffers[frameIndex];

        vertexBuffer.Update(sizeof(QuadVertex), quads[i].Data(), quads[i].Count(), fontOffset);

        vertexBuffer.Bind(buffer, &fontOffset);
        indexBuffer.Bind(buffer);

        vkCmdDrawIndexed(buffer.Get(), 6, quads[i].Count(), 0, 0, 0);
    }
}

void QuadRenderer3D::Free()
{
    defaultTexture.Free();
    defaultMaterial.Free();
    for (auto it = perFrameVertexBuffers.CreateIterator(); it; ++it) it->Free();
    indexBuffer.Free();
}

void QuadRenderer3D::Flush()
{
    for (size_t i = 0; i < quads.Count(); i++) quads[i].Clear();
}

void QuadRenderer3D::Update()
{
    defaultMaterial.Update();
}
} // namespace Siege