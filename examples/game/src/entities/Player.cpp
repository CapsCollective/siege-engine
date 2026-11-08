//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Player.h"

#include <audio/AudioSystem.h>
#include <core/Statics.h>
#include <core/physics/CollisionSystem.h>
#include <core/scene/SceneFile.h>
#include <resources/ResourceSystem.h>
#include <resources/PackFile.h>
#include <window/Input.h>

#include "../ServiceLocator.h"

void Player::OnStart()
{
    Entity::OnStart();

    Siege::PackFile* packFile = Siege::ResourceSystem::GetInstance().GetPackFile();
    clickSoundSource.clipData = packFile->FindData("assets/audio/click.wav");
    clickSoundSource.contextToken = Siege::Token::FindOrRegisterToken("click_clip");
    musicSoundSource.clipData = packFile->FindData("assets/audio/call_to_adventure.mp3");
    musicSoundSource.looping = true;
    musicSoundSource.maxPolyphony = 1;
    musicSoundSource.contextToken = Siege::Token::FindOrRegisterToken("music_clip");
}
void Player::OnDestroy()
{
    Siege::AudioSystem::GetInstance().StopSoundSource(clickSoundSource);
    Siege::AudioSystem::GetInstance().StopSoundSource(musicSoundSource);

    Entity::OnDestroy();
}

void Player::OnUpdate()
{
    // Get move axes as vector
    Siege::Vec3 move = {
        (float) (-Siege::Input::IsKeyDown(Siege::Key::KEY_LEFT) +
                 Siege::Input::IsKeyDown(Siege::Key::KEY_RIGHT)),
        0.f,
        (float) (Siege::Input::IsKeyDown(Siege::Key::KEY_UP) +
                 -Siege::Input::IsKeyDown(Siege::Key::KEY_DOWN)),
    };

    // Normalise and apply move to velocity
    float deltaTime = ServiceLocator::GetTicker()->GetDeltaTime();
    velocity += Siege::Vec3::Normalise(move) * speed * deltaTime;

    bool pressedC = Siege::Input::IsKeyJustPressed(Siege::Key::KEY_C);
    if (pressedC)
    {
        Siege::AudioSystem::GetInstance().PlaySoundSource(clickSoundSource);
    }

    bool pressedF = Siege::Input::IsKeyJustPressed(Siege::Key::KEY_F);
    if (pressedF)
    {
        Siege::AudioSystem::GetInstance().PlaySoundSource(musicSoundSource);
    }

    bool pressedG = Siege::Input::IsKeyJustPressed(Siege::Key::KEY_G);
    if (pressedG)
    {
        Siege::AudioSystem::GetInstance().StopSoundSource(musicSoundSource);
    }

    // Apply force of gravity
    velocity += {0.f, 0.01f, 0.f};

    // Set the resulting attempted move's velocity to the object's position
    velocity = Siege::Statics::Collision().MoveAndSlide(GetBoundingBox(), velocity);
    transform.SetPosition(transform.GetPosition() + velocity);

    // Dampen velocity
    velocity = velocity * 0.9f;
}

Siege::BoundedBox Player::GetBoundingBox() const
{
    Siege::Vec3 position = transform.GetPosition();
    return Siege::BoundedBox {
        position - Siege::Vec3::One(),
        position + Siege::Vec3::One(),
    };
}

Siege::Entity* Player::Clone() const
{
    return new Player(transform);
}

void Player::OnDraw3D()
{
    Siege::Renderer3D::DrawMesh(ServiceLocator::GetRenderResources()->GetCubeMesh(),
                                GetPosition(),
                                GetScale(),
                                GetRotation());
}

static Siege::Entity* Deserialise(const std::map<Siege::Token, Siege::String>& attributes)
{
    Siege::EntityData data = Siege::SceneFile::GetBaseEntityData(attributes);
    return new Player({data.position, data.rotation});
}

REGISTER_SERIALISATION_INTERFACE(TOKEN_Player, nullptr, Deserialise);
