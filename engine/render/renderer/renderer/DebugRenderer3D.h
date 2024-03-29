//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DEBUG_RENDERER3D_H
#define SIEGE_ENGINE_DEBUG_RENDERER3D_H

#include <utils/Colour.h>
#include <utils/String.h>
#include <utils/math/vec/Vec3.h>

#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/VertexBuffer.h"

namespace Siege
{
class DebugRenderer3D
{
public:

    DebugRenderer3D();
    ~DebugRenderer3D();

    void Initialise(const String& globalDataAttributeName);
    void Destroy();

    // Wire primitives
    void DrawLine(const Vec3& origin, const Vec3& destination, const IColour& colour);

    void SetGridEnabled(bool enabled);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t currentFrame);

    void Flush();

    void RecreateMaterials();

private:

    static inline constexpr size_t MAX_LINES = 512;

    struct LineVertex
    {
        Vec3 position;
        FColour colour;
    };

    void RenderLines(Vulkan::CommandBuffer& commandBuffer,
                     const uint64_t& globalDataSize,
                     const void* globalData,
                     uint32_t currentFrame);

    void RenderGrid(Vulkan::CommandBuffer& commandBuffer,
                    const uint64_t& globalDataSize,
                    const void* globalData);

    Vulkan::Material lineMaterial;

    MHArray<Vulkan::VertexBuffer> perFrameVertexBuffers;

    Hash::StringId globalDataId;

    MSArray<LineVertex, MAX_LINES> lines;

    Vulkan::Material gridMaterial;

    bool drawGrid;
};
} // namespace Siege

#endif
