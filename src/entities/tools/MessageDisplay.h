#ifndef A_DARK_DISCOMFORT_MESSAGEDISPLAY_H
#define A_DARK_DISCOMFORT_MESSAGEDISPLAY_H

#include "../../systems/entity/Entity.h"

class MessageDisplay : public Entity
{
public:

    // 'Structors

    MessageDisplay() :
    displayTime(0.f)
    {
        Entity::SetName("MessageDisplay");
    };

    // Public methods

    void DisplayMessage(const std::string& message, float time = 10.f);

protected:

    // Protected overrides

    void OnDraw2D() override;

private:

    // Private fields

    float displayTime;

    std::string displayMessage;
};


#endif //A_DARK_DISCOMFORT_MESSAGEDISPLAY_H
