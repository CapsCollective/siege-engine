#include "ModelData.h"

void ModelData::SetModelPath(const std::string& path)
{
    modelPath = path;
}

void ModelData::SetTexturePath(const std::string& path)
{
    texturePath = path;
}

const std::string& ModelData::GetModelPath() const
{
    return modelPath;
}

const std::string& ModelData::GetTexturePath() const
{
    return texturePath;
}

void ModelData::SetTexture(raylib::Model& model, raylib::Texture2D& texture) {
    // Set model texture using raylib
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;
}


