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

    static constexpr size_t MAX_CHARS_PER_TEXT = 256;
    static constexpr size_t MAX_TEXT_OBJS_PER_FONT = 100;

    struct PerInstanceData
    {
        Mat4 transform;
        Vec4 uvData;
        FColour colour;
        Vec4 position;
    };

    struct FontPushConstant
    {
        uint32_t textureIndex;
    };

    float GetTotalTextWidth(const char* text, size_t textLength, SArray<Vulkan::Font::Glyph, 256>& glyphs, Vec2 scale);

    Hash::StringId globalDataId;

    MSArray<MSArray<PerInstanceData, MAX_TEXT_OBJS_PER_FONT * MAX_CHARS_PER_TEXT>, MAX_FONTS> instanceData;

    Vulkan::VertexBuffer vertexBuffer;
    Vulkan::IndexBuffer indexBuffer;

    Vulkan::Font defaultFont;
    Vulkan::Material defaultMaterial;
};

} // namespace Siege

#endif // SIEGE_ENGINE_TEXTRENDERER_H
