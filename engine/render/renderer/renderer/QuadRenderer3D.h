//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_QUADRENDERER3D_H
#define SIEGE_ENGINE_QUADRENDERER3D_H

#include <utils/Colour.h>
#include <utils/collections/StackArray.h>
#include <utils/math/mat/Mat4.h>

#include "render/renderer/platform/vulkan/IndexBuffer.h"
#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/Texture2D.h"
#include "render/renderer/platform/vulkan/VertexBuffer.h"

namespace Siege
{
class QuadRenderer3D
{
public:

    void Initialise(const String& globalDataAttributeName);
    void Free();

    void DrawQuad(const Vec3& position,
                  const Vec2& scale,
                  const Vec3& rotation,
                  const IColour& colour,
                  Vulkan::Texture2D* texture = nullptr);

    void RecreateMaterials();

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t frameIndex);
    void Update();
    void Flush();

private:

    struct QuadVertex
    {
        Mat4 transform {};
        FColour colour {};
        Vec4 uv {};
    };

    static constexpr size_t MAX_TEXTURES = 16;
    static constexpr size_t MAX_QUADS_PER_TEXTURE = 256;
    static constexpr size_t VERTEX_BUFFER_SIZE = MAX_TEXTURES * MAX_QUADS_PER_TEXTURE;

    Hash::StringId globalDataId;
    Hash::StringId textureId;

    MSArray<MHArray<QuadVertex>, MAX_TEXTURES> quads;

    MHArray<Vulkan::VertexBuffer> perFrameVBuffers;
    Vulkan::IndexBuffer indexBuffer;

    Vulkan::Texture2D defaultTexture;
    Vulkan::Material defaultMaterial;
};
} // namespace Siege

#endif // SIEGE_ENGINE_QUADRENDERER3D_H
