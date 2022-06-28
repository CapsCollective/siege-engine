//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "DebugUtilsMessenger.h"
#include <utils/Logging.h>

namespace Siege::DebugUtilsMessenger
{

// TODO(Aryeh): Utilise the messageSeverity and messageType variables
static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData)
{
    String type;

    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
    {
        type = "[VALIDATION]";
    }
    else if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
    {
        type = "[PERFORMANCE]";
    }
    else
    {
        type = "[GENERAL]";
    }

    String message = String(type) + String(" ") + String(pCallbackData->pMessage);

    if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        CC_LOG_WARNING("{}",message.Str())
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        CC_LOG_ERROR("{}",message.Str())
    }
    else
    {
        CC_LOG_INFO("{}",message.Str())
    }


    return VK_FALSE;
}

VkResult CreateMessenger(VkInstance instance,
                         const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator,
                         VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    // Vulkan does not load validation layer-related function by default.
    // vkGetIntstanceProcAddr allows us to query for a relevant function pointer
    // and use that. We get the pointer, cast it to the type of function we want,
    // then use it in the 'func' variable.

    // In our case, we're looking 'vkCreateDebugUtilsMessengerEXT' so we can create
    // our debug messenger.
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance,
        "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyMessenger(VkInstance instance,
                      VkDebugUtilsMessengerEXT debugMessenger,
                      const VkAllocationCallbacks* pAllocator)
{
    // Please see 'CreateMessenger' for an explanation for what this is doing.

    // In this case, we're looking for the 'vkDestroyDebugUtilsMessengerEXT' function
    // so that we can destroy the debug messenger.
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
        instance,
        "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}
} // namespace Siege::DebugUtilsMessenger