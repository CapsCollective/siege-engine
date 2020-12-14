//
// Created by Jonathan Moallem on 14/12/20.
//

#ifndef A_DARK_DISCOMFORT_FREECAM_H
#define A_DARK_DISCOMFORT_FREECAM_H

#include <Camera3D.hpp>
#include "Entity.h"

class FreeCam : public Entity
{
public:

    // Constructors

    explicit FreeCam(raylib::Camera3D* camera) :
    speed(10.0f),
    camera(camera)
    {
        camera->SetMode(CAMERA_CUSTOM);
    };

protected:

    // Protected overrides

    void OnUpdate() override;

private:

    // Private fields

    float speed;

    raylib::Camera3D* camera;
};


#endif //A_DARK_DISCOMFORT_FREECAM_H
