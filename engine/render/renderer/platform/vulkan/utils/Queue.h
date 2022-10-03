//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_QUEUE_H
#define SIEGE_ENGINE_QUEUE_H

#include "Types.h"

#include <volk/volk.h>
#include <utils/collections/StackArray.h>

namespace Siege::Vulkan::Utils
{
struct SubmitGraphicsCommand
{
    static inline constexpr size_t MAX_SEMAPHORES = 10;
    static inline constexpr size_t MAX_COMMAND_BUFFERS = 10;

    SubmitGraphicsCommand& ToQueue(VkQueue targetQueue);
    SubmitGraphicsCommand& WaitOnSemaphores(std::initializer_list<VkSemaphore> list);
    SubmitGraphicsCommand& WithCommandBuffers(std::initializer_list<VkCommandBuffer> list);
    SubmitGraphicsCommand& SignalSemaphores(std::initializer_list<VkSemaphore> list);
    SubmitGraphicsCommand& WithPipelineStages(std::initializer_list<VkPipelineStageFlags> list);
    void Submit(VkFence inFlightFence);

    VkQueue queue {VK_NULL_HANDLE};
    ::Siege::Utils::MSArray<VkSemaphore, MAX_SEMAPHORES> waitSemaphores;
    ::Siege::Utils::MSArray<VkCommandBuffer, MAX_COMMAND_BUFFERS> commandBuffers;
    ::Siege::Utils::MSArray<VkSemaphore, MAX_SEMAPHORES> signalSemaphores;
    ::Siege::Utils::MSArray<VkPipelineStageFlags, MAX_SEMAPHORES> waitDstStageMask;
};

struct SubmitPresentCommand
{
    static inline constexpr size_t MAX_SEMAPHORES = 10;
    static inline constexpr size_t MAX_SWAPCHAINS = 10;
    static inline constexpr size_t MAX_INDICES = 10;

    SubmitPresentCommand& ToQueue(VkQueue targetQueue);
    SubmitPresentCommand& SignalSemaphores(std::initializer_list<VkSemaphore> list);
    SubmitPresentCommand& ForSwapchains(std::initializer_list<VkSwapchainKHR> list);
    SubmitPresentCommand& WithImageIndices(std::initializer_list<uint32_t> list);
    Utils::Result Submit();

    VkQueue queue {VK_NULL_HANDLE};
    ::Siege::Utils::MSArray<VkSemaphore, MAX_SEMAPHORES> signalSemaphores;
    ::Siege::Utils::MSArray<VkSwapchainKHR, MAX_SWAPCHAINS> swapchains;
    ::Siege::Utils::MSArray<uint32_t, MAX_INDICES> indices;
};
} // namespace Siege

#endif // SIEGE_ENGINE_QUEUE_H
