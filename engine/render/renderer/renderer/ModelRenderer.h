//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MODEL_RENDERER_H
#define SIEGE_ENGINE_MODEL_RENDERER_H

#include "../Core.h"
#include "../model/Model.h"
#include "../utils/Math.h"

namespace Siege
{
class ModelRenderer
{
public:

    ModelRenderer();
    ~ModelRenderer();

    void Initialise(const String& globalDataAttributeName, const uint64_t& globalDataSize);
    void Destroy();

    void DrawModel(Model* model, const Vec3& position, const Vec3& scale, const Vec3& rotation);

    void Render(VkCommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    // TODO(Aryeh): Make this configurable via macros
    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

    Hash::StringId globalDataId;
    Hash::StringId transformId;

    StackArray<Model::Transform, MAX_OBJECT_TRANSFORMS> transforms;
    StackArray<Model*, MAX_OBJECT_TRANSFORMS> models;

    Material* currentMaterial {nullptr};
    Model* currentModel {nullptr};
};
} // namespace Siege

#endif
