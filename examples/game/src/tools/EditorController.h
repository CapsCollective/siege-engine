//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_EDITORCONTROLLER_H
#define SIEGE_ENGINE_EDITORCONTROLLER_H

#include <core/entity/Entity.h>
#include <core/entity/EntityPtr.h>
#include <core/entity/EntitySystem.h>

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

#endif // SIEGE_ENGINE_EDITORCONTROLLER_H
