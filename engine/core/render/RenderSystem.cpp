#include "RenderSystem.h"

#include <raylib/raylib-cpp.hpp>

#include "../TransitionAdapter.h"
#include "ResourceSystem.h"

void RenderSystem::Add(Entity* entity, const ModelData& modelData)
{
    Add(entity, modelData, entity->GetTransform());
}

void RenderSystem::Add(Entity* entity, const ModelData& modelData, const Xform& transform)
{
    Statics::Resource().RegisterModel(modelData.modelPath);
    Statics::Resource().RegisterTexture(modelData.texturePath);
    renderItems.emplace(entity, RenderItem(modelData, transform));
}

void RenderSystem::DrawFrame()
{
    for (const auto& pair : renderItems)
    {
        const auto& item = pair.second;
        if (!item.isEnabled) continue;

        Model model = *((Model*) Statics::Resource().GetModel(item.data.modelPath));
        Texture texture = *((Texture*) Statics::Resource().GetTexture(item.data.texturePath));

        // Set model texture
        SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE, texture);

        // Draw the model
        DrawModelEx(model,
                    FromVec3(item.transform.GetPosition()),
                    FromVec3(Vec3::Up),
                    item.transform.GetRotation(),
                    FromVec3(item.transform.GetScale()),
                    WHITE);
        DrawModelWiresEx(model,
                         FromVec3(item.transform.GetPosition()),
                         FromVec3(Vec3::Up),
                         item.transform.GetRotation(),
                         FromVec3(item.transform.GetScale()),
                         PINK);
    }
}

void RenderSystem::Remove(Entity* entity)
{
    auto it = renderItems.find(EntityPtr<Entity>(entity));
    if (it != renderItems.end()) renderItems.erase(it);
}

void RenderSystem::DrawText2D(const String& text, int posX, int posY, int fontSize, Colour color)
{
    DrawText(text, posX, posY, fontSize, FromColour(color));
}

void RenderSystem::DrawRectangle2D(int posX, int posY, int width, int height, Colour color)
{
    DrawRectangle(posX, posY, width, height, FromColour(color));
}
