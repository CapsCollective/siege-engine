//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"
#include "../material/Material.h"
#include "../model/Model.h"

namespace Siege
{
class DebugRenderer3D
{
public:

    DebugRenderer3D();
    ~DebugRenderer3D();

    void Initialise(const String& globalDataAttributeName, const u64& globalDataSize);
    void Destroy();

    // Wire primitives
    void DrawLine(const Vec3& origin, const Vec3& destination, const Vec3& colour);

    void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct LineVertex
    {
        Vec3 position;
        Vec3 colour;
    };

    void RenderLines(VkCommandBuffer& commandBuffer,
                     const u64& globalDataSize,
                     const void* globalData);

    Vec3 lineColor {1.f, 1.f, 1.f};

    Material lineMaterial {};
    Model lineModel {};

    Material rectMaterial {};
    Model rectModel {};

    Utils::StringId globalDataId {};

    Utils::StackArray<LineVertex, Mesh::MAX_VERTICES> lines;
};
} // namespace Siege