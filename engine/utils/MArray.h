//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MARRAY_H
#define SIEGE_ENGINE_MARRAY_H

#include <algorithm>

namespace Siege::Utils
{
// The number of states we can fit per segment (1 byte = 8 states)
inline constexpr uint8_t BYTE_SIZE_IN_BITS = 8;
// The default object size for each segment (1 byte)
inline constexpr uint8_t BYTE_MASK_SIZE = sizeof(uint8_t);
// Each position represents a byte where all bits before the index are 1.
// i.e: index 2 = 0000 00111
inline constexpr uint8_t MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] = {1, 3, 7, 15, 31, 63, 127, 255};

class MArrayUtils
{
public:

    static inline constexpr void SetBitsToOne(uint8_t* bitField, const size_t& bits)
    {
        uint8_t bitsToProcess = bits;
        size_t newByteCount = (bitsToProcess / BYTE_SIZE_IN_BITS) + 1;

        for (size_t i = 0; i < newByteCount; i++)
        {
            bool isFullByte = (bitsToProcess - BYTE_SIZE_IN_BITS) >= 0;

            uint8_t maskBits = (bitsToProcess * !isFullByte) + (BYTE_SIZE_IN_BITS * isFullByte);

            bitField[i] = MAX_BIT_VALUES[std::clamp<size_t>(maskBits - 1, 0, BYTE_SIZE_IN_BITS)];

            bitsToProcess -= BYTE_SIZE_IN_BITS;
        }
    }

    static inline constexpr bool Active(const uint8_t* bitField, const size_t& index)
    {
        // Get the byte chunk that our index falls into
        auto byteMask = bitField[index / BYTE_SIZE_IN_BITS];
        /**
         * Get a bit representation of the position we want to search for.
         * For example, given a bit position in index 1, we create a bit mask with a value of
         * 1 left shifted by 1 position (0000 0010). Finally, since our bytes contain 8 bits,
         * we want to make sure that we just check the index we get is relative to our byte.
         * i.e: 2 % 8 = 2 or 9 % 8 = 1. No matter how large the index, we'll always get its
         * position in 8s.
         */
        return (byteMask & (1 << (index % BYTE_SIZE_IN_BITS))) != 0;
    }

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

    static void AssertIsActive(const uint8_t* bitField, const size_t& index);

    static void AssertIsInBoundsAndActive(const uint8_t* bitField,
                                          const size_t& index,
                                          const size_t& size);

    static inline void CopyData(void* dst, const void* src, const size_t& size)
    {
        memcpy(dst, src, size);
    }

    static inline constexpr void RemoveFromStateMask(uint8_t* bitField,
                                                     const size_t& index,
                                                     const size_t& size)
    {
        AssertIsInBoundsAndActive(bitField, index, size);
        bitField[index / BYTE_SIZE_IN_BITS] &= ~(1 << (index % BYTE_SIZE_IN_BITS));
    }

    static inline void ResetStateMask(uint8_t* bitField, const size_t& maskSize)
    {
        if (bitField) memset(bitField, 0, (BYTE_MASK_SIZE * maskSize));
    }

    static inline constexpr size_t AddToBitMask(uint8_t* bitField,
                                                const size_t& index,
                                                const size_t& size)
    {
        AssertIsInBounds(index, size);
        bool exists = MArrayUtils::Active(bitField, index);
        bitField[index / BYTE_SIZE_IN_BITS] |= (1 << (index % BYTE_SIZE_IN_BITS));
        return !exists;
    }

    static inline constexpr size_t CalculateBitFieldSize(const size_t& arraySize)
    {
        return (arraySize / BYTE_SIZE_IN_BITS) + 1;
    }
};
} // namespace Siege::Utils

#endif // SIEGE_ENGINE_MARRAY_H
