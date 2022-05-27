#include "Renderer3D.h"
#include <iostream>

namespace SnekVk
{
    // static initialisation
    Utils::StringId Renderer3D::globalDataId;

    Material Renderer3D::gridMaterial;

    ModelRenderer Renderer3D::modelRenderer;
    DebugRenderer3D Renderer3D::debugRenderer;
    BillboardRenderer Renderer3D::billboardRenderer;
    LightRenderer Renderer3D::lightRenderer;

    Renderer3D::GlobalData Renderer3D::global3DData;

    void Renderer3D::Initialise()
    {
        globalDataId = INTERN_STR("globalData");

        modelRenderer.Initialise("globalData", sizeof(GlobalData));
        debugRenderer.Initialise("globalData", sizeof(GlobalData));
        billboardRenderer.Initialise("globalData", sizeof(GlobalData));
        lightRenderer.Initialise("globalData", sizeof(GlobalData));
        
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

    void Renderer3D::DrawBillboard(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& colour)
    {
        billboardRenderer.DrawBillboard(position, scale, colour);
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
    {
        modelRenderer.DrawModel(model, position, scale, rotation);
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale)
    {
        DrawModel(model, position, scale, glm::vec3{0.f});
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position)
    {
        DrawModel(model, position, glm::vec3{1.f}, glm::vec3{0.f});
    }

    void Renderer3D::DrawPointLight(const glm::vec3& position, const float& radius, const glm::vec4& colour, const glm::vec4& ambientColor)
    {   
        global3DData.lightData = {colour, ambientColor, position};

        lightRenderer.DrawPointLight(position, 0.05f, colour, ambientColor);
    }

    void Renderer3D::Render(VkCommandBuffer& commandBuffer, const CameraData& cameraData)
    {
        global3DData.cameraData = cameraData;
        u64 globalDataSize = sizeof(global3DData);

        modelRenderer.Render(commandBuffer, globalDataSize, &global3DData);
        lightRenderer.Render(commandBuffer, globalDataSize, &global3DData);
        debugRenderer.Render(commandBuffer, globalDataSize, &global3DData);
        billboardRenderer.Render(commandBuffer, globalDataSize, &global3DData);
        
        #ifdef ENABLE_GRID
        RenderGrid(commandBuffer, global3DData);
        #endif
    }

    void Renderer3D::DrawLine(const glm::vec3& origin, const glm::vec3& destination, const glm::vec3& colour)
    {
        debugRenderer.DrawLine(origin, destination, colour);
    }

    void Renderer3D::RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        gridMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
        gridMaterial.Bind(commandBuffer);

        vkCmdDraw(commandBuffer, 6, 1, 0, 0);
    }

    void Renderer3D::RecreateMaterials()
    {
        modelRenderer.RecreateMaterials();
        debugRenderer.RecreateMaterials();
        billboardRenderer.RecreateMaterials();
        lightRenderer.RecreateMaterials();

        gridMaterial.RecreatePipeline();
    }

    void Renderer3D::Flush()
    {
        modelRenderer.Flush();
        debugRenderer.Flush();
        billboardRenderer.Flush();
        lightRenderer.Flush();
    }

    void Renderer3D::DestroyRenderer3D()
    {
        modelRenderer.Destroy();
        debugRenderer.Destroy();
        billboardRenderer.Destroy();
        lightRenderer.Destroy();

        gridMaterial.DestroyMaterial();
    }
}
