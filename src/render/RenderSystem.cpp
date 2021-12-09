#include "RenderSystem.h"
#include "../resource/ResourceManager.h"
#include "../utils/TransitionAdapter.h"

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

        // Set model texture
        SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);

        // Draw the model
        DrawModelEx(model, FromVec3(item.transform.GetPosition()), FromVec3(Vec3::Up),
                    item.transform.GetRotation(), FromVec3(item.transform.GetScale()), WHITE);
        DrawModelWiresEx(model, FromVec3(item.transform.GetPosition()), FromVec3(Vec3::Up),
                         item.transform.GetRotation(), FromVec3(item.transform.GetScale()), PINK);
    }
}

void RenderSystem::Remove(Entity* entity)
{
    auto it = renderItems.find(EntityPtr<Entity>(entity));
    if (it != renderItems.end()) renderItems.erase(it);
}

void RenderSystem::DrawText2D(const std::string& text, int posX, int posY, int fontSize, Colour color)
{
    DrawText(text.c_str(), posX, posY, fontSize, FromColour(color));
}

void RenderSystem::DrawRectangle2D(int posX, int posY, int width, int height, Colour color)
{
    DrawRectangle(posX, posY, width, height, FromColour(color));
}
