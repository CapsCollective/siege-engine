//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "RenderPass.h"

#include <utils/Logging.h>

namespace Siege::Vulkan::Utils
{

VkRenderPass Create(VkDevice device,
                    VkAttachmentDescription* attachments,
                    unsigned int attachmentCount,
                    VkSubpassDescription* subpasses,
                    unsigned int subpassCount,
                    VkSubpassDependency* dependencies,
                    unsigned int dependencyCount)
{
    VkRenderPass renderPass {VK_NULL_HANDLE};

    VkRenderPassCreateInfo renderPassCreateInfo {};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = attachmentCount;
    renderPassCreateInfo.pAttachments = attachments;
    renderPassCreateInfo.subpassCount = subpassCount;
    renderPassCreateInfo.pSubpasses = subpasses;
    renderPassCreateInfo.dependencyCount = dependencyCount;
    renderPassCreateInfo.pDependencies = dependencies;

    CC_ASSERT(
        vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, OUT & renderPass) == VK_SUCCESS,
        "Failed to create render pass!")

    return renderPass;
}

void BeginRenderPass(VkCommandBuffer commandBuffer,
                     VkRenderPass renderPass,
                     VkClearValue* clearValues,
                     uint32_t clearValueCount,
                     VkFramebuffer framebuffer,
                     VkRect2D renderArea)
{
    VkRenderPassBeginInfo renderPassInfo {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;

    renderPassInfo.renderArea = renderArea;

    renderPassInfo.clearValueCount = clearValueCount;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(OUT commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void EndRenderPass(VkCommandBuffer commandBuffer)
{
    vkCmdEndRenderPass(OUT commandBuffer);
}

void Free(VkDevice device, VkRenderPass renderPass)
{
    vkDestroyRenderPass(device, renderPass, nullptr);
}

} // namespace Siege::Vulkan::Utils