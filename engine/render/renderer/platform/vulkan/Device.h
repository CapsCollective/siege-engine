//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#pragma once

#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "VulkanInstance.h"
#include "render/renderer/Core.h"
#include "render/window/Window.h"

namespace Siege
{

class Device
{
public:

    // 'Structors

    Device(Siege::Window* window);
    Device();

    ~Device() = default;

    // Deleting move and copy constructors
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    Device(Device&&);
    Device& operator=(Device&&);

    static Device* GetDeviceInstance()
    {
        return deviceInstance;
    }

    /**
     * Returns a copy of the command pool held by the device.
     * Command pools are objects that command buffer memory is typically allocated from.
     * Ensures that memory is not dynamically allocated every time we need a new command buffer.
     **/
    VkCommandPool GetCommandPool()
    {
        return logicalDevice.CommandPool();
    }

    /**
     * Returns a copy of the logical device held by the VulkanDevice.
     * Logical devices represent an interface to a piece of hardware (typically the GPU).
     * Logical devices allow us to allocate memory and transition memory between the GPU
     * and CPU.
     **/
    VkDevice LogicalDevice()
    {
        return logicalDevice.Device();
    }

    Vulkan::PhysicalDevice PhysicalDevice()
    {
        return physicalDevice;
    }

    VkPhysicalDevice VulkanPhysicalDevice()
    {
        return physicalDevice.Device();
    }

    /**
     * Returns a copy of the window surface held by the VulkanDevice.
     * A window surface represents the actual window we wish to draw to. Vulkan can't
     * draw anything to a screen without an actual window to send vertex data to.
     **/
    VkSurfaceKHR Surface()
    {
        return instance.Surface();
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
        return logicalDevice.GraphicsQueue();
    }

    /**
     * Returns a copy of the present queue held by the VulkanDevice.
     * A present queue takes rendering data and provides it to the window surface. Present
     * queues are needed if data is to actually make it to the window at all.
     **/
    VkQueue PresentQueue()
    {
        return logicalDevice.PresentQueue();
    }

    size_t GetDeviceAlignment()
    {
        return physicalDevice.Properties().limits.minUniformBufferOffsetAlignment;
    }

    /**
     * Returns a valid Vulkan image format from a list of formats. Returns the first format that's
     * found to be valid.
     * @param candidates - an array of acceptable formats.
     * @param formatCount - the number of elements in the candidates array.
     * @param tiling - the ideal image tiling required for the given format.
     * @param features - a bitmask of required format features. Each format has different features
     *associated with it. This ensures that we only get the features we want.
     **/
    inline VkFormat FindSupportedFormat(const VkFormat* candidates,
                                        size_t formatCount,
                                        VkImageTiling tiling,
                                        VkFormatFeatureFlags features)
    {
        return physicalDevice.FindSupportedFormat(candidates, formatCount, tiling, features);
    }

    // Buffer Helper Functions

    /**
     * Prepares a command buffer for writing.
     * @returns - a command buffer that's been activated.
     **/
    inline VkCommandBuffer BeginSingleTimeCommands()
    {
        return logicalDevice.BeginSingleTimeCommands();
    }

    /**
     * Stops a command buffer from beign written to (after having been written to already).
     * Submits the resulting command buffer to the graphics queue after all writing is
     * complete.
     **/
    inline void EndSingleTimeCommands(VkCommandBuffer commandBuffer)
    {
        logicalDevice.EndSingleTimeCommands(commandBuffer);
    }

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

    void Move(Device& other);

    static void SetDeviceInstance(Device* device)
    {
        deviceInstance = device;
    }

    static Device* deviceInstance;

    Vulkan::VulkanInstance instance;
    Vulkan::PhysicalDevice physicalDevice;
    Vulkan::LogicalDevice logicalDevice;
};

} // namespace Siege
