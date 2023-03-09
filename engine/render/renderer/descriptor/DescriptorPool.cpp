//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "DescriptorPool.h"

#include <utils/Logging.h>

#include "render/renderer/platform/vulkan/Context.h"

namespace Siege
{
VkDescriptorPool DescriptorPool::descriptorPool {VK_NULL_HANDLE};
MSArray<VkDescriptorPoolSize, DescriptorPool::MAX_DESCRIPTOR_POOL_SIZES> DescriptorPool::sizes;

void DescriptorPool::AddPoolSize(const VkDescriptorType type, const uint32_t size)
{
    sizes.Append({type, size});
}

void DescriptorPool::BuildPool()
{
    if (descriptorPool == VK_NULL_HANDLE)
    {
        VkDescriptorPoolCreateInfo poolCreateInfo {};
        poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.flags = 0;
        poolCreateInfo.maxSets = MAX_DESCRIPTOR_POOL_SIZES;
        poolCreateInfo.poolSizeCount = static_cast<uint32_t>(sizes.Count());
        poolCreateInfo.pPoolSizes = sizes.Data();

        CC_ASSERT(vkCreateDescriptorPool(Vulkan::Context::GetVkLogicalDevice(),
                                         &poolCreateInfo,
                                         nullptr,
                                         &descriptorPool) == VK_SUCCESS,
                  "Unable to create descriptor pool!");
    }
}

void DescriptorPool::DestroyPool()
{
    if (descriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(Vulkan::Context::GetVkLogicalDevice(), descriptorPool, nullptr);
    }
}
} // namespace Siege
