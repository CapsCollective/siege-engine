//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "BitSet.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>

#include "../Branchless.h"

// Macro for translating a byte value into an index position.
#define GET_BIT_POS(pos) static_cast<size_t>(BitUtils::BIT_POSITION_##pos)

namespace Siege::Utils
{
// +----------------------------------------------------------------------------------------------+
// |------------------------------------- BitUtils -----------------------------------------------|
// +----------------------------------------------------------------------------------------------+

unsigned long BitUtils::UnsetBit(unsigned char* bitfield,
                                 const unsigned long& bit,
                                 const unsigned long& leftMostBit,
                                 const unsigned long& size)
{
    AssertIsInBounds(bit - (bit > 0), size * BYTE_SIZE_IN_BITS);

    auto indexedBit = bit - 1;
    auto byteIndex = Byte(indexedBit);

    BitUtils::SetBitToZero(bitfield[byteIndex], indexedBit);

    // Skip the loop below if the bit being processed is not the leftMostBit
    byteIndex = IF(bit < leftMostBit THEN - 1 ELSE byteIndex);

    size_t newLeftMostBit = leftMostBit;

    for (int32_t i = byteIndex; i >= 0; i--)
    {
        auto leftBit = CalculateLeftMostBit(bitfield[i]);
        auto bitsLeft = leftBit + ((leftBit > 0) * (BYTE_SIZE_IN_BITS * i));

        newLeftMostBit =
            IF(bitsLeft > 0 THEN bitsLeft ELSE newLeftMostBit * (i != 0 || bitsLeft != 0));

        i = IF(bitsLeft > 0 THEN - 1 ELSE i);
    }
    return newLeftMostBit;
}

unsigned long BitUtils::SetBit(unsigned char* bitfield,
                               const unsigned long& bit,
                               const unsigned long& leftMostBit)
{
    auto indexedBit = bit - 1;
    bitfield[Byte(indexedBit)] |= BitAtIndex(indexedBit);
    return IF(bit > leftMostBit THEN bit ELSE leftMostBit);
}

unsigned long BitUtils::CalculateLeftMostBit(const unsigned char& byte)
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
    return GetBitPosIndex(r & -r);
}

unsigned long BitUtils::GetBitPosIndex(const unsigned long& bit)
{
    switch (bit)
    {
        case 1:
            return GET_BIT_POS(1);
        case 2:
            return GET_BIT_POS(2);
        case 4:
            return GET_BIT_POS(4);
        case 8:
            return GET_BIT_POS(8);
        case 16:
            return GET_BIT_POS(16);
        case 32:
            return GET_BIT_POS(32);
        case 64:
            return GET_BIT_POS(64);
        case 128:
            return GET_BIT_POS(128);
        default:
            return 0;
    }
}

void BitUtils::SetBitsToOne(unsigned char* bitfield, const unsigned long& bits)
{
    size_t bitsToProcess = bits;
    size_t newByteCount = (bitsToProcess / BYTE_SIZE_IN_BITS);

    auto bitsLeft = bits - (newByteCount * BYTE_SIZE_IN_BITS);
    memset(bitfield, 255, newByteCount);
    bitfield[newByteCount] = MAX_BIT_VALUES[bitsLeft - 1];
}

void BitUtils::Clear(unsigned char* bitfield, const unsigned long& size)
{
    memset(bitfield, 0, BYTE_MASK_SIZE * size);
}

void BitUtils::AssertIsInBounds(const size_t& index, const size_t& size)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(IsInBounds(index, size) && "The provided index is out of bounds!");
}

void BitUtils::AssertIsSet(const uint8_t* bitField, const size_t& index)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(BitUtils::IsSet(bitField, index) &&
           "Cannot index into element that hasn't been assigned!");
}

// +----------------------------------------------------------------------------------------------+
// |----------------------------------------- BitSet ---------------------------------------------|
// +----------------------------------------------------------------------------------------------+

BitUtils::BitSet::BitSet(const size_t& bytes) : leftMostBit {0}, size {bytes}
{
    bitfield = static_cast<uint8_t*>(malloc(BYTE_MASK_SIZE * bytes));
    BitUtils::Clear(bitfield, size);
}

BitUtils::BitSet::BitSet(const BitSet& other) : leftMostBit {other.leftMostBit}, size {other.size}
{
    bitfield = static_cast<uint8_t*>(malloc(BYTE_MASK_SIZE * other.size));
    memcpy(bitfield, other.bitfield, BYTE_MASK_SIZE * other.size);
}

BitUtils::BitSet::BitSet(BitSet&& other) : leftMostBit {other.leftMostBit}, size {other.size}
{
    bitfield = std::move(other.bitfield);
    leftMostBit = std::move(other.leftMostBit);
    size = std::move(other.size);

    other.ResetValues();
}

BitUtils::BitSet::~BitSet()
{
    free(bitfield);
    bitfield = nullptr;
}

const uint8_t& BitUtils::BitSet::operator[](const size_t& index)
{
    AssertIsInBounds(index, size);
    return bitfield[index];
}

BitUtils::BitSet& BitUtils::BitSet::operator=(const BitSet& other)
{
    if (this == &other) return *this;

    bitfield = static_cast<uint8_t*>(realloc(bitfield, BYTE_MASK_SIZE * other.size));
    memcpy(bitfield, other.bitfield, BYTE_MASK_SIZE * other.size);

    leftMostBit = other.leftMostBit;
    size = other.size;

    return *this;
}

BitUtils::BitSet& BitUtils::BitSet::operator=(BitSet&& other)
{
    bitfield = std::move(other.bitfield);
    leftMostBit = std::move(other.leftMostBit);
    size = std::move(other.size);

    other.ResetValues();

    return *this;
}

void BitUtils::BitSet::SetBit(const size_t& bit)
{
    leftMostBit = BitUtils::SetBit(bitfield, bit, leftMostBit);
}

bool BitUtils::BitSet::IsSet(const size_t& bit)
{
    return BitUtils::IsSet(bitfield, bit);
}

void BitUtils::BitSet::UnsetBit(const size_t& bit)
{
    leftMostBit = BitUtils::UnsetBit(bitfield, bit, leftMostBit, size);
}

void BitUtils::BitSet::SetBitsToOne(const size_t& bits)
{
    BitUtils::SetBitsToOne(bitfield, bits);
    leftMostBit = bits;
}

void BitUtils::BitSet::Clear()
{
    BitUtils::Clear(bitfield, size);
    leftMostBit = size = 0;
}

void BitUtils::BitSet::UnsetPostBits(const size_t& bit)
{
    auto byteIndex = Byte(bit);

    bitfield[byteIndex] &= MAX_BIT_VALUES[(bit % BYTE_SIZE_IN_BITS) - 1];

    memset(bitfield + (byteIndex + 1), 0, size - (byteIndex + (byteIndex == 0)));

    leftMostBit = CalculateLeftMostBit(bitfield[byteIndex]) + (byteIndex * BYTE_SIZE_IN_BITS);
}

void BitUtils::BitSet::Resize(const size_t& newSize)
{
    bitfield = static_cast<uint8_t*>(realloc(bitfield, BYTE_MASK_SIZE * newSize));

    auto bitIndex = CalculateLeftMostBit(bitfield[newSize - 1]);

    // Use an AND operation to reset the byte mask to the position that we want.
    bitfield[newSize - 1] &= MAX_BIT_VALUES[bitIndex];

    leftMostBit =
        IF(newSize > size THEN leftMostBit ELSE(bitIndex + ((newSize - 1) * BYTE_SIZE_IN_BITS)));

    size = newSize;

    UnsetPostBits(leftMostBit);
}

void BitUtils::BitSet::ResetValues()
{
    bitfield = nullptr;
    leftMostBit = size = 0;
}
} // namespace Siege::Utils
