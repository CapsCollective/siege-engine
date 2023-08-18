//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "DescriptorPool.h"

#include "render/renderer/platform/vulkan/Context.h"
#include "render/renderer/platform/vulkan/utils/Descriptor.h"

namespace Siege
{
Vulkan::Utils::UniformAllocator DescriptorPool::descriptorPool {nullptr};
MHArray<Vulkan::Utils::UniformAllocation> DescriptorPool::sizes;

void DescriptorPool::AddPoolSize(const Vulkan::Utils::UniformType type, const uint32_t size)
{
    sizes.Append({type, size});
}

void DescriptorPool::Initialise()
{
    using namespace Vulkan::Utils;
    sizes = MHArray<UniformAllocation>(Descriptor::MAX_DESCRIPTOR_POOL_SIZES);
}

void DescriptorPool::BuildPool()
{
    using namespace Vulkan::Utils;
    descriptorPool = Descriptor::CreateDescriptorPool(Vulkan::Context::GetVkLogicalDevice(),
                                                      sizes.Data(),
                                                      sizes.Count());
}

void DescriptorPool::DestroyPool()
{
    Vulkan::Utils::Descriptor::DestroyDescriptorPool(Vulkan::Context::GetVkLogicalDevice(),
                                                     descriptorPool);
}
} // namespace Siege
