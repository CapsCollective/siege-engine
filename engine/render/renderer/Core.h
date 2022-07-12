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
#include <utils/Logging.h>
#include <volk/volk.h>

#include <cstdint>

#include "utils/Array.h"
#include "utils/StackArray.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <utils/mat/Mat4x4.h>

#include <glm/glm.hpp>

namespace Siege
{
struct CameraData
{
    Siege::Mat4x4 projectionMatrix;
    Siege::Mat4x4 viewMatrix;
};
} // namespace Siege

#endif
