//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SHADERMODULE_H
#define SIEGE_ENGINE_SHADERMODULE_H

#include <utils/collections/HeapArray.h>
#include <volk/volk.h>

namespace Siege::Vulkan::Utils::Shader
{
/**
 * Creates a ShaderModule from raw binary (represented by a char array)
 * @param binary an array of bytes
 * @return a newly created VkShaderModule
 */
VkShaderModule CreateShaderModule(MHArray<char> binary);

/**
 * Destroys a Vulkan ShaderModule object
 * @param module the module being destroyed
 */
void DestroyShaderModule(VkShaderModule module);
} // namespace Siege::Vulkan::Utils::Shader

#endif // SIEGE_ENGINE_SHADERMODULE_H
