#include "Entity.h"

#include <utils/Logging.h>

#include <cmath>
#include <utility>

#include "EntitySystem.h"

// Static member initialisation
const String Entity::ENTITY_NAME("Entity");

Entity::Entity(const String& name, const Xform& transform, int zIndex) :
    transform(transform),
    name(name),
    index(GenerationalIndex()),
    zIndex(zIndex)
{
    CC_LOG_INFO("Registered {} at ({})", GetName(), GetIndex().ToString());
}

BoundedBox Entity::GetBoundingBox() const
{
    return {};
}

Entity* Entity::Clone() const
{
    return nullptr;
}

void Entity::QueueFree()
{
    OnDestroy();
    CC_LOG_INFO("Freeing {} at ({})", GetName(), GetIndex().ToString());
    Statics::Entity().QueueFree(this);
}

const String& Entity::GetName() const
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

const Xform& Entity::GetTransform() const
{
    return transform;
}

int Entity::GetZIndex() const
{
    return zIndex;
}

const Vec3& Entity::GetPosition() const
{
    return transform.GetPosition();
}

float Entity::GetRotation() const
{
    return transform.GetRotation();
}

const Vec3& Entity::GetScale() const
{
    return transform.GetScale();
}

void Entity::SetPosition(const Vec3& position)
{
    transform.SetPosition(position);
}

void Entity::SetRotation(float rotation)
{
    transform.SetRotation(fmod(rotation, 360.f));
}

void Entity::SetScale(const Vec3& scale)
{
    transform.SetScale(scale);
}

void Entity::SetZIndex(int idx)
{
    // Only update z index if there's an actual change
    if (idx == zIndex) return;

    // Swap the values
    int oldZIndex = zIndex;
    zIndex = idx;

    // Inform the entity system of the change
    Statics::Entity().SortPartial(this, oldZIndex);
}
