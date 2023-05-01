//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer2D.h"
#include <utils/Logging.h>
#include <utils/math/Graphics.h>

namespace Siege
{
void Renderer2D::Initialise(const char* const globalDataName)
{
    globalDataId = INTERN_STR(globalDataName);
    textureId = INTERN_STR("texture");

    using Vulkan::Material;
    using Vulkan::Shader;

    defaultTexture = Siege::Vulkan::Texture2D("default");

    quadMaterial =
        Material(
            Shader::Builder()
                .FromVertexShader("assets/shaders/Quad2DInstanced.vert.spv")
                .WithVertexBinding(
                    Shader::VertexBinding()
                        .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                        .AddMat4Attribute()
                        .AddFloatVec4Attribute()
                        .AddFloatVec4Attribute())
                .WithUniform<CameraData>(globalDataName)
                .WithPushConstant(sizeof(uint32_t))
                .Build(),
            Shader::Builder()
                .FromFragmentShader("assets/shaders/Quad2DInstanced.frag.spv")
                .WithTexture("texture", 0, 16)
                .WithDefaultTexture(&defaultTexture)
                .Build());

    uint32_t fontIndices[] = {0, 1, 3, 1, 2, 3};

    quadIndexBuffer = Vulkan::IndexBuffer(6, fontIndices);

    quadVertexBuffer = Vulkan::VertexBuffer(sizeof(QuadVertex) * VERTEX_BUFFER_SIZE);
}

void Renderer2D::DrawQuad(const Vec2 position, const Vec2 scale, IColour colour, float rotation, uint8_t zIndex, Vulkan::Texture2D* texture)
{
    CC_ASSERT(zIndex < MAX_LAYERS, "zIndex provided is larger than the maximum number of layers")

    auto targetTexture = texture == nullptr ? &defaultTexture : texture;

    auto texIndex = quadMaterial.SetTexture(textureId, targetTexture);

    auto& layerQuads = quads[zIndex];

    if (texIndex >= layerQuads.Count()) layerQuads.Append(MHArray<QuadVertex>(MAX_QUADS_PER_LAYER));

    layerQuads[texIndex].Append({Graphics::CalculateTransform3D(position, {0.f, 0.f, rotation}, scale), ToFColour(colour), {0.f, 0.f, 1.f, 1.f}});
}

void Renderer2D::Render(Vulkan::CommandBuffer& buffer,
                        const uint64_t& globalDataSize,
                        const void* globalData,
                        uint32_t frameIndex)
{
    quadMaterial.SetUniformData(globalDataId, globalDataSize, globalData);

    for (int i = MAX_LAYERS-1; i >= 0; i--)
    {
        if (quads[i].Count() == 0) continue;

        auto& perTextureQuads = quads[i];

        for(size_t j = 0; j < perTextureQuads.Count(); j++)
        {
            auto& quadArr = perTextureQuads[j];

            if (quadArr.Count() == 0) continue;

            quadMaterial.BindPushConstant(buffer, &j);
            quadMaterial.Bind(buffer);

            uint64_t vertexBufferOffset = (i * MAX_TEXTURES) + (j * MAX_QUADS_PER_LAYER);

            quadVertexBuffer.Update(sizeof(QuadVertex), quadArr.Data(), quadArr.Count(), vertexBufferOffset);

            quadVertexBuffer.Bind(buffer, &vertexBufferOffset);
            quadIndexBuffer.Bind(buffer);

            vkCmdDrawIndexed(buffer.Get(), 6, quadArr.Count(), 0, 0, 0);
        }
    }
}

void Renderer2D::Update()
{
    quadMaterial.Update();
}

void Renderer2D::Flush()
{
    for (int i = quads.Size()-1; i >= 0; i--)
    {
        for(size_t j = 0; j < quads[i].Count(); j++) quads[i][j].Clear();
    }
}
} // namespace Siege