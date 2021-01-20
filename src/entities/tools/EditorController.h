#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include "../../systems/entity/Entity2D.h"
#include "../../utils/ServiceLocator.h"
#include "Gizmo.h"
#include "../../systems/entity/EntityStorage.h"
#include <Camera3D.hpp>
#include <vector>
#include <cstdint>

enum EditorMode {
    POSITION,
    ROTATION,
};

class EditorController : public Entity2D
{
public:

    // Constructors
    EditorController() :
    messageDisplay(ServiceLocator::GetMessageDisplay()),
    selectedEntity(nullptr),
    camera(ServiceLocator::GetCamera()),
    movePrecision(2),
    rotatePrecision(3),
    currentMode(POSITION),
    isGridActive(true),
    selectedIdx(0),
    gizmo(new Gizmo(false))
    {
        EntityStorage::Register(gizmo);
        Entity::SetName("EditorController");
    };

    ~EditorController()
    {
        gizmo->QueueFree();
    };

    // Class methods

    static bool TryAddEntity(std::string& entityName);

    // Public methods

    void SelectEntity(class Entity3D* entity);

    bool TrySetPos(raylib::Vector3 position);

    bool TrySetRot(float rotation);

    bool TrySetModelData(const std::string& model, const std::string& texture);

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

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

    Entity3D* selectedEntity;

    class MessageDisplay* messageDisplay;

    Gizmo* gizmo;
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
