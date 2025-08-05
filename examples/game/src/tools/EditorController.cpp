//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "EditorController.h"

#include <core/Statics.h>
#include <core/scene/SceneSystem.h>
#include <utils/Colour.h>
#include <utils/math/Projection.h>
#include <window/Input.h>

#include "../entities/Geometry.h"
#include "../entities/Player.h"
#include "MessageDisplay.h"

// Static member initialisation
static float MOVE_LEVELS[] = {.01f, .1f, 1.f, 5.f, 10.f, 50.f, 100.f};
static float ROTATE_LEVELS[] = {.01f, .1f, 1.f, 15.f, 45.f, 90.f};

void EditorController::OnUpdate()
{
    // The editor should not be able to receive input while the console is open
    if ((!camera || !messageDisplay) || !isHandlingInput) return;

    // Check for deselection and activation keys
    if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_ESCAPE)) SelectEntity(nullptr);

    // Check for command key presses
    if (Siege::Input::IsKeyDown(Siege::Key::KEY_LEFT_SUPER))
    {
        if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_G))
        {
            // Toggle grid display
            isGridActive = !isGridActive;
            messageDisplay->DisplayMessage("Grid display toggled");
        }
        else if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_R))
        {
            // Toggle rotation mode
            currentMode = currentMode == ROTATION ? POSITION : ROTATION;
            messageDisplay->DisplayMessage("Rotation mode toggled");
        }
        else if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_S))
        {
            // Save the scene
            Siege::Statics::Scene().SaveScene();
            messageDisplay->DisplayMessage("Scene saved");
        }
        else if (selectedEntity)
        {
            if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_D))
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
            else if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_BACKSPACE))
            {
                // Free the entity
                selectedEntity->QueueFree();
                SelectEntity(nullptr);
                messageDisplay->DisplayMessage("Entity deleted");
            }
            // Adjust the transformation precision level
            else if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_EQUAL)) AdjustPrecision(1);
            else if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_MINUS)) AdjustPrecision(-1);
        }
    }

    // Check for mouse clicks
    if (Siege::Input::IsMouseButtonJustPressed(Siege::Mouse::MOUSE_BUTTON_LEFT))
    {
        // Get the ray cast by the mouse position
        Siege::Vec2 cursorPos = {(float) Siege::Input::GetCursorPosition().x,
                                 (float) Siege::Input::GetCursorPosition().y};
        FPSCamera* camera3d = ServiceLocator::GetFPSCamera();
        Siege::Window* window = ServiceLocator::GetWindow();

        auto ray = camera3d->GetMouseRay(cursorPos.x,
                                         cursorPos.y,
                                         window->GetWidth(),
                                         window->GetHeight());

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
    if (Siege::Input::IsKeyJustPressed(Siege::Key::KEY_TAB))
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
        CC_LOG_INFO("IDX: {}", selectedIdx)
    }

    if (selectedEntity)
    {
        // Run the appropriate editor mode logic on the selected entity
        switch (currentMode)
        {
            case POSITION:
            {
                // Calculate move from input
                Siege::Vec3 move = Siege::Vec3::Zero();
                float precision = MOVE_LEVELS[movePrecision];
                move.x = precision * (float) -(Siege::Input::IsKeyDown(Siege::Key::KEY_LEFT) +
                                               -Siege::Input::IsKeyDown(Siege::Key::KEY_RIGHT));

                // Switch vertical move input between z and y-axis based on shift key down
                float verticalMove =
                    precision * (float) (Siege::Input::IsKeyDown(Siege::Key::KEY_UP) +
                                         -Siege::Input::IsKeyDown(Siege::Key::KEY_DOWN));
                Siege::Input::IsKeyDown(Siege::Key::KEY_LEFT_SHIFT) ? move.y = -verticalMove :
                                                                      move.z = verticalMove;

                // Apply the move to the position of the entity
                Siege::Vec3 entityPosition = selectedEntity->GetPosition();
                selectedEntity->SetPosition(entityPosition + move);
                break;
            }
            case ROTATION:
            {
                // Calculate rotation from input and apply it to the rotation of the entity
                float precision = ROTATE_LEVELS[rotatePrecision];
                float rotation =
                    precision * (float) (Siege::Input::IsKeyDown(Siege::Key::KEY_LEFT) +
                                         -Siege::Input::IsKeyDown(Siege::Key::KEY_RIGHT));
                Siege::Xform& xform = selectedEntity->GetTransform();
                xform.SetRotationY(selectedEntity->GetRotation().y + rotation);
                break;
            }
        }
    }
}

void EditorController::OnDraw2D()
{
    if (!selectedEntity) return;

    Siege::Window* window = ServiceLocator::GetWindow();
    auto position = Siege::WorldToScreen(selectedEntity->GetPosition(),
                                         camera->GetCamera().projection * camera->GetCamera().view,
                                         window->GetWidth(),
                                         window->GetHeight());

    // Format display text on the selected entity
    Siege::String nameLabel = Siege::String("%s").Formatted(selectedEntity->GetType());
    Siege::String posLabel = Siege::String("Position: <%.2f, %.2f, %.2f>")
                                 .Formatted(selectedEntity->GetPosition().x,
                                            selectedEntity->GetPosition().y,
                                            selectedEntity->GetPosition().z);
    Siege::String rotLabel =
        Siege::String("Rotation: %.2f").Formatted(selectedEntity->GetRotation().y);

    // Draw display text just above the entity in world-space
    auto& pixel = ServiceLocator::GetRenderResources()->GetFont();
    ServiceLocator::GetRenderer()->DrawText2D(nameLabel,
                                              pixel,
                                              position - Siege::Vec2 {48.f, 42.f},
                                              {12.f, 12.f},
                                              0.f,
                                              Siege::IColour::Pink);
    ServiceLocator::GetRenderer()->DrawText2D(posLabel,
                                              pixel,
                                              position - Siege::Vec2 {48.f, 26.f},
                                              {12.f, 12.f},
                                              0.f,
                                              Siege::IColour::Pink);
    ServiceLocator::GetRenderer()->DrawText2D(rotLabel,
                                              pixel,
                                              position - Siege::Vec2 {48.f, 10.f},
                                              {12.f, 12.f},
                                              0.f,
                                              Siege::IColour::Pink);
}

void EditorController::OnDraw3D()
{
    Siege::Renderer3D::SetGridEnabled(isGridActive);

    if (selectedEntity)
    {
        float extent = 10.f;
        const Siege::Vec3& entityPos = selectedEntity->GetPosition();
        Siege::Renderer3D::DrawLine(entityPos,
                                    entityPos + Siege::Vec3::Right() * extent,
                                    {255, 0, 0, 255});
        Siege::Renderer3D::DrawLine(entityPos,
                                    entityPos + Siege::Vec3::Up() * extent,
                                    {0, 255, 0, 255});
        Siege::Renderer3D::DrawLine(entityPos,
                                    entityPos + Siege::Vec3::Forward() * extent,
                                    {0, 0, 255, 255});
    }
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
        case POSITION:
        {
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
        case ROTATION:
        {
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
    Siege::Xform xform = selectedEntity->GetTransform();
    xform.SetRotationY(rotation);
    selectedEntity->SetTransform(xform);
    return true;
}

void EditorController::SetIsHandlingInput(bool state)
{
    isHandlingInput = state;
}
