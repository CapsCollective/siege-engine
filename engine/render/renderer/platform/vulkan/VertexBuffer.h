//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_VERTEXBUFFER_H
#define SIEGE_ENGINE_RENDER_VERTEXBUFFER_H

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

class VertexBuffer
{
public:

    // 'Structor

    VertexBuffer() = default;
    VertexBuffer(void* data, unsigned long bufferSize);
    VertexBuffer(unsigned long bufferSize);
    VertexBuffer(VertexBuffer&& other);
    VertexBuffer(const VertexBuffer& other);
    ~VertexBuffer();

    // Operators

    VertexBuffer& operator=(const VertexBuffer& other);
    VertexBuffer& operator=(VertexBuffer&& other);

    // Functions

    void Copy(const void* data, unsigned long size, unsigned long offset = 0);
    void Bind(const CommandBuffer& commandBuffer,
              unsigned long long* offset,
              unsigned int binding = 0);
    void Free();

    template<typename T>
    const T* Cast(unsigned long offset = 0)
    {
        return reinterpret_cast<T*>(rawBuffer) + offset;
    }

private:

    void Swap(VertexBuffer& other);
    void Allocate(VkDevice device);
    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    unsigned long size {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_RENDER_VERTEXBUFFER_H
