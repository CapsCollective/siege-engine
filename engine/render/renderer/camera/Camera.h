//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_CAMERA_H
#define SIEGE_ENGINE_RENDER_CAMERA_H

#include <utils/math/mat/Mat4.h>
#include <utils/math/vec/Vec3.h>

namespace Siege
{
struct Camera
{
    static inline constexpr Mat4 ViewProjection(const Camera& camera)
    {
        return camera.projection * camera.view;
    }

    Mat4 projection;
    Mat4 view;
};
} // namespace Siege

#endif // SIEGE_ENGINE_RENDER_CAMERA_H
