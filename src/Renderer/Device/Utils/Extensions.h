#pragma once

#include "../../Core.h"
#include <unordered_set>
#include <vector>
#include <cstring>

namespace SnekVk::Extensions
{
    bool CheckValidationLayerSupport(const char* const* validationLayers, size_t size);

    std::vector<const char *> GetRequiredExtensions(bool enableValidationLayers);

    void HasGflwRequiredInstanceExtensions(bool enableValidationLayers);
}