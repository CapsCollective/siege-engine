#ifndef A_DARK_DISCOMFORT_MODELDATA_H
#define A_DARK_DISCOMFORT_MODELDATA_H

#include <raylib-cpp.hpp>
#include <string>

class ModelData {

public:

    ModelData(std::string  modelPath, std::string  texturePath) :
    modelPath(std::move(modelPath)), texturePath(std::move(texturePath))
    {};

    ModelData() :
    modelPath(""), texturePath("")
    {};

    ModelData(const ModelData& other) {
        this->modelPath = other.modelPath;
        this->texturePath = other.texturePath;
    }

    ~ModelData() = default;

    void SetModelPath(const std::string&);
    void SetTexturePath(const std::string&);

    const std::string& GetModelPath() const;
    const std::string& GetTexturePath() const;

    static void SetTexture(raylib::Model&, raylib::Texture2D&);

private:
    // Model Metadata
    std::string modelPath;

    // Texture Metadata
    std::string texturePath;
};


#endif //A_DARK_DISCOMFORT_MODELDATA_H
