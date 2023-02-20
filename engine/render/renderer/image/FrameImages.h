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
#include "render/renderer/platform/vulkan/Image.h"
#include "render/renderer/platform/vulkan/utils/Image.h"

namespace Siege
{
class FrameImages
{
public:

    static constexpr uint32_t MAX_IMAGES = 5;

    FrameImages();
    FrameImages(Vulkan::Utils::Extent3D imageExtent, Vulkan::Utils::ImageFormat format);
    FrameImages(VkSwapchainKHR swapchain,
                Vulkan::Utils::Extent3D imageExtent,
                Vulkan::Utils::ImageFormat format);
    FrameImages(FrameImages&& other);
    ~FrameImages();

    FrameImages& operator=(FrameImages&& other);

    void InitDepthImageView2D(uint32_t imageWidth, uint32_t imageHeight, uint32_t imageDepth);

    Vulkan::Utils::ImageFormat GetFormat()
    {
        return imageFormat;
    }

    Vulkan::Image& GetVkImage(size_t index)
    {
        return vkImages[index];
    }

    const Vulkan::Utils::ImageFormat GetFormat() const
    {
        return imageFormat;
    }

    const Vulkan::Image& GetVkImage(size_t index) const
    {
        return vkImages[index];
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

    void Move(FrameImages& other);

    Vulkan::Image vkImages[MAX_IMAGES];

    Vulkan::Utils::ImageFormat imageFormat {Vulkan::Utils::NONE};

    static uint32_t imageCount;
};
} // namespace Siege

#endif // SIEGE_ENGINE_FRAME_IMAGES_H
