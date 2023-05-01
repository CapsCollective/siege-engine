//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER2D_H
#define SIEGE_ENGINE_RENDERER2D_H

#include "render/renderer/platform/vulkan/Material.h"

#include "render/renderer/platform/vulkan/VertexBuffer.h"
#include "render/renderer/platform/vulkan/IndexBuffer.h"

#include <utils/Colour.h>
#include <utils/math/mat/Mat2.h>

namespace Siege
{
class Renderer2D
{
public:
    Renderer2D() = default;
    void Initialise(const char* const globalDataName);
    void DrawQuad(const Vec2 position, const Vec2 scale = Vec2::Zero, const IColour colour = IColour::White, float rotation = 0.f, const uint8_t zIndex = 0, Vulkan::Texture2D* texture = nullptr);
    void Render(Vulkan::CommandBuffer& buffer,
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
        Vec4 uvData {};
    };

    static constexpr size_t MAX_LAYERS = 5;
    static constexpr size_t MAX_TEXTURES = 16;
    static constexpr size_t MAX_QUADS_PER_LAYER = 1000;

    static constexpr size_t VERTEX_BUFFER_SIZE = MAX_LAYERS * MAX_TEXTURES * MAX_QUADS_PER_LAYER;

    Hash::StringId globalDataId;
    Hash::StringId textureId;

    // 2D Quads (used for sprites)

    Vulkan::Texture2D defaultTexture;
    Vulkan::Material quadMaterial;
    Vulkan::VertexBuffer quadVertexBuffer;
    Vulkan::IndexBuffer quadIndexBuffer;

    SArray<MSArray<MHArray<QuadVertex>, MAX_TEXTURES>, MAX_LAYERS> quads;

    // 2D Text


};
} // namespace Siege

#endif // SIEGE_ENGINE_RENDERER2D_H
