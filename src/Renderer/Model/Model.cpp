#include "Model.h"

#include <cstring>

namespace SnekVk
{
    Model::Model(VulkanDevice& device, const Data& configData)
        : device{device}, vertexCount{configData.vertexCount}, indexCount{configData.indexCount}
    {
        CreateVertexBuffers(configData.vertices);
        CreateIndexBuffer(configData.indices);
    }

    Model::Model(VulkanDevice& device) : device{device} {}

    // Destroy the vertex buffer and free the memory
    Model::~Model() 
    {
        std::cout << "Destroying Model" << std::endl;
        vkDestroyBuffer(device.Device(), vertexBuffer, nullptr);
        vkFreeMemory(device.Device(), vertexBufferMemory, nullptr);

        if (hasIndexBuffer)
        {
            vkDestroyBuffer(device.Device(), indexBuffer, nullptr);
            vkFreeMemory(device.Device(), indexBufferMemory, nullptr);
        }
    }

    void Model::DestroyModel()
    {
        vkDestroyBuffer(device.Device(), vertexBuffer, nullptr);
        vkFreeMemory(device.Device(), vertexBufferMemory, nullptr);
    }

    void Model::CreateVertexBuffers(const Vertex* vertices)
    {
        SNEK_ASSERT(vertexCount >= 3, "Vertex count must be at least 3!");

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        device.CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingMemory
        );

        // Map the vertex buffer to GPU memory
        void* data;
        vkMapMemory(device.Device(), stagingMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices, bufferSize);
        vkUnmapMemory(device.Device(), stagingMemory);

        device.CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vertexBuffer,
            vertexBufferMemory
        );

        device.CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device.Device(), stagingBuffer, nullptr);
        vkFreeMemory(device.Device(), stagingMemory, nullptr);
    }

    void Model::CreateIndexBuffer(const u32* indices)
    {
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;

        device.CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingMemory
        );

        void* data;
        vkMapMemory(device.Device(), stagingMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices, bufferSize);
        vkUnmapMemory(device.Device(), stagingMemory);

        device.CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer,
            indexBufferMemory
        );

        device.CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device.Device(), stagingBuffer, nullptr);
        vkFreeMemory(device.Device(), stagingMemory, nullptr);
    }

    void Model::SetVertices(const Vertex* vertices, u32 vertexCount)
    {
        CreateVertexBuffers(vertices);
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

        if (hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void Model::Draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer) vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        else vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
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
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
}
