#pragma once

#include "../../Renderer.h"
#include <iostream>

namespace SnekVk::DebugUtilsMessenger
{
      // local callback functions
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
      void *pUserData);

  VkResult CreateMessenger(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
      const VkAllocationCallbacks *pAllocator,
      VkDebugUtilsMessengerEXT *pDebugMessenger);

  void DestroyMessenger(
      VkInstance instance,
      VkDebugUtilsMessengerEXT debugMessenger,
      const VkAllocationCallbacks *pAllocator);

  void PopulateCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
}