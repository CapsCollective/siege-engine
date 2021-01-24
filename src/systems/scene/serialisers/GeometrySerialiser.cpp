#include "GeometrySerialiser.h"
#include "../../../entities/Geometry.h"
#include "../../../utils/StringHelpers.h"

std::string GeometrySerialiser::Serialise(Entity* entity)
{
    std::string fileData = EntitySerialiser::Serialise(entity);
    auto geometry = dynamic_cast<Geometry*>(entity);
    fileData += DEFINE_FIELD("DIMENSIONS", StringHelpers::VectorToString(geometry->GetDimensions()));
    fileData += DEFINE_FIELD("MODEL_PATH", geometry->GetModelData().GetModelPath());
    fileData += DEFINE_FIELD("TEXTURE_PATH", geometry->GetModelData().GetTexturePath());
    return fileData;
}

Entity* GeometrySerialiser::Deserialise(const std::vector<std::string>& entityArgs)
{
    // Get custom fields
    raylib::Vector3 dimensions = StringHelpers::StringToVector(entityArgs[CUSTOM_FIELD_1]);
    std::string modelPath = entityArgs[CUSTOM_FIELD_2];
    std::string texturePath = entityArgs[CUSTOM_FIELD_3];

    EntityData entityData = GetEntityData(entityArgs);
    return new Geometry(entityData.position, entityData.rotation, dimensions,ModelData(modelPath, texturePath));
}
