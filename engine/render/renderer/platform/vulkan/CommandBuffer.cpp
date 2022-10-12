//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "CommandBuffer.h"
#include "Swapchain.h"
#include "Context.h"

#include "render/renderer/Renderer.h"

#include "utils/CommandBuffer.h"

namespace Siege::Vulkan
{
CommandBuffer::CommandBuffer(uint32_t count)
{
    auto device = Context::GetVkLogicalDevice();
    auto pool = Context::GetCurrentDevice()->GetCommandPool();

    count = ((count == 0) * Swapchain::MAX_FRAMES_IN_FLIGHT) + ((count != 0) * count);

    commandBuffers = Utils::CommandBuffer::AllocateCommandBuffers(device, pool, count);
}

void CommandBuffer::Begin(int32_t idx)
{
    auto bufferIndex = Renderer::Get()->GetCurrentFrameIndex();
}

void CommandBuffer::End(int32_t idx)
{

}
} // namespace Siege::Vulkan