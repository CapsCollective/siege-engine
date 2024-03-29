//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TEXTURE2D_H
#define SIEGE_ENGINE_TEXTURE2D_H

#include <utils/String.h>

#include "Image.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The Texture2D class represents a texture which is intended to wrap over a 2D primitive
 * (typically) a quad.This is the primary texture type used for 2D operations
 */
class Texture2D
{
public:

    // Enum dictating which texture colour channel will be used. Currently only supports full RGBA
    // and single unsigned integers (the red channel).
    enum Usage
    {
        TEX_USAGE_TEX2D = Utils::RGBASRGB,
        TEX_USAGE_FONT = Utils::RED8UN // Fonts only take data from the red channel
    };

    // Structs

    /**
     * Texture info which can be used for Vulkan-based drawing
     */
    struct Info
    {
        Image::Info imageInfo {};
        VkSampler sampler {nullptr};
        Usage usage {TEX_USAGE_TEX2D};
    };

    // 'Structors

    /**
     * A default empty constructor for the Texture2D class
     */
    Texture2D() = default;

    /**
     * A default constructor for the Texture2D. This will automatically set the texture to be the
     * default white texture
     * @param name the name of the texture being created
     */
    Texture2D(const char* name,
              Utils::TextureFilter filter = Utils::TEXTURE_FILTER_LINEAR,
              Usage texUsage = TEX_USAGE_TEX2D);

    /**
     * The texture constructor used for loading a texture from file. This loads data from a given
     * file path and stores pixel data in the class
     * @param name the name of the texture
     * @param filePath the path of the PNG or JPG file
     */
    Texture2D(const char* name,
              const char* filePath,
              Utils::TextureFilter filter = Utils::TEXTURE_FILTER_LINEAR);

    Texture2D(const char* name,
              const uint8_t* pixels,
              size_t size,
              uint32_t width,
              uint32_t height,
              Utils::TextureFilter filter = Utils::TEXTURE_FILTER_LINEAR,
              Usage texUsage = Usage::TEX_USAGE_TEX2D);

    /**
     * A move constructor for the Texture2D class
     * @param other the Texture2D to move
     */
    Texture2D(Texture2D&& other);

    /**
     * The destructor for the Texture2D class
     */
    ~Texture2D();

    // Operators

    /**
     * A move assignment operator
     * @param other the Texture2D to move
     * @return a reference to the new Texture2D
     */
    Texture2D& operator=(Texture2D&& other);

    // Functions

    void CopyToRegion(uint8_t* pixels,
                      unsigned long size,
                      Utils::Extent3D copyExtent,
                      Utils::Offset3D copyOffset);

    /**
     * Frees all memory held by the Texture2D
     */
    void Free();

    /**
     * Returns the ID of the Texture2D (based on the name of the Texture)
     * @return the Texture2D's ID
     */
    inline const Hash::StringId GetId() const
    {
        return id;
    }

    /**
     * Returns an info containing all info required by for rendering
     * @return a Texture2D::Info struct
     */
    inline const Info& GetInfo() const
    {
        return info;
    }

    inline const Utils::Extent2D& GetExtent() const
    {
        return extent;
    }

private:

    /**
     * Loads pixel information from a PNG or JPG file located on disk
     * @param filePath the path of the file
     */
    void LoadFromFile(const char* filePath);

    /**
     * Loads a texture from a pixel buffer. Useful if you already have pixel info loaded into a
     * buffer
     * @param pixels an array of pixel values (range from 0 to 255)
     * @param size the size of the pixel buffer
     * @param width the image width
     * @param height the image height
     */
    void LoadTexture(const uint8_t* pixels,
                     size_t size,
                     uint32_t width,
                     uint32_t height,
                     Usage texUsage);

    /**
     * Swaps the values of two Texture2Ds
     * @param other the Texture2D to swap values with
     */
    void Swap(Texture2D& other);

    Hash::StringId id {};
    Image image;
    Info info {};
    Utils::Extent2D extent;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_TEXTURE2D_H
