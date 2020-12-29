#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include "../Entity.h"
#include "MessageDisplay.h"
#include "../../utils/ServiceLocator.h"

class DevConsole : public Entity
{
public:

    // Constructors

    explicit DevConsole() :
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    isActive(false),
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

    void OnToolUpdate() override;

    void OnUIDraw() override;

private:

    // Private fields

    bool isActive;

    std::string currentScene;

    std::string inputText;

    std::string lastInput;

    MessageDisplay* messageDisplay;
};

#endif //A_DARK_DISCOMFORT_DEVCONSOLE_H
