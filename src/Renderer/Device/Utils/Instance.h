#pragma once

#include "../../Renderer.h"

namespace SnekVK
{
    bool CheckValidationLayerSupport(const char** validationLayers, size_t validationLayerCount);

    struct LayerAndExtensionInfo GetRequiredExtensions(bool enableValidationLayers);

    bool HasGlfwInstanceExtensions(bool enableValidationLayers);

    struct LayerAndExtensionInfo 
    {
        const char** names;
        u32 count;
    };

    void DestroyLayerAndExtensionInfo(LayerAndExtensionInfo& info);
}