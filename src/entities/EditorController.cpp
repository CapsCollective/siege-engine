//
// Created by Jonathan Moallem on 11/12/20.
//

#include "EditorController.h"
#include "../entity_system/EntityStorage.h"

void EditorController::OnUpdate()
{
    if (!camera) return;

    // Check for deselection key
    if (IsKeyPressed(KEY_ESCAPE)) selectedEntity = nullptr;

    // Check for mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (!camera) return;

        // Get the ray cast by the mouse position
        Ray ray = camera->GetMouseRay(GetMousePosition());

        // Check if any entities fall within the ray and set them as selected
        selectedEntity = nullptr;
        for (auto& entity : EntityStorage::GetEntities())
        {
            if (CheckCollisionRayBox(ray, entity->GetBoundingBox()))
            {
                selectedEntity = entity;
                break;
            }
        }
    }

    if (selectedEntity)
    {
        // Calculate move from input
        raylib::Vector3 move = raylib::Vector3::Zero();
        move.z = moveDistance * (-(float)IsKeyPressed(KEY_UP) + (float)IsKeyPressed(KEY_DOWN));
        move.x = moveDistance * (-(float)IsKeyPressed(KEY_LEFT) + (float)IsKeyPressed(KEY_RIGHT));

        // Apply the move to the position of the entity
        selectedEntity->SetPosition(selectedEntity->GetPosition() + move);
    }
}

void EditorController::OnDraw()
{
    if (!selectedEntity) return;

    // Draw gizmo display to selected entity location
    raylib::Vector3 entityPos = selectedEntity->GetPosition();
    DrawLine3D(entityPos, entityPos + raylib::Vector3(3.f, 0.f, 0.f), RED);
    DrawLine3D(entityPos, entityPos + raylib::Vector3(0.f, 3.f, 0.f), GREEN);
    DrawLine3D(entityPos, entityPos + raylib::Vector3(0.f, 0.f, 3.f), BLUE);
}

void EditorController::OnUIDraw()
{
    if (!selectedEntity) return;

    // Draw text to the screen with selected entity coordinates
    DrawText(FormatText("Selected Entity Position: <%f, %f, %f>",
                        selectedEntity->GetPosition().x,
                        selectedEntity->GetPosition().y,
                        selectedEntity->GetPosition().z),
             10.f, 40.f, 20.f, PINK);
}
