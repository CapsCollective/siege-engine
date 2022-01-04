#include "Mesh.h"

namespace SnekVk
{
    bool operator==(const Vertex& left, const Vertex& right) 
    {
        return left.position == right.position && left.color == right.color 
            && left.normal == right.normal && left.uv == right.uv;
    }

    bool operator==(const Vertex2D& left, const Vertex2D& right)
    {
        return left.color == right.color && left.position == right.position;
    }

    Mesh::Mesh() {}

    Mesh::Mesh(const MeshData& meshData)
    {
        LoadVertices(meshData);
    }

    Mesh::~Mesh()
    {
        if (isFreed) return;
        std::cout << "Destroying Mesh" << std::endl;
        DestroyMesh();
    }

    void Mesh::LoadVertices(const Mesh::MeshData& meshData)
    {
        vertexCount = meshData.vertexCount;
        indexCount = meshData.indexCount;
        vertexSize = meshData.vertexSize;

        CreateVertexBuffers(meshData.vertices);
        CreateIndexBuffer(meshData.indices);
    }

    void Mesh::DestroyMesh()
    {
        Buffer::DestroyBuffer(vertexBuffer);

        if (hasIndexBuffer) Buffer::DestroyBuffer(indexBuffer);

        isFreed = true;
    }

    void Mesh::CreateVertexBuffers(const void* vertices)
    {
        VkDeviceSize bufferSize = vertexSize;

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

        Buffer::CopyData(stagingBuffer, bufferSize, vertices);

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

        Buffer::CopyData(stagingBuffer, bufferSize, indices);

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