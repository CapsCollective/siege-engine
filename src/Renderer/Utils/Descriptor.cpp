#include "Descriptor.h"
#include <iostream>

namespace SnekVk::Utils
{
    VkDescriptorSetLayoutBinding Descriptor::CreateLayoutBinding(
        uint32_t binding, 
        uint32_t count, 
        uint32_t type, 
        VkShaderStageFlags stageFlags,
        const VkSampler* sampler
    )
    {
        return {binding, (VkDescriptorType)type, count, stageFlags, sampler};
    }

    bool Descriptor::CreateLayout(
        VkDevice device,
        VkDescriptorSetLayout& layout,
        VkDescriptorSetLayoutBinding* bindings, 
        uint32_t bindingCount, 
        VkDescriptorSetLayoutCreateFlags flags,
        const void* pNext
    )
    {
        auto createInfo = CreateLayoutInfo(bindings, bindingCount, flags, pNext);

        return vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout) == VK_SUCCESS;
    }

    VkDescriptorSetLayoutCreateInfo Descriptor::CreateLayoutInfo(
        VkDescriptorSetLayoutBinding* bindings, 
        uint32_t bindingCount, 
        VkDescriptorSetLayoutCreateFlags flags,
        const void* pNext
    )
    {
        return 
        {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            pNext,
            flags, 
            bindingCount, 
            bindings
        };
    }

    void Descriptor::AllocateSets(
        VkDevice device, 
        VkDescriptorSet* descriptorSets,
        VkDescriptorPool descriptorPool, 
        uint32_t setCount, 
        const VkDescriptorSetLayout* layouts, 
        const void* pNext
    )
    {
        auto allocateInfo = CreateAllocateInfo(descriptorPool, setCount, layouts, pNext);
        
        vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets);
    }

    VkDescriptorSetAllocateInfo Descriptor::CreateAllocateInfo(
        VkDescriptorPool pool, 
        uint32_t setCount, 
        const VkDescriptorSetLayout* layouts, 
        const void* pNext
    )
    {
        return 
        {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            pNext, 
            pool, 
            setCount, 
            layouts
        };
    }

    VkDescriptorBufferInfo Descriptor::CreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size)
    {
        return { buffer, offset, size };
    }

    VkWriteDescriptorSet Descriptor::CreateWriteSet(
        uint32_t dstBinding,
        VkDescriptorSet& dstSet,
        uint32_t descriptorCount,
        Type type, 
        VkDescriptorBufferInfo& bufferInfo,
        const VkDescriptorImageInfo* imageInfos,
        const VkBufferView* texelBufferView,
        const void* pNext
    )
    {
        return 
        {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            pNext, 
            dstSet, 
            dstBinding,
            0,
            descriptorCount,
            (VkDescriptorType)type,
            imageInfos,
            &bufferInfo,
            texelBufferView
        };
    }

    void Descriptor::WriteSets(
        VkDevice device,
        const VkWriteDescriptorSet* sets, 
        uint32_t setCount, 
        const VkCopyDescriptorSet* copies,
        uint32_t copyCount
    )
    {
        vkUpdateDescriptorSets(device, setCount, sets, copyCount, copies);
    }
}