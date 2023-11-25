//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "TextureAtlas.h"

namespace Siege::Vulkan
{

Texture2D& TextureAtlas::SubTextureRef::operator*()
{
    return parentAtlas->texture;
}

TextureAtlas* TextureAtlas::SubTextureRef::operator->()
{
    return parentAtlas;
}

TextureAtlas::SubTextureRef::operator bool() const
{
    return parentAtlas;
}

void TextureAtlas::SubTextureRef::Swap(SubTextureRef& other)
{
    auto tmpAtlas = parentAtlas;
    auto tmpMinX = minX;
    auto tmpMinY = minY;
    auto tmpWidth = width;
    auto tmpHeight = height;

    parentAtlas = other.parentAtlas;
    minX = other.minX;
    minY = other.minY;
    width = other.width;
    height = other.height;

    other.parentAtlas = tmpAtlas;
    other.minX = tmpMinX;
    other.minY = tmpMinY;
    other.width = tmpWidth;
    other.height = tmpHeight;
}
TextureAtlas::SubTextureRef& TextureAtlas::SubTextureRef::operator=(
    TextureAtlas::SubTextureRef& other)
{
    parentAtlas = other.parentAtlas;
    minX = other.minX;
    minY = other.minY;
    width = other.width;
    height = other.height;

    return *this;
}

TextureAtlas::TextureAtlas(const char* name,
                           const char* filePath,
                           Utils::Extent2DF imageExtents,
                           Utils::TextureFilter filter) :
    fixedExtent {imageExtents}
{
    texture = Texture2D(name, filePath, filter);
}

TextureAtlas::TextureAtlas(TextureAtlas&& other)
{
    Swap(other);
}

TextureAtlas::~TextureAtlas()
{
    fixedExtent = {};
}

TextureAtlas::SubTextureRef TextureAtlas::operator[](size_t index)
{
    // TODO(Aryeh): Add some level of error handling here (assert if index is higher than number of
    //  textures)

    // NOTE(Aryeh): only works for fixed size textures
    size_t elementsInRow = 1 / fixedExtent.width;

    return SubTextureRef(this,
                         (index % elementsInRow) * fixedExtent.width, // potentially slow code
                         (index / elementsInRow) * fixedExtent.height,
                         fixedExtent.width,
                         fixedExtent.height);
}

TextureAtlas& TextureAtlas::operator=(TextureAtlas&& other)
{
    Swap(other);
    return *this;
}

void TextureAtlas::Swap(TextureAtlas& other)
{
    auto tmpTexture = std::move(texture);
    auto tmpFixedExtent = fixedExtent;

    texture = std::move(other.texture);
    fixedExtent = other.fixedExtent;

    other.texture = std::move(tmpTexture);
    other.fixedExtent = tmpFixedExtent;
}

} // namespace Siege::Vulkan