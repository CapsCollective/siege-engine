#pragma once

#include "../Core.h"
#include "../Device/VulkanDevice.h"
#include "Utils/Image.h"

namespace SnekVk
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

        void SetFormat(VkFormat format) { imageFormat = format; }
        void SetImageView(VkImageView imageView, size_t index) { imageViews[index] = imageView; }

        VkFormat GetFormat() { return imageFormat; }

        VkImage* GetImages() { return images; }
        VkImageView* GetImageViews() { return imageViews; }
        VkDeviceMemory* GetImageMemorys() { return imageMemorys; }

        VkImage GetImage(size_t index) { return images[index]; }
        VkImageView GetImageView(size_t index) { return imageViews[index]; }

        static void SetImageCount(u32 count) { imageCount = count; }
        static u32 GetImageCount() { return imageCount; }
    private:

        VulkanDevice* device {nullptr};

        VkImage images[MAX_IMAGES] {VK_NULL_HANDLE};
        VkImageView imageViews[MAX_IMAGES] {VK_NULL_HANDLE};
        VkDeviceMemory imageMemorys[MAX_IMAGES] {VK_NULL_HANDLE};

        VkFormat imageFormat {VK_FORMAT_UNDEFINED};

        bool hasInfo{false};

        static u32 imageCount;
    };
}

