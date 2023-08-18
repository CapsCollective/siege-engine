//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H
#define SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H

#include <utils/collections/HeapArray.h>

#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The CommandBuffer Wrapper class. This class represents either a single or a collection of
 * CommandBuffer objects. CommandBuffers allow us to record commands for each frame of execution
 */
class CommandBuffer
{
public:

    /**
     * Default constructor for the CommandBuffer. Initialises all values as empty
     */
    CommandBuffer() = default;

    /**
     *  Default destructor. Destroys all stored vulkan objects by default. Since CommandBuffers are
     *  destroyed as soon as the CommandPool is destroyed (CommandPool is stored on the
     * LogicalDevice)
     */
    ~CommandBuffer() = default;

    /**
     * Constructor for the CommandBuffer - Requires a count of the number of buffers to create
     * @param count the number of buffers to create
     */
    CommandBuffer(uint32_t count);
    /**
     * Move constructor for the CommandBuffer
     * @param other the CommandBuffer to move
     */
    inline CommandBuffer(CommandBuffer&& other)
    {
        Swap(other);
    }

    /**
     * Move assignment operator for CommandBuffers
     * @param other the CommandBuffer to move
     * @return A reference to the current CommandBuffer
     */
    inline CommandBuffer& operator=(CommandBuffer&& other)
    {
        Swap(other);
        return *this;
    }

    /**
     * Subscript operator into the stored command buffers
     * @param idx the index of the CommandBuffer to be accessed
     * @return the raw Vulkan Command Buffer
     */
    inline VkCommandBuffer& operator[](const size_t idx)
    {
        return commandBuffers[idx];
    }

    /**
     * Returns the raw Vulkan CommandBuffer pointer
     * @param index the index of the CommandBuffer to be accessed
     * @warn this function does not bounds check - please ensure that the provided index is in
     * bounds, otherwise undefined behaviour may occur
     * @return a raw Vulkan Command Buffer
     */
    VkCommandBuffer Get(const size_t index) const
    {
        return commandBuffers[index];
    }

    /**
     * Returns the raw Vulkan CommandBuffer pointer of the current active Command Buffer
     * @return the current active command buffer
     */
    VkCommandBuffer Get() const
    {
        return commandBuffers[currentActiveBufferIndex];
    }

    /**
     * Returns the active command buffer (the one being recorded)
     * @return the vulkan command buffer pointer
     */
    VkCommandBuffer GetActiveCommandBuffer()
    {
        return activeCommandBuffer;
    }

    /**
     * Starts recording a command buffer
     * @param index the index of the command buffer to be recorded. Defaults to 0
     */
    void Begin(int32_t index = 0);

    /**
     * Ends the recording of the active command buffer
     */
    void End();

    /**
     * Submits the specified command buffer to the graphics queue. Defaults to 0
     */
    void Submit(const size_t index = 0);

    /**
     * Submits a command to be recorded and immediately submitted to the graphics queue
     * @tparam F The function type being sent
     * @param func The function storing the one-time command
     */
    template<typename F>
    static void ExecuteSingleTimeCommand(F&& func)
    {
        auto commandBuffer = Vulkan::CommandBuffer(1);
        commandBuffer.Begin();
        func(commandBuffer.Get());
        commandBuffer.End();
        commandBuffer.Submit();
    }

    inline void SetActiveBufferIndex(uint32_t index)
    {
        currentActiveBufferIndex = index;
    }

private:

    /**
     * Swaps the contents of two CommandBuffers
     * @param other the CommandBuffer to swap contents with
     */
    void Swap(CommandBuffer& other);

    MHArray<VkCommandBuffer> commandBuffers;
    VkCommandBuffer activeCommandBuffer {nullptr};
    uint32_t currentActiveBufferIndex {0};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H
