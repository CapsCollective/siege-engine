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
    void DrawLine(const glm::vec3& origin, const glm::vec3& destination, const glm::vec3& colour);
    void DrawCube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct LineVertex
    {
        glm::vec3 position;
        glm::vec3 colour;
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

    Utils::StringId globalDataId;

    Utils::StackArray<LineVertex, Mesh::MAX_VERTICES> lines;
    Utils::StackArray<glm::vec3, Mesh::MAX_VERTICES> rects;
};
} // namespace Siege

#endif