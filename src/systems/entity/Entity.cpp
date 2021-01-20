#include "Entity.h"
#include <utility>
#include "EntityStorage.h"

void Entity::QueueFree()
{
    EntityStorage::QueueFree(this);
}

void Entity::Free()
{
    EntityStorage::Remove(this);
}

const std::string& Entity::GetName() const
{
    return name;
}

const GenerationalIndex& Entity::GetIndex() const
{
    return index;
}

void Entity::SetName(std::string entityName)
{
    name = std::move(entityName);
}

void Entity::SetIndex(GenerationalIndex idx)
{
    index = idx;
}
