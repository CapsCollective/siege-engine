//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "GameObject.h"

GameObject::GameObject() {}

GameObject::GameObject(Siege::Vulkan::StaticMesh* newMesh) : mesh {newMesh} {}

GameObject::~GameObject() {}

void GameObject::SetColour(const Siege::IColour& newColor)
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

void GameObject::SetRotationX(float rotation)
{
    transform.SetRotationX(rotation);
}

void GameObject::SetRotationY(float rotation)
{
    transform.SetRotationY(rotation);
}