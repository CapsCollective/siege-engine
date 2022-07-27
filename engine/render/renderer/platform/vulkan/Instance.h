//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INSTANCE_H
#define SIEGE_ENGINE_INSTANCE_H

#include "../../utils/Array.h"
#include "utils/Instance.h"

namespace Siege::Vulkan
{
class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(VulkanInstance&& other);

    VulkanInstance& operator=(VulkanInstance&& other);

    VkInstance VkInstance() { return instance; }

    VkSurfaceKHR CreateGLFWSurface();
private:
    Array<const char*> GetRequiredExtensions();
    void HasRequiredGLFWInstanceExtensions();
    bool ValidationLayersSupported();

    void SetupDebugMessenger();

    VkDebugUtilsMessengerEXT debugMessenger;
    ::VkInstance instance;
};

} // namespace Siege

#endif // SIEGE_ENGINE_INSTANCE_H
