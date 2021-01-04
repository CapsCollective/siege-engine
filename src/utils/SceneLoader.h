#ifndef A_DARK_DISCOMFORT_SCENELOADER_H
#define A_DARK_DISCOMFORT_SCENELOADER_H

class SceneLoader
{
public:

    // Public methods

    /**
     * Closes the current scene and starts a new, empty scene
     */
    static void NewScene();

    /**
     * Saves the currently loaded scene by its name, or "untitled"
     * if the scene is not already saved
     */
    static void SaveScene();

    /**
     * Saves the currently loaded scene by the name supplied
     * @param sceneName - the name to save the scene as
     */
    static void SaveScene(const std::string& sceneName);

    /**
     * Safely prepares the current scene for loading of a newly
     * specified scene
     * @param sceneName - the name of the new scene to load
     */
    static void QueueNextScene(const std::string& sceneName);

    /**
     * Loads a newly queued scene if one is set, should be called
     * at the end of the frame
     */
    static void LoadNextScene();

private:

    // Private methods

    /**
     * Serialise the current scene to a file by name
     * @param sceneName - the name of the file to serialise to
     */
    static void SerialiseScene(const std::string& sceneName);

    /**
     * Deserialise a specified scene from a file by name
     * @param sceneName - the name of the file to deserialise
     * @return Success status of the operation as a bool
     */
    static bool DeserialiseScene(const std::string& sceneName);

    /**
     * Clears all entities in the scene (and related resources)
     */
    static void ClearScene();

    // Private fields

    /**
     * The name of the currently loaded scene
     */
    static std::string currentScene;

    /**
     * The name of the next scene to be loaded (if any)
     */
    static std::string nextScene;
};


#endif //A_DARK_DISCOMFORT_SCENELOADER_H
