#include "../../entities/tools/MessageDisplay.h"
#include "../../systems/resources/ResourceManager.h"
#include "../../systems/entities/EntityStorage.h"
#include "../../utils/ServiceLocator.h"
#include "SceneSerialiser.h"
#include "SceneManager.h"
#include <iostream>
#include <fstream>
#include <vector>

// Define macros
#define OUT
#define UNKNOWN_FILENAME "untitled"
#define SCENE_PATH(sceneName) "assets/scenes/" + sceneName + ".scene"

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
    std::ifstream file(SCENE_PATH(nextScene));

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
        for (auto entity : entities) EntityStorage::Register(entity);

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
    SaveScene(currentScene.empty() ? "" : currentScene);
}

void SceneManager::SaveScene(const std::string& sceneName)
{
    // Get and set the current scene name
    currentScene = sceneName.empty() ? UNKNOWN_FILENAME : sceneName;

    // Open a new file stream, serialise the data to it and close it
    std::ofstream fileStream(SCENE_PATH(currentScene));
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

