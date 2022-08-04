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

#define TO_VK_CREATE_INFO(src)                                                                 \
    {                                                                                          \
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, src.next,                                         \
            static_cast<VkImageCreateFlags>(src.createFlag),                                   \
            static_cast<VkImageType>(src.imageType), static_cast<VkFormat>(src.format),        \
            {src.extent.width, src.extent.height, src.extent.depth}, src.mipLevels,            \
            src.arrayLayers, static_cast<VkSampleCountFlagBits>(src.samples),                  \
            static_cast<VkImageTiling>(src.tiling), static_cast<VkImageUsageFlags>(src.usage), \
            static_cast<VkSharingMode>(src.sharingMode), src.queueFamilyIndexCount,            \
            src.pQueueFamilyIndices, static_cast<VkImageLayout>(src.initialLayout)             \
    }

namespace Siege::Vulkan
{
class Image
{
public:
    struct Config
    {
        Config& WithFlag(const uint32_t& flag);
        Config& WithType(const uint32_t& typeFlag);
        Config& WithFormat(const uint32_t& formatFlag);
        Config& WithExtent(const uint32_t& width, const uint32_t& height, const uint32_t& depth);
        Config& WithMipLevels(const uint32_t& mipLevel);
        Config& WithArrayLayers(const uint32_t& arrayLayerCount);
        Config& WithSampleFlags(const uint32_t& sampleFlags);
        Config& WithImageTiling(const uint32_t& imageTiling);
        Config& WithImageUsage(const uint32_t& imageUsage);
        Config& WithSharingMode(const uint32_t& mode);
        Config& WithQueueFamilies(const uint32_t* indices, uint32_t count);
        Config& WithInitialLayout(const uint32_t& initialLayout);

        uint32_t createFlag {0};
        uint32_t imageType {0};
        uint32_t format {0};
        Utils::Extent3D extent {0, 0, 0};
        uint32_t mipLevels {0};
        uint32_t arrayLayers {0};
        uint32_t samples {0};
        uint32_t tiling {0};
        uint32_t usage {0};
        uint32_t sharingMode {0};
        uint32_t queueFamilyIndexCount {0};
        const uint32_t* pQueueFamilyIndices {nullptr};
        uint32_t initialLayout {0};
        void* next {nullptr};
    };

    Image() = default;
    Image(const Config& config);
    ~Image() = default;

private:

    VkImage image;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_IMAGE_H
