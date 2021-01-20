#include "Gizmo.h"
#include "EditorController.h"

void Gizmo::OnDraw()
{
    if (!isActive) return;

    // Draw gizmo display to its location
    DrawLine3D(position, position + raylib::Vector3(extents, 0.f, 0.f), RED);
    DrawLine3D(position, position + raylib::Vector3(0.f, extents, 0.f), GREEN);
    DrawLine3D(position, position + raylib::Vector3(0.f, 0.f, extents), BLUE);
}

void Gizmo::SetActive(bool active)
{
    isActive = active;
}
