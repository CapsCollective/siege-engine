//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VERTEXBUFFER_H
#define SIEGE_ENGINE_VERTEXBUFFER_H

#include <cstdint>

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

/**
 * This VertexBuffer class stores all per-vertex and per-instance data to be bound. This buffer
 * stores data which can be bound to the vertex shader
 */
class VertexBuffer
{
public:

    // 'Structors

    /**
     * The default empty constructor for the VertexBuffer
     */
    VertexBuffer() = default;

    /**
     * A base constructor which allocates the vertex buffer memory, but assigns no data to it
     * @param newSize the size to allocate to the vertex buffer
     */
    VertexBuffer(uint32_t newSize);

    /**
     * A constructor for the vertex buffer which allocates the buffer's size and assigns the vertex
     * memory.
     * @param vertexSize the size of the base vertex object
     * @param vertices an array of vertices to be assigned to the buffer
     * @param vertexCount the number of vertices being allocated
     */
    VertexBuffer(uint32_t vertexSize, void* vertices, uint32_t vertexCount);

    /**
     * A base constructor which allocates the vertex buffer memory without assigning data to it
     * @param vertexSize the size of the base vertex object
     * @param vertexCount the number of vertices being allocated
     */
    VertexBuffer(uint32_t vertexSize, uint32_t vertexCount);

    /**
     * A move constructor for the VertexBuffer class
     * @param other the Vertex buffer to move
     */
    VertexBuffer(VertexBuffer&& other);

    /**
     * A VertexBuffer destructor
     */
    ~VertexBuffer();

    // Operator Overloads

    /**
     * A move assignment operator
     * @param other the VertexBuffer to move
     * @return a reference to the current VertexBuffer object
     */
    inline VertexBuffer& operator=(VertexBuffer&& other)
    {
        Swap(other);
        return *this;
    }

    // Public Functions:

    /**
     * Frees the memory stored by the vertex buffer
     */
    void Free();

    /**
     * Updates the vertexBuffer's contents
     * @param vertexSize the size of the base vertex object
     * @param vertices an array of vertices to be assigned to the buffer
     * @param vertexCount the number of vertices to be assigned to the buffer
     * @param offset an offset used to copy data into the buffer
     */
    void Update(uint32_t vertexSize, void* vertices, uint32_t vertexCount, uint32_t offset = 0);

    /**
     * Bind the buffer to the GPU
     * @param commandBuffer the command buffer to bind the vertex buffer to
     * @param offsets an array of offsets to bind to
     * @param offsetCount the number of offsets to bind
     * @param binding the binding location to bind to
     * @param firstOffset the first offset to bind into
     */
    void Bind(CommandBuffer& commandBuffer,
              uint64_t* offsets = nullptr,
              uint32_t offsetCount = 1,
              uint32_t binding = 0,
              uint64_t firstOffset = 0);

private:

    /**
     * Swaps the contents to two vertex buffers
     * @param other the vertex buffer to swap with
     */
    void Swap(VertexBuffer& other);

    /**
     * Allocates memory for the vertex buffer
     * @param device the device to allocate memory from
     */
    void Allocate(VkDevice device);

    /**
     * Copies an array of data into the buffer
     * @param device the GPU device to copy memory from
     * @param newSize the size of the memory to be copied
     * @param data the data to be copied into the buffer
     * @param offset the offset to copy data into
     */
    void Copy(VkDevice device, uint32_t newSize, void* data, uint32_t offset = 0);

    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    uint32_t size {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VERTEXBUFFER_H
