//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SCENESYSTEM_H
#define SIEGE_ENGINE_SCENESYSTEM_H

#include <utils/String.h>

#include "SceneFile.h"

namespace Siege
{
class SceneSystem
{
public:

    // Public methods

    /**
     * Scene system constructor method
     */
    SceneSystem() : nextSceneName(), currentScene(nextSceneName) {};

    /**
     * Closes the current scene and starts a new, empty scene
     */
    void NewScene();

    /**
     * Saves the currently loaded scene by its name, or "untitled"
     * if the scene is not already saved
     */
    void SaveScene();

    /**
     * Saves the currently loaded scene by the name supplied
     * @param sceneName - the name to save the scene as
     */
    void SaveScene(const String& sceneName);

    /**
     * Safely prepares the current scene for loading of a newly
     * specified scene
     * @param sceneName - the name of the new scene to load
     */
    void QueueNextScene(const String& sceneName);

    /**
     * Loads a newly queued scene if one is set, should be called
     * at the end of the frame
     */
    void LoadNextScene();

private:

    // Private methods

    /**
     * Clears all entities in the scene (and related resources)
     */
    void ClearScene();

    // Private fields

    /**
     * The name of the next scene to be loaded (if any)
     */
    String nextSceneName;

    /**
     * The currently loaded scene file
     */
    SceneFile currentScene;
};
} // namespace Siege

#endif // SIEGE_ENGINE_SCENESYSTEM_H
