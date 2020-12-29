#ifndef A_DARK_DISCOMFORT_SCENELOADER_H
#define A_DARK_DISCOMFORT_SCENELOADER_H

class SceneLoader
{
public:

    // Public methods

    static void NewScene();

    static void SaveScene();

    static void SaveScene(const std::string& sceneName);

    static bool LoadScene(const std::string& sceneName);

private:

    // Private methods

    static void SerialiseScene(const std::string& sceneName);

    static bool DeserialiseScene(const std::string& sceneName);

    static void ClearScene();

    // Private fields

    static std::string currentScene;
};


#endif //A_DARK_DISCOMFORT_SCENELOADER_H
