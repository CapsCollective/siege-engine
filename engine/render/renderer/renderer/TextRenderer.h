//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_TEXTRENDERER_H
#define SIEGE_ENGINE_TEXTRENDERER_H

#include <utils/math/vec/Vec3.h>
#include <utils/math/vec/Vec4.h>

#include "render/renderer/model/Model.h"
#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/Font.h"

namespace Siege
{

class TextRenderer
{
public:

    void Initialise(const char* defaultTextPath,
                    const String& globalDataAttributeName,
                    const uint64_t& globalDataSize);
    void DestroyTextRenderer();

    void DrawFont(const char* text, const Vec3& position, const Vec2 scale, const IColour& colour,
                         Vulkan::Font* font = nullptr);

    void RecreateMaterials();

    void Render(Vulkan::CommandBuffer& buffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t frameIndex);

    void Update();
    void Flush();
private:

    static constexpr size_t MAX_FONTS = 16;
    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;
    static constexpr size_t MAX_VERTICES_PER_FONT = 4000;
    static constexpr size_t MAX_INDICES_PER_FONT = 6000;

    struct FontPushConstant
    {
        uint32_t textureIndex;
    };

    struct FontVertex
    {
        Vec3 position;
        FColour colour;
        Vec2 uv;
    };

    float GetTotalTextWidth(const char* text, size_t textLength, SArray<Vulkan::Font::Glyph, 256>& glyphs, Vec2 scale);

    Hash::StringId globalDataId;

    MSArray<FontPushConstant, MAX_OBJECT_TRANSFORMS> pushConstants;

    MSArray<MSArray<FontVertex, MAX_VERTICES_PER_FONT>, MAX_FONTS> perFontVertices;
    MSArray<MSArray<uint32_t, MAX_INDICES_PER_FONT>, MAX_FONTS> perFontIndices;

    MSArray<FontVertex, Vulkan::Mesh::MAX_VERTICES> vertices;
    MSArray<uint32_t, Vulkan::Mesh::MAX_INDICES> indices;

    Vulkan::Font defaultFont;
    Vulkan::Material defaultMaterial;
    Model model;
};

} // namespace Siege

#endif // SIEGE_ENGINE_TEXTRENDERER_H
