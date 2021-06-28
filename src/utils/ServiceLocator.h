#ifndef A_DARK_DISCOMFORT_SERVICELOCATOR_H
#define A_DARK_DISCOMFORT_SERVICELOCATOR_H

#include <Camera3D.hpp>
#include "../utils/Macros.h"

#define MAKE_GETTER(name) CONCAT_SYMBOL(Get, name)
#define REGISTER_SERVICE(name, type) \
    static type*& MAKE_GETTER(name)() \
    { \
        static type* service; \
        return service; \
    } \
    \
    static void Provide(type* service) \
    { \
        MAKE_GETTER(name)() = service; \
    }

class ServiceLocator
{
public:

    REGISTER_SERVICE(Camera, raylib::Camera)

    REGISTER_SERVICE(EditorController, class EditorController)

    REGISTER_SERVICE(MessageDisplay, class MessageDisplay)

    REGISTER_SERVICE(Profiler, class Profiler)
};

#endif //A_DARK_DISCOMFORT_SERVICELOCATOR_H
