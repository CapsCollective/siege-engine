//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FONT_H
#define SIEGE_ENGINE_FONT_H

#include "Texture2D.h"

#include <freetype/freetype.h>
#include <utils/Hash.h>
#include <utils/collections/StackArray.h>

namespace Siege::Vulkan
{

class Font
{
public:
    // Constexpr declarations
    static constexpr uint32_t INITIAL_FONT_ATLAS_WIDTH {512};
    static constexpr uint32_t INITIAL_FONT_ATLAS_HEIGHT {512};

    struct Info
    {
        Texture2D::Info textureInfo {};
        VkSampler sampler {nullptr};
    };

    // Structs
    struct Glyph
    {
        float uvxMin {0};
        float uvyMin {0};
        float uvxMax {0};
        float uvyMax {0};
        float width {0};
        float height {0};
        float bearingX {0};
        float bearingY {0};
        unsigned int advance {0};
    };

    // 'Structors

    Font() = default;
    Font(const char* filePath);
    inline Font(Font&& other) { Swap(other); }
    ~Font();

    // Operators

    inline Font& operator=(Font&& other)
    {
        Swap(other);
        return *this;
    }

    // Public methods

    void Free();

    static void InitialiseFontLibs();
    static void DestroyFontLibs();

    inline const Texture2D* GetTexture() const { return &texture; }
    inline Texture2D* GetTexture() { return &texture; }
    const inline Glyph& GetGlyph(const unsigned char c)
    {
        return glyphs[c];
    }
    inline SArray<Glyph, 256>& GetGlyphs() { return glyphs; }

private:
    void Swap(Font& other);
    void AddChar(const unsigned char c);

    static FT_Library freetype;
    FT_Face fontFace {nullptr};

    uint32_t width {INITIAL_FONT_ATLAS_WIDTH};
    uint32_t height {INITIAL_FONT_ATLAS_HEIGHT};

    SArray<Glyph, 256> glyphs;

    int32_t spaceFilledX {0};
    int32_t spaceFilledY {0};

    uint32_t maxHeight {0};

    Hash::StringId id {0};
    Texture2D texture;
    Info info {};
};

} // namespace Siege

#endif // SIEGE_ENGINE_FONT_H
