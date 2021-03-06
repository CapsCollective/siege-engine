//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DESCRIPTOR_H
#define SIEGE_ENGINE_DESCRIPTOR_H

#include <volk/volk.h>

#include <cstdint>

namespace Siege::Utils
{
// TODO document this
class Descriptor
{
public:

    enum Type
    {
        UNIFORM = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        STORAGE = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        UNIFORM_DYNAMIC = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        STORAGE_DYNAMIC = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
    };

    static VkDescriptorSetLayoutBinding CreateLayoutBinding(uint32_t binding,
                                                            uint32_t count,
                                                            uint32_t type,
                                                            VkShaderStageFlags stageFlags,
                                                            const VkSampler* sampler = nullptr);

    static bool CreateLayout(VkDevice device,
                             VkDescriptorSetLayout& layout,
                             VkDescriptorSetLayoutBinding* bindings,
                             uint32_t bindingCount,
                             VkDescriptorSetLayoutCreateFlags flags = 0,
                             const void* pNext = nullptr);

    static VkDescriptorSetLayoutCreateInfo CreateLayoutInfo(
        VkDescriptorSetLayoutBinding* bindings,
        uint32_t bindingCount,
        VkDescriptorSetLayoutCreateFlags flags = 0,
        const void* pNext = nullptr);

    static void AllocateSets(VkDevice device,
                             VkDescriptorSet* descriptorSets,
                             VkDescriptorPool descriptorPool,
                             uint32_t setCount,
                             const VkDescriptorSetLayout* layouts,
                             const void* pNext = nullptr);

    static VkDescriptorSetAllocateInfo CreateAllocateInfo(VkDescriptorPool pool,
                                                          uint32_t setCount,
                                                          const VkDescriptorSetLayout* layouts,
                                                          const void* pNext = nullptr);

    static VkDescriptorBufferInfo CreateBufferInfo(VkBuffer buffer,
                                                   VkDeviceSize offset,
                                                   VkDeviceSize size);
    static VkWriteDescriptorSet CreateWriteSet(
        uint32_t dstBinding,
        VkDescriptorSet& dstSet,
        uint32_t descriptorCount,
        VkDescriptorType type,
        VkDescriptorBufferInfo& bufferInfo,
        const VkDescriptorImageInfo* texelBufferView = nullptr,
        const VkBufferView* imageInfos = nullptr,
        const void* pNext = nullptr);

    static void WriteSets(VkDevice device,
                          const VkWriteDescriptorSet* sets,
                          uint32_t setCount,
                          const VkCopyDescriptorSet* copies = nullptr,
                          uint32_t copyCount = 0);

private:
};
} // namespace Siege::Utils

#endif