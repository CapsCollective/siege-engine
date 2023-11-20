//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_TEXTURE_ATLAS_H
#define SIEGE_ENGINE_VULKAN_TEXTURE_ATLAS_H

#include "Texture2D.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

/**
 * A vulkan-based texture atlas class. This class contains a single texture made out of many
 * sub-textures. Texture Atlases are a way to store multiple textures in a single space within the
 * same memory space. Currently, the TextureAtlas supports fixed size sub-textures
 *
 * NOTE: Currently only supports fixed size texture atlases
 *
 * TODO(Aryeh): Add the ability to add sub-textures from png files
 * TODO(Aryeh): Add the ability to manually specify the sub-texture extents
 */
class TextureAtlas
{
public:

    /**
     * A subTextureRef is a reference to a sub-texture. These are textures which are contained
     * within a texture atlas. These can be accessed from the TextureAtlas via indexing
     */
    class SubTextureRef
    {
    public:

        // 'Structors

        /**
         * The SubTextureRef constructor
         * @param parent the parent texture atlas this sub-texture belongs to
         * @param minX the leftmost normalised x coordinate within the parent texture
         * @param minY the top-most normalised y coordinate within the parent texture
         * @param width the width of the texture
         * @param height the height of the texture
         */
        inline SubTextureRef(TextureAtlas* parent,
                             float minX,
                             float minY,
                             float width,
                             float height) :
            parentAtlas {parent},
            minX {minX},
            width {width},
            minY {minY},
            height {height}
        {}

        /**
         * The SubTextureRef move constructor
         * @param other the SubTextureRef to move information from
         */
        inline SubTextureRef(SubTextureRef&& other)
        {
            Swap(other);
        }

        /**
         * The SubTextureRef copy constructor
         * @param other the SubTextureRef to copy information from
         */
        inline SubTextureRef(SubTextureRef& other) :
            parentAtlas {other.parentAtlas},
            minX {other.minX},
            width {other.width},
            minY {other.minY},
            height {other.height}
        {}

        // Operator Overloads

        /**
         * The SubTextureRef copy assignment operator
         * @param other the SubTextureRef to copy information from
         * @return a reference to the SubTextureRef
         */
        SubTextureRef& operator=(SubTextureRef& other);

        /**
         * The SubTextureRef move assignment operator
         * @param other the SubTextureRef to move information from
         * @return a reference to the SubTextureRef
         */
        inline SubTextureRef& operator=(SubTextureRef&& other)
        {
            Swap(other);
            return *this;
        }

        /**
         * The dereference operator
         * @return returns a reference to the stored Texture2D within the parent texture
         */
        Texture2D& operator*();

        /**
         * The pointer arrow operator
         * @return returns the pointer to the parentTexture for member access
         */
        TextureAtlas* operator->();

        /**
         * A boolean operator for the SubTextureRef
         * @return A boolean specifying if the parentAtlas exists (is not a nullptr)
         */
        operator bool() const;

        // Getters and setters

        /**
         * Returns the leftmost coordinate of the sub-texture
         * @return a float specifying the leftmost extent of the sub-texture
         */
        inline float GetMinX() const
        {
            return minX;
        }
        /**
         * Returns the topmost coordinate of the sub-texture
         * @return a float specifying the topmost coordinate of the sub-texture
         */
        inline float GetMinY() const
        {
            return minY;
        }
        /**
         * Returns the width of the sub-texture
         * @return the width of the sub-texture
         */
        inline float GetWidth() const
        {
            return width;
        }
        /**
         * Returns the height of the sub-texture
         * @return the height of the sub-texture
         */
        inline float GetHeight() const
        {
            return height;
        }

    private:

        // Private methods

        /**
         * Swaps the contents of two SubTextureRefs
         * @param other the SubTextureRef to swap contents with
         */
        void Swap(SubTextureRef& other);

        // Private member variables

        TextureAtlas* parentAtlas {nullptr};
        float minX, width, minY, height {0.f};
    };

    // 'Structors

    /**
     * The default TextureAtlas constructor (empty constructor)
     */
    TextureAtlas() = default;

    /**
     * The file upload TextureAtlas constructor. Loads texture data from disk and stores it in the
     * memory
     * @param name the name of the texture
     * @param filePath the path to the texture file
     * @param imageExtents the width and height of the texture
     * @param filter the scaling filter used for the texture
     */
    TextureAtlas(const char* name,
                 const char* filePath,
                 Utils::Extent2DF imageExtents,
                 Utils::TextureFilter filter = Utils::TEXTURE_FILTER_LINEAR);

    /**
     * The TextureAtlas move constructor. Moves the contents of one TextureAtlas to another
     * @param other the TextureAtlas to move information from
     */
    TextureAtlas(TextureAtlas&& other);

    /**
     * The TextureAtlas Destructor. Destroys all memory stored by the TextureAtlas
     */
    ~TextureAtlas();

    // Operator Overloads

    /**
     * The TextureAtlas subscript operator. Indexes into a texture atlas and returns the SubTexture
     * stored in that position.
     * NOTE: So far TextureAtlases only support fixed-size textures
     * @param index the index of the SubTexture
     * @return the SubTextureRef stored in the specified location
     */
    SubTextureRef operator[](size_t index);

    /**
     * The TextureAtlas move assignment operator
     * @param other the TextureAtlas to move information from
     * @return a reference to the TextureAtlas
     */
    TextureAtlas& operator=(TextureAtlas&& other);

    // Getters & Setters

    // TODO(Aryeh): Need a way to add new textures to the Atlas in future
    // TODO(Aryeh): Need a way to manually specify each texture coordinate when not using fixed
    // textures

    /**
     * Gets the Texture2D stored within the Atlas
     * @return the atlas' texture
     */
    inline Texture2D& GetTexture()
    {
        return texture;
    }

private:

    // Private functions

    /**
     * Swaps the contents of two TextureAtlases
     * @param other the TextureAtlas to swap contents with
     */
    void Swap(TextureAtlas& other);

    // Private member variables

    Texture2D texture;
    Utils::Extent2DF fixedExtent {};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_TEXTURE_ATLAS_H
