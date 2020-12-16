//
// Created by Jonathan Moallem on 15/12/20.
//

#ifndef A_DARK_DISCOMFORT_SCENELOADER_H
#define A_DARK_DISCOMFORT_SCENELOADER_H

#include <string>

class SceneLoader
{
public:

    // Public methods

    static void SaveScene(const std::string& sceneName);

    static void LoadScene(const std::string& sceneName);
};


#endif //A_DARK_DISCOMFORT_SCENELOADER_H
