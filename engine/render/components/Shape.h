//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SHAPE_H
#define SIEGE_ENGINE_SHAPE_H

#include <utils/mat/Mat3.h>
#include <utils/mat/Mat4.h>
#include <utils/vec/Vec3.h>

#include "../renderer/Renderer.h"

namespace Components
{
struct Transform
{
    Siege::Vec3 position {Siege::Vec3::Zero};
    Siege::Vec3 scale {Siege::Vec3::One};
    Siege::Vec3 rotation {};
};

class Shape
{
public:

    Shape();
    explicit Shape(Siege::Model* model);

    ~Shape();

    Siege::Model::Transform GetTransform()
    {
        return {CalculateTransform(transform), CalculateNormalMatrix(transform)};
    };
    Siege::Model::Transform2D GetTransform2D()
    {
        return {CalculateTransform(transform)};
    };

    Siege::Vec3& GetColor()
    {
        return fillColor;
    }
    Siege::Model* GetModel()
    {
        return model;
    }

    Siege::Vec3& GetRotation()
    {
        return transform.rotation;
    }
    Siege::Vec3& GetPosition()
    {
        return transform.position;
    }
    Siege::Vec3& GetScale()
    {
        return transform.scale;
    }

    Siege::Vec2 GetPosition2D()
    {
        return transform.position;
    }
    Siege::Vec2 GetScale2D()
    {
        return transform.scale;
    }
    float GetRotation2D()
    {
        return transform.rotation.z;
    }
    float GetZIndex()
    {
        return transform.position.z;
    }

    const Siege::Vec3& GetRotation() const
    {
        return transform.rotation;
    }
    const Siege::Vec3& GetPosition() const
    {
        return transform.position;
    }
    const Siege::Vec3& GetScale() const
    {
        return transform.scale;
    }
    const float GetZIndex() const
    {
        return transform.position.z;
    }

    const Siege::Vec2 GetPosition2D() const
    {
        return transform.position;
    }
    const Siege::Vec2 GetScale2D() const
    {
        return transform.scale;
    }
    const float GetRotation2D() const
    {
        return transform.rotation.z;
    }

    void SetColor(const Siege::Vec3& newColor);
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

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    Siege::Mat4 CalculateTransform(Transform& transform);

    Siege::Mat3 CalculateNormalMatrix(Transform& transform);

    Siege::Model* model;
    Transform transform {};
    Siege::Vec3 fillColor {};
};
} // namespace Components

#endif