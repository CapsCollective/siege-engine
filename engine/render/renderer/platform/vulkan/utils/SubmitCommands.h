//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SUBMIT_COMMANDS_H
#define SIEGE_ENGINE_SUBMIT_COMMANDS_H

#include <utils/collections/StackArray.h>
#include <volk/volk.h>

#include "Types.h"

namespace Siege::Vulkan::Utils
{

/**
 * Specifies the number of acceptable semaphores which can be submitted for signalling at any point
 * in time
 */
static inline constexpr size_t MAX_SEMAPHORES = 10;

/**
 * The SubmitGraphicsCommand type handles all requirements for submitting draw commands to
 * the GPU's graphics queue. The purpose is to act as a readable storage container for submitting
 * graphics commands to the GPU
 */
struct SubmitGraphicsCommand
{
    /**
     * The number of command buffers which can be submitted for any single command submission
     */
    static inline constexpr size_t MAX_COMMAND_BUFFERS = 10;

    /**
     * Specifies the Queue to submit to
     * @param targetQueue the queue to submit commands to
     * @return A reference to the command builder
     */
    SubmitGraphicsCommand& ToQueue(VkQueue targetQueue);

    /**
     * Specifies which semaphores to wait on before submitting the commands
     * @param list a list of semaphores to wait on before submitting the commands
     * @return A reference to the command builder
     */
    SubmitGraphicsCommand& WaitOnSemaphores(std::initializer_list<VkSemaphore> list);

    /**
     * Specifies which command buffers to submit to the graphics queue
     * @param list a list of all command buffers to use for drawing
     * @return A reference to the command builder
     */
    SubmitGraphicsCommand& WithCommandBuffers(std::initializer_list<VkCommandBuffer> list);

    /**
     * Specifies which semaphores to signal once the submission has completed
     * @param list a list of semaphores to signal once submission is complete
     * @return A reference to the command builder
     */
    SubmitGraphicsCommand& SignalSemaphores(std::initializer_list<VkSemaphore> list);

    /**
     * Specifies which pipeline stages the command applies to (vertex, fragment... etc)
     * @param list a list of the pipeline stages the command will affect
     * @return A reference to the command builder
     */
    SubmitGraphicsCommand& WithPipelineStages(std::initializer_list<VkPipelineStageFlags> list);

    /**
     * Submits the command to the graphics queue
     * @param inFlightFence the per-frame fence the command should be submitted to
     */
    void Submit(VkFence inFlightFence);

    VkQueue queue {VK_NULL_HANDLE};
    ::Siege::Utils::MSArray<VkSemaphore, MAX_SEMAPHORES> waitSemaphores;
    ::Siege::Utils::MSArray<VkCommandBuffer, MAX_COMMAND_BUFFERS> commandBuffers;
    ::Siege::Utils::MSArray<VkSemaphore, MAX_SEMAPHORES> signalSemaphores;
    ::Siege::Utils::MSArray<VkPipelineStageFlags, MAX_SEMAPHORES> waitDstStageMask;
};

/**
 * The SubmitPresentCommand type handles all requirements for submitting commands to the present
 * queue (the present queue actually handles the drawing of images to the screen). It acts as a
 * storage container for all data required to submit commands to the present queue.
 */
struct SubmitPresentCommand
{
    /**
     * The maximum number of swapchains to use for submission
     */
    static inline constexpr size_t MAX_SWAPCHAINS = 10;
    /**
     * The maximum number of images (represented by indices) which can be submitted at once
     */
    static inline constexpr size_t MAX_INDICES = 10;

    /**
     * Specifies the Queue to submit to
     * @param targetQueue the queue to submit commands to
     * @return A reference to the command builder
     */
    SubmitPresentCommand& ToQueue(VkQueue targetQueue);

    /**
     * Specifies which semaphores to signal once the submission has completed
     * @param list a list of semaphores to signal once submission is complete
     * @return A reference to the command builder
     */
    SubmitPresentCommand& SignalSemaphores(std::initializer_list<VkSemaphore> list);

    /**
     * Specifies which swapchains the command will apply to
     * @param list a list of swapchains to use for submission
     * @return A reference to the command builder
     */
    SubmitPresentCommand& ForSwapchains(std::initializer_list<VkSwapchainKHR> list);

    /**
     * Specifies which images to use for submission
     * @param list a list of images to be used for presentation
     * @return A reference to the command builder
     */
    SubmitPresentCommand& WithImageIndices(std::initializer_list<uint32_t> list);

    /**
     * @brief Submits the command to the present queue
     * @return the result of the submission. Can return the following:\n
     * SUCCESS - The submission was successful\n
     * SUBOPTIMAL - The swapchain no longer matches the surface exactly (this can happen when a
     * window is resized)\n OUT_OF_DATE - A surface has changed in such a way that the swapchain is
     * no longer compatible with it\n RESIZED - The window has been resized and we must rebuild our
     * swapchain to match the new extents\n
     */
    Utils::Result Submit();

    VkQueue queue {VK_NULL_HANDLE};
    ::Siege::Utils::MSArray<VkSemaphore, MAX_SEMAPHORES> signalSemaphores;
    ::Siege::Utils::MSArray<VkSwapchainKHR, MAX_SWAPCHAINS> swapchains;
    ::Siege::Utils::MSArray<uint32_t, MAX_INDICES> indices;
};
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_SUBMIT_COMMANDS_H
