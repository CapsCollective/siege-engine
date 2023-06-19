//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Fence.h"

#include "utils/TypeAdaptor.h"

#include <utils/Logging.h>

#include "Context.h"

namespace Siege::Vulkan
{
Fence::Fence(size_t fenceCount, FenceInitState initState) : size {fenceCount}, initState {initState}
{
    auto device = Context::GetVkLogicalDevice();

    VkFenceCreateInfo fenceInfo {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = (initState == FENCE_EMPTY_SIGNALED) * VK_FENCE_CREATE_SIGNALED_BIT;

    fences = new VkFence[fenceCount];

    for (size_t i = 0; i < fenceCount; i++)
    {
        if (initState == FENCE_NULL) fences[i] = VK_NULL_HANDLE;
        else
            CC_ASSERT(vkCreateFence(device, &fenceInfo, nullptr, OUT & fences[i]) == VK_SUCCESS,
                      "[FENCE] Failed to create Fence!")
    }
}

Fence::~Fence()
{
    if (fences == nullptr) return;

    auto device = Context::GetVkLogicalDevice();

    size = (initState == FENCE_EMPTY || initState == FENCE_EMPTY_SIGNALED) * size;

    for (size_t i = 0; i < size; i++)
    {
        vkDestroyFence(device, fences[i], nullptr);
    }
    delete[] fences;
    fences = VK_NULL_HANDLE;
    initState = FENCE_NULL;
    size = 0;
}

void Fence::Reset(const size_t idx)
{
    vkResetFences(Context::GetVkLogicalDevice(), 1, OUT & fences[idx]);
}

void Fence::Wait(const size_t idx)
{
    if (fences[idx] == VK_NULL_HANDLE) return;

    // If the image being asked for is being used, we wait for it to become available
    vkWaitForFences(Context::GetVkLogicalDevice(), 1, &fences[idx], VK_TRUE, UINT64_MAX);
}

void Fence::Swap(Fence& other)
{
    auto tmpFences = fences;
    auto tmpSize = size;
    auto tmpInitState = initState;

    fences = other.fences;
    size = other.size;
    initState = other.initState;

    other.fences = tmpFences;
    other.size = tmpSize;
    other.initState = tmpInitState;
}
} // namespace Siege::Vulkan