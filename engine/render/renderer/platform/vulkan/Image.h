//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_IMAGE_H
#define SIEGE_ENGINE_VULKAN_IMAGE_H

#include <cstdint>

#include "utils/Types.h"

namespace Siege::Vulkan
{
class Image
{
public:

    struct Config
    {
        Utils::ImageFormat imageFormat;
        Utils::Extent3D imageExtent;
        Utils::ImageUsage usage;
        uint32_t mipLevels;
        uint32_t layers;
    };

    struct Info
    {
        VkImageView view {nullptr};
        Utils::ImageLayout layout {Utils::ImageLayout::LAYOUT_UNDEFINED};
        Utils::PipelineStage stage {Utils::PipelineStage::STAGE_TOP_OF_PIPE};
        Utils::MemoryAccess access {Utils::MemoryAccess::ACCESS_NONE};
    };

    Image() = default;
    Image(const Config& config);
    Image(VkImage swapchainImage, const Config& config);
    Image(Image&& other);
    ~Image();

    Image& operator=(Image&& other);

    const Info GetInfo() const
    {
        return info;
    }

    VkImageView GetView()
    {
        return info.view;
    }

    const VkImage& GetImage() const
    {
        return image;
    }
    const VkDeviceMemory& GetMemory() const
    {
        return memory;
    }
    const VkImageView GetView() const
    {
        return info.view;
    }

    void Free();
    void Invalidate();

    bool IsValid();
    bool HasInfo();

    void CopyBuffer(VkBuffer buffer,
                    Utils::Extent3D bufferExtent,
                    Utils::Offset3D offset = {0, 0, 0});

    /**
     * Transitions an image from one layout to another
     * @param newStage the stage in the pipeline to transition to image to
     * @param newLayout the layout you want to transition into
     * @param newAccess the new access rules we want to transition into
     */
    void TransitionLayout(Utils::PipelineStage newStage,
                          Utils::ImageLayout newLayout,
                          Utils::MemoryAccess newAccess);

private:

    static bool IsDepthFormat(Utils::ImageFormat format);
    static uint32_t GetVkUsageFlag(Utils::ImageUsage usage, Utils::ImageFormat format);

    void CreateImage(const Config& config);
    void AllocateMemory();
    void BindImageMemory(VkDevice);
    void CreateImageView(const Config& config);

    void Swap(Image& other);

    VkImage image {nullptr};
    VkDeviceMemory memory {nullptr};
    Utils::Extent3D extent {0, 0, 0};
    Info info;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_IMAGE_H
