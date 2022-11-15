//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer2D.h"

#include <utils/math/Graphics.h>

namespace Siege
{
Hash::StringId Renderer2D::transformId;
Hash::StringId Renderer2D::globalDataId;

uint64_t Renderer2D::transformSize = sizeof(Model::Transform2D) * MAX_OBJECT_TRANSFORMS;

Utils::MSArray<Model::Transform2D, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::transforms;
Utils::MSArray<Model*, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::models;

Material* Renderer2D::currentMaterial = nullptr;
Vulkan::Material* Renderer2D::currentMaterial2 = nullptr;
Model* Renderer2D::currentModel = nullptr;

void Renderer2D::Initialise()
{
    transformId = INTERN_STR("transforms");
    globalDataId = INTERN_STR("globalData");
}

void Renderer2D::DrawModel(Model* model,
                           const Vec2& position,
                           const Vec2& scale,
                           float rotation,
                           float zIndex)
{
    models.Append(model);

    auto transform = Graphics::CalculateTransform3D({position.x, position.y, zIndex},
                                                    {0.f, 0.f, rotation},
                                                    {scale.x, scale.y, 0.f});
    transforms.Append({transform});
}

void Renderer2D::DrawModel(Model* model, const Vec2& position, const Vec2& scale, float zIndex)
{
    DrawModel(model, position, scale, 0.f, zIndex);
}

void Renderer2D::DrawModel(Model* model, const Vec2& position)
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

        if (currentMaterial2 != model->GetMaterial2())
        {
            currentMaterial2 = model->GetMaterial2();
            currentMaterial2->SetUniformData(transformId, transformSize, transforms.Data());
            currentMaterial2->SetUniformData(globalDataId, sizeof(globalData), &globalData);
            currentMaterial2->Bind(commandBuffer);
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
    currentMaterial2 = nullptr;
}

void Renderer2D::Flush()
{
    models.Clear();
    transforms.Clear();
}
} // namespace Siege
