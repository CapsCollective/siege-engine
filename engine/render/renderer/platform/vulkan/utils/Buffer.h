//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BUFFER_H
#define SIEGE_ENGINE_BUFFER_H

#include <volk/volk.h>

namespace Siege::Vulkan::Utils
{
class Buffer
{
public:

    static VkBufferCopy CopyRegion(VkDeviceSize size,
                                   VkDeviceSize srcOffset = 0,
                                   VkDeviceSize dstOffset = 0);

    static void CopyBuffer(VkCommandBuffer commandBuffer,
                           VkBuffer srcBuffer,
                           VkBuffer dstBuffer,
                           const VkBufferCopy& copyRegions,
                           uint32_t regionCount = 1);
};

} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_BUFFER_H
