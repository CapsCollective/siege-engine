//
// Created by Jonathan Moallem on 14/12/20.
//

#ifndef A_DARK_DISCOMFORT_FREECAM_H
#define A_DARK_DISCOMFORT_FREECAM_H

#include <Camera3D.hpp>
#include "../Entity.h"

class FreeCam : public Entity
{
public:

    // Constructors

    explicit FreeCam(raylib::Camera3D* camera) :
    moveSpeed(10.f),
    lookSpeed(0.1f),
    camera(camera),
    previousMousePosition({0.f, 0.f}),
    rotation(raylib::Vector3(0.f, -1.f, -1.f))
    {
        Entity::SetName("FreeCam");
        camera->SetMode(CAMERA_CUSTOM);
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void QueueFree() override { /* block implementation */ };

private:

    // Private fields

    float moveSpeed;

    float lookSpeed;

    raylib::Camera3D* camera;

    raylib::Vector3 rotation;

    Vector2 previousMousePosition;
};


#endif //A_DARK_DISCOMFORT_FREECAM_H
