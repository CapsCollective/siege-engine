//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_VIEWPORT_H
#define SIEGE_ENGINE_VULKAN_VIEWPORT_H

#include "TypeAdaptor.h"

namespace Siege::Vulkan::Utils
{

void SetViewport(VkCommandBuffer buffer,
                 float width,
                 float height,
                 float x,
                 float y,
                 float minDepth,
                 float maxDepth);
void SetViewport(VkCommandBuffer buffer, float width, float height, float x, float y);
void SetViewport(VkCommandBuffer buffer, float width, float height);

void SetScissor(VkCommandBuffer buffer,
                int offsetX,
                unsigned int offsetY,
                unsigned int width,
                unsigned int height);
void SetScissor(VkCommandBuffer buffer, unsigned int width, unsigned int height);

} // namespace Siege::Vulkan::Utils

#endif // SIEGE_ENGINE_VULKAN_VIEWPORT_H
