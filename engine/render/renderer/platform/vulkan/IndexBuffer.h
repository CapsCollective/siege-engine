//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_INDEX_BUFFER_H
#define SIEGE_ENGINE_RENDER_INDEX_BUFFER_H

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

class IndexBuffer
{
public:

    // 'Structors

    IndexBuffer() = default;
    IndexBuffer(void* data, unsigned long long dataSize);
    IndexBuffer(unsigned long bufferSize);
    IndexBuffer(const IndexBuffer& other);
    IndexBuffer(IndexBuffer&& other);
    ~IndexBuffer();

    // Operator overload

    IndexBuffer& operator=(const IndexBuffer& other);
    IndexBuffer& operator=(IndexBuffer&& other);

    // Functions

    void Free();
    void Copy(const void* data, unsigned long size, unsigned long offset = 0);
    void Bind(const CommandBuffer& commandBuffer, unsigned long long offset = 0);

private:

    void Swap(IndexBuffer& other);
    void Allocate(VkDevice device);
    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    unsigned long size {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_RENDER_INDEX_BUFFER_H
