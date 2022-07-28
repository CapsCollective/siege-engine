//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RENDERER3D_H
#define SIEGE_ENGINE_RENDERER3D_H

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
                          const glm::vec3& position,
                          const glm::vec3& scale,
                          const glm::vec3& rotation);
    static void DrawModel(Model* model, const glm::vec3& position, const glm::vec3& scale);
    static void DrawModel(Model* model, const glm::vec3& position);

    static void DrawBillboard(const glm::vec3& position,
                              const glm::vec2& scale,
                              const glm::vec4& colour);

    // Debug rendering
    // TODO(Aryeh): Move this to it's own renderer module?
    static void DrawLine(const glm::vec3& origin,
                         const glm::vec3& destination,
                         const glm::vec3& colour);
    static void DrawRect(const glm::vec3& position, const glm::vec2& scale, glm::vec3 color);
    static void DrawPointLight(const glm::vec3& position,
                               const float& radius,
                               const glm::vec4& colour,
                               const glm::vec4& ambientColor);

    static void RecreateMaterials();

    static void Render(VkCommandBuffer& commandBuffer, const CameraData& globalData);
    static void Flush();

    static void DestroyRenderer3D();

private:

    // static void RenderRects(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
    static void RenderGrid(VkCommandBuffer& commandBuffer, const GlobalData& globalData);

    static Utils::StringId globalDataId;

    // FIXME(Aryeh): Everything below this needs to change.

    static ModelRenderer modelRenderer;
    static DebugRenderer3D debugRenderer;
    static BillboardRenderer billboardRenderer;
    static LightRenderer lightRenderer;

    static Material gridMaterial;

    static GlobalData global3DData;
};
} // namespace Siege

#endif