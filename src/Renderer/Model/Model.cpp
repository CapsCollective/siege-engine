#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <cstring>

namespace std 
{
    template<>
    struct hash<SnekVk::Model::Vertex>
    {
        size_t operator()(const SnekVk::Model::Vertex &vertex) const 
        {
            size_t seed = 0;
            SnekVk::Utils::HashCombine(seed, vertex.color, vertex.normal, vertex.uv);
            return seed;
        };
    };
}

namespace SnekVk
{
    Model::Model(const Data& configData)
        : vertexCount{configData.vertexCount}, indexCount{configData.indexCount}
    {
        CreateVertexBuffers(configData.vertices);
        CreateIndexBuffer(configData.indices);
    }

    Model::Model(const char* filePath)
    {
        Data data{};
        LoadModelFromFile(data, filePath);
        std::cout << "Vertex Count: " << data.vertexCount << std::endl;

        vertexCount = data.vertexCount;
        indexCount = data.indexCount;

        CreateVertexBuffers(data.vertices);
        CreateIndexBuffer(data.indices);
        
        delete [] data.vertices;
        delete [] data.indices;
    }

    Model::Model() {}

    // Destroy the vertex buffer and free the memory
    Model::~Model() 
    {
        std::cout << "Destroying Model" << std::endl;
        Buffer::DestroyBuffer(vertexBuffer);

        if (hasIndexBuffer) Buffer::DestroyBuffer(indexBuffer);
    }

    void Model::DestroyModel()
    {
        Buffer::DestroyBuffer(vertexBuffer);

        if (hasIndexBuffer) Buffer::DestroyBuffer(indexBuffer);
    }

    void Model::CreateVertexBuffers(const Vertex* vertices)
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

        // Copy the data in the staging buffer to the index buffer
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

    void Model::CreateIndexBuffer(const u32* indices)
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

    void Model::LoadModelFromFile(Data& data, const char* filePath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        SNEK_ASSERT(tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath),
            warn + err);
        
        std::vector<Vertex> objVertices;
        std::vector<u32> objIndices;
        std::unordered_map<Vertex, u32> uniqueVertices{};

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                if (index.vertex_index >= 0)
                {
                    vertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    vertex.color = {
                        attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2]
                    };
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<u32>(objVertices.size());
                    objVertices.push_back(vertex);
                }
                objIndices.push_back(uniqueVertices[vertex]);
            }
        }

        data.vertices = new Vertex[objVertices.size()];
        data.indices = new u32[objIndices.size()];

        memcpy(data.vertices, objVertices.data(), sizeof(Vertex) * objVertices.size());
        memcpy(data.indices, objIndices.data(), sizeof(u32) * objIndices.size());

        data.vertexCount = static_cast<u32>(objVertices.size());
        data.indexCount = static_cast<u32>(objIndices.size());
    }

    void Model::SetVertices(const Vertex* vertices, u32 vertexCount)
    {
        CreateVertexBuffers(vertices);
    }

    void Model::Bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer.buffer};
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

        if (hasIndexBuffer) vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
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

    std::array<VkVertexInputAttributeDescription, 4> Model::Vertex::GetAttributeDescriptions()
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
}
