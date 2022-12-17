//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BILLBOARD_RENDERER_H
#define SIEGE_ENGINE_BILLBOARD_RENDERER_H

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

    void Initialise(const String& globalDataAttributeName, const uint64_t& globalDataSize);
    void Destroy();

    void DrawBillboard(const Vec3& position, const Vec2& scale, const Vec4& colour);

    void Render(VkCommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct BillboardVertex
    {
        Vec3 position;
        Vec4 colour;
    };

    struct BillboardUBO
    {
        alignas(16) Vec3 position;
        alignas(16) Vec3 scale;
    };

    Material billboardMaterial;
    Model billboardModel;

    Hash::StringId globalDataId;
    Hash::StringId positionsId;

    StackArray<BillboardVertex, Mesh::MAX_VERTICES> vertices;
    StackArray<uint32_t, Mesh::MAX_INDICES> indices;
    StackArray<BillboardUBO, 1000> positions;
};
} // namespace Siege

#endif
