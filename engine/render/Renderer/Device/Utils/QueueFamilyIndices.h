#pragma once

#include "../../Core.h"

namespace SnekVk::QueueFamilyIndices
{
    /**
     * A struct storing the indices of our present and graphics queues.  
     * Vulkan stores the queues in an array, the only way to differentiate 
     * between the queues is through their indices.
     **/
    struct QueueFamilyIndices {
        u32 graphicsFamily;
        u32 presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
    };

    /**
     * Evaluates if we have both the graphics and present family indices present. 
     * @param indices the QueueFamilyIndices struct to be evaluated
     * @returns a boolean that evaulates to true if both a graphics and present family are present. 
     **/
    bool IsComplete(QueueFamilyIndices& indices);

    /**
     * Finds all the available queue indices for the given device. 
     * @param device the physical device required for finding queue indices. 
     * @param surface the window surface to render images to. 
     * @returns a QueueFamilyIndices struct containing the queue indices for graphics and presentation.
     **/
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR& surface);
}