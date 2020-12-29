//
// Created by Jonathan Moallem on 29/12/20.
//

#ifndef A_DARK_DISCOMFORT_SERVICELOCATOR_H
#define A_DARK_DISCOMFORT_SERVICELOCATOR_H

#include <Camera3D.hpp>
#include "../entities/tools/MessageDisplay.h"

class ServiceLocator
{
public:

    // Camera functions

    static raylib::Camera* GetCamera()
    {
        return camera;
    }

    static void Provide(raylib::Camera* cam)
    {
        camera = cam;
    }

    // MessageDisplay functions

    static MessageDisplay* GetMessageDisplay()
    {
        return messageDisplay;
    }

    static void Provide(MessageDisplay* display)
    {
        messageDisplay = display;
    }

private:

    // Private fields

    static raylib::Camera* camera;

    static MessageDisplay* messageDisplay;
};


#endif //A_DARK_DISCOMFORT_SERVICELOCATOR_H
