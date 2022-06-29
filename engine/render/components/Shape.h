//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "../renderer/Renderer.h"

namespace Components
{
struct Transform
{
    Vec3 position {Vec3::Zero};
    Vec3 scale {1.f, 1.f, 1.f};
    Vec3 rotation {};
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
    Vec3& GetColor()
    {
        return fillColor;
    }
    Siege::Model* GetModel()
    {
        return model;
    }

    Vec3& GetRotation()
    {
        return transform.rotation;
    }
    Vec3& GetPosition()
    {
        return transform.position;
    }
    Vec3& GetScale()
    {
        return transform.scale;
    }

    glm::vec2 GetPosition2D()
    {
        return glm::vec2(transform.position.x, transform.position.y);
    }
    glm::vec2 GetScale2D()
    {
        return glm::vec2(transform.scale.x, transform.scale.y);
    }
    float GetRotation2D()
    {
        return transform.rotation.z;
    }
    float GetZIndex()
    {
        return transform.position.z;
    }

    const Vec3& GetRotation() const
    {
        return transform.rotation;
    }
    const Vec3& GetPosition() const
    {
        return transform.position;
    }
    const Vec3& GetScale() const
    {
        return transform.scale;
    }
    const float GetZIndex() const
    {
        return transform.position.z;
    }

    const glm::vec2 GetPosition2D() const
    {
        return glm::vec2(transform.position.x, transform.position.y);
    }
    const glm::vec2 GetScale2D() const
    {
        return glm::vec2(transform.scale.x, transform.scale.y);
    }
    const float GetRotation2D() const
    {
        return transform.rotation.z;
    }

    void SetColor(Vec3 newColor);
    void SetScale(Vec3 newScale);
    void SetPosition(Vec3 newPos);
    void SetRotation(Vec3 rotation);
    void SetRotationX(float rotation);
    void SetRotationY(float rotation);
    void SetRotationZ(float rotation);

    void SetZIndex(float zIndex);
    void SetPosition2D(glm::vec2 newPos);
    void SetScale2D(glm::vec2 newPos);
    void SetRotation2D(float rotation);

private:

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 CalculateTransform(Transform& transform);

    glm::mat3 CalculateNormalMatrix(Transform& transform);

    Siege::Model* model;
    Transform transform {};
    Vec3 fillColor {Vec3::Zero};
};
} // namespace Components