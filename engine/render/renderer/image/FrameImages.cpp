//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "FrameImages.h"

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/Image.h"
#include "render/renderer/platform/vulkan/utils/TypeAdaptor.h"

namespace Siege
{
uint32_t FrameImages::imageCount = 0;

FrameImages::FrameImages() = default;

FrameImages::FrameImages(Vulkan::Utils::Extent3D imageExtent, Vulkan::Utils::ImageFormat format) :
    imageFormat {format}
{
    Siege::Vulkan::Image::Config config = {imageFormat,
                                           imageExtent,
                                           Vulkan::Utils::USAGE_ATTACHMENT,
                                           1,
                                           1};

    for (size_t i = 0; i < GetImageCount(); i++) vkImages[i] = Vulkan::Image(config);
}

FrameImages::FrameImages(FrameImages&& other)
{
    Move(other);
}

FrameImages::FrameImages(VkSwapchainKHR swapchain,
                         Vulkan::Utils::Extent3D imageExtent,
                         Vulkan::Utils::ImageFormat format) :
    FrameImages(imageExtent, format)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);

    FrameImages::SetImageCount(imageCount);

    VkImage swapchainImages[imageCount];

    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages);

    Siege::Vulkan::Image::Config config = {imageFormat,
                                           imageExtent,
                                           Vulkan::Utils::USAGE_ATTACHMENT,
                                           1,
                                           1};

    for (size_t i = 0; i < imageCount; i++) vkImages[i] = Vulkan::Image(swapchainImages[i], config);
}

FrameImages::~FrameImages() = default;

FrameImages& FrameImages::operator=(FrameImages&& other)
{
    Move(other);
    return *this;
}

void FrameImages::InitDepthImageView2D(uint32_t imageWidth,
                                       uint32_t imageHeight,
                                       uint32_t imageDepth)
{
    Siege::Vulkan::Image::Config config = {imageFormat,
                                           {imageWidth, imageHeight, imageDepth},
                                           Vulkan::Utils::USAGE_ATTACHMENT,
                                           1,
                                           1};

    for (size_t i = 0; i < GetImageCount(); i++) vkImages[i] = Vulkan::Image(config);
}

void FrameImages::Move(FrameImages& other)
{
    imageFormat = other.imageFormat;

    for (size_t i = 0; i < imageCount; i++)
    {
        vkImages[i] = std::move(other.vkImages[i]);
    }
}
} // namespace Siege
