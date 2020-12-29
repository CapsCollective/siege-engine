#ifndef A_DARK_DISCOMFORT_MODELDATA_H
#define A_DARK_DISCOMFORT_MODELDATA_H

#include <raylib-cpp.hpp>
#include <string>

class ModelData {
public:

    // Constructors

    ModelData(std::string  modelPath, std::string  texturePath) :
    modelPath(std::move(modelPath)),
    texturePath(std::move(texturePath))
    {};

    ModelData() = default;

    ModelData(const ModelData& other) {
        this->modelPath = other.modelPath;
        this->texturePath = other.texturePath;
    }

    // Public methods

    void SetModelPath(const std::string&);

    void SetTexturePath(const std::string&);

    const std::string& GetModelPath() const;

    const std::string& GetTexturePath() const;

    static void SetTexture(raylib::Model&, raylib::Texture2D&);

private:

    // Private fields

    std::string modelPath;

    std::string texturePath;
};


#endif //A_DARK_DISCOMFORT_MODELDATA_H
