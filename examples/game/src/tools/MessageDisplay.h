//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MESSAGEDISPLAY_H
#define SIEGE_ENGINE_MESSAGEDISPLAY_H

#include <core/entity/Entity.h>

class MessageDisplay : public Siege::Entity
{
public:

    // 'Structors

    MessageDisplay() : displayTime(0.f) {};

    // Public methods

    void DisplayMessage(const Siege::String& message, float time = 10.f);

protected:

    // Protected overrides

    void OnDraw2D() override;

private:

    // Private fields

    float displayTime;

    Siege::String displayMessage;
};

#endif // SIEGE_ENGINE_MESSAGEDISPLAY_H
