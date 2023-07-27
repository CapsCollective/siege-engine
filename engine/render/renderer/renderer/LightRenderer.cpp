//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "LightRenderer.h"

#include <cstdint>

#include "render/renderer/platform/vulkan/Swapchain.h"
#include "render/renderer/platform/vulkan/utils/Draw.h"

namespace Siege
{
void LightRenderer::Initialise(const String& globalDataAttributeName)
{
    using Vulkan::StaticMesh;

    globalDataId = INTERN_STR(globalDataAttributeName);

    lightMaterial = Vulkan::Material(
        Vulkan::Shader::Builder()
            .FromVertexShader("assets/shaders/PointLightGizmoInstanced.vert.spv")
            .WithVertexBinding(Vulkan::Shader::VertexBinding()
                                   .WithInputRate(Vulkan::Utils::INPUT_RATE_INSTANCE)
                                   .AddFloatVec3Attribute()
                                   .AddFloatVec4Attribute())
            .WithGlobalData3DUniform()
            .Build(),
        Vulkan::Shader::Builder()
            .FromFragmentShader("assets/shaders/PointLightGizmoInstanced.frag.spv")
            .Build());

    perFrameVertexBuffers = MHArray<Vulkan::VertexBuffer>(Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT);
    lights = MHArray<LightVertex>(MAX_LIGHTS);

    for (size_t i = 0; i < Vulkan::Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
    {
        perFrameVertexBuffers[i] = Vulkan::VertexBuffer(sizeof(Vec2) * MAX_LIGHTS);
    }

    uint32_t quadIndices[] = {0, 1, 3, 1, 2, 3};

    indexBuffer = Vulkan::IndexBuffer(quadIndices, sizeof(unsigned int) * 6);
}

void LightRenderer::Free()
{
    lightMaterial.Free();
    for (auto it = perFrameVertexBuffers.CreateFIterator(); it; ++it) it->Free();
    indexBuffer.Free();
}

void LightRenderer::DrawPointLight(const Vec3& position, const IColour& colour)
{
    lights.Append({position, ToFColour(colour)});
}

void LightRenderer::Render(Vulkan::CommandBuffer& buffer,
                           const uint64_t& globalDataSize,
                           const void* globalData,
                           uint32_t currentFrame)
{
    if (lights.Count() == 0) return;

    lightMaterial.SetUniformData(globalDataId, globalDataSize, globalData);
    lightMaterial.Bind(buffer);

    auto& vBuffer = perFrameVertexBuffers[currentFrame];

    vBuffer.Copy(lights.Data(), sizeof(LightVertex) * lights.Count());

    uint64_t bindOffset = 0;

    vBuffer.Bind(buffer, &bindOffset);
    indexBuffer.Bind(buffer);

    Vulkan::Utils::DrawIndexed(buffer.Get(), 6, lights.Count(), 0, 0, 0);
}

void LightRenderer::Flush()
{
    lights.Clear();
}

void LightRenderer::RecreateMaterials()
{
    lightMaterial.Recreate();
}
} // namespace Siege
