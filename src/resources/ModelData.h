#ifndef A_DARK_DISCOMFORT_MODELDATA_H
#define A_DARK_DISCOMFORT_MODELDATA_H

#include <raylib-cpp.hpp>
#include <string>

class ModelData {

public:

    explicit ModelData(const std::string& modelPath, const std::string& modelName, const std::string& texturePath,
              const std::string& textureName) :
              modelPath(modelPath),
              modelName(modelName),
              texturePath(texturePath),
              textureName(textureName) {};

    ~ModelData() = default;

    void SetModel(const std::string&, const std::string&);
    void SetTexture(const std::string&, const std::string&);

    std::pair<std::string, std::string> GetModelData();
    std::pair<std::string, std::string> GetTextureData();

private:

    // Model Metadata
    std::string modelPath;
    std::string modelName;

    // Texture Metadata
    std::string texturePath;
    std::string textureName;

    raylib::Model* model;
    raylib::Texture2D* texture;
};


#endif //A_DARK_DISCOMFORT_MODELDATA_H
