#include "QueueFamilyIndices.h"

namespace SnekVk::QueueFamilyIndices
{
    bool IsComplete(QueueFamilyIndices& indices) { return indices.graphicsFamilyHasValue && indices.presentFamilyHasValue; }

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR& surface) 
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, OUT &queueFamilyCount, nullptr);

        VkQueueFamilyProperties queueFamilies[queueFamilyCount];
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, OUT queueFamilies);

        for (size_t i = 0; i < queueFamilyCount; i++) 
        {
            VkQueueFamilyProperties queueFamily = queueFamilies[i];
            
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                indices.graphicsFamily = i;
                indices.graphicsFamilyHasValue = true;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, OUT &presentSupport);

            if (queueFamily.queueCount > 0 && presentSupport) 
            {
                indices.presentFamily = i;
                indices.presentFamilyHasValue = true;
            }
            if (IsComplete(indices)) 
            {
                break;
            }
        }

        return indices;
    }
}