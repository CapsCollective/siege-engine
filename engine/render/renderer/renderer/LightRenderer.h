//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_LIGHT_RENDERER_H
#define SIEGE_ENGINE_LIGHT_RENDERER_H

#include "../Core.h"
#include "../model/Model.h"

namespace Siege
{
class LightRenderer
{
public:

    LightRenderer();
    ~LightRenderer();

    void Initialise(const String& globalDataAttributeName, const uint64_t& globalDataSize);

    void Destroy();

    void DrawPointLight(const Vec3& position,
                        float radius,
                        const Vec4& colour,
                        const Vec4& ambientColor);

    void Render(VkCommandBuffer& commandBuffer,
                const uint64_t& globalDataSize,
                const void* globalData);

    void Flush();

    void RecreateMaterials();

private:

    struct PointLightVertex
    {
        alignas(16) Vec2 position;
    };

    struct LightUBO
    {
        Vec4 lightColor {1.f, 1.f, 1.f, 0.2f};
        alignas(16) Vec4 ambientLightColor {1.f, 1.f, 1.f, .02f};
        alignas(16) Vec3 position = {Vec3::Zero};
        alignas(16) float radius = .05f;
    };

    Model lightModel;
    Material lightMaterial;

    Hash::StringId globalDataId;
    Hash::StringId lightDataId;

    Utils::MSArray<Vec2, Mesh::MAX_VERTICES> pointLightVertices;
    Utils::MSArray<uint32_t, Mesh::MAX_INDICES> pointLightIndices;
};
} // namespace Siege

#endif
