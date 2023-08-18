//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PLAYER_H
#define SIEGE_ENGINE_PLAYER_H

#include <core/entity/Entity.h>

class Player : public Siege::Entity
{
public:

    // Public constants

    static const Siege::String ENTITY_NAME;

    // 'Structors

    Player() : Player({Siege::Vec3::Zero(), 0.f}) {};

    explicit Player(const Siege::Xform& transform) :
        Entity(ENTITY_NAME, transform),
        speed(1.5f),
        velocity(Siege::Vec3::Zero()) {};

    // Public overrides

    Entity* Clone() const override;

    Siege::BoundedBox GetBoundingBox() const override;

    void OnDraw3D() override;

protected:

    // Protected overrides

    void OnUpdate() override;

private:

    // Private fields

    float speed;

    Siege::Vec3 velocity;
};

#endif // SIEGE_ENGINE_PLAYER_H
