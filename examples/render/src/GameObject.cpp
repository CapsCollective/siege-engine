//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "GameObject.h"

GameObject::GameObject() {}

GameObject::GameObject(Siege::Model* model) : model {model} {}

GameObject::~GameObject() {}

void GameObject::SetColor(const Siege::Vec3& newColor)
{
    fillColor = newColor;
}
void GameObject::SetPosition(const Siege::Vec3& newPos)
{
    transform.SetPosition(newPos);
}

void GameObject::SetScale(const Siege::Vec3& newScale)
{
    transform.SetScale(newScale);
}

void GameObject::SetRotation(const Siege::Vec3& rotation)
{
    transform.SetRotation(rotation);
}

void GameObject::SetRotationX(float rotation)
{
    transform.SetRotationX(rotation);
}

void GameObject::SetRotationY(float rotation)
{
    transform.SetRotationY(rotation);
}

void GameObject::SetRotationZ(float rotation)
{
    transform.SetRotationZ(rotation);
}

void GameObject::SetZIndex(float zIndex)
{
    transform.SetPositionZ(zIndex);
}

void GameObject::SetRotation2D(float rotation)
{
    transform.SetRotationZ(rotation);
}

void GameObject::SetPosition2D(const Siege::Vec2& newPos)
{
    transform.SetPosition({newPos.x, newPos.y, transform.GetPosition().z});
}

void GameObject::SetScale2D(const Siege::Vec2& newScale)
{
    transform.SetScale(newScale);
}