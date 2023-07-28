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
#include "MessageDisplay.h"

enum EditorMode
{
    POSITION,
    ROTATION,
};

class EditorController : public Siege::Entity
{
public:

    // 'Structors

    EditorController() :
        isGridActive(true),
        isHandlingInput(true),
        currentMode(POSITION),
        movePrecision(2),
        rotatePrecision(3),
        selectedIdx(0),
        camera(ServiceLocator::GetFPSCamera()),
        selectedEntity(nullptr),
        messageDisplay(ServiceLocator::GetMessageDisplay()) {};

    // Class methods

    static bool TryAddEntity(Siege::String entityName);

    // Public methods

    void SelectEntity(class Entity* entity);

    bool TrySetPos(Siege::Vec3 position);

    bool TrySetRot(float rotation);

    void SetIsHandlingInput(bool state);

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

    bool isHandlingInput;

    EditorMode currentMode;

    int movePrecision;

    int rotatePrecision;

    size_t selectedIdx;

    FPSCamera* camera;

    Siege::EntityPtr<Entity> selectedEntity;

    Siege::EntityPtr<MessageDisplay> messageDisplay;
};

#endif // SIEGE_ENGINE_EDITORCONTROLLER_H
