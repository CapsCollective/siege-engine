//
// Copyright (c) 2020-present Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "WavefrontDataPacker.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <resources/StaticMeshData.h>
#include <tiny_obj_loader.h>
#include <utils/Logging.h>

#include <unordered_map>

using Siege::BaseVertex;
using Siege::FColour;
using Siege::Vec2;
using Siege::Vec3;

void* PackWavefrontFile(const Siege::String& filePath)
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

    Siege::StaticMeshData* staticMeshData = Siege::StaticMeshData::Create(objIndices, objVertices);
    return staticMeshData;
}
