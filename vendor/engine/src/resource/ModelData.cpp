#include "ModelData.h"

const std::string& ModelData::GetModelPath() const
{
    return modelPath;
}

const std::string& ModelData::GetTexturePath() const
{
    return texturePath;
}

void ModelData::SetTexture(const Model& model, const Texture& texture)
{
    // Set model texture using raylib
    model.materials[0].maps[MAP_DIFFUSE].texture = texture;
}


