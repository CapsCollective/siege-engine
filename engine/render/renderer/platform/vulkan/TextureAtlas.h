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

class TextureAtlas
{
public:

    class TextureRef
    {
    public:

        // 'Structors
        TextureRef(TextureAtlas* parent, float minX, float minY, float width, float height) :
            parentAtlas {parent},
            minX {minX},
            width {width},
            minY {minY},
            height {height}
        {}

        // Operator Overloads

        Texture2D& operator*();

        TextureAtlas* operator->();

        operator bool() const;

        // Getters and setters

        inline float GetMinX() const
        {
            return minX;
        }
        inline float GetMinY() const
        {
            return minY;
        }
        inline float GetWidth() const
        {
            return width;
        }
        inline float GetHeight() const
        {
            return height;
        }

    private:

        // NOTE(Aryeh): Can we always guarantee that the atlas will exist?
        TextureAtlas* parentAtlas {nullptr};
        float minX, width, minY, height {0.f};
    };

    // 'Structors

    TextureAtlas() = default;
    TextureAtlas(const char* name,
                 const char* filePath,
                 Utils::Extent2DF imageExtents,
                 Utils::TextureFilter filter = Utils::TEXTURE_FILTER_LINEAR);
    TextureAtlas(TextureAtlas&& other);
    ~TextureAtlas();

    // Operator Overloads

    TextureRef operator[](size_t index);

    // Getters & Setters

    inline Texture2D& GetTexture()
    {
        return texture;
    }
    inline const Texture2D& GetTexture() const
    {
        return texture;
    }

private:

    // Private functions

    void Swap(TextureAtlas& other);

    // Private member variables

    Texture2D texture;
    Utils::Extent2DF fixedExtent {};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_TEXTURE_ATLAS_H
