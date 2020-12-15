#include "EditorController.h"
#include "../entity_system/EntityStorage.h"
#include "Geometry.h"

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

    // THIS IS A TEST METHOD - PLEASE REMOVE WHEN ENTITY REMOVAL IS FULLY OPERATIONAL.
    if (IsKeyPressed(KEY_ENTER)) {
        EntityStorage::Instance()->Register(new Geometry(
                raylib::Vector3::Zero(),
                raylib::Vector3(5.f, 0.1f, 5.f)));
    }

    if (selectedEntity)
    {
        // Calculate move from input
        raylib::Vector3 move = raylib::Vector3::Zero();
        move.z = moveDistance * (-(float)IsKeyPressed(KEY_UP) + (float)IsKeyPressed(KEY_DOWN));
        move.x = moveDistance * (-(float)IsKeyPressed(KEY_LEFT) + (float)IsKeyPressed(KEY_RIGHT));

        // Apply the move to the position of the entity
        selectedEntity->SetPosition(selectedEntity->GetPosition() + move);

        // PLACEHOLDER: If an entity is selected, delete it only if SPACE is clicked with it.
        // NOTE -> Entity MUST implement QueueFree for this to work.
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
    const char* entityLabel = FormatText("Position: <%.2f, %.2f, %.2f>",
                        selectedEntity->GetPosition().x,
                        selectedEntity->GetPosition().y,
                        selectedEntity->GetPosition().z);

    // Draw display text just above the entity in world-space
    Vector2 cubeScreenPosition = GetWorldToScreen(
            selectedEntity->GetPosition() + raylib::Vector3(0.f, 3.f, 0.f), *camera);
    DrawText(entityLabel,(int) cubeScreenPosition.x - MeasureText(entityLabel, 20)/2,
             (int) cubeScreenPosition.y, 20, PINK);
}
