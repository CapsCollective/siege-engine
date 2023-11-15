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
Texture2D& TextureAtlas::TextureRef::operator*()
{
    return parentAtlas.texture;
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

TextureAtlas::~TextureAtlas() {}

void TextureAtlas::Swap(TextureAtlas& other)
{
    auto tmpTexture = std::move(texture);
    auto tmpFixedExtent = fixedExtent;

    texture = std::move(other.texture);
    fixedExtent = other.fixedExtent;

    other.texture = std::move(tmpTexture);
    other.fixedExtent = tmpFixedExtent;
}

TextureAtlas::TextureRef TextureAtlas::operator[](size_t index)
{
    size_t elementsInRow = 1 / fixedExtent.width;

    return TextureRef(*this,
                      (index % elementsInRow) * fixedExtent.width, // potentially slow code
                      (index / elementsInRow) * fixedExtent.height,
                      fixedExtent.width,
                      fixedExtent.height);
}

} // namespace Siege::Vulkan