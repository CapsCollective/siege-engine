//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SEMAPHORE_H
#define SIEGE_ENGINE_SEMAPHORE_H

#include "utils/Types.h"

namespace Siege::Vulkan
{
class Semaphore
{
public:

    Semaphore() = default;

    Semaphore(size_t semaphoresCount);

    inline Semaphore(Semaphore&& other) { Swap(other); }

    inline Semaphore& operator=(Semaphore&& other)
    {
        Swap(other);
        return *this;
    }

    inline VkSemaphore Get(size_t index) { return semaphores[index]; }

    inline VkSemaphore operator[](size_t index) { return semaphores[index]; }

    ~Semaphore();
private:

    void Swap(Semaphore& other);

    VkSemaphore* semaphores {nullptr};
    size_t size {0};
};
}

#endif // SIEGE_ENGINE_SEMAPHORE_H
