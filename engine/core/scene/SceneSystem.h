#ifndef SIEGE_ENGINE_SCENESYSTEM_H
#define SIEGE_ENGINE_SCENESYSTEM_H

#include <utils/String.h>

class SceneSystem
{
public:

    // Public methods

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
     * The name of the currently loaded scene
     */
    String currentScene;

    /**
     * The name of the next scene to be loaded (if any)
     */
    String nextScene;
};

#endif // SIEGE_ENGINE_SCENESYSTEM_H
