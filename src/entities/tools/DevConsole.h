#ifndef A_DARK_DISCOMFORT_DEVCONSOLE_H
#define A_DARK_DISCOMFORT_DEVCONSOLE_H

#include "../Entity.h"

class DevConsole : public Entity
{
public:

    // Constructors

    DevConsole() :
    isActive(false)
    {
        Entity::SetName("DevConsole");
    };

    // Public methods

    bool IsActive() const;

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnUIDraw() override;

    void QueueFree() override { /* block implementation */ };

    void Free() override { /* block implementation */ };

private:

    // Private fields

    bool isActive;

    std::string inputText;
};


#endif //A_DARK_DISCOMFORT_DEVCONSOLE_H
