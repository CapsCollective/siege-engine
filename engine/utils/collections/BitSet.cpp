//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
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

namespace Siege::BitUtils
{

unsigned long UnsetBit(unsigned char* bitfield,
                       const unsigned long& bit,
                       const unsigned long& leftMostBit,
                       const unsigned long& size)
{
    AssertIsInBounds(bit - (bit > 0), size * BYTE_SIZE_IN_BITS);

    auto indexedBit = bit - 1;
    auto byteIndex = Byte(indexedBit);

    SetBitToZero(bitfield[byteIndex], indexedBit);

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

unsigned long SetBit(unsigned char* bitfield,
                     const unsigned long& bit,
                     const unsigned long& leftMostBit)
{
    auto indexedBit = bit - 1;
    bitfield[Byte(indexedBit)] |= BitAtIndex(indexedBit);
    return IF(bit > leftMostBit THEN bit ELSE leftMostBit);
}

unsigned long CalculateLeftMostBit(const unsigned char& byte)
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

unsigned long GetBitPosIndex(const unsigned long& bit)
{
    return ((bit == 1) * 1) + ((bit == 2) * 2) + ((bit == 4) * 3) + ((bit == 8) * 4) +
           ((bit == 16) * 5) + ((bit == 32) * 6) + ((bit == 64) * 7) + ((bit == 128) * 8);
}

void SetBitsToOne(unsigned char* bitfield, const unsigned long& bits)
{
    size_t bitsToProcess = bits;
    size_t newByteCount = (bitsToProcess / BYTE_SIZE_IN_BITS);

    auto bitsLeft = bits - (newByteCount * BYTE_SIZE_IN_BITS);
    memset(bitfield, 255, newByteCount);
    bitfield[newByteCount] = MAX_BIT_VALUES[bitsLeft - 1];
}

void Clear(unsigned char* bitfield, const unsigned long& size)
{
    if (bitfield == nullptr) return;
    memset(bitfield, 0, BYTE_MASK_SIZE * size);
}

void AssertIsInBounds(const unsigned long& index, const unsigned long& size)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(IsInBounds(index, size) && "The provided index is out of bounds!");
}

void AssertIsSet(const unsigned char* bitField, const unsigned long& index)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(IsSet(bitField, index) && "Cannot index into element that hasn't been assigned!");
}

// BitSet functrion implementations

BitSet::BitSet(const unsigned long& bytes) : leftMostBit {0}, size {bytes}
{
    bitfield = static_cast<uint8_t*>(malloc(BYTE_MASK_SIZE * bytes));
    ::Siege::BitUtils::Clear(bitfield, size);
}

BitSet::BitSet(const BitSet& other) : leftMostBit {other.leftMostBit}, size {other.size}
{
    bitfield = static_cast<uint8_t*>(malloc(BYTE_MASK_SIZE * other.size));
    memcpy(bitfield, other.bitfield, BYTE_MASK_SIZE * other.size);
}

BitSet::BitSet(BitSet&& other) : leftMostBit {other.leftMostBit}, size {other.size}
{
    bitfield = std::move(other.bitfield);
    leftMostBit = std::move(other.leftMostBit);
    size = std::move(other.size);

    other.ResetValues();
}

BitSet::~BitSet()
{
    free(bitfield);
    bitfield = nullptr;
}

const uint8_t& BitSet::operator[](const unsigned long& index)
{
    AssertIsInBounds(index, size);
    return bitfield[index];
}

BitSet& BitSet::operator=(const BitSet& other)
{
    if (this == &other) return *this;

    bitfield = static_cast<uint8_t*>(realloc(bitfield, BYTE_MASK_SIZE * other.size));
    memcpy(bitfield, other.bitfield, BYTE_MASK_SIZE * other.size);

    leftMostBit = other.leftMostBit;
    size = other.size;

    return *this;
}

BitSet& BitSet::operator=(BitSet&& other)
{
    bitfield = std::move(other.bitfield);
    leftMostBit = std::move(other.leftMostBit);
    size = std::move(other.size);

    other.ResetValues();

    return *this;
}

void BitSet::SetBit(const unsigned long& bit)
{
    leftMostBit = ::Siege::BitUtils::SetBit(bitfield, bit, leftMostBit);
}

bool BitSet::IsSet(const unsigned long& bit) const
{
    return ::Siege::BitUtils::IsSet(bitfield, bit);
}

void BitSet::UnsetBit(const unsigned long& bit)
{
    leftMostBit = ::Siege::BitUtils::UnsetBit(bitfield, bit, leftMostBit, size);
}

void BitSet::SetBitsToOne(const unsigned long& bits)
{
    ::Siege::BitUtils::SetBitsToOne(bitfield, bits);
    leftMostBit = bits;
}

void BitSet::Clear()
{
    ::Siege::BitUtils::Clear(bitfield, size);
    leftMostBit = 0;
}

void BitSet::UnsetPostBits(const unsigned long& bit)
{
    auto byteIndex = Byte(bit);

    bitfield[byteIndex] &= MAX_BIT_VALUES[(bit % BYTE_SIZE_IN_BITS) - 1];

    memset(bitfield + (byteIndex + 1), 0, size - (byteIndex + (byteIndex == 0)));

    leftMostBit = CalculateLeftMostBit(bitfield[byteIndex]) + (byteIndex * BYTE_SIZE_IN_BITS);
}

void BitSet::Resize(const unsigned long& newSize)
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

void BitSet::ResetValues()
{
    bitfield = nullptr;
    leftMostBit = size = 0;
}
} // namespace Siege::BitUtils
