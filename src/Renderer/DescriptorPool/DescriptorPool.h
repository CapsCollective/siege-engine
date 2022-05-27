#pragma once

#include "../Core.h"
#include "../Device/VulkanDevice.h"

namespace SnekVk
{
    class DescriptorPool
    {
        public:
        
        static void AddPoolSize(const VkDescriptorType type, const u32 size);
        static VkDescriptorPool& GetDescriptorPool() { return descriptorPool; }
        
        static void BuildPool();
        static void DestroyPool();

        private:

        static constexpr size_t MAX_DESCRIPTOR_POOL_SIZES = 10;
        
        static VkDescriptorPool descriptorPool;
        static Utils::StackArray<VkDescriptorPoolSize, MAX_DESCRIPTOR_POOL_SIZES> sizes;
    };
}