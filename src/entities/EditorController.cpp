//
// Created by Jonathan Moallem on 11/12/20.
//

#include "EditorController.h"
#include "../entity_system/EntityStorage.h"

void EditorController::OnUpdate()
{
    if (!camera) return;

    // Check for mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Get the ray cast by teh mouse position
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
        move.z = moveDistance * (-(float)IsKeyPressed(KEY_W) + (float)IsKeyPressed(KEY_S));
        move.x = moveDistance * (-(float)IsKeyPressed(KEY_A) + (float)IsKeyPressed(KEY_D));

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
             10.f, 70.f, 20.f, PINK);
}
