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
    void DrawLine(const Siege::Vec3& origin,
                  const Siege::Vec3& destination,
                  const Siege::Vec4& colour);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct LineVertex
    {
        Vec3 position;
        Vec4 colour;
    };

    void RenderLines(Vulkan::CommandBuffer& commandBuffer,
                     const uint64_t& globalDataSize,
                     const void* globalData);

    Vec3 lineColor;

    Vulkan::Material lineMaterial;
    Model lineModel;

    Hash::StringId globalDataId;

    Utils::MSArray<LineVertex, Mesh::MAX_VERTICES> lines;
    Utils::MSArray<Vec3, Mesh::MAX_VERTICES> rects;
};
} // namespace Siege

#endif
