//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DESCRIPTOR_POOL_H
#define SIEGE_ENGINE_DESCRIPTOR_POOL_H

#include <utils/collections/HeapArray.h>

#include "render/renderer/platform/vulkan/utils/Types.h"

namespace Siege
{
// TODO(Aryeh): Remove the statics in this file (remember to create constructors and Swap functions)
class DescriptorPool
{
public:

    static void Initialise();
    static void AddPoolSize(const Vulkan::Utils::UniformType type, const unsigned int size);

    static inline Vulkan::Utils::UniformAllocator& GetDescriptorPool()
    {
        return descriptorPool;
    }

    static void BuildPool();
    static void DestroyPool();

private:

    static Vulkan::Utils::UniformAllocator descriptorPool;
    static MHArray<Vulkan::Utils::UniformAllocation> sizes;
};
} // namespace Siege

#endif
