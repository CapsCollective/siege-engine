//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_SWAPCHAIN_H
#define SIEGE_ENGINE_VULKAN_SWAPCHAIN_H

#include "render/renderer/image/FrameImages.h"
#include "render/renderer/pass/RenderPass.h"
#include "render/renderer/platform/vulkan/utils/Types.h"

#include "Semaphore.h"
#include "Fence.h"

namespace Siege::Vulkan
{
class Swapchain
{
public:

    /**
     *  When swapping out images for our frames, we can have multiple frames 'in flight',
     *  meaning frames that act as additional memory buffers. In our case we allow for two
     *additional frames in flight for added image buffering.
     **/
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    Swapchain() = default;
    Swapchain(const Utils::Extent2D& imageExtent, VkSwapchainKHR oldSwapchain = nullptr);
    Swapchain(Swapchain&& other);

    Swapchain(const Swapchain& other) = delete;

    Swapchain& operator=(Swapchain&& other);
    Swapchain& operator=(const Swapchain& other) = delete;
    ~Swapchain();

    /**
     * @brief Loads in the next image to be written to in the Swapchain.
     *
     * @param imageIndex - the index of the next image.
     * @return VkResult - the result of acquiring the next image.
     */
    VkResult AcquireNextImage(uint32_t* imageIndex);

    /**
     * @brief Submits a command buffer for drawing.
     *
     * @param buffers - an array of command buffers.
     * @param imageIndex - the index of the image being drawn.
     * @return VkResult - the result of submitting the buffer.
     */
    VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

    VkSwapchainKHR GetRaw()
    {
        return swapchain;
    }

    Utils::DepthFormat GetDepthFormat()
    {
        return swapChainDepthFormat;
    }
    Utils::ImageFormat GetImageFormat()
    {
        return swapChainImageFormat;
    }

    bool IsSameSwapFormat(Utils::ImageFormat oldImageFormat, Utils::DepthFormat oldDepthFormat);

    RenderPass* GetRenderPass()
    {
        return &renderPass;
    }

    /**
     * @brief Get a frame buffer object corresponding to index i. The swapchain object stores
     * multiple frame buffers equal to the number of images that can be held at once.
     *
     * @param i the index of the required framebuffer. NOTE: this can return a segfault if an
     * incorrect index is provided. Please ensure that the index provided is between 0 and the
     * swapchain's max image count.
     * @return VkFramebuffer the framebuffer in index i
     */
    VkFramebuffer GetFrameBuffer(uint32_t i)
    {
        return swapChainFrameBuffers[i];
    }

    Utils::Extent2D GetExtent()
    {
        return swapchainExtent;
    }

private:

    void CreateSwapChain(VkSwapchainKHR oldSwapchain);
    void CreateRenderPass();
    void CreateDepthResources();
    void CreateFrameBuffers();
    void CreateSyncObjects();

    void Swap(Swapchain& other);

    Utils::Extent2D windowExtent {0, 0};
    Utils::Extent2D swapchainExtent {0, 0};

    VkSwapchainKHR swapchain {nullptr};

    VkFramebuffer* swapChainFrameBuffers {VK_NULL_HANDLE};

    FrameImages swapchainImages;
    FrameImages depthImages;

    RenderPass renderPass;

    Utils::ImageFormat swapChainImageFormat {Utils::NONE};
    Utils::DepthFormat swapChainDepthFormat {Utils::DEPTH_NONE};

    Semaphore imageAvailableSemaphores;
    Semaphore renderFinishedSemaphores;

    Fence inFlightFences;
    Fence imagesInFlight;

    size_t currentFrame = 0;
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_SWAPCHAIN_H
