#include "BillboardRenderer.h"

namespace SnekVk
{
    BillboardRenderer::BillboardRenderer() {}
    BillboardRenderer::~BillboardRenderer() {}

    void BillboardRenderer::Initialise(const char* globalDataAttributeName, const u64& globalDataSize)
    {
        globalDataId = INTERN_STR(globalDataAttributeName);
        positionsId = INTERN_STR("positions");

        auto vertexShader = Shader::BuildShader()
            .FromShader("shaders/billboard.vert.spv")
            .WithStage(PipelineConfig::VERTEX)
            .WithVertexType(sizeof(BillboardVertex))
            .WithVertexAttribute(offsetof(BillboardVertex, position), SnekVk::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(BillboardVertex, colour), SnekVk::VertexDescription::VEC4)
            .WithUniform(0, globalDataAttributeName, globalDataSize)
            .WithStorage(1, "positions", sizeof(BillboardUBO), 1000);
        
        auto fragmentShader = Shader::BuildShader()
            .FromShader("shaders/billboard.frag.spv")
            .WithStage(PipelineConfig::FRAGMENT);
        
        billboardMaterial.SetVertexShader(&vertexShader);
        billboardMaterial.SetFragmentShader(&fragmentShader);
        billboardMaterial.BuildMaterial();

        billboardModel.SetMesh({
            sizeof(BillboardVertex),
            nullptr,
            0,
            nullptr,
            0
        });

        billboardModel.SetMaterial(&billboardMaterial);
    }

    void BillboardRenderer::RecreateMaterials()
    {
        billboardMaterial.RecreatePipeline();
    };

    void BillboardRenderer::Destroy()
    {
        billboardMaterial.DestroyMaterial();
        billboardModel.DestroyModel();
    }

    void BillboardRenderer::DrawBillboard(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& colour) 
    {
        vertices.Append({{1.f, 1.f, 1.f}, colour});
        vertices.Append({{1.f, -1.f, 1.f}, colour});
        vertices.Append({{-1.f, -1.f, 1.f}, colour});
        vertices.Append({{-1.f, 1.f, 1.f}, colour});

        // Pattern: 0, 1, 3, 1, 2, 3
        indices.Append(vertices.Count()-4);
        indices.Append(vertices.Count()-3);
        indices.Append(vertices.Count()-1);

        indices.Append(vertices.Count()-3);
        indices.Append(vertices.Count()-2);
        indices.Append(vertices.Count()-1);

        positions.Append({position, glm::vec3(scale, 0.f)});
    }

    void BillboardRenderer::Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData)
    {
        if (vertices.Count() == 0) return;

        billboardMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
        billboardMaterial.SetUniformData(positionsId, sizeof(positions[0]) * positions.Count(), positions.Data());
        billboardMaterial.Bind(commandBuffer);

        billboardModel.UpdateMesh({
            sizeof(BillboardVertex),
            vertices.Data(),
            static_cast<u32>(vertices.Count()),
            indices.Data(),
            static_cast<u32>(indices.Count())
        });

        billboardModel.Bind(commandBuffer);
        billboardModel.Draw(commandBuffer);
    }

    void BillboardRenderer::Flush()
    {
        vertices.Clear();
        indices.Clear();
        positions.Clear();
    }
}