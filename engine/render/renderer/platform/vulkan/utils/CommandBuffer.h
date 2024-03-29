//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COMMANDBUFFER_H
#define SIEGE_ENGINE_COMMANDBUFFER_H

#include <utils/collections/HeapArray.h>
#include <volk/volk.h>

namespace Siege::Vulkan::Utils
{
class CommandBuffer
{
public:

    static VkCommandBuffer AllocateCommandBuffer(VkDevice device, VkCommandPool pool);
    static MHArray<VkCommandBuffer> AllocateCommandBuffers(VkDevice, VkCommandPool, uint32_t);
    static void BeginSingleTimeCommand(VkCommandBuffer buffer);
    static void EndCommandBuffer(VkCommandBuffer commandBuffer);
};
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_COMMANDBUFFER_H
