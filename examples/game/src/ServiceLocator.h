//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SERVICELOCATOR_H
#define SIEGE_ENGINE_SERVICELOCATOR_H

#include <core/Ticker.h>
#include <render/renderer/Renderer.h>
#include <utils/ServiceLocatorUtils.h>
#include <window/Window.h>

#include "RenderResources.h"
#include "entities/FPSCamera.h"

class ServiceLocator
{
public:

    REGISTER_SERVICE(Renderer, class Siege::Renderer)

    REGISTER_SERVICE(FPSCamera, class FPSCamera)

    REGISTER_SERVICE(EditorController, class EditorController)

    REGISTER_SERVICE(MessageDisplay, class MessageDisplay)

    REGISTER_SERVICE(Window, class Siege::Window)

    REGISTER_SERVICE(Ticker, class Siege::Ticker)

    REGISTER_SERVICE(RenderResources, class RenderResources)
};

#endif // SIEGE_ENGINE_SERVICELOCATOR_H
