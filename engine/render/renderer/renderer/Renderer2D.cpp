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
Hash::StringId Renderer2D::globalDataId;

MSArray<Renderer2D::QuadPushConstant, Renderer2D::MAX_OBJECT_TRANSFORMS> Renderer2D::pushConstants;

Model Renderer2D::quadModel;
Vulkan::Material Renderer2D::defaultMaterial;
Vulkan::Texture2D Renderer2D::defaultTexture;

void Renderer2D::Initialise()
{
    globalDataId = INTERN_STR("globalData");

    using Vulkan::Material;
    using Vulkan::Mesh;
    using Vulkan::Shader;

    defaultTexture = Siege::Vulkan::Texture2D("default");

    defaultMaterial =
        Material(Shader::Builder()
                     .FromVertexShader("assets/shaders/texturedQuad.vert.spv")
                     .WithVertexBinding(
                         Shader::VertexBinding().AddFloatVec2Attribute().AddFloatVec2Attribute())
                     .WithGlobalData2DUniform()
                     .WithPushConstant(sizeof(QuadPushConstant))
                     .Build(),
                 Shader::Builder()
                     .FromFragmentShader("assets/shaders/texturedQuad.frag.spv")
                     .WithTexture("texture", 0, 16)
                     .WithDefaultTexture(&defaultTexture)
                     .Build());

    QuadVertex quadVertices[] = {{{1.f, 1.f}, {1.f, 1.f}},
                                 {{1.f, -1.f}, {1.f, 0.f}},
                                 {{-1.f, -1.f}, {0.f, 0.f}},
                                 {{-1.f, 1.f}, {0.f, 1.f}}};

    uint32_t quadIndices[] = {0, 1, 3, 1, 2, 3};

    quadModel.SetMesh(Mesh({sizeof(QuadVertex), quadVertices, 4}, {quadIndices, 6}));

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

    auto texIndex = defaultMaterial.SetTexture(INTERN_STR("texture"), targetTexture);

    auto transform = Graphics::CalculateTransform3D({position.x, position.y, zIndex},
                                                    {0.f, 0.f, rotation},
                                                    {scale.x, scale.y, 0.f});
    pushConstants.Append({transform, ToFColour(colour), texIndex});
}

void Renderer2D::RecreateMaterials()
{
    defaultMaterial.Recreate();
}

void Renderer2D::Render(Vulkan::CommandBuffer& buffer,
                        const GlobalData& globalData,
                        uint32_t frameIndex)
{
    if (pushConstants.Count() == 0) return;

    defaultMaterial.SetUniformData(globalDataId, sizeof(GlobalData), &globalData);

    quadModel.BindIndexed(buffer, frameIndex);

    for (uint32_t i = 0; i < pushConstants.Count(); i++)
    {
        defaultMaterial.BindPushConstant(buffer, &pushConstants[i]);

        defaultMaterial.Bind(buffer);

        quadModel.DrawIndexed(buffer, frameIndex, i);
    }
}

void Renderer2D::DestroyRenderer2D()
{
    quadModel.DestroyModel();
    defaultTexture.Free();
    defaultMaterial.Destroy();
}

void Renderer2D::Flush()
{
    pushConstants.Clear();
}

void Renderer2D::Update()
{
    defaultMaterial.Update();
}
} // namespace Siege