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
class BillboardRenderer
{
public:

    BillboardRenderer();
    ~BillboardRenderer();

    void Initialise(const char* globalDataAttributeName, const u64& globalDataSize);
    void Destroy();

    void DrawBillboard(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& colour);

    void Render(VkCommandBuffer& commandBuffer, const u64& globalDataSize, const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct BillboardVertex
    {
        glm::vec3 position;
        glm::vec4 colour;
    };

    struct BillboardUBO
    {
        alignas(16) glm::vec3 position;
        alignas(16) glm::vec3 scale;
    };

    Material billboardMaterial;
    Model billboardModel;

    Mat::StringId globalDataId;
    Mat::StringId positionsId;

    Mat::StackArray<BillboardVertex, Mesh::MAX_VERTICES> vertices;
    Mat::StackArray<u32, Mesh::MAX_INDICES> indices;
    Mat::StackArray<BillboardUBO, 1000> positions;
};
} // namespace Siege