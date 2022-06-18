//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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

class DevConsole : public Entity
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

    bool CheckArgs(const String& command, const String& args);

    // Private fields

    bool isEditorMode;

    bool isActive;

    String inputText;

    String lastInput;

    EntityPtr<MessageDisplay> messageDisplay;
};

#endif // SIEGE_ENGINE_DEVCONSOLE_H
