//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "CommandBuffer.h"

#include <utils/Logging.h>

namespace Siege::Vulkan::Utils
{
VkCommandBuffer CommandBuffer::AllocateCommandBuffer(VkDevice device, VkCommandPool pool)
{
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = pool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;

    CC_ASSERT(vkAllocateCommandBuffers(device, &allocInfo, OUT & commandBuffer) == VK_SUCCESS,
              "Failed to allocate command buffer!")

    return commandBuffer;
}

void CommandBuffer::BeginSingleTimeCommand(VkCommandBuffer buffer)
{
    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    CC_ASSERT(vkBeginCommandBuffer(OUT buffer, &beginInfo) == VK_SUCCESS,
              "Failed to begin recording command buffer!")
}
} // namespace Siege::Vulkan::Utils