//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DEVCONSOLE_H
#define SIEGE_ENGINE_DEVCONSOLE_H

#include <core/entity/Entity.h>
#include <core/entity/EntityPtr.h>

#include "../ServiceLocator.h"
#include "MessageDisplay.h"

class DevConsole : public Siege::Entity
{
public:

    // 'Structors

    explicit DevConsole(bool isEditorMode) :
        isEditorMode(isEditorMode),
        isActive(false),
        messageDisplay(ServiceLocator::GetMessageDisplay()) {};

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw2D() override;

private:

    // Private methods

    bool CheckEditorMode();

    bool CheckArgs(const Siege::String& command, const Siege::String& args);

    // Private fields

    bool isEditorMode;

    bool isActive;

    Siege::String inputText;

    Siege::String lastInput;

    Siege::EntityPtr<MessageDisplay> messageDisplay;
};

#endif // SIEGE_ENGINE_DEVCONSOLE_H
