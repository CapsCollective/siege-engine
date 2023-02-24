//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "CommandBuffer.h"

#include "Context.h"
#include "utils/CommandBuffer.h"
#include "utils/SubmitCommands.h"

namespace Siege::Vulkan
{
CommandBuffer::CommandBuffer(uint32_t count)
{
    auto device = Context::GetVkLogicalDevice();
    auto pool = Context::GetCurrentDevice()->GetCommandPool();

    commandBuffers = Utils::CommandBuffer::AllocateCommandBuffers(device, pool, count);
}

void CommandBuffer::Begin(int32_t index)
{
    auto commandBuffer = commandBuffers[index];

    Utils::CommandBuffer::BeginSingleTimeCommand(commandBuffer);

    activeCommandBuffer = commandBuffer;
}

void CommandBuffer::End()
{
    Utils::CommandBuffer::EndCommandBuffer(activeCommandBuffer);

    activeCommandBuffer = nullptr;
}

void CommandBuffer::Submit(const size_t index)
{
    Vulkan::Fence fence(1, Fence::FENCE_EMPTY);

    Utils::SubmitGraphicsCommand()
        .ToQueue(Vulkan::Context::GetCurrentDevice()->GetGraphicsQueue())
        .WithCommandBuffers({commandBuffers[index]})
        .Submit(fence.Get());

    fence.Wait();
}

void CommandBuffer::Swap(CommandBuffer& other)
{
    auto tmpCmdBuffers = std::move(commandBuffers);
    auto tmpActiveCmdBuffer = activeCommandBuffer;
    auto tmpCurrentActiveBufferIndex = currentActiveBufferIndex;

    commandBuffers = std::move(other.commandBuffers);
    activeCommandBuffer = other.activeCommandBuffer;
    currentActiveBufferIndex = other.currentActiveBufferIndex;

    other.commandBuffers = std::move(tmpCmdBuffers);
    other.activeCommandBuffer = tmpActiveCmdBuffer;
    other.currentActiveBufferIndex = tmpCurrentActiveBufferIndex;
}
} // namespace Siege::Vulkan
