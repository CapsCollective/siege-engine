#include "Geometry.h"
#include "../resources/ResourceManager.h"

void Geometry::OnDraw()
{
    // Set the model's texture to this entity's texture
    ModelData::SetTexture(ResourceManager::Get<raylib::Model>(modelData.GetModelPath()),
                          ResourceManager::Get<raylib::Texture2D>(modelData.GetTexturePath()));

    // Draw the model
    DrawModelEx (
            ResourceManager::Get<raylib::Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3::Zero(),
            0.0f,
            dimensions,
            BLUE
    );

    // Draw the model wireframe
    DrawModelWiresEx (
            ResourceManager::Get<raylib::Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3::Zero(),
            0.0f,
            dimensions,
            DARKBLUE
    );
}

BoundingBox Geometry::GetBoundingBox()
{
    return BoundingBox {
            position - raylib::Vector3(dimensions.x, dimensions.y, dimensions.z),
            position + raylib::Vector3(dimensions.x, dimensions.y, dimensions.z),
    };
}

raylib::Vector3 Geometry::GetDimensions()
{
    return dimensions;
}

Entity* Geometry::Clone()
{
    return new Geometry(position, dimensions, modelData);
}

ModelData &Geometry::GetModelData() {
    return modelData;
}
