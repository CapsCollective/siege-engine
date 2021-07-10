#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include "../../engine/entity/Entity.h"
#include "../../engine/entity/EntityStorage.h"
#include "../../engine/entity/EntityPtr.h"
#include "../../ServiceLocator.h"
#include <Camera3D.hpp>
#include <vector>
#include <cstdint>

enum EditorMode {
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
        messageDisplay(ServiceLocator::GetMessageDisplay())
    {};

    // Class methods

    static bool TryAddEntity(std::string& entityName);

    // Public methods

    void SelectEntity(class Entity* entity);

    bool TrySetPos(raylib::Vector3 position);

    bool TrySetRot(float rotation);

    bool TrySetModelData(const std::string& model, const std::string& texture);

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnDraw2D() override;

private:

    // Private methods

    void AdjustPrecision(int adjustment);

    // Private fields

    static float moveLevels[];

    static float rotateLevels[];

    bool isGridActive;

    EditorMode currentMode;

    int movePrecision;

    int rotatePrecision;

    size_t selectedIdx;

    raylib::Camera3D* camera;

    EntityPtr<Entity> selectedEntity;

    EntityPtr<MessageDisplay> messageDisplay;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
