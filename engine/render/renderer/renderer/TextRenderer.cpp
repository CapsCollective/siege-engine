//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "TextRenderer.h"

#include <utils/math/Transform.h>

#include <cstdint>

#include "render/renderer/platform/vulkan/Swapchain.h"
#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{

void TextRenderer::Initialise(const String& globalDataAttributeName)
{
    using Vulkan::Material;
    using Vulkan::Shader;

    globalDataId = INTERN_STR(globalDataAttributeName);

    defaultTexture = Vulkan::Texture2D("default");

    defaultMaterial =
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/Text3D.vert.spv")
                     .WithVertexBinding(Shader::VertexBinding()
                                            .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                            .AddMat4Attribute()
                                            .AddFloatVec4Attribute()
                                            .AddFloatVec4Attribute()
                                            .AddFloatVec4Attribute())
                     .WithGlobalData3DUniform()
                     .WithPushConstant(sizeof(uint32_t))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/Text3D.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(&defaultTexture)
                     .Build());

    textureId = INTERN_STR("texture");

    auto size = (sizeof(QuadData) * MAX_FONTS * OFFSET_PER_FONT);

    uint32_t fontIndices[] = {0, 1, 3, 1, 2, 3};

    perFrameVertexBuffers = MHArray<Vulkan::VertexBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        perFrameVertexBuffers[i] = Vulkan::VertexBuffer(size);
    }

    indexBuffer = Vulkan::IndexBuffer(fontIndices, sizeof(unsigned int) * 6);
}

void TextRenderer::Free()
{
    defaultTexture.Free();
    defaultMaterial.Free();
    for (auto it = perFrameVertexBuffers.CreateIterator(); it; ++it) it->Free();
    indexBuffer.Free();
}

void TextRenderer::DrawFont(const char* text,
                            const Vec3& position,
                            const Vec3& rotation,
                            const Vec2 scale,
                            const IColour& colour,
                            Vulkan::Font* font)
{
    if (font == nullptr) return;

    auto texIndex = defaultMaterial.SetTexture(textureId, font->GetTexture());

    if (texIndex >= characters.Count()) characters.Append(MHArray<QuadData>(OFFSET_PER_FONT));

    auto& fontTexts = characters[texIndex];

    size_t textSize = strlen(text);
    float textScale = 64.f;

    float totalWidth = (GetTotalTextWidth(text, textSize, font->GetGlyphs()));

    float x = 0 - (totalWidth / 2.f);
    float y = 0;

    for (size_t i = 0; i < textSize; i++)
    {
        auto glyph = font->GetGlyph(text[i]);

        float height = glyph.height;

        float yOffset = (glyph.bearingY != height) * (glyph.bearingY - height);

        Vec4 coordinates = {x + glyph.bearingX, y - (height + yOffset), glyph.width, height};

        fontTexts.Append(
            {Transform3D(position, rotation, scale),
             {glyph.uvxMin, glyph.uvyMin, glyph.widthNormalised, glyph.heightNormalised},
             ToFColour(colour),
             coordinates / textScale});

        x += glyph.advance >> 6;
    }
}

void TextRenderer::RecreateMaterials()
{
    defaultMaterial.Recreate();
}

void TextRenderer::Render(Vulkan::CommandBuffer& buffer,
                          const uint64_t& globalDataSize,
                          const void* globalData,
                          uint32_t frameIndex)
{
    defaultMaterial.SetUniformData(globalDataId, globalDataSize, globalData);

    for (uint32_t i = 0; i < characters.Count(); i++)
    {
        if (characters[i].Count() == 0) continue;

        uint32_t texIndex = i;

        defaultMaterial.BindPushConstant(buffer, &texIndex);

        defaultMaterial.Bind(buffer);

        auto& vertexBuffer = perFrameVertexBuffers[frameIndex];

        uint64_t bindOffset = sizeof(QuadData) * (i * OFFSET_PER_FONT);

        vertexBuffer.Copy(characters[i].Data(),
                          sizeof(QuadData) * characters[i].Count(),
                          bindOffset);

        vertexBuffer.Bind(buffer, &bindOffset);
        indexBuffer.Bind(buffer);

        Vulkan::Utils::DrawIndexed(buffer.Get(), 6, characters[i].Count(), 0, 0, 0);
    }
}

void TextRenderer::Update()
{
    defaultMaterial.Update();
}

void TextRenderer::Flush()
{
    for (size_t i = 0; i < characters.Count(); i++) characters[i].Clear();
}

float TextRenderer::GetTotalTextWidth(const char* text,
                                      size_t textLength,
                                      SArray<Vulkan::Font::Glyph, 256>& glyphs)
{
    float texWidth = 0;

    for (size_t i = 0; i < textLength; i++) texWidth += glyphs[text[i]].advance >> 6;

    return texWidth;
}

} // namespace Siege