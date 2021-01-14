#include "../entities/tools/MessageDisplay.h"
#include "../resources/ResourceManager.h"
#include "../entity_system/EntityStorage.h"
#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include "StringHelpers.h"
#include "SceneLoader.h"
#include "ServiceLocator.h"
#include <iostream>
#include <fstream>
#include <vector>

// Define macros
#define SEP '|'
#define UNKNOWN_FILENAME "untitled"
#define DEFINE_FIELD(name, content) name + std::string(":") + content + SEP
#define SCENE_PATH(sceneName) "assets/scenes/" + sceneName + ".scene"

// Define static members
std::string SceneLoader::currentScene;
std::string SceneLoader::nextScene;


void SceneLoader::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene = UNKNOWN_FILENAME;
}

void SceneLoader::QueueNextScene(const std::string &sceneName)
{
    // Free all current items from storage
    ClearScene();
    nextScene = sceneName;
}

void SceneLoader::LoadNextScene()
{
    if (nextScene.empty()) return;

    // Try deserialise the scene by name and set it as current if successful
    MessageDisplay* messageDisplay = ServiceLocator::GetMessageDisplay();
    if (DeserialiseScene(nextScene)) {
        currentScene = nextScene;
        messageDisplay->DisplayMessage("Successfully loaded " + nextScene + ".scene");
    }
    else messageDisplay->DisplayMessage("Unable to find \"" + nextScene +  ".scene\"");

    // Clear the next scene
    nextScene.clear();
}

void SceneLoader::SaveScene()
{
    // Save the scene as the current scene or untitled
    SaveScene(currentScene.empty() ? "" : currentScene);
}

void SceneLoader::SaveScene(const std::string& sceneName)
{
    std::string name = sceneName.empty() ? UNKNOWN_FILENAME : sceneName;
    // Serialise the scene by name and set it as current
    SerialiseScene(name);
    currentScene = name;
}

void SceneLoader::SerialiseScene(const std::string& sceneName)
{
    // TODO move serialisation and deserialisation logic into its own module with interface Serialisable
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : EntityStorage::GetEntities())
    {
        // Add its name, position and rotation to the data
        fileData += (entity->GetName() + SEP +
                DEFINE_FIELD("POSITION", StringHelpers::VectorToString(entity->GetPosition())) +
                DEFINE_FIELD("ROTATION", std::to_string(entity->GetRotation())));

        // Add any additional fields needed to the data
        if (entity->GetName() == "Geometry")
        {
            auto geometry = dynamic_cast<Geometry*>(entity);
            fileData += DEFINE_FIELD("DIMENSIONS", StringHelpers::VectorToString(geometry->GetDimensions()));
            fileData += DEFINE_FIELD("MODEL_PATH", geometry->GetModelData().GetModelPath());
            fileData += DEFINE_FIELD("TEXTURE_PATH", geometry->GetModelData().GetTexturePath());
        }
        else if (entity->GetName() == "Player")
        {
            auto player = dynamic_cast<Player*>(entity);
            fileData += DEFINE_FIELD("MODEL_PATH", player->GetModelData().GetModelPath());
            fileData += DEFINE_FIELD("TEXTURE_PATH", player->GetModelData().GetTexturePath());
        }

        // Add new line as entity delimiter
        fileData += "\n";
    }

    // Open a new file stream, dave the data to it and close it
    std::ofstream fileStream(SCENE_PATH(sceneName));
    fileStream << fileData;
    fileStream.close();
}

bool SceneLoader::DeserialiseScene(const std::string& sceneName)
{
    // Begin the loading process, open the file for streaming
    std::ifstream file(SCENE_PATH(sceneName));
    if (!file.is_open()) return false;

    // Iterate over each line of the file
    std::string line;
    while (getline(file, line))
    {
        // Define default field values
        enum {
            ENTITY_NAME = 0,
            ENTITY_POS = 1,
            ENTITY_ROT = 2,
            CUSTOM_FIELD_1 = 3,
            CUSTOM_FIELD_2 = 4,
            CUSTOM_FIELD_3 = 5,
        };

        // Split the line into arguments
        std::vector<std::string> args = StringHelpers::SplitString(line, SEP);

        // Strip labels from each item
        for (std::string& arg : args)
        {
            arg = arg.substr(arg.find(':') + 1, arg.size());
        }

        // Get the position and rotation of the entity
        raylib::Vector3 position = StringHelpers::StringToVector(args[ENTITY_POS]);
        float rotation = std::stof(args[ENTITY_ROT]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            // Get custom fields
            raylib::Vector3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);
            std::string modelPath = args[CUSTOM_FIELD_2];
            std::string texturePath = args[CUSTOM_FIELD_3];

            // Register the new entity
            EntityStorage::Register(new Geometry(position, rotation, dimensions, ModelData(modelPath, texturePath)));

            std::cout << texturePath << std::endl;
        }
        else if (args[ENTITY_NAME] == "Player")
        {
            // Register the new entity
            EntityStorage::Register(new Player(position, rotation));
        }
        else std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
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

    // Clear out all resources
    ResourceManager::ClearResources();
}

