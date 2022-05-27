#include "LightRenderer.h"

namespace SnekVk
{
    LightRenderer::LightRenderer() {}
    LightRenderer::~LightRenderer() {}

    void LightRenderer::Initialise(const char* globalDataAttributeName, const u64& globalDataSize)
    {
        globalDataId = INTERN_STR(globalDataAttributeName);
        lightDataId = INTERN_STR("lightUBO");

        auto pointLightVertShader = SnekVk::Shader::BuildShader()
            .FromShader("shaders/pointLight.vert.spv")
            .WithStage(SnekVk::PipelineConfig::VERTEX)
            .WithVertexType(sizeof(glm::vec2))
            .WithVertexAttribute(0, SnekVk::VertexDescription::VEC2)
            .WithUniform(0, globalDataAttributeName, globalDataSize);

        auto pointLightFragShader = SnekVk::Shader::BuildShader()
            .FromShader("shaders/pointLight.frag.spv")
            .WithStage(SnekVk::PipelineConfig::FRAGMENT)
            .WithUniform(0, globalDataAttributeName, globalDataSize);
        
        lightMaterial.SetVertexShader(&pointLightVertShader);
        lightMaterial.SetFragmentShader(&pointLightFragShader);
        lightMaterial.BuildMaterial();

        lightModel.SetMesh({ 
            sizeof(glm::vec2),
            nullptr,
            0,
            nullptr,
            0
        });

        lightModel.SetMaterial(&lightMaterial);
    }

    void LightRenderer::Destroy()
    {
        lightMaterial.DestroyMaterial();
        lightModel.DestroyModel();
    }

    void LightRenderer::DrawPointLight(const glm::vec3& position, const float& radius, const glm::vec4& colour, const glm::vec4& ambientColor)
    {
        pointLightVertices.Append({1.f, 1.f});
        pointLightVertices.Append({1.f, -1.f});
        pointLightVertices.Append({-1.f, -1.f});
        pointLightVertices.Append({-1.f, 1.f});

        // Pattern: 0, 1, 3, 1, 2, 3
        pointLightIndices.Append(pointLightVertices.Count()-4);
        pointLightIndices.Append(pointLightVertices.Count()-3);
        pointLightIndices.Append(pointLightVertices.Count()-1);

        pointLightIndices.Append(pointLightVertices.Count()-3);
        pointLightIndices.Append(pointLightVertices.Count()-2);
        pointLightIndices.Append(pointLightVertices.Count()-1);
    }

    void LightRenderer::Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData)
    {
        if (pointLightVertices.Count() == 0) return;

        lightMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
        lightMaterial.Bind(commandBuffer);

        lightModel.UpdateMesh({
            sizeof(glm::vec2),
            pointLightVertices.Data(),
            static_cast<u32>(pointLightVertices.Count()),
            pointLightIndices.Data(),
            static_cast<u32>(pointLightIndices.Count())
        });

        lightModel.Bind(commandBuffer);
        lightModel.Draw(commandBuffer, 0);
    }

    void LightRenderer::Flush()
    {
        pointLightVertices.Clear();
        pointLightIndices.Clear();
    }

    void LightRenderer::RecreateMaterials()
    {
        lightMaterial.RecreatePipeline();
    }
}