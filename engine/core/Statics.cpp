//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Statics.h"

#include "input/InputSystem.h"
#include "physics/CollisionSystem.h"
#include "render/RenderSystem.h"
#include "render/ResourceSystem.h"
#include "scene/SceneSystem.h"

namespace Siege
{
CollisionSystem& Statics::Collision()
{
    static CollisionSystem system;
    return system;
}

InputSystem& Statics::Input()
{
    static InputSystem system;
    return system;
}

ResourceSystem& Statics::Resource()
{
    static ResourceSystem system;
    return system;
}

RenderSystem& Statics::Render()
{
    static RenderSystem system;
    return system;
}

SceneSystem& Statics::Scene()
{
    static SceneSystem system;
    return system;
}

class EntitySystem& Statics::Entity()
{
    static EntitySystem system;
    return system;
}

class EntitySystem& Statics::Tool()
{
    static EntitySystem system;
    return system;
}
}