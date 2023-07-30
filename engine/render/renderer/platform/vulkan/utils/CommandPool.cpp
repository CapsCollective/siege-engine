//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "CommandPool.h"

namespace Siege::Vulkan
{
VkCommandPoolCreateInfo CommandPool::CreateInfo(uint32_t queueFamilyIndex,
                                                VkCommandPoolCreateFlags flags,
                                                const void* pNext)
{
    return {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, pNext, flags, queueFamilyIndex};
}

void CommandPool::Create(VkDevice device,
                         VkCommandPoolCreateInfo info,
                         VkAllocationCallbacks* callbacks,
                         VkCommandPool* pool) {
    CC_ASSERT(vkCreateCommandPool(device, &info, callbacks, OUT pool) == VK_SUCCESS,
              "Failed to create command pool!")}

// Builder

VkCommandPool CommandPool::Builder::Build(VkDevice device)
{
    VkCommandPoolCreateInfo info = CreateInfo(queueFamilyIndex, flags, pNext);
    VkCommandPool pool {};
    Create(device, info, callbacks, &pool);
    return pool;
}

CommandPool::Builder& CommandPool::Builder::WithQueueFamily(const uint32_t index)
{
    queueFamilyIndex = index;
    return *this;
}

CommandPool::Builder& CommandPool::Builder::WithCallbacks(
    VkAllocationCallbacks* allocationCallbacks)
{
    callbacks = allocationCallbacks;
    return *this;
}

CommandPool::Builder& CommandPool::Builder::WithFlag(VkCommandPoolCreateFlags createFlags)
{
    flags |= createFlags;
    return *this;
}

CommandPool::Builder& CommandPool::Builder::WithExtension(const void* next)
{
    pNext = next;
    return *this;
}
} // namespace Siege::Vulkan