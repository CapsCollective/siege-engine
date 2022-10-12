//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H
#define SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H

#include "utils/Types.h"

#include <utils/collections/HeapArray.h>

namespace Siege::Vulkan
{
class CommandBuffer
{
public:
    CommandBuffer(uint32_t count = 0);
    ~CommandBuffer() = default;

    void Begin(int32_t idx = -1);
    void End(int32_t idx = -1);
private:
    ::Siege::Utils::MHArray<VkCommandBuffer> commandBuffers;
    VkCommandBuffer activeCommandBuffer {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_COMMANDBUFFER_H
