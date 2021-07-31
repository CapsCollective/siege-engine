#pragma once

#include "../../Renderer.h"

namespace SnekVk::QueueFamilyIndices
{
    struct QueueFamilyIndices {
        u32 graphicsFamily;
        u32 presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
    };

    bool IsComplete(QueueFamilyIndices& indices);

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR& surface);
}