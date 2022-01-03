#include "DescriptorPool.h"

namespace SnekVk
{
    VkDescriptorPool DescriptorPool::descriptorPool {VK_NULL_HANDLE};
    Utils::StackArray<VkDescriptorPoolSize, DescriptorPool::MAX_DESCRIPTOR_POOL_SIZES> DescriptorPool::sizes;

    void DescriptorPool::AddPoolSize(const VkDescriptorType type, const u32 size)
    {
        sizes.Append({type, size});
    }

    void DescriptorPool::BuildPool()
    {
        auto device = VulkanDevice::GetDeviceInstance();

        if (descriptorPool == VK_NULL_HANDLE) 
        {
            VkDescriptorPoolCreateInfo poolCreateInfo {};
            poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolCreateInfo.flags = 0;
            poolCreateInfo.maxSets = MAX_DESCRIPTOR_POOL_SIZES;
            poolCreateInfo.poolSizeCount = static_cast<u32>(sizes.Count());
            poolCreateInfo.pPoolSizes = sizes.Data();

            SNEK_ASSERT(vkCreateDescriptorPool(device->Device(), &poolCreateInfo, nullptr, &descriptorPool) == VK_SUCCESS,
                "Unable to create descriptor pool!");
        }
    }

    void DescriptorPool::DestroyPool()
    {
        if (descriptorPool != VK_NULL_HANDLE) 
        {
            vkDestroyDescriptorPool(VulkanDevice::GetDeviceInstance()->Device(), descriptorPool, nullptr);
        }
    }
}