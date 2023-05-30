//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Font.h"

#include <freetype/freetype.h>
#include <utils/Defer.h>
#include <utils/Logging.h>

#include "render/renderer/statics/Statics.h"
#include "utils/Descriptor.h"
#include "utils/TypeAdaptor.h"

#define CAST_F(val) static_cast<float>(val)

namespace Siege::Vulkan
{
Font::Font(const char* filePath)
{
    FT_Face fontFace;

    CC_ASSERT(!FT_New_Face(Statics::GetFontLib(), filePath, 0, &fontFace), "Failed to load font!")

    FT_Set_Pixel_Sizes(fontFace, 0, 64);

    Utils::Offset2D textPadding = {1, 2};

    uint8_t* buffer = new uint8_t[MAX_ATLAS_WIDTH * MAX_ATLAS_HEIGHT];
    defer([buffer]() { delete[] buffer; });
    memset(buffer, 0, sizeof(uint8_t) * MAX_ATLAS_WIDTH * MAX_ATLAS_HEIGHT);

    extent = PopulateGlyphs(fontFace, textPadding, buffer);

    texture = Texture2D(filePath,
                        buffer,
                        sizeof(uint8_t) * extent.width * extent.height,
                        extent.width,
                        extent.height,
                        Texture2D::Usage::TEX_USAGE_FONT);

    PopulateTextureAtlas(buffer);

    id = INTERN_STR(filePath);

    info.textureInfo = texture.GetInfo();
}

Font::~Font()
{
    Free();
}

void Font::Free()
{
    texture.Free();

    info = {{}};
}

void Font::Swap(Font& other)
{
    auto tmpId = id;
    auto tmpAtlasExtent = extent;
    auto tmpGlyphs = glyphs;
    auto tmpInfo = info;
    auto tmpTexture = std::move(texture);

    id = other.id;
    extent = other.extent;
    glyphs = other.glyphs;
    info = other.info;
    texture = std::move(other.texture);

    other.id = tmpId;
    other.extent = tmpAtlasExtent;
    other.glyphs = tmpGlyphs;
    other.info = tmpInfo;
    other.texture = std::move(tmpTexture);
}

Utils::Extent2DF Font::PopulateGlyphs(FontFace fontFace, Utils::Offset2D padding, uint8_t* buffer)
{
    float x {0.f}, y {0.f}, rowHeight {0.f}, maxX {0.f}, maxY {0.f};

    uint32_t offset {0};

    for (unsigned char c = 32; c < 126; c++)
    {
        auto message = String("Unable to load char: ") + String(c);
        CC_ASSERT(!FT_Load_Char(fontFace, c, FT_LOAD_RENDER), message.Str())

        auto glyph = fontFace->glyph;

        float width {CAST_F(glyph->bitmap.width)}, height {CAST_F(glyph->bitmap.rows)};

        bool exceededWidth = (width + x) >= MAX_ATLAS_WIDTH;

        float atlasX = (x * !exceededWidth);
        float atlasY = (y + (exceededWidth * rowHeight));

        glyphs[c] = {atlasX,
                     atlasY,
                     width,
                     height,
                     width,
                     height,
                     CAST_F(glyph->bitmap_left),
                     CAST_F(glyph->bitmap_top),
                     glyph->advance.x,
                     offset};

        rowHeight =
            ((height > rowHeight) * height + (height <= rowHeight) * rowHeight) + padding.height;

        x = ((!exceededWidth * (width + x)) + (exceededWidth * width)) + glyph->bitmap_left +
            padding.width;
        y += (exceededWidth * (rowHeight)) + glyph->bitmap_top + padding.height;

        maxX = ((x > maxX) * x + (x <= maxX) * maxX);
        maxY = ((y > maxY) * (y + rowHeight) + (y <= maxY) * maxY);

        auto size = sizeof(uint8_t) * width * height;

        x += (glyph->advance.x >> 6) + padding.width;

        memcpy(buffer + offset, glyph->bitmap.buffer, size);
        offset += size;
    }

    return {maxX + padding.width, maxY + padding.height};
}

void Font::PopulateTextureAtlas(uint8_t* buffer)
{
    for (unsigned char c = 32; c < 126; c++)
    {
        auto& glyph = glyphs[c];
        float xPos {glyph.uvxMin}, yPos {glyph.uvyMin};

        // TODO:  Add an extent type which uses floating point numbers

        glyph.uvxMin /= extent.width;
        glyph.uvyMin /= extent.height;

        glyph.widthNormalised /= extent.width;
        glyph.heightNormalised /= extent.height;

        Utils::Extent3D imageExtent {(uint32_t) (glyph.width), (uint32_t) (glyph.height), 1};

        size_t size = sizeof(uint8_t) * (glyph.width * glyph.height);
        if (size == 0) continue;

        texture.CopyToRegion(buffer + glyph.bufferOffset,
                             size,
                             imageExtent,
                             {int32_t(xPos), int32_t(yPos)});
    }
}
} // namespace Siege::Vulkan