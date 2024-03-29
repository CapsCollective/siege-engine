//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BUFFER_H
#define SIEGE_ENGINE_BUFFER_H

#include "render/renderer/platform/vulkan/utils/Types.h"

namespace Siege::Buffer
{
struct Buffer
{
    VkBuffer buffer {nullptr};
    VkDeviceMemory bufferMemory {nullptr};
    uint64_t size = 0;
};

/**
 * Creates a memory buffer for transferring data to our GPU. Allocates resulting data to the
 *'buffer' and 'bufferMemory' variables respectively.
 *
 * @param size - specifies the size of the buffer.
 * @param usage - specifies what the buffer will be used for (i.e: vertex definitions).
 * @param properties - specifies the the properties the buffer should have.
 * @param buffer - the buffer that the function should write data to.
 * @param bufferMemory - the buffer memory data the function should write data to.
 **/
void CreateBuffer(unsigned long size,
                  unsigned int usage,
                  unsigned int properties,
                  VkBuffer& buffer,
                  VkDeviceMemory& bufferMemory);

/**
 * Returns a bitmask value representing the memory type required to allocate GPU memory.
 * Different rendering operations require different memory types. These are typically
 * differentiated using a bitmask. This functions throws an exception if the required
 * memory type is unavailable.
 * @param typeFilter - The bitmask for the required memory type
 * @param properties - The available device memory properties. Used to query for available memory.
 **/
uint32_t FindMemoryType(unsigned long typeFilter, Vulkan::Utils::MemoryProperty properties);

/**
 * Copies data into a buffer. Data can be any type as specified by
 * the templated type.
 *
 * @param dstBuffer - the buffer which data should be copied to.
 * @param size - the size of the buffer
 * @param bufferData - the data being copied into the buffer
 **/
void CopyData(Buffer& dstBuffer,
              unsigned long size,
              const void* bufferData,
              unsigned long offset = 0);

void AppendData(Buffer& dstBuffer, unsigned long size, const void* bufferData);

/**
 * Copies a buffer. This is generally used when copying the values within a buffer into
 * another (such as when setting up a staging buffer between the CPU and GPU).
 *
 * @param srcBuffer - the buffer being copied.
 * @param dstBuffer - the buffer where the data is being copied into.
 * @param size - the expected size of the resulting buffer.
 **/
void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, unsigned long size);

/**
 * Destroys a buffer struct and releases memory back to the device.
 *
 * @param buffer - the buffer to be destroyed.
 **/
void DestroyBuffer(Buffer& buffer);

size_t PadUniformBufferSize(size_t originalSize);
size_t PadStorageBufferSize(size_t originalSize);
} // namespace Siege::Buffer

#endif