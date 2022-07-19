//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"
#include "../device/VulkanDevice.h"
#include "utils/Image.h"

namespace Siege
{
class FrameImages
{
public:

    static constexpr uint32_t MAX_IMAGES = 5;

    FrameImages();
    FrameImages(VulkanDevice* vulkanDevice, VkFormat format);
    ~FrameImages();

    void InitColorImageView2D();
    void InitDepthImageView2D(uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);

    void DestroyFrameImages();

    void SetFormat(VkFormat format)
    {
        imageFormat = format;
    }
    void SetImageView(VkImageView imageView, size_t index)
    {
        imageViews[index] = imageView;
    }

    VkFormat GetFormat()
    {
        return imageFormat;
    }

    VkImage* GetImages()
    {
        return images;
    }
    VkImageView* GetImageViews()
    {
        return imageViews;
    }
    VkDeviceMemory* GetImageMemorys()
    {
        return imageMemorys;
    }

    VkImage GetImage(size_t index)
    {
        return images[index];
    }
    VkImageView GetImageView(size_t index)
    {
        return imageViews[index];
    }

    static void SetImageCount(uint32_t count)
    {
        imageCount = count;
    }
    static uint32_t GetImageCount()
    {
        return imageCount;
    }

private:

    VulkanDevice* device {nullptr};

    VkImage images[MAX_IMAGES] {VK_NULL_HANDLE};
    VkImageView imageViews[MAX_IMAGES] {VK_NULL_HANDLE};
    VkDeviceMemory imageMemorys[MAX_IMAGES] {VK_NULL_HANDLE};

    VkFormat imageFormat {VK_FORMAT_UNDEFINED};

    bool hasInfo {false};

    static uint32_t imageCount;
};
} // namespace Siege
