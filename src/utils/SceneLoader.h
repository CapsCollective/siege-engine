#ifndef A_DARK_DISCOMFORT_SCENELOADER_H
#define A_DARK_DISCOMFORT_SCENELOADER_H

// TODO comment API functions
class SceneLoader
{
public:

    // Public methods

    static void NewScene();

    static void SaveScene();

    static void SaveScene(const std::string& sceneName);

    static void QueueNextScene(const std::string& sceneName);

    static void LoadNextScene();

private:

    // Private methods

    static void SerialiseScene(const std::string& sceneName);

    static bool DeserialiseScene(const std::string& sceneName);

    static void ClearScene();

    // Private fields

    static std::string currentScene;

    static std::string nextScene;
};


#endif //A_DARK_DISCOMFORT_SCENELOADER_H
