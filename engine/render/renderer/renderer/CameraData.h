//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER_COMMON_H
#define SIEGE_ENGINE_RENDERER_COMMON_H

#include <utils/math/mat/Mat4.h>

namespace Siege
{
struct CameraData
{
    Mat4 projectionMatrix;
    Mat4 viewMatrix;
};
} // namespace Siege

#endif // SIEGE_ENGINE_RENDERER_COMMON_H
