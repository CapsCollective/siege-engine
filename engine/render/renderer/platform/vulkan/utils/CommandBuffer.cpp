//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "CommandBuffer.h"

#include <utils/Macros.h>

namespace Siege::Vulkan::CommandBuffer
{
VkCommandBufferAllocateInfo AllocInfo(VkCommandPool commandPool,
                                      VkCommandBufferLevel level,
                                      uint32_t count,
                                      void* pNext)
{
    return {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, pNext, commandPool, level, count};
}

VkCommandBuffer Allocate(VkDevice device, VkCommandBufferAllocateInfo allocInfo)
{
    VkCommandBuffer buffer;
    vkAllocateCommandBuffers(device, &allocInfo, OUT & buffer);
    return buffer;
}

void BeginCommand(VkCommandBuffer commandBuffer,
                  VkCommandBufferUsageFlagBits flags,
                  void* pNext,
                  VkCommandBufferInheritanceInfo* inheritanceInfo)
{
    VkCommandBufferBeginInfo beginInfo {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                                        pNext,
                                        flags,
                                        inheritanceInfo};

    vkBeginCommandBuffer(OUT commandBuffer, &beginInfo);
}

} // namespace Siege::Vulkan::CommandBuffer