#include "Renderer2D.h"

namespace SnekVk
{
    Utils::StringId Renderer2D::transformId;
    Utils::StringId Renderer2D::globalDataId;

    u64 Renderer2D::transformSize = sizeof(Model::Transform2D) * MAX_OBJECT_TRANSFORMS;

    Utils::StackArray<Model::Transform2D, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::transforms;
    Utils::StackArray<Model*, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::models;

    Material* Renderer2D::currentMaterial = nullptr;
    Model* Renderer2D::currentModel = nullptr;

    void Renderer2D::Initialise()
    {
        transformId = INTERN_STR("objectBuffer");
        globalDataId = INTERN_STR("globalData");
    }

    void Renderer2D::DrawModel(Model* model, const glm::vec2& position, const glm::vec2& scale, const float& rotation, const float& zIndex)
    {
        models.Append(model);

        auto transform = Utils::Math::CalculateTransform3D(glm::vec3(position.x, position.y, zIndex), glm::vec3(0.f, 0.f, rotation), glm::vec3(scale.x, scale.y, 0.f));
        transforms.Append({ transform });
    }

    void Renderer2D::DrawModel(Model* model, const glm::vec2& position, const glm::vec2& scale, const float& zIndex)
    {
        DrawModel(model, position, scale, 0.f, zIndex);
    }

    void Renderer2D::DrawModel(Model* model, const glm::vec2& position)
    {
        DrawModel(model, position, glm::vec2(1.f), 0.f, 0.f);
    }

    void Renderer2D::RecreateMaterials()
    {
        if (currentMaterial) currentMaterial->RecreatePipeline(); 
    }

    void Renderer2D::Render(VkCommandBuffer& commandBuffer, void* globalData, u64 globalDataSize)
    {
        if (models.Count() == 0) return;

        for (size_t i = 0; i < models.Count(); i++)
        {
            auto model = models[i];

            if (currentMaterial != model->GetMaterial())
            {
                currentMaterial = model->GetMaterial();
                currentMaterial->SetUniformData(transformId, transformSize, transforms.Data());
                currentMaterial->SetUniformData(globalDataId, globalDataSize, globalData);
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
}