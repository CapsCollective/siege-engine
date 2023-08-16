//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DESCRIPTOR_H
#define SIEGE_ENGINE_DESCRIPTOR_H

#include <cstdint>

#include "Types.h"
#include "volk/volk.h"

namespace Siege::Vulkan::Utils::Descriptor
{

static constexpr unsigned int MAX_DESCRIPTOR_POOL_SIZES = 1024;

VkDescriptorPool CreateDescriptorPool(VkDevice device,
                                      UniformAllocation* sizes,
                                      unsigned int sizeCount);

void DestroyDescriptorPool(VkDevice device, VkDescriptorPool pool);

VkDescriptorSetLayoutBinding CreateLayoutBinding(uint32_t binding,
                                                 uint32_t count,
                                                 uint32_t type,
                                                 VkShaderStageFlags stageFlags,
                                                 const VkSampler* sampler = nullptr);

bool CreateLayout(VkDevice device,
                  VkDescriptorSetLayout& layout,
                  VkDescriptorSetLayoutBinding* bindings,
                  uint32_t bindingCount,
                  VkDescriptorSetLayoutCreateFlags flags = 0,
                  const void* pNext = nullptr);

VkDescriptorSetLayoutCreateInfo CreateLayoutInfo(VkDescriptorSetLayoutBinding* bindings,
                                                 uint32_t bindingCount,
                                                 VkDescriptorSetLayoutCreateFlags flags = 0,
                                                 const void* pNext = nullptr);

void AllocateSets(VkDevice device,
                  VkDescriptorSet* descriptorSets,
                  VkDescriptorPool descriptorPool,
                  uint32_t setCount,
                  const VkDescriptorSetLayout* layouts,
                  const void* pNext = nullptr);

VkDescriptorSetAllocateInfo CreateAllocateInfo(VkDescriptorPool pool,
                                               uint32_t setCount,
                                               const VkDescriptorSetLayout* layouts,
                                               const void* pNext = nullptr);

VkDescriptorBufferInfo CreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size);
VkWriteDescriptorSet CreateWriteSet(uint32_t dstBinding,
                                    VkDescriptorSet& dstSet,
                                    uint32_t descriptorCount,
                                    VkDescriptorType type,
                                    const VkDescriptorBufferInfo* bufferInfo,
                                    const VkDescriptorImageInfo* texelBufferView = nullptr,
                                    const VkBufferView* imageInfos = nullptr,
                                    const void* pNext = nullptr);

void WriteSets(VkDevice device,
               const VkWriteDescriptorSet* sets,
               uint32_t setCount,
               const VkCopyDescriptorSet* copies = nullptr,
               uint32_t copyCount = 0);

VkWriteDescriptorSet WriteDescriptorImage(VkDescriptorType type,
                                          VkDescriptorSet& dstSet,
                                          VkDescriptorImageInfo* imageInfo,
                                          uint32_t binding,
                                          uint32_t count,
                                          uint32_t index = 0);

VkSamplerCreateInfo SamplerCreateInfo(
    VkFilter filters,
    VkSamplerAddressMode samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
} // namespace Siege::Vulkan::Utils::Descriptor

#endif
