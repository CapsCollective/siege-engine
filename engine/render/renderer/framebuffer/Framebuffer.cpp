//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Framebuffer.h"

namespace Siege
{
u32 Framebuffer::IMAGE_COUNT = 0;

Framebuffer::Framebuffer(const Framebuffer::Config& config, const VkDevice& device)
{
    Initialise(config, device);
}

void Framebuffer::DestroyFramebuffer(VkDevice const& device)
{
    for (size_t i = 0; i < IMAGE_COUNT; i++) vkDestroyFramebuffer(device, framebuffers[i], nullptr);
}

void Framebuffer::Initialise(const Framebuffer::Config& config, const VkDevice& device)
{
    framebuffers = Utils::Array<VkFramebuffer>(IMAGE_COUNT);

    auto* colorAttachments = config.GetColorAttachments();
    auto* depthAttachments = config.GetDepthAttachments();

    for (size_t i = 0; i < IMAGE_COUNT; i++)
    {
        VkImageView attachments[config.GetAttachmentCount()];

        if (colorAttachments != nullptr) attachments[0] = colorAttachments->GetImageView(i);
        if (depthAttachments != nullptr) attachments[1] = depthAttachments->GetImageView(i);

        VkFramebufferCreateInfo frameBufferInfo {};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = config.GetRenderPass();
        frameBufferInfo.attachmentCount = config.GetAttachmentCount();
        frameBufferInfo.pAttachments = attachments;
        frameBufferInfo.width = config.GetWidth();
        frameBufferInfo.height = config.GetHeight();
        frameBufferInfo.layers = config.GetLayers();

        CC_ASSERT(vkCreateFramebuffer(device, &frameBufferInfo, nullptr, OUT & framebuffers[i]) ==
                      VK_SUCCESS,
                  "Failed to create framebuffer");
    }
}

// Config functions

Framebuffer::Config& Framebuffer::Config::WithRenderPass(VkRenderPass targetRenderPass)
{
    renderPass = targetRenderPass;
    return *this;
}

Framebuffer::Config& Framebuffer::Config::WithImageDimensions(u32 targetWidth, u32 targetHeight)
{
    width = targetWidth;
    height = targetHeight;
    return *this;
}

Framebuffer::Config& Framebuffer::Config::WithLayers(u32 targetLayers)
{
    layers = targetLayers;
    return *this;
}

Framebuffer::Config& Framebuffer::Config::WithDepthAttachments(FrameImages* targetAttachments)
{
    depthAttachments = targetAttachments;
    attachmentCount += 1;
    return *this;
}

Framebuffer::Config& Framebuffer::Config::WithColorAttachments(FrameImages* targetAttachments)
{
    colorAttachments = targetAttachments;
    attachmentCount += 1;
    return *this;
}

} // namespace Siege