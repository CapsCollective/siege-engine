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

class VertexBuffer
{
public:

    // 'Structors

    VertexBuffer() = default;
    VertexBuffer(uint32_t newSize);
    VertexBuffer(uint32_t vertexSize, void* vertices, uint32_t vertexCount);
    VertexBuffer(uint32_t vertexSize, uint32_t vertexCount);
    VertexBuffer(VertexBuffer&& other);
    ~VertexBuffer();

    // Operator Overloads

    inline VertexBuffer& operator=(VertexBuffer&& other)
    {
        Swap(other);
        return *this;
    }

    // Public Functions:

    void Free();
    void Update(uint32_t vertexSize, void* vertices, uint32_t vertexCount, uint32_t offset = 0);
    void Bind(CommandBuffer& commandBuffer,
              uint64_t* offsets = nullptr,
              uint32_t offsetCount = 1,
              uint32_t binding = 0,
              uint64_t firstOffset = 0);

    // Getters

    //    inline uint32_t GetCount() const
    //    {
    //        return count;
    //    }

private:

    void Swap(VertexBuffer& other);
    void Allocate(VkDevice device);
    void Copy(VkDevice device, uint32_t newSize, void* data, uint32_t offset = 0);

    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    uint32_t size {0};
    // uint32_t count {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VERTEXBUFFER_H
