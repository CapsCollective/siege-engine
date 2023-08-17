//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SEMAPHORE_H
#define SIEGE_ENGINE_SEMAPHORE_H

#include "utils/Types.h"

namespace Siege::Vulkan
{
/**
 * The Semaphore class is a thin wrapper over the Vulkan VkSemaphore type. This type supports both
 * single and multiple semaphore instances, specifically for use cases such as synchronisation
 * across multiple frames
 */
class Semaphore
{
public:

    /**
     * Default empty constructor for the Semaphore class;
     */
    Semaphore() = default;

    /**
     * The main constructor for the Semaphore class. This constructor defaults to creating a single
     * Semaphore instance, but can create multiple within the same contiguous memory block (as an
     * array)
     * @param semaphoresCount How many semaphore instances to create. A value of 1 means the
     * creation of a single instance
     */
    explicit Semaphore(size_t semaphoresCount);

    /**
     * Move constructor for the semaphore class. Utilises the Copy & Swap idiom
     * @param other the other semaphore to be moved
     */
    inline Semaphore(Semaphore&& other) noexcept
    {
        Swap(other);
    }

    /**
     * Move assignment operator. Assigns the right hand expression to the left hand variable
     * @param other the semaphore to move
     * @return a reference to the assigned semaphore
     */
    inline Semaphore& operator=(Semaphore&& other) noexcept
    {
        Swap(other);
        return *this;
    }

    /**
     * Returns the raw vulkan semaphore. Can be given an index to access other semaphore stored
     * in the same collection
     * @param index the index of the Semaphore you want to access. Defaults to zero
     * @return the raw vulkan semaphore pointer
     */
    inline VkSemaphore Get(size_t index = 0)
    {
        return semaphores[index];
    }

    /**
     * Subscript operator. Used to index into specific vulkan semaphore pointers
     * @param index the index of the semaphore to be accessed
     * @return the raw vulkan semaphore pointer
     */
    inline VkSemaphore operator[](size_t index)
    {
        return semaphores[index];
    }

    /**
     * Destructor for the semaphore
     */
    ~Semaphore();

private:

    /**
     * Swaps the values of two semaphores
     * @param other the semaphore to swap values with
     */
    void Swap(Semaphore& other);

    VkSemaphore* semaphores {nullptr};
    size_t size {0};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_SEMAPHORE_H
