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
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <utility>
#include <cmath>

#define CLAMP_T(val, min, max, type) std::clamp<size_t>(sizeof(type) * val, min, sizeof(type) * max)

#define CLAMP(val, min, max) std::clamp<size_t>(val, min, max)

namespace Siege::Utils
{
class ArrayUtils
{
public:
    /**
     * Checks if an index is within the bounds of a given size.
     * @param index the index being checked.
     * @param size the size of the array.
     * @return true if the index is in bounds, false if it is not.
     */
    static inline constexpr bool IsInBounds(const size_t& index, const size_t& size)
    {
        return index < size;
    }

    /**
     * @brief Checks if the index is within bounds of the array. If it is not, an assertion failure
     * will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsInBounds(const size_t& index, const size_t& size);

    /**
     * Copied data from one array pointer to another.
     * @param dst the array being kcopied to.
     * @param src the array being chhyopied from.
     * @param size the amount of data to copy.
     */
    static inline void CopyData(void* dst, const void* src, const size_t& size)
    {
        memcpy(dst, src, size);
    }
};
} // namespace Siege::Utils

#endif // SIEGE_ENGINE_ARRAYUTILS_H
