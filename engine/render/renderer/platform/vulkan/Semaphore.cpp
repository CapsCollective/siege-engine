//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Semaphore.h"

#include <utils/Logging.h>

#include "Context.h"

namespace Siege::Vulkan
{
Semaphore::Semaphore(size_t semaphoreCount) : size {semaphoreCount}
{
    auto device = Context::GetVkLogicalDevice();

    VkSemaphoreCreateInfo semaphoreInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    semaphores = new VkSemaphore[semaphoreCount];

    for (size_t i = 0; i < semaphoreCount; i++)
    {
        CC_ASSERT(
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, OUT & semaphores[i]) == VK_SUCCESS,
            "[SEMAPHORE] Failed to create Semaphore!")
    }
}

Semaphore::~Semaphore()
{
    if (semaphores == nullptr) return;

    CC_LOG_INFO("[SEMAPHORE] Destroying Semaphore")

    auto device = Context::GetVkLogicalDevice();
    for (size_t i = 0; i < size; i++) vkDestroySemaphore(device, semaphores[i], nullptr);

    delete[] semaphores;
    semaphores = VK_NULL_HANDLE;
    size = 0;
}

void Semaphore::Swap(Semaphore& other)
{
    auto tmpSemaphores = semaphores;
    auto tmpSize = size;

    semaphores = other.semaphores;
    size = other.size;

    other.semaphores = tmpSemaphores;
    other.size = tmpSize;
}
} // namespace Siege::Vulkan