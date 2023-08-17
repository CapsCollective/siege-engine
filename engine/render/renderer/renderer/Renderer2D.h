//
// Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER2D_H
#define SIEGE_ENGINE_RENDERER2D_H

#include <utils/Colour.h>
#include <utils/math/mat/Mat4.h>
#include <utils/math/vec/Vec2.h>

#include "render/renderer/platform/vulkan/Font.h"
#include "render/renderer/platform/vulkan/IndexBuffer.h"
#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/VertexBuffer.h"

namespace Siege
{
class Renderer2D
{
public:

    Renderer2D() = default;
    void Initialise(const char* const globalDataName);
    void DrawQuad(const Vec2 position,
                  const Vec2 scale = Vec2::One(),
                  const IColour colour = IColour::White,
                  float rotation = 0.f,
                  const uint8_t zIndex = 0,
                  Vulkan::Texture2D* texture = nullptr);
    void DrawText2D(const char* const text,
                    const Vec2 position,
                    const Vec2 scale,
                    Vulkan::Font& font,
                    IColour colour = IColour::White,
                    float rotation = 0.f,
                    uint8_t zIndex = 0);
    void DrawGrid2D(float spacing,
                    const Vec3& lineColouring,
                    float scale,
                    float lineWidth,
                    float fadeFactor,
                    float cellMultiple);
    void Render(Vulkan::CommandBuffer& buffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t frameIndex);
    void RenderText(Vulkan::CommandBuffer& buffer, size_t index);
    void RenderQuads(Vulkan::CommandBuffer& buffer, size_t index);
    void RenderGrid(Vulkan::CommandBuffer& buffer);
    void Update();
    void Flush();
    void RecreateMaterials();

private:

    struct QuadVertex
    {
        Mat4 transform {};
        FColour colour {};
        Vec4 uvData {};
    };

    struct FontVertex
    {
        Mat4 transform {};
        FColour colour {};
        Vec4 uvData {};
        Vec4 position {};
    };

    struct GridData
    {
        FColour cellColouring {}; // The Colour of the grids cells with the alpha specifying the
                                  // fade factor for sub-grids
        Vec4 cellDimensions {}; // The cell dimensions. X = cell area, Y = number of cells between
                                // primary cells, Z = DPI scale, W = cell line width
    };

    static constexpr size_t MAX_LAYERS = 5;
    static constexpr size_t MAX_TEXTURES = 16;
    static constexpr size_t MAX_QUADS_PER_LAYER = 1000;

    static constexpr size_t QUAD_VERTEX_BUFFER_SIZE =
        MAX_LAYERS * MAX_TEXTURES * MAX_QUADS_PER_LAYER;

    static constexpr size_t MAX_CHARS_PER_TEXT = 256;
    static constexpr size_t MAX_TEXTS_PER_FONT = 100;

    static constexpr size_t TEXT_VERTEX_BUFFER_SIZE =
        MAX_TEXTURES * MAX_TEXTS_PER_FONT * MAX_CHARS_PER_TEXT;

    // NOTE(Aryeh): This function isn't being used right now. It was useful for centering text on a
    // coordinate and can be re-used when we add text formatting. As such, I don't want to remove it
    // just yet.
    float GetTotalTextWidth(const char* text,
                            size_t textLength,
                            SArray<Vulkan::Font::Glyph, 256>& glyphs);

    Hash::StringId globalDataId;
    Hash::StringId textureId;

    Vulkan::Texture2D defaultTexture;
    Vulkan::IndexBuffer quadIndexBuffer;

    // 2D Quads (used for sprites)

    Vulkan::Material quadMaterial;
    Vulkan::VertexBuffer quadVertexBuffer;

    Vulkan::VertexBuffer quadVBuffer;

    SArray<MSArray<MHArray<QuadVertex>, MAX_TEXTURES>, MAX_LAYERS> quads;

    // 2D Text

    Vulkan::Material textMaterial;
    Vulkan::VertexBuffer textVertexBuffer;

    SArray<MSArray<MHArray<FontVertex>, MAX_TEXTURES>, MAX_LAYERS> characters;

    // 2D grid

    Vulkan::Material gridMaterial;
    MSArray<GridData, 1> grid;
};
} // namespace Siege

#endif // SIEGE_ENGINE_RENDERER2D_H
