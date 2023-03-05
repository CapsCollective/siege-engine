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

    struct Info
    {
        VkImageView view {nullptr};
        Utils::ImageLayout layout {Utils::ImageLayout::LAYOUT_UNDEFINED};
    };

    Image() = default;
    Image(const Config& config);
    Image(VkImage swapchainImage, const Config& config);
    Image(Image&& other);
    ~Image();

    Image& operator=(Image&& other);

    const Info& GetInfo() const
    {
        return info;
    }

    VkImageView GetView()
    {
        return info.view;
    }

    const VkImage& GetImage() const
    {
        return image;
    }
    const VkDeviceMemory& GetMemory() const
    {
        return memory;
    }
    const VkImageView GetView() const
    {
        return info.view;
    }

    void Free();
    void Invalidate();

    bool IsValid();
    bool HasInfo();

    void CopyBuffer(VkBuffer buffer);

private:

    static bool IsDepthFormat(Utils::ImageFormat format);
    static uint32_t GetVkUsageFlag(Utils::ImageUsage usage, Utils::ImageFormat format);

    void CreateImage(const Config& config);
    void AllocateMemory();
    void BindImageMemory(VkDevice);
    void CreateImageView(const Config& config);

    void Swap(Image& other);

    VkImage image {nullptr};
    VkDeviceMemory memory {nullptr};
    Utils::Extent3D extent {0, 0, 0};
    Info info;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_IMAGE_H
