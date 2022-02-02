#include "Statics.h"

#include "../input/InputSystem.h"
#include "../physics/CollisionSystem.h"
#include "../render/RenderSystem.h"
#include "../render/ResourceManager.h"
#include "../scene/SceneManager.h"

CollisionSystem& Statics::Collision()
{
    static CollisionSystem system;
    return system;
}

InputSystem& Statics::Input()
{
    static InputSystem system;
    return system;
}

ResourceManager& Statics::Resource()
{
    static ResourceManager system;
    return system;
}

RenderSystem& Statics::Render()
{
    static RenderSystem system;
    return system;
}

SceneManager& Statics::Scene()
{
    static SceneManager system;
    return system;
}
