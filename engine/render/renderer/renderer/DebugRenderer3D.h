//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_DEBUG_RENDERER3D_H
#define SIEGE_ENGINE_DEBUG_RENDERER3D_H

#include "../Core.h"
#include "../model/Model.h"

namespace Siege
{
class DebugRenderer3D
{
public:

    DebugRenderer3D();
    ~DebugRenderer3D();

    void Initialise(const String& globalDataAttributeName, const uint64_t& globalDataSize);
    void Destroy();

    // Wire primitives
    void DrawLine(const Vec3& origin, const Vec3& destination, const IColour& colour);
    void DrawCube(const Vec3& position, const Vec3& rotation, const Vec3& scale);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct LineVertex
    {
        Vec3 position;
        FColour colour;
    };

    void RenderLines(Vulkan::CommandBuffer& commandBuffer,
                     const uint64_t& globalDataSize,
                     const void* globalData);

    Vec3 lineColor;

    Vulkan::Material lineMaterial;
    Model lineModel;

    Hash::StringId globalDataId;

    MSArray<LineVertex, Mesh::MAX_VERTICES> lines;
    MSArray<Vec3, Mesh::MAX_VERTICES> rects;
};
} // namespace Siege

#endif
