#include "Mesh.h"

namespace SnekVk
{
    bool operator==(const Vertex& left, const Vertex& right) 
    {
        return left.position == right.position && left.color == right.color 
            && left.normal == right.normal && left.uv == right.uv;
    }

    std::array<VkVertexInputBindingDescription, 1> GetVertexBindingDescriptions()
    {
        std::array<VkVertexInputBindingDescription, 1> bindingDescriptions;
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::array<VkVertexInputAttributeDescription, 4> GetVertexAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions;

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, normal);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, uv);

        return attributeDescriptions;
    }

    Mesh::Mesh() {}

    Mesh::Mesh(const MeshData& meshData)
    {
        LoadVertices(meshData);
    }

    Mesh::~Mesh()
    {
        std::cout << "Destroying Model" << std::endl;
        Buffer::DestroyBuffer(vertexBuffer);

        if (hasIndexBuffer) Buffer::DestroyBuffer(indexBuffer);
    }

    void Mesh::LoadVertices(const Mesh::MeshData& meshData)
    {
        this->vertexCount = meshData.vertexCount;
        this->indexCount = meshData.indexCount;

        CreateVertexBuffers(meshData.vertices);
        CreateIndexBuffer(meshData.indices);
    }

    void Mesh::CreateVertexBuffers(const Vertex* vertices)
    {
        SNEK_ASSERT(vertexCount >= 3, "Vertex count must be at least 3!");

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        Buffer::Buffer stagingBuffer;

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT stagingBuffer.buffer,
            OUT stagingBuffer.bufferMemory);

        Buffer::CopyData<Vertex>(stagingBuffer, bufferSize, vertices);

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            OUT vertexBuffer.buffer,
            OUT vertexBuffer.bufferMemory
        );

        Buffer::CopyBuffer(stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);

        Buffer::DestroyBuffer(stagingBuffer);
    }

    void Mesh::CreateIndexBuffer(const u32* indices)
    {
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        Buffer::Buffer stagingBuffer;

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT stagingBuffer.buffer,
            OUT stagingBuffer.bufferMemory
        );

        Buffer::CopyData<u32>(stagingBuffer, bufferSize, indices);

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer.buffer,
            indexBuffer.bufferMemory
        );

        Buffer::CopyBuffer(stagingBuffer.buffer, indexBuffer.buffer, bufferSize);

        Buffer::DestroyBuffer(stagingBuffer);
    }

    void Mesh::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    }
}