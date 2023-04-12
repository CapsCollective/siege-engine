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

#include "Context.h"
#include "render/renderer/statics/Statics.h"
#include "utils/Descriptor.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Font::Font(const char* filePath)
{
    auto device = Context::GetCurrentDevice();

    auto ft = Statics::GetFontLib();

    FT_Face fontFace;

    CC_ASSERT(!FT_New_Face(ft, filePath, 0, &fontFace), "Failed to load font!")

    uint8_t* buffer = new uint8_t[width * height];
    defer([buffer]() { delete[] buffer; });

    memset(buffer, 0, sizeof(uint8_t) * width * height);

    texture = Texture2D(filePath,
                        buffer,
                        sizeof(uint8_t) * width * height,
                        width,
                        height,
                        Texture2D::Usage::TEX_USAGE_FONT);

    VkSamplerCreateInfo samplerInfo =
        Utils::Descriptor::SamplerCreateInfo(VK_FILTER_LINEAR,
                                             VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);

    vkCreateSampler(device->GetDevice(), &samplerInfo, nullptr, &info.sampler);

    id = INTERN_STR(filePath);

    for (size_t i = 32; i < 126; i++)
    {
        AddChar(static_cast<unsigned char>(i), fontFace);
    }

    info.textureInfo = texture.GetInfo();
}

Font::~Font()
{
    Free();
}

void Font::Free()
{
    auto device = Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    vkDestroySampler(device, info.sampler, nullptr);

    texture.Free();

    info = {{}, nullptr};
}

void Font::Swap(Font& other)
{
    auto tmpId = id;
    auto tmpWidth = width;
    auto tmpHeight = height;
    auto tmpGlyphs = glyphs;
    auto tmpSpaceFilledX = spaceFilledX;
    auto tmpSpaceFilledY = spaceFilledY;
    auto tmpMaxHeight = maxHeight;
    auto tmpInfo = info;
    auto tmpTexture = std::move(texture);

    id = other.id;
    width = other.width;
    height = other.height;
    spaceFilledX = other.spaceFilledX;
    spaceFilledY = other.spaceFilledY;
    maxHeight = other.maxHeight;
    glyphs = other.glyphs;
    info = other.info;
    texture = std::move(other.texture);

    other.id = tmpId;
    other.width = tmpWidth;
    other.height = tmpHeight;
    other.spaceFilledX = tmpSpaceFilledX;
    other.spaceFilledY = tmpSpaceFilledY;
    other.maxHeight = tmpMaxHeight;
    other.glyphs = tmpGlyphs;
    other.info = tmpInfo;
    other.texture = std::move(tmpTexture);
}

void Font::AddChar(const unsigned char c, FontFace fontFace)
{
    FT_Set_Pixel_Sizes(fontFace, 0, 64);

    CC_ASSERT(!FT_Load_Char(fontFace, c, FT_LOAD_RENDER),
              String("Unable to load char: ") + String(c))

    auto glyph = fontFace->glyph;

    bool exceededWidth = (glyph->bitmap.width + spaceFilledX) >= width;
    bool exceededHeight = (glyph->bitmap.rows + spaceFilledY) >= height;

    int32_t positionXInAtlas = (spaceFilledX) * !exceededWidth;
    int32_t positionYInAtlas = spaceFilledY + static_cast<int32_t>(exceededWidth * maxHeight);

    unsigned long size = glyph->bitmap.width * glyph->bitmap.rows;

    glyphs[c] = {
        static_cast<float>(static_cast<float>(positionXInAtlas + 1) / static_cast<float>(width)),
        static_cast<float>(static_cast<float>(positionYInAtlas + 2) / static_cast<float>(height)),
        static_cast<float>(static_cast<float>(glyph->bitmap.width + positionXInAtlas + 1) /
                           static_cast<float>(width)),
        static_cast<float>(static_cast<float>(glyph->bitmap.rows + positionYInAtlas + 2) /
                           static_cast<float>(height)),
        static_cast<float>(glyph->bitmap.width),
        static_cast<float>(glyph->bitmap.rows),
        static_cast<float>(glyph->bitmap.width) / static_cast<float>(width),
        static_cast<float>(glyph->bitmap.rows) / static_cast<float>(height),
        static_cast<float>(glyph->bitmap_left),
        static_cast<float>(glyph->bitmap_top),
        static_cast<unsigned int>(glyph->advance.x)};

    if (size == 0) return;

    Utils::Extent3D imageExtent {static_cast<uint32_t>(glyph->bitmap.width),
                                 static_cast<uint32_t>(glyph->bitmap.rows),
                                 1};

    texture.CopyToRegion(glyph->bitmap.buffer,
                         size,
                         imageExtent,
                         {positionXInAtlas + 1, positionYInAtlas + 2});

    CC_ASSERT(!exceededWidth || !exceededHeight, "Cannot add any more images to the atlas!")

    maxHeight = ((glyph->bitmap.rows > maxHeight) * glyph->bitmap.rows) +
                ((glyph->bitmap.rows <= maxHeight) * maxHeight);

    spaceFilledX = ((!exceededWidth * (glyph->bitmap.width + spaceFilledX)) +
                    (exceededWidth * glyph->bitmap.width)) +
                   1;
    spaceFilledY += (exceededWidth * (maxHeight + 2));
}

} // namespace Siege::Vulkan