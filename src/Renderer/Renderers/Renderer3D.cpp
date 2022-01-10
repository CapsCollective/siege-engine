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

    Material Renderer3D::lineMaterial;
    Model Renderer3D::lineModel;
    Renderer3D::LineData Renderer3D::lineData;

    Material Renderer3D::rectMaterial;
    Model Renderer3D::rectModel;

    Utils::StackArray<glm::vec3, Mesh::MAX_VERTICES> Renderer3D::lines;
    Utils::StackArray<glm::vec3, Mesh::MAX_VERTICES> Renderer3D::rects;

    Renderer3D::GridData Renderer3D::gridData;

    void Renderer3D::Initialise()
    {
        transformId = INTERN_STR("objectBuffer");
        globalDataId = INTERN_STR("globalData");

        auto vertexShader = Shader::BuildShader()
            .FromShader("shaders/line.vert.spv")
            .WithStage(PipelineConfig::VERTEX)
            .WithVertexType(sizeof(glm::vec3))
            .WithVertexAttribute(0, SnekVk::VertexDescription::VEC3)
            .WithUniform(0, "globalData", sizeof(GlobalData), 1)
            .WithUniform(1, "lineData", sizeof(LineData), 1);
        
        auto fragmentShader = Shader::BuildShader()
            .FromShader("shaders/line.frag.spv")
            .WithStage(PipelineConfig::FRAGMENT);
        
        lineMaterial.SetVertexShader(&vertexShader);
        lineMaterial.SetFragmentShader(&fragmentShader);
        lineMaterial.SetTopology(Material::Topology::LINE_LIST);
        lineMaterial.BuildMaterial();

        rectMaterial.SetVertexShader(&vertexShader);
        rectMaterial.SetFragmentShader(&fragmentShader);
        rectMaterial.SetTopology(Material::Topology::LINE_STRIP);
        rectMaterial.BuildMaterial();

        lineModel.SetMesh({
            sizeof(glm::vec3),
            nullptr,
            0,
            nullptr,
            0
        });

        rectModel.SetMesh({
            sizeof(glm::vec3),
            nullptr,
            0,
            nullptr,
            0
        });

        lineModel.SetMaterial(&lineMaterial);
        rectModel.SetMaterial(&rectMaterial);
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation)
    {
        models.Append(model);

        auto transform = Utils::Math::CalculateTransform3D(position, rotation, scale);
        auto normal = Utils::Math::CalculateNormalMatrix(rotation, scale);
        transforms.Append({transform, normal});
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale)
    {
        DrawModel(model, position, scale, glm::vec3{0.f});
    }

    void Renderer3D::DrawModel(Model* model, const glm::vec3& position)
    {
        DrawModel(model, position, glm::vec3{1.f}, glm::vec3{0.f});
    }

    void Renderer3D::DrawBillboard(Model* model, const glm::vec3& position, const glm::vec2& scale, const float& rotation)
    {
        
    }

    void Renderer3D::DrawLight(Model* model)
    {
        lightModel = model;
    }

    void Renderer3D::Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        if (models.Count() == 0) return;

        RenderModels(commandBuffer, globalData);
        RenderLights(commandBuffer, globalData);
        RenderLines(commandBuffer, globalData);
        //RenderRects(commandBuffer, globalData);
        if (gridData.enabled) RenderGrid(commandBuffer, globalData);

        currentModel = nullptr;
        currentMaterial = nullptr;
    }

    void Renderer3D::DrawLine(const glm::vec3& origin, const glm::vec3& destination, glm::vec3 color)
    {
        lines.Append(origin);
        lines.Append(destination);

        lineData.color = color;
    }

    void Renderer3D::DrawGrid(size_t rowCount, size_t columnCount, glm::vec2 scale)
    {
        gridData.rows = rowCount; 
        gridData.columns = columnCount;
        gridData.gridScale = scale;
        gridData.enabled = true;
    }

    void Renderer3D::DrawRect(const glm::vec3& position, const glm::vec2& scale, glm::vec3 color)
    {
        rects.Append({position.x - (scale.x / 2.0), position.y + (scale.y / 2.0), position.z});
        rects.Append({position.x + (scale.x / 2.0), position.y + (scale.y / 2.0), position.z});
        rects.Append({position.x + (scale.x / 2.0), position.y - (scale.y / 2.0), position.z});
        rects.Append({position.x - (scale.x / 2.0), position.y - (scale.y / 2.0), position.z});

        lineData.color = color;
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
    
    // TODO(Aryeh): This is rendering a billboard. Might want to defer this to a billboard renderer
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

    void Renderer3D::RenderLines(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        lineMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
        lineMaterial.SetUniformData("lineData", sizeof(LineData), &lineData);
        lineMaterial.Bind(commandBuffer);

        lineModel.UpdateMesh(
            {
                sizeof(glm::vec3),
                lines.Data(),
                static_cast<u32>(lines.Count()),
                nullptr,
                0
            }
        );

        lineModel.Bind(commandBuffer);
        lineModel.Draw(commandBuffer, 0);
    }

    void Renderer3D::RenderRects(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        rectMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
        rectMaterial.SetUniformData("lineData", sizeof(LineData), &lineData);
        rectMaterial.Bind(commandBuffer);

        u32 indices[] = {0, 1, 2, 3, 0};

        rectModel.UpdateMesh(
            {
                sizeof(glm::vec3),
                rects.Data(),
                static_cast<u32>(rects.Count()),
                indices,
                5
            }
        );

        rectModel.Bind(commandBuffer);
        rectModel.Draw(commandBuffer, 0);
    }

    void Renderer3D::RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData)
    {
        rectMaterial.SetUniformData(globalDataId, sizeof(globalData), &globalData);
        rectMaterial.SetUniformData("lineData", sizeof(LineData), &lineData);
        rectMaterial.Bind(commandBuffer);

        DrawFirstGridQuad(
        {
            (gridData.rows / 2.0) - (gridData.gridScale.x * 0.5f), 
            (gridData.columns / 2.0) - (gridData.gridScale.y * 0.5f), 
            0.f
        });

        DrawFirstRow();

        for (size_t i = 1; i < gridData.columns; i++)
        {
            DrawRow(i);
        }

        rectModel.UpdateMesh(
            {
                sizeof(glm::vec3),
                gridData.vertices,
                gridData.vertexCount,
                gridData.indices,
                gridData.indexCount
            }
        );

        rectModel.Bind(commandBuffer);
        rectModel.Draw(commandBuffer, 0);
    }

    void Renderer3D::DrawFirstGridQuad(const glm::vec3& offset)
    {
        glm::vec3 position = {0.f, 0.f, 0.f};

        auto& scale = gridData.gridScale;

        gridData.vertices[0] = {(position.x - (scale.x / 2.0) - offset.x), position.y - (scale.y / 2.0) - offset.y, position.z};
        gridData.vertices[1] = {(position.x + (scale.x / 2.0) - offset.x), position.y - (scale.y / 2.0) - offset.y, position.z};
        gridData.vertices[2] = {(position.x + (scale.x / 2.0) - offset.x), position.y + (scale.y / 2.0) - offset.y, position.z};
        gridData.vertices[3] = {(position.x - (scale.x / 2.0) - offset.x), position.y + (scale.y / 2.0) - offset.y, position.z};

        gridData.vertexCount = 4;

        u32 firstQuadIndices[] = 
        {
            0, 1, 2, 3, 0
        };

        gridData.indexCount = 5;

        memcpy(gridData.indices, firstQuadIndices, sizeof(firstQuadIndices));

        gridData.nextTopRightIndex = gridData.indices[1];
        gridData.nextBottomRightIndex = gridData.indices[2];
    }

    void Renderer3D::DrawFirstRow()
    {   
        AddWireQuad(
        {
            gridData.nextTopRightIndex + 3, 
            gridData.nextBottomRightIndex + 3,
            gridData.nextBottomRightIndex,
        }, 
        {
            gridData.vertices[gridData.nextTopRightIndex],
            gridData.vertices[gridData.nextBottomRightIndex]
        }, 
        {
            gridData.nextTopLeftIndex,
            gridData.nextTopRightIndex + 3,
            gridData.nextBottomRightIndex + 3,
            gridData.nextBottomLeftIndex
        }, 
        {
            gridData.gridScale.x,
            0.f, 
            0.f
        });

        for(size_t i = 2; i < gridData.rows; i++)
        {
            bool isEven = i % 2 == 0;

            AddWireQuad({
                (isEven * gridData.nextBottomRightIndex) + (!isEven * gridData.nextTopRightIndex) + 2,
                (isEven * gridData.nextTopRightIndex) + (!isEven * gridData.nextBottomRightIndex) + 2,
                (isEven * gridData.nextTopRightIndex) + (!isEven * gridData.nextBottomRightIndex)
            }, 
            {
                gridData.vertices[gridData.nextTopRightIndex],
                gridData.vertices[gridData.nextBottomRightIndex]
            }, 
            {
                gridData.nextTopLeftIndex,
                gridData.nextTopRightIndex + 2,
                gridData.nextBottomRightIndex + 2,
                gridData.nextBottomLeftIndex
            }, 
            {
                gridData.gridScale.x,
                0.f, 
                0.f,
            });
        }

        // Reset to first quad for this row.
        gridData.nextTopRightIndex = 1;
        gridData.nextBottomRightIndex = 2;
        gridData.nextBottomLeftIndex = 3;
        gridData.nextTopLeftIndex = 0;
    }

    // TODO(Aryeh): Automate this so that we can put in n number of quads
    void Renderer3D::DrawRow(size_t row)
    {
        u32 start = 0;

        bool isEven = gridData.rows % 2 == 0;
        
        // First Quad
        start = (isEven * gridData.nextBottomLeftIndex) + (!isEven * gridData.nextTopLeftIndex);

        glm::vec3 bottomRightPosition = gridData.vertices[gridData.nextBottomRightIndex];

        AddWireQuad(
        {
            start, 
            (gridData.rows * 2) + 2,
            (gridData.rows * 2) + 3,
            gridData.nextBottomRightIndex
        }, 
        {
            gridData.vertices[gridData.nextBottomLeftIndex],
            gridData.vertices[gridData.nextBottomRightIndex],
        }, 
        {
            gridData.nextTopLeftIndex,
            gridData.nextBottomRightIndex + 3,
            (gridData.rows * 2) + 4,
            (gridData.rows * 2) + 3
        },
        {
            0.f, 
            gridData.gridScale.y,
            0.f
        });

        bottomRightPosition.y += gridData.gridScale.y;

        AddWireQuad(
        {
            gridData.nextTopRightIndex,
            gridData.nextBottomRightIndex,
            gridData.nextBottomLeftIndex, 
        }, 
        {
            bottomRightPosition
        }, 
        {
            gridData.nextTopLeftIndex,
            gridData.nextTopRightIndex + 2,
            gridData.nextBottomRightIndex + 1,
            gridData.nextBottomLeftIndex + 1,
        },
        {
            gridData.gridScale.x, 
            0.f,
            0.f
        });

        bottomRightPosition.x += gridData.gridScale.x;

        AddWireQuad(
        {
            gridData.nextBottomRightIndex,
            gridData.nextTopRightIndex,
        }, 
        {
            bottomRightPosition
        }, 
        {
            gridData.nextTopLeftIndex,
            gridData.nextTopRightIndex + 2,
            gridData.nextBottomRightIndex + 1,
            gridData.nextBottomLeftIndex + 1,
        },
        {
            gridData.gridScale.x, 
            0.f,
            0.f
        });

        bottomRightPosition.x += gridData.gridScale.x;
    }

    void Renderer3D::AddWireQuad(std::initializer_list<u32> indices, std::initializer_list<glm::vec3> vertices, const QuadIndexExtents& extentIndices, const glm::vec3& scale)
    {
        for (auto& index: indices)
        {
            gridData.indices[gridData.indexCount++] = index;
        }

        for (auto& vertex : vertices)
        {
            gridData.vertices[gridData.vertexCount++] = vertex + scale; 
        }

        gridData.nextTopLeftIndex = extentIndices.topLeft;
        gridData.nextTopRightIndex = extentIndices.topRight;
        gridData.nextBottomRightIndex = extentIndices.bottomRight;
        gridData.nextBottomLeftIndex = extentIndices.bottomLeft;
    }

    void Renderer3D::RecreateMaterials()
    {
        if (currentMaterial) currentMaterial->RecreatePipeline(); 
    }

    void Renderer3D::Flush()
    {
        transforms.Clear();
        lines.Clear();
        rects.Clear();
        models.Clear();
        gridData.vertexCount = 0;
        gridData.indexCount = 0;
    }

    void Renderer3D::DestroyRenderer3D()
    {
        lineModel.DestroyModel();
        lineMaterial.DestroyMaterial();
        rectModel.DestroyModel();
        rectMaterial.DestroyMaterial();
    }
}
