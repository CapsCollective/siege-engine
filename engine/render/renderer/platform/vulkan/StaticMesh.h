//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VULKAN_MESH_H
#define SIEGE_ENGINE_VULKAN_MESH_H

#include <utils/collections/HeapArray.h>

#include <cstdint>

#include "CommandBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The Mesh class represents a Vulkan implementation of a Mesh. This class stores Vulkan vertex and
 * index buffers. It also supports binding and drawing of all stored meshes
 */
class StaticMesh
{
public:

    struct SubMesh
    {
        unsigned int baseVertex;
        unsigned int baseIndex;
        unsigned int materialIndex;
        unsigned int vertexCount;
        unsigned int indexCount;
    };
    // Constexpr variables

    // These values are totally arbitrary right now, just need some form of upper bound
    static constexpr unsigned int MAX_VERTICES = 10000;
    static constexpr unsigned int MAX_INDICES = 100000;

    // 'Structors

    /**
     * A default constructor for the Mesh class
     */
    StaticMesh() = default;

    StaticMesh(unsigned int vertexBufferSize,
               unsigned int vertCount,
               unsigned int indexBufferSize,
               unsigned int idxCount,
               unsigned int subMeshCount);

    StaticMesh(const char* filePath, Material* material);

    /**
     * A move constructor
     * @param other the Mesh being moved
     */
    inline StaticMesh(StaticMesh&& other) noexcept
    {
        Swap(other);
    }

    inline StaticMesh(const StaticMesh& other) :
        subMeshes {other.subMeshes},
        materials {other.materials},
        indexBuffer {other.indexBuffer},
        vertexBuffer {other.vertexBuffer},
        vertexCount {other.vertexCount},
        indexCount {other.indexCount}
    {}

    /**
     * The Mesh class' destructor
     */
    ~StaticMesh();

    // Operator overloads

    inline StaticMesh& operator=(StaticMesh&& other)
    {
        Swap(other);
        return *this;
    }

    inline StaticMesh& operator=(const StaticMesh& other)
    {
        indexBuffer = other.indexBuffer;
        vertexBuffer = other.vertexBuffer;

        vertexCount = other.vertexCount;
        indexCount = other.indexCount;

        subMeshes = other.subMeshes;
        materials = other.materials;
        return *this;
    }

    // Public functions

    /**
     * A function to manually free all Mesh vulkan memory
     */
    void Free();

    void Bind(CommandBuffer& commandBuffer, uint64_t offset);

    void BindIndexed(CommandBuffer& commandBuffer,
                     uint64_t vertOffset = 0,
                     uint64_t indexOffset = 0);

    // Getters and Setters

    inline Material* GetMaterial(size_t index)
    {
        return materials[index];
    }

    inline MHArray<SubMesh>& GetSubMeshes()
    {
        return subMeshes;
    }

private:

    /**
     * Swaps the values of two Mesh classes
     * @param other the Mesh to swap values with
     */
    void Swap(StaticMesh& other);

    MHArray<SubMesh> subMeshes;
    MHArray<Material*> materials;

    IndexBuffer indexBuffer;
    VertexBuffer vertexBuffer;

    unsigned int vertexCount {0};
    unsigned int indexCount {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_MESH_H
