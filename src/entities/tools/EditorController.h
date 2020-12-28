#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include <Camera3D.hpp>
#include <vector>
#include <cstdint>
#include "../Entity.h"
#include "MessageDisplay.h"

class EditorController : public Entity
{
public:

    // Constructors

    explicit EditorController(raylib::Camera3D* camera, MessageDisplay* display) :
    messageDisplay(display),
    selectedEntity(nullptr),
    camera(camera),
    moveDistance(0.5f),
    isGridActive(true),
    selectedIdx(0)
    {
        Entity::SetName("EditorController");
        Entity::SetFreeable(false);
        Entity::SetSerialisable(false);
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

private:

    // Private fields

    bool isGridActive;

    float moveDistance;

    size_t selectedIdx;

    raylib::Camera3D* camera;

    Entity* selectedEntity;

    MessageDisplay* messageDisplay;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
