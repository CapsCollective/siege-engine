//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_COMMANDBUFFER_H
#define SIEGE_ENGINE_COMMANDBUFFER_H

#include <volk/volk.h>

namespace Siege::Vulkan::CommandBuffer
{

VkCommandBufferAllocateInfo AllocInfo(VkCommandPool commandPool,
                                      VkCommandBufferLevel level,
                                      uint32_t count,
                                      void* pNext = nullptr);

VkCommandBuffer Allocate(VkDevice device, VkCommandBufferAllocateInfo allocInfo);

void BeginCommand(VkCommandBuffer commandBuffer,
                  VkCommandBufferUsageFlagBits flags,
                  void* pNext = nullptr,
                  VkCommandBufferInheritanceInfo* inheritanceInfo = nullptr);

inline void EndCommand(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);
}

inline void DestroyCommandBuffer(VkDevice device,
                                 VkCommandPool pool,
                                 uint32_t amount,
                                 VkCommandBuffer commandBuffer)
{
    vkFreeCommandBuffers(device, pool, amount, &commandBuffer);
}

} // namespace Siege::Vulkan::CommandBuffer

#endif // SIEGE_ENGINE_COMMANDBUFFER_H
