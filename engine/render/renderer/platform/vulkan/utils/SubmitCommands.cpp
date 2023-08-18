//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SubmitCommands.h"

#include <utils/Logging.h>

#include "TypeAdaptor.h"

namespace Siege::Vulkan::Utils
{

SubmitGraphicsCommand& SubmitGraphicsCommand::ToQueue(VkQueue targetQueue)
{
    queue = targetQueue;
    return *this;
}

SubmitGraphicsCommand& SubmitGraphicsCommand::WaitOnSemaphores(
    std::initializer_list<VkSemaphore> list)
{
    for (auto& element : list) waitSemaphores.Append(element);
    return *this;
}

SubmitGraphicsCommand& SubmitGraphicsCommand::WithCommandBuffers(
    std::initializer_list<VkCommandBuffer> list)
{
    for (auto& element : list) commandBuffers.Append(element);
    return *this;
}

SubmitGraphicsCommand& SubmitGraphicsCommand::SignalSemaphores(
    std::initializer_list<VkSemaphore> list)
{
    for (auto& element : list) signalSemaphores.Append(element);
    return *this;
}

SubmitGraphicsCommand& SubmitGraphicsCommand::WithPipelineStages(
    std::initializer_list<VkPipelineStageFlags> list)
{
    for (auto& element : list) waitDstStageMask.Append(element);
    return *this;
}

void SubmitGraphicsCommand::Submit(VkFence inFlightFence)
{
    VkSubmitInfo submitInfo {};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.waitSemaphoreCount = waitSemaphores.Count();
    submitInfo.pWaitSemaphores = waitSemaphores.Data();

    submitInfo.pWaitDstStageMask = waitDstStageMask.Data();

    submitInfo.commandBufferCount = commandBuffers.Count();
    submitInfo.pCommandBuffers = commandBuffers.Data();

    submitInfo.signalSemaphoreCount = signalSemaphores.Count();
    submitInfo.pSignalSemaphores = signalSemaphores.Data();

    CC_ASSERT(vkQueueSubmit(queue, 1, &submitInfo, OUT inFlightFence) == VK_SUCCESS,
              "Failed to submit draw command buffer");
}

SubmitPresentCommand& SubmitPresentCommand::ToQueue(VkQueue targetQueue)
{
    queue = targetQueue;
    return *this;
}

SubmitPresentCommand& SubmitPresentCommand::SignalSemaphores(
    std::initializer_list<VkSemaphore> list)
{
    for (auto& element : list) signalSemaphores.Append(element);
    return *this;
}

SubmitPresentCommand& SubmitPresentCommand::ForSwapchains(
    std::initializer_list<VkSwapchainKHR> list)
{
    for (auto& element : list) swapchains.Append(element);
    return *this;
}

SubmitPresentCommand& SubmitPresentCommand::WithImageIndices(std::initializer_list<uint32_t> list)
{
    for (auto& element : list) indices.Append(element);
    return *this;
}

Utils::Result SubmitPresentCommand::Submit()
{
    VkPresentInfoKHR presentInfo {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = signalSemaphores.Count();
    presentInfo.pWaitSemaphores = signalSemaphores.Data();

    presentInfo.swapchainCount = swapchains.Count();
    presentInfo.pSwapchains = swapchains.Data();

    // Store the image we want to render
    presentInfo.pImageIndices = indices.Data();

    auto result = vkQueuePresentKHR(queue, &presentInfo);

    CC_ASSERT(
        result == VK_SUCCESS || result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR,
        "Failed to submit to present queue!")

    return static_cast<Utils::Result>(result == VK_ERROR_OUT_OF_DATE_KHR ||
                                      result == VK_SUBOPTIMAL_KHR);
}

} // namespace Siege::Vulkan::Utils
