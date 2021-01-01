#ifndef A_DARK_DISCOMFORT_PROFILER_H
#define A_DARK_DISCOMFORT_PROFILER_H

#include "../Entity.h"

class Profiler : public Entity
{
public:

    // Constructors

    explicit Profiler(bool isActive) :
            isActive(isActive)
    {
        Entity::SetName("Profiler");
        Entity::SetFreeable(false);
        Entity::SetSerialisable(false);
    };

    // Public methods

    void ToggleActive();

protected:

    // Protected overrides

    void OnUIDraw() override;

private:

    // Private fields

    bool isActive;
};


#endif //A_DARK_DISCOMFORT_PROFILER_H
