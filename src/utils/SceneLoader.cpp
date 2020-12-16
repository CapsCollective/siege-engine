#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "SceneLoader.h"
#include "../entity_system/EntityStorage.h"
#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include "HelperFuncs.h"

void SceneLoader::SaveScene(const std::string& sceneName)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : EntityStorage::GetEntities())
    {
        // Check if the entity is serialisable
        if (!entity->IsSerialisable()) continue;

        // Add its name and position to the data
        fileData += (entity->GetName() + ";" + HelperFuncs::VectorToString(entity->GetPosition()) + ";");


        // Add any additional fields needed to the data
        if (entity->GetName() == "Geometry")
        {
            fileData += HelperFuncs::VectorToString(dynamic_cast<Geometry *>(entity)->GetDimensions()) + ";";
        }

        // Add new line as entity delimiter
        fileData += "\n";
    }

    // Open a new file stream, dave the data to it and close it
    std::ofstream fileStream("./assets/scenes/" + sceneName + ".scene");
    fileStream << fileData;
    fileStream.close();
}

bool SceneLoader::LoadScene(const std::string& sceneName)
{
    // Begin the loading process, open the file for streaming
    std::ifstream file("./assets/scenes/" + sceneName + ".scene");
    if (!file.is_open()) return false;

    // Free all current entities from storage
    for (auto entity : EntityStorage::GetEntities())
    {
        // Need to queue here since we can't modify a list while using it
        entity->QueueFree();
    }

    // Iterate over each line of the file
    std::string line;
    while (getline(file, line))
    {
        // Define default field values
        enum {
            ENTITY_NAME = 0,
            ENTITY_POS = 1,
            CUSTOM_FIELD_1 = 2,
            CUSTOM_FIELD_2 = 3,
            CUSTOM_FIELD_3 = 4,
        };

        // Split the line into arguments
        std::vector<std::string> args = HelperFuncs::SplitString(line, ';');

        // Calculate the position of the entity
        raylib::Vector3 position = HelperFuncs::StringToVector(args[ENTITY_POS]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            raylib::Vector3 dimensions = HelperFuncs::StringToVector(args[CUSTOM_FIELD_1]);
            EntityStorage::Register(new Geometry(position, dimensions));
        }
        else if (args[ENTITY_NAME] == "Player")
        {
            EntityStorage::Register(new Player(position));
        }
        else
        {
            // Notify console of default case
            std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
        }
    }

    // Close the file stream
    file.close();
    return true;
}

