#include "../../entities/tools/MessageDisplay.h"
#include "../../systems/resource/ResourceManager.h"
#include "../../systems/entity/EntityStorage.h"
#include "../../utils/ServiceLocator.h"
#include "SceneSerialiser.h"
#include "SceneManager.h"
#include <iostream>
#include <fstream>
#include <vector>

// Define constants
static constexpr const char UNKNOWN_FILENAME[] = "untitled";

// Define static members
std::string SceneManager::currentScene;
std::string SceneManager::nextScene;

void SceneManager::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene = UNKNOWN_FILENAME;
}

void SceneManager::QueueNextScene(const std::string &sceneName)
{
    // Free all current items from storage
    ClearScene();
    nextScene = sceneName;
}

void SceneManager::LoadNextScene()
{
    if (nextScene.empty()) return;
    MessageDisplay* messageDisplay = ServiceLocator::GetMessageDisplay();

    // Try open the next scene file for streaming
    std::ifstream file(MakeScenePath(nextScene));

    // Exit if next scene is invalid
    if (file.is_open())
    {
        // Get the scene file's lines
        std::string line;
        std::vector<std::string> sceneLines;
        while (getline(file, line)) sceneLines.push_back(line);

        // Deserialise and register all entities to current scene
        std::vector<Entity*> entities;
        SceneSerialiser::Deserialise(sceneLines, entities);
        for (auto entity : entities) EntityStorage::Add(entity);

        // Set the current scene details
        currentScene = nextScene;
        messageDisplay->DisplayMessage("Successfully loaded " + nextScene + ".scene");
    }
    else messageDisplay->DisplayMessage("Unable to find \"" + nextScene +  ".scene\"");

    // Close the file stream and clear next scene
    file.close();
    nextScene.clear();
}

void SceneManager::SaveScene()
{
    // Save the scene as the current scene or untitled
    SaveScene(currentScene.empty() ? std::string() : currentScene);
}

void SceneManager::SaveScene(const std::string& sceneName)
{
    // Get and set the current scene name
    currentScene = sceneName.empty() ? UNKNOWN_FILENAME : sceneName;

    // Open a new file stream, serialise the data to it and close it
    std::ofstream fileStream(MakeScenePath(currentScene));
    fileStream << SceneSerialiser::Serialise(EntityStorage::GetEntities());
    fileStream.close();
}

void SceneManager::ClearScene()
{
    // Free all current entities from storage
    for (auto entity : EntityStorage::GetEntities()) entity->QueueFree();

    // Clear out all resources
    ResourceManager::ClearResources();
}

