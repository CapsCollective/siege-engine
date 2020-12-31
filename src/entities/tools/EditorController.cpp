#include "EditorController.h"
#include "../../entity_system/EntityStorage.h"
#include "MessageDisplay.h"
#include "../Geometry.h"
#include "../Player.h"
#include "../../utils/StringHelpers.h"

// Define custom colours
#define BRIGHT_PINK CLITERAL(Color){ 255, 5, 146, 255 }

void EditorController::OnUpdate()
{
    if (!camera || !messageDisplay) return;

    // Check for deselection and activation keys
    if (IsKeyPressed(KEY_ESCAPE)) selectedEntity = nullptr;
    if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_G))
    {
        isGridActive = !isGridActive;
        messageDisplay->DisplayMessage("Grid display toggled");
    }
    if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_R))
    {
        isRotationModeActive = !isRotationModeActive;
        messageDisplay->DisplayMessage("Rotation mode toggled");
    }

    // Check for mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Get the ray cast by the mouse position
        Ray ray = camera->GetMouseRay(GetMousePosition());

        // Check if any entities fall within the ray and set them as selected
        selectedEntity = nullptr;
        for (auto &entity : EntityStorage::GetEntities())
        {
            if (CheckCollisionRayBox(ray, entity->GetBoundingBox()))
            {
                TrySelectEntity(entity);
                break;
            }
        }
    }

    // Cycle through all entities
    if (IsKeyPressed(KEY_TAB))
    {
        // Select the first packed entity by index
        int totalEntities = EntityStorage::GetEntities().size();
        size_t startIdx = selectedIdx = !selectedEntity ? 0 : ++selectedIdx % totalEntities;
        do {
            // Try select the entity
            TrySelectEntity(EntityStorage::GetPackedEntity(selectedIdx));

            // If valid, break the loop, or select the next entity
            if (selectedEntity) break;
            else selectedIdx = ++selectedIdx % totalEntities;
        } while (selectedIdx != startIdx);
    }

    if (selectedEntity)
    {
        if (isRotationModeActive)
        {
            // Calculate rotation from input and apply it to the rotation of the entity
            float rotation = rotationAmount * (float) (-IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT));
            selectedEntity->SetRotation(selectedEntity->GetRotation() + rotation);
        }
        else
        {
            // Calculate move from input
            raylib::Vector3 move = raylib::Vector3::Zero();
            move.x = moveDistance * (float) (-IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT));

            // Switch vertical move input between z and y axis based on shift key down
            float verticalMove = moveDistance * (float) (-IsKeyPressed(KEY_UP) + IsKeyPressed(KEY_DOWN));
            IsKeyDown(KEY_LEFT_SHIFT) ? move.y = -verticalMove : move.z = verticalMove;

            // Apply the move to the position of the entity
            selectedEntity->SetPosition(selectedEntity->GetPosition() + move);
        }

        // Free the entity if backspace is pressed
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            selectedEntity->QueueFree();
            selectedEntity = nullptr;
            messageDisplay->DisplayMessage("Entity deleted");
        }

        // Duplicate the entity if D is pressed
        if (IsKeyDown(KEY_LEFT_SUPER) && IsKeyPressed(KEY_D))
        {
            EntityStorage::Register(selectedEntity->Clone());
            messageDisplay->DisplayMessage("Entity duplicated");
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
    const char* rotLabel = FormatText("Rotation: %.2f°", selectedEntity->GetRotation());

    // Draw display text just above the entity in world-space
    Vector2 screenPosition = GetWorldToScreen(
            selectedEntity->GetPosition() + raylib::Vector3(0.f, 4.f, 0.f), *camera);
    DrawText(nameLabel,(int) screenPosition.x - MeasureText(nameLabel, 20)/2,
             (int) screenPosition.y, 20, PINK);
    DrawText(posLabel,(int) screenPosition.x - MeasureText(posLabel, 18)/2,
             (int) screenPosition.y + 20, 18, isRotationModeActive ? PINK : BRIGHT_PINK);
    DrawText(rotLabel,(int) screenPosition.x - MeasureText(posLabel, 18)/2,
             (int) screenPosition.y + 40, 18, isRotationModeActive ? BRIGHT_PINK : PINK);
}

void EditorController::TrySelectEntity(Entity *entity)
{
    // Prevent selection of entities that are not serialisable
    if (entity && !entity->IsSerialisable()) entity = nullptr;
    selectedEntity = entity;
}

bool EditorController::TryAddEntity(std::string& entityName)
{
    // Lowercase the supplied entity name
    entityName = StringHelpers::LowercaseString(entityName);

    // Check for matching cases
    if (entityName == "geometry")
    {
        EntityStorage::Register(new Geometry());
        return true;
    }
    else if (entityName == "player")
    {
        EntityStorage::Register(new Player());
        return true;
    }
    return false;
}
