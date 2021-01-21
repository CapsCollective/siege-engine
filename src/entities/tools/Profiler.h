#ifndef A_DARK_DISCOMFORT_PROFILER_H
#define A_DARK_DISCOMFORT_PROFILER_H

#include "../../systems/entity/Tool.h"

class Profiler : public Tool
{
public:

    // Constructors

    explicit Profiler(bool isActive) :
    isActive(isActive)
    {
        Entity::SetName("Profiler");
    };

    // Public methods

    void ToggleActive();

protected:

    // Protected overrides

    void OnDraw2D() override;

private:

    // Private fields

    bool isActive;
};


#endif //A_DARK_DISCOMFORT_PROFILER_H
