//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Image.h"

#include <utils/Logging.h>

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/utils/Image.h"
#include "render/renderer/platform/vulkan/utils/TypeAdaptor.h"

namespace Siege::Vulkan
{
Image::Image(const Config& config) : extent {config.imageExtent}
{
    auto device = Context::GetCurrentDevice()->GetDevice();

    CreateImage(config);
    AllocateMemory();
    BindImageMemory(device);

    CreateImageView(config);
}

Image::Image(VkImage swapchainImage, const Config& config) :
    image {swapchainImage},
    extent {config.imageExtent}
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

    info.view = Utils::Image::CreateImageView(image,
                                              viewType,
                                              Utils::ToVkFormat(config.imageFormat),
                                              {},
                                              {aspectMask, 0, config.mipLevels, 0, config.layers});
}

Image::Image(Image&& other)
{
    Swap(other);
}

Image::~Image()
{
    if (!IsValid()) return;

    CC_LOG_INFO("Destroying Image")
    Free();
}

Image& Image::operator=(Image&& other)
{
    if (info.view) Free();

    Swap(other);
    return *this;
}

void Image::Free()
{
    auto device = Context::GetCurrentDevice()->GetDevice();

    if (device == nullptr) return;

    vkDestroyImageView(device, info.view, nullptr);

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
    info.view = nullptr;
    memory = nullptr;
}

bool Image::HasInfo()
{
    return memory;
}

bool Image::IsValid()
{
    return info.view && image;
}

// TODO: we'll likely need to send some sort of config object into this to get better config options
void Image::CopyBuffer(VkBuffer buffer, Utils::Extent3D bufferExtent, Utils::Offset3D offset)
{
    TransitionLayout(Utils::STAGE_TRANSFER_BIT,
                     Utils::LAYOUT_TRANSFER_DST_OPTIMAL,
                     Utils::ACCESS_TRANSFER_WRITE);

    CommandBuffer::ExecuteSingleTimeCommand([&](VkCommandBuffer commandBuffer) {
        VkBufferImageCopy copyRegion = {};
        copyRegion.bufferOffset = 0;
        copyRegion.bufferRowLength = 0;
        copyRegion.bufferImageHeight = 0;

        copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.imageSubresource.mipLevel = 0;
        copyRegion.imageSubresource.baseArrayLayer = 0;
        copyRegion.imageSubresource.layerCount = 1;
        copyRegion.imageOffset = {offset.width, offset.height, offset.depth};
        copyRegion.imageExtent = {bufferExtent.width, bufferExtent.height, 1};

        vkCmdCopyBufferToImage(commandBuffer,
                               buffer,
                               image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &copyRegion);
    });

    TransitionLayout(Utils::STAGE_FRAGMENT_SHADER,
                     Utils::LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                     Utils::ACCESS_SHADER_READ);

    info.layout = Utils::ImageLayout::LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    info.access = Utils::MemoryAccess::ACCESS_SHADER_READ;
    info.stage = Utils::PipelineStage::STAGE_FRAGMENT_SHADER;
}

void Image::TransitionLayout(Utils::PipelineStage newStage,
                             Utils::ImageLayout newLayout,
                             Utils::MemoryAccess newAccess)
{
    CommandBuffer::ExecuteSingleTimeCommand([&](VkCommandBuffer commandBuffer) {
        auto range = Utils::Image::SubResourceRange()
                         .WithAspect(VK_IMAGE_ASPECT_COLOR_BIT)
                         .WithMipLevel(0)
                         .WithLevelCount(1)
                         .WithLayerCount(1)
                         .WithArrayLayer(0)
                         .Build();

        auto barrier = Utils::Image::Barrier()
                           .ToAccessMask(newAccess)
                           .FromAccessMask(info.access)
                           .FromLayout(Utils::ToVkImageLayout(info.layout))
                           .ToLayout(Utils::ToVkImageLayout(newLayout))
                           .WithImage(image)
                           .WithSubResourceRange(range)
                           .Build();

        vkCmdPipelineBarrier(commandBuffer,
                             Utils::ToVkPipelineStageFlagBits(info.stage),
                             Utils::ToVkPipelineStageFlagBits(newStage),
                             0,
                             0,
                             nullptr,
                             0,
                             nullptr,
                             1,
                             &barrier);

        info.access = newAccess;
        info.layout = newLayout;
        info.stage = newStage;
    });
}

void Image::Swap(Image& other)
{
    auto tmpImage = image;
    auto tmpImageInfo = info;
    auto tmpMemory = memory;
    auto tmpExtent = extent;

    image = other.image;
    info = other.info;
    memory = other.memory;
    extent = other.extent;

    other.image = tmpImage;
    other.info = tmpImageInfo;
    other.memory = tmpMemory;
    other.extent = tmpExtent;
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
            return VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                   VK_IMAGE_USAGE_SAMPLED_BIT;
        case Utils::USAGE_STORAGE:
            return VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        default:
            return VK_IMAGE_USAGE_SAMPLED_BIT;
    }
}
} // namespace Siege::Vulkan