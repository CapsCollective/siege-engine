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

Shape::Shape() {}

Shape::Shape(Siege::Model* model) : model {model} {}

Shape::~Shape() {}

void Shape::SetColor(const Siege::Vec3& newColor)
{
    fillColor = newColor;
}
void Shape::SetPosition(const Siege::Vec3& newPos)
{
    transform.SetPosition(newPos);
}

void Shape::SetScale(const Siege::Vec3& newScale)
{
    transform.SetScale(newScale);
}

void Shape::SetRotation(const Siege::Vec3& rotation)
{
    transform.SetRotation(rotation);
}

void Shape::SetRotationX(float rotation)
{
    transform.SetRotationX(rotation);
}

void Shape::SetRotationY(float rotation)
{
    transform.SetRotationY(rotation);
}

void Shape::SetRotationZ(float rotation)
{
    transform.SetRotationZ(rotation);
}

void Shape::SetZIndex(float zIndex)
{
    transform.SetPositionZ(zIndex);
}

void Shape::SetRotation2D(float rotation)
{
    transform.SetRotationZ(rotation);
}

void Shape::SetPosition2D(const Siege::Vec2& newPos)
{
    transform.SetPosition({newPos.x, newPos.y});
}

void Shape::SetScale2D(const Siege::Vec2& newScale)
{
    transform.SetScale(newScale);
}
} // namespace Components
