#ifndef A_DARK_DISCOMFORT_MESSAGEDISPLAY_H
#define A_DARK_DISCOMFORT_MESSAGEDISPLAY_H

#include "../../systems/entity/Tool.h"

class MessageDisplay : public Tool
{
public:

    // Constructors

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
