#include "EditorController.h"
#include "MessageDisplay.h"
#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include <utils/StringHelpers.h>
#include <scene/SceneManager.h>
#include <render/Camera.h>

// Static member initialisation
static float MOVE_LEVELS[] = {.01f, .1f, 1.f, 5.f, 10.f, 50.f, 100.f};
static float ROTATE_LEVELS[] = {.01f, .1f, 1.f, 15.f, 45.f, 90.f};

// Define colours
static Colour NORMAL_PINK(255, 109, 194);
static Colour BRIGHT_PINK(255, 5, 146);

void EditorController::OnStart()
{
    // TODO temporarily disabled transform gizmo and grid
//    Vec3 extents(3.f, 3.f, 3.f);
//    gizmoRenderItem = RenderSystem::Add(this, {extents.XComp(), Colour::Red}, gizmoPos);
//    gizmoRenderItem = RenderSystem::Add(this, {extents.YComp(), Colour::Green}, gizmoPos);
//    gizmoRenderItem = RenderSystem::Add(this, {extents.ZComp(), Colour::Blue}, gizmoPos);

//if (isGridActive) DrawGrid(100, 1.0f);
//gizmoRenderItem->isEnabled = selectedEntity;
}

void EditorController::OnUpdate()
{
    if (!camera || !messageDisplay) return;

    // Check for deselection and activation keys
    if (IsKeyPressed(KEY_ESCAPE)) SelectEntity(nullptr);

    // Check for command key presses
    if (IsKeyDown(KEY_LEFT_SUPER)){
        if (IsKeyPressed(KEY_G))
        {
            // Toggle grid display
            isGridActive = !isGridActive;
            messageDisplay->DisplayMessage("Grid display toggled");
        }
        else if (IsKeyPressed(KEY_R))
        {
            // Toggle rotation mode
            currentMode = currentMode == ROTATION ? POSITION : ROTATION;
            messageDisplay->DisplayMessage("Rotation mode toggled");
        }
        else if (IsKeyPressed(KEY_S))
        {
            // Save the scene
            SceneManager::SaveScene();
            messageDisplay->DisplayMessage("Scene saved");
        }
        else if (selectedEntity)
        {
            if (IsKeyPressed(KEY_D))
            {
                // Try duplicate the entity
                auto newEntity = selectedEntity->Clone();
                if (newEntity)
                {
                    EntityStorage::Add(newEntity);
                    messageDisplay->DisplayMessage("Entity duplicated");
                }
                else messageDisplay->DisplayMessage("Entity not duplicatable");
            }
            else if (IsKeyPressed(KEY_BACKSPACE))
            {
                // Free the entity
                selectedEntity->QueueFree();
                SelectEntity(nullptr);
                messageDisplay->DisplayMessage("Entity deleted");
            }
            // Adjust the transformation precision level
            else if (IsKeyPressed(KEY_EQUAL)) AdjustPrecision(1);
            else if (IsKeyPressed(KEY_MINUS)) AdjustPrecision(-1);
        }
    }

    // Check for mouse clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Get the ray cast by the mouse position
        RayCast ray = camera->GetMouseRay();

        // Check if any entities fall within the ray and set them as selected
        SelectEntity(nullptr);
        for (auto& entity : EntityStorage::GetEntities())
        {
            if (entity->GetBoundingBox().Intersects(ray))
            {
                SelectEntity(entity);
                break;
            }
        }
    }

    // Cycle through all entities
    if (IsKeyPressed(KEY_TAB))
    {
        // Select the first packed entity by index
        size_t totalEntities = EntityStorage::GetEntities().size();
        if (totalEntities > 0)
        {
            size_t startIdx = selectedIdx = !selectedEntity ? 0 : ++selectedIdx % totalEntities;
            do {
                // Try select the entity
                SelectEntity(EntityStorage::GetEntities()[selectedIdx]);

                // If valid, break the loop, or select the next entity
                if (selectedEntity) break;
                else selectedIdx = ++selectedIdx % totalEntities;
            } while (selectedIdx != startIdx);
        }
    }

    if (selectedEntity)
    {
        // Run the appropriate editor mode logic on the selected entity
        switch (currentMode)
        {
            case POSITION: {
                // Calculate move from input
                Vec3 move = Vec3::Zero;
                float precision = MOVE_LEVELS[movePrecision];
                move.x = precision * (float) (-IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT));

                // Switch vertical move input between z and y axis based on shift key down
                float verticalMove = precision * (float) (-IsKeyPressed(KEY_UP) + IsKeyPressed(KEY_DOWN));
                IsKeyDown(KEY_LEFT_SHIFT) ? move.y = -verticalMove : move.z = verticalMove;

                // Apply the move to the position of the entity
                Vec3 entityPosition = selectedEntity->GetPosition();
                selectedEntity->SetPosition(entityPosition + move);
                break;
            }
            case ROTATION: {
                // Calculate rotation from input and apply it to the rotation of the entity
                float precision = ROTATE_LEVELS[rotatePrecision];
                float rotation = precision * (float) (-IsKeyPressed(KEY_LEFT) + IsKeyPressed(KEY_RIGHT));
                selectedEntity->SetRotation(selectedEntity->GetRotation() + rotation);
                break;
            }
        }
    }
}

void EditorController::OnDraw2D()
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
    Vec3 screenPosition = camera->GetScreenPos(selectedEntity->GetPosition());
    DrawText(nameLabel,(int) screenPosition.x - MeasureText(nameLabel, 20)/2,
             (int) screenPosition.y, 20, PINK);
    DrawText(posLabel,(int) screenPosition.x - MeasureText(posLabel, 18)/2,
             (int) screenPosition.y + 20, 18, currentMode == POSITION ? BRIGHT_PINK : NORMAL_PINK);
    DrawText(rotLabel,(int) screenPosition.x - MeasureText(posLabel, 18)/2,
             (int) screenPosition.y + 40, 18, currentMode == ROTATION ? BRIGHT_PINK : NORMAL_PINK);
}

void EditorController::SelectEntity(Entity* entity)
{
    // Display the entity based on its existence
    selectedEntity = entity;
}

bool EditorController::TryAddEntity(std::string& entityName)
{
    // Lowercase the supplied entity name
    entityName = StringHelpers::LowercaseString(entityName);

    // Check for matching cases to run
    if (entityName == "geometry")
    {
        EntityStorage::Add(new Geometry());
        return true;
    }
    else if (entityName == "player")
    {
        EntityStorage::Add(new Player());
        return true;
    }
    return false;
}

void EditorController::AdjustPrecision(int adjustment)
{
    // Precision adjust for the relevant mode
    switch (currentMode)
    {
        case POSITION: {
            // Check boundaries on move precision
            int newPrecision = movePrecision + adjustment;
            if (newPrecision >= 0 && newPrecision < sizeof(MOVE_LEVELS)/sizeof(int))
            {
                // Apply the new precision value
                movePrecision = newPrecision;
                messageDisplay->DisplayMessage(FormatText("Move precision set to %.2f", MOVE_LEVELS[movePrecision]));
            }
            break;
        }
        case ROTATION: {
            // Check boundaries on rotate precision
            int newPrecision = rotatePrecision + adjustment;
            if (newPrecision >= 0 && newPrecision < sizeof(ROTATE_LEVELS)/sizeof(int))
            {
                // Apply the new precision value
                rotatePrecision = newPrecision;
                messageDisplay->DisplayMessage(FormatText("Rotate precision set to %.2f°", ROTATE_LEVELS[rotatePrecision]));
            }
            break;
        }
    }
}

bool EditorController::TrySetPos(Vec3 position)
{
    if (!selectedEntity) return false;
    selectedEntity->SetPosition(position);
    return true;
}

bool EditorController::TrySetRot(float rotation)
{
    if (!selectedEntity) return false;
    selectedEntity->SetRotation(rotation);
    return true;
}

bool EditorController::TrySetModelData(const std::string& model, const std::string& texture)
{
    if (!selectedEntity) return false;
    // TODO modelable interface temporarily disabled
//    auto modelableEntity = selectedEntity.Get<Modelable>();
//    if (modelableEntity)
//    {
//        auto modelData = modelableEntity->GetModelData();
//        modelableEntity->SetModelData(ModelData(model, texture));
//        return true;
//    }
    return false;
}
