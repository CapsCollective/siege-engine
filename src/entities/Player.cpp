#include "Player.h"
#include "../resources/ResourceManager.h"

void Player::OnUpdate()
{
    // Get move axes as vector
    raylib::Vector3 move = {
            (float)(-IsKeyDown(KEY_LEFT) + IsKeyDown(KEY_RIGHT)),
            0.f,
            (float)(-IsKeyDown(KEY_UP) + IsKeyDown(KEY_DOWN)),
    };

    // Normalise and apply move to velocity
    position += velocity += move.Normalize() * speed * GetFrameTime();

    // Dampen velocity
    velocity = velocity * 0.9f;
}

void Player::OnDraw()
{
    auto model = ResourceManager::GetRef<raylib::Model>(modelData.modelName);
    model->materials[0].maps[MAP_DIFFUSE].texture = ResourceManager::Get<raylib::Texture2D>(modelData.textureName);

    DrawModelEx
    (
        ResourceManager::Get<raylib::Model>(modelData.modelName),
        position,
        raylib::Vector3::Zero(),
        0.0f,
        raylib::Vector3(1.0f, 1.0f, 1.0f),
        RED
    );
    DrawModelWiresEx
    (
        ResourceManager::Get<raylib::Model>(modelData.modelName),
        position,
        raylib::Vector3::Zero(),
        0.0f,
        raylib::Vector3(1.0f, 1.0f, 1.0f),
        MAROON
    );
}

void Player::OnUIDraw()
{
    DrawText("Move with the arrow keys", 10.f, GetScreenHeight() - 30, 20.f, DARKGRAY);
}

BoundingBox Player::GetBoundingBox()
{
    return BoundingBox{
            position - raylib::Vector3::One(),
            position + raylib::Vector3::One(),
    };
}
