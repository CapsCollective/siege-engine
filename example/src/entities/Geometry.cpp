#include "Geometry.h"

#include <physics/CollisionSystem.h>
#include <scene/SceneFile.h>

#include "utils/Logging.h"
#include "utils/Statics.h"

// Static member initialisation
const String Geometry::ENTITY_NAME("Geometry");

void Geometry::OnStart()
{
    // Register the entity with systems
    Statics::CollisionSystem.Add(this);
    RenderSystem::Add(this, {modelPath, texturePath});
}

void Geometry::OnDestroy()
{
    // Deregister the entity from systems before freeing it
    RenderSystem::Remove(this);
    Statics::CollisionSystem.Remove(this);
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

static String Serialise(Entity* entity)
{
    String fileData;
    auto geometry = dynamic_cast<Geometry*>(entity);
    fileData += DefineField("DIMENSIONS", geometry->GetDimensions().ToString());

    auto modelData = geometry->GetModelData();
    fileData += DefineField("MODEL_PATH", modelData.modelPath);
    fileData += DefineField("TEXTURE_PATH", modelData.texturePath);
    return fileData;
}

static Entity* Deserialise(const EntityData& data, const std::vector<String>& args)
{
    Vec3 dimensions;
    if (!Vec3::FromString(dimensions, args[CUSTOM_FIELD_1]))
    {
        CC_LOG_WARNING("Failed to deserialise dimensions with value {}", args[CUSTOM_FIELD_1]);
    }
    String modelPath = args[CUSTOM_FIELD_2];
    String texturePath = args[CUSTOM_FIELD_3];

    return new Geometry({data.position, data.rotation, dimensions}, modelPath, texturePath);
}

REGISTER_SERIALISATION_INTERFACE(Geometry::ENTITY_NAME, Serialise, Deserialise);
