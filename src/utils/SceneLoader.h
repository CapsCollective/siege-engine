//
// Created by Jonathan Moallem on 15/12/20.
//

#ifndef A_DARK_DISCOMFORT_SCENELOADER_H
#define A_DARK_DISCOMFORT_SCENELOADER_H

#include <string>
#include <Vector3.hpp>
#include <vector>

class SceneLoader
{
public:

    // Public methods

    static void SaveScene(const std::string& sceneName);

    static void LoadScene(const std::string& sceneName);

private:

    // Private methods

    static std::string VectorToString(raylib::Vector3 vector);

    static raylib::Vector3 StringToVector(std::string string);

    static std::vector<std::string> SplitString(std::string string, char delimiter);
};


#endif //A_DARK_DISCOMFORT_SCENELOADER_H
