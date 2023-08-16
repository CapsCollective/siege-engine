//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Descriptor.h"

#include <utils/Logging.h>

namespace Siege::Vulkan::Utils::Descriptor
{
VkDescriptorPool CreateDescriptorPool(VkDevice device,
                                      UniformAllocation* sizes,
                                      unsigned int sizeCount)
{
    VkDescriptorPool pool {VK_NULL_HANDLE};

    VkDescriptorPoolCreateInfo poolCreateInfo {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.flags = 0;
    poolCreateInfo.maxSets = MAX_DESCRIPTOR_POOL_SIZES;
    poolCreateInfo.poolSizeCount = sizeCount;
    poolCreateInfo.pPoolSizes = reinterpret_cast<VkDescriptorPoolSize*>(sizes);

    CC_ASSERT(vkCreateDescriptorPool(device, &poolCreateInfo, nullptr, &pool) == VK_SUCCESS,
              "Unable to create descriptor pool!")

    return pool;
}

void DestroyDescriptorPool(VkDevice device, VkDescriptorPool pool)
{
    if (pool == nullptr) return;
    vkDestroyDescriptorPool(device, pool, nullptr);
}

VkDescriptorSetLayoutBinding CreateLayoutBinding(uint32_t binding,
                                                 uint32_t count,
                                                 uint32_t type,
                                                 VkShaderStageFlags stageFlags,
                                                 const VkSampler* sampler)
{
    return {binding, (VkDescriptorType) type, count, stageFlags, sampler};
}

bool CreateLayout(VkDevice device,
                  VkDescriptorSetLayout& layout,
                  VkDescriptorSetLayoutBinding* bindings,
                  uint32_t bindingCount,
                  VkDescriptorSetLayoutCreateFlags flags,
                  const void* pNext)
{
    auto createInfo = CreateLayoutInfo(bindings, bindingCount, flags, pNext);

    return vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout) == VK_SUCCESS;
}

VkDescriptorSetLayoutCreateInfo CreateLayoutInfo(VkDescriptorSetLayoutBinding* bindings,
                                                 uint32_t bindingCount,
                                                 VkDescriptorSetLayoutCreateFlags flags,
                                                 const void* pNext)
{
    return {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            pNext,
            flags,
            bindingCount,
            bindings};
}

void AllocateSets(VkDevice device,
                  VkDescriptorSet* descriptorSets,
                  VkDescriptorPool descriptorPool,
                  uint32_t setCount,
                  const VkDescriptorSetLayout* layouts,
                  const void* pNext)
{
    auto allocateInfo = CreateAllocateInfo(descriptorPool, setCount, layouts, pNext);

    vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets);
}

VkDescriptorSetAllocateInfo CreateAllocateInfo(VkDescriptorPool pool,
                                               uint32_t setCount,
                                               const VkDescriptorSetLayout* layouts,
                                               const void* pNext)
{
    return {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, pNext, pool, setCount, layouts};
}

VkDescriptorBufferInfo CreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size)
{
    return {buffer, offset, size};
}

VkWriteDescriptorSet CreateWriteSet(uint32_t dstBinding,
                                    VkDescriptorSet& dstSet,
                                    uint32_t descriptorCount,
                                    VkDescriptorType type,
                                    const VkDescriptorBufferInfo* bufferInfo,
                                    const VkDescriptorImageInfo* imageInfos,
                                    const VkBufferView* texelBufferView,
                                    const void* pNext)
{
    return {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            pNext,
            dstSet,
            dstBinding,
            0,
            descriptorCount,
            type,
            imageInfos,
            bufferInfo,
            texelBufferView};
}

void WriteSets(VkDevice device,
               const VkWriteDescriptorSet* sets,
               uint32_t setCount,
               const VkCopyDescriptorSet* copies,
               uint32_t copyCount)
{
    vkUpdateDescriptorSets(device, setCount, sets, copyCount, copies);
}

VkWriteDescriptorSet WriteDescriptorImage(VkDescriptorType type,
                                          VkDescriptorSet& dstSet,
                                          VkDescriptorImageInfo* imageInfo,
                                          uint32_t binding,
                                          uint32_t count,
                                          uint32_t index)
{
    VkWriteDescriptorSet write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;

    write.dstBinding = binding;
    write.dstSet = dstSet;
    write.dstArrayElement = index;
    write.descriptorCount = count;
    write.descriptorType = type;
    write.pImageInfo = imageInfo;

    return write;
}

VkSamplerCreateInfo SamplerCreateInfo(VkFilter filters, VkSamplerAddressMode samplerAddressMode)
{
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;

    info.magFilter = filters;
    info.minFilter = filters;
    info.addressModeU = samplerAddressMode;
    info.addressModeV = samplerAddressMode;
    info.addressModeW = samplerAddressMode;
    info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_ALWAYS;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.mipLodBias = 0.0f;
    info.minLod = 0.0f;
    info.maxLod = 0.0f;

    return info;
}

} // namespace Siege::Vulkan::Utils::Descriptor
