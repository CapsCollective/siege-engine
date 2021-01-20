#ifndef A_DARK_DISCOMFORT_GIZMO_H
#define A_DARK_DISCOMFORT_GIZMO_H

#include "../../systems/entity/Entity3D.h"

class Gizmo : public Entity3D
{
public:

    explicit Gizmo(bool isActive = true) :
    isActive(isActive),
    extents(3.f)
    {};

    // Public methods

    void SetActive(bool active);

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    bool isActive;

    float extents;

};


#endif //A_DARK_DISCOMFORT_GIZMO_H
