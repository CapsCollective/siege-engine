//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FRAME_IMAGES_H
#define SIEGE_ENGINE_FRAME_IMAGES_H

#include "../Core.h"
#include "../device/VulkanDevice.h"
#include "utils/Image.h"

namespace Siege
{
class FrameImages
{
public:

    static constexpr u32 MAX_IMAGES = 5;

    FrameImages();
    FrameImages(VulkanDevice* vulkanDevice, VkFormat format);
    ~FrameImages();

    void InitColorImageView2D();
    void InitDepthImageView2D(u32 imageWidth, u32 imageHeight, u32 imageDepth);

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

    const VkImage& GetImage(size_t index) const
    {
        return images[index];
    }
    const VkImageView& GetImageView(size_t index) const
    {
        return imageViews[index];
    }

    static void SetImageCount(u32 count)
    {
        imageCount = count;
    }
    static u32 GetImageCount()
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

    static u32 imageCount;
};
} // namespace Siege

#endif // SIEGE_ENGINE_FRAME_IMAGES_H
