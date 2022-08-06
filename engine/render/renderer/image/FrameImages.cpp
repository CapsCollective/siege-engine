//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FrameImages.h"

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/Image.h"

namespace Siege
{
uint32_t FrameImages::imageCount = 0;

FrameImages::FrameImages() = default;

FrameImages::FrameImages(VkFormat format) : imageFormat {format} {}

FrameImages::~FrameImages() = default;

void FrameImages::InitDepthImageView2D(uint32_t imageWidth,
                                       uint32_t imageHeight,
                                       uint32_t imageDepth)
{
    auto device = Vulkan::Context::GetCurrentDevice();
    for (size_t i = 0; i < GetImageCount(); i++)
    {
        VkImageCreateInfo imageInfo =
            Image::CreateImageCreateInfo(VK_IMAGE_TYPE_2D,
                                         imageFormat,
                                         imageWidth,
                                         imageHeight,
                                         imageDepth,
                                         1,
                                         1,
                                         VK_SAMPLE_COUNT_1_BIT,
                                         VK_IMAGE_TILING_OPTIMAL,
                                         VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                         VK_SHARING_MODE_EXCLUSIVE);

        // Create the depth images using the device
        device->CreateImageWithInfo(imageInfo, OUT images[i], OUT imageMemorys[i]);

        // Set up the view image creation struct
        imageViews[i] = Image::CreateImageView(device->GetDevice(),
                                               images[i],
                                               VK_IMAGE_VIEW_TYPE_2D,
                                               imageFormat,
                                               {},
                                               {VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1});
    }

    hasInfo = true;
}

void FrameImages::InitColorImageView2D()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();
    for (size_t i = 0; i < GetImageCount(); i++)
    {
        SetImageView(Image::CreateImageView(device,
                                            GetImage(i),
                                            VK_IMAGE_VIEW_TYPE_2D,
                                            imageFormat,
                                            {},
                                            {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1}),
                     i);
    }
}

void FrameImages::DestroyFrameImages()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();
    for (size_t i = 0; i < GetImageCount(); i++)
    {
        vkDestroyImageView(device, imageViews[i], nullptr);

        if (hasInfo)
        {
            vkDestroyImage(device, images[i], nullptr);
            vkFreeMemory(device, imageMemorys[i], nullptr);
        }
    }
}
} // namespace Siege
