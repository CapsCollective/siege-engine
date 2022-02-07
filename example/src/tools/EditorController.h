#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include <engine/entity/Entity.h>
#include <engine/entity/EntityPtr.h>
#include <engine/entity/EntitySystem.h>

#include <cstdint>
#include <vector>

#include "../ServiceLocator.h"

enum EditorMode
{
    POSITION,
    ROTATION,
};

class EditorController : public Entity
{
public:

    // 'Structors

    EditorController() :
        isGridActive(true),
        currentMode(POSITION),
        movePrecision(2),
        rotatePrecision(3),
        selectedIdx(0),
        camera(ServiceLocator::GetCamera()),
        selectedEntity(nullptr),
        messageDisplay(ServiceLocator::GetMessageDisplay()) {};

    // Class methods

    static bool TryAddEntity(String entityName);

    // Public methods

    void SelectEntity(class Entity* entity);

    bool TrySetPos(Vec3 position);

    bool TrySetRot(float rotation);

protected:

    // Protected overrides

    void OnStart() override;

    void OnUpdate() override;

    void OnDraw2D() override;

private:

    // Private methods

    void AdjustPrecision(int adjustment);

    // Private fields

    bool isGridActive;

    EditorMode currentMode;

    int movePrecision;

    int rotatePrecision;

    size_t selectedIdx;

    Cam* camera;

    EntityPtr<Entity> selectedEntity;

    EntityPtr<MessageDisplay> messageDisplay;
};

#endif // A_DARK_DISCOMFORT_EDITORCONTROLLER_H
