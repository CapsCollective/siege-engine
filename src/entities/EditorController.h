#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include <Camera3D.hpp>
#include <vector>
#include "Entity.h"
#include <cstdint>

class EditorController : public Entity
{
public:

    // Constructors

    explicit EditorController(raylib::Camera3D* camera) :
    selectedEntity(nullptr),
    camera(camera),
    moveDistance(0.5f),
    isGridActive(true)
    {
        Entity::SetName("EditorController");
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

    void QueueFree() override { /* block implementation */ };

private:

    // Private fields

    bool isGridActive;

    float moveDistance;

    raylib::Camera3D* camera;

    Entity* selectedEntity;

    size_t lastIndex;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
