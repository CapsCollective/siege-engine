//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BITSET_H
#define SIEGE_ENGINE_BITSET_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <utility>

#define IF_ASSIGN(expr, if, else) ((expr) * if) + (!(expr) * else)

#define IF(...) IF_ASSIGN(__VA_ARGS__)

#define THEN ,
#define ELSE THEN

namespace Siege::Utils
{
class BitUtils
{
public:
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

    // The number of states we can fit per segment (1 byte = 8 states)
    static inline constexpr unsigned char BYTE_SIZE_IN_BITS = 8;
    // The default object size for each segment (1 byte)
    static inline constexpr unsigned char BYTE_MASK_SIZE = sizeof(unsigned char);
    // Each position represents a byte where all bits before the index are 1.
    // i.e: index 2 = 0000 0111
    static inline constexpr unsigned char MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] =
        {1, 3, 7, 15, 31, 63, 127, 255};

    static inline constexpr unsigned char BitAtIndex(const unsigned long& index)
    {
        return 1 << (index % BYTE_SIZE_IN_BITS);
    }
    static inline constexpr unsigned long Byte(const unsigned long& index)
    {
        return index / BYTE_SIZE_IN_BITS;
    }
    static inline constexpr unsigned long CalculateBitFieldSize(const unsigned long& arraySize)
    {
        return (arraySize / BYTE_SIZE_IN_BITS) + 1;
    }

    static unsigned long CalculateLeftMostBit(const unsigned char& byte);

    static unsigned long GetBitPosIndex(const unsigned long& bit);

    static unsigned long SetBit(unsigned char* bitfield,
                                const unsigned long& bit,
                                const unsigned long& leftMostBit);

    static constexpr inline bool IsSet(const unsigned char* bitfield, const unsigned long& bit)
    {
        auto indexedBit = bit - (bit > 0);
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

    static unsigned long UnsetBit(unsigned char* bitfield,
                                  const unsigned long& bit,
                                  const unsigned long& leftMostBit,
                                  const unsigned long& size);

    /**
     * Sets all bits up to a specified count to 1. For example, if the function is invoked with
     * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
     * @param bitfield an array of 8-bit unsigned integers.
     * @param bits the bits to set to 1.
     */
    static void SetBitsToOne(unsigned char* bitfield, const unsigned long& bits);

    static void Clear(unsigned char* bitfield, const unsigned long& size);

    class BitSet
    {
    public:
        BitSet() : leftMostBit{0}, size{0} {}

        BitSet(const unsigned long& bytes);

        BitSet(const BitSet& other);

        BitSet(BitSet&& other);

        ~BitSet();

        const uint8_t& operator[](const size_t& index);

        BitSet& operator=(const BitSet& other);

        BitSet& operator=(BitSet&& other);

        // TODO: Finish cleaning this up.

        inline constexpr void SetBit(const size_t& bit)
        {
            leftMostBit = BitUtils::SetBit(bitfield, bit, leftMostBit);
        }

        inline constexpr bool IsSet(const size_t& bit)
        {
            return BitUtils::IsSet(bitfield, bit);
        }

        inline constexpr const size_t& LeftMostBit() const { return leftMostBit; }

        inline constexpr void UnsetBit(const size_t& bit)
        {
            leftMostBit = BitUtils::UnsetBit(bitfield, bit, leftMostBit, size);
        }

        inline constexpr void SetBitsToOne(const size_t& bits)
        {
            BitUtils::SetBitsToOne(bitfield, bits);
            leftMostBit = bits;
        }

        inline constexpr void Clear()
        {
            BitUtils::Clear(bitfield, size);
            leftMostBit = 0;
            size = 0;
        }

        inline void UnsetPostBits(const size_t& bit)
        {
            auto byteIndex = Byte(bit);

            bitfield[byteIndex] &= MAX_BIT_VALUES[(bit % BYTE_SIZE_IN_BITS) - 1];

            memset(bitfield + (byteIndex + 1), 0, size - (byteIndex + (byteIndex == 0)));

            leftMostBit =
                CalculateLeftMostBit(bitfield[byteIndex]) + (byteIndex * BYTE_SIZE_IN_BITS);
        }

        /**
         * Calculates the number of bytes in a bitfield.
         * @param arraySize the size of the array.
         * @return the number of bytes comprising the array.
         */
        static inline size_t CalculateBitFieldSize(const size_t& arraySize)
        {
            return (arraySize / BYTE_SIZE_IN_BITS) + 1;
        }

        inline const uint8_t* BitField() const
        {
            return bitfield;
        }
        inline uint8_t* BitField()
        {
            return bitfield;
        }

        /**
         * @brief Resizes the state mask in accordance to the new array size.
         * @param newSize the new array size.
         * @param newByteCount the new number of byte masks.
         */
        inline void Resize(const size_t& newSize)
        {
            // 0000 0000 0000 0001 | 0010 0000 | 0000 1010
            bitfield = static_cast<uint8_t*>(realloc(bitfield, BYTE_MASK_SIZE * newSize));

            auto bitIndex = CalculateLeftMostBit(bitfield[newSize - 1]);

            // Use an AND operation to reset the byte mask to the position that we want.
            bitfield[newSize - 1] &= MAX_BIT_VALUES[bitIndex];

            leftMostBit = IF(newSize > size THEN leftMostBit ELSE(
                                           bitIndex + ((newSize - 1) * BYTE_SIZE_IN_BITS)));

            size = newSize;

            UnsetPostBits(leftMostBit);
        }

        inline void ResetValues()
        {
            bitfield = nullptr;
            leftMostBit = 0;
            size = 0;
        }

    private:

        uint8_t* bitfield {nullptr};
        size_t leftMostBit {0};
        size_t size{0};
    };

    template<size_t S>
    class SBitSet
    {
    public:
        inline constexpr uint8_t& operator[](const size_t& index)
        {
            AssertIsInBounds(index, S);
            return bitfield[index];
        }

        void UnsetBit(const size_t& bit)
        {
            leftMostBit = BitUtils::UnsetBit(bitfield, bit, leftMostBit, S);
        }

        void SetBit(const size_t& bit) {leftMostBit = BitUtils::SetBit(bitfield, bit, leftMostBit);}

        bool IsSet(const size_t& bit) const { return BitUtils::IsSet(bitfield, bit); }

        /**
         * Sets all bits up to a specified count to 1. For example, if the function is invoked with
         * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
         * @param bits the bits to set to 1.
         */
        inline constexpr void SetBitsToOne(const size_t& bits)
        {
            BitUtils::SetBitsToOne(bitfield, bits);
            leftMostBit = bits;
        }

        inline const size_t& LeftMostBit() { return leftMostBit; }

        inline constexpr void Clear()
        {
            BitUtils::Clear(bitfield, S);
            leftMostBit = 0;
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

        inline const uint8_t* BitField() const { return bitfield; }
        inline uint8_t* BitField() { return bitfield; }

    private:

        uint8_t bitfield[S] {0};
        size_t leftMostBit {0};
    };

    /**
     * @brief Checks if the index is Active (has been assigned to). If it is not, an assertion
     * failure will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsSet(const uint8_t* bitField, const size_t& index);

private:

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

    /**
     * @brief Checks if the index is within bounds of the array. If it is not, an assertion failure
     * will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsInBounds(const size_t& index, const size_t& size);
};
} // namespace Siege

#endif // SIEGE_ENGINE_BITSET_H
