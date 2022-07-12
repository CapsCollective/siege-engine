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

    void Initialise(const char* globalDataAttributeName, const u64& globalDataSize);
    void Destroy();

    // Wire primitives
    void DrawLine(const Siege::Vec3& origin,
                  const Siege::Vec3& destination,
                  const Siege::Vec4& colour);
    void DrawCube(const Siege::Vec3& position,
                  const Siege::Vec3& rotation,
                  const Siege::Vec3& scale);

    void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct LineVertex
    {
        Siege::Vec3 position;
        Siege::Vec4 colour;
    };

    void RenderLines(VkCommandBuffer& commandBuffer,
                     const u64& globalDataSize,
                     const void* globalData);
    void RenderRects();

    glm::vec3 lineColor;

    Material lineMaterial;
    Model lineModel;

    Material rectMaterial;
    Model rectModel;

    StringId globalDataId;

    StackArray<LineVertex, Mesh::MAX_VERTICES> lines;
    StackArray<glm::vec3, Mesh::MAX_VERTICES> rects;
};
} // namespace Siege