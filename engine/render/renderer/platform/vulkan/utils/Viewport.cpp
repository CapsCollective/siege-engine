//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Viewport.h"

namespace Siege::Vulkan::Utils
{

void SetViewport(VkCommandBuffer buffer,
                 float width,
                 float height,
                 float x,
                 float y,
                 float minDepth,
                 float maxDepth)
{
    VkViewport viewport {};
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;

    vkCmdSetViewport(buffer, 0, 1, &viewport);
}

void SetViewport(VkCommandBuffer buffer, float width, float height, float x, float y)
{
    SetViewport(buffer, width, height, x, y, 0.f, 1.f);
}

void SetViewport(VkCommandBuffer buffer, float width, float height)
{
    SetViewport(buffer, width, height, 0.f, 0.f);
}

void SetScissor(VkCommandBuffer buffer,
                int offsetX,
                int offsetY,
                unsigned int width,
                unsigned int height)
{
    VkRect2D scissor {{offsetX, offsetY}, {width, height}};

    vkCmdSetScissor(buffer, 0, 1, &scissor);
}

void SetScissor(VkCommandBuffer buffer, unsigned int width, unsigned int height)
{
    SetScissor(buffer, 0, 0, width, height);
}

} // namespace Siege::Vulkan::Utils