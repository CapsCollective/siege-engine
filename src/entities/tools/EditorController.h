#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include "../../utils/ServiceLocator.h"
#include <Camera3D.hpp>
#include <vector>
#include <cstdint>
#include "../Entity.h"

enum EditorMode {
    POSITION,
    ROTATION,
};

class EditorController : public Entity
{
public:

    // Constructors
    explicit EditorController() :
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    selectedEntity(nullptr),
    camera(ServiceLocator::GetCamera()),
    movePrecision(2),
    rotatePrecision(3),
    currentMode(POSITION),
    isGridActive(true),
    selectedIdx(0)
    {
        Entity::SetName("EditorController");
    };

    // Class methods

    static bool TryAddEntity(std::string& entityName);

    // Public methods

    void SelectEntity(Entity* entity);

    bool TrySetPos(raylib::Vector3 position);

    bool TrySetRot(float rotation);

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

private:

    // Private methods

    void AdjustPrecision(int adjustment);

    // Private fields

    static float moveLevels[];

    static float rotateLevels[];

    EditorMode currentMode;

    bool isGridActive;

    int movePrecision;

    int rotatePrecision;

    size_t selectedIdx;

    raylib::Camera3D* camera;

    Entity* selectedEntity;

    class MessageDisplay* messageDisplay;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
