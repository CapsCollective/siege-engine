//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_INDEXBUFFER_H
#define SIEGE_ENGINE_INDEXBUFFER_H

#include "utils/Types.h"
#include "CommandBuffer.h"

namespace Siege::Vulkan
{
class IndexBuffer
{
public:
    // 'Structors

    IndexBuffer() = default;
    IndexBuffer(unsigned int count, const unsigned int* indices);
    IndexBuffer(unsigned int count);
    inline IndexBuffer(IndexBuffer&& other)
    {
        Swap(other);
    }
    inline ~IndexBuffer() { Free(); }

    // Operator Overloads

    inline IndexBuffer& operator=(IndexBuffer&& other)
    {
        Swap(other);
        return *this;
    }

    // Public Methods

    void Update(unsigned int* data, unsigned int indexCount);
    void Bind(CommandBuffer& commandBuffer);
    void Free();

    // Getters

    inline unsigned long GetCount() const { return count; }
private:
    static constexpr size_t INDEX_BUFFER_ELEMENT_SIZE = sizeof(unsigned int);

    void Swap(IndexBuffer& other);
    void Allocate(VkDevice device);
    void Copy(VkDevice device, unsigned int newSize, const unsigned int* data);

    unsigned int size {0};
    unsigned int count {0};
    unsigned int* rawBuffer {nullptr};

    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_INDEXBUFFER_H
