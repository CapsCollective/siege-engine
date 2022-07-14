//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SceneSystem.h"

#include <utils/Logging.h>

#include <vector>

#include "../Statics.h"
#include "../entity/Entity.h"
#include "../render/ResourceSystem.h"
#include "SceneFile.h"

namespace Siege
{
// Define constants
static constexpr const char UNKNOWN_FILENAME[] = "untitled";

void SceneSystem::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene = UNKNOWN_FILENAME;
}

void SceneSystem::QueueNextScene(const String& sceneName)
{
    // Free all current items from storage
    ClearScene();
    nextScene = sceneName;
}

void SceneSystem::LoadNextScene()
{
    if (nextScene.IsEmpty()) return;

    // Deserialise and register all entities to current scene
    std::vector<Entity*> entities;
    SceneFile file(nextScene);
    if (file.Deserialise(entities))
    {
        for (auto& entity : entities) Statics::Entity().Add(entity);

        // Set the current scene details
        currentScene = nextScene;
        CC_LOG_INFO("Successfully loaded {}.scene", nextScene);
    }
    else CC_LOG_WARNING("Unable to load \"{}.scene\"", nextScene);

    // Clear next scene
    nextScene.Clear();
}

void SceneSystem::SaveScene()
{
    // Save the scene as the current scene or untitled
    SaveScene(currentScene.IsEmpty() ? String() : currentScene);
}

void SceneSystem::SaveScene(const String& sceneName)
{
    // Get and set the current scene name
    currentScene = sceneName.IsEmpty() ? UNKNOWN_FILENAME : sceneName;

    // Serialise the data to it and close it
    SceneFile file(currentScene);
    if (file.Serialise(Statics::Entity().GetEntities())) return;
    CC_LOG_WARNING("Unable to save \"{}.scene\"", sceneName);
}

void SceneSystem::ClearScene()
{
    // Free all current entities from storage
    for (auto& entity : Statics::Entity().GetEntities()) entity->QueueFree();

    // Clear out all resources
    Statics::Resource().ClearResources();
}
}