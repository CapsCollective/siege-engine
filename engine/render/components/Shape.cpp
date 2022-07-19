//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Shape.h"

namespace Components
{
Siege::Mat4 Shape::CalculateTransform(Transform& transform)
{
    const float c3 = glm::cos(transform.rotation.z);
    const float s3 = glm::sin(transform.rotation.z);
    const float c2 = glm::cos(transform.rotation.x);
    const float s2 = glm::sin(transform.rotation.x);
    const float c1 = glm::cos(transform.rotation.y);
    const float s1 = glm::sin(transform.rotation.y);

    return {{
                transform.scale.x * (c1 * c3 + s1 * s2 * s3),
                transform.scale.x * (c2 * s3),
                transform.scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                transform.scale.y * (c3 * s1 * s2 - c1 * s3),
                transform.scale.y * (c2 * c3),
                transform.scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                transform.scale.z * (c2 * s1),
                transform.scale.z * (-s2),
                transform.scale.z * (c1 * c2),
                0.0f,
            },
            {transform.position.x, transform.position.y, transform.position.z, 1.0f}};
}

Siege::Mat3 Shape::CalculateNormalMatrix(Transform& transform)
{
    const float c3 = glm::cos(transform.rotation.z);
    const float s3 = glm::sin(transform.rotation.z);
    const float c2 = glm::cos(transform.rotation.x);
    const float s2 = glm::sin(transform.rotation.x);
    const float c1 = glm::cos(transform.rotation.y);
    const float s1 = glm::sin(transform.rotation.y);

    Siege::Vec3 inverseScale =
        1.0f / Siege::Vec3 {transform.scale.x, transform.scale.y, transform.scale.z};

    return {{
                inverseScale.x * (c1 * c3 + s1 * s2 * s3),
                inverseScale.x * (c2 * s3),
                inverseScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                inverseScale.y * (c3 * s1 * s2 - c1 * s3),
                inverseScale.y * (c2 * c3),
                inverseScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                inverseScale.z * (c2 * s1),
                inverseScale.z * (-s2),
                inverseScale.z * (c1 * c2),
            }};
}

Shape::Shape() {}

Shape::Shape(Siege::Model* model) : model {model} {}

Shape::~Shape() {}

void Shape::SetColor(const Siege::Vec3& newColor)
{
    fillColor = newColor;
}
void Shape::SetPosition(const Siege::Vec3& newPos)
{
    transform.position = newPos;
}

void Shape::SetScale(const Siege::Vec3& newScale)
{
    transform.scale = newScale;
}

void Shape::SetRotation(const Siege::Vec3& rotation)
{
    transform.rotation = rotation;
}

void Shape::SetRotationX(float rotation)
{
    transform.rotation.x = rotation;
}

void Shape::SetRotationY(float rotation)
{
    transform.rotation.y = rotation;
}

void Shape::SetRotationZ(float rotation)
{
    transform.rotation.z = rotation;
}

void Shape::SetZIndex(float zIndex)
{
    transform.position.z = zIndex;
}

void Shape::SetRotation2D(float rotation)
{
    transform.rotation.z = rotation;
}

void Shape::SetPosition2D(const Siege::Vec2& newPos)
{
    transform.position = {newPos.x, newPos.y, transform.position.z};
}

void Shape::SetScale2D(const Siege::Vec2& newScale)
{
    transform.scale = newScale;
}
} // namespace Components