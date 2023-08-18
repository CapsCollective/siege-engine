//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_BILLBOARD_RENDERER_H
#define SIEGE_ENGINE_BILLBOARD_RENDERER_H

#include <utils/Colour.h>
#include <utils/String.h>
#include <utils/math/vec/Vec2.h>
#include <utils/math/vec/Vec3.h>
#include <utils/math/vec/Vec4.h>

#include "render/renderer/platform/vulkan/IndexBuffer.h"
#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/VertexBuffer.h"

namespace Siege
{
class BillboardRenderer
{
public:

    BillboardRenderer();
    ~BillboardRenderer();

    void Initialise(const String& globalDataAttributeName);
    void Free();

    void DrawBillboard(const Vec3& position,
                       const Vec2& scale,
                       const IColour& colour,
                       Vulkan::Texture2D* texture = nullptr);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t currentFrame);

    void Flush();

    void RecreateMaterials();

    void Update();

private:

    struct BillboardVertex
    {
        Vec3 position;
        Vec3 scale;
        FColour colour;
        Vec4 uv;
    };

    static constexpr size_t MAX_TEXTURES = 16;
    static constexpr size_t MAX_QUADS_PER_TEXTURE = 256;
    static constexpr size_t VERTEX_BUFFER_SIZE = MAX_TEXTURES * MAX_QUADS_PER_TEXTURE;

    Vulkan::Material billboardMaterial;

    Hash::StringId globalDataId;
    Hash::StringId textureId;

    MHArray<MHArray<BillboardVertex>> vertices;
    MHArray<Vulkan::VertexBuffer> perFrameVertexBuffers;
    Vulkan::IndexBuffer indexBuffer;
    Vulkan::Texture2D defaultTexture;
};
} // namespace Siege

#endif
