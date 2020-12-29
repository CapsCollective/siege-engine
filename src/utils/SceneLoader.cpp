#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "SceneLoader.h"
#include "../entity_system/EntityStorage.h"
#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include "StringHelpers.h"

// Static members

std::string SceneLoader::currentScene;

void SceneLoader::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene = "untitled";
}

void SceneLoader::SaveScene()
{
    // Save the scene as the current scene or untitled
    SaveScene(currentScene.empty() ? "untitled" : currentScene);
}

void SceneLoader::SaveScene(const std::string& sceneName)
{
    // Serialise the scene by name and set it as current
    SerialiseScene(sceneName);
    currentScene = sceneName;
}

bool SceneLoader::LoadScene(const std::string& sceneName)
{
    // Try deserialise the scene by name and set it as current if successful
    bool result = DeserialiseScene(sceneName);
    if (result) currentScene = sceneName;
    return result;
}

void SceneLoader::SerialiseScene(const std::string& sceneName)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : EntityStorage::GetEntities())
    {
        // Check if the entity is serialisable
        if (!entity->IsSerialisable()) continue;

        // Add its name and position to the data
        fileData += (entity->GetName() + ";" + StringHelpers::VectorToString(entity->GetPosition()) + ";");


        // Add any additional fields needed to the data
        if (entity->GetName() == "Geometry")
        {
            fileData += StringHelpers::VectorToString(dynamic_cast<Geometry *>(entity)->GetDimensions()) + ";";
        }

        // Add new line as entity delimiter
        fileData += "\n";
    }

    // Open a new file stream, dave the data to it and close it
    std::ofstream fileStream("./assets/scenes/" + sceneName + ".scene");
    fileStream << fileData;
    fileStream.close();
}

bool SceneLoader::DeserialiseScene(const std::string& sceneName)
{
    // Begin the loading process, open the file for streaming
    std::ifstream file("./assets/scenes/" + sceneName + ".scene");
    if (!file.is_open()) return false;

    // Free all current items from storage
    ClearScene();

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
        std::vector<std::string> args = StringHelpers::SplitString(line, ';');

        // Calculate the position of the entity
        raylib::Vector3 position = StringHelpers::StringToVector(args[ENTITY_POS]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            raylib::Vector3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);
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

void SceneLoader::ClearScene()
{
    // Free all current entities from storage
    for (auto entity : EntityStorage::GetEntities())
    {
        // Need to queue here since we can't modify a list while using it
        entity->QueueFree();
    }
}

