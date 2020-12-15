//
// Created by Jonathan Moallem on 15/12/20.
//

#ifndef A_DARK_DISCOMFORT_SCENELOADER_H
#define A_DARK_DISCOMFORT_SCENELOADER_H

#include <string>
#include <Vector3.hpp>

class SceneLoader
{
public:

    // Public methods

    static void SaveScene();

    static void LoadScene(const std::string& sceneName);

private:

    static std::string VectorToString(raylib::Vector3 vector) ;
};


#endif //A_DARK_DISCOMFORT_SCENELOADER_H
