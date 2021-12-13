#include "Geometry.h"
#include <utils/StringHelpers.h>
#include <scene/SceneSerialiser.h>
#include <physics/CollisionSystem.h>

// Static member initialisation
const std::string Geometry::ENTITY_NAME("Geometry");

void Geometry::OnStart()
{
    // Register the entity with systems
    CollisionSystem::Add(this);
    RenderSystem::Add(this, {modelPath, texturePath});
}

void Geometry::OnDestroy()
{
    // Deregister the entity from systems before freeing it
    RenderSystem::Remove(this);
    CollisionSystem::Remove(this);
}

BoundedBox Geometry::GetBoundingBox() const
{
    return BoundedBox {
        GetPosition() - GetScale(),
        GetPosition() + GetScale(),
    };
}

const Vec3& Geometry::GetDimensions()
{
    return GetScale();
}

Entity* Geometry::Clone() const
{
    return new Geometry(transform);
}

ModelData Geometry::GetModelData()
{
    return {modelPath, texturePath};
}

static std::string Serialise(Entity* entity)
{
    std::string fileData;
    auto geometry = dynamic_cast<Geometry*>(entity);
    fileData += DefineField("DIMENSIONS", geometry->GetDimensions().ToString());

    auto modelData = geometry->GetModelData();
    fileData += DefineField("MODEL_PATH", modelData.modelPath);
    fileData += DefineField("TEXTURE_PATH", modelData.texturePath);
    return fileData;
}

static Entity* Deserialise(const EntityData& data, const std::vector<std::string>& args)
{
    Vec3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);
    std::string modelPath = args[CUSTOM_FIELD_2];
    std::string texturePath = args[CUSTOM_FIELD_3];

    return new Geometry({data.position, data.rotation, dimensions}, modelPath, texturePath);
}

REGISTER_SERIALISATION_INTERFACE(Geometry::ENTITY_NAME, Serialise, Deserialise);
