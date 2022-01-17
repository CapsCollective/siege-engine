#include "Renderer3D.h"
#include <iostream>

namespace SnekVk
{
    // static initialisation
    Utils::StringId Renderer3D::transformId;
    Utils::StringId Renderer3D::globalDataId;

    u64 Renderer3D::transformSize = sizeof(Model::Transform) * MAX_OBJECT_TRANSFORMS;

    Utils::StackArray<Model::Transform, Renderer3D::MAX_OBJECT_TRANSFORMS> Renderer3D::transforms;
    Utils::StackArray<Model*, Renderer3D::MAX_OBJECT_TRANSFORMS> Renderer3D::models;

    Material* Renderer3D::currentMaterial = nullptr;
    Model* Renderer3D::currentModel = nullptr;

    Model* Renderer3D::lightModel = nullptr;

    Material Renderer3D::gridMaterial;

    bool Renderer3D::gridEnabled = false;

    DebugRenderer3D Renderer3D::debugRenderer;
    BillboardRenderer Renderer3D::billboardRenderer;

    void Renderer3D::Initialise()
    {
        transformId = INTERN_STR("objectBuffer");
        globalDataId = INTERN_STR("globalData");

        debugRenderer.Initialise("globalData", sizeof(GlobalData));
        billboardRenderer.Initialise("globalData", sizeof(GlobalData));
        
        auto gridShader = SnekVk::Shader::BuildShader()
            .FromShader("shaders/grid.vert.spv")
            .WithStage(SnekVk::PipelineConfig::VERTEX)
            .WithUniform(0, "globalData", sizeof(SnekVk::Renderer3D::GlobalData));
        
        auto gridFragShader = SnekVk::Shader::BuildShader()
            .FromShader("shaders/grid.frag.spv")
            .WithStage(SnekVk::PipelineConfig::FRAGMENT);

        gridMaterial.SetVertexShader(&gridShader);
        gridMaterial.SetFragmentShader(&gridFragShader);
        gridMaterial.BuildMaterial();
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
    {
        models.Append(model);

        auto transform = Utils::Math::CalculateTransform3D(position, rotation, scale);
        auto normal = Utils::Math::CalculateNormalMatrix(rotation, scale);
        transforms.Append({transform, normal});
    }

    void Renderer3D::DrawBillboard(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& colour)
    {
        billboardRenderer.DrawBillboard(position, scale, colour);
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale)
    {
        DrawModel(model, position, scale, glm::vec3{0.f});
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position)
    {
        DrawModel(model, position, glm::vec3{1.f}, glm::vec3{0.f});
    }

    void Renderer3D::DrawLight(Model* model)
    {
        lightModel = model;
    }

    void Renderer3D::Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        if (models.Count() == 0) return;

        u64 globalDataSize = sizeof(GlobalData);

        RenderModels(commandBuffer, globalData);
        RenderLights(commandBuffer, globalData);

        debugRenderer.Render(commandBuffer, globalDataSize, &globalData);
        billboardRenderer.Render(commandBuffer, globalDataSize, &globalData);
        
        if (gridEnabled) RenderGrid(commandBuffer, globalData);

        currentModel = nullptr;
        currentMaterial = nullptr;
    }

    void Renderer3D::DrawLine(const glm::vec3& origin, const glm::vec3& destination, const glm::vec3& colour)
    {
        debugRenderer.DrawLine(origin, destination, colour);
    }

    void Renderer3D::RenderModels(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        for (size_t i = 0; i < models.Count(); i++)
        {
            auto& model = models.Get(i);

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
    }
    
    void Renderer3D::RenderLights(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        if (lightModel == nullptr) return; 

        auto lightMaterial = lightModel->GetMaterial();

        if (lightMaterial == nullptr) return;

        lightMaterial->SetUniformData(globalDataId, sizeof(globalData), &globalData);
        lightMaterial->Bind(commandBuffer);

        lightModel->Bind(commandBuffer);
        lightModel->Draw(commandBuffer, 0);
    }

    void Renderer3D::RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        gridMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
        gridMaterial.Bind(commandBuffer);

        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }

    void Renderer3D::EnableGrid()
    {
        gridEnabled = true;
    }

    void Renderer3D::RecreateMaterials()
    {
        if (currentMaterial) currentMaterial->RecreatePipeline(); 
        debugRenderer.RecreateMaterials();
        billboardRenderer.RecreateMaterials();
        gridMaterial.RecreatePipeline();
    }

    void Renderer3D::Flush()
    {
        debugRenderer.Flush();
        billboardRenderer.Flush();
        transforms.Clear();
        models.Clear();
    }

    void Renderer3D::DestroyRenderer3D()
    {
        debugRenderer.Destroy();
        billboardRenderer.Destroy();
        gridMaterial.DestroyMaterial();
    }
}
