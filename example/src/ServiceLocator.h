#ifndef A_DARK_DISCOMFORT_SERVICELOCATOR_H
#define A_DARK_DISCOMFORT_SERVICELOCATOR_H

#include <engine/utils/ServiceLocatorUtils.h>

class ServiceLocator
{
public:

    REGISTER_SERVICE(Camera, class Cam)

    REGISTER_SERVICE(EditorController, class EditorController)

    REGISTER_SERVICE(MessageDisplay, class MessageDisplay)

    REGISTER_SERVICE(Window, class Window)
};

#endif // A_DARK_DISCOMFORT_SERVICELOCATOR_H
