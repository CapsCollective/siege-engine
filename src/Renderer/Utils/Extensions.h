#pragma once

#include "../Renderer.h"
#include <unordered_set>

namespace SnekVk
{
    bool CheckValidationLayerSupport(const char* const* validationLayers, size_t size);
    std::vector<const char *> GetRequiredExtensions(bool enableValidationLayers);
    void HasGflwRequiredInstanceExtensions(bool enableValidationLayers);
}