//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#pragma once

#include "../Core.h"
#include "../camera/Camera.h"
#include "../lights/PointLight.h"
#include "../material/Material.h"
#include "../model/Model.h"
#include "../utils/Math.h"
#include "BillboardRenderer.h"
#include "DebugRenderer3D.h"
#include "LightRenderer.h"
#include "ModelRenderer.h"

namespace Siege
{
class Renderer3D
{
public:

    struct GlobalData
    {
        CameraData cameraData;
        PointLight::Data lightData;
    };

    static void Initialise();

    static void DrawModel(Model* model,
                          const Siege::Vec3& position,
                          const Siege::Vec3& scale,
                          const Siege::Vec3& rotation);
    static void DrawModel(Model* model, const Siege::Vec3& position, const Siege::Vec3& scale);
    static void DrawModel(Model* model, const Siege::Vec3& position);

    static void DrawBillboard(const Siege::Vec3& position,
                              const Siege::Vec2& scale,
                              const Siege::Vec4& colour);

    // Debug rendering
    // TODO(Aryeh): Move this to it's own renderer module?
    static void DrawLine(const Siege::Vec3& origin,
                         const Siege::Vec3& destination,
                         const Siege::Vec4& colour);

    static void DrawRect(const Siege::Vec3& position, const Siege::Vec2& scale, Siege::Vec4& color);
    static void DrawPointLight(const Siege::Vec3& position,
                               const float& radius,
                               const Siege::Vec4& colour,
                               const Siege::Vec4& ambientColor);

    static void RecreateMaterials();

    static void Render(VkCommandBuffer& commandBuffer, const CameraData& globalData);
    static void Flush();

    static void DestroyRenderer3D();

private:

    // static void RenderRects(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
    static void RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData);

    static Hash::StringId globalDataId;

    // FIXME(Aryeh): Everything below this needs to change.

    static ModelRenderer modelRenderer;
    static DebugRenderer3D debugRenderer;
    static BillboardRenderer billboardRenderer;
    static LightRenderer lightRenderer;

    static Material gridMaterial;

    static GlobalData global3DData;
};
} // namespace Siege