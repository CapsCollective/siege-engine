//
// Created by Jonathan Moallem on 11/12/20.
//

#include "EditorController.h"
#include "../entity_system/EntityStorage.h"

void EditorController::OnUpdate()
{
    if (!camera) { return; }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Ray ray = camera->GetMouseRay(GetMousePosition());

        selectedEntity = nullptr;
        for (auto& entity : EntityStorage::GetEntities())
        {
            if (CheckCollisionRayBox(ray, entity->GetBoundingBox()))
            {
                selectedEntity = entity;
            }
        }
    }
}

void EditorController::OnDraw()
{
    if (!selectedEntity) { return; }

    DrawLine3D(selectedEntity->GetPosition(), selectedEntity->GetPosition() + raylib::Vector3(3.f, 0.f, 0.f), RED);
    DrawLine3D(selectedEntity->GetPosition(), selectedEntity->GetPosition() + raylib::Vector3(0.f, 3.f, 0.f), GREEN);
    DrawLine3D(selectedEntity->GetPosition(), selectedEntity->GetPosition() + raylib::Vector3(0.f, 0.f, 3.f), BLUE);
}

void EditorController::OnUIDraw()
{
    if (!selectedEntity) { return; }

    DrawText(FormatText("Selected Entity Position: <%f, %f, %f>",
                        selectedEntity->GetPosition().x,
                        selectedEntity->GetPosition().y,
                        selectedEntity->GetPosition().z),
             10.f, 70.f, 20.f, PINK);
}
