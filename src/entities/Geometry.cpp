#include "Geometry.h"

void Geometry::OnDraw()
{
    // Set the model's texture to this entity's texture
    ModelData::SetTexture(ResourceManager::Get<Model>(modelData.GetModelPath()),
                          ResourceManager::Get<Texture2D>(modelData.GetTexturePath()));

    // Draw the model
    DrawModelEx(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3(0, 1, 0),
            rotation,
            dimensions,
            BLUE
    );

    // Draw the model wireframe
    DrawModelWiresEx(
            ResourceManager::Get<Model>(modelData.GetModelPath()),
            position,
            raylib::Vector3(0, 1, 0),
            rotation,
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
    return new Geometry(position, rotation, dimensions, modelData);
}

ModelData& Geometry::GetModelData() {
    return modelData;
}
