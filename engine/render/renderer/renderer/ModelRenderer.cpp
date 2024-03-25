//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "ModelRenderer.h"

#include <resources/StaticMeshData.h>
#include <utils/math/Transform.h>

#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
ModelRenderer::ModelRenderer() {}
ModelRenderer::~ModelRenderer() {}

void ModelRenderer::Initialise(const String& globalDataAttributeName)
{
    globalDataId = INTERN_STR(globalDataAttributeName);
    transformId = INTERN_STR("transforms");
}

void ModelRenderer::DrawMesh(Vulkan::StaticMesh* mesh,
                             const Vec3& position,
                             const Vec3& scale,
                             const Vec3& rotation)
{
    staticMeshes.Append(mesh);
    transforms.Append({Transform3D(position, rotation, scale), Normal(rotation, scale)});
}

void ModelRenderer::Render(Vulkan::CommandBuffer& buffer,
                           const uint64_t& globalDataSize,
                           const void* globalData)
{
    Vulkan::Material* mat = nullptr;

    for (size_t i = 0; i < staticMeshes.Count(); i++)
    {
        auto& mesh = staticMeshes[i];
        auto& subMeshes = mesh->GetSubMeshes();

        for (size_t j = 0; j < subMeshes.Count(); j++)
        {
            auto subMesh = subMeshes[j];
            auto subMeshMaterial = mesh->GetMaterial(subMesh.materialIndex);
            if (mat != subMeshMaterial)
            {
                mat = subMeshMaterial;
                mat->SetUniformData(transformId,
                                    sizeof(ModelTransform) * transforms.Count(),
                                    transforms.Data());
                mat->SetUniformData(globalDataId, globalDataSize, globalData);
                mat->Bind(buffer);
            }

            mesh->BindIndexed(buffer,
                              sizeof(BaseVertex) * subMesh.baseVertex,
                              sizeof(unsigned int) * subMesh.baseIndex);
            Vulkan::Utils::DrawIndexed(buffer.Get(), subMesh.indexCount, 1, 0, 0, i);
        }
    }
}

void ModelRenderer::Flush()
{
    transforms.Clear();
    staticMeshes.Clear();
}

void ModelRenderer::RecreateMaterials() {}
} // namespace Siege
