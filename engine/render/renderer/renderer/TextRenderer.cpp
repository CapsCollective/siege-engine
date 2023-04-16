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
                             .AddFloatVec3Attribute()
                             .AddFloatVec4Attribute()
                             .AddFloatVec2Attribute())
                     .WithGlobalData3DUniform()
                     .WithPushConstant(sizeof(FontPushConstant))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/text2D.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(defaultFont.GetTexture())
                     .Build());

    model.SetMesh(Mesh({sizeof(FontVertex), nullptr, 0}, {nullptr, 0}));

    model.SetMaterial(&defaultMaterial);
}

void TextRenderer::DestroyTextRenderer()
{
    defaultFont.Free();
    defaultMaterial.Free();
    model.DestroyModel();
}

void TextRenderer::DrawFont(const char* text, const Vec3& position, const Vec2 scale, const IColour& colour,  Vulkan::Font* font)
{

    auto targetFont = font == nullptr ? &defaultFont : font;

    auto texIndex = defaultMaterial.SetTexture(INTERN_STR("texture"), targetFont->GetTexture());

    if (texIndex >= perFontVertices.Count())
    {
        perFontVertices.Append({});
        perFontIndices.Append({});
        pushConstants.Append({texIndex});
    }

    auto& fontVertices = perFontVertices[texIndex];
    auto& fontIndices = perFontIndices[texIndex];

    float textSize = strlen(text);
    float textScale = 64.f;

    float totalWidth = (GetTotalTextWidth(text, textSize, targetFont->GetGlyphs(), scale)/textScale);

    float x = position.x - (totalWidth / 2.f) * scale.x;
    float y = position.y;

    for (size_t i = 0; i < textSize; i++)
    {
        auto glyph = targetFont->GetGlyph(text[i]);

        float width = (glyph.width/textScale) * scale.x;
        float height = (glyph.height/textScale) * scale.y;

        float yOffset = ((glyph.bearingY != glyph.height) * ((glyph.bearingY - glyph.height/textScale)));

        float xPos = x + glyph.bearingX/textScale * scale.x;
        float yPos = y - (height + yOffset);

        fontVertices.Append({{xPos + width, yPos + height, position.z}, ToFColour(colour), {glyph.uvxMax, glyph.uvyMax}});
        fontVertices.Append({{xPos + width, yPos, position.z}, ToFColour(colour), {glyph.uvxMax, glyph.uvyMin}});
        fontVertices.Append({{xPos, yPos, position.z}, ToFColour(colour), {glyph.uvxMin, glyph.uvyMin}});
        fontVertices.Append({{xPos, yPos + height, position.z}, ToFColour(colour), {glyph.uvxMin, glyph.uvyMax}});

        fontIndices.Append(fontVertices.Count() - 4);
        fontIndices.Append(fontVertices.Count() - 3);
        fontIndices.Append(fontVertices.Count() - 1);

        fontIndices.Append(fontVertices.Count() - 3);
        fontIndices.Append(fontVertices.Count() - 2);
        fontIndices.Append(fontVertices.Count() - 1);

        x += ((glyph.advance >> 6)/textScale) * scale.x;
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
    if (pushConstants.Count() == 0) return;

    for(size_t i = 0 ; i < perFontVertices.Count(); i++)
    {
        auto& fontVertices = perFontVertices[i];
        auto& fontIndices = perFontIndices[i];

        defaultMaterial.SetUniformData(globalDataId, globalDataSize, globalData);

        defaultMaterial.BindPushConstant(buffer, &pushConstants[i]);

        defaultMaterial.Bind(buffer);

        model.UpdateMeshIndexed(frameIndex,
                                {sizeof(FontVertex), fontVertices.Data(), static_cast<uint32_t>(fontVertices.Count())},
                                {fontIndices.Data(), static_cast<uint32_t>(fontIndices.Count())});

        model.BindIndexed(buffer, frameIndex);
        model.DrawIndexed(buffer, frameIndex);
    }
}

void TextRenderer::Update()
{
    defaultMaterial.Update();
}

void TextRenderer::Flush()
{
    pushConstants.Clear();
    vertices.Clear();
    indices.Clear();
    for (auto it = perFontIndices.CreateIterator(); it; ++it) it->Clear();
    for (auto it = perFontVertices.CreateIterator(); it; ++it) it->Clear();
    perFontVertices.Clear();
    perFontIndices.Clear();
}

float TextRenderer::GetTotalTextWidth(const char* text, size_t textLength, SArray<Vulkan::Font::Glyph, 256>& glyphs, Vec2 scale)
{
    float texWidth = 0;

    for (size_t i = 0; i < textLength; i++)
    {
        auto glyph = glyphs[text[i]];
        texWidth += glyph.advance >> 6;
    }

    return texWidth;
}

} // namespace Siege