//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SHAPE_H
#define SIEGE_ENGINE_SHAPE_H

#include <glm/gtc/matrix_transform.hpp>

#include "../renderer/Renderer.h"

namespace Components
{
struct Transform
{
    glm::vec3 position {0.0f};
    glm::vec3 scale {1.f, 1.f, 1.f};
    glm::vec3 rotation {};
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
    glm::vec3& GetColor()
    {
        return fillColor;
    }
    Siege::Model* GetModel()
    {
        return model;
    }

    glm::vec3& GetRotation()
    {
        return transform.rotation;
    }
    glm::vec3& GetPosition()
    {
        return transform.position;
    }
    glm::vec3& GetScale()
    {
        return transform.scale;
    }

    glm::vec2 GetPosition2D()
    {
        return glm::vec2(transform.position);
    }
    glm::vec2 GetScale2D()
    {
        return glm::vec2(transform.scale);
    }
    float GetRotation2D()
    {
        return transform.rotation.z;
    }
    float GetZIndex()
    {
        return transform.position.z;
    }

    const glm::vec3& GetRotation() const
    {
        return transform.rotation;
    }
    const glm::vec3& GetPosition() const
    {
        return transform.position;
    }
    const glm::vec3& GetScale() const
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

    void SetColor(glm::vec3 newColor);
    void SetScale(glm::vec3 newScale);
    void SetPosition(glm::vec3 newPos);
    void SetRotation(glm::vec3 rotation);
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
    glm::vec3 fillColor {0.0f};
};
} // namespace Components

#endif