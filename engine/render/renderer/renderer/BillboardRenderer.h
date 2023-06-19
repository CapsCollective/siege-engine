//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BILLBOARD_RENDERER_H
#define SIEGE_ENGINE_BILLBOARD_RENDERER_H

#include <utils/Colour.h>

#include "../model/Model.h"

namespace Siege
{
class BillboardRenderer
{
public:

    BillboardRenderer();
    ~BillboardRenderer();

    void Initialise(const String& globalDataAttributeName);
    void Destroy();

    void DrawBillboard(const Vec3& position, const Vec2& scale, const IColour& colour);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t currentFrame);

    void Flush();

    void RecreateMaterials();

private:

    struct BillboardVertex
    {
        Vec3 position;
        FColour colour;
    };

    struct BillboardUBO
    {
        alignas(16) Vec3 position;
        alignas(16) Vec3 scale;
    };

    Vulkan::Material billboardMaterial;
    Model billboardModel;

    Hash::StringId globalDataId;
    Hash::StringId positionsId;

    MSArray<BillboardVertex, Vulkan::Mesh::MAX_VERTICES> vertices;
    MSArray<uint32_t, Vulkan::Mesh::MAX_INDICES> indices;
    MSArray<BillboardUBO, 1000> positions;
};
} // namespace Siege

#endif
