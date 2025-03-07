//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_STATICS_H
#define SIEGE_ENGINE_STATICS_H

namespace Siege
{
class Statics
{
public:

    static class CollisionSystem& Collision();

    static class SceneSystem& Scene();

    static class EntitySystem& Entity();

    static class EntitySystem& Tool();
};
} // namespace Siege

#endif // SIEGE_ENGINE_STATICS_H
