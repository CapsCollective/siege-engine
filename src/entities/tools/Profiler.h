#ifndef A_DARK_DISCOMFORT_PROFILER_H
#define A_DARK_DISCOMFORT_PROFILER_H

#include "../../systems/entity/Entity.h"

class Profiler : public Entity
{
public:

    // 'Structors

    explicit Profiler(bool isActive) :
        isActive(isActive)
    {};

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
