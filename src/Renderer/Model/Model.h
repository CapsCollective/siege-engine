#pragma once

#include "../Device/VulkanDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace SnekVk 
{
    class Model
    {
        public:

        struct Vertex
        {
            glm::vec2 position;

            static std::array<VkVertexInputBindingDescription, 1> GetBindingDescriptions();
            static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescriptions();
        };

        Model(VulkanDevice& device, const Vertex* vertices, u32 vertexCount);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);

        private:

        void CreateVertexBuffers(const Vertex* vertices);
        
        VulkanDevice& device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        u32 vertexCount;
    };
}