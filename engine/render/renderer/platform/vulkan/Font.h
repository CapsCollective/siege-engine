//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FONT_H
#define SIEGE_ENGINE_FONT_H

#include <utils/Hash.h>
#include <utils/collections/StackArray.h>

#include "Texture2D.h"

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FontFace;

namespace Siege::Vulkan
{

/**
 * The Font class is responsible for loading in fonts from disk. Fonts are loaded into a texture
 * atlas with every character represented as a glyph. The Font class controls how these glyphs are
 * loaded in and stored
 */
class Font
{
public:

    // Constexpr declarations
    static constexpr uint32_t INITIAL_FONT_ATLAS_WIDTH {512};
    static constexpr uint32_t INITIAL_FONT_ATLAS_HEIGHT {512};

    // Structs

    /**
     * Stores information about the texture atlas held by the font. This is used for rendering
     */
    struct Info
    {
        Texture2D::Info textureInfo {};
        VkSampler sampler {nullptr};
    };

    /**
     * A glyph is a single character (currently only ASCII characters). Each character takes up a
     * specific region of space in the texture atlas. The glyph struct stores information such as
     * the glyph's uv coordinates and dimensions
     */
    struct Glyph
    {
        float uvxMin {0};
        float uvyMin {0};
        float uvxMax {0};
        float uvyMax {0};
        float width {0};
        float height {0};
        float widthNormalised {0};
        float heightNormalised {0};
        float bearingX {0};
        float bearingY {0};
        unsigned int advance {0};
    };

    // 'Structors

    /**
     * A default constructor for the Font class
     */
    Font() = default;

    /**
     * A base constructor. This constructor loads in a font from a specific location on disk
     * @param filePath the file path to load the font from
     */
    Font(const char* filePath);

    /**
     * Swaps the contents of two Font objects
     * @param other the Font object to swap values with
     */
    inline Font(Font&& other)
    {
        Swap(other);
    }

    /**
     * The Font destructor. Frees all memory held by the Font class
     */
    ~Font();

    // Operators

    /**
     * The Font move assignment operator. Creates a new Font and moves it's contents into the
     * current Font
     * @param other the Font to move data into
     * @return a reference to the current Font object
     */
    inline Font& operator=(Font&& other)
    {
        Swap(other);
        return *this;
    }

    // Public methods

    /**
     * Frees the memory held by the Font
     */
    void Free();

    // Getters

    /**
     * Returns a const pointer to the texture atlas stored by the font
     * @return a pointer to the texture atlas
     */
    inline const Texture2D* GetTexture() const
    {
        return &texture;
    }

    /**
     * Returns a pointer to the texture atlas stored by the font
     * @return a pointer to the texture atlas
     */
    inline Texture2D* GetTexture()
    {
        return &texture;
    }

    /**
     * Returns a glyph corresponding to the provided character
     * @param c the character to search for
     * @return the glyph which corresponds to the character
     */
    const inline Glyph& GetGlyph(const unsigned char c)
    {
        return glyphs[c];
    }

    /**
     * Returns the list of glyphs stored by the font
     * @return an SArray which holds all the font's glyphs
     */
    inline SArray<Glyph, 256>& GetGlyphs()
    {
        return glyphs;
    }

private:

    /**
     * Swaps the values of two Fonts
     * @param other the Font to swap values with
     */
    void Swap(Font& other);

    /**
     * Adds a character glyph to the font atlas
     * @param c the character to add
     */
    void AddChar(const unsigned char c, FontFace fontFace);

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

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_FONT_H
