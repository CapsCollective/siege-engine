//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_GAME_OBJECT_H
#define SIEGE_ENGINE_GAME_OBJECT_H

#include <render/renderer/model/Model.h>
#include <utils/math/Xform.h>
#include <utils/math/mat/Mat3.h>
#include <utils/math/mat/Mat4.h>
#include <utils/math/vec/Vec3.h>

struct Transform
{
    Siege::Vec3 position {Siege::Vec3::Zero};
    Siege::Vec3 scale {Siege::Vec3::One};
    Siege::Vec3 rotation {};
};

class GameObject
{
public:

    GameObject();
    explicit GameObject(Siege::Model* model);

    ~GameObject();

    Siege::IColour& GetColor()
    {
        return fillColor;
    }

    Siege::Model* GetModel()
    {
        return model;
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

    Siege::Vec2 GetPosition2D()
    {
        return transform.GetPosition();
    }

    Siege::Vec2 GetScale2D()
    {
        return transform.GetScale();
    }

    float GetRotation2D()
    {
        return transform.GetRotation().z;
    }
    float GetZIndex()
    {
        return transform.GetPosition().z;
    }

    const float GetZIndex() const
    {
        return transform.GetPosition().z;
    }

    const Siege::Vec2 GetPosition2D() const
    {
        return transform.GetPosition();
    }
    const Siege::Vec2 GetScale2D() const
    {
        return transform.GetScale();
    }
    const float GetRotation2D() const
    {
        return transform.GetRotation().z;
    }

    void SetColour(const Siege::IColour& newColor);
    void SetScale(const Siege::Vec3& newScale);
    void SetPosition(const Siege::Vec3& newPos);
    void SetRotation(const Siege::Vec3& rotation);
    void SetRotationX(float rotation);
    void SetRotationY(float rotation);
    void SetRotationZ(float rotation);

    void SetZIndex(float zIndex);
    void SetPosition2D(const Siege::Vec2& newPos);
    void SetScale2D(const Siege::Vec2& newPos);
    void SetRotation2D(float rotation);

private:

    Siege::Xform transform {};
    Siege::Model* model;
    Siege::IColour fillColor {};
};
#endif