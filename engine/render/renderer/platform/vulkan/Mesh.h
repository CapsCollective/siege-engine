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
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The Mesh class represents a Vulkan implementation of a Mesh. This class stores Vulkan vertex and
 * index buffers. It also supports binding and drawing of all stored meshes
 */
class Mesh
{
public:

    /**
     * A struct which stores vertex buffer configuration data
     */
    struct VertexData
    {
        uint64_t vertexSize {0};
        void* vertices {nullptr};
        uint32_t count {0};
    };

    /**
     * A struct which stores index buffer configuration data
     */
    struct IndexData
    {
        uint32_t* indices {nullptr};
        uint32_t count {0};
    };

    // Constexpr variables

    // These values are totally arbitrary right now, just need some form of upper bound
    static constexpr uint32_t MAX_VERTICES = 10000;
    static constexpr uint32_t MAX_INDICES = 100000;

    // 'Structors

    /**
     * A default constructor for the Mesh class
     */
    Mesh() = default;

    Mesh(uint32_t vertexSize, uint32_t vertexCount, uint32_t indexCount);

    /**
     * A non-indexed Mesh class. This only creates a vertex buffer and uses no index buffers
     * @param vertices a VertexData struct containing all our vertices
     */
    Mesh(const VertexData& vertices);

    /**
     * An indexed Mesh constructor. This creates both a vertex and index buffer
     * @param vertices a VertexData struct containing all our vertices
     * @param indices an IndexData struct containing all our indices
     */
    Mesh(const VertexData& vertices, const IndexData& indices);

    /**
     * A move constructor
     * @param other the Mesh being moved
     */
    inline Mesh(Mesh&& other) noexcept
    {
        Swap(other);
    }

    /**
     * The Mesh class' destructor
     */
    ~Mesh();

    // Operator overloads

    /**
     * The Mesh's move assignment operator
     * @param other the Mesh to move
     * @return a reference to the new Mesh
     */
    inline Mesh& operator=(Mesh&& other)
    {
        Swap(other);
        return *this;
    }

    // Public functions

    /**
     * A function to manually free all Mesh vulkan memory
     */
    void Free();

    /**
     * Binds the Vertex buffer without binding an index buffer
     * @param commandBuffer the command buffer to bind to
     * @param frameIndex the index of the frame we're currently on
     */
    void Bind(CommandBuffer& commandBuffer, uint32_t frameIndex);

    /**
     * Binds both the vertex and index buffers to the command buffer
     * @param commandBuffer the command buffer to bind to
     * @param frameIndex the index of the frame we're currently on
     */
    void BindIndexed(CommandBuffer& commandBuffer, uint32_t frameIndex);

    /**
     * Updates the data stored in our vertex buffer
     * @param vertices a VertexData struct with our vertex configuration data
     * @param currentFrameIndex the index of the current frame
     */
    void Update(const VertexData& vertices, uint32_t currentFrameIndex);

    /**
     * Updates the data stored in both the vertex and index buffers
     * @param vertices a VertexData struct with our vertex configuration data
     * @param indices an IndexData struct with our index configuration data
     * @param currentFrameIndex the index of the current frame
     */
    void UpdateIndexed(const VertexData& vertices,
                       const IndexData& indices,
                       uint32_t currentFrameIndex);

    // Getters and Setters

    /**
     * Returns the number of indices stored for the current frame
     * @param frameIndex the index of the current frame
     * @return the number of indices stored for the current frame
     */
    const uint32_t GetIndexCount(uint32_t frameIndex) const
    {
        return perFrameIndexBuffers[frameIndex].GetCount();
    }

    /**
     * Returns the number of vertices stored for the current frame
     * @param frameIndex the index of the current frame
     * @return the number of vertices for the current frame
     */
    const uint32_t GetVertexCount(uint32_t frameIndex) const
    {
        return perFrameVertexBuffers[frameIndex].GetCount();
    }

private:

    /**
     * Swaps the values of two Mesh classes
     * @param other the Mesh to swap values with
     */
    void Swap(Mesh& other);

    /**
     * Sets the data in our vertex buffer. If the provided vertices is larger than what is stored, a
     * reallocation occurs. Be careful when using this method every frame
     * @param vertices a VertexData class containing our vertex configuration data
     * @param currentFrame the index of the current frame
     */
    void SetVertexBuffers(const VertexData& vertices, uint32_t currentFrame);

    /**
     * Sets the data in our index buffer. If the provided indices is larger than what is stored, a
     * reallocation occurs. Be careful when using this method every frame
     * @param indices an IndexData class containing our index configuration data
     * @param currentFrame the index of the current frame
     */
    void SetIndexBuffers(const IndexData& indices, uint32_t currentFrame);

    // NOTE: Because Vulkan is multithreaded we need to store a buffer for every frame. Since we
    // don't want to wait on a frame to release its resources, we simply switch between our buffers
    // for every frame in flight
    MHArray<IndexBuffer> perFrameIndexBuffers;
    MHArray<VertexBuffer> perFrameVertexBuffers;
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VULKAN_MESH_H
