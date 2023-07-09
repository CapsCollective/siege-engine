//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_LIGHT_RENDERER_H
#define SIEGE_ENGINE_LIGHT_RENDERER_H

#include <utils/Colour.h>
#include <utils/math/vec/Vec2.h>

#include "../model/Model.h"

namespace Siege
{
class LightRenderer
{
public:

    void Initialise(const String& globalDataAttributeName);

    void Destroy();

    void DrawPointLight(const Vec3& position,
                        float radius,
                        const IColour& colour,
                        const IColour& ambientColor);

    void Render(Vulkan::CommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData,
                uint32_t currentFrame);

    void Flush();

    void RecreateMaterials();

private:

    Model lightModel;
    Vulkan::Material lightMaterial;

    Hash::StringId globalDataId;

    MSArray<Vec2, Vulkan::Mesh::MAX_VERTICES> pointLightVertices;
    MSArray<uint32_t, Vulkan::Mesh::MAX_INDICES> pointLightIndices;
};
} // namespace Siege

#endif
