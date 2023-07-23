//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_VERTEXBUFFER_H
#define SIEGE_ENGINE_RENDER_VERTEXBUFFER_H

#include <cstdint>

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

class VertexBuffer
{
public:

    // 'Structor

    /**
     * The default constructor for the VertexBuffer
     */
    VertexBuffer() = default;

    /**
     * A constructor for the VertexBuffer which accepts vertex data during construction
     * @param data the vertex data to be copied in
     * @param bufferSize the size of the data in bytes
     */
    VertexBuffer(void* data, unsigned long bufferSize);

    /**
     * A zero-data constructor for the VertxBuffer. Initialises a vertex buffer with no data but
     * with the given size
     * @param bufferSize the total size of the buffer in bytes
     */
    VertexBuffer(unsigned long bufferSize);

    /**
     * The move constructor from another VertexBuffer
     * @param other the other vertex buffer to move
     */
    VertexBuffer(VertexBuffer&& other);

    /**
     * The copy constructor from another VertexBuffer
     * @param other the other vertex buffer to copy from
     */
    VertexBuffer(const VertexBuffer& other);

    /**
     * The VertexBuffer destructor
     */
    ~VertexBuffer();

    // Operators

    /**
     * The copy assignment operator. Copies the values of one Vertex Buffer to another
     * @param other the Vertex Buffer to copy
     * @return a reference to the current VertexBuffer
     */
    VertexBuffer& operator=(const VertexBuffer& other);

    /**
     * The move assignment operator. Moves the values of one Vertex Buffer to another
     * @param other the Vertex buffer being moved
     * @return a reference to the current VertexBuffer
     */
    VertexBuffer& operator=(VertexBuffer&& other);

    // Functions

    /**
     * Copies the provided data into the buffer
     * @param data the data to be copied
     * @param size the size of the data in bytes
     * @param offset the offset to copy the data to within the buffer
     */
    void Copy(const void* data, unsigned long size, unsigned long offset = 0);

    /**
     * Binds the VertexBuffer for rendering
     * @param commandBuffer the command buffer the binding should apply to
     * @param offset the offset of the buffer to bind in bytes
     * @param binding the vertex binding ID to use
     */
    void Bind(const CommandBuffer& commandBuffer, const uint64_t* offset, unsigned int binding = 0);

    /**
     * Frees the contents of the VertexBuffer
     */
    void Free();

    /**
     * Casts the contents of the buffer to a specific type
     * @tparam T the type to cast the contents to
     * @param offset the offset to retrieve in bytes
     * @return the entire buffer cast to the requested type
     */
    template<typename T>
    const T* Cast(unsigned long offset = 0)
    {
        return reinterpret_cast<T*>(rawBuffer) + offset;
    }

private:

    /**
     * Swaps the contents of two VertexBuffers
     * @param other the VertexBuffer to swap with
     */
    void Swap(VertexBuffer& other);

    /**
     * Allocates the memory for the VertexBuffer
     * @param device the device used for allocation
     */
    void Allocate(VkDevice device);

    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    unsigned long size {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_RENDER_VERTEXBUFFER_H
