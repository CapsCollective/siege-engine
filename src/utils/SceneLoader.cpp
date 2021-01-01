#include "../resources/ResourceManager.h"
#include "../entity_system/EntityStorage.h"
#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include "StringHelpers.h"
#include "SceneLoader.h"
#include <iostream>
#include <fstream>
#include <vector>

// Define macros
#define SEP ';'
#define UNKNOWN_FILENAME "untitled"
#define SCENE_DIR "assets/scenes/"
#define SCENE_FILE_EXT ".scene"

// Define static members
std::string SceneLoader::currentScene;
std::string SceneLoader::nextScene;

void SceneLoader::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene = UNKNOWN_FILENAME;
}

void SceneLoader::SaveScene()
{
    // Save the scene as the current scene or untitled
    SaveScene(currentScene.empty() ? UNKNOWN_FILENAME : currentScene);
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

        // TODO add labels to serialised fields

        // Add its name, position and rotation to the data
        fileData += (entity->GetName() + SEP +
                StringHelpers::VectorToString(entity->GetPosition()) + SEP +
                std::to_string(entity->GetRotation()) + SEP);

        // Add any additional fields needed to the data
        if (entity->GetName() == "Geometry")
        {
            auto geometry = dynamic_cast<Geometry*>(entity);
            fileData += StringHelpers::VectorToString(dynamic_cast<Geometry *>(entity)->GetDimensions()) + ";";
            fileData += geometry->GetModelData().GetModelPath() + SEP;
            fileData += geometry->GetModelData().GetTexturePath() + SEP;
        }
        else if (entity->GetName() == "Player")
        {
            auto player = dynamic_cast<Player*>(entity);
            fileData += player->GetModelData().GetModelPath() + SEP;
            fileData += player->GetModelData().GetTexturePath() + SEP;
        }

        // Add new line as entity delimiter
        fileData += "\n";
    }

    // Open a new file stream, dave the data to it and close it
    std::ofstream fileStream(SCENE_DIR + sceneName + SCENE_FILE_EXT);
    fileStream << fileData;
    fileStream.close();
}

bool SceneLoader::DeserialiseScene(const std::string& sceneName)
{
    // Begin the loading process, open the file for streaming
    std::ifstream file(SCENE_DIR + sceneName + SCENE_FILE_EXT);
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

        // Get the position and rotation of the entity
        raylib::Vector3 position = StringHelpers::StringToVector(args[ENTITY_POS]);
        float rotation = std::stof(args[ENTITY_ROT]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            raylib::Vector3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);

            std::string modelPath = args[CUSTOM_FIELD_2];
            std::string texturePath = args[CUSTOM_FIELD_3];

            EntityStorage::Register(new Geometry(position, rotation, dimensions, ModelData(modelPath, texturePath)));
        }
        else if (args[ENTITY_NAME] == "Player")
        {
            std::string modelPath = args[CUSTOM_FIELD_1];
            std::string texturePath = args[CUSTOM_FIELD_2];

            EntityStorage::Register(new Player(position, rotation));
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

    ResourceManager::ClearResources();
}

bool SceneLoader::QueueNextScene(const std::string &sceneName) {
    // Free all current items from storage
    ClearScene();
    nextScene = sceneName;
    return true;
}

void SceneLoader::LoadNextScene() {

    if (!nextScene.empty()) {
        if (LoadScene(nextScene)) {
            std::cout << "Successfully loaded scene: " << nextScene << std::endl;
        }
        nextScene.clear();
    }
}

