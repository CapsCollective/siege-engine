#include "Geometry.h"
#include "../utils/StringHelpers.h"
#include "../systems/scene/SceneSerialiser.h"

// Static member initialisation
const std::string Geometry::ENTITY_NAME("Geometry");

void Geometry::OnDraw()
{
    // Set the model's texture to this entity's texture
    ModelData::SetTexture(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            ResourceManager::Get<Texture2D>(modelData.GetTexturePath()));

    // Draw the model
    DrawModelEx(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3(0, 1, 0),
            rotation,
            dimensions,
            WHITE
    );

    // Draw the model wireframe
    DrawModelWiresEx(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3(0, 1, 0),
            rotation,
            dimensions,
            PINK
    );
}

BoundingBox Geometry::GetBoundingBox()
{
    return BoundingBox {
            position - raylib::Vector3(dimensions.x, dimensions.y, dimensions.z),
            position + raylib::Vector3(dimensions.x, dimensions.y, dimensions.z),
    };
}

const raylib::Vector3& Geometry::GetDimensions()
{
    return dimensions;
}

Entity* Geometry::Clone()
{
    return new Geometry(position, rotation, dimensions, modelData);
}

void Geometry::QueueFree()
{
    // Deregister the entity as collidable before freeing it
    CollisionSystem::Remove(this);
    Entity::QueueFree();
}

const ModelData& Geometry::GetModelData()
{
    return modelData;
}

void Geometry::SetModelData(const ModelData &data)
{
    modelData = data;
}

static std::string Serialise(Entity* entity)
{
    std::string fileData;
    auto geometry = dynamic_cast<Geometry*>(entity);
    fileData += DefineField("DIMENSIONS", StringHelpers::VectorToString(geometry->GetDimensions()));
    fileData += DefineField("MODEL_PATH", geometry->GetModelData().GetModelPath());
    fileData += DefineField("TEXTURE_PATH", geometry->GetModelData().GetTexturePath());
    return fileData;
}

static Entity* Deserialise(const EntityData& data, const std::vector<std::string>& args)
{
    raylib::Vector3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);
    std::string modelPath = args[CUSTOM_FIELD_2];
    std::string texturePath = args[CUSTOM_FIELD_3];

    return new Geometry(
            data.position,
            data.rotation,
            dimensions,
            ModelData(modelPath, texturePath));
}

REGISTER_SERIALISATION_INTERFACE(Geometry::ENTITY_NAME, Serialise, Deserialise);
