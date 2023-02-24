//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_SWAPCHAIN_H
#define SIEGE_ENGINE_VULKAN_SWAPCHAIN_H

#include "Fence.h"
#include "Semaphore.h"
#include "render/renderer/image/FrameImages.h"
#include "render/renderer/pass/RenderPass.h"
#include "render/renderer/platform/vulkan/CommandBuffer.h"
#include "render/renderer/platform/vulkan/utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The Swapchain class is a wrapper over a Swapchain vulkan concept. The swapchain is responsible
 * for managing and swapping out images between frames. The swapchain will store an image per frame
 * and cycle through them as we submit images for drawing. The swapchain will also handle drawing
 * to the screen and queue submissions.
 */
class Swapchain
{
public:

    /**
     *  When swapping out images for our frames, we can have multiple frames 'in flight',
     *  meaning frames that act as additional memory buffers. In our case we allow for two
     *  additional frames in flight for added image buffering
     **/
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    // 'Structors

    /**
     * Default empty constructor
     */
    Swapchain() = default;

    /**
     * Constructor for the Swapchain
     * @param imageExtent the width/height of the swapchain images
     * @param oldSwapchain the old swapchain to copy information from. Defaults to nullptr
     */
    Swapchain(const Utils::Extent2D& imageExtent, VkSwapchainKHR oldSwapchain = nullptr);

    /**
     * Swapchain move constructor
     * @param other the Swapchain to move
     */
    Swapchain(Swapchain&& other);

    /**
     * Deleted copy constructor
     * @param other the Swapchain to copy from
     */
    Swapchain(const Swapchain& other) = delete;

    /**
     * Destructor for the Swapchain
     */
    ~Swapchain();

    // Operators

    /**
     * Move assignment operator
     * @param other the swapchain to move
     * @return a reference to the created Swapchain
     */
    Swapchain& operator=(Swapchain&& other);

    /**
     * Deleted copy assignment operator.
     * @param other the swapchain to copy from.
     * @return a reference to the created Swapchain
     */
    Swapchain& operator=(const Swapchain& other) = delete;

    /**
     * @brief Loads in the next image to be written to in the Swapchain
     *
     * @param imageIndex - the index of the next image
     * @return the result of acquiring the next image
     */
    Utils::Result AcquireNextImage(uint32_t* imageIndex);

    /**
     * @brief Submits a command buffer for drawing
     *
     * @param buffers - an array of command buffers
     * @param imageIndex - the index of the image being drawn
     * @return VkResult - the result of submitting the buffer
     */
    Utils::Result SubmitCommandBuffers(const CommandBuffer& commandBuffer, uint32_t imageIndex);

    /**
     * Compares the inputted image and depth formats to the ones used in the Swapchain
     * @param oldImageFormat the image format to compare
     * @param oldDepthFormat the depth format to compare
     * @return true if both the image and depth formats are true
     */
    bool IsSameSwapFormat(Utils::ImageFormat oldImageFormat, Utils::DepthFormat oldDepthFormat);

    // Getters

    /**
     * Returns the raw vulkan swapchain
     * @return the raw swapchain pointer
     */
    VkSwapchainKHR GetRaw()
    {
        return swapchain;
    }

    /**
     * Returns the depth format used by the swapchain images
     * @return the image depth format
     */
    Utils::DepthFormat GetDepthFormat()
    {
        return swapChainDepthFormat;
    }

    /**
     * Returns the image format used by the swapchain images
     * @return the image format
     */
    Utils::ImageFormat GetImageFormat()
    {
        return swapChainImageFormat;
    }

    /**
     * Returns a pointer to the Swapchain's renderpass
     * @return a pointer to the Swapchain's renderpass
     */
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

    /**
     * Returns the swapchain image extent
     * @return the width and height of the swapchain images
     */
    Utils::Extent2D GetExtent()
    {
        return swapchainExtent;
    }

    void BeginRenderPass(Vulkan::CommandBuffer& commandBuffer,
                         uint32_t imageIndex,
                         std::initializer_list<VkClearValue> clearValues);

    void EndRenderPass(Vulkan::CommandBuffer& commandBuffer);

private:

    /**
     * Initiates the Vulkan swapchain
     * @param oldSwapchain allows an old swapchain pointer to be passed into the swapchain creation
     */
    void CreateSwapChain(VkSwapchainKHR oldSwapchain);

    /**
     * Initiates the vulkan render pass
     */
    void CreateRenderPass();

    /**
     * Initiates our depth images
     */
    void CreateDepthResources();

    /**
     * Initiates our per-frame frame buffers
     */
    void CreateFrameBuffers();

    /**
     * Initiates all the Semaphore and Fence objects requires to synchronise image rendering
     */
    void CreateSyncObjects();

    /**
     * Swaps the contents of two Swapchains. Used primarily for moving content between swapchains
     * @param other
     */
    void Swap(Swapchain& other);

    Utils::Extent2D windowExtent {0, 0};
    Utils::Extent2D swapchainExtent {0, 0};

    VkSwapchainKHR swapchain {nullptr};

    // TODO: Need to catch up with main so that we can replace this with the Framebuffer class
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
