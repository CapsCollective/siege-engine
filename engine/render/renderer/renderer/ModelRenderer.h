//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MODEL_RENDERER_H
#define SIEGE_ENGINE_MODEL_RENDERER_H

#include "../model/Model.h"
#include "render/renderer/platform/vulkan/Material.h"

namespace Siege
{
class ModelRenderer
{
public:

    ModelRenderer();
    ~ModelRenderer();

    void Initialise(const String& globalDataAttributeName);

    void DrawModel(Model* model, const Vec3& position, const Vec3& scale, const Vec3& rotation);

    void Render(Vulkan::CommandBuffer& buffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t currentFrame);

    void Flush();

    void RecreateMaterials();

private:

    // TODO(Aryeh): Make this configurable via macros
    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

    Hash::StringId globalDataId;
    Hash::StringId transformId;

    MSArray<Model::Transform, MAX_OBJECT_TRANSFORMS> transforms;
    MSArray<Model*, MAX_OBJECT_TRANSFORMS> models;

    Vulkan::Material* currentMaterial {nullptr};
    Model* currentModel {nullptr};
};
} // namespace Siege

#endif
