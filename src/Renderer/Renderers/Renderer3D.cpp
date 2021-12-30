#include "Renderer3D.h"
#include <iostream>

namespace SnekVk
{
    // static initialisation
    Utils::StringId Renderer3D::transformId;
    Utils::StringId Renderer3D::lightId;
    Utils::StringId Renderer3D::cameraDataId;

    u64 Renderer3D::transformSize = sizeof(Model::Transform) * MAX_OBJECT_TRANSFORMS;

    Utils::StackArray<Model::Transform, Renderer3D::MAX_OBJECT_TRANSFORMS> Renderer3D::transforms;
    Utils::StackArray<Model*, Renderer3D::MAX_OBJECT_TRANSFORMS> Renderer3D::models;

    Material* Renderer3D::currentMaterial = nullptr;
    Model* Renderer3D::currentModel = nullptr;

    void Renderer3D::Initialise()
    {
        transformId = INTERN_STR("objectBuffer");
        lightId = INTERN_STR("lightData");
        cameraDataId = INTERN_STR("cameraData");
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
    {
        models.Append(model);

        auto transform = Utils::Math::CalculateTransform(position, rotation, scale);
        auto normal = Utils::Math::CalculateNormalMatrix(rotation, scale);
        transforms.Append({transform, normal});
    }

    void Renderer3D::DrawModel(Model* model, const Model::Transform& transform)
    {
        models.Append(model);
        transforms.Append({transform.transform, transform.normalMatrix});
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale)
    {
        DrawModel(model, position, scale, glm::vec3{0.f});
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position)
    {
        DrawModel(model, position, glm::vec3{0.f}, glm::vec3{0.f});
    }

    void Renderer3D::Render(VkCommandBuffer& commandBuffer, PointLight& light, Camera* camera)
    {
        if (models.Count() == 0) return;

        VkDeviceSize dirToLightBufferSize = sizeof(PointLight::Data);
        VkDeviceSize cameraDataBufferSize = sizeof(glm::mat4);

        auto projectionView = camera->GetProjView();

        u32 modelCount = 0;
        for (auto& model : models)
        {
            if (currentMaterial != model->GetMaterial())
            {
                currentMaterial = model->GetMaterial();
                currentMaterial->SetUniformData(transformId, transformSize, transforms.Data());
                currentMaterial->SetUniformData(lightId, dirToLightBufferSize, &light.GetLightData());
                currentMaterial->SetUniformData(cameraDataId, cameraDataBufferSize, &projectionView);
                currentMaterial->Bind(commandBuffer);
            } 

            if (currentModel != model)
            {
                currentModel = model;
                currentModel->Bind(commandBuffer);
            }

            model->Draw(commandBuffer, modelCount);
            modelCount++;
        }

        currentModel = nullptr;
        currentMaterial = nullptr;
    }

    void Renderer3D::Flush()
    {
        transforms.Clear();
        models.Clear();
    }
}
