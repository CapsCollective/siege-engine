#include "Model.h"

#include <cstring>

namespace SnekVk
{
    Model::Model(VulkanDevice& device, const Vertex* vertices, u32 vertexCount)
        : device{device}, vertexCount{vertexCount}
    {
        CreateVertexBuffers(vertices);
    }

    // Destroy the vertex buffer and free the memory
    Model::~Model()
    {
        std::cout << "Destroying Model" << std::endl;
        vkDestroyBuffer(device.Device(), vertexBuffer, nullptr);
        vkFreeMemory(device.Device(), vertexBufferMemory, nullptr);
    }

    void Model::DestroyModel()
    {
        std::cout << "Destroying Model" << std::endl;
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

        // Map the vertex buffer to GPU memory
        void* data;
        vkMapMemory(device.Device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices, bufferSize);
        vkUnmapMemory(device.Device(), vertexBufferMemory);
    }

    void Model::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        /**
         * BindVertexBuffer allows us to bind the type of buffers we want to send to the GPU. 
         * It accepts 5 parameters:
         * commandBuffer - the command buffer which this is being recorded to
         * firstBinding - the index of the first binding we want to write to
         * bindingCount - the number of bindings we're writing to
         * pBuffers - an array of buffers
         * pOffsets - if we only want to write to some buffers we can include a data offset
         */
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void Model::Draw(VkCommandBuffer commandBuffer)
    {
        /**
         * The draw command accepts four parameters: 
         * commandBuffer - the buffer containing our frame data.
         * vertexCount - how many vertices need to be drawn
         * instanceCount - if we're using instanced rendering we declare which instance we're drawing
         * firstVertex - if we want to skip some vertices we can declare where we want to start
         * firstInstance - if we're using multiple instances we can declare where to start
         */
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    std::array<VkVertexInputBindingDescription, 1> Model::Vertex::GetBindingDescriptions()
    {
        std::array<VkVertexInputBindingDescription, 1> bindingDescriptions;
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex); // how much memory each element takes
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // The type of data being inputted
        return bindingDescriptions;
    }

    std::array<VkVertexInputAttributeDescription, 2> Model::Vertex::GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions;

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
}
