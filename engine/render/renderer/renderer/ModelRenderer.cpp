//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ModelRenderer.h"

#include <utils/math/Graphics.h>

namespace Siege
{
ModelRenderer::ModelRenderer() {}
ModelRenderer::~ModelRenderer() {}

void ModelRenderer::Initialise(const String& globalDataAttributeName)
{
    globalDataId = INTERN_STR(globalDataAttributeName);
    transformId = INTERN_STR("transforms");
}

void ModelRenderer::DrawModel(Model* model,
                              const Vec3& position,
                              const Vec3& scale,
                              const Vec3& rotation)
{
    models.Append(model);

    auto transform = Graphics::CalculateTransform3D(position, rotation, scale);
    auto normal = Graphics::CalculateNormal(rotation, scale);
    transforms.Append({transform, normal});
}

void ModelRenderer::Render(Vulkan::CommandBuffer& buffer,
                           const uint64_t& globalDataSize,
                           const void* globalData,
                           uint32_t currentFrame)
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
            currentMaterial->Bind(buffer);
        }

        if (currentModel != model)
        {
            currentModel = model;
            currentModel->BindIndexed(buffer, currentFrame);
        }

        model->DrawIndexed(buffer, currentFrame, i);
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
    if (currentMaterial) currentMaterial->Recreate();
}
} // namespace Siege
