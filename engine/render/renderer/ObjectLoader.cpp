//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ObjectLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <utils/Logging.h>
#include <utils/math/vec/Hashing.h>

#include <unordered_map>
#include <vector>

namespace std
{
template<>
struct hash<Siege::BaseVertex>
{
    size_t operator()(const Siege::BaseVertex& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    };
};
} // namespace std

namespace Siege
{

ObjectVertexData LoadObjectFromFile(const char* filePath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::vector<BaseVertex> objVertices;
    std::vector<unsigned int> objIndices;
    std::unordered_map<BaseVertex, unsigned int> uniqueVertices {};

    CC_ASSERT(tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath),
              (warn + err).c_str())

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            BaseVertex vertex {};

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
                uniqueVertices[vertex] = static_cast<unsigned int>(objVertices.size());
                objVertices.push_back(vertex);
            }
            objIndices.push_back(uniqueVertices[vertex]);
        }
    }

    return {sizeof(BaseVertex),
            MHArray<BaseVertex>(objVertices.data(), objVertices.size()),
            MHArray(objIndices.data(), objIndices.size())};
}

} // namespace Siege