//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include <utils/Hash.h>
#include <utils/Logging.h>
#include <utils/mat/Mat4.h>
#include <volk/volk.h>

#include <cstdint>

#include "utils/Array.h"
#include "utils/StackArray.h"

namespace Siege
{
struct CameraData
{
    Siege::Mat4 projectionMatrix;
    Siege::Mat4 viewMatrix;
};
} // namespace Siege