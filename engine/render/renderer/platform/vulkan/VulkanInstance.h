//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKANINSTANCE_H
#define SIEGE_ENGINE_VULKANINSTANCE_H

#include "../../utils/Array.h"
#include "utils/Instance.h"

namespace Siege::Vulkan
{
class VulkanInstance
{
public:

    typedef bool (*SurfaceFunc)(VkInstance, VkSurfaceKHR*);

    VulkanInstance() = default;

    VulkanInstance(SurfaceFunc surfaceCreationCallback,
                   const char** requiredExtensions = nullptr,
                   const size_t& count = 0);
    ~VulkanInstance();

    VulkanInstance(VulkanInstance&& other);

    VulkanInstance& operator=(VulkanInstance&& other);

    VkInstance& Instance()
    {
        return instance;
    }

    VkSurfaceKHR& Surface()
    {
        return surface;
    }

private:

    Array<const char*> GetRequiredExtensions(const char** requiredExtensions = nullptr,
                                             const size_t& count = 0);
    void HasRequiredGLFWInstanceExtensions(const char** requiredExtensions, const size_t& count);
    bool ValidationLayersSupported();

    void SetupDebugMessenger();

    VkSurfaceKHR surface {VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT debugMessenger {VK_NULL_HANDLE};
    ::VkInstance instance {VK_NULL_HANDLE};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKANINSTANCE_H
