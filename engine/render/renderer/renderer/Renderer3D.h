//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER3D_H
#define SIEGE_ENGINE_RENDERER3D_H

#include "../camera/Camera.h"
#include "../lights/PointLight.h"
#include "BillboardRenderer.h"
#include "DebugRenderer3D.h"
#include "LightRenderer.h"
#include "ModelRenderer.h"
#include "QuadRenderer3D.h"
#include "TextRenderer.h"

// TODO(Aryeh): Convert this class into a normal class and remove the statics. Their existence is a
// TODO(Aryeh): blight on humanity and ends up really messing with Vulkan's memory management

namespace Siege
{
class Renderer3D
{
public:

    struct GlobalData
    {
        Camera cameraData;
        PointLight::Data lightData;
    };

    static void Initialise();

    static void DrawMesh(Vulkan::StaticMesh* mesh,
                         const Vec3& position,
                         const Vec3& scale,
                         const Vec3& rotation);

    static void DrawBillboard(const Vec3& position,
                              const Vec2& scale,
                              const IColour& colour,
                              Vulkan::Texture2D* texture = nullptr);

    // Debug rendering
    // TODO(Aryeh): Move this to its own renderer module?
    static void DrawLine(const Vec3& origin, const Vec3& destination, const IColour& colour);

    static void DrawPointLight(const Vec3& position,
                               float radius,
                               const IColour& colour,
                               const IColour& ambientColor);

    static void DrawText3D(const char* text,
                           const Vec3 position,
                           const Vec3 rotation,
                           const Vec2 scale,
                           const IColour& colour,
                           Vulkan::Font* font);

    static void DrawQuad(const Vec3 position,
                         const Vec2 scale,
                         const Vec3 rotation,
                         IColour colour,
                         Vulkan::Texture2D* texture = nullptr);

    static void SetGridEnabled(bool enabled);

    static void RecreateMaterials();

    static void Render(uint32_t currentFrame,
                       Vulkan::CommandBuffer& commandBuffer,
                       const Camera& cameraData);
    static void Flush();

    static void DestroyRenderer3D();

    static void Update();

private:

    static Hash::StringId globalDataId;

    // FIXME(Aryeh): These statics are evil and need to be removed.

    static ModelRenderer modelRenderer;
    static DebugRenderer3D debugRenderer;
    static BillboardRenderer billboardRenderer;
    static LightRenderer lightRenderer;
    static TextRenderer textRenderer;
    static QuadRenderer3D quadRenderer3D;

    static GlobalData global3DData;
};
} // namespace Siege

#endif
