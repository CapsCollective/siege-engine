//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ArrayUtils.h"

#include <cassert>

#include "../Branchless.h"

namespace Siege
{
void ArrayUtils::AssertIsInBounds(const unsigned long& index, const unsigned long& size)
{
    // TODO: Change this to CC_ASSERT once the logging integration is complete
    assert(IsInBounds(index, size) && "The provided index is out of bounds!");
}

unsigned long ArrayUtils::SetCount(const unsigned long& size,
                                   const unsigned long& min,
                                   const unsigned long& max)
{
    return IF(size <= max THEN IF(size > 0 THEN size ELSE 0) ELSE max);
}

} // namespace Siege