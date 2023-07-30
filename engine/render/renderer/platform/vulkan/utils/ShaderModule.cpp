//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ShaderModule.h"

#include <utils/Logging.h>

#include "../Context.h"

namespace Siege::Vulkan::Utils::Shader
{
VkShaderModule CreateShaderModule(MHArray<char> binary)
{
    VkShaderModule shaderModule {nullptr};

    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = binary.Size();
    // Because the code is expected to be numerical, we need to cast the values in the
    // array to 32-bit unsigned integers.
    createInfo.pCode = reinterpret_cast<const uint32_t*>(binary.Data());

    CC_ASSERT(vkCreateShaderModule(Context::GetVkLogicalDevice(),
                                   &createInfo,
                                   nullptr,
                                   OUT & shaderModule) == VK_SUCCESS,
              "Failed to create shader module!")

    return shaderModule;
}

void DestroyShaderModule(VkShaderModule module)
{
    auto device = Vulkan::Context::GetVkLogicalDevice();

    if (device == nullptr) return;

    vkDestroyShaderModule(device, module, nullptr);
}
} // namespace Siege::Vulkan::Utils::Shader