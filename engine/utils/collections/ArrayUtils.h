//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_ARRAYUTILS_H
#define SIEGE_ENGINE_ARRAYUTILS_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <utility>

namespace Siege
{
class ArrayUtils
{
public:

    /**
     * @brief Allocates a chunk of memory and returns it as a pointer to a type.
     * @tparam T the type of the pointer to allocate.
     * @param dataSize the size of the memory we want to allocate.
     * @return the pointer we allocated.
     */
    template<typename T>
    static inline T* Allocate(const unsigned long& dataSize)
    {
        return static_cast<T*>(malloc(dataSize));
    }

    /**
     * @brief Reallocate a chunk of memory to a new size.
     * @tparam T the type of the pointer.
     * @param ptr the pointer to be reallocated.
     * @param dataSize the size of the new memory chunk.
     * @return the new reallocated pointer.
     */
    template<typename T>
    static inline T* Reallocate(T* ptr, const unsigned long& dataSize)
    {
        return static_cast<T*>(realloc(ptr, dataSize));
    }

    /**
     * Checks if an index is within the bounds of a given size.
     * @param index the index being checked.
     * @param size the size of the array.
     * @return true if the index is in bounds, false if it is not.
     */
    static inline constexpr bool IsInBounds(const unsigned long& index, const unsigned long& size)
    {
        return index < size;
    }

    /**
     * @brief Checks if the index is within bounds of the array. If it is not, an assertion failure
     * will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsInBounds(const unsigned long& index, const unsigned long& size);

    /**
     * Copied data from one array pointer to another.
     * @param dst the array being copied to.
     * @param src the array being copied from.
     * @param size the amount of data to copy.
     */
    static inline void CopyData(void* dst, const void* src, const unsigned long& size)
    {
        memcpy(dst, src, size);
    }

    static unsigned long SetCount(const unsigned long& size,
                                  const unsigned long& min,
                                  const unsigned long& max);
};
} // namespace Siege

#endif // SIEGE_ENGINE_ARRAYUTILS_H
