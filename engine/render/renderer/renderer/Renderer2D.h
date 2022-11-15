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
#include "../material/Material.h"
#include "../model/Model.h"

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

    static void DrawModel(Model* model,
                          const Vec2& position,
                          const Vec2& scale,
                          float rotation,
                          float zIndex);
    static void DrawModel(Model* model, const Vec2& position, const Vec2& scale, float zIndex);
    static void DrawModel(Model* model, const Vec2& position);

    static void RecreateMaterials();

    static void Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
    static void Flush();

private:

    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

    static Utils::MSArray<Model::Transform2D, MAX_OBJECT_TRANSFORMS> transforms;
    static Utils::MSArray<Model*, MAX_OBJECT_TRANSFORMS> models;

    static uint64_t transformSize;

    static Hash::StringId transformId;
    static Hash::StringId globalDataId;

    static Material* currentMaterial;
    static Vulkan::Material* currentMaterial2;
    static Model* currentModel;
};
} // namespace Siege

#endif
