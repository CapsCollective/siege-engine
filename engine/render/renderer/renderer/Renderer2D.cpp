//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "Renderer2D.h"

#include <utils/math/Graphics.h>

namespace Siege
{
Hash::StringId Renderer2D::transformId;
Hash::StringId Renderer2D::globalDataId;

Utils::MSArray<Model::Transform2D, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::transforms;
Utils::MSArray<uint32_t, Renderer2D::MAX_OBJECT_TRANSFORMS * Renderer2D::INDICES_PER_QUAD>
    Renderer2D::indices;
Utils::MSArray<Renderer2D::QuadVertex,
               Renderer2D::MAX_OBJECT_TRANSFORMS * Renderer2D::VERTICES_PER_QUAD>
    Renderer2D::vertices;
Utils::MSArray<Vulkan::Texture2D*, Renderer2D::MAX_TEXTURES> Renderer2D::textures;

Model Renderer2D::quadModel;
Vulkan::Material Renderer2D::defaultMaterial;
Vulkan::Texture2D Renderer2D::defaultTexture;

void Renderer2D::Initialise()
{
    transformId = INTERN_STR("transforms");
    globalDataId = INTERN_STR("globalData");

    using Vulkan::Material;
    using Vulkan::Shader;

    defaultTexture = Siege::Vulkan::Texture2D("default");

    defaultMaterial = Material(Shader::Builder()
                                   .FromVertexShader("assets/shaders/texturedQuad.vert.spv")
                                   .WithVertexBinding(Shader::VertexBinding()
                                                          .AddFloatVec2Attribute()
                                                          .AddFloatVec4Attribute()
                                                          .AddFloatVec2Attribute()
                                                          .AddU32Attribute())
                                   .WithTransform2DStorage(0, 1000)
                                   .WithGlobalData2DUniform()
                                   .Build(),
                               Shader::Builder()
                                   .FromFragmentShader("assets/shaders/texturedQuad.frag.spv")
                                   .WithTexture("texture", 0, 16)
                                   .WithDefaultTexture(&defaultTexture)
                                   .Build());

    quadModel.SetMesh({sizeof(QuadVertex), nullptr, 0, nullptr, 0});

    quadModel.SetMaterial(&defaultMaterial);
}

void Renderer2D::DrawQuad(const Siege::Vec2& position,
                          const Siege::Vec2& scale,
                          const IColour& colour,
                          const float& rotation,
                          const float& zIndex,
                          Vulkan::Texture2D* texture)
{
    auto targetTexture = texture == nullptr ? &defaultTexture : texture;

    uint32_t texIdx = 0;

    // TODO(Aryeh): Find a better way to search over all our textures. The texture array is just
    // TODO(Aryeh): taking up space at this point
    //    auto found = textures.Find(
    //        [&](Vulkan::Texture* tex) {
    //            if (targetTexture->GetId() == tex->GetId())
    //            {
    //                texIdx++;
    //                return true;
    //            }
    //            else return false;
    //        },
    //        textures.begin(),
    //        textures.end());

    //    // TODO: Add some form of check to make sure we don't go over the maximum number of
    //    textures
    //    if (found == textures.end())
    //    {
    //        texIdx = textures.Count();
    //        textures.Append(targetTexture);
    //
    //        auto& texInfo = targetTexture->GetInfo();
    //
    //        imageInfos[texIdx] = {texInfo.sampler,
    //                              texInfo.imageInfo.view,
    //                              Vulkan::Utils::ToVkImageLayout(texInfo.imageInfo.layout)};
    //
    //        defaultMaterial.SetUniformTexture(INTERN_STR("texture"),
    //                                          imageInfos.Data(),
    //                                          imageInfos.Count(),
    //                                          0);
    //    }

    vertices.Append({{1.f, 1.f}, ToFColour(colour), {1.f, 1.f}, texIdx});
    vertices.Append({{1.f, -1.f}, ToFColour(colour), {1.f, 0.f}, texIdx});
    vertices.Append({{-1.f, -1.f}, ToFColour(colour), {0.f, 0.f}, texIdx});
    vertices.Append({{-1.f, 1.f}, ToFColour(colour), {0.f, 1.f}, texIdx});

    // Pattern: 0, 1, 3, 1, 2, 3
    indices.Append(vertices.Count() - 4);
    indices.Append(vertices.Count() - 3);
    indices.Append(vertices.Count() - 1);

    indices.Append(vertices.Count() - 3);
    indices.Append(vertices.Count() - 2);
    indices.Append(vertices.Count() - 1);

    auto transform = Graphics::CalculateTransform3D({position.x, position.y, zIndex},
                                                    {0.f, 0.f, rotation},
                                                    {scale.x, scale.y, 0.f});
    transforms.Append({transform});
}

void Renderer2D::RecreateMaterials()
{
    defaultMaterial.Recreate();
}

void Renderer2D::Render(Vulkan::CommandBuffer& buffer, const GlobalData& globalData)
{
    if (vertices.Count() == 0) return;

    defaultMaterial.SetUniformData(transformId,
                                   sizeof(transforms[0]) * transforms.Count(),
                                   transforms.Data());
    defaultMaterial.SetUniformData(globalDataId, sizeof(GlobalData), &globalData);

    defaultMaterial.Bind(buffer);

    quadModel.UpdateMesh({sizeof(QuadVertex),
                          vertices.Data(),
                          static_cast<uint32_t>(vertices.Count()),
                          indices.Data(),
                          static_cast<uint32_t>(indices.Count())});

    quadModel.Bind(buffer);
    quadModel.Draw(buffer);
}

void Renderer2D::DestroyRenderer2D()
{
    quadModel.DestroyModel();
    defaultTexture.~Texture2D();
    defaultMaterial.~Material();
}

void Renderer2D::Flush()
{
    transforms.Clear();
    vertices.Clear();
    indices.Clear();
    textures.Clear();
}

void Renderer2D::Update()
{
    defaultMaterial.Update();
}
} // namespace Siege