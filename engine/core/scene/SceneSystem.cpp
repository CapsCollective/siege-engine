//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SceneSystem.h"

#include <resources/ResourceSystem.h>
#include <utils/Logging.h>

#include <vector>

#include "SceneFile.h"

namespace Siege
{
// Define constants
static constexpr const char UNKNOWN_FILENAME[] = "untitled";

String SceneSystem::baseDir;

void SceneSystem::NewScene()
{
    // Clear the current scene and reset current scene
    ClearScene();
    currentScene.SetSceneName(baseDir + UNKNOWN_FILENAME);
}

void SceneSystem::QueueNextScene(const String& sceneName)
{
    // Free all current items from storage
    ClearScene();
    nextSceneName = sceneName;
}

void SceneSystem::LoadNextScene()
{
    if (nextSceneName.IsEmpty()) return;

    // Deserialise and register all entities to current scene
    std::vector<Entity*> entities;
    currentScene.SetSceneName(baseDir + nextSceneName);
    if (currentScene.Deserialise(entities))
    {
        for (auto& entity : entities) Statics::Entity().Add(entity);
        currentScene.InitialiseEntityPathMappings();

        CC_LOG_INFO("Successfully loaded {}.scene", nextSceneName);
    }
    else CC_LOG_WARNING("Unable to load \"{}.scene\"", nextSceneName);

    // Clear next scene
    nextSceneName.Clear();
}

void SceneSystem::SaveScene()
{
    // Save the scene as the current scene or untitled
    if (currentScene.GetSceneName().IsEmpty())
        currentScene.SetSceneName(baseDir + UNKNOWN_FILENAME);

    // Serialise the data to it and close it
    if (currentScene.Serialise(Statics::Entity().GetEntities())) return;
    CC_LOG_WARNING("Unable to save \"{}.scene\"", currentScene.GetSceneName());
}

void SceneSystem::SaveScene(const String& sceneName)
{
    if (sceneName.IsEmpty()) currentScene.SetSceneName(baseDir + UNKNOWN_FILENAME);
    else currentScene.SetSceneName(baseDir + sceneName);
    SaveScene();
}

void SceneSystem::ClearScene()
{
    // Free all current entities from storage
    for (auto& entity : Statics::Entity().GetEntities()) entity->QueueFree();
}

void SceneSystem::SetBaseDirectory(const String& dir)
{
    baseDir = dir;
}

const String& SceneSystem::GetBaseDirectory()
{
    return baseDir;
}

} // namespace Siege