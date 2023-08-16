//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GAME_OBJECT_H
#define SIEGE_ENGINE_GAME_OBJECT_H

#include <render/renderer/platform/vulkan/StaticMesh.h>
#include <utils/math/Xform.h>
#include <utils/math/vec/Vec3.h>

struct Transform
{
    Siege::Vec3 position {Siege::Vec3::Zero()};
    Siege::Vec3 scale {Siege::Vec3::One()};
    Siege::Vec3 rotation {};
};

class GameObject
{
public:

    GameObject();
    explicit GameObject(Siege::Vulkan::StaticMesh* mesh);

    ~GameObject();

    Siege::Vulkan::StaticMesh* GetMesh()
    {
        return mesh;
    }

    Siege::Vec3 GetRotation()
    {
        return transform.GetRotation();
    }

    Siege::Vec3 GetPosition()
    {
        return transform.GetPosition();
    }

    Siege::Vec3 GetScale()
    {
        return transform.GetScale();
    }

    void SetColour(const Siege::IColour& newColor);
    void SetScale(const Siege::Vec3& newScale);
    void SetPosition(const Siege::Vec3& newPos);
    void SetRotationX(float rotation);
    void SetRotationY(float rotation);

private:

    Siege::Xform transform {};
    Siege::Vulkan::StaticMesh* mesh {};
    Siege::IColour fillColor {};
};
#endif