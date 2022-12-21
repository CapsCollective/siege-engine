//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_CORE_H
#define SIEGE_ENGINE_CORE_H

#include <utils/Hash.h>
#include <utils/math/mat/Mat4.h>
#include <volk/volk.h>

#include <cstdint>

#include "utils/collections/HeapArray.h"
#include "utils/collections/StackArray.h"

namespace Siege
{
struct CameraData
{
    Mat4 projectionMatrix;
    Mat4 viewMatrix;
};
} // namespace Siege

#endif
