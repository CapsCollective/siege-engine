#ifndef A_DARK_DISCOMFORT_MODELDATA_H
#define A_DARK_DISCOMFORT_MODELDATA_H

#include <raylib-cpp.hpp>
#include "ResourceManager.h"

class ModelData {
public:

    // Constructors
    ModelData(std::string  modelPath, std::string  texturePath) :
    modelPath(std::move(modelPath)), texturePath(std::move(texturePath))
    {
        ResourceManager::Register<Model>(modelPath);
        ResourceManager::Register<Texture>(texturePath);
    };

    ModelData(const ModelData& other) {
        this->modelPath = other.modelPath;
        this->texturePath = other.texturePath;

        ResourceManager::Register<Model>(modelPath);
        ResourceManager::Register<Texture>(texturePath);
    }

    // Public methods

    const std::string& GetModelPath() const;

    const std::string& GetTexturePath() const;

    static void SetTexture(Model&, Texture2D&);

private:

    // Private fields

    std::string modelPath;

    std::string texturePath;
};

#endif //A_DARK_DISCOMFORT_MODELDATA_H
