//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_DRAW_H
#define SIEGE_ENGINE_VULKAN_DRAW_H

#include <volk/volk.h>

namespace Siege::Vulkan::Utils
{
inline void Draw(VkCommandBuffer buffer,
                 unsigned int vertexCount,
                 unsigned int instances,
                 unsigned int firstVertex,
                 unsigned int firstInstance)
{
    vkCmdDraw(buffer, vertexCount, instances, firstVertex, firstInstance);
}

inline void DrawIndexed(VkCommandBuffer buffer,
                        unsigned int indexCount,
                        unsigned int instances,
                        unsigned int firstIndex,
                        unsigned int vertexOffset,
                        unsigned int firstInstance)
{
    vkCmdDrawIndexed(buffer, indexCount, instances, firstIndex, vertexOffset, firstInstance);
}
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_VULKAN_DRAW_H
