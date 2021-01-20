#include "Grid.h"

void Grid::OnDraw()
{
    // Draw a reference grid at the centre of the scene if toggled on
    if (isActive) DrawGrid(size, 1.0f);
}

void Grid::SetActive(bool active)
{
    isActive = active;
}

bool Grid::GetActive() const
{
    return isActive;
}
