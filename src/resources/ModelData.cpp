#include "ModelData.h"

void ModelData::SetModel(const std::string& path, const std::string& name)
{
    modelPath = path;
    modelName = name;
}

void ModelData::SetTexture(const std::string& path, const std::string& name) {
    texturePath = path;
    textureName = name;
}

std::pair<std::string, std::string> ModelData::GetModelData()
{
    return {modelPath, modelName};
}

std::pair<std::string, std::string> ModelData::GetTextureData()
{
    return {texturePath, textureName};
}
