//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER2D_H
#define SIEGE_ENGINE_RENDERER2D_H

#include "../Core.h"
#include "../model/Model.h"
#include "render/renderer/platform/vulkan/Texture2D.h"

namespace Siege
{
class Renderer2D
{
public:

    struct GlobalData
    {
        CameraData cameraData;
    };

    static void Initialise();
    static void DestroyRenderer2D();

    static void DrawQuad(const Vec2& position,
                         const Vec2& scale,
                         const IColour& colour,
                         const float& rotation,
                         const float& zIndex,
                         Vulkan::Texture2D* texture = nullptr);

    static void RecreateMaterials();

    static void Render(Vulkan::CommandBuffer& commandBuffer, const GlobalData& globalData);
    static void Update();
    static void Flush();

private:

    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;
    static constexpr size_t VERTICES_PER_QUAD = 4;
    static constexpr size_t INDICES_PER_QUAD = 6;
    static constexpr size_t MAX_TEXTURES = 16;

    struct QuadVertex
    {
        Vec2 position;
        FColour colour;
        Vec2 uv;
        uint32_t texIdx;
    };

    static Hash::StringId transformId;
    static Hash::StringId globalDataId;

    static MSArray<QuadVertex, MAX_OBJECT_TRANSFORMS * VERTICES_PER_QUAD> vertices;
    static MSArray<uint32_t, MAX_OBJECT_TRANSFORMS * INDICES_PER_QUAD> indices;
    static MSArray<Model::Transform2D, MAX_OBJECT_TRANSFORMS> transforms;

    static Vulkan::Texture2D defaultTexture;
    static Vulkan::Material defaultMaterial;
    static Model quadModel;
};
} // namespace Siege

#endif
