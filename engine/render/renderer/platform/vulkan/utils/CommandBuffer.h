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

namespace Siege::Vulkan::Utils
{
class CommandBuffer
{
public:

    static VkCommandBuffer AllocateCommandBuffer(VkDevice device, VkCommandPool pool);
    static void BeginSingleTimeCommand(VkCommandBuffer buffer);
};
} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_COMMANDBUFFER_H
