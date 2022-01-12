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

        VkDeviceSize bufferSize = vertexSize * MAX_VERTICES;

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT globalStagingBuffer.buffer,
            OUT globalStagingBuffer.bufferMemory);

        Buffer::CreateBuffer(
            sizeof(u32) * MAX_INDICES,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
            // Ensures that CPU and GPU memory are consistent across both devices.
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            OUT globalIndexStagingBuffer.buffer,
            OUT globalIndexStagingBuffer.bufferMemory);

        hasVertexBuffer = vertexCount > 0;

        if (hasVertexBuffer) CreateVertexBuffers(meshData.vertices);

        hasIndexBuffer = indexCount > 0;

        if (hasIndexBuffer) CreateIndexBuffer(meshData.indices);
    }

    void Mesh::DestroyMesh()
    {
        Buffer::DestroyBuffer(vertexBuffer);
        Buffer::DestroyBuffer(globalStagingBuffer);

        if (hasIndexBuffer) Buffer::DestroyBuffer(indexBuffer);

        isFreed = true;
    }

    void Mesh::CreateVertexBuffers(const void* vertices)
    {
        VkDeviceSize bufferSize = vertexSize * MAX_VERTICES;

        Buffer::CopyData(globalStagingBuffer, vertexSize * vertexCount, vertices);

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            OUT vertexBuffer.buffer,
            OUT vertexBuffer.bufferMemory
        );

        Buffer::CopyBuffer(globalStagingBuffer.buffer, vertexBuffer.buffer, bufferSize);
    }

    void Mesh::CreateIndexBuffer(const u32* indices)
    {
        VkDeviceSize bufferSize = sizeof(u32) * MAX_INDICES;

        Buffer::CopyData(globalIndexStagingBuffer, bufferSize, indices);

        Buffer::CreateBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            // specifies that data is accessible on the CPU.
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer.buffer,
            indexBuffer.bufferMemory
        );

        Buffer::CopyBuffer(globalIndexStagingBuffer.buffer, indexBuffer.buffer, bufferSize);
    }

    void Mesh::Bind(VkCommandBuffer commandBuffer)
    {
        if (vertexCount > 0)
        {
            VkBuffer buffers[] = {vertexBuffer.buffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        }

        if (hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
    }

    void Mesh::UpdateVertices(const Mesh::MeshData& meshData)
    {
        vertexCount = meshData.vertexCount;
        indexCount = meshData.indexCount;
        vertexSize = meshData.vertexSize;

        UpdateVertexBuffer(meshData.vertices);

        UpdateIndexBuffer(meshData.indices);
    }

    void Mesh::UpdateVertexBuffer(const void* vertices)
    {
        if (vertexCount == 0) return;

        if (!hasVertexBuffer)
        {
            CreateVertexBuffers(vertices);
            hasVertexBuffer = true;
            return;
        }

        Buffer::CopyData(globalStagingBuffer, vertexSize * vertexCount, vertices);
        Buffer::CopyBuffer(globalStagingBuffer.buffer, vertexBuffer.buffer, vertexSize * vertexCount);
    }

    void Mesh::UpdateIndexBuffer(u32* indices)
    {
        if (indexCount == 0) return;

        if (!hasIndexBuffer)
        {
            CreateIndexBuffer(indices);
            hasIndexBuffer = true;
            return;
        }

        VkDeviceSize indexSize = sizeof(u32) * indexCount;

        Buffer::CopyData(globalIndexStagingBuffer, indexSize, indices);
        Buffer::CopyBuffer(globalIndexStagingBuffer.buffer, indexBuffer.buffer, indexSize);
    }
}