//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TEXTURE2D_H
#define SIEGE_ENGINE_TEXTURE2D_H

#include "Image.h"
#include "utils/Types.h"

#include <utils/String.h>

namespace Siege::Vulkan
{
class Texture2D
{
public:

    // Structs

    struct Info
    {
        Image::Info imageInfo {};
        VkSampler sampler {nullptr};
    };

    // 'Structors

    Texture2D() = default;
    Texture2D(const String& name, const String& filePath);
    Texture2D(Texture2D&& other);
    ~Texture2D();

    // Operators

    Texture2D& operator=(Texture2D&& other);

    // Functions

    inline const Hash::StringId GetId() const
    {
        return id;
    }
    inline const Info& GetInfo() const
    {
        return info;
    }
private:
    void Swap(Texture2D& other);
    Hash::StringId id {};
    Image image;
    Info info {};
};
} // namespace Siege

#endif // SIEGE_ENGINE_TEXTURE2D_H
