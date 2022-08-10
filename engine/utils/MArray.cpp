//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "MArray.h"

#include <cassert>

namespace Siege::Utils
{
void MArrayUtils::AssertIsInBounds(const size_t& index, const size_t& size)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(IsInBounds(index, size) && "The provided index is out of bounds!");
}

void MArrayUtils::AssertIsActive(const uint8_t* bitField, const size_t& index)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(Active(bitField, index) && "Cannot index into element that hasn't been assigned!");
}

void MArrayUtils::AssertIsInBoundsAndActive(const uint8_t* bitField,
                                            const size_t& index,
                                            const size_t& size)
{
    AssertIsInBounds(index, size);
    AssertIsActive(bitField, index);
}
} // namespace Siege::Utils