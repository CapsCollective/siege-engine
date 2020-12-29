#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include "../Entity.h"
#include "MessageDisplay.h"
#include "../../utils/ServiceLocator.h"

class DevConsole : public Entity
{
public:

    // Constructors

    explicit DevConsole(bool isEditorMode) :
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    isEditorMode(isEditorMode),
    isActive(false)
    {
        Entity::SetName("DevConsole");
        Entity::SetFreeable(false);
        Entity::SetSerialisable(false);
    };

protected:

    // Protected overrides

    void OnToolUpdate() override;

    void OnUIDraw() override;

private:

    // Private fields

    bool isEditorMode;

    bool isActive;

    std::string inputText;

    std::string lastInput;

    MessageDisplay* messageDisplay;
};

#endif //A_DARK_DISCOMFORT_DEVCONSOLE_H
