#include "Model.h"

#include <cstring>

namespace SnekVk
{
    Model::Model(VulkanDevice& device, const Vertex* vertices, u32 vertexCount)
        : device{device}, vertexCount{vertexCount}
    {
        CreateVertexBuffers(vertices);
    }

    Model::~Model()
    {
        vkDestroyBuffer(device.Device(), vertexBuffer, nullptr);
        vkFreeMemory(device.Device(), vertexBufferMemory, nullptr);
    }

    void Model::CreateVertexBuffers(const Vertex* vertices)
    {
        SNEK_ASSERT(vertexCount >= 3, "Vertex count must be at least 3!");

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        device.CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory
        );

        void* data;
        vkMapMemory(device.Device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices, bufferSize);
        vkUnmapMemory(device.Device(), vertexBufferMemory);
    }

    void Model::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void Model::Draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    std::array<VkVertexInputBindingDescription, 1> Model::Vertex::GetBindingDescriptions()
    {
        std::array<VkVertexInputBindingDescription, 1> bindingDescriptions;
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::array<VkVertexInputAttributeDescription, 1> Model::Vertex::GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions;

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = 0;
        return attributeDescriptions;
    }
}