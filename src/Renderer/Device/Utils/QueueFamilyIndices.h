#pragma once

#include "../../Renderer.h"

namespace SnekVK 
{
    struct QueueFamilyIndices 
    {
        u32 graphicsFamily;
        u32 presentFamily;

        bool hasGraphicsFamily = false;
        bool hasPresentFamily = false;
    };

    bool IsComplete(const QueueFamilyIndices& indices) { return indices.hasGraphicsFamily && indices.hasPresentFamily; };

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}