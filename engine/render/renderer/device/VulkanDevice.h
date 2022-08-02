//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_DEVICE_H
#define SIEGE_ENGINE_VULKAN_DEVICE_H

#include "../../window/Window.h"
#include "../Core.h"
#include "utils/DebugUtilsMessenger.h"
#include "utils/Extensions.h"
#include "utils/PhysicalDevice.h"
#include "utils/QueueFamilyIndices.h"
#include "utils/SwapChainSupportDetails.h"
#include "../platform/vulkan/Context.h"

namespace Siege
{
class VulkanDevice
{
public:
    // 'Structors

    VulkanDevice();

    ~VulkanDevice();

    // Deleting move and copy constructors
    VulkanDevice(const VulkanDevice&) = delete;
    VulkanDevice& operator=(const VulkanDevice&) = delete;
    VulkanDevice(VulkanDevice&&) = delete;
    VulkanDevice& operator=(VulkanDevice&&) = delete;

    static VulkanDevice* GetDeviceInstance()
    {
        return vulkanDeviceInstance;
    }

    /**
     * Returns a copy of the command pool held by the device.
     * Command pools are objects that command buffer memory is typically allocated from.
     * Ensures that memory is not dynamically allocated every time we need a new command buffer.
     **/
    VkCommandPool GetCommandPool()
    {
        return context.GetLogicalDevice().GetCommandPool();
    }

    /**
     * Returns a copy of the logical device held by the VulkanDevice.
     * Logical devices represent an interface to a piece of hardware (typically the GPU).
     * Logical devices allow us to allocate memory and transition memory between the GPU
     * and CPU.
     **/
    VkDevice Device()
    {
        return context.GetLogicalDevice().GetDevice();
    }

    /**
     * Returns a copy of the window surface held by the VulkanDevice.
     * A window surface represents the actual window we wish to draw to. Vulkan can't
     * draw anything to a screen without an actual window to send vertex data to.
     **/
    VkSurfaceKHR Surface()
    {
        return Vulkan::Context::GetInstance().GetSurface();
    }

    /**
     * Returns a copy of the graphics queue held by the VulkanDevice.
     * A graphics queue is a queue where all command buffers which require a rendering step
     * are initially sent. This queue takes a command buffer and reads the rendering data
     * stored within. There can be multiple graphics queue (typically for multi-threaded)
     * draw calls.
     **/
    VkQueue GraphicsQueue()
    {
        return context.GetLogicalDevice().GetGraphicsQueue();
    }

    /**
     * Returns a copy of the present queue held by the VulkanDevice.
     * A present queue takes rendering data and provides it to the window surface. Present
     * queues are needed if data is to actually make it to the window at all.
     **/
    VkQueue PresentQueue()
    {
        return context.GetLogicalDevice().GetPresentQueue();
    }

    size_t GetDeviceAlignment();

    /**
     * Returns a struct containing all relevant swapChain support information.
     **/
    SwapChainSupportDetails::SwapChainSupportDetails GetSwapChainSupport();

    /**
     * Returns a bitmask value representing the memory typre required to allocate GPU memory.
     * Different rendering operations require different memory types. These are typically
     * differentiated using a bitmask. This functions throws an exception if the required
     * memory type is unavailable.
     * @param typeFilter - The bitmask for the required memory type
     * @param properties - The available device memory properties. Used to query for available
     *memory.
     **/
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    /**
     * Returns a struct containing the indices for our graphics and present queues.
     * Queues are stored in an array, each of which are differentiated by a specific index.
     * These indices act as identifiers which allow Vulkan to know which queue to submit something
     *to.
     **/
    QueueFamilyIndices::QueueFamilyIndices FindPhysicalQueueFamilies();

    /**
     * Returns a valid Vulkan image format from a list of formats. Returns the first format that's
     * found to be valid.
     * @param candidates - an array of acceptable formats.
     * @param formatCount - the number of elements in the candidates array.
     * @param tiling - the ideal image tiling required for the given format.
     * @param features - a bitmask of required format features. Each format has different features
     *associated with it. This ensures that we only get the features we want.
     **/
    VkFormat FindSupportedFormat(const VkFormat* candidates,
                                 size_t formatCount,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features);

    // Buffer Helper Functions

    /**
     * Prepares a command buffer for writing.
     * @returns - a command buffer that's been activated.
     **/
    VkCommandBuffer BeginSingleTimeCommands();

    /**
     * Stops a command buffer from beign written to (after having been written to already).
     * Submits the resulting command buffer to the graphics queue after all writing is
     * complete.
     **/
    void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    /**
     * Copies a buffer. This is generally used when copying the values within a buffer into
     * another (such as when setting up a staging buffer between the CPU and GPU).
     *
     * @param srcBuffer - the buffer being copied.
     * @param dstBuffer - the buffer where the data is being copied into.
     * @param size - the expected size of the resulting buffer.
     **/
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    /**
     *  Copies the a buffer into an image (useful for textures).
     *
     * 	@param buffer - the buffer being copied over
     *  @param image - the image housing the resulting data
     *  @param width - the size of the image
     *  @param height - the height of the image
     *  @param layerCount - the number of layers contained within the image (if any).
     **/
    void CopyBufferToImage(VkBuffer buffer,
                           VkImage image,
                           uint32_t width,
                           uint32_t height,
                           uint32_t layerCount);

    /**
     * Creates an image with memory allocation information
     *
     * @param imageInfo - the creation info for the image.
     * @param properties - the required memory properties.
     * @param image - the image where memory will be allocated to.
     * @param imageMemory - the image memory being allocated to.
     **/
    void CreateImageWithInfo(const VkImageCreateInfo& imageInfo,
                             VkMemoryPropertyFlags properties,
                             VkImage& image,
                             VkDeviceMemory& imageMemory);
private:
    static void SetVulkanDeviceInstance(VulkanDevice* device)
    {
        vulkanDeviceInstance = device;
    }

    static VulkanDevice* vulkanDeviceInstance;

    Vulkan::Context context;
};
} // namespace Siege

#endif
