//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_PLAYER_H
#define SIEGE_ENGINE_PLAYER_H

#include <core/entity/Entity.h>
#include <core/render/RenderSystem.h>

class Player : public Entity
{
public:

    // Public constants

    static const String ENTITY_NAME;

    // 'Structors

    Player() : Player({Vec3::Zero, 0.f}) {};

    explicit Player(const Xform& transform) :
        Entity(ENTITY_NAME, transform),
        speed(1.5f),
        velocity(Vec3::Zero) {};

    // Public overrides

    Entity* Clone() const override;

    BoundedBox GetBoundingBox() const override;

protected:

    // Protected overrides

    void OnStart() override;

    void OnUpdate() override;

    void OnDestroy() override;

private:

    // Private fields

    float speed;

    Vec3 velocity;
};

#endif // SIEGE_ENGINE_PLAYER_H
