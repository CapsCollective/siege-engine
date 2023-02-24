//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "RenderPass.h"

#include <utils/Logging.h>

#include "render/renderer/platform/vulkan/Context.h"

namespace Siege
{

RenderPass::~RenderPass()
{
    DestroyRenderPass();
}

void RenderPass::DestroyRenderPass()
{
    auto device = Vulkan::Context::GetVkLogicalDevice();
    if (device)
    {
        vkDestroyRenderPass(device, renderPass, nullptr);
        renderPass = VK_NULL_HANDLE;
    }
}

void RenderPass::Begin(VkRenderPass renderPass,
                       Vulkan::CommandBuffer& commandBuffer,
                       VkFramebuffer frameBuffer,
                       VkOffset2D offset,
                       VkExtent2D extent,
                       const VkClearValue* clearValues,
                       uint32_t clearValueCount)
{
    VkRenderPassBeginInfo renderPassInfo {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = frameBuffer;

    renderPassInfo.renderArea.offset = offset;
    renderPassInfo.renderArea.extent = extent;

    renderPassInfo.clearValueCount = clearValueCount;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(OUT commandBuffer.GetActiveCommandBuffer(),
                         &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);
}

void RenderPass::End(Vulkan::CommandBuffer& commandBuffer)
{
    vkCmdEndRenderPass(OUT commandBuffer.GetActiveCommandBuffer());
}

RenderPass::RenderPass() = default;

void RenderPass::Initialise(const RenderPass::Config& config)
{
    auto& attachments = config.GetAttachments();
    auto& subPasses = config.GetSubPasses();
    auto& dependencies = config.GetDependencies();

    VkRenderPassCreateInfo renderPassCreateInfo {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = attachments.Count();
    renderPassCreateInfo.pAttachments = attachments.Data();
    renderPassCreateInfo.subpassCount = subPasses.Count();
    renderPassCreateInfo.pSubpasses = subPasses.Data();
    renderPassCreateInfo.dependencyCount = dependencies.Count();
    renderPassCreateInfo.pDependencies = dependencies.Data();

    CC_ASSERT(vkCreateRenderPass(Vulkan::Context::GetVkLogicalDevice(),
                                 &renderPassCreateInfo,
                                 nullptr,
                                 OUT & renderPass) == VK_SUCCESS,
              "Failed to create render pass!");
}

void RenderPass::Initialise(RenderPass& renderpass, const RenderPass::Config& config)
{
    renderpass.Initialise(config);
}

// Config functions.

RenderPass::Config& RenderPass::Config::WithAttachment(const VkAttachmentDescription& attachment)
{
    attachments.Append(attachment);
    return *this;
}

RenderPass::Config& RenderPass::Config::WithSubPass(const VkSubpassDescription& subpass)
{
    subpasses.Append(subpass);
    return *this;
}

RenderPass::Config& RenderPass::Config::WithDependency(const VkSubpassDependency& dependency)
{
    dependencies.Append(dependency);
    return *this;
}

void RenderPass::Swap(RenderPass& other)
{
    auto tmpRenderPass = renderPass;

    renderPass = other.renderPass;

    other.renderPass = tmpRenderPass;
}
} // namespace Siege
