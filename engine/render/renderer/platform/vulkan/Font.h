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

    // Structs

    /**
     * Stores information about the texture atlas held by the font. This is used for rendering
     */
    struct Info
    {
        Texture2D::Info textureInfo {};
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
        float width {0};
        float height {0};
        float widthNormalised {0};
        float heightNormalised {0};
        float bearingX {0};
        float bearingY {0};
        signed long advance {0};
        unsigned long bufferOffset {0};
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

    // Constexpr declarations
    static constexpr uint32_t MAX_ATLAS_WIDTH {512};
    static constexpr uint32_t MAX_ATLAS_HEIGHT {1024};

    /**
     * Swaps the values of two Fonts
     * @param other the Font to swap values with
     */
    void Swap(Font& other);

    /**
     * Populates the font with all glyphs stored in the ttf file
     * @param fontFace the font face used to render the characters
     * @param padding padding between the characters in the font
     * @param buffer a buffer of pixels to copy data from the font face into
     * @return the width and height of the texture
     */
    Utils::Extent2D PopulateGlyphs(FontFace fontFace, Utils::Offset2D padding, uint8_t* buffer);

    /**
     * Populates the texture atlas with the glyphs collected from PopulateGlyphs
     * @param buffer the pixel buffer storing our glyph pixels
     */
    void PopulateTextureAtlas(uint8_t* buffer);

    Utils::Extent2D extent {};

    SArray<Glyph, 256> glyphs;

    Hash::StringId id {0};
    Texture2D texture;
    Info info {};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_FONT_H
