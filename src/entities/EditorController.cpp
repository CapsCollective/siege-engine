//
// Created by Jonathan Moallem on 11/12/20.
//

#include "EditorController.h"

void EditorController::onUpdate()
{
    if (!camera) { return; }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Ray ray = camera->GetMouseRay(GetMousePosition());

        selectedEntity = nullptr;
        for (auto& entity : *entities)
        {
            if (CheckCollisionRayBox(ray, entity->getBoundingBox()))
            {
                selectedEntity = entity;
            }
        }
    }
}

void EditorController::onDraw()
{
    if (!selectedEntity) { return; }

    DrawLine3D(selectedEntity->getPosition(), selectedEntity->getPosition() + raylib::Vector3(3.f, 0.f, 0.f), RED);
    DrawLine3D(selectedEntity->getPosition(), selectedEntity->getPosition() + raylib::Vector3(0.f, 3.f, 0.f), GREEN);
    DrawLine3D(selectedEntity->getPosition(), selectedEntity->getPosition() + raylib::Vector3(0.f, 0.f, 3.f), BLUE);
}

void EditorController::onUIDraw()
{
    if (!selectedEntity) { return; }

    DrawText(FormatText("Selected Entity Position: <%f, %f, %f>",
                        selectedEntity->getPosition().x,
                        selectedEntity->getPosition().y,
                        selectedEntity->getPosition().z),
             10.f, 70.f, 20.f, PINK);
}
