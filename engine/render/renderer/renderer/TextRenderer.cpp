//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "TextRenderer.h"
#include <utils/math/Graphics.h>

namespace Siege
{

void TextRenderer::Initialise(const char* defaultTextPath, const String& globalDataAttributeName, const uint64_t& globalDataSize)
{
    using Vulkan::Mesh;
    using Vulkan::Material;
    using Vulkan::Shader;

    globalDataId = INTERN_STR(globalDataAttributeName);

    defaultFont = Vulkan::Font(defaultTextPath);

    defaultMaterial =
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/text2D.vert.spv")
                     .WithVertexBinding(
                         Shader::VertexBinding()
                                .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                .AddMat4Attribute()
                                .AddFloatVec4Attribute()
                                .AddFloatVec4Attribute()
                                .AddFloatVec4Attribute())
                     .WithGlobalData3DUniform()
                     .WithPushConstant(sizeof(FontPushConstant))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/text2D.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(defaultFont.GetTexture())
                     .Build());

    auto size = (sizeof(PerInstanceData) * MAX_FONTS * MAX_TEXT_OBJS_PER_FONT * MAX_CHARS_PER_TEXT);

    uint32_t fontIndices[] = {0, 1, 3, 1, 2, 3};

    vertexBuffer = Vulkan::VertexBuffer(size);
    indexBuffer = Vulkan::IndexBuffer(6, fontIndices);
}

void TextRenderer::DestroyTextRenderer()
{
    defaultFont.Free();
    defaultMaterial.Free();
}

void TextRenderer::DrawFont(const char* text, const Vec3& position, const Vec2 scale, const IColour& colour,  Vulkan::Font* font)
{

    auto targetFont = font == nullptr ? &defaultFont : font;

    auto texIndex = defaultMaterial.SetTexture(INTERN_STR("texture"), targetFont->GetTexture());

    if (texIndex >= instanceData.Count())
    {
        instanceData.Append({});
    }

    auto& fontTexts = instanceData[texIndex];

    size_t textSize = strlen(text);
    float textScale = 64.f;

    float totalWidth = (GetTotalTextWidth(text, textSize, targetFont->GetGlyphs(), scale)/textScale);

    float x = 0 - (totalWidth / 2.f);
    float y = 0;

    for (size_t i = 0; i < textSize; i++)
    {
        auto glyph = targetFont->GetGlyph(text[i]);

        float width = (glyph.width/textScale);
        float height = (glyph.height/textScale);

        float yOffset = ((glyph.bearingY != glyph.height) * ((glyph.bearingY - glyph.height)/textScale));

        float xPos = x + glyph.bearingX/textScale;
        float yPos = y - (height + yOffset);

        x += glyph.bearingX/textScale;

        fontTexts.Append({Graphics::CalculateTransform3D({position.x, position.y, position.z},
                                                         {0.f, 0.f, 0.f},
                                                         {scale.x, scale.y, 0.f}),
                          {glyph.uvxMin, glyph.uvyMin, glyph.widthNormalised, glyph.heightNormalised},
                          ToFColour(colour),
                          {xPos, yPos , width, height}});

        x += ((glyph.advance >> 6)/textScale);
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

    for(uint32_t i = 0; i < instanceData.Count(); i++)
    {
        FontPushConstant push = {i};

        defaultMaterial.BindPushConstant(buffer, &push);

        defaultMaterial.Bind(buffer);

        uint64_t fontOffset = (i * MAX_CHARS_PER_TEXT * MAX_TEXT_OBJS_PER_FONT);

        vertexBuffer.Update(sizeof(PerInstanceData), instanceData[i].Data(), instanceData[i].Count(), fontOffset);

        uint64_t offsets[] = {fontOffset};
        vertexBuffer.Bind(buffer, offsets, 1);
        indexBuffer.Bind(buffer);

        vkCmdDrawIndexed(buffer.Get(), 6, instanceData[i].Count(), 0, 0, 0);
    }
}

void TextRenderer::Update()
{
    defaultMaterial.Update();
}

void TextRenderer::Flush()
{
    instanceData.Clear();
}

float TextRenderer::GetTotalTextWidth(const char* text, size_t textLength, SArray<Vulkan::Font::Glyph, 256>& glyphs, Vec2 scale)
{
    float texWidth = 0;

    for (size_t i = 0; i < textLength; i++)
    {
        auto glyph = glyphs[text[i]];
        texWidth += glyph.bearingX + (glyph.advance >> 6);
    }

    return texWidth;
}

} // namespace Siege