//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Geometry.h"

#include <core/Statics.h>
#include <core/physics/CollisionSystem.h>
#include <core/scene/SceneFile.h>
#include <utils/Logging.h>

// Static member initialisation
const Siege::String Geometry::ENTITY_NAME("Geometry");

void Geometry::OnStart()
{
    // Register the entity with systems
    Siege::Statics::Collision().Add(this);
    Siege::Statics::Render().Add(this, {modelPath, texturePath});
}

void Geometry::OnDestroy()
{
    // Deregister the entity from systems before freeing it
    Siege::Statics::Render().Remove(this);
    Siege::Statics::Collision().Remove(this);
}

Siege::BoundedBox Geometry::GetBoundingBox() const
{
    return Siege::BoundedBox {
        GetPosition() - GetScale(),
        GetPosition() + GetScale(),
    };
}

const Siege::Vec3& Geometry::GetDimensions()
{
    return GetScale();
}

Siege::Entity* Geometry::Clone() const
{
    return new Geometry(transform);
}

Siege::ModelData Geometry::GetModelData()
{
    return {modelPath, texturePath};
}

static Siege::String Serialise(Siege::Entity* entity)
{
    Siege::String fileData;
    auto geometry = dynamic_cast<Geometry*>(entity);
    fileData += DefineField("DIMENSIONS", geometry->GetDimensions().ToString());

    auto modelData = geometry->GetModelData();
    fileData += DefineField("MODEL_PATH", modelData.modelPath);
    fileData += DefineField("TEXTURE_PATH", modelData.texturePath);
    return fileData;
}

static Siege::Entity* Deserialise(const Siege::EntityData& data,
                                  const std::vector<Siege::String>& args)
{
    Siege::Vec3 dimensions;
    if (!Siege::Vec3::FromString(dimensions, args[Siege::CUSTOM_FIELD_1]))
    {
        CC_LOG_WARNING("Failed to deserialise dimensions with value {}",
                       args[Siege::CUSTOM_FIELD_1]);
    }
    Siege::String modelPath = args[Siege::CUSTOM_FIELD_2];
    Siege::String texturePath = args[Siege::CUSTOM_FIELD_3];

    return new Geometry({data.position, data.rotation, dimensions}, modelPath, texturePath);
}

REGISTER_SERIALISATION_INTERFACE(Geometry::ENTITY_NAME, Serialise, Deserialise);
