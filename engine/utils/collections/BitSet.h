//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BITSET_H
#define SIEGE_ENGINE_BITSET_H

namespace Siege::Utils
{
/**
 * A static class containing bit manipulation utilities. This class contains universal functions for
 * setting, unsetting, and managing complex bitmasks.
 */
class BitUtils
{
public:

    /**
     * A set of enums representing the exact positions of specific bits in a byte based on their
     * index position + 1.
     */
    enum BIT_POSITIONS
    {
        BIT_POSITION_1 [[maybe_unused]] = 1,
        BIT_POSITION_2 [[maybe_unused]] = 2,
        BIT_POSITION_4 [[maybe_unused]] = 3,
        BIT_POSITION_8 [[maybe_unused]] = 4,
        BIT_POSITION_16 [[maybe_unused]] = 5,
        BIT_POSITION_32 [[maybe_unused]] = 6,
        BIT_POSITION_64 [[maybe_unused]] = 7,
        BIT_POSITION_128 [[maybe_unused]] = 8,
    };

    // The number of states we can fit per segment (1 byte = 8 states)
    static inline constexpr unsigned char BYTE_SIZE_IN_BITS = 8;
    // The default object size for each segment (1 byte)
    static inline constexpr unsigned char BYTE_MASK_SIZE = sizeof(unsigned char);
    // Each position represents a byte where all bits before the index are 1.
    // i.e: index 2 = 0000 0111
    static inline constexpr unsigned char MAX_BIT_VALUES[BYTE_SIZE_IN_BITS] =
        {1, 3, 7, 15, 31, 63, 127, 255};

    /**
     * Calculates the number of bytes which would be allocated for a given array size.
     * @param arraySize the number of elements needed to be tracked by an array.
     * @return the number of bytes that would be allocated based on the array size.
     */
    static inline constexpr const unsigned long CalculateBitSetSize(const unsigned long& arraySize)
    {
        return (arraySize / BYTE_SIZE_IN_BITS) + 1;
    }

    /**
     * The BitSet class is a collection of bytes which store state information. The BitSet stores an
     * array of bytes and stores the location of the left-most bit which was set in the array. This
     * allows the BitSet to calculate the number of bits which have been set and reduces the number
     * of iterations required to recalculate the left most bit.
     *
     * One important thing to note is that bit positions are configured to range from 1 - 8 for each
     * respective byte. The reason for this is that the bitset needs to be able to determine when no
     * bits have been set at all. In this case, the left most bit would be set to 0. Without this,
     * it would be impossible to determine if a BitSet is empty or not.
     */
    class BitSet
    {
    public:

        /**
         * Zero-constructor for the BitSet.
         */
        BitSet() : leftMostBit {0}, size {0} {}

        /**
         * Creates an empty bitset with a set number of allocated bytes.
         * @param bytes the number of bytes to be allocated.
         */
        BitSet(const unsigned long& bytes);

        /**
         * A copy constructor for the BitSet class.
         * @param other the BitSet to copy.
         */
        BitSet(const BitSet& other);

        /**
         * A move constructor for the BitSet class.
         * @param other the BitSet to copy.
         */
        BitSet(BitSet&& other);

        /**
         * A destructor for the BitSet class. Deallocates all allocated memory.
         */
        ~BitSet();

        /**
         * A subscript operator for the BitSet. Returns the byte stored at the provided index.
         * @param index the index of the byte to return.
         * @return the byte stored at the provided index.
         */
        const unsigned char& operator[](const unsigned long& index);

        /**
         * A copy assignment operator.
         * @param other the BitSet to copy.
         * @return a reference to the created BitSet.
         */
        BitSet& operator=(const BitSet& other);

        /**
         * A move assignment operator.
         * @param other the BitSet to move.
         * @return a reference to the created BitSet.
         */
        BitSet& operator=(BitSet&& other);

        /**
         * Sets a specific bit to 1.
         * @param bit the bit to set, ranging from 1 - bytes * 8.
         */
        void SetBit(const unsigned long& bit);

        /**
         * Checks if a specific bit is set to 1.
         * @param bit the bit to set, ranging from 1 - bytes * 8.
         * @return true of the bit is set, false if it is set to 0.
         */
        bool IsSet(const unsigned long& bit);

        /**
         * Returns the position of the largest set bit. For example, the left most bit in the byte:
         * 0010 0010 would be 6.
         * @return the position of the largest set bit (starting from 1). A 0 indicates that no
         * bits have been set.
         */
        const unsigned long& LeftMostBit() const
        {
            return leftMostBit;
        }

        /**
         * Sets a specified bit to 0.
         * @param bit the bit to unset, ranging from 1 - bytes * 8.
         */
        void UnsetBit(const unsigned long& bit);

        /**
         * Sets all bits up to a specified count to 1. For example, if the function is invoked with
         * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
         * @param bits the bits to set to 1.
         */
        void SetBitsToOne(const unsigned long& bits);

        /**
         * Resets the BitSet to 0.
         */
        void Clear();

        /**
         * Resets all bits after a specified bit position.
         * @param bit the bit to unset all bits after.
         */
        void UnsetPostBits(const unsigned long& bit);

        /**
         * Returns the array of bytes being stored by the BitSet as a constant.
         * @return the array of bytes being stored by the BitSet.
         */
        inline const unsigned char* BitField() const
        {
            return bitfield;
        }

        /**
         * @brief Resizes the state mask in accordance to the new array size.
         * @param newSize the new array size.
         */
        void Resize(const unsigned long& newSize);

    private:

        /**
         * Resets all values in the BitSet.
         * WARNING: This an unsafe function. This should only be called after the BitSet memory has
         * been deallocated.
         */
        void ResetValues();

        unsigned char* bitfield {nullptr};
        unsigned long leftMostBit {0};
        unsigned long size {0};
    };

    /**
     * The SBitSet class is a statically allocated collection of bytes which store state
     * information. The SBitSet stores an array of bytes and stores the location of the left-most
     * bit which was set in the array. This allows the BitSet to calculate the number of bits which
     * have been set and reduces the number of iterations required to recalculate the left most bit.
     *
     * One important thing to note is that bit positions are configured to range from 1 - 8 for each
     * respective byte. The reason for this is that the bitset needs to be able to determine when no
     * bits have been set at all. In this case, the left most bit would be set to 0. Without this,
     * it would be impossible to determine if a SBitSet is empty or not.
     */
    template<unsigned long S>
    class SBitSet
    {
    public:

        /**
         * A subscript operator for the SBitSet. Returns the byte stored at the provided index.
         * @param index the index of the byte to return.
         * @return the byte stored at the provided index.
         */
        unsigned char& operator[](const unsigned long& index)
        {
            AssertIsInBounds(index, S);
            return bitfield[index];
        }

        /**
         * Sets a specified bit to 0.
         * @param bit the bit to unset, ranging from 1 - bytes * 8.
         */
        void UnsetBit(const unsigned long& bit)
        {
            leftMostBit = BitUtils::UnsetBit(bitfield, bit, leftMostBit, S);
        }

        /**
         * Sets a specific bit to 1.
         * @param bit the bit to set, ranging from 1 - bytes * 8.
         */
        void SetBit(const unsigned long& bit)
        {
            leftMostBit = BitUtils::SetBit(bitfield, bit, leftMostBit);
        }

        /**
         * Checks if a specific bit is set to 1.
         * @param bit the bit to set, ranging from 1 - bytes * 8.
         * @return true of the bit is set, false if it is set to 0.
         */
        bool IsSet(const unsigned long& bit) const
        {
            return BitUtils::IsSet(bitfield, bit);
        }

        /**
         * Sets all bits up to a specified count to 1. For example, if the function is invoked with
         * the value of `bits` set to 5, the resulting bitmask would be: 0001 1111.
         * @param bits the bits to set to 1.
         */
        inline constexpr void SetBitsToOne(const unsigned long& bits)
        {
            BitUtils::SetBitsToOne(bitfield, bits);
            leftMostBit = bits;
        }

        /**
         * Returns the position of the largest set bit. For example, the left most bit in the byte:
         * 0010 0010 would be 6.
         * @return the position of the largest set bit (starting from 1). A 0 indicates that no
         * bits have been set.
         */
        inline const unsigned long& LeftMostBit()
        {
            return leftMostBit;
        }

        /**
         * Resets the BitSet to 0.
         */
        inline constexpr void Clear()
        {
            BitUtils::Clear(bitfield, S);
            leftMostBit = 0;
        }

        /**
         * Returns the array of bytes being stored by the BitSet as a constant.
         * @return the array of bytes being stored by the BitSet.
         */
        inline constexpr const unsigned char* BitField() const
        {
            return bitfield;
        }

    private:

        unsigned char bitfield[S] {0};
        unsigned long leftMostBit {0};
    };
    /**
     * @brief Checks if the index is Active (has been assigned to). If it is not, an assertion
     * failure will be triggered.
     * @param index the index to evaluate.
     * @param size the size of the array
     */
    static void AssertIsSet(const unsigned char* bitField, const unsigned long& index);

private:

    /**
     *
     * @param index
     * @return
     */
    static inline constexpr unsigned char BitAtIndex(const unsigned long& index)
    {
        return 1 << (index % BYTE_SIZE_IN_BITS);
    }

    /**
     *
     * @param index
     * @return
     */
    static inline constexpr unsigned long Byte(const unsigned long& index)
    {
        return index / BYTE_SIZE_IN_BITS;
    }

    /**
     *
     * @param byte
     * @return
     */
    static unsigned long CalculateLeftMostBit(const unsigned char& byte);

    /**
     *
     * @param bit
     * @return
     */
    static unsigned long GetBitPosIndex(const unsigned long& bit);

    /**
     *
     * @param bitfield
     * @param bit
     * @param leftMostBit
     * @return
     */
    static unsigned long SetBit(unsigned char* bitfield,
                                const unsigned long& bit,
                                const unsigned long& leftMostBit);

    /**
     * Sets a specified bit position to zero.
     * @param byte the byte to process.
     * @param bit the bit to set to 0
     */
    static inline constexpr void SetBitToZero(unsigned char& byte, const unsigned long& bit)
    {
        byte &= ~BitAtIndex(bit);
    }

    /**
     *
     * @param bitfield
     * @param bit
     * @return
     */
    static constexpr inline bool IsSet(const unsigned char* bitfield, const unsigned long& bit)
    {
        auto indexedBit = bit - (bit > 0);
        // Get the byte chunk that our index falls into
        auto byteMask = bitfield[Byte(indexedBit)];
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
     *
     * @param bitfield
     * @param bit
     * @param leftMostBit
     * @param size
     * @return
     */
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

    /**
     *
     * @param bitfield
     * @param size
     */
    static void Clear(unsigned char* bitfield, const unsigned long& size);

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
};
} // namespace Siege::Utils

#endif // SIEGE_ENGINE_BITSET_H
