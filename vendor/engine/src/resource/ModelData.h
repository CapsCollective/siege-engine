#ifndef A_DARK_DISCOMFORT_MODELDATA_H
#define A_DARK_DISCOMFORT_MODELDATA_H

#include <raylib/raylib-cpp.hpp>
#include "ResourceManager.h"

class ModelData
{
public:

    // 'Structors

    ModelData(std::string  modelPath, std::string  texturePath) :
    modelPath(std::move(modelPath)),
    texturePath(std::move(texturePath))
    {
        ResourceManager::RegisterModel(this->modelPath);
        ResourceManager::RegisterTexture(this->texturePath);
    };

    ModelData(const ModelData& other)
    {
        this->modelPath = other.modelPath;
        this->texturePath = other.texturePath;

        ResourceManager::RegisterModel(modelPath);
        ResourceManager::RegisterTexture(texturePath);
    }

    // Public methods

    const std::string& GetModelPath() const;

    const std::string& GetTexturePath() const;

    static void SetTexture(const Model& model, const Texture& texture);

private:

    // Private fields

    std::string modelPath;

    std::string texturePath;
};

#endif //A_DARK_DISCOMFORT_MODELDATA_H
