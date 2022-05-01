#pragma once

#include "../../Core.h"
#include <unordered_set>
#include <vector>
#include <cstring>

namespace SnekVk::Extensions
{
    /**
     * Searches for available validation layers and checks that all required layers exist. 
     * Required layers are provided by the user as a char array. 
     * 
     * @param validationLayers a const char** array containing all required valiation layers.
     * @param size - the size of the array.  
     **/
    bool CheckValidationLayerSupport(const char* const* validationLayers, size_t size);

    /**
     * Compiles a list of all required extensions.
     * 
     * @param enableValidationLayers a boolean specifying if validation layers are enabled
     * @returns a vector of required validation layers (represented as const chars) 
     **/
    std::vector<const char *> GetRequiredExtensions(bool enableValidationLayers);

    /**
     * Validates that all required extensions exist for our Vulkan instance. 
     * All required validation layers MUST exist, otherwise the program crashes. 
     * 
     * @param enableValidationLayers a boolian specifying if validation layers are enabled.
     **/
    void HasGflwRequiredInstanceExtensions(bool enableValidationLayers);
}