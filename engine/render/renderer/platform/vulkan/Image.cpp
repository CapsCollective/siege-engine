//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Image.h"

namespace Siege::Vulkan
{
Image::Config& Image::Config::WithFlag(const uint32_t& flag)
{
    createFlag |= flag;
    return *this;
}

Image::Config& Image::Config::WithType(const uint32_t& typeFlag)
{
    imageType |= typeFlag;
    return *this;
}

Image::Config& Image::Config::WithFormat(const uint32_t& formatFlag)
{
    format = formatFlag;
    return *this;
}

Image::Config& Image::Config::WithExtent(const uint32_t& width,
                                         const uint32_t& height,
                                         const uint32_t& depth)
{
    extent = {width, height, depth};
    return *this;
}

Image::Config& Image::Config::WithMipLevels(const uint32_t& mipLevel)
{
    mipLevels = mipLevel;
    return *this;
}

Image::Config& Image::Config::WithArrayLayers(const uint32_t& arrayLayerCount)
{
    arrayLayers = arrayLayerCount;
    return *this;
}

Image::Config& Image::Config::WithSampleFlags(const uint32_t& sampleFlags)
{
    samples |= sampleFlags;
    return *this;
}

Image::Config& Image::Config::WithImageTiling(const uint32_t& imageTiling)
{
    tiling = imageTiling;
    return *this;
}

Image::Config& Image::Config::WithImageUsage(const uint32_t& imageUsage)
{
    usage = imageUsage;
    return *this;
}

Image::Config& Image::Config::WithSharingMode(const uint32_t& mode)
{
    sharingMode = mode;
    return *this;
}

Image::Config& Image::Config::WithQueueFamilies(const uint32_t* indices, uint32_t count)
{
    pQueueFamilyIndices = indices;
    queueFamilyIndexCount = count;
    return *this;
}

Image::Config& Image::Config::WithInitialLayout(const uint32_t& newInitialLayout)
{
    initialLayout = newInitialLayout;
    return *this;
}

Image::Image(const Config& config) {}
} // namespace Siege::Vulkan