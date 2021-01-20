#ifndef A_DARK_DISCOMFORT_EDITORCONTROLLER_H
#define A_DARK_DISCOMFORT_EDITORCONTROLLER_H

#include "../../systems/entity/Entity3D.h"
#include "../../utils/ServiceLocator.h"
#include <Camera3D.hpp>
#include <vector>
#include <cstdint>

class EditorController : public Entity3D
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

    void SelectEntity(Entity3D* entity);

    bool TrySetPos(raylib::Vector3 position);

    bool TrySetRot(float rotation);

    bool TrySetModelData(const std::string& model, const std::string& texture);

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

private:

    // Private enums

    enum EditorMode {
        POSITION,
        ROTATION,
    };

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
};


#endif //A_DARK_DISCOMFORT_EDITORCONTROLLER_H
