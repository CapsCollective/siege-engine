#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <cstring>

namespace std 
{
    template<>
    struct hash<SnekVk::Vertex>
    {
        size_t operator()(const SnekVk::Vertex &vertex) const 
        {
            size_t seed = 0;
            SnekVk::Utils::HashCombine(seed, vertex.color, vertex.normal, vertex.uv);
            return seed;
        };
    };
}

namespace SnekVk
{
    Model::Model(const Mesh::MeshData& meshData)
    {
        modelMesh.LoadVertices(meshData);
    }

    Model::Model(const char* filePath)
    {
        LoadModelFromFile(filePath);
    }

    Model::Model() {}

    // Destroy the vertex buffer and free the memory
    Model::~Model() 
    {}

    void Model::DestroyModel()
    {}

    void Model::LoadModelFromFile(const char* filePath)
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

        modelMesh.LoadVertices(
            {
                objVertices.data(), 
                static_cast<u32>(objVertices.size()), 
                objIndices.data(), 
                static_cast<u32>(objIndices.size())
            }
        );
    }

    void Model::Bind(VkCommandBuffer commandBuffer)
    {
        modelMesh.Bind(commandBuffer);
    }

    void Model::Draw(VkCommandBuffer commandBuffer)
    {
        if (modelMesh.HasIndexBuffer()) vkCmdDrawIndexed(commandBuffer, modelMesh.GetIndexCount(), 1, 0, 0, 0);
        else vkCmdDraw(commandBuffer, modelMesh.GetVertexCount(), 1, 0, 0);
    }
}
