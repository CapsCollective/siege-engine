#ifndef SIEGE_ENGINE_SERVICELOCATOR_H
#define SIEGE_ENGINE_SERVICELOCATOR_H

#include <utils/ServiceLocatorUtils.h>

class ServiceLocator
{
public:

    REGISTER_SERVICE(Camera, class Cam)

    REGISTER_SERVICE(EditorController, class EditorController)

    REGISTER_SERVICE(MessageDisplay, class MessageDisplay)

    REGISTER_SERVICE(Window, class Window)
};

#endif // SIEGE_ENGINE_SERVICELOCATOR_H
