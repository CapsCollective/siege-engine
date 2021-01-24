#include "PlayerSerialiser.h"
#include "../../../entities/Player.h"

std::string PlayerSerialiser::Serialise(Entity* entity)
{
    std::string fileData = EntitySerialiser::Serialise(entity);
    auto player = dynamic_cast<Player*>(entity);
    fileData += DEFINE_FIELD("MODEL_PATH", player->GetModelData().GetModelPath());
    fileData += DEFINE_FIELD("TEXTURE_PATH", player->GetModelData().GetTexturePath());
    return fileData;
}

Entity* PlayerSerialiser::Deserialise(const std::vector<std::string>& entityArgs)
{
    EntityData entityData = GetEntityData(entityArgs);
    return new Player(entityData.position, entityData.rotation);
}
