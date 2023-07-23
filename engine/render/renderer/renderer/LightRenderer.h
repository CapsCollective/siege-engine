//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_LIGHT_RENDERER_H
#define SIEGE_ENGINE_LIGHT_RENDERER_H

#include <utils/Colour.h>
#include <utils/String.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>

#include "render/renderer/platform/vulkan/IndexBuffer.h"
#include "render/renderer/platform/vulkan/StaticMesh.h"
#include "render/renderer/platform/vulkan/VertexBuffer.h"

namespace Siege
{
class LightRenderer
{
public:

    void Initialise(const String& globalDataAttributeName);

    void Free();

    void DrawPointLight(const Vec3& position, const IColour& colour);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t currentFrame);

    void Flush();

    void RecreateMaterials();

private:

    struct LightVertex
    {
        Vec3 position;
        FColour colour;
    };

    static inline constexpr size_t MAX_LIGHTS = 4;

    Vulkan::Material lightMaterial;

    Hash::StringId globalDataId;

    MHArray<Vulkan::VertexBuffer> perFrameVertexBuffers;

    MHArray<LightVertex> lights;

    Vulkan::IndexBuffer indexBuffer;
};
} // namespace Siege

#endif
