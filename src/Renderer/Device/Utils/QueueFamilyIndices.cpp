#include "QueueFamilyIndices.h"

namespace SnekVK 
{
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

        for (size_t i = 0; i < queueFamilyCount; i++)
        {
            auto& queueFamily = queueFamilies[i];

            // Check if device queues support graphics operatrions
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
                indices.hasGraphicsFamily = true;
            }

            VkBool32 presentSupport = false;

            // Check if device supports operations to our surface (glfw)
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (queueFamily.queueCount > 0 && presentSupport)
            {
                indices.presentFamily = i;
                indices.hasPresentFamily = true;
            }

            if (IsComplete(indices)) break;
        }
        return indices;
    }
}