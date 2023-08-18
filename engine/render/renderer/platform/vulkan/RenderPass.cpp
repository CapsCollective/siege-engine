//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "RenderPass.h"

#include "utils/RenderPass.h"
#include "utils/TypeAdaptor.h"

namespace Siege::Vulkan
{

RenderPass::Builder& RenderPass::Builder::WithAttachments(unsigned int attachmentCount)
{
    attachments = MHArray<Attachment>(attachmentCount);
    return *this;
}

RenderPass::Builder& RenderPass::Builder::WithSubpasses(unsigned int subpassCount)
{
    subpasses = MHArray<Subpass>(subpassCount);
    return *this;
}

RenderPass::Builder& RenderPass::Builder::WithDependencies(unsigned int dependencyCount)
{
    dependencies = MHArray<Dependency>(dependencyCount);
    return *this;
}

RenderPass::Builder& RenderPass::Builder::WithColourAttachment(Utils::ImageFormat layout)
{
    attachments.Append({ATTACHMENT_COLOUR, layout});
    return *this;
}

RenderPass::Builder& RenderPass::Builder::WithDepthAttachment(Utils::DepthFormat layout)
{
    attachments.Append({ATTACHMENT_DEPTH, (Utils::ImageFormat) layout});
    return *this;
}

RenderPass::Builder& RenderPass::Builder::WithSubpass(MHArray<unsigned int> colourAttachments,
                                                      unsigned int depthAttachment)
{
    subpasses.Append({depthAttachment, colourAttachments});
    return *this;
}

RenderPass::Builder& RenderPass::Builder::WithDependency(unsigned int src,
                                                         unsigned int dst,
                                                         unsigned int srcStageMask,
                                                         unsigned int dstStageMask,
                                                         unsigned int srcAccessMask,
                                                         unsigned int dstAccessMask)
{
    dependencies.Append({src, dst, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask});
    return *this;
}

RenderPass RenderPass::Builder::Build()
{
    VkAttachmentDescription vkAttachments[attachments.Count()];

    for (auto it = attachments.CreateFIterator(); it; ++it)
    {
        vkAttachments[it.GetIndex()] = it->type == ATTACHMENT_COLOUR ?
                                           Utils::DEFAULT_COLOR_ATTACHMENT :
                                           Utils::DEFAULT_DEPTH_ATTACHMENT;
        vkAttachments[it.GetIndex()].format = Utils::ToVkFormat(it->format);
    }

    MHArray<MHArray<VkAttachmentReference>> colourReferences(subpasses.Count());

    VkAttachmentReference depthReferences[subpasses.Count()];
    VkSubpassDescription vkSubpasses[subpasses.Count()];
    VkSubpassDependency vkDependencies[dependencies.Count()];

    for (auto it = subpasses.CreateFIterator(); it; ++it)
    {
        auto idx = it.GetIndex();
        auto& subpass = *it;
        colourReferences.Append(MHArray<VkAttachmentReference>(it->colourAttachments.Count()));

        for (auto colourIt = subpass.colourAttachments.CreateFIterator(); colourIt; ++colourIt)
        {
            colourReferences[idx].Append({*colourIt, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL});
        }

        depthReferences[idx] = {subpass.depthAttachment,
                                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

        vkSubpasses[it.GetIndex()] = Utils::CreateGraphicsSubpass(colourReferences[idx].Data(),
                                                                  colourReferences[idx].Count(),
                                                                  &depthReferences[idx]);
    }

    for (auto it = dependencies.CreateFIterator(); it; ++it)
    {
        vkDependencies[it.GetIndex()] = {it->srcSubpassIndex,
                                         it->dstSubpassIndex,
                                         it->srcStageMask,
                                         it->dstStageMask,
                                         it->srcAccessMask,
                                         it->dstAccessMask,
                                         0};
    }

    RenderPass newRenderPass;
    newRenderPass.renderPass = Utils::Create(Context::GetVkLogicalDevice(),
                                             vkAttachments,
                                             attachments.Count(),
                                             vkSubpasses,
                                             subpasses.Count(),
                                             vkDependencies,
                                             dependencies.Count());

    return newRenderPass;
}

RenderPass::RenderPass(RenderPass&& other)
{
    Swap(other);
}

RenderPass::~RenderPass()
{
    auto device = Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    Utils::Free(Context::GetVkLogicalDevice(), renderPass);
}

RenderPass& RenderPass::operator=(RenderPass&& other)
{
    Swap(other);
    return *this;
}

void RenderPass::Begin(CommandBuffer& commandBuffer,
                       unsigned int width,
                       unsigned int height,
                       FColour clearColour,
                       VkFramebuffer framebuffer)
{
    VkClearValue clearValues[2] = {{{{clearColour.r, clearColour.g, clearColour.b, clearColour.a}}},
                                   {{{1.f, 0.f}}}};

    Utils::BeginRenderPass(commandBuffer.Get(),
                           renderPass,
                           clearValues,
                           2,
                           framebuffer,
                           {{}, {width, height}});
}

void RenderPass::End(CommandBuffer& commandBuffer)
{
    Utils::EndRenderPass(commandBuffer.Get());
}

void RenderPass::Swap(RenderPass& other)
{
    auto tmpRenderPass = renderPass;

    renderPass = other.renderPass;

    other.renderPass = tmpRenderPass;
}
} // namespace Siege::Vulkan