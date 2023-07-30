//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "OrthoCamera.h"

#include <utils/math/Projection.h>
#include <utils/math/Transform.h>

OrthoCamera::OrthoCamera(const Siege::Vec3& pos, const Siege::Vec3& front) :
    position {pos},
    front {front}
{}

void OrthoCamera::Update(float left, float right, float top, float bottom, float near, float far)
{
    cam.projection = Siege::Orthographic(left, right, bottom, top, near, far);
    cam.view = Siege::LookAt(position, front);
}