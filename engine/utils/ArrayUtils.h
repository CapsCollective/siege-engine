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

#define MAX(left, right, type) left - ((left -right) & ((left-right) >> 31))

#define IF_ASSIGN(expr, if, else) ((expr) * if) + (!(expr) * else)

#define IF(...) IF_ASSIGN(__VA_ARGS__)

#define THEN ,
#define ELSE THEN

#define GET_BIT_POS(pos) static_cast<size_t>(SBitMaskField::BIT_POSITION_##pos)

namespace Siege::Utils
{
// The number of states we can fit per segment (1 byte = 8 states)
inline constexpr uint8_t BYTE_SIZE_IN_BITS = 8;
// The default object size for each segment (1 byte)
inline constexpr uint8_t BYTE_MASK_SIZE = sizeof(uint8_t);
// Each position represents a byte where all bits before the index are 1.
// i.e: index 2 = 0000 0111
inline constexpr uint8_t MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] = {1, 3, 7, 15, 31, 63, 127, 255};

class ArrayUtils
{
public:
    template<size_t S>
    class SBitMaskField
    {
    enum BIT_POSITIONS
    {   // 0000 1111
        BIT_POSITION_1 = 1,
        BIT_POSITION_2 = 2,
        BIT_POSITION_4 = 3,
        BIT_POSITION_8 = 4,
        BIT_POSITION_16 = 5,
        BIT_POSITION_32 = 6,
        BIT_POSITION_64 = 7,
        BIT_POSITION_128 = 8,
    };
    public:

        inline constexpr uint8_t& operator[](const size_t& index)
        {
            AssertIsInBounds(index, S);
            return bitfield[index];
        }

        void UnsetBit(const size_t& bit)
        {
            AssertIsInBounds(bit - (bit > 0), S * BYTE_SIZE_IN_BITS);

            auto indexedBit = bit-1;
            auto byteIndex = Byte(indexedBit);

            bitfield[byteIndex] &= ~BitAtIndex(indexedBit);

            byteIndex = IF(bit < leftMostBit THEN -1 ELSE byteIndex);

            for(int32_t i = byteIndex; i >= 0; i--)
            {
                auto leftBit = CalculateLeftMostBit(bitfield[i]);
                auto bitsLeft = leftBit + ((leftBit > 0) * (BYTE_SIZE_IN_BITS * i));

                leftMostBit = IF(bitsLeft > 0 THEN bitsLeft
                              ELSE leftMostBit * (i != 0 || bitsLeft != 0));

                i = IF(bitsLeft > 0 THEN -1 ELSE i);
            }
        }

        void SetBit(const size_t& bit)
        {
            auto indexedBit = bit-1;
            bitfield[Byte(indexedBit)] |= BitAtIndex(indexedBit);
            leftMostBit = IF(bit > leftMostBit THEN bit ELSE leftMostBit);
        }

        bool IsSet(const size_t& bit)
        {
            auto indexedBit = bit-1;
            // Get the byte chunk that our index falls into
            auto  byteMask = bitfield[Byte(indexedBit)];
            /**
             * Get a bit representation of the position we want to search for.
             * For example, given a bit position in index 1, we create a bit mask with a value ofJ
             * 1 left shifted by 1 position (0000 0010). Finally, since our bytes contain 8 bits,
             * we want to make sure that we just check the index we get is relative to our byte.
             * i.e: 2 % 8 = 2 or 9 % 8 = 1. No matter how large the index, we'll always get its
             * position in 8s.
            */
            return (byteMask & BitAtIndex(indexedBit)) != 0;
        }

        /**
         * Sets all bits up to a specified count to 1. For example, if the function is invoked with
         * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
         * @param bits the bits to set to 1.
         */
        inline constexpr void SetBitsToOne(const size_t& bits)
        {
            size_t bitsToProcess = bits;
            size_t newByteCount = (bitsToProcess / BYTE_SIZE_IN_BITS);

            auto bitsLeft = bits - (newByteCount * BYTE_SIZE_IN_BITS);
            memset(bitfield, 255, newByteCount);
            bitfield[newByteCount] = MAX_BIT_VALUES[bitsLeft-1];

            leftMostBit = bits;
        }

        const size_t& LeftMostBit() { return leftMostBit; }
        static inline constexpr size_t Byte(const size_t& index) {return index / BYTE_SIZE_IN_BITS;}
        static inline constexpr size_t CalculateLeftMostBit(const uint8_t& byte)
        {
            size_t r = byte;
            // Active all bits after leftmost bit
            r = r | (r >> 1);
            r = r | (r >> 2);
            r = r | (r >> 4);
            r = r | (r >> 8);
            r = r | (r >> 16);
            // Get the value of leftmost bit
            r = r ^ (r >> 1);
            return GetBitPosIndex(r &-r);
        }

    private:
        static inline constexpr size_t GetBitPosIndex(const size_t& bit)
        {
            switch(bit)
            {
                case 0: return 0;
                case 1: return GET_BIT_POS(1);
                case 2: return GET_BIT_POS(2);
                case 4: return GET_BIT_POS(4);
                case 8: return GET_BIT_POS(8);
                case 16: return GET_BIT_POS(16);
                case 32: return GET_BIT_POS(32);
                case 64: return GET_BIT_POS(64);
                case 128: return GET_BIT_POS(128);
            }
        }
        const uint8_t BitAtIndex(const size_t& index) { return 1 << (index % BYTE_SIZE_IN_BITS); }

        uint8_t bitfield[S] {0};
        size_t leftMostBit {0};
    };

    /**
     * Sets all bits up to a specified count to 1. For example, if the function is invoked with
     * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
     * @param bitField the array of bytes to be processed.
     * @param bits the bits to set to 1.
     */
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

    /**
     * Checks for a 1 value in a position in a bitfield. For example, given a bitfield: 00010101,
     * and an index of 2, the function would return true (the bit at position 2 is a 1).
     * @param bitField the array of bytes to be processed.
     * @param index the position of the bit the check.
     * @return
     */
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

    static inline size_t GetTotalBitMask(const uint8_t* bitField, const size_t& bitFieldSize)
    {
        size_t size = 0;
        for (size_t i = 0; i < bitFieldSize; i++) size += bitField[0];
        return size;
    }

    static inline size_t GetBitsToProcess(const uint8_t* bitField, const size_t& bitFieldSize)
    {
        // 1010
        // Empty? return 0
        // Only has one element? Return 1.
        // Otherwise, return whatever the value is
        auto totalBitMask = GetTotalBitMask(bitField, bitFieldSize);
        return log(totalBitMask)/log(2);
    }

    /**
     * @brief Checks if the index is within bounds of the array. If it is not, an assertion failure
     * will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsInBounds(const size_t& index, const size_t& size);

    /**
     * @brief Checks if the index is Active (has been assigned to). If it is not, an assertion
     * failure will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsActive(const uint8_t* bitField, const size_t& index);

    /**
     * Checks if an index is both active AND is ini bounds. If either condition is false, an
     * assertion failure will be thrown.
     * @param bitField the array of bytes to process.
     * @param index the index being checked.
     * @param size the total size of the array.
     */
    static void AssertIsInBoundsAndActive(const uint8_t* bitField,
                                          const size_t& index,
                                          const size_t& size);

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

    /**
     * Sets the value in the provided index in the bitmask to 0.
     * @param bitField the array of bytes to be processed.
     * @param index the index to set to 0.
     * @param size the size of the array.
     */
    static inline void RemoveFromStateMask(uint8_t* bitField,
                                           const size_t& index,
                                           const size_t& size)
    {
        AssertIsInBoundsAndActive(bitField, index, size);
        bitField[index / BYTE_SIZE_IN_BITS] &= ~(1 << (index % BYTE_SIZE_IN_BITS));
    }

    /**
     * Resets an array of bytes to zero.
     * @param bitField the bitfield to reset.
     * @param maskSize the number of bytes in the bitfield.
     */
    static inline void ResetStateMask(uint8_t* bitField, const size_t& maskSize)
    {
        if (bitField) memset(bitField, 0, (BYTE_MASK_SIZE * maskSize));
    }

    /**
     * Sets the value in the specified index to 1.
     * @param bitField the bit field to process.
     * @param index the index to set.
     * @param size the size of the array.
     * @return returns true if the element was not previously active, false if it was.
     */
    static inline bool AddToBitMask(uint8_t* bitField, const size_t& index, const size_t& size)
    {
        AssertIsInBounds(index, size);
        bool exists = ArrayUtils::Active(bitField, index);
        bitField[index / BYTE_SIZE_IN_BITS] |= (1 << (index % BYTE_SIZE_IN_BITS));
        return !exists;
    }

    /**
     * Calculates the number of bytes in a bitfield.
     * @param arraySize the size of the array.
     * @return the number of bytes comprising the array.
     */
    static inline constexpr size_t CalculateBitFieldSize(const size_t& arraySize)
    {
        return (arraySize / BYTE_SIZE_IN_BITS) + 1;
    }
};
} // namespace Siege::Utils

#endif // SIEGE_ENGINE_ARRAYUTILS_H
