#include "RenderSystem.h"
#include "../resource/ResourceManager.h"
#include <raylib/raylib-cpp.hpp>

std::vector<RenderItem> RenderSystem::renderItems;

void RenderSystem::Add(Entity* entity, const std::string& modelPath, const std::string& texturePath)
{
    ResourceManager::Register<Model>(modelPath);
    ResourceManager::Register<Texture>(texturePath);
    renderItems.emplace_back(entity, modelPath, texturePath);
}

void RenderSystem::DrawFrame()
{
    for (const auto& item : renderItems)
    {
        const Model& model = ResourceManager::Get<Model>(item.modelData.modelPath);
        const Texture& texture = ResourceManager::Get<Texture>(item.modelData.texturePath);

        // Set model texture using raylib
        model.materials[0].maps[MAP_DIFFUSE].texture = texture;

        // Draw the model
        DrawModelEx(model, item.entity->GetPosition(), Vec3::Up, item.entity->GetRotation(), item.entity->GetScale(), WHITE);
        DrawModelWiresEx(model, item.entity->GetPosition(), Vec3::Up, item.entity->GetRotation(), item.entity->GetScale(), PINK);
    }
}

RenderItem* RenderSystem::Get(Entity* entity)
{
    auto it = std::find_if(renderItems.begin(), renderItems.end(), [&entity] (const RenderItem& r) {
        return r.entity == entity;
    });
    return it != renderItems.end() ? &(*it) : nullptr;
}

void RenderSystem::Remove(Entity* entity)
{
    auto it = std::find_if(renderItems.begin(), renderItems.end(), [&entity] (const RenderItem& r) {
        return r.entity == entity;
    });
    if (it != renderItems.end()) renderItems.erase(it);
}
