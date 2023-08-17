//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Statics.h"

#include "ResourceSystem.h"
#include "physics/CollisionSystem.h"
#include "scene/SceneSystem.h"

namespace Siege
{
CollisionSystem& Statics::Collision()
{
    static CollisionSystem system;
    return system;
}

ResourceSystem& Statics::Resource()
{
    static ResourceSystem system;
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
} // namespace Siege