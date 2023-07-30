//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COMMANDPOOL_H
#define SIEGE_ENGINE_COMMANDPOOL_H

#include <utils/Logging.h>

#include "volk/volk.h"

namespace Siege::Vulkan
{
class CommandPool
{
public:

    struct Builder
    {
        VkCommandPool Build(VkDevice device);
        Builder& WithQueueFamily(const uint32_t index);
        Builder& WithCallbacks(VkAllocationCallbacks* allocationCallbacks);
        Builder& WithFlag(VkCommandPoolCreateFlags createFlags);
        Builder& WithExtension(const void* next);

        uint32_t queueFamilyIndex;
        VkAllocationCallbacks* callbacks;
        VkCommandPoolCreateFlags flags = 0;
        const void* pNext = nullptr;
    };

    static VkCommandPoolCreateInfo CreateInfo(uint32_t queueFamilyIndex,
                                              VkCommandPoolCreateFlags flags = 0,
                                              const void* pNext = nullptr);

    static void Create(VkDevice device,
                       VkCommandPoolCreateInfo info,
                       VkAllocationCallbacks* callbacks,
                       VkCommandPool* pool);
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_COMMANDPOOL_H
