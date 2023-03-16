//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Descriptor.h"

namespace Siege
{
VkDescriptorSetLayoutBinding Descriptor::CreateLayoutBinding(uint32_t binding,
                                                             uint32_t count,
                                                             uint32_t type,
                                                             VkShaderStageFlags stageFlags,
                                                             const VkSampler* sampler)
{
    return {binding, (VkDescriptorType) type, count, stageFlags, sampler};
}

bool Descriptor::CreateLayout(VkDevice device,
                              VkDescriptorSetLayout& layout,
                              VkDescriptorSetLayoutBinding* bindings,
                              uint32_t bindingCount,
                              VkDescriptorSetLayoutCreateFlags flags,
                              const void* pNext)
{
    auto createInfo = CreateLayoutInfo(bindings, bindingCount, flags, pNext);

    return vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout) == VK_SUCCESS;
}

VkDescriptorSetLayoutCreateInfo Descriptor::CreateLayoutInfo(VkDescriptorSetLayoutBinding* bindings,
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

void Descriptor::AllocateSets(VkDevice device,
                              VkDescriptorSet* descriptorSets,
                              VkDescriptorPool descriptorPool,
                              uint32_t setCount,
                              const VkDescriptorSetLayout* layouts,
                              const void* pNext)
{
    auto allocateInfo = CreateAllocateInfo(descriptorPool, setCount, layouts, pNext);

    VkResult r = vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets);
}

VkDescriptorSetAllocateInfo Descriptor::CreateAllocateInfo(VkDescriptorPool pool,
                                                           uint32_t setCount,
                                                           const VkDescriptorSetLayout* layouts,
                                                           const void* pNext)
{
    return {VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, pNext, pool, setCount, layouts};
}

VkDescriptorBufferInfo Descriptor::CreateBufferInfo(VkBuffer buffer,
                                                    VkDeviceSize offset,
                                                    VkDeviceSize size)
{
    return {buffer, offset, size};
}

VkWriteDescriptorSet Descriptor::CreateWriteSet(uint32_t dstBinding,
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

void Descriptor::WriteSets(VkDevice device,
                           const VkWriteDescriptorSet* sets,
                           uint32_t setCount,
                           const VkCopyDescriptorSet* copies,
                           uint32_t copyCount)
{
    vkUpdateDescriptorSets(device, setCount, sets, copyCount, copies);
}

VkWriteDescriptorSet Descriptor::WriteDescriptorImage(VkDescriptorType type,
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

VkSamplerCreateInfo Descriptor::SamplerCreateInfo(
    VkFilter filters,
    VkSamplerAddressMode samplerAddressMode)
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
} // namespace Siege
