#include "Player.h"
#include <collision/CollisionSystem.h>
#include <scene/SceneSerialiser.h>
#include <render/RenderSystem.h>
#include <input/Input.h>
#include <window/Window.h>

// Static member initialisation
const std::string Player::ENTITY_NAME("Player");

void Player::OnStart()
{
    // Register the entity with systems
    std::string modelPath = "models/cube/cube.obj";
    std::string texturePath = "models/cube/cube.png";
    RenderSystem::Add(this, {modelPath, texturePath});
}

void Player::OnUpdate()
{
    // Get move axes as vector
    Vec3 move = {
            (float)(-Input::KeyDown(Input::KEY_LEFT) + Input::KeyDown(Input::KEY_RIGHT)),
            0.f,
            (float)(-Input::KeyDown(Input::KEY_UP) + Input::KeyDown(Input::KEY_DOWN)),
    };


    // Normalise and apply move to velocity
    velocity += move.Normalise() * speed * Window::GetDeltaTime();

    // Apply force of gravity
    velocity += Vec3(0.f, -0.01f, 0.f);

    // Set the resulting attempted move's velocity to the object's position
    velocity = CollisionSystem::MoveAndSlide(GetBoundingBox(), velocity);
    transform.SetPosition(transform.GetPosition() + velocity);

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Player::OnDestroy()
{
    // Deregister the entity from systems before freeing it
    RenderSystem::Remove(this);
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

static Entity* Deserialise(const EntityData& data, const std::vector<std::string>& args)
{
    return new Player({data.position, data.rotation});
}

REGISTER_SERIALISATION_INTERFACE(Player::ENTITY_NAME, nullptr, Deserialise);
