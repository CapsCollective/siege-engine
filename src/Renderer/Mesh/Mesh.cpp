#include "Mesh.h"

namespace SnekVk
{
    bool operator==(const Vertex& left, const Vertex& right) 
    {
        return left.position == right.position && left.color == right.color 
            && left.normal == right.normal && left.uv == right.uv;
    }

    void SetVertexBindingDescriptions(VertexDescription::Data* vertexData)
    {
        vertexData->bindings = { VertexDescription::CreateBinding(0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX) }; 
    }

    void SetVertexAttributeDescriptions(VertexDescription::Data* vertexData)
    {
        vertexData->attributes = {
            VertexDescription::CreateAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)),
            VertexDescription::CreateAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)),
            VertexDescription::CreateAttribute(2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)),
            VertexDescription::CreateAttribute(3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv))
        };
    }

    Utils::Array<VkVertexInputBindingDescription> GetVertexBindingDescriptions()
    {
        Utils::Array<VkVertexInputBindingDescription> bindingDescriptions = 
        {
            VertexDescription::CreateBinding(0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX)
        };
                
        return bindingDescriptions;
    }

    Utils::Array<VkVertexInputAttributeDescription> GetVertexAttributeDescriptions()
    {
        Utils::Array<VkVertexInputAttributeDescription> attributeDescriptions = {
            VertexDescription::CreateAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)),
            VertexDescription::CreateAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)),
            VertexDescription::CreateAttribute(2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)),
            VertexDescription::CreateAttribute(3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv))
        };

        return attributeDescriptions;
    }

    VertexDescription::Data GetDescriptionData()
    {
        return 
        { 
            GetVertexBindingDescriptions(), 
            GetVertexAttributeDescriptions()
        };
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