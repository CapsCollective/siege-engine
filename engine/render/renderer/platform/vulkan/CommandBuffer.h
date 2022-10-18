//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H
#define SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H

#include "utils/Types.h"

#include <utils/collections/HeapArray.h>

namespace Siege::Vulkan
{
/**
 *
 */
class CommandBuffer
{
    typedef void (*cmdFunc)();
public:
    /**
     *
     */
    CommandBuffer() = default;
    /**
     *
     */
    ~CommandBuffer() = default;

    /**
     *
     * @param count
     */
    CommandBuffer(uint32_t count);
    /**
     *
     * @param other
     */
    inline CommandBuffer(CommandBuffer&& other) { Swap(other); }

    /**
     *
     * @param other
     * @return
     */
    inline CommandBuffer& operator=(CommandBuffer&& other)
    {
        Swap(other);
        return *this;
    }
    /**
     *
     * @param idx
     * @return
     */
    inline VkCommandBuffer& operator[](const size_t idx) { return commandBuffers[idx]; }

    /**
     *
     * @param index
     * @return
     */
    VkCommandBuffer Get(const size_t index = 0) { return commandBuffers[index]; }
    /**
     *
     * @return
     */
    VkCommandBuffer GetActiveCommandBuffer() { return activeCommandBuffer; }

    /**
     *
     * @param index
     */
    void Begin(int32_t index = 0);
    /**
     *
     */
    void End();

    /**
     *
     */
    void Submit(const size_t index = 0);

    template<typename F>
    static void ExecuteSingleTimeCommand(F&& func)
    {
        auto commandBuffer = Vulkan::CommandBuffer(1);
        commandBuffer.Begin();
        func(commandBuffer.Get());
        commandBuffer.End();
        commandBuffer.Submit();
    }
private:
    /**
     *
     * @param other
     */
    void Swap(CommandBuffer& other);
    /**
     *
     */
    ::Siege::Utils::MHArray<VkCommandBuffer> commandBuffers;
    /**
     *
     */
    VkCommandBuffer activeCommandBuffer {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H
