#ifndef A_DARK_DISCOMFORT_SERVICELOCATOR_H
#define A_DARK_DISCOMFORT_SERVICELOCATOR_H

#include <Camera3D.hpp>
#include <utils/ServiceLocatorUtils.h>

class ServiceLocator
{
public:

    REGISTER_SERVICE(Camera, raylib::Camera3D)

    REGISTER_SERVICE(EditorController, class EditorController)

    REGISTER_SERVICE(MessageDisplay, class MessageDisplay)

    REGISTER_SERVICE(Profiler, class Profiler)

};

#endif //A_DARK_DISCOMFORT_SERVICELOCATOR_H
