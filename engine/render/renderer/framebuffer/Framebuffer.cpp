//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Framebuffer.h"

#include <utils/Logging.h>

#include "render/renderer/platform/vulkan/Context.h"

namespace Siege
{
uint32_t Framebuffer::IMAGE_COUNT = 0;

Framebuffer::Framebuffer(const Framebuffer::Config& config, const VkDevice& device)
{
    Initialise(config, device);
}

void Framebuffer::DestroyFramebuffer(VkDevice const& device)
{
    for (auto it = framebuffers.CreateIterator(); it; ++it)
        vkDestroyFramebuffer(device, *it, nullptr);
}

void Framebuffer::Initialise(const Framebuffer::Config& config, const VkDevice& device)
{
    framebuffers = MHArray<VkFramebuffer>(IMAGE_COUNT);

    auto* colorAttachments = config.GetColorAttachments();
    auto* depthAttachments = config.GetDepthAttachments();

    for (size_t i = 0; i < IMAGE_COUNT; i++)
    {
        VkImageView attachments[config.GetAttachmentCount()];

        if (colorAttachments != nullptr) attachments[0] = colorAttachments->GetVkImage(i).GetView();
        if (depthAttachments != nullptr) attachments[1] = depthAttachments->GetVkImage(i).GetView();

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
                  "Failed to create framebuffer")
    }
}

Framebuffer& Framebuffer::operator=(Framebuffer&& other) noexcept
{
    Swap(other);
    return *this;
}

Framebuffer::~Framebuffer()
{
    DestroyFramebuffer(Vulkan::Context::GetVkLogicalDevice());
    framebuffers.Clear();
}

// Config functions

Framebuffer::Config& Framebuffer::Config::WithRenderPass(VkRenderPass targetRenderPass)
{
    renderPass = targetRenderPass;
    return *this;
}

Framebuffer::Config& Framebuffer::Config::WithImageDimensions(uint32_t targetWidth,
                                                              uint32_t targetHeight)
{
    width = targetWidth;
    height = targetHeight;
    return *this;
}

Framebuffer::Config& Framebuffer::Config::WithLayers(uint32_t targetLayers)
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

void Framebuffer::Swap(Framebuffer& other)
{
    auto tmpFrameBuffers = framebuffers;

    framebuffers = other.framebuffers;

    other.framebuffers = tmpFrameBuffers;
}

} // namespace Siege
