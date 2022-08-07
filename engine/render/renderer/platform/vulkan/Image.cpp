//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Image.h"

#include <utils/Logging.h>

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/utils/Image.h"
#include "render/renderer/platform/vulkan/utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Image::Image(const Config& config)
{
    auto device = Context::GetCurrentDevice()->GetDevice();

    CreateImage(config);
    AllocateMemory();
    BindImageMemory(device);

    CreateImageView(config);
}

Image::Image(VkImage swapchainImage, const Config& config) : image {swapchainImage}
{
    CreateImageView(config);
}

void Image::CreateImage(const Config& config)
{
    image = Utils::Image::CreateImage(
        VK_IMAGE_TYPE_2D,
        Utils::ToVkFormat(config.imageFormat),
        Utils::ToVkExtent3D(config.imageExtent),
        config.mipLevels,
        config.layers,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        static_cast<VkImageUsageFlagBits>(GetVkUsageFlag(config.usage, config.imageFormat)));
}

void Image::AllocateMemory()
{
    Utils::Image::AllocateImageMemory(image, memory);
}

void Image::BindImageMemory(VkDevice device)
{
    CC_ASSERT(vkBindImageMemory(device, OUT image, memory, 0) == VK_SUCCESS,
              "Failed to bind image memory!")
}

void Image::CreateImageView(const Config& config)
{
    bool isDepthFormat = IsDepthFormat(config.imageFormat);

    VkImageAspectFlags aspectMask = (isDepthFormat * VK_IMAGE_ASPECT_DEPTH_BIT) +
                                    ((!isDepthFormat * VK_IMAGE_ASPECT_COLOR_BIT));

    aspectMask |= ((config.imageFormat == Utils::DEPTH24STENCIL8) * VK_IMAGE_ASPECT_STENCIL_BIT);

    VkImageViewType viewType =
        static_cast<VkImageViewType>(((config.layers > 1) * VK_IMAGE_VIEW_TYPE_2D_ARRAY) +
                                     ((config.layers == 1) * VK_IMAGE_VIEW_TYPE_2D));

    imageView = Utils::Image::CreateImageView(image,
                                              viewType,
                                              Utils::ToVkFormat(config.imageFormat),
                                              {},
                                              {aspectMask, 0, config.mipLevels, 0, config.layers});
}

Image::Image(Image&& other)
{
    Move(other);
}

Image::~Image()
{
    if (!IsValid()) return;

    CC_LOG_INFO("Destroying Image")
    Free();
}

Image& Image::operator=(Image&& other)
{
    if (imageView) Free();

    Move(other);
    return *this;
}

void Image::Free()
{
    auto device = Context::GetCurrentDevice()->GetDevice();

    vkDestroyImageView(device, imageView, nullptr);

    if (HasInfo())
    {
        vkDestroyImage(device, image, nullptr);
        vkFreeMemory(device, memory, nullptr);
    }

    Invalidate();
}

void Image::Invalidate()
{
    image = nullptr;
    imageView = nullptr;
    memory = nullptr;
}

bool Image::HasInfo()
{
    return memory;
}

bool Image::IsValid()
{
    return imageView && image;
}

void Image::Move(Image& other)
{
    image = other.image;
    imageView = other.imageView;
    memory = other.memory;

    other.image = nullptr;
    other.imageView = nullptr;
    other.memory = nullptr;
}

bool Image::IsDepthFormat(Utils::ImageFormat format)
{
    return format == Utils::DEPTH24STENCIL8 || format == Utils::DEPTH32F ||
           format == Utils::DEPTH32FSTENCIL8UINT;
}

uint32_t Image::GetVkUsageFlag(Utils::ImageUsage usage, Utils::ImageFormat format)
{
    bool isDepthFormat = IsDepthFormat(format);

    switch (usage)
    {
        case Utils::USAGE_ATTACHMENT:
            return ((isDepthFormat * VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) +
                    (!isDepthFormat * VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
        case Utils::USAGE_TEXTURE:
            return VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        case Utils::USAGE_STORAGE:
            return VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        default:
            return VK_IMAGE_USAGE_SAMPLED_BIT;
    }
}
} // namespace Siege::Vulkan