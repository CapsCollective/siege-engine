//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Image.h"
#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/utils/TypeAdaptor.h"
#include "render/renderer/platform/vulkan/utils/Device.h"

#include <volk/volk.h>
#include <utils/Logging.h>

namespace Siege::Vulkan
{
Image::Image(const Config& config)
{
    VkImageUsageFlags usage = 0;

    bool isDepthFormat = IsDepthFormat(config.imageFormat);
    auto device = Context::GetCurrentDevice()->GetDevice();
    auto physicalDevice = Context::GetPhysicalDevice()->GetDevice();

    switch(config.usage)
    {
        case Utils::USAGE_ATTACHMENT:
            usage |= ((isDepthFormat * VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) +
                     (!isDepthFormat * VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT));
            break;
        case Utils::USAGE_TEXTURE:
            usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            break;
        case Utils::USAGE_STORAGE:
            usage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            break;
        default:
            usage |= VK_IMAGE_USAGE_SAMPLED_BIT; break;
    }

    VkImageCreateInfo createInfo =
    {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        nullptr,
        0,
        VK_IMAGE_TYPE_2D,
        Utils::ToVkFormat(config.imageFormat),
        Utils::ToVkExtent3D(config.imageExtent),
        config.mipLevels,
        config.layers,
        VK_SAMPLE_COUNT_1_BIT,
        VK_IMAGE_TILING_OPTIMAL,
        usage
    };

    CC_ASSERT(vkCreateImage(device, &createInfo, nullptr, OUT & image) == VK_SUCCESS,
              "Failed to create FrameImages!")

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, OUT & memRequirements);

    VkMemoryAllocateInfo allocInfo
        {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            nullptr,
            memRequirements.size,
            Device::Physical::FindMemoryType(physicalDevice,
                                             memRequirements.memoryTypeBits,
                                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        };

    CC_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, OUT & memory) == VK_SUCCESS,
              "Failed to allocate image memory!")

    CC_ASSERT(vkBindImageMemory(device, OUT image, memory, 0) == VK_SUCCESS,
              "Failed to bind image memory!")

    VkImageAspectFlags aspectMask = (isDepthFormat * VK_IMAGE_ASPECT_DEPTH_BIT) +
                                    ((!isDepthFormat * VK_IMAGE_ASPECT_COLOR_BIT));

    aspectMask |= ((config.imageFormat == Utils::DEPTH24STENCIL8) * VK_IMAGE_ASPECT_STENCIL_BIT);

    VkImageViewCreateInfo viewCreateInfo =
    {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        nullptr,
        0,
        image,
        static_cast<VkImageViewType>(((config.layers > 1) * VK_IMAGE_VIEW_TYPE_2D_ARRAY) +
                                     ((config.layers == 1) * VK_IMAGE_VIEW_TYPE_2D)),
        Utils::ToVkFormat(config.imageFormat),
        {},
        {aspectMask, 0, config.mipLevels, 0, config.layers}
        };

    CC_ASSERT(vkCreateImageView(device, &viewCreateInfo, nullptr, OUT & imageView) == VK_SUCCESS,
              "Failed to create texture image view!")
}

Image::Image(VkImage swapchainImage, const Config& config) : image {swapchainImage}
{
    bool isDepthFormat = IsDepthFormat(config.imageFormat);
    auto device = Context::GetCurrentDevice()->GetDevice();

    VkImageAspectFlags aspectMask = (isDepthFormat * VK_IMAGE_ASPECT_DEPTH_BIT) +
                                    ((!isDepthFormat * VK_IMAGE_ASPECT_COLOR_BIT));

    aspectMask |= ((config.imageFormat == Utils::DEPTH24STENCIL8) * VK_IMAGE_ASPECT_STENCIL_BIT);

    VkImageViewCreateInfo viewCreateInfo =
        {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            nullptr,
            0,
            image,
            static_cast<VkImageViewType>(((config.layers > 1) * VK_IMAGE_VIEW_TYPE_2D_ARRAY) +
                                         ((config.layers == 1) * VK_IMAGE_VIEW_TYPE_2D)),
            Utils::ToVkFormat(config.imageFormat),
            {},
            {aspectMask, 0, config.mipLevels, 0, config.layers}
        };

    CC_ASSERT(vkCreateImageView(device, &viewCreateInfo, nullptr, OUT & imageView) == VK_SUCCESS,
              "Failed to create texture image view!")
}

Image::Image(Image&& other)
{
    Move(other);
}

Image::~Image()
{
    if (!IsValid() && !image) return;

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
    return imageView;
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
    return format == Utils::DEPTH24STENCIL8 ||
           format == Utils::DEPTH32F ||
           format == Utils::DEPTH32FSTENCIL8UINT;
}
} // namespace Siege::Vulkan