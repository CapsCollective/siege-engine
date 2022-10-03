//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_FENCE_H
#define SIEGE_ENGINE_FENCE_H

#include "utils/Types.h"

namespace Siege::Vulkan
{
class Fence
{
public:
    enum FenceInitState
    {
        FENCE_EMPTY = 0,
        FENCE_NULL = 1
    };

    Fence(): size{0}, initState{FENCE_NULL} {}
    Fence(size_t fenceCount, FenceInitState = FENCE_EMPTY);

    inline Fence(Fence&& other) { Swap(other); };

    inline Fence& operator=(Fence&& other)
    {
        Swap(other);
        return *this;
    }

    inline VkFence& operator[](size_t index) {return Get(index); }
    inline VkFence& Get(size_t index) { return fences[index]; }
    inline void Set(size_t index, VkFence fence) { fences[index] = fence; }

    void Wait(const size_t idx = 0);
    void Reset(const size_t idx = 0);
    ~Fence();
private:
    void Swap(Fence& other);
    VkFence* fences {nullptr};
    size_t size {0};
    FenceInitState initState{FENCE_NULL};
};
}

#endif // SIEGE_ENGINE_FENCE_H
