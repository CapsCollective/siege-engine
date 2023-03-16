//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <utils/Logging.h>

namespace std
{
// We need to use the Siege namespace here since the Hash functions only operate in the std
// namespace
template<>
struct hash<Siege::Model::ModelVertex3D>
{
    size_t operator()(const Siege::Model::ModelVertex3D& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    };
};

template<>
struct hash<Siege::Vec3>
{
    size_t operator()(const Siege::Vec3& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.x, vertex.y, vertex.z);
        return seed;
    };
};

template<>
struct hash<Siege::Vec2>
{
    size_t operator()(const Siege::Vec2& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.x, vertex.y);
        return seed;
    };
};

template<>
struct hash<Siege::Vec4>
{
    size_t operator()(const Siege::Vec4& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.x, vertex.y, vertex.z, vertex.w);
        return seed;
    };
};
} // namespace std

namespace Siege
{
Model::Model(const Vulkan::Mesh::VertexData& vertices, const Vulkan::Mesh::IndexData& indices)
{
    mesh = Vulkan::Mesh(vertices, indices);
}

Model::Model(Vulkan::Mesh&& newMesh)
{
    mesh = std::move(newMesh);
}

Model::Model(const String& filePath)
{
    LoadModelFromFile(filePath);
}

Model::Model() {}

// Free the vertex buffer and free the memory
Model::~Model() {}

void Model::DestroyModel()
{
    mesh.Free();
}

void Model::LoadModelFromFile(const String& filePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    CC_ASSERT(tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath),
              (warn + err).c_str())

    std::vector<ModelVertex3D> objVertices;
    std::vector<uint32_t> objIndices;
    std::unordered_map<ModelVertex3D, uint32_t> uniqueVertices {};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            ModelVertex3D vertex {};

            if (index.vertex_index >= 0)
            {
                vertex.position = Vec3 {attrib.vertices[3 * index.vertex_index + 0],
                                        attrib.vertices[3 * index.vertex_index + 1],
                                        attrib.vertices[3 * index.vertex_index + 2]};

                vertex.color = FColour {attrib.colors[3 * index.vertex_index + 0],
                                        attrib.colors[3 * index.vertex_index + 1],
                                        attrib.colors[3 * index.vertex_index + 2],
                                        1.f};
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = Vec3 {attrib.normals[3 * index.normal_index + 0],
                                      attrib.normals[3 * index.normal_index + 1],
                                      attrib.normals[3 * index.normal_index + 2]};
            }

            if (index.texcoord_index >= 0)
            {
                vertex.uv = Vec2 {attrib.texcoords[2 * index.texcoord_index + 0],
                                  attrib.texcoords[2 * index.texcoord_index + 1]};
            }

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(objVertices.size());
                objVertices.push_back(vertex);
            }
            objIndices.push_back(uniqueVertices[vertex]);
        }
    }

    using Vulkan::Mesh;

    SetMesh(Mesh({sizeof(ModelVertex3D),
                                 objVertices.data(),
                                 static_cast<uint32_t>(objVertices.size())},{objIndices.data(),
                                 static_cast<uint32_t>(objIndices.size())}));
}

void Model::UpdateMeshIndexed(uint32_t currentFrame, const Vulkan::Mesh::VertexData& vertexData, const Vulkan::Mesh::IndexData& indices)
{
    mesh.UpdateIndexed(vertexData, indices, currentFrame);
}

void Model::UpdateMesh(uint32_t currentFrame, const Vulkan::Mesh::VertexData& vertexData)
{
    mesh.Update(vertexData, currentFrame);
}

void Model::SetMesh(Vulkan::Mesh&& newMesh)
{
    mesh = std::move(newMesh);
}

void Model::Bind(Vulkan::CommandBuffer& commandBuffer, uint32_t frameIndex)
{
    mesh.Bind(commandBuffer, frameIndex);
}

void Model::BindIndexed(Vulkan::CommandBuffer& commandBuffer, uint32_t frameIndex)
{
    mesh.BindIndexed(commandBuffer, frameIndex);
}

void Model::Draw(Vulkan::CommandBuffer& commandBuffer, uint32_t currentFrame, const uint32_t& instances)
{
    vkCmdDraw(commandBuffer.Get(), mesh.GetVertexCount(currentFrame), 1, 0, instances);
}

void Model::DrawIndexed(Vulkan::CommandBuffer& commandBuffer, const uint32_t frameIndex, const uint32_t& instances)
{
    vkCmdDrawIndexed(commandBuffer.Get(), mesh.GetIndexCount(frameIndex), 1, 0, 0, instances);
}
} // namespace Siege
