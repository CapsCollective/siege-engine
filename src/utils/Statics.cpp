#include "Statics.h"

#include "../input/Input.h"
#include "../physics/CollisionSystem.h"
#include "../render/RenderSystem.h"
#include "../render/ResourceManager.h"
#include "../scene/SceneManager.h"

CollisionSystem Statics::CollisionSystem;

Input Statics::Input;

ResourceManager Statics::ResourceManager;

RenderSystem Statics::RenderSystem;

SceneManager Statics::SceneManager;