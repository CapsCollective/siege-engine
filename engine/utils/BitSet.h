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

#define GET_BIT_POS(pos) static_cast<size_t>(BitUtils::BIT_POSITION_##pos)

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

    static inline constexpr uint8_t BitAtIndex(const size_t& index) {return 1 << (index % BYTE_SIZE_IN_BITS);}
    static inline constexpr size_t Byte(const size_t& index) {return index / BYTE_SIZE_IN_BITS;}
    static inline constexpr size_t CalculateBitFieldSize(const size_t& arraySize)
    {
        return (arraySize / BYTE_SIZE_IN_BITS) + 1;
    }

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

    static inline constexpr size_t GetBitPosIndex(const size_t& bit)
    {
        switch(bit)
        {
            case 1: return GET_BIT_POS(1);
            case 2: return GET_BIT_POS(2);
            case 4: return GET_BIT_POS(4);
            case 8: return GET_BIT_POS(8);
            case 16: return GET_BIT_POS(16);
            case 32: return GET_BIT_POS(32);
            case 64: return GET_BIT_POS(64);
            case 128: return GET_BIT_POS(128);
            default: return 0;
        }
    }

    static inline constexpr size_t SetBit(uint8_t* bitfield, const size_t& bit, const size_t& leftMostBit)
    {
        auto indexedBit = bit-1;
        bitfield[Byte(indexedBit)] |= BitAtIndex(indexedBit);
        return IF(bit > leftMostBit THEN bit ELSE leftMostBit);
    }

    static inline constexpr bool IsSet(const uint8_t* bitfield, const size_t& bit)
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

    static inline constexpr size_t UnsetBit(uint8_t* bitfield,
                                            const size_t& bit,
                                            const size_t& leftMostBit,
                                            const size_t& size)
    {
        AssertIsInBounds(bit - (bit > 0), size * BYTE_SIZE_IN_BITS);

        auto indexedBit = bit-1;
        auto byteIndex = Byte(indexedBit);

        bitfield[byteIndex] &= ~BitAtIndex(indexedBit);

        byteIndex = IF(bit < leftMostBit THEN -1 ELSE byteIndex);

        size_t newLeftMostBit = leftMostBit;

        for(int32_t i = byteIndex; i >= 0; i--)
        {
            auto leftBit = CalculateLeftMostBit(bitfield[i]);
            auto bitsLeft = leftBit + ((leftBit > 0) * (BYTE_SIZE_IN_BITS * i));

            newLeftMostBit = IF(bitsLeft > 0 THEN bitsLeft
                                                ELSE newLeftMostBit * (i != 0 || bitsLeft != 0));

            i = IF(bitsLeft > 0 THEN -1 ELSE i);
        }

        return newLeftMostBit;
    }

    /**
     * Sets all bits up to a specified count to 1. For example, if the function is invoked with
     * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
     * @param bitfield an array of 8-bit unsigned integers.
     * @param bits the bits to set to 1.
     */
    static inline constexpr void SetBitsToOne(uint8_t* bitfield, const size_t& bits)
    {
        size_t bitsToProcess = bits;
        size_t newByteCount = (bitsToProcess / BYTE_SIZE_IN_BITS);

        auto bitsLeft = bits - (newByteCount * BYTE_SIZE_IN_BITS);
        memset(bitfield, 255, newByteCount);
        bitfield[newByteCount] = MAX_BIT_VALUES[bitsLeft-1];
    }

    static inline constexpr void Clear(uint8_t* bitfield, const size_t& size)
    {
        memset(bitfield, 0, CalculateBitFieldSize(size));
    }

    class BitSet
    {
    public:
        BitSet() : leftMostBit{0}, size{0} {}

        BitSet(const size_t& bytes) : leftMostBit{0}, size{bytes}
        {
            bitfield = static_cast<uint8_t*>(malloc(BYTE_MASK_SIZE * bytes));
        }

        BitSet(const BitSet& other) : leftMostBit{other.leftMostBit}, size{other.size}
        {
            bitfield = static_cast<uint8_t*>(malloc(BYTE_MASK_SIZE * other.size));
            memcpy(bitfield, other.bitfield, BYTE_MASK_SIZE * other.size);
        }

        BitSet(BitSet&& other) : leftMostBit{other.leftMostBit}, size{other.size}
        {
            bitfield = std::move(other.bitfield);
            leftMostBit = std::move(other.leftMostBit);
            size = std::move(other.size);

            other.ResetValues();
        }

        ~BitSet()
        {
            free(bitfield);
            bitfield = nullptr;
        }

        inline constexpr const uint8_t& operator[](const size_t& index)
        {
            AssertIsInBounds(index, size);
            return bitfield[index];
        }

        inline BitSet& operator=(const BitSet& other)
        {
            if (this == &other) return *this;

            bitfield = static_cast<uint8_t*>(realloc(bitfield, BYTE_MASK_SIZE * other.size));
            memcpy(bitfield, other.bitfield, BYTE_MASK_SIZE * other.size);

            leftMostBit = other.leftMostBit;
            size = other.size;

            return *this;
        }

        inline BitSet& operator=(BitSet&& other)
        {
            bitfield = std::move(other.bitfield);
            leftMostBit = std::move(other.leftMostBit);
            size = std::move(other.size);

            other.ResetValues();

            return *this;
        }

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

    private:

        inline void ResetValues()
        {
            bitfield = nullptr;
            leftMostBit = 0;
            size = 0;
        }

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
private:
    // The number of states we can fit per segment (1 byte = 8 states)
    static inline constexpr uint8_t BYTE_SIZE_IN_BITS = 8;
    // The default object size for each segment (1 byte)
    static inline constexpr uint8_t BYTE_MASK_SIZE = sizeof(uint8_t);
    // Each position represents a byte where all bits before the index are 1.
    // i.e: index 2 = 0000 0111
    static inline constexpr uint8_t MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] = {1, 3, 7, 15, 31, 63, 127, 255};

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

};
} // namespace Siege

#endif // SIEGE_ENGINE_BITSET_H
