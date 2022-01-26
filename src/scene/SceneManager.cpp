#include "SceneManager.h"

#include <vector>

#include "../resource/ResourceManager.h"
#include "../utils/Logging.h"
#include "SceneFile.h"

// Define constants
static constexpr const char UNKNOWN_FILENAME[] = "untitled";

// Define static members
String SceneManager::currentScene;
String SceneManager::nextScene;

void SceneManager::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene = UNKNOWN_FILENAME;
}

void SceneManager::QueueNextScene(const String& sceneName)
{
    // Free all current items from storage
    ClearScene();
    nextScene = sceneName;
}

void SceneManager::LoadNextScene()
{
    if (nextScene.IsEmpty()) return;

    // Deserialise and register all entities to current scene
    std::vector<Entity*> entities;
    SceneFile file(nextScene);
    if (file.Deserialise(entities))
    {
        for (auto& entity : entities) EntityStorage::Add(entity);

        // Set the current scene details
        currentScene = nextScene;
        CC_LOG_INFO("Successfully loaded {}.scene", nextScene);
    }
    else CC_LOG_WARNING("Unable to load \"{}.scene\"", nextScene);

    // Clear next scene
    nextScene.Clear();
}

void SceneManager::SaveScene()
{
    // Save the scene as the current scene or untitled
    SaveScene(currentScene.IsEmpty() ? String() : currentScene);
}

void SceneManager::SaveScene(const String& sceneName)
{
    // Get and set the current scene name
    currentScene = sceneName.IsEmpty() ? UNKNOWN_FILENAME : sceneName;

    // Serialise the data to it and close it
    SceneFile file(currentScene);
    if (file.Serialise(EntityStorage::GetEntities())) return;
    CC_LOG_WARNING("Unable to save \"{}.scene\"", sceneName);
}

void SceneManager::ClearScene()
{
    // Free all current entities from storage
    for (auto& entity : EntityStorage::GetEntities()) entity->QueueFree();

    // Clear out all resources
    ResourceManager::ClearResources();
}
