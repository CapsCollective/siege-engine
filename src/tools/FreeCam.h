#ifndef A_DARK_DISCOMFORT_FREECAM_H
#define A_DARK_DISCOMFORT_FREECAM_H

#include "../ServiceLocator.h"
#include <entity/Entity.h>

class FreeCam : public Entity
{
public:

    // 'Structors

    FreeCam() :
        moveSpeed(10.f),
        lookSpeed(0.1f),
        camera(ServiceLocator::GetCamera()),
        rotation(Vec3(0.f, -1.f, -1.f)),
        previousMousePosition({0.f, 0.f})
    {}

protected:

    // Protected overrides

    void OnUpdate() override;

private:

    // Private fields

    float moveSpeed;

    float lookSpeed;

    Cam* camera;

    Vec3 rotation;

    Vector2 previousMousePosition;
};


#endif //A_DARK_DISCOMFORT_FREECAM_H
