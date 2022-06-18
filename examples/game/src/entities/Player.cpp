//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Player.h"

#include <core/Statics.h>
#include <core/input/InputSystem.h>
#include <core/physics/CollisionSystem.h>
#include <core/render/Window.h>
#include <core/scene/SceneFile.h>

// Static member initialisation
const String Player::ENTITY_NAME("Player");

void Player::OnStart()
{
    // Register the entity with systems
    String modelPath = "models/cube/cube.obj";
    String texturePath = "models/cube/cube.png";
    Statics::Render().Add(this, {modelPath, texturePath});
}

void Player::OnUpdate()
{
    // Get move axes as vector
    Vec3 move = {
        (float) (-Statics::Input().KeyDown(Key::LEFT) + Statics::Input().KeyDown(Key::RIGHT)),
        0.f,
        (float) (-Statics::Input().KeyDown(Key::UP) + Statics::Input().KeyDown(Key::DOWN)),
    };

    // Normalise and apply move to velocity
    velocity += move.Normalise() * speed * Window::GetDeltaTime();

    // Apply force of gravity
    velocity += Vec3(0.f, -0.01f, 0.f);

    // Set the resulting attempted move's velocity to the object's position
    velocity = Statics::Collision().MoveAndSlide(GetBoundingBox(), velocity);
    transform.SetPosition(transform.GetPosition() + velocity);

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Player::OnDestroy()
{
    // Deregister the entity from systems before freeing it
    Statics::Render().Remove(this);
}

BoundedBox Player::GetBoundingBox() const
{
    Vec3 position = transform.GetPosition();
    return BoundedBox {
        position - Vec3::One,
        position + Vec3::One,
    };
}

Entity* Player::Clone() const
{
    return new Player(transform);
}

static Entity* Deserialise(const EntityData& data, const std::vector<String>& args)
{
    return new Player({data.position, data.rotation});
}

REGISTER_SERIALISATION_INTERFACE(Player::ENTITY_NAME, nullptr, Deserialise);
