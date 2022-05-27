#include "FrameImages.h"

namespace SnekVk {
    u32 FrameImages::imageCount = 0;

    FrameImages::FrameImages() = default;

    FrameImages::FrameImages(VulkanDevice *vulkanDevice, VkFormat format)
    : device{ vulkanDevice }, imageFormat { format }
    {}

    FrameImages::~FrameImages() = default;

    void FrameImages::InitDepthImageView2D(u32 imageWidth, u32 imageHeight, u32 imageDepth)
    {
        for (size_t i = 0; i < GetImageCount(); i++)
        {
            VkImageCreateInfo imageInfo = Image::CreateImageCreateInfo(VK_IMAGE_TYPE_2D,
                                                                       imageFormat,
                                                                       imageWidth,
                                                                       imageHeight,
                                                                       imageDepth, 1, 1,
                                                                       VK_SAMPLE_COUNT_1_BIT,
                                                                       VK_IMAGE_TILING_OPTIMAL,
                                                                       VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                                       VK_SHARING_MODE_EXCLUSIVE);

            // Create the depth images using the device
            device->CreateImageWithInfo(
                    imageInfo,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    OUT images[i],
                    OUT imageMemorys[i]);

            // Set up the view image creation struct
            imageViews[i] = Image::CreateImageView(device->Device(),
                                                                    images[i],
                                                                    VK_IMAGE_VIEW_TYPE_2D,
                                                                    imageFormat,
                                                                    {},
                                                                    {
                                                                        VK_IMAGE_ASPECT_DEPTH_BIT,
                                                                        0,
                                                                        1,
                                                                        0,
                                                                        1
                                                                    });
        }

        hasInfo = true;
    }

    void FrameImages::InitColorImageView2D()
    {
        for (size_t i = 0; i < GetImageCount(); i++)
        {
            SetImageView(Image::CreateImageView(device->Device(),
                                                GetImage(i),
                                                VK_IMAGE_VIEW_TYPE_2D,
                                                imageFormat,
                                                {},
                                                {
                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                                    0,
                                                    1,
                                                    0,
                                                    1}), i);
        }
    }

    void FrameImages::DestroyFrameImages()
    {
        for (size_t i = 0; i < GetImageCount(); i++)
        {
            vkDestroyImageView(device->Device(), imageViews[i], nullptr);

            if (hasInfo)
            {
                vkDestroyImage(device->Device(), images[i], nullptr);
                vkFreeMemory(device->Device(), imageMemorys[i], nullptr);
            }
        }
    }
}

