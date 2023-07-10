//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_VBUFFER_H
#define SIEGE_ENGINE_VBUFFER_H

#include "CommandBuffer.h"
#include "utils/Types.h"

namespace Siege::Vulkan
{

class VBuffer
{
public:

    // 'Structor

    VBuffer() = default;
    VBuffer(void* data, unsigned long bufferSize);
    VBuffer(unsigned long bufferSize);
    VBuffer(VBuffer&& other);
    VBuffer(const VBuffer& other);
    ~VBuffer();

    // Operators

    VBuffer& operator=(const VBuffer& other);
    VBuffer& operator=(VBuffer&& other);

    // Functions

    void Copy(void* data, unsigned long size, unsigned long offset = 0);
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

    void Allocate(void* data, unsigned long long newSize);
    void Swap(VBuffer& other);
    void* rawBuffer {nullptr};
    VkBuffer buffer {nullptr};
    VkDeviceMemory memory {nullptr};
    unsigned long size {0};
};

} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_VBUFFER_H
