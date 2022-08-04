//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Buffer.h"

namespace Siege::Vulkan::Utils
{
VkBufferCopy Buffer::CopyRegion(VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset)
{
    return {srcOffset, dstOffset, size};
}

void Buffer::CopyBuffer(VkCommandBuffer commandBuffer,
                        VkBuffer srcBuffer,
                        VkBuffer dstBuffer,
                        const VkBufferCopy& copyRegions,
                        uint32_t regionCount)
{
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, &copyRegions);
}
} // namespace Siege::Vulkan::Utils