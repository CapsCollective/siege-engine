#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include "../../utils/ServiceLocator.h"
#include <Camera3D.hpp>
#include <vector>
#include <cstdint>
#include "../Entity.h"

class EditorController : public Entity
{
public:

    // Constructors

    explicit EditorController() :
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    selectedEntity(nullptr),
    camera(ServiceLocator::GetCamera()),
    moveDistance(0.5f),
    isGridActive(true),
    selectedIdx(0)
    {
        Entity::SetName("EditorController");
        Entity::SetFreeable(false);
        Entity::SetSerialisable(false);
    };

    // Class methods

    static bool TryAddEntity(std::string& entityName);

    // Public methods

    void TrySelectEntity(Entity* entity);

protected:

    // Protected overrides

    void OnToolUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

private:

    // Private fields

    bool isGridActive;

    float moveDistance;

    size_t selectedIdx;

    raylib::Camera3D* camera;

    Entity* selectedEntity;

    class MessageDisplay* messageDisplay;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
