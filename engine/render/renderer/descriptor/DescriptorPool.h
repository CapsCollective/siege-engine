//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DESCRIPTOR_POOL_H
#define SIEGE_ENGINE_DESCRIPTOR_POOL_H

#include "../Core.h"
#include "../device/VulkanDevice.h"

namespace Siege
{
class DescriptorPool
{
public:

    static void AddPoolSize(const VkDescriptorType type, const uint32_t size);
    static VkDescriptorPool& GetDescriptorPool()
    {
        return descriptorPool;
    }

    static void BuildPool();
    static void DestroyPool();

private:

    static constexpr size_t MAX_DESCRIPTOR_POOL_SIZES = 10;

    static VkDescriptorPool descriptorPool;
    static Utils::MSArray<VkDescriptorPoolSize, MAX_DESCRIPTOR_POOL_SIZES> sizes;
};
} // namespace Siege

#endif
