//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INDEXBUFFER_H
#define SIEGE_ENGINE_INDEXBUFFER_H

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The index buffer wrapper class. This class represents a buffer which stores vertex index values.
 * The base assumption is that all indices are stored as 32-bit unsigned integers
 */
class IndexBuffer
{
public:

    // 'Structors

    /**
     * A default Index buffer constructor
     */
    IndexBuffer() = default;

    /**
     * An index buffer constructor. This constructor allocates the index buffer memory and assigns
     * the values to the buffer
     * @param count the number of indices to be stored in the buffer
     * @param indices an array storing the buffer indices
     */
    IndexBuffer(unsigned int count, const unsigned int* indices);

    /**
     * An index buffer constructor. This constructor allocates the memory needed by the buffer
     * without assigning any data to it
     * @param count the number of indices to allocate memory for
     */
    IndexBuffer(unsigned int count);

    /**
     * A move constructor for the index buffer.
     * @param other the index buffer to move data from
     */
    inline IndexBuffer(IndexBuffer&& other)
    {
        Swap(other);
    }

    /**
     * An index buffer destructor. Frees all memory held by the buffer
     */
    inline ~IndexBuffer()
    {
        Free();
    }

    // Operator Overloads

    /**
     * An index buffer move assignment operator. Creates a new index buffer and moves its contents
     * into the current buffer
     * @param other the buffer to move
     * @return a reference to the current buffer
     */
    inline IndexBuffer& operator=(IndexBuffer&& other)
    {
        Swap(other);
        return *this;
    }

    // Public Methods

    /**
     * Updates the data in the index buffer
     * @param data an array of indices to be copied into the buffer
     * @param indexCount the number of indices to copy in
     */
    void Update(unsigned int* data, unsigned int indexCount);

    /**
     * Binds the buffer memory to the command buffer
     * @param commandBuffer the command buffer to bind to
     */
    void Bind(CommandBuffer& commandBuffer);

    /**
     * Frees the memory held by the buffer
     */
    void Free();

    // Getters

    /**
     * Returns the number of indices held by the bufer
     * @return
     */
    inline unsigned long GetCount() const
    {
        return count;
    }

private:

    static constexpr size_t INDEX_BUFFER_ELEMENT_SIZE = sizeof(unsigned int);

    /**
     * Swaps the contents of two index buffers
     * @param other the index buffer to swap contents with
     */
    void Swap(IndexBuffer& other);

    /**
     * Allocates memory for the buffer
     * @param device
     */
    void Allocate(VkDevice device);

    /**
     * Copies contents from an array of indices into the buffer
     * @param device the GPU device used to copy data into
     * @param newSize the size of the data being copied into the buffer
     * @param data the array of data being copied into the buffer
     */
    void Copy(VkDevice device, unsigned int newSize, const unsigned int* data);

    unsigned int size {0};
    unsigned int count {0};
    unsigned int* rawBuffer {nullptr};

    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_INDEXBUFFER_H
