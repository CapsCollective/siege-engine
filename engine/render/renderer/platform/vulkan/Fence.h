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
/**
 * The Fence class is a thin wrapper around the Vulkan VkFence type. This type supports both single
 * and multiple Fence instances (such as frame synchronisation). By default, the fences are
 * initialised as null values
 */
class Fence
{
public:

    /**
     * The initial state of the Fence
     */
    enum FenceInitState
    {
        FENCE_EMPTY = 0,
        FENCE_NULL = 1
    };

    /**
     * Zero-value constructor. Initialises the Fence object with null values to begin with
     */
    Fence() : size {0}, initState {FENCE_NULL} {}

    /**
     * Initialises the Fence class. Can be initiated to store a contiguous block of fences, or can
     * be used to create only a single fence
     * @param fenceCount the number of fences to be initialised
     * @param initState the initialisation state - can either be NULL or EMPTY
     */
    Fence(size_t fenceCount, FenceInitState = FENCE_EMPTY);

    /**
     * Move constructor for the Fence class
     * @param other the Fence class to be moved
     */
    inline Fence(Fence&& other)
    {
        Swap(other);
    };

    /**
     * Destructor for the Fence class
     */
    ~Fence();

    /**
     * Move assignment operator
     * @param other the Fence class to be moved
     * @return a reference to the created Fence
     */
    inline Fence& operator=(Fence&& other)
    {
        Swap(other);
        return *this;
    }

    /**
     * Subscript operator to index into the stored fences
     * @param index the index to access
     * @return the Fence that's stored in the index location
     */
    inline VkFence& operator[](size_t index)
    {
        return Get(index);
    }

    // TODO (Aryeh): Add some form of bounds checking?
    /**
     * Gets the raw vulkan Fence pointer stored by the wrapper
     * @warn this method does not bounds check the index. It is the user's responsibility to make
     * sure the index is valid
     * @param index the index to access, defaults to 0
     * @return the pointer to the raw vulkan pointer
     */
    inline VkFence& Get(size_t index = 0)
    {
        return fences[index];
    }

    /**
     * Sets the Fence stored at the provided index
     * @param index the index to store the fence in
     * @param fence the fence to store in the collection
     */
    inline void Set(size_t index, VkFence fence)
    {
        fences[index] = fence;
    }

    /**
     * Waits for a fence to become available
     * @param idx the index of the fence to wait for
     */
    void Wait(const size_t idx = 0);

    /**
     * Resets the state of the provided fence
     * @param idx the index of the fence to reset
     */
    void Reset(const size_t idx = 0);

private:

    /**
     * Swaps the values of a fence with the contents of another
     * @param other the fence to swap with
     */
    void Swap(Fence& other);

    VkFence* fences {nullptr};
    size_t size {0};
    FenceInitState initState {FENCE_NULL};
};
} // namespace Siege::Vulkan

#endif // SIEGE_ENGINE_FENCE_H
