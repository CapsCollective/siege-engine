//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "EditorController.h"

#include <core/Statics.h>
#include <core/input/InputSystem.h>
#include <core/render/Camera.h>
#include <core/render/Window.h>
#include <core/scene/SceneSystem.h>
#include <utils/Colour.h>

#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include "MessageDisplay.h"

// Static member initialisation
static float MOVE_LEVELS[] = {.01f, .1f, 1.f, 5.f, 10.f, 50.f, 100.f};
static float ROTATE_LEVELS[] = {.01f, .1f, 1.f, 15.f, 45.f, 90.f};

// Define colours
static Siege::IColour BRIGHT_PINK(255, 5, 146);

void EditorController::OnStart()
{
    // TODO temporarily disabled transform gizmo and grid
    //    Vec3 extents(3.f, 3.f, 3.f);
    //    gizmoRenderItem = RenderSystem::Add(this, {extents.XComp(), Siege::Colour::Red},
    //    gizmoPos); gizmoRenderItem = RenderSystem::Add(this, {extents.YComp(),
    //    Siege::Colour::Green}, gizmoPos); gizmoRenderItem = RenderSystem::Add(this,
    //    {extents.ZComp(), Siege::Colour::Blue}, gizmoPos);

    // if (isGridActive) DrawGrid(100, 1.0f);
    // gizmoRenderItem->isEnabled = selectedEntity;
}

void EditorController::OnUpdate()
{
    if (!camera || !messageDisplay) return;

    // Check for deselection and activation keys
    if (Siege::Statics::Input().KeyPressed(Siege::Key::ESCAPE)) SelectEntity(nullptr);

    // Check for command key presses
    if (Siege::Statics::Input().KeyDown(Siege::Key::LEFT_SUPER))
    {
        if (Siege::Statics::Input().KeyPressed(Siege::Key::G))
        {
            // Toggle grid display
            isGridActive = !isGridActive;
            messageDisplay->DisplayMessage("Grid display toggled");
        }
        else if (Siege::Statics::Input().KeyPressed(Siege::Key::R))
        {
            // Toggle rotation mode
            currentMode = currentMode == ROTATION ? POSITION : ROTATION;
            messageDisplay->DisplayMessage("Rotation mode toggled");
        }
        else if (Siege::Statics::Input().KeyPressed(Siege::Key::S))
        {
            // Save the scene
            Siege::Statics::Scene().SaveScene();
            messageDisplay->DisplayMessage("Scene saved");
        }
        else if (selectedEntity)
        {
            if (Siege::Statics::Input().KeyPressed(Siege::Key::D))
            {
                // Try duplicate the entity
                auto newEntity = selectedEntity->Clone();
                if (newEntity)
                {
                    Siege::Statics::Entity().Add(newEntity);
                    messageDisplay->DisplayMessage("Entity duplicated");
                }
                else messageDisplay->DisplayMessage("Entity not duplicatable");
            }
            else if (Siege::Statics::Input().KeyPressed(Siege::Key::BACKSPACE))
            {
                // Free the entity
                selectedEntity->QueueFree();
                SelectEntity(nullptr);
                messageDisplay->DisplayMessage("Entity deleted");
            }
            // Adjust the transformation precision level
            else if (Siege::Statics::Input().KeyPressed(Siege::Key::EQUAL)) AdjustPrecision(1);
            else if (Siege::Statics::Input().KeyPressed(Siege::Key::MINUS)) AdjustPrecision(-1);
        }
    }

    // Check for mouse clicks
    if (Siege::Statics::Input().MousePressed(Siege::Mouse::LEFT))
    {
        // Get the ray cast by the mouse position
        Siege::RayCast ray = camera->GetMouseRay();

        // Check if any entities fall within the ray and set them as selected
        SelectEntity(nullptr);
        for (auto& entity : Siege::Statics::Entity().GetEntities())
        {
            if (entity->GetBoundingBox().Intersects(ray))
            {
                SelectEntity(entity);
                break;
            }
        }
    }

    // Cycle through all entities
    if (Siege::Statics::Input().KeyPressed(Siege::Key::TAB))
    {
        // Select the first packed entity by index
        size_t totalEntities = Siege::Statics::Entity().GetEntities().size();
        if (totalEntities > 0)
        {
            size_t startIdx = selectedIdx = !selectedEntity ? 0 : ++selectedIdx % totalEntities;
            do
            {
                // Try select the entity
                SelectEntity(Siege::Statics::Entity().GetEntities()[selectedIdx]);

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
                Siege::Vec3 move = Siege::Vec3::Zero;
                float precision = MOVE_LEVELS[movePrecision];
                move.x =
                    precision * (float) (-Siege::Statics::Input().KeyPressed(Siege::Key::LEFT) +
                                         Siege::Statics::Input().KeyPressed(Siege::Key::RIGHT));

                // Switch vertical move input between z and y axis based on shift key down
                float verticalMove =
                    precision * (float) (-Siege::Statics::Input().KeyPressed(Siege::Key::UP) +
                                         Siege::Statics::Input().KeyPressed(Siege::Key::DOWN));
                Siege::Statics::Input().KeyDown(Siege::Key::LEFT_SHIFT) ? move.y = -verticalMove :
                                                                          move.z = verticalMove;

                // Apply the move to the position of the entity
                Siege::Vec3 entityPosition = selectedEntity->GetPosition();
                selectedEntity->SetPosition(entityPosition + move);
                break;
            }
            case ROTATION: {
                // Calculate rotation from input and apply it to the rotation of the entity
                float precision = ROTATE_LEVELS[rotatePrecision];
                float rotation =
                    precision * (float) (-Siege::Statics::Input().KeyPressed(Siege::Key::LEFT) +
                                         Siege::Statics::Input().KeyPressed(Siege::Key::RIGHT));
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
    Siege::String nameLabel = Siege::String("%s").Formatted(selectedEntity->GetName().Str());
    Siege::String posLabel = Siege::String("Position: <%.2f, %.2f, %.2f>")
                                 .Formatted(selectedEntity->GetPosition().x,
                                            selectedEntity->GetPosition().y,
                                            selectedEntity->GetPosition().z);
    Siege::String rotLabel =
        Siege::String("Rotation: %.2f°").Formatted(selectedEntity->GetRotation());

    // Draw display text just above the entity in world-space
    Siege::Vec3 screenPosition = camera->GetScreenPos(selectedEntity->GetPosition());
    Siege::Statics::Render().DrawText2D(
        nameLabel,
        (int) screenPosition.x - Siege::Window::GetTextWidth(nameLabel, 20) / 2,
        (int) screenPosition.y,
        20,
        Siege::IColour::Pink);
    Siege::Statics::Render().DrawText2D(
        posLabel,
        (int) screenPosition.x - Siege::Window::GetTextWidth(posLabel, 18) / 2,
        (int) screenPosition.y + 20,
        18,
        currentMode == POSITION ? BRIGHT_PINK : Siege::IColour::Pink);
    Siege::Statics::Render().DrawText2D(
        rotLabel,
        (int) screenPosition.x - Siege::Window::GetTextWidth(posLabel, 18) / 2,
        (int) screenPosition.y + 40,
        18,
        currentMode == ROTATION ? BRIGHT_PINK : Siege::IColour::Pink);
}

void EditorController::SelectEntity(Entity* entity)
{
    // Display the entity based on its existence
    selectedEntity = entity;
}

bool EditorController::TryAddEntity(Siege::String entityName)
{
    // Lowercase the supplied entity name
    entityName.ToLower();

    // Check for matching cases to run
    if (entityName == "geometry")
    {
        Siege::Statics::Entity().Add(new Geometry());
        return true;
    }
    else if (entityName == "player")
    {
        Siege::Statics::Entity().Add(new Player());
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
            if (newPrecision >= 0 && newPrecision < sizeof(MOVE_LEVELS) / sizeof(int))
            {
                // Apply the new precision value
                movePrecision = newPrecision;
                messageDisplay->DisplayMessage(Siege::String("Move precision set to %.2f")
                                                   .Formatted(MOVE_LEVELS[movePrecision]));
            }
            break;
        }
        case ROTATION: {
            // Check boundaries on rotate precision
            int newPrecision = rotatePrecision + adjustment;
            if (newPrecision >= 0 && newPrecision < sizeof(ROTATE_LEVELS) / sizeof(int))
            {
                // Apply the new precision value
                rotatePrecision = newPrecision;
                messageDisplay->DisplayMessage(Siege::String("Rotate precision set to %.2f°")
                                                   .Formatted(ROTATE_LEVELS[rotatePrecision]));
            }
            break;
        }
    }
}

bool EditorController::TrySetPos(Siege::Vec3 position)
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
