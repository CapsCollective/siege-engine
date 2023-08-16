//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_INDEX_BUFFER_H
#define SIEGE_ENGINE_RENDER_INDEX_BUFFER_H

#include <cstdint>

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

class IndexBuffer
{
public:

    // 'Structors

    /**
     * The default constructor for the IndexBuffer class
     */
    IndexBuffer() = default;

    /**
     * The IndexBuffer base constructor. Accepts index data along with a size to construct the
     * buffer with
     * @param data the data to copy into the buffer
     * @param dataSize the size of the data in bytes
     */
    IndexBuffer(void* data, uint64_t dataSize);

    /**
     * A zero copy constructor. Initialises the IndexBuffer with a maximum size but no data
     * @param bufferSize the total size of the buffer in bytes
     */
    IndexBuffer(unsigned long bufferSize);

    /**
     * An IndexBuffer copy constructor. Creates a new IndexBuffer with the contents of another
     * IndexBuffer
     * @param other the index buffer to copy data from
     */
    IndexBuffer(const IndexBuffer& other);

    /**
     * An IndexBuffer move constructor. Creates a new IndexBuffer with the moved memory from another
     * IndexBuffer
     * @param other the IndexBuffer to move data from
     */
    IndexBuffer(IndexBuffer&& other);

    /**
     * The IndexBuffer destructor. Frees all memory and resets the IndexBuffer
     */
    ~IndexBuffer();

    // Operator overload

    /**
     * The IndexBuffer copy assignment operator. Copies the value from one IndexBuffer into another
     * @param other the IndexBuffer to copy data from
     * @return a reference to the current IndexBuffer
     */
    IndexBuffer& operator=(const IndexBuffer& other);

    /**
     * The IndexBuffer move assignment operator. Moves the value from one IndexBuffer into another
     * @param other the IndexBuffer to move data from
     * @return a reference to the current IndexBuffer
     */
    IndexBuffer& operator=(IndexBuffer&& other);

    // Functions

    /**
     * Frees all memory from the IndexBuffer and resets all values
     */
    void Free();

    /**
     * Copies the provided data into the IndexBuffer with an offset
     * @param data the data to be copied into the buffer
     * @param size the size in bytes of the data
     * @param offset the byte offset to copy the data to
     */
    void Copy(const void* data, unsigned long size, unsigned long offset = 0);

    /**
     * Binds the IndexBuffer for rendering
     * @param commandBuffer the command buffer to bind to
     * @param offset the byte offset to bind
     */
    void Bind(const CommandBuffer& commandBuffer, uint64_t offset = 0);

private:

    /**
     * Swaps the contents of two buffers
     * @param other the buffer to swap values with
     */
    void Swap(IndexBuffer& other);

    /**
     * Allocates memory for the buffer
     * @param device the device to allocate memory from
     */
    void Allocate(VkDevice device);

    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    unsigned long size {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_RENDER_INDEX_BUFFER_H
