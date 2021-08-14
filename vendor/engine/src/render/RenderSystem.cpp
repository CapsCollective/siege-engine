#include "RenderSystem.h"
#include "../resource/ResourceManager.h"

std::map<EntityPtr<Entity>, RenderItem> RenderSystem::renderItems;

RenderItem* RenderSystem::Add(Entity* entity, const ModelData& modelData)
{
    return Add(entity, modelData, entity->GetTransform());
}

RenderItem* RenderSystem::Add(Entity* entity, const ModelData& modelData, const Xform& transform)
{
    ResourceManager::Register<Model>(modelData.modelPath);
    ResourceManager::Register<Texture>(modelData.texturePath);
    renderItems.emplace(entity, RenderItem(modelData, transform));
    return nullptr;
}

void RenderSystem::DrawFrame()
{
    for (const auto& pair : renderItems)
    {
        const auto& item = pair.second;
        if (!item.isEnabled) continue;

        const Model& model = ResourceManager::Get<Model>(item.data.modelPath);
        const Texture& texture = ResourceManager::Get<Texture>(item.data.texturePath);

        // Set model texture using raylib
        model.materials[0].maps[MAP_DIFFUSE].texture = texture;

        // Draw the model
        DrawModelEx(model, item.transform.GetPosition(), Vec3::Up,
                    item.transform.GetRotation(), item.transform.GetScale(), WHITE);
        DrawModelWiresEx(model, item.transform.GetPosition(), Vec3::Up,
                         item.transform.GetRotation(), item.transform.GetScale(), PINK);
    }
}

void RenderSystem::Remove(Entity* entity)
{
    auto it = renderItems.find(EntityPtr<Entity>(entity));
    if (it != renderItems.end()) renderItems.erase(it);
}

void RenderSystem::DrawFrame2D()
{
    // TODO implement me
}
