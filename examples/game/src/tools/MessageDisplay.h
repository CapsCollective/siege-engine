#ifndef SIEGE_ENGINE_MESSAGEDISPLAY_H
#define SIEGE_ENGINE_MESSAGEDISPLAY_H

#include <core/entity/Entity.h>

class MessageDisplay : public Entity
{
public:

    // 'Structors

    MessageDisplay() : displayTime(0.f) {};

    // Public methods

    void DisplayMessage(const String& message, float time = 10.f);

protected:

    // Protected overrides

    void OnDraw2D() override;

private:

    // Private fields

    float displayTime;

    String displayMessage;
};

#endif // SIEGE_ENGINE_MESSAGEDISPLAY_H
