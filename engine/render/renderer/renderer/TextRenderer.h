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
#include "render/renderer/platform/vulkan/Font.h"
#include "render/renderer/platform/vulkan/Material.h"

namespace Siege
{

class TextRenderer
{
public:

    void Initialise(const char* defaultTextPath, const String& globalDataAttributeName);
    void Free();

    void DrawFont(const char* text,
                  const Vec3& position,
                  const Vec3& rotation,
                  const Vec2 scale,
                  const IColour& colour,
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

    static constexpr size_t MAX_CHARS_PER_TEXT = 256;
    static constexpr size_t MAX_TEXT_OBJS_PER_FONT = 100;
    static constexpr size_t OFFSET_PER_FONT = MAX_TEXT_OBJS_PER_FONT * MAX_CHARS_PER_TEXT;

    struct QuadData
    {
        Mat4 transform {};
        Vec4 uvData {};
        FColour colour {};
        Vec4 position {};
    };

    float GetTotalTextWidth(const char* text,
                            size_t textLength,
                            SArray<Vulkan::Font::Glyph, 256>& glyphs);

    Hash::StringId globalDataId;
    Hash::StringId textureId;

    MSArray<MHArray<QuadData>, MAX_FONTS> characters;

    MHArray<Vulkan::VertexBuffer> perFrameVertexBuffers;
    Vulkan::IndexBuffer indexBuffer;

    Vulkan::Font defaultFont;
    Vulkan::Material defaultMaterial;
};

} // namespace Siege

#endif // SIEGE_ENGINE_TEXTRENDERER_H
