//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Statics.h"

#include <freetype/freetype.h>
#include <utils/Logging.h>
#include FT_FREETYPE_H

namespace Siege
{
FontLib Statics::freetype {nullptr};

void Statics::Initialise()
{
    CC_ASSERT(!FT_Init_FreeType(&freetype), "Failed to initialise freetype!")
}

void Statics::Free()
{
    FT_Done_FreeType(freetype);
}
} // namespace Siege