#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include <entity/Entity.h>
#include <entity/EntityPtr.h>
#include "../ServiceLocator.h"

class DevConsole : public Entity
{
public:

    // 'Structors

    explicit DevConsole(bool isEditorMode) :
        isEditorMode(isEditorMode),
        isActive(false),
        messageDisplay(ServiceLocator::GetMessageDisplay())
    {};

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw2D() override;

private:

    // Private methods

    bool CheckEditorMode();

    bool CheckArgs(const std::string& command, const std::string& args);

    // Private fields

    bool isEditorMode;

    bool isActive;

    std::string inputText;

    std::string lastInput;

    EntityPtr<MessageDisplay> messageDisplay;

};

#endif //A_DARK_DISCOMFORT_DEVCONSOLE_H
