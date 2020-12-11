//
// Created by Jonathan Moallem on 11/12/20.
//

#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include <Camera3D.hpp>
#include <vector>
#include "Entity.h"

class EditorController : public Entity
{
public:

    // Constructors

    explicit EditorController(raylib::Camera3D* camera, std::vector<Entity*>* entities) :
    selectedEntity(nullptr),
    entities(entities),
    camera(camera)
    {};

protected:

    // Protected overrides

    void onUpdate() override;

    void onDraw() override;

    void onUIDraw() override;

private:

    // Private fields

    raylib::Camera3D* camera;

    std::vector<Entity*>* entities;

    Entity* selectedEntity;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
