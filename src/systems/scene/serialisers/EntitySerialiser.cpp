#include "EntitySerialiser.h"
#include "../../../utils/StringHelpers.h"

const std::string& EntitySerialiser::GetName()
{
    return name;
}

std::string EntitySerialiser::Serialise(Entity* entity)
{
    return (entity->GetName() + SEP +
            DEFINE_FIELD("POSITION", StringHelpers::VectorToString(entity->GetPosition())) +
            DEFINE_FIELD("ROTATION", std::to_string(entity->GetRotation())) +
            DEFINE_FIELD("Z-INDEX", std::to_string(entity->GetZIndex())));
}

EntityData EntitySerialiser::GetEntityData(const std::vector<std::string> &entityArgs)
{
    // Get the position, rotation and z-index of the entity
    return EntityData(StringHelpers::StringToVector(entityArgs[ENTITY_POS]),
                                       std::stof(entityArgs[ENTITY_ROT]),
                                       std::stoi(entityArgs[ENTITY_Z_IDX]));
}
