#include "EntityStorage.h"
#include "Entity.h"
#include <utility>
#include <cmath>

// Static member initialisation
const std::string Entity::ENTITY_NAME("Entity");

BoundedBox Entity::GetBoundingBox() const
{
    return BoundedBox();
}

Entity* Entity::Clone() const
{
    return nullptr;
}

void Entity::QueueFree()
{
    EntityStorage::QueueFree(this);
}

const std::string& Entity::GetName() const
{
    return name;
}

const GenerationalIndex& Entity::GetIndex() const
{
    return index;
}

void Entity::SetIndex(GenerationalIndex idx)
{
    index = idx;
}

const Vec3& Entity::GetPosition() const
{
    return position;
}

float Entity::GetRotation() const
{
    return rotation;
}

int Entity::GetZIndex() const
{
    return zIndex;
}

void Entity::SetPosition(Vec3 newPosition)
{
    position = newPosition;
}

void Entity::SetRotation(float newRotation)
{
    rotation = fmod(newRotation, 360.f);
}

void Entity::SetZIndex(int idx)
{
    // Only update z index if there's an actual change
    if (idx == zIndex) return;

    // Swap the values
    int oldZIndex = zIndex;
    zIndex = idx;

    // Inform the entity system of the change
    EntityStorage::SortPartial(this, oldZIndex);
}
