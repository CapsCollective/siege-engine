#pragma once

#include <vulkan/vulkan.h>
#include <iostream>

namespace SnekVk::DebugUtilsMessenger
{
  /**
   * A function pointer that acts as a callback for validation layers. 
   * This allows us to specify any behaviours or functions that should
   * be triggered if a validation error or warning are generated.
   * 
   * In our case, all this callback does is log the error to stdErr.
   *   
   * @param messageSeverity a bitmask specifying therequired message severity
   * @param messageType a bitmask specifying the message type required
   * @param pCallbackData the data being returned by the validationLayer
   * @param pUserData a void pointer containing any data inserted for usage. 
   **/
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData);

    /**
     * Creates a DebugUtilsMessenger and returns the result to the 
     * 'pDebugMessenger' variable.
     * 
     * @param instance the Vulkan instance being used to create the messenger.
     * @param pCreateInfo the DebugUtilsMessengerCreateInfoEXT struct used to configure the messenger.
     * @param pAllocator the memory allocator used to allocate the messenger.
     * @param pDebugMessenger the debug messenger struct to be created and returned. 
     **/
    VkResult CreateMessenger(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger);

    /**
     * De-allocates the memory of a DebugUtilsMessengerEXT struct. 
     * 
     * @param instance the instance used to create the debug messenger.
     * @param debugMessenger the debugMessenger to be de-allocated
     * @param pAllocator the allocator used to create the messenger
     **/
    void DestroyMessenger(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator);

    /**
     * Populates a DebugUtilsMessengerCreateInfoEXT struct with pre-configured values. 
     * In our case, we only care about warning and error validation layers.  
     * We're also interested in general errors, optimisation errors, and validation errors. 
     * 
     * @param createInfo an empty DebugUtilsMessengerCreateInfoEXT struct to be populated.
     **/
    void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
}