//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_IMAGE_H
#define SIEGE_ENGINE_VULKAN_IMAGE_H

#include <cstdint>

#include "utils/Types.h"

namespace Siege::Vulkan
{
class Image
{
public:
    struct Config
    {
        Utils::ImageFormat imageFormat;
        Utils::Extent3D imageExtent;
        Utils::ImageUsage usage;
        uint32_t mipLevels;
        uint32_t layers;
    };

    Image() = default;
    Image(const Config& config);
    Image(Image&& other);
    ~Image();

    Image& operator=(Image&& other);

    VkImage GetImage() { return image; }
    VkDeviceMemory GetMemory() { return memory; }
    VkImageView GetView() { return imageView; }

    bool IsValid();

private:
    void Move(Image& other);
    bool IsDepthFormat(Utils::ImageFormat);

    VkImage image {nullptr};
    VkDeviceMemory memory {nullptr};
    VkImageView imageView {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_IMAGE_H
