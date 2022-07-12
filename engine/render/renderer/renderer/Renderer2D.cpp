//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer2D.h"

namespace Siege
{
Hash::StringId Renderer2D::transformId;
Hash::StringId Renderer2D::globalDataId;

uint64_t Renderer2D::transformSize = sizeof(Model::Transform2D) * MAX_OBJECT_TRANSFORMS;

StackArray<Model::Transform2D, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::transforms;
StackArray<Model*, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::models;

Material* Renderer2D::currentMaterial = nullptr;
Model* Renderer2D::currentModel = nullptr;

void Renderer2D::Initialise()
{
    transformId = INTERN_STR("objectBuffer");
    globalDataId = INTERN_STR("globalData");
}

void Renderer2D::DrawModel(Model* model,
                           const Siege::Vec2& position,
                           const Siege::Vec2& scale,
                           const float& rotation,
                           const float& zIndex)
{
    models.Append(model);

    auto transform = Math::CalculateTransform3D(Siege::Vec3(position.x, position.y, zIndex),
                                                Siege::Vec3(0.f, 0.f, rotation),
                                                Siege::Vec3(scale.x, scale.y, 0.f));
    transforms.Append({transform});
}

void Renderer2D::DrawModel(Model* model,
                           const Siege::Vec2& position,
                           const Siege::Vec2& scale,
                           const float& zIndex)
{
    DrawModel(model, position, scale, 0.f, zIndex);
}

void Renderer2D::DrawModel(Model* model, const Siege::Vec2& position)
{
    DrawModel(model, position, {1.f, 1.f}, 0.f, 0.f);
}

void Renderer2D::RecreateMaterials()
{
    if (currentMaterial) currentMaterial->RecreatePipeline();
}

void Renderer2D::Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
{
    if (models.Count() == 0) return;

    for (size_t i = 0; i < models.Count(); i++)
    {
        auto model = models[i];

        if (currentMaterial != model->GetMaterial())
        {
            currentMaterial = model->GetMaterial();
            currentMaterial->SetUniformData(transformId, transformSize, transforms.Data());
            currentMaterial->SetUniformData(globalDataId, sizeof(globalData), &globalData);
            currentMaterial->Bind(commandBuffer);
        }

        if (currentModel != model)
        {
            currentModel = model;
            currentModel->Bind(commandBuffer);
        }

        model->Draw(commandBuffer, i);
    }

    currentModel = nullptr;
    currentMaterial = nullptr;
}

void Renderer2D::Flush()
{
    models.Clear();
    transforms.Clear();
}
} // namespace Siege
