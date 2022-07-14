//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SERVICELOCATOR_H
#define SIEGE_ENGINE_SERVICELOCATOR_H

#include <core/render/Camera.h>
#include <core/render/Window.h>
#include <utils/ServiceLocatorUtils.h>

class ServiceLocator
{
public:

    REGISTER_SERVICE(Camera, class Siege::Cam)

    REGISTER_SERVICE(EditorController, class EditorController)

    REGISTER_SERVICE(MessageDisplay, class MessageDisplay)

    REGISTER_SERVICE(Window, class Siege::Window)
};

#endif // SIEGE_ENGINE_SERVICELOCATOR_H
