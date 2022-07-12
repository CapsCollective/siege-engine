//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ModelRenderer.h"

namespace Siege
{
ModelRenderer::ModelRenderer() {}
ModelRenderer::~ModelRenderer() {}

void ModelRenderer::Initialise(const String& globalDataAttributeName,
                               const uint64_t& globalDataSize)
{
    globalDataId = INTERN_STR(globalDataAttributeName);
    transformId = INTERN_STR("objectBuffer");
}

void ModelRenderer::Destroy() {}

void ModelRenderer::DrawModel(Model* model,
                              const Siege::Vec3& position,
                              const Siege::Vec3& scale,
                              const Siege::Vec3& rotation)
{
    models.Append(model);

    auto transform = Math::CalculateTransform3D(position, rotation, scale);
    auto normal = Math::CalculateNormalMatrix(rotation, scale);
    transforms.Append({transform, normal});
}

void ModelRenderer::Render(VkCommandBuffer& commandBuffer,
                           const uint64_t& globalDataSize,
                           const void* globalData)
{
    if (models.Count() == 0) return;

    for (size_t i = 0; i < models.Count(); i++)
    {
        auto& model = models.Get(i);

        if (currentMaterial != model->GetMaterial())
        {
            currentMaterial = model->GetMaterial();
            currentMaterial->SetUniformData(transformId,
                                            sizeof(transforms[0]) * transforms.Count(),
                                            transforms.Data());
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

void ModelRenderer::Flush()
{
    transforms.Clear();
    models.Clear();
}

void ModelRenderer::RecreateMaterials()
{
    if (currentMaterial) currentMaterial->RecreatePipeline();
}
} // namespace Siege
