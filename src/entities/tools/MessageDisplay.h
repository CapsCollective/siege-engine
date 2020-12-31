#ifndef A_DARK_DISCOMFORT_MESSAGEDISPLAY_H
#define A_DARK_DISCOMFORT_MESSAGEDISPLAY_H

#include "../Entity.h"

class MessageDisplay : public Entity
{
public:

    // Constructors

    explicit MessageDisplay() :
    displayTime(0.f)
    {
        Entity::SetName("MessageDisplay");
        Entity::SetFreeable(false);
        Entity::SetSerialisable(false);
        Entity::SetIsTool(true);
    };

    // Public methods

    void DisplayMessage(const std::string& message, float time = 10.f);

protected:

    // Protected overrides

    void OnUIDraw() override;

private:

    // Private fields

    float displayTime;

    std::string displayMessage;
};


#endif //A_DARK_DISCOMFORT_MESSAGEDISPLAY_H
