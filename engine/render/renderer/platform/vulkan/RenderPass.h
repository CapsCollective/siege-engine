//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_RENDERPASS_H
#define SIEGE_ENGINE_VULKAN_RENDERPASS_H

#include <utils/Colour.h>
#include <utils/collections/HeapArray.h>

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
typedef VkRenderPass_T* RenderPassRaw;

class RenderPass
{
public:

    struct Builder
    {
        static inline constexpr unsigned int DEFAULT_STAGE_MASK =
            Utils::PipelineStage::STAGE_COLOUR_ATTACHMENT_OUTPUT |
            Utils::PipelineStage::STAGE_EARLY_FRAGMENT_TEST;

        static inline constexpr unsigned int DEFAULT_ACCESS_MASK =
            Utils::MemoryAccess::ACCESS_COLOUR_ATTACHMENT_WRITE |
            Utils::MemoryAccess::ACCESS_DEPTH_ATTACHMENT_WRITE;

        enum AttachmentType
        {
            ATTACHMENT_NONE = 0,
            ATTACHMENT_COLOUR = 1,
            ATTACHMENT_DEPTH = 2
        };

        struct Attachment
        {
            AttachmentType type {ATTACHMENT_NONE};
            Utils::ImageFormat format {Utils::ImageFormat::NONE};
        };

        struct Subpass
        {
            unsigned int depthAttachment {0};
            MHArray<unsigned int> colourAttachments;
        };

        struct Dependency
        {
            unsigned int srcSubpassIndex {0};
            unsigned int dstSubpassIndex {0};

            unsigned int srcStageMask {DEFAULT_STAGE_MASK};
            unsigned int dstStageMask {DEFAULT_STAGE_MASK};

            unsigned int srcAccessMask {0};
            unsigned int dstAccessMask {DEFAULT_ACCESS_MASK};
        };

        Builder& WithAttachments(unsigned int attachmentCount);
        Builder& WithSubpasses(unsigned int attachmentCount);
        Builder& WithDependencies(unsigned int attachmentCount);
        Builder& WithColourAttachment(Utils::ImageFormat layout);
        Builder& WithDepthAttachment(Utils::DepthFormat layout);

        Builder& WithSubpass(MHArray<unsigned int> colourAttachments, unsigned int depthAttachment);
        Builder& WithDependency(unsigned int src,
                                unsigned int dst,
                                unsigned int srcStageMask = DEFAULT_STAGE_MASK,
                                unsigned int dstStageMask = DEFAULT_STAGE_MASK,
                                unsigned int srcAccessMask = 0,
                                unsigned int dstAccessMask = DEFAULT_ACCESS_MASK);

        RenderPass Build();

        MHArray<struct Attachment> attachments;
        MHArray<struct Subpass> subpasses;
        MHArray<struct Dependency> dependencies;
    };

    RenderPass() = default;
    RenderPass(RenderPass&& other);
    ~RenderPass();

    RenderPass& operator=(RenderPass&& other);

    void Begin(CommandBuffer& commandBuffer,
               unsigned int width,
               unsigned int height,
               FColour clearColour,
               VkFramebuffer framebuffer);
    void End(CommandBuffer& commandBuffer);

    inline constexpr RenderPassRaw GetRawRenderPass() const
    {
        return renderPass;
    }

private:

    void Swap(RenderPass& other);
    RenderPassRaw renderPass {nullptr};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_RENDERPASS_H
