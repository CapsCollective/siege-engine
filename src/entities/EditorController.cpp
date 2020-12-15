#include "EditorController.h"
#include "../entity_system/EntityStorage.h"
#include "Geometry.h"
#include <iostream>

void EditorController::OnUpdate()
{
    if (!camera) return;

    // Check for deselection and activation keys
    if (IsKeyPressed(KEY_ESCAPE)) selectedEntity = nullptr;
    if (IsKeyPressed(KEY_G)) isGridActive = !isGridActive;

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

    // TODO implement cycling through all entities on tab
    if (IsKeyPressed(KEY_TAB)) 
    {
        if (!selectedEntity) 
        {
            selectedEntity = EntityStorage::GetEntities()[0];
            lastIndex = 0;
        } 
        else
        {
            lastIndex = ++lastIndex % EntityStorage::GetEntities().size();
            selectedEntity = EntityStorage::GetEntities()[lastIndex];
        }
    }

    if (selectedEntity)
    {
        // Calculate move from input
        raylib::Vector3 move = raylib::Vector3::Zero();
        move.z = moveDistance * (float) (-IsKeyPressed(KEY_UP) + IsKeyPressed(KEY_DOWN));
        move.x = moveDistance * (float) (-IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT));

        // Apply the move to the position of the entity
        selectedEntity->SetPosition(selectedEntity->GetPosition() + move);

        // Free the entity if backspace is pressed
        if (IsKeyPressed(KEY_BACKSPACE)) {
            selectedEntity->QueueFree();
            selectedEntity = nullptr;
        }
    }
}

void EditorController::OnDraw()
{
    // Draw a reference grid at the centre of the scene if toggled on
    if (isGridActive) DrawGrid(100, 1.0f);

    if (selectedEntity)
    {
        // Draw gizmo display to selected entity location
        raylib::Vector3 entityPos = selectedEntity->GetPosition();
        DrawLine3D(entityPos, entityPos + raylib::Vector3(3.f, 0.f, 0.f), RED);
        DrawLine3D(entityPos, entityPos + raylib::Vector3(0.f, 3.f, 0.f), GREEN);
        DrawLine3D(entityPos, entityPos + raylib::Vector3(0.f, 0.f, 3.f), BLUE);
    }
}

void EditorController::OnUIDraw()
{
    if (!selectedEntity) return;

    // Format display text on the selected entity
    const char* nameLabel = FormatText("%s", selectedEntity->GetName().c_str());
    const char* posLabel = FormatText("Position: <%.2f, %.2f, %.2f>",
                                         selectedEntity->GetPosition().x,
                                         selectedEntity->GetPosition().y,
                                         selectedEntity->GetPosition().z);

    // Draw display text just above the entity in world-space
    Vector2 cubeScreenPosition = GetWorldToScreen(
            selectedEntity->GetPosition() + raylib::Vector3(0.f, 3.f, 0.f), *camera);
    DrawText(nameLabel,(int) cubeScreenPosition.x - MeasureText(nameLabel, 20)/2,
             (int) cubeScreenPosition.y, 20, PINK);
    DrawText(posLabel,(int) cubeScreenPosition.x - MeasureText(posLabel, 18)/2,
             (int) cubeScreenPosition.y + 20, 18, PINK);
}
