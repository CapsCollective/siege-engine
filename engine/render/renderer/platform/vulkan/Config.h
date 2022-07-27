//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_CONFIG_H
#define SIEGE_ENGINE_VULKAN_CONFIG_H

#include <volk/volk.h>
#include <array>

namespace Siege::Vulkan
{
    class Config {

    public:

    static constexpr size_t  VALIDATION_LAYERS_COUNT = 1;
    /**
     * An array storing all required validation layers (if enabled).
     **/
    static constexpr std::array<const char*, VALIDATION_LAYERS_COUNT> validationLayers = {
            "VK_LAYER_KHRONOS_validation"};

        /**
     * An array storing all required extensions. All of these must be present for the renderer to
     *start.
     **/
    static constexpr std::array<const char*, 2> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME};
    };
}

#endif // SIEGE_ENGINE_CONFIG_H
