//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "QuadRenderer3D.h"

#include <utils/math/Transform.h>

#include <cstdint>

#include "render/renderer/platform/vulkan/Swapchain.h"
#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
void QuadRenderer3D::Initialise(const String& globalDataAttributeName)
{
    globalDataId = INTERN_STR(globalDataAttributeName);

    using Vulkan::Material;
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

    uint32_t quadIndices[] = {0, 1, 3, 1, 2, 3};

    perFrameVBuffers = MHArray<Vulkan::VertexBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        perFrameVBuffers[i] = Vulkan::VertexBuffer(sizeof(QuadVertex) * VERTEX_BUFFER_SIZE);
    }

    indexBuffer = Vulkan::IndexBuffer(quadIndices, sizeof(unsigned int) * 6);
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

    textureQuads.Append(
        {Transform3D(position, rotation, scale), ToFColour(colour), {0.f, 0.f, 1.f, 1.f}});
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

    auto& vBuffer = perFrameVBuffers[frameIndex];

    for (uint32_t i = 0; i < quads.Count(); i++)
    {
        defaultMaterial.BindPushConstant(buffer, &i);

        defaultMaterial.Bind(buffer);

        uint64_t bindOffset = sizeof(QuadVertex) * (i * MAX_QUADS_PER_TEXTURE);

        vBuffer.Copy(quads[i].Data(), sizeof(QuadVertex) * quads[i].Count(), bindOffset);

        vBuffer.Bind(buffer, &bindOffset);
        indexBuffer.Bind(buffer);

        Vulkan::Utils::DrawIndexed(buffer.Get(), 6, quads[i].Count(), 0, 0, 0);
    }
}

void QuadRenderer3D::Free()
{
    defaultTexture.Free();
    defaultMaterial.Free();
    for (auto it = perFrameVBuffers.CreateIterator(); it; ++it) it->Free();
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