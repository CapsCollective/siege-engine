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

        glm::vec3 position = {0.f, 0.f, 0.f};
        auto& scale = gridData.gridScale;

        float rightOffset = (gridData.rows / 2.0) - (scale.x * 0.5f);
        float upOffset = (gridData.columns / 2.0) - (scale.y * 0.5f);

        DrawFirstGridQuad({rightOffset, upOffset, position.z});

        auto& indexCount = gridData.indexCount;
        auto& vertexCount = gridData.vertexCount;

        auto& indices = gridData.indices;
        auto& vertices = gridData.vertices;

        u32& topRightIndex = gridData.nextTopRightIndex;
        u32& bottomRightIndex = gridData.nextBottomRightIndex;

        glm::vec3 topRightVertexPosition = vertices[topRightIndex];
        glm::vec3 bottomRightVertexPosition = vertices[bottomRightIndex];

        //DrawWireQuad(1);

        u32 currentIndex = indexCount-1;

        indices[indexCount] = topRightIndex + 3;
        indices[indexCount+1] = bottomRightIndex + 3;
        indices[indexCount+2] = bottomRightIndex;

        indexCount += 3;
        currentIndex++;

        topRightIndex = topRightIndex + 3;
        bottomRightIndex = bottomRightIndex + 3;

        topRightVertexPosition.x += scale.x;
        bottomRightVertexPosition.x += scale.x;

        vertices[vertexCount++] = topRightVertexPosition;
        vertices[vertexCount++] = bottomRightVertexPosition;

        for(size_t i = 2; i < gridData.rows; i++)
        {
            std::cout << "ROW " << i << std::endl;
            bool isEven = i % 2 == 0;

            std::cout << topRightIndex << std::endl;
            std::cout << bottomRightIndex << std::endl;

            indices[indexCount] = isEven ? bottomRightIndex + 2 : topRightIndex + 2;
            indices[indexCount+1] = isEven ? topRightIndex + 2 : bottomRightIndex + 2;
            indices[indexCount+2] = isEven ? topRightIndex : bottomRightIndex;

            std::cout << indices[indexCount] << std::endl;
            std::cout << indices[indexCount+1] << std::endl;
            std::cout << indices[indexCount+2] << std::endl;

            indexCount += 3;

            currentIndex += 2;

            topRightIndex = topRightIndex + 2;
            bottomRightIndex = bottomRightIndex + 2;

            std::cout << "TOP RIGHT " << topRightIndex << std::endl;
            std::cout << "BOTTOM RIGHT " << bottomRightIndex << std::endl;

            topRightVertexPosition.x += scale.x;
            bottomRightVertexPosition.x += scale.x;

            vertices[vertexCount++] = topRightVertexPosition;
            vertices[vertexCount++] = bottomRightVertexPosition;
        }

        rectModel.UpdateMesh(
            {
                sizeof(glm::vec3),
                vertices,
                vertexCount,
                indices,
                indexCount
            }
        );

        rectModel.Bind(commandBuffer);
        rectModel.Draw(commandBuffer, 0);
    }

    void Renderer3D::DrawFirstGridQuad(const glm::vec3& offset)
    {
        glm::vec3 position = {0.f, 0.f, 0.f};

        auto& scale = gridData.gridScale;

        float rightOffset = (gridData.rows / 2.0) - (scale.x * 0.5f);

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

        gridData.latestIndex = gridData.indexCount - 1;
    }

    void Renderer3D::DrawFirstRow()
    {

    }

    void Renderer3D::DrawWireQuad(const u32 baseIndex)
    {
        u32& topRightIndex = gridData.nextTopRightIndex;
        u32& bottomRightIndex = gridData.nextBottomRightIndex;

        glm::vec3 topRightVertexPosition = gridData.vertices[topRightIndex];
        glm::vec3 bottomRightVertexPosition = gridData.vertices[bottomRightIndex];

        gridData.indices[gridData.indexCount] = baseIndex;
        gridData.indices[gridData.indexCount+1] = baseIndex+1;
        gridData.indices[gridData.indexCount+2] = bottomRightIndex;

        gridData.indexCount += 3;
        gridData.latestIndex++;

        gridData.nextTopRightIndex = gridData.indices[gridData.indexCount - 3];
        gridData.nextBottomRightIndex = gridData.indices[gridData.indexCount - 2];

        topRightVertexPosition.x += gridData.gridScale.x;
        bottomRightVertexPosition.x += gridData.gridScale.x;

        gridData.vertices[gridData.vertexCount++] = topRightVertexPosition;
        gridData.vertices[gridData.vertexCount++] = bottomRightVertexPosition;
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
