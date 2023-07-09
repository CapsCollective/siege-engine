//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDER_EXAMPLE_ORTHOCAMERA_H
#define SIEGE_ENGINE_RENDER_EXAMPLE_ORTHOCAMERA_H

#include <utils/math/vec/Vec3.h>

#include "render/renderer/camera/Camera.h"

class OrthoCamera
{
public:

    OrthoCamera() = default;

    OrthoCamera(const Siege::Vec3& pos, const Siege::Vec3& front);
    void Update(float left, float right, float top, float bottom, float near, float far);

    inline const Siege::Camera& GetCamera() const
    {
        return cam;
    }

private:

    Siege::Camera cam {};
    Siege::Vec3 position {};
    Siege::Vec3 front {};
};

#endif // SIEGE_ENGINE_RENDER_EXAMPLE_ORTHOCAMERA_H
