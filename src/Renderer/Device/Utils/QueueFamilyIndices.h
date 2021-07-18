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

    bool IsComplete(const QueueFamilyIndices& indices);

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}