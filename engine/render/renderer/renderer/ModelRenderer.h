//
// Copyright (c) 2022 Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MODEL_RENDERER_H
#define SIEGE_ENGINE_MODEL_RENDERER_H

#include <utils/math/mat/Mat4.h>

#include "render/renderer/platform/vulkan/Material.h"
#include "render/renderer/platform/vulkan/StaticMesh.h"

namespace Siege
{
struct ModelTransform
{
    Mat4 transform;
    Mat4 normal;
};

class ModelRenderer
{
public:

    ModelRenderer();
    ~ModelRenderer();

    void Initialise(const String& globalDataAttributeName);

    void DrawMesh(Vulkan::StaticMesh* mesh,
                  const Vec3& position,
                  const Vec3& scale,
                  const Vec3& rotation);

    void Render(Vulkan::CommandBuffer& buffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct Batch
    {
        Vulkan::StaticMesh* mesh;
        unsigned int instances;
    };

    // TODO(Aryeh): Make this configurable via macros
    static constexpr size_t MAX_OBJECT_TRANSFORMS = 1000;

    Hash::StringId globalDataId;
    Hash::StringId transformId;

    MSArray<ModelTransform, MAX_OBJECT_TRANSFORMS> transforms;
    MSArray<Vulkan::StaticMesh*, MAX_OBJECT_TRANSFORMS> staticMeshes;
};
} // namespace Siege

#endif
