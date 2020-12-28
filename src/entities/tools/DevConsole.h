#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include "../Entity.h"
#include "MessageDisplay.h"

class DevConsole : public Entity
{
public:

    // Constructors

    explicit DevConsole(bool isEditorMode, MessageDisplay* display) :
    messageDisplay(display),
    isActive(false),
    isEditorMode(isEditorMode),
    currentScene("main")
    {
        Entity::SetName("DevConsole");
        Entity::SetFreeable(false);
        Entity::SetSerialisable(false);
    };

    // Public methods

    bool IsActive() const;

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnUIDraw() override;

private:

    // Private fields

    bool isEditorMode;

    bool isActive;

    std::string currentScene;

    std::string inputText;

    std::string lastInput;

    MessageDisplay* messageDisplay;
};

#endif //A_DARK_DISCOMFORT_DEVCONSOLE_H
