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
#include "../material/Material.h"
#include "../model/Model.h"
#include "../utils/Math.h"

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
                          const Siege::Vec2& position,
                          const Siege::Vec2& scale,
                          const float& rotation,
                          const float& zIndex);
    static void DrawModel(Model* model,
                          const Siege::Vec2& position,
                          const Siege::Vec2& scale,
                          const float& zIndex);
    static void DrawModel(Model* model, const Siege::Vec2& position);

    static void RecreateMaterials();

    static void Render(VkCommandBuffer& commandBuffer, const GlobalData& globalData);
    static void Flush();

private:

    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

    static StackArray<Model::Transform2D, MAX_OBJECT_TRANSFORMS> transforms;
    static StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

    static u64 transformSize;

    static StringId transformId;
    static StringId globalDataId;

    static Material* currentMaterial;
    static Model* currentModel;
};
} // namespace Siege