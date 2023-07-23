//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer2D.h"

#include <utils/Logging.h>
#include <utils/math/Transform.h>

#include <cstdint>

#include "render/renderer/Renderer.h"
#include "render/renderer/platform/vulkan/utils/Draw.h"

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
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/Quad2DInstanced.vert.spv")
                     .WithVertexBinding(Shader::VertexBinding()
                                            .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                            .AddMat4Attribute()
                                            .AddFloatVec4Attribute()
                                            .AddFloatVec4Attribute())
                     .WithUniform<Camera>(globalDataName)
                     .WithPushConstant(sizeof(uint32_t))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/Quad2DInstanced.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(&defaultTexture)
                     .Build(),
                 false);

    textMaterial =
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/Text2DInstanced.vert.spv")
                     .WithVertexBinding(Shader::VertexBinding()
                                            .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                            .AddMat4Attribute()
                                            .AddFloatVec4Attribute()
                                            .AddFloatVec4Attribute()
                                            .AddFloatVec4Attribute())
                     .WithUniform<Camera>(globalDataName)
                     .WithPushConstant(sizeof(uint32_t))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/Text2DInstanced.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(&defaultTexture)
                     .Build(),
                 false);

    gridMaterial =
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/Grid2D.vert.spv")
                     .WithPushConstant(sizeof(GridData))
                     .Build(),
                 Shader::Builder().FromFragmentShader("assets/shaders/Grid2D.frag.spv").Build(),
                 false);

    uint32_t fontIndices[] = {0, 1, 3, 1, 2, 3};

    quadIndexBuffer = Vulkan::IndexBuffer(fontIndices, sizeof(unsigned int) * 6);

    quadVertexBuffer = Vulkan::VertexBuffer(sizeof(QuadVertex) * QUAD_VERTEX_BUFFER_SIZE);
    quadVBuffer = Vulkan::VertexBuffer(sizeof(QuadVertex) * QUAD_VERTEX_BUFFER_SIZE);
    textVertexBuffer = Vulkan::VertexBuffer(sizeof(FontVertex) * TEXT_VERTEX_BUFFER_SIZE);
}

void Renderer2D::DrawQuad(const Vec2 position,
                          const Vec2 scale,
                          IColour colour,
                          float rotation,
                          uint8_t zIndex,
                          Vulkan::Texture2D* texture)
{
    CC_ASSERT(zIndex < MAX_LAYERS, "zIndex provided is larger than the maximum number of layers")

    auto targetTexture = texture == nullptr ? &defaultTexture : texture;

    auto texIndex = quadMaterial.SetTexture(textureId, targetTexture);

    auto& layerQuads = quads[zIndex];

    if (texIndex >= layerQuads.Count())
        layerQuads[texIndex] = MHArray<QuadVertex>(MAX_QUADS_PER_LAYER);

    layerQuads[texIndex].Append({Transform3D({position.x + scale.x, position.y + scale.y, 1.f},
                                             {0.f, 0.f, rotation},
                                             scale),
                                 ToFColour(colour),
                                 {0.f, 0.f, 1.f, 1.f}});
}

void Renderer2D::DrawText2D(const char* const text,
                            const Vec2 position,
                            const Vec2 scale,
                            Vulkan::Font& font,
                            IColour colour,
                            float rotation,
                            uint8_t zIndex)
{
    CC_ASSERT(zIndex < MAX_LAYERS, "zIndex provided is larger than the maximum number of layers")

    auto texIndex = textMaterial.SetTexture(textureId, font.GetTexture());

    auto& layerQuads = characters[zIndex];

    if (texIndex >= layerQuads.Count())
        layerQuads[texIndex] = MHArray<FontVertex>(MAX_TEXTS_PER_FONT * MAX_CHARS_PER_TEXT);

    auto& fontTexts = layerQuads[texIndex];

    size_t textSize = strlen(text);
    float textScale = 64.f;

    float x = 0;
    float y = 0;

    for (size_t i = 0; i < textSize; i++)
    {
        auto glyph = font.GetGlyph(text[i]);

        float height = glyph.height;

        float yOffset = (glyph.bearingY != height) * (glyph.bearingY - height);

        Vec4 coordinates = {x + glyph.bearingX, y - (height + yOffset), glyph.width, height};

        fontTexts.Append(
            {Transform3D({position.x, position.y + scale.y, 1.f}, {0.f, 0.f, rotation}, scale),
             ToFColour(colour),
             {glyph.uvxMin, glyph.uvyMin, glyph.widthNormalised, glyph.heightNormalised},
             coordinates / textScale});

        x += glyph.advance >> 6;
    }
}

void Renderer2D::DrawGrid2D(float spacing,
                            const Vec3& lineColouring,
                            float scale,
                            float lineWidth,
                            float fadeFactor,
                            float cellMultiple)
{
    grid[0] = {{lineColouring.x, lineColouring.y, lineColouring.z, fadeFactor},
               {spacing, cellMultiple, scale, lineWidth}};
}

void Renderer2D::Render(Vulkan::CommandBuffer& buffer,
                        const uint64_t& globalDataSize,
                        const void* globalData,
                        uint32_t frameIndex)
{
    quadMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    textMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    quadIndexBuffer.Bind(buffer);

    RenderGrid(buffer);

    for (int i = MAX_LAYERS - 1; i >= 0; i--)
    {
        // Render 2D quads for this layer

        RenderQuads(buffer, i);

        // Render text for this layer

        RenderText(buffer, i);
    }
}

void Renderer2D::RenderText(Vulkan::CommandBuffer& buffer, size_t index)
{
    if (characters[index].Count() == 0) return;

    auto& perFontQuads = characters[index];

    for (size_t j = 0; j < perFontQuads.Count(); j++)
    {
        auto& quadArr = perFontQuads[j];

        if (quadArr.Count() == 0) continue;

        textMaterial.BindPushConstant(buffer, &j);
        textMaterial.Bind(buffer);

        uint64_t vertexBufferOffset =
            sizeof(FontVertex) *
            ((index * MAX_TEXTURES) + (j * MAX_TEXTS_PER_FONT * MAX_CHARS_PER_TEXT));

        textVertexBuffer.Copy(quadArr.Data(),
                              sizeof(FontVertex) * quadArr.Count(),
                              vertexBufferOffset);

        textVertexBuffer.Bind(buffer, &vertexBufferOffset);

        Vulkan::Utils::DrawIndexed(buffer.Get(), 6, quadArr.Count(), 0, 0, 0);
    }
}

void Renderer2D::RenderQuads(Vulkan::CommandBuffer& buffer, size_t index)
{
    if (quads[index].Count() == 0) return;

    auto& perTextureQuads = quads[index];

    for (size_t j = 0; j < perTextureQuads.Count(); j++)
    {
        auto& quadArr = perTextureQuads[j];

        if (quadArr.Count() == 0) continue;

        quadMaterial.BindPushConstant(buffer, &j);
        quadMaterial.Bind(buffer);

        uint64_t vertexBufferOffset = (index * MAX_TEXTURES) + (j * MAX_QUADS_PER_LAYER);

        quadVBuffer.Copy(quadArr.Data(),
                         sizeof(QuadVertex) * quadArr.Count(),
                         sizeof(QuadVertex) * vertexBufferOffset);

        uint64_t bindOffset = sizeof(QuadVertex) * vertexBufferOffset;

        quadVBuffer.Bind(buffer, &bindOffset);

        Vulkan::Utils::DrawIndexed(buffer.Get(), 6, quadArr.Count(), 0, 0, 0);
    }
}

void Renderer2D::RenderGrid(Vulkan::CommandBuffer& buffer)
{
    for (size_t j = 0; j < grid.Count(); j++)
    {
        gridMaterial.BindPushConstant(buffer, &grid[j]);
        gridMaterial.Bind(buffer);

        Vulkan::Utils::DrawIndexed(buffer.Get(), 6, 1, 0, 0, 0);
    }
}

void Renderer2D::Update()
{
    quadMaterial.Update();
    textMaterial.Update();
    gridMaterial.Update();
}

void Renderer2D::Flush()
{
    for (int i = quads.Size() - 1; i >= 0; i--)
    {
        for (size_t j = 0; j < quads[i].Count(); j++) quads[i][j].Clear();
    }
    for (int i = characters.Size() - 1; i >= 0; i--)
    {
        for (size_t j = 0; j < characters[i].Count(); j++) characters[i][j].Clear();
    }

    grid.Clear();
}

void Renderer2D::RecreateMaterials()
{
    quadMaterial.Recreate();
    textMaterial.Recreate();
    gridMaterial.Recreate();
}

float Renderer2D::GetTotalTextWidth(const char* text,
                                    size_t textLength,
                                    SArray<Vulkan::Font::Glyph, 256>& glyphs)
{
    float texWidth = 0;

    for (size_t i = 0; i < textLength; i++) texWidth += glyphs[text[i]].advance >> 6;

    return texWidth;
}
} // namespace Siege