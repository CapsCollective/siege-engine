//
// Created by Menchy Kintslinger on 15/12/20.
//

#include <iostream>
#include <fstream>
#include "SceneLoader.h"
#include "../entity_system/EntityStorage.h"
#include "../entities/Geometry.h"

void SceneLoader::SaveScene()
{
    std::string fileData;
    for (auto entity : EntityStorage::GetEntities())
    {
        fileData += (entity->GetName() + ";" + VectorToString(entity->GetPosition()));

        if (entity->GetName() == "Geometry")
        {
            fileData += ";" + VectorToString(dynamic_cast<Geometry*>(entity)->GetDimensions());
        }

        fileData += "\n";
    }

    std::ofstream fileStream("./assets/scenes/main.scene");
    fileStream << fileData;
    fileStream.close();
}

void SceneLoader::LoadScene(const std::string& sceneName)
{
    std::string fileData; // Get the data from the file

    std::cout << "Loading scene" << std::endl;

    // For each split at new line
        std::string entityName = ""; // split at [0]
        std::string entityPos = ""; // Spilt [1]
        raylib::Vector3 position = {}; // assign to vector3 string to float

        std::ifstream file("./assets/scenes/" + sceneName);
        std::string line;

        if (file.is_open()) {

            while (getline (file, line)) {
                std::cout << line << std::endl;
            }
        }

        file.close();

        if (entityName == "Geometry")
        {
            std::string entityDims = ""; // Spilt [3]
            raylib::Vector3 dimensions = {}; // assign to vector3 string to float
            EntityStorage::Instance()->Register(new Geometry(position, dimensions));
        }
        else
        {
            std::cout << "Entity \"" << entityName << "\" has no deserialisation protocols defined" << std::endl;
        }
}

std::string SceneLoader::VectorToString(raylib::Vector3 vector)
{
    return std::to_string(vector.x) + "," +
           std::to_string(vector.y) + "," +
           std::to_string(vector.z);
}

